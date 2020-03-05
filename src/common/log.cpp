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
#include <common/log.hpp>
#include <platform/args.hpp>
#include <platform/handle.hpp>
#include <platform/lock.hpp>
#include <platform/clock.hpp>

namespace common {

class LogPriv {
 public:
    LogPriv(): level(Log::LOG_WARN) {
        handle = getHandle(level);
    }

    platform::Handle *getHandle(Log::Level level) {
        if (level > Log::LOG_NONE &&
            level <= Log::LOG_WARN) {
            return platform::Handle::err();
        }
        return platform::Handle::out();
    }

    Log::Level level;
    platform::Handle *handle;
    platform::Lock lock;
};

static LogPriv logPriv;

/**
 * @brief Return a string literal with the log message prefix.
*/
static const char *getLogLevelString(Log::Level level) {
    switch (level) {
    case Log::LOG_WARN:
        return "WRN";
        break;
    case Log::LOG_INFO:
        return "INF";
    case Log::LOG_DEBUG:
        return "DBG";
    default:
        return "ERR";
        break;
    }
}

void Log::put(Level level, const char *fmt, ...) {
    va_list ap;
    char clock_str[CLOCK_FORMAT_STRING_LEN];
    platform::Handle *handle = logPriv.handle;

    if (level <= logPriv.level) {
        platform::Clock::Instance().getFormat(clock_str, sizeof(clock_str));
        handle->print("[%s] %s ",
            getLogLevelString(level), clock_str);
        va_start(ap, fmt);
        handle->vprint(fmt, ap);
        va_end(ap);
        handle->print("\n");
    }
}

Log::Level Log::getLevel() {
    return logPriv.level;
}

void Log::setLevel(Level level) {
    logPriv.lock.lock();
    logPriv.level = level;
    logPriv.handle = logPriv.getHandle(level);
    logPriv.lock.unlock();
}

}  // namespace common
