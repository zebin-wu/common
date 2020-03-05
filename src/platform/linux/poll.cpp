/**
 * Copyright (c) 2020 KNpTrue
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/
#include <unistd.h>
#include <sys/epoll.h>
#include <cerrno>
#include <map>
#include <common/assert.hpp>
#include <platform/poll.hpp>
#include <platform/lock.hpp>
#include <platform/handle_int.hpp>
#include <platform/error.hpp>

#define PFM_EPOLL_FD_MAX 1024
#define PFM_EPOLL_MAX_LISTEN 64

namespace platform {

class HandleState;

static const ErrorDesc epollCtlCommonErrDescs[] = {
    {EBADF, common::ERR_INVAL_ARG, "the handle is invalid"},
    {EINVAL, common::ERR_INVAL_ARG, "epoll_ctl() has invalid arguments"},
    {ENOMEM, common::ERR_MEM, NULL},
    {EPERM, common::ERR_PERM, "the handle does not support poll"},
};

static const ErrorDesc epollCtlAddErrDescs[] = {
    {EEXIST, common::ERR_EXIST, "the handle is added"},
    {ENOSPC, common::ERR_INVAL_ARG, "counld not watch too many handle"},
};

static const ErrorDesc epollCtlModErrDescs[] = {
    {ENOENT, common::ERR_NOENT, "The handle is not added"},
};

static uint32_t getEpollEvent(Poll::PollMode mode);
static void epollCtlExcept(int epopt, int err, Poll *poll);
static void callPollEvent(HandleState *state, Poll::PollMode mode);

class PollCallback {
 public:
    explicit PollCallback(Poll::cb_t cb, void *arg): cb(cb), arg(arg) {}
    Poll::cb_t cb;
    void *arg;
};

class HandleState {
 public:
    explicit HandleState(Poll::PollMode mode, Handle *handle,
        Poll::cb_t cb, void *arg = nullptr): handle(handle) {
        cbMap.insert({mode, new PollCallback(cb, arg)});
    }

    ~HandleState() {
        std::map<Poll::PollMode, PollCallback *>::iterator it;
        for (it = cbMap.begin(); it != cbMap.end(); it++) {
            delete it->second;
        }
    }

    PollCallback *findPollCallback(Poll::PollMode mode) {
        std::map<Poll::PollMode, PollCallback *>::iterator it;

        it = cbMap.find(mode);
        if (it == cbMap.end()) {
            return nullptr;
        } else {
            return it->second;
        }
    }

    Handle *handle;
    std::map<Poll::PollMode, PollCallback *> cbMap;
};

class PollPriv {
 public:
    PollPriv(): epfd(-1), mutex(Lock(Lock::LOCK_MUTEX)), isPolling(false) {}

    bool isPolling;
    int epfd;
    u32 maxListen;
    Lock mutex;
    struct epoll_event *events;
    std::map<Handle *, HandleState *> stateMap;
};

Poll::Poll(): priv(new PollPriv) {
    priv->epfd = epoll_create(PFM_EPOLL_FD_MAX);
    priv->maxListen = PFM_EPOLL_MAX_LISTEN;
    priv->events = new struct epoll_event[priv->maxListen];
    if (priv->epfd < 0) {
        switch (errno) {
        case EINVAL:
            throw PollException(this, common::ERR_INVAL_ARG,
                "epoll_create(): size is not positive.");
            break;
        case EMFILE:
            throw PollException(this, common::ERR_OVER_RANGE,
                "epoll_create(): Limit on the total number "
                "of open files has been reached");
            break;
        case ENOMEM:
            throw PollException(this, common::ERR_MEM);
            break;
        default:
            break;
        }
    }
}

Poll::~Poll() {
    if (priv->epfd >= 0) {
        close(priv->epfd);
    }
    delete priv->events;
    delete priv;
}

void Poll::add(Handle *handle, PollMode mode, cb_t cb,  void *arg) {
    std::map<Handle *, HandleState *>::iterator stateIt;
    std::map<Poll::PollMode, PollCallback *>::iterator cbIt;
    HandleState *state = nullptr;
    PollCallback *pollCb = nullptr;
    struct epoll_event epevt;
    int epopt;
    int ret;

    ASSERT(handle);
    ASSERT(cb);
    priv->mutex.lock();
    stateIt = priv->stateMap.find(handle);
    if (stateIt != priv->stateMap.end()) {
        epopt = EPOLL_CTL_MOD;
        state = stateIt->second;
        pollCb = state->findPollCallback(mode);
        if (pollCb) {
            throw PollException(this, common::ERR_EXIST,
                "the poll callback of the handle is added");
            goto end;
        }
        for (cbIt = state->cbMap.begin();
            cbIt != state->cbMap.end(); cbIt++) {
            epevt.events |= getEpollEvent(cbIt->first);
        }
    } else {
        epopt = EPOLL_CTL_ADD;
        state = new HandleState(mode, handle, cb, arg);
    }
    epevt.events |= getEpollEvent(mode);
    epevt.data.ptr = static_cast<void *>(state);
    ret = epoll_ctl(priv->epfd, epopt, handle->priv->fd, &epevt);
    if (ret < 0) {
        if (epopt == EPOLL_CTL_ADD) {
            delete state;
        }
        epollCtlExcept(epopt, errno, this);
        goto end;
    }
    if (epopt == EPOLL_CTL_MOD) {
        state->cbMap.insert({mode, new PollCallback(cb, arg)});
    }
    priv->stateMap.insert({handle, state});
end:
    priv->mutex.unlock();
}

void Poll::mod(Handle *handle, PollMode mode, cb_t cb, void *arg) {
    std::map<Handle *, HandleState *>::iterator stateIt;
    HandleState *state = nullptr;
    PollCallback *pollCb = nullptr;

    ASSERT(handle);
    ASSERT(cb);
    priv->mutex.lock();
    stateIt = priv->stateMap.find(handle);
    if (stateIt != priv->stateMap.end()) {
        state = stateIt->second;
        pollCb = state->findPollCallback(mode);
        if (!pollCb) {
            throw PollException(this, common::ERR_NOENT,
                "the poll callback of the handle is not added");
            goto end;
        }
        pollCb->cb = cb;
        pollCb->arg = arg;
    } else {
        throw PollException(this, common::ERR_NOENT,
            " the handle is not added");
    }
end:
    priv->mutex.unlock();
}

void Poll::del(Handle *handle, PollMode mode) {
    std::map<Handle *, HandleState *>::iterator stateIt;
    std::map<Poll::PollMode, PollCallback *>::iterator cbIt;
    HandleState *state = nullptr;
    PollCallback *pollCb = nullptr;
    struct epoll_event epevt;
    int epopt;
    int ret;

    ASSERT(handle);
    priv->mutex.lock();
    stateIt = priv->stateMap.find(handle);
    if (stateIt != priv->stateMap.end()) {
        state = stateIt->second;
        pollCb = state->findPollCallback(mode);
        if (!pollCb) {
            throw PollException(this, common::ERR_NOENT,
                "the poll callback of the handle is not added");
            goto end;
        }
        if (state->cbMap.size() == 1) {
            epopt = EPOLL_CTL_DEL;
        } else {
            epopt = EPOLL_CTL_MOD;
            for (cbIt = state->cbMap.begin();
                cbIt != state->cbMap.end(); cbIt++) {
                epevt.events |= getEpollEvent(cbIt->first);
            }
            epevt.events &= ~getEpollEvent(mode);
        }
    } else {
        throw PollException(this, common::ERR_NOENT,
            " the handle is not added");
    }
    epevt.data.ptr = static_cast<void *>(state);
    ret = epoll_ctl(priv->epfd, epopt, handle->priv->fd,
        epopt == EPOLL_CTL_MOD ? &epevt : nullptr);
    if (ret < 0) {
        epollCtlExcept(epopt, errno, this);
        goto end;
    }
    if (epopt == EPOLL_CTL_MOD) {
        state->cbMap.erase(mode);
    } else {
        priv->stateMap.erase(handle);
    }
end:
    priv->mutex.unlock();
}

void Poll::polling(int timeout) {
    struct epoll_event *epevt;
    HandleState *state;
    PollCallback *pollCb;
    if (priv->isPolling) {
        throw PollException(this, common::ERR_BUSY, "polling");
        return;
    }
    priv->isPolling = true;
    int ret = epoll_wait(priv->epfd, priv->events,
        priv->maxListen, timeout);
    if (ret < 0) {
        switch (errno) {
        case EINTR:
            break;
        default:
            throw PollException(this, common::ERR_ERR);
        }
    }
    for (int i = 0; i < ret; i++) {
        epevt = priv->events + i;
        state = static_cast<HandleState *>(epevt->data.ptr);
        if (epevt->events & EPOLLIN) {
            callPollEvent(state, POLL_READ);
        }
        if (epevt->events & EPOLLOUT) {
            callPollEvent(state, POLL_WRITE);
        }
        if (epevt->events & EPOLLERR) {
            callPollEvent(state, POLL_ERR);
        }
    }
    priv->isPolling = false;
}

static uint32_t getEpollEvent(Poll::PollMode mode) {
    switch (mode) {
    case Poll::POLL_WRITE:
        return EPOLLOUT;
    case Poll::POLL_READ:
        return EPOLLIN;
    case Poll::POLL_ERR:
        return EPOLLERR;
    default:
        return 0;
    }
}

static void epollCtlExcept(int epopt, int err, Poll *poll) {
    const ErrorDesc *desc = nullptr;
    switch (epopt) {
    case EPOLL_CTL_ADD:
        desc = getErrorDesc(err, epollCtlAddErrDescs,
            ARRAY_LEN(epollCtlAddErrDescs));
        break;
    case EPOLL_CTL_MOD:
        desc = getErrorDesc(err, epollCtlModErrDescs,
            ARRAY_LEN(epollCtlModErrDescs));
        break;
    case EPOLL_CTL_DEL:
        break;
    default:
        return;
    }
    if (!desc) {
        desc = getErrorDesc(err, epollCtlCommonErrDescs,
            ARRAY_LEN(epollCtlCommonErrDescs));
    }
    if (desc) {
        throw PollException(poll, desc->err, desc->msg);
    }
}

static void callPollEvent(HandleState *state, Poll::PollMode mode) {
    PollCallback *pollCb = state->findPollCallback(mode);
    if (pollCb) {
        pollCb->cb(mode, state->handle, pollCb->arg);
    }
}

}  // namespace platform
