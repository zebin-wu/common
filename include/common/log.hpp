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

#include <platform/args.hpp>
#include <common/error.hpp>

/**
 * @file log.hpp
 * @brief Common log interfaces.
 */

namespace common {

class Log {
 public:
    /**
     * @enum Log level.
     */
    enum Level {
        LOG_NONE,
        LOG_ERR,
        LOG_WARN,
        LOG_INFO,
        LOG_DEBUG,
    };

    /**
     * @brief Log message.
     * 
     * @param level is the log level of the message.
     * @param fmt is the format string.(see printf() in C library)
     */
    static void put(Level level, const char *fmt, ...) ARGS_FORMAT(2, 3);

    /**
     * @brief Get the level of log system.
     * 
     * @return the log level.
     */
    static Level getLevel();

    /**
     * @brief Set the level of log system.
     * 
     * @param level is the level of log system.
     */
    static void setLevel(Level level);

 private:
    Log();  /// not need to implement
    explicit Log(Log const &);  /// not need to implement
    Log &operator = (const Log &);  /// not need to implement
};

}  // namespace common

#define log_put(level, ...) \
    do { \
        common::Log::put(level, __VA_ARGS__); \
    } while (0)

#define log_err(...)   log_put(common::Log::LOG_ERR, __VA_ARGS__)
#define log_warn(...)  log_put(common::Log::LOG_WARN, __VA_ARGS__)
#define log_info(...)  log_put(common::Log::LOG_INFO, __VA_ARGS__)
#define log_debug(...) log_put(common::Log::LOG_DEBUG, __VA_ARGS__)
