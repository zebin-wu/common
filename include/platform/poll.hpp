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
        POLL_ERR,
    };

    typedef void (*cb_t)(Poll::PollMode mode, Handle *handle, void *arg);

    Poll();
    ~Poll();

    void add(Handle *handle, PollMode mode, cb_t cb, void *arg);

    void mod(Handle *handle, PollMode mode, cb_t cb, void *arg);

    void del(Handle *handle, PollMode mode);

    /**
     * @brief It blocks the thread for the max_wait ms, and do the platform polling.
     * 
     * @param timeout specifies the maximum wait time in milliseconds(-1 == infinite)
    */
    void polling(int timeout);

 private:
    PollPriv *priv;
};

class PollException: public common::Exception {
 public:
    explicit PollException(Poll *poll, common::ErrorCode err):
        Exception(err), poll(poll) {}
    explicit PollException(Poll *poll,
        common::ErrorCode err, const char *message):
        Exception(err, message), poll(poll) {}
 private:
    Poll *poll;
};

}  // namespace platform
