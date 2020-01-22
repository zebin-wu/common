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
#include <pthread.h>
#include <platform/lock.hpp>

namespace platform {

class LockPriv {
 public:
    union {
       pthread_mutex_t mutex;
    };
};

Lock::Lock(Type type): type(type), priv(new LockPriv) {
    switch (type) {
    case LOCK_MUTEX:
        pthread_mutex_init(&priv->mutex, NULL);
        break;
    default:
        break;
    }
}

Lock::~Lock() {
    switch (type) {
    case LOCK_MUTEX:
        pthread_mutex_destroy(&priv->mutex);
        break;
    default:
        break;
    }
    delete priv;
}

void Lock::lock() {
    switch (type) {
    case LOCK_MUTEX:
        pthread_mutex_lock(&priv->mutex);
        break;
    defualt:
        break;
    }
}

void Lock::unlock() {
    switch (type) {
    case LOCK_MUTEX:
        pthread_mutex_unlock(&priv->mutex);
        break;
    defualt:
        break;
    }
}

}  // namespace platform
