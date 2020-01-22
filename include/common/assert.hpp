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
 * @file assert.hpp
 * @brief Common assert interfaces
 */

#include <platform/config.hpp>

#ifdef PFM_SUPPORT_C_LIBRARY
#include <cassert>
#else  // PFM_SUPPORT_C_LIBRARY
#include <platform/assert.hpp>
#endif  // PFM_SUPPORT_C_LIBRARY

#ifdef PFM_SUPPORT_C_LIBRARY
#define ASSERT(expr)  assert(expr)
#define ASSERT_NOTREACHED()   ASSERT(0)
#else  // PFM_SUPPORT_C_LIBRARY
#define ASSERT(expr) \
    do { \
        if (!expr) { \
            pfm_assert_handle(__FILE__, __LINE__, #expr, __func__); \
        } \
    } while (0)
#define ASSERT_NOTREACHED() \
    do { \
        pfm_assert_handle(__FILE__, __LINE__, "", __func__); \
    } while (0)
#endif  // PFM_SUPPORT_C_LIBRARY
