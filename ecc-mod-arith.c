/* ecc-mod-arith.c

   Copyright (C) 2013, 2014 Niels Möller

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

/* Development of Nettle's ECC support was funded by the .SE Internet Fund. */

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>

#include "ecc-internal.h"

/* Routines for modp arithmetic. All values are ecc->size limbs, but
   not necessarily < p. */

void
ecc_mod_add (const struct ecc_modulo *m, mp_limb_t *rp,
	     const mp_limb_t *ap, const mp_limb_t *bp)
{
  mp_limb_t cy;
  cy = mpn_add_n (rp, ap, bp, m->size);
  cy = mpn_cnd_add_n (cy, rp, rp, m->B, m->size);
  cy = mpn_cnd_add_n (cy, rp, rp, m->B, m->size);
  assert (cy == 0);  
}

void
ecc_mod_sub (const struct ecc_modulo *m, mp_limb_t *rp,
	     const mp_limb_t *ap, const mp_limb_t *bp)
{
  mp_limb_t cy;
  cy = mpn_sub_n (rp, ap, bp, m->size);
  cy = mpn_cnd_sub_n (cy, rp, rp, m->B, m->size);
  cy = mpn_cnd_sub_n (cy, rp, rp, m->B, m->size);
  assert (cy == 0);  
}

void
ecc_mod_mul_1 (const struct ecc_modulo *m, mp_limb_t *rp,
	       const mp_limb_t *ap, mp_limb_t b)
{
  mp_limb_t hi;

  assert (b <= 0xffffffff);
  hi = mpn_mul_1 (rp, ap, m->size, b);
  hi = mpn_addmul_1 (rp, m->B, m->size, hi);
  assert (hi <= 1);
  hi = mpn_cnd_add_n (hi, rp, rp, m->B, m->size);
  /* Sufficient if b < B^size / p */
  assert (hi == 0);
}

void
ecc_mod_addmul_1 (const struct ecc_modulo *m, mp_limb_t *rp,
		  const mp_limb_t *ap, mp_limb_t b)
{
  mp_limb_t hi;

  assert (b <= 0xffffffff);
  hi = mpn_addmul_1 (rp, ap, m->size, b);
  hi = mpn_addmul_1 (rp, m->B, m->size, hi);
  assert (hi <= 1);
  hi = mpn_cnd_add_n (hi, rp, rp, m->B, m->size);
  /* Sufficient roughly if b < B^size / p */
  assert (hi == 0);
}
  
void
ecc_mod_submul_1 (const struct ecc_modulo *m, mp_limb_t *rp,
		  const mp_limb_t *ap, mp_limb_t b)
{
  mp_limb_t hi;

  assert (b <= 0xffffffff);
  hi = mpn_submul_1 (rp, ap, m->size, b);
  hi = mpn_submul_1 (rp, m->B, m->size, hi);
  assert (hi <= 1);
  hi = mpn_cnd_sub_n (hi, rp, rp, m->B, m->size);
  /* Sufficient roughly if b < B^size / p */
  assert (hi == 0);
}

/* NOTE: mul and sqr needs 2*m->size limbs at rp */
void
ecc_mod_mul (const struct ecc_modulo *m, mp_limb_t *rp,
	     const mp_limb_t *ap, const mp_limb_t *bp)
{
  mpn_mul_n (rp, ap, bp, m->size);
  m->reduce (m, rp);
}

void
ecc_mod_sqr (const struct ecc_modulo *m, mp_limb_t *rp,
	     const mp_limb_t *ap)
{
  mpn_sqr (rp, ap, m->size);
  m->reduce (m, rp);
}

/* Compute R <-- X^{2^k} mod M. Needs 2*ecc->size limbs at rp, and
   2*ecc->size additional limbs of scratch space. No overlap
   allowed. */
void
ecc_mod_pow_2k (const struct ecc_modulo *m,
		mp_limb_t *rp, const mp_limb_t *xp,
		unsigned k, mp_limb_t *tp)
{
  if (k & 1)
    {
      ecc_mod_sqr (m, rp, xp);
      k--;
    }
  else
    {
      ecc_mod_sqr (m, tp, xp);
      ecc_mod_sqr (m, rp, tp);
      k -= 2;
    }
  while (k > 0)
    {
      ecc_mod_sqr (m, tp, rp);
      ecc_mod_sqr (m, rp, tp);
      k -= 2;
    }
}

/* Computes R <-- X^{2^k} * Y. Scratch requirements as ecc_mod_pow_2k. */
void
ecc_mod_pow_2k_mul (const struct ecc_modulo *m,
		    mp_limb_t *rp, const mp_limb_t *xp,
		    unsigned k, const mp_limb_t *yp,
		    mp_limb_t *tp)
{
  ecc_mod_pow_2k (m, tp, xp, k, rp);
  ecc_mod_mul (m, rp, tp, yp);
}
