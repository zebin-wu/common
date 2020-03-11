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

/// Only used by class Poll, need a platform to implement.
class PollPriv;

class Poll {
 public:
    /**
     * @enum Handle events that can be polled.
    */
    enum Event {
        EV_READ,      ///< read event
        EV_WRITE,     ///< write event
        EV_ERR,       ///< error event
    };

    /// A callback function of the handle evnet.
    typedef void (*cb_t)(Poll::Event event, Handle *handle, void *arg);

    Poll();
    ~Poll();

    /**
     * @brief Add handle event to the poll.
     * @note A handle can be added multiple events, but a handle event can be added only once.
     *
     * @param handle is a point to handle
     * @param event is the event of the handle
     * @param cb is the callback of the handle evnet
     * @param arg is a argument to pass to the callback function
    */
    void add(Handle *handle, Event event, cb_t cb, void *arg);

    /**
     * @brief Modify already added handle events.
     *
     * @param handle is a point to handle
     * @param event is the event of the handle
     * @param cb is the callback of the handle evnet
     * @param arg is a argument to pass to the callback function
    */
    void mod(Handle *handle, Event event, cb_t cb, void *arg);

    /**
     * @brief Delete already added handle events.
     *
     * @param handle is a point to handle
     * @param event is the event of the handle
    */
    void del(Handle *handle, Event event);

    /**
     * @brief It blocks the thread for the max_wait ms, and do the platform polling.
     * 
     * @param timeout specifies the maximum wait time in milliseconds(-1 == infinite)
    */
    void polling(int timeout);

    /**
     * @brief Wakeup the polling().
    */
    void wakeup();

 private:
    PollPriv *priv;
};

typedef common::ObjectException<Poll> PollException;

}  // namespace platform
