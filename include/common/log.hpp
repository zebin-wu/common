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

/**
 * @file log.hpp
 * @brief Common log
 */

namespace common {

class Log {
public:
    enum Level {
        LOG_NONE,
        LOG_ERR,
        LOG_WARN,
        LOG_INFO,
        LOG_DEBUG,
    };

    static Log &Instence() {
        static Log log;
        return log;
    }

    void put(Level level, const char *fmt, ...);
private:
    explicit Log(Level level = LOG_WARN);
    explicit Log(Log const &); /// not need to implement
    Log &operator = (const Log &); /// not need to implement
};

} // namespace common

#define log_put(level, fmt, ...) \
    do { \
        common::Log::Instence().put(level, fmt, __VA_ARGS__) \
    } while (0)

#define log_err(fmt, ...)   log_put(common::Log::LOG_ERR, fmt, __VA_ARGS__)
#define log_warn(fmt, ...)  log_put(common::Log::LOG_WARN, fmt, __VA_ARGS__)
#define log_info(fmt, ...)  log_put(common::Log::LOG_INFO, fmt, __VA_ARGS__)
#define log_debug(fmt, ...) log_put(common::Log::LOG_DEBUG, fmt, __VA_ARGS__)
