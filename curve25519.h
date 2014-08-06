/* curve25519.h

   Copyright (C) 2014 Niels Möller

   This file is part of GNU Nettle.

   GNU Nettle is free software: you can redistribute it and/or
   modify it under the terms of either:

     * the GNU Lesser General Public License as published by the Free
       Software Foundation; either version 3 of the License, or (at your
       option) any later version.

   or

     * the GNU General Public License as published by the Free
       Software Foundation; either version 2 of the License, or (at your
       option) any later version.

   or both in parallel, as here.

   GNU Nettle is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received copies of the GNU General Public License and
   the GNU Lesser General Public License along with this program.  If
   not, see http://www.gnu.org/licenses/.
*/

#ifndef NETTLE_CURVE25519_H
#define NETTLE_CURVE25519_H

#include "nettle-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Name mangling */
#define curve25519_base nettle_curve25519_base

#define CURVE25519_SIZE 32

void
curve25519_base (uint8_t *q, const uint8_t *n);
  
#endif /* NETTLE_CURVE25519_H */
