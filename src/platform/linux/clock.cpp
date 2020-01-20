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
#include <ctime>
#include <sys/time.h>
#include <platform/clock.hpp>

#define THOUSAND 1000

using namespace platform;

Clock *Clock::m_pClock = NULL;
Lock Clock::m_mutex(Lock::LOCK_MUTEX);
Clock::GC Clock::GC::gc;

Clock::Clock()
{
    resetSource();
}

Clock::~Clock()
{
}

ErrorCode Clock::set(time_t timestamp, Source src)
{
    ErrorCode err = ERR_OK;
	struct timeval now;

	m_mutex.lock();
	if (src < this->src || src >= CS_LIMIT) {
		err = ERR_INVAL_ARG;
		goto exit;
	}

	now.tv_sec = timestamp;
	now.tv_usec = 0;
	if (settimeofday(&now, NULL)) {
        err = ERR_ERR;
    }
	this->src = src;
exit:
	m_mutex.unlock();
	return err;
}

time_t Clock::get(Source *src) const
{
	struct timeval now;
	time_t ct;

	m_mutex.lock();
	gettimeofday(&now, NULL);
	if (src) {
		*src = this->src;
	}
	ct = now.tv_sec + (now.tv_usec / THOUSAND / THOUSAND);
	m_mutex.unlock();
	return ct;
}

u64 Clock::getUTCMs(Source *src) const
{
	struct timeval now;
	u64 ct;

	m_mutex.lock();
	gettimeofday(&now, NULL);
	if (src) {
		*src = this->src;
	}
	ct = (u64)now.tv_sec * THOUSAND +
		(u64)now.tv_usec / THOUSAND;
	m_mutex.unlock();
	return (u64)ct;
}

u64 Clock::getTotalMs() const
{
	struct timespec ts;
	u64 ct;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	ct = (u64)ts.tv_sec * THOUSAND +
	    (u64)ts.tv_nsec / THOUSAND / THOUSAND;
	return ct;
}
