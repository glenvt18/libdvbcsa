/*

    This file is part of libdvbcsa.

    libdvbcsa is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published
    by the Free Software Foundation; either version 2 of the License,
    or (at your option) any later version.

    libdvbcsa is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libdvbcsa; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307 USA

    Based on FFdecsa, Copyright (C) 2003-2004  fatih89r

    (c) 2006-2011 Alexandre Becoulet <alexandre.becoulet@free.fr>

*/

#include "dvbcsa/dvbcsa.h"
#include "dvbcsa_bs.h"

#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif

void
dvbcsa_bs_key_set (const dvbcsa_cw_t cw, struct dvbcsa_bs_key_s *key)
{
  dvbcsa_keys_t kk;
  int i;

  /* precalculations for stream */

  uint64_t ck = dvbcsa_load_le64(cw);

  for (i = 0; i < DVBCSA_CWBITS_SIZE; i++)
    key->stream[i] = (ck >> (i^4)) & 1 ? BS_VAL8(ff) : BS_VAL8(00);

  /* precalculations for block */

  dvbcsa_key_schedule_block(cw, kk);

  for (i = 0; i < DVBCSA_KEYSBUFF_SIZE; i++)
    {
#if BS_BATCH_SIZE == 32
      *(dvbcsa_u32_aliasing_t *)(key->block + i) = kk[i] * 0x01010101;

#elif BS_BATCH_SIZE == 64
      *(dvbcsa_u64_aliasing_t *)(key->block + i) = kk[i] * 0x0101010101010101ULL;

#elif BS_BATCH_SIZE > 64 && BS_BATCH_SIZE % 64 == 0
      uint64_t v = kk[i] * 0x0101010101010101ULL;
      int j;

      for (j = 0; j < BS_BATCH_BYTES / 8; j++)
        *((dvbcsa_u64_aliasing_t *)(key->block + i) + j) = v;
#else
# error
#endif
    }
}

void
dvbcsa_bs_key_set_mx(struct dvbcsa_bs_key_s *key,
                     const struct dvbcsa_bs_mx_stream_s *stream)
{
  dvbcsa_keys_t kk;
  unsigned int i, s;

#if BS_BATCH_SIZE == 32
  uint32_t mask = 0;
#else
  uint64_t smask[BS_BATCH_BYTES / sizeof(uint64_t)];
  uint64_t bmask[BS_BATCH_BYTES / sizeof(uint64_t)];

  memset(bmask, 0, sizeof(bmask));
  memset(smask, 0, sizeof(smask));
#endif

  if (stream->n_slots == 0)
    return;

#ifdef HAVE_ASSERT_H
  assert(stream->first_slot + stream->n_slots <= BS_BATCH_BYTES);
#endif

#if BS_BATCH_SIZE == 32

  for (s = stream->first_slot; s < stream->first_slot + stream->n_slots; s++)
    {
#ifdef DVBCSA_ENDIAN_LITTLE
      mask |= 0xff << s * 8;
#else
      mask |= 0xff000000 >> s * 8;
#endif
    }

#else

  for (s = stream->first_slot; s < stream->first_slot + stream->n_slots; s++)
    {
      int bi, si, bshift, sshift;

      /* block */
      bi = s / sizeof(uint64_t);
      bshift = (s % sizeof(uint64_t)) * 8;

      /* stream */
      si = (s / (sizeof(uint64_t) / 2)) %
           (BS_BATCH_BYTES / sizeof(uint64_t));

      sshift = (s % (sizeof(uint64_t) / 2)) * 16;

#ifndef DVBCSA_ENDIAN_LITTLE
      bshift = sizeof(uint64_t) * 8 -  8 - bshift;
      sshift = sizeof(uint64_t) * 8 - 16 - sshift;
#endif

      bmask[bi] |= (uint64_t)0xff << bshift;

      smask[si] |= (uint64_t)0x5555 << (sshift + (s >= BS_BATCH_BYTES / 2));
    }

#endif

  /* precalculations for stream */

  uint64_t ck = dvbcsa_load_le64(stream->cw);

  for (i = 0; i < DVBCSA_CWBITS_SIZE; i++)
    {
#if BS_BATCH_SIZE == 32
      dvbcsa_u32_aliasing_t *p = (dvbcsa_u32_aliasing_t *)(key->stream + i);
      uint32_t val = (ck >> (i^4)) & 1 ? 0xffffffff : 0;
      *p = (*p & ~mask) | (val & mask);
#else
      dvbcsa_u64_aliasing_t *p = (dvbcsa_u64_aliasing_t *)(key->stream + i);
      unsigned int j;
      uint64_t val = (ck >> (i^4)) & 1 ? 0xffffffffffffffffULL : 0ULL;

      for (j = 0; j < BS_BATCH_BYTES / sizeof(uint64_t); j++)
        p[j] = (p[j] & ~smask[j]) | (val & smask[j]);
#endif
    }

  /* precalculations for block */

  dvbcsa_key_schedule_block(stream->cw, kk);

  for (i = 0; i < DVBCSA_KEYSBUFF_SIZE; i++)
    {
#if BS_BATCH_SIZE == 32
      dvbcsa_u32_aliasing_t *p = (dvbcsa_u32_aliasing_t *)(key->block + i);
      uint32_t val = kk[i] * 0x01010101;
      *p = (*p & ~mask) | (val & mask);
#else
      dvbcsa_u64_aliasing_t *p = (dvbcsa_u64_aliasing_t *)(key->block + i);
      unsigned int j;
      uint64_t val = kk[i] * 0x0101010101010101ULL;

      for (j = 0; j < BS_BATCH_BYTES / sizeof(uint64_t); j++)
        p[j] = (p[j] & ~bmask[j]) | (val & bmask[j]);
#endif
    }
}

