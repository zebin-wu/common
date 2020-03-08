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

#include <common/exception.hpp>
#include <platform/type.hpp>
#include <platform/args.hpp>
#include <platform/config.hpp>

/**
 * @file handle.hpp
 * @brief Platform Handle interfaces
*/

namespace platform {

/// Only used by class Handle, need a platform to implement.
class HandlePriv;

class Handle {
 public:
    static Handle *in();
    static Handle *out();
    static Handle *err();

    ~Handle();

    size_t write(const void *buf, size_t len);
    size_t read(void *buf, size_t len);

    void print(const char *fmt, ...) ARGS_FORMAT(2, 3);
    void vprint(const char *fmt, va_list args);

 protected:
    friend class Poll;
    HandlePriv *priv;
    Handle();
};

#ifdef PFM_SUPPORT_FILE_HANDLE
class FileHandle: public Handle {
 public:
    enum Flag {
        F_WRITE = (1 << 0),
        F_READ = (1 << 1),
        F_CREAT = (1 << 2),
        F_TRUNC = (1 << 3),
        F_NOBLOCK = (1 << 4),
    };

    enum SeekMode {
        S_SET,
        S_END,
        S_CUR,
    };

    explicit FileHandle(const char *path, int flag);
    size_t seek(SeekMode mode, ssize_t len);
};
#endif  // PFM_SUPPORT_FILE_HANDLE

#ifdef PFM_SUPPORT_SOCKET_HANDLE
class SocketHandle: public Handle {
 public:
    enum DomainType {
        D_UNIX,
        D_IPV4,
        D_IPV6,
    };

    enum SockType {
        S_TCP,
        S_UDP,
        S_RAM,
    };

    explicit SocketHandle(DomainType domain, SockType sock);
};
#endif  // PFM_SUPPORT_SOCKET_HANDLE

class HandleException: public common::Exception {
 public:
    explicit HandleException(Handle *handle, common::ErrorCode err):
        Exception(err), handle(handle) {}
    explicit HandleException(Handle *handle,
        common::ErrorCode err, const char *message):
        Exception(err, message), handle(handle) {}

    Handle *getHandle() const { return handle; }
 private:
    Handle *handle;
};

}  // namespace platform
