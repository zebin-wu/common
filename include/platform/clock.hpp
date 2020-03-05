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

/**
 * @file clock.hpp
 * @brief Platform clock interfaces
*/

/// The length of clock format string("MM/DD/YYYY hh:mm:ss").
#define CLOCK_FORMAT_STRING_LEN 20

/// Returns true if the time a(u64) is after time b(u64).
#define TIME_AFTER(a, b) ((s64)(b) - (s64)(a) < 0)

namespace platform {

/// Only used by class Clock, need a platform to implement.
class ClockPriv;

class Clock {
 public:
    /**
     * @enum Time source codes.
     * 
     * @note Larger numbers indicate more reliable clock sources.
    */
    enum Source {
        CS_NONE,            ///< Never been set.
        CS_MIN,             ///< The min source
        CS_DEF,             ///< Set when clock initiliaze.
        CS_LOCAL,           ///< Set by local.
        CS_MCU_LO,          ///< Set by MCU. low priority.
        CS_SERVER,          ///< Set using server time.
        CS_SNTP,            ///< Set using SNTP, not kept in sync.
        CS_NTP,             ///< Set using NTP.
        CS_MCU_HI,          ///< Set by MCU. high priority.
        CS_LIMIT,           ///< Must be last.
    };

    /**
     * @enum 
    */
    enum TimeZone {
        CT_LOCAL,
        CT_UTC,
    };

    ~Clock();

    /**
     * @brief Get a clock instence.
     * 
     * @return a clock instence.
    */
    static Clock &Instance() {
        static Clock instance;
        return instance;
    }

    /**
     * @brief Set clock
     * 
     * @param timestamp is the time as the number of seconds since 1970-01-01 00:00 (UTC).
     * @param src is the time source.
    */
    void set(time_t timestamp, Source src);

    /**
     * @brief Get clock
     * 
     * @param src is the buffer to retrieve the clock source, it can be NULL.
     * @return the time as the number of seconds since 1970-01-01 00:00 (UTC).
    */
    time_t get(Source *src) const;

    /**
     * @brief convert clock to "MM/DD/YYYY hh:mm:ss" format
     * @note str length is 20bytes
     * 
     * @param buf is the buf to store the format string.
     * @param len is the length of str.
     * @return the format string.
    */
    const char *getFormat(char *buf, size_t len);

    /**
     * @brief Get clock (ms)
     * 
     * @param src is the buffer to retrieve the clock source, it can be NULL.
     * @return the time as the number of milliseconds since 1970-01-01 00:00 (UTC).
    */
    u64 getUTCMs(Source *src) const;

    /**
     * @brief Get 64 bits system tick
     * 
     * @return the system tick since the system boots.
    */
    u64 getTotalMs() const;

    /**
     * @brief Reset clock source
    */
    void resetSource();

 private:
    Clock();
    explicit Clock(Clock const &);  /// not need to implement
    Clock &operator = (const Clock &);  /// not need to implement
    ClockPriv *priv;
    Source src;
};

}  // namespace platform
