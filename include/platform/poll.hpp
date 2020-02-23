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
#pragma once

#include <platform/handle.hpp>
#include <platform/lock.hpp>

/**
 * @file poll.hpp
 * @brief Platform poll interfaces
*/

namespace platform {

class PollPriv;

class Poll {
 public:
    enum PollMode {
        POLL_READ,
        POLL_WRITE,
    };

    Poll(): stateHead(nullptr), mutex(Lock::LOCK_MUTEX) {}

    typedef void (*cb_t)(Poll::PollMode mode, Handle *handle, void *arg);
    typedef void (*exception_t)(Poll::PollMode mode, Handle *handle, void *arg);

    void add(Handle *handle, PollMode mode, cb_t cb, void *arg) {
        mutex.lock();
        stateHead = new StateNode(mode, handle, cb, arg, stateHead);
        mutex.unlock();
        _add(stateHead);
    }

    void mod(Handle *handle, PollMode mode, cb_t cb, void *arg) {
        StateNode *t = stateHead;
        mutex.lock();
        while (t) {
            if (handle == t->handle) {
                _mod(t);
                break;
            }
            t = t->next;
        }
        mutex.unlock();
    }

    void del(Handle *handle) {
        StateNode **t = &stateHead, *cur;
        mutex.lock();
        while (*t) {
            cur = *t;
            if (handle == cur->handle) {
                _del(cur);
                *t = cur->next;
                delete cur;
                break;
            }
            t = &cur->next;
        }
        mutex.unlock();
    }

    void wait(u32 ms);
 private:
    class StateNode {
     public:
        explicit StateNode(PollMode mode, Handle *handle, cb_t cb, void *arg = nullptr, StateNode *next = nullptr):
            mode(mode), handle(handle), cb(cb), arg(arg) {}

        PollMode mode;
        Handle *handle;                                                                                
        cb_t cb;
        void *arg;

        StateNode *next;
     private:
        StateNode();
    } *stateHead;

    void _add(StateNode *state);
    void _mod(StateNode *state);
    void _del(StateNode *state);

    PollPriv *priv;
    Lock mutex;
};

}  // namespace platform
