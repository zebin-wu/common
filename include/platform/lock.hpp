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
 * @file lock.hpp
 * @brief Platform lock interfaces
*/

namespace platform {

/// Only used by class Lock, need a platform to implement.
class LockPriv;

class Lock {
 public:
    /**
     * @enum Lock types.
    */
    enum Type{
        LOCK_MUTEX,         ///< mutex
    };

    explicit Lock(Type type = LOCK_MUTEX);
    ~Lock();

    /**
     * @brief Lock the lock or take the semaphore.
     * @details The function blocks the current thread when the lock is locked.
    */
    void lock();

    /**
     * @brief Unlock the lock or give the semaphore.
    */
    void unlock();

    /**
     * @brief Get the type of lock.
     * 
     * @return the type of lock.
    */
    Type getType() const { return type; }

 private:
    LockPriv *priv;
    const Type type;
};

}  // namespace platform
