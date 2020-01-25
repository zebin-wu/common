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
#include <platform/io.hpp>
#include <platform/lock.hpp>

namespace common {

class LogPriv {
 public:
    LogPriv(): level(Log::LOG_WARN) {}

    platform::IO::FileNo getFileNo() const {
        return level <= Log::LOG_WARN ?
            platform::IO::STDERR : platform::IO::STDOUT;
    }

    Log::Level level;
    platform::Lock lock;
};

static LogPriv logPriv;

/**
 * @brief Return a string literal with the log message prefix.
 */
const char *getLogPrefix(Log::Level level) {
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
    int fileNo;

    if (level <= logPriv.level) {
        fileNo = logPriv.getFileNo();
        platform::IO::printNo(fileNo, "[%s]", getLogPrefix(level));
        va_start(ap, fmt);
        platform::IO::printNo(fileNo, fmt, ap);
        va_end(ap);
        platform::IO::printNo(fileNo, "\n");
    }
}

Log::Level Log::getLevel() {
    return logPriv.level;
}

void Log::setLevel(Level level) {
    logPriv.lock.lock();
    logPriv.level = level;
    logPriv.lock.unlock();
}

}  // namespace common
