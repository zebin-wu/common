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

#include <common/error.hpp>

/**
 * @file errno.hpp
 * @brief Change the system error number to common::ErrorCode
*/

namespace platform {

/**
 * @brief Used to describe the error when it occurs
*/
struct ErrorDesc {
    int sys_err;    ///< system call error number
    common::ErrorCode err;  ///< common error code
    const char *msg;    ///< error message
};

static const ErrorDesc *getErrorDesc(int err,
    const ErrorDesc *descs, int len) {
    for (int i = 0; i < len; i++) {
        if (descs[i].sys_err == err) {
            return &descs[i];
        }
    }
    return nullptr;
}

}  // namespace platform
