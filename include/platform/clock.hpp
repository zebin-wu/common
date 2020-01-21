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
#include <platform/type.hpp>

/**
 * @file clock.hpp
 * @brief Platform clock interfaces
 */

using namespace common;

namespace platform {

class ClockPriv;    /// Only used by Clock, need a platform to implement.

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
        CS_SNTP = CS_SERVER,///< Set using SNTP, not kept in sync.
        CS_NTP,             ///< Set using NTP.
        CS_MCU_HI,          ///< Set by MCU. high priority.
        CS_LIMIT,           ///< Must be last.
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
    ErrorCode set(time_t timestamp, Source src);

    /**
     * @brief Get clock
     * 
     * @param src is the buffer to retrieve the clock source, it can be NULL.
     * @return the time as the number of seconds since 1970-01-01 00:00 (UTC).
     */
    time_t get(Source *src) const;

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
    explicit Clock();
    explicit Clock(Clock const &); /// not need to implement
    Clock &operator = (const Clock &); /// not need to implement
    ClockPriv *priv;
    Source src;
};

} // namespace platform
