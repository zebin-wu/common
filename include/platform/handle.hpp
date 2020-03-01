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

/**
 * @file handle.hpp
 * @brief Platform Handle interfaces
*/

namespace platform {

class HandlePriv;

class Handle {
 public:
    enum FileNo {
        STDIN,
        STDOUT,
        STDERR,
    };

    enum Mode {
        MO_WRITE = (1 << 0),
        MO_READ = (1 << 1),
        MO_CREAT = (1 << 2),
        MO_TRUNC = (1 << 3),
        MO_NOBLOCK = (1 << 4),
    };

    enum SeekMode {
        SEEK_MO_SET,
        SEEK_MO_END,
        SEEK_MO_CUR,
    };

    static void printNo(FileNo fileNo, const char *fmt, ...) ARGS_FORMAT(2, 3);
    static void vprintNo(FileNo fileNo, const char *fmt, va_list args);

    explicit Handle(const char *path, int mode);
    ~Handle();

    int getFileNo();
    size_t write(const void *buf, size_t len);
    size_t read(void *buf, size_t len);
    size_t seek(SeekMode mode, ssize_t len);

 private:
    HandlePriv *priv;
};

class HandleException: public common::Exception {
 public:
    explicit HandleException(Handle *handle, common::ErrorCode err):
        Exception(err), handle(handle) {}
    explicit HandleException(Handle *handle,
        common::ErrorCode err, const char *message):
        Exception(err, message), handle(handle) {}
 private:
    Handle *handle;
};

}  // namespace platform
