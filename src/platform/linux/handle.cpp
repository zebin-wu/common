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
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <common/assert.hpp>
#include <platform/args.hpp>
#include <platform/handle.hpp>
#include <platform/handle_int.hpp>
#include <platform/error.hpp>

/// The size of handle buffer
#define PFM_HANDLE_BUF_SIZE 4096

namespace platform {

static const ErrorDesc openErrDescs[] = {
    {EACCES, common::ERR_PERM,
        "the requested access to the handle is not allowed"},
    {EDQUOT, common::ERR_DQUOT,
        "when MO_CREAT is specified, the file does not exist"},
    {EFAULT, common::ERR_ERR,
        "handle points outside your accessible address space"},
    {EINTR, common::ERR_BUSY,
        "while blocked waiting to complete an open of a slow "
        "device(e.g., a FIFO), the call was interrupted by a signal  handler"},
    {EINVAL, common::ERR_INVAL_ARG},
    {ENOENT, common::ERR_NOENT, "the handle does not exist"},
};

static const ErrorDesc rwErrDescs[] = {
    {EAGAIN, common::ERR_AGAIN,
        "the handle has been marked nonblocking, try again"},
    {EINVAL, common::ERR_INVAL_ARG},
    {EFAULT, common::ERR_OVER_RANGE,
        "buf is outside your accessible address space"},
    {EINTR, common::ERR_INTR, "The call was interrupted by a signal"},
};

Handle *Handle::in() {
    static Handle *inHandle = new Handle;
    inHandle->priv->fd = STDIN_FILENO;
    return inHandle;
}

Handle *Handle::out() {
    static Handle *outHandle = new Handle;
    outHandle->priv->fd = STDOUT_FILENO;
    return outHandle;
}

Handle *Handle::err() {
    static Handle *errHandle = new Handle;
    errHandle->priv->fd = STDERR_FILENO;
    return errHandle;
}

void Handle::print(const char *fmt, ...) {
    va_list ap;
    int size;
    char buf[PFM_HANDLE_BUF_SIZE];

    va_start(ap, fmt);
    size = vsnprintf(buf, sizeof(buf), fmt, ap);
    if (size <= 0) {
        throw HandleException(this, common::ERR_ERR);
        goto end;
    }
    write(buf, size);
end:
    va_end(ap);
}

void Handle::vprint(const char *fmt, va_list args) {
    int size;
    char buf[PFM_HANDLE_BUF_SIZE];

    size = vsnprintf(buf, sizeof(buf), fmt, args);
    if (size <= 0) {
        throw HandleException(this, common::ERR_ERR);
        return;
    }
    write(buf, size);
}

Handle::Handle(): priv(new HandlePriv) {}

Handle::~Handle() {
    close(priv->fd);
    delete priv;
}

size_t Handle::write(const void *buf, size_t len) {
    ssize_t wlen;
    wlen = ::write(priv->fd, buf, len);
    if (wlen <= 0) {
        const ErrorDesc *desc = getErrorDesc(errno,
            rwErrDescs, ARRAY_LEN(rwErrDescs));
        throw HandleException(this, desc->err, desc->msg);
        return 0;
    }
    return static_cast<size_t>(wlen);
}

size_t Handle::read(void *buf, size_t len) {
    ssize_t rlen;
    rlen = ::read(priv->fd, buf, len);
    if (rlen <= 0) {
        const ErrorDesc *desc = getErrorDesc(errno,
            rwErrDescs, ARRAY_LEN(rwErrDescs));
        throw HandleException(this, desc->err, desc->msg);
        return 0;
    }
    return static_cast<size_t>(rlen);
}

static int getOpenFlag(int mode) {
    int flag = 0;
    bool read = mode & FileHandle::F_READ;
    bool write = mode & FileHandle::F_WRITE;
    if (read && write) {
        flag |= O_RDWR;
    } else if (read) {
        flag |= O_RDONLY;
    } else if (write) {
        flag |= O_WRONLY;
    }
    if (mode & FileHandle::F_CREAT) {
        flag |= O_CREAT;
    }
    if (mode & FileHandle::F_NOBLOCK) {
        flag |= O_NONBLOCK;
    }
    if (mode & FileHandle::F_TRUNC) {
        flag |= O_TRUNC;
    }
    return flag;
}

FileHandle::FileHandle(const char *path, int mode) {
    ASSERT(path);
    int fd = open(path, getOpenFlag(mode), 0664);
    if (fd < 0) {
        const ErrorDesc *desc = getErrorDesc(errno,
            openErrDescs, ARRAY_LEN(openErrDescs));
        throw HandleException(this, desc->err, desc->msg);
        return;
    }
    priv->fd = fd;
}

size_t FileHandle::seek(SeekMode mode, ssize_t len) {
    int whence;
    off_t ret;
    switch (mode) {
    case S_SET:
        whence = SEEK_SET;
        break;
    case S_CUR:
        whence = S_CUR;
        break;
    case S_END:
        whence = S_END;
        break;
    }
    ret = ::lseek(priv->fd, len, whence);
    if (ret < 0) {
        throw HandleException(this, common::ERR_ERR);
        return 0;
    }
    return static_cast<size_t>(ret);
}

}  // namespace platform
