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
 * @file type.hpp
 * @brief Platform types.
*/

#include <sys/types.h>
#include <cstdint>
#include <cstddef>

#ifndef PFM_HAVE_UTYPES
typedef uint8_t u8;             ///< unsigned 8-bit integer
typedef uint16_t u16;           ///< unsigned 16-bit integer
typedef uint32_t u32;           ///< unsigned 32-bit integer

typedef int8_t s8;              ///< signed 8-bit integer
typedef int16_t s16;            ///< signed 16-bit integer
typedef int32_t s32;            ///< signed 32-bit integer
#endif

#ifndef PFM_HAVE_UTYPES_64
typedef uint64_t u64;           ///< unsigned 64-bit integer
typedef int64_t s64;            ///< signed 64-bit integer
#endif
