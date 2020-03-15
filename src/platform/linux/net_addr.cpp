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
#include <platform/net/addr.hpp>
#include <platform/net/addr_int.hpp>

namespace platform {

namespace net {

Addr4::Addr4(u32 ip): Addr(Addr::IPV4), priv(new Addr4Priv) {
    priv->sin_addr.s_addr = (in_addr_t)htonl((uint32_t)ip);
}

Addr4::Addr4(const Addr4 &addr): Addr(addr.getType()) {
    priv = new Addr4Priv(*addr.priv);
}

Addr4::~Addr4() {
    delete priv;
}

void Addr4::setIp(u32 ip) {
    priv->sin_addr.s_addr = (in_addr_t)htonl((uint32_t)ip);
}

u32 Addr4::getIp() const {
    return (u32)ntohl((uint32_t)priv->sin_addr.s_addr);
}

}  // namespace net

}  // namespace platform
