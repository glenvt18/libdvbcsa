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

    (c) 2006-2008 Alexandre Becoulet <alexandre.becoulet@free.fr>

*/

#ifndef DVBCSA_AVX_H_
#define DVBCSA_AVX_H_

#include <immintrin.h>

typedef __m256i dvbcsa_bs_word_t;

#define BS_BATCH_SIZE 256
#define BS_BATCH_BYTES 32

#define BS_VAL(n, m)    _mm256_set_epi64x(n, m, n, m)
#define BS_VAL64(n)     BS_VAL(0x##n##ULL, 0x##n##ULL)
#define BS_VAL32(n)     BS_VAL64(n##n)
#define BS_VAL16(n)     BS_VAL32(n##n)
#define BS_VAL8(n)      BS_VAL16(n##n)

#define BS_AND(a, b)    _mm256_and_si256((a), (b))
#define BS_OR(a, b)     _mm256_or_si256((a), (b))
#define BS_XOR(a, b)    _mm256_xor_si256((a), (b))
#define BS_NOT(a)       _mm256_andnot_si256((a), BS_VAL8(ff))

#define BS_SHL(a, n)    _mm256_slli_epi64(a, n)
#define BS_SHR(a, n)    _mm256_srli_epi64(a, n)

#define BS_EXTRACT8(a, n) ((dvbcsa_u8_aliasing_t *)&(a))[n]

#define BS_EMPTY()

/* block cipher 2-word load with byte-deinterleaving */
#define BS_LOAD_DEINTERLEAVE_8(ptr, var_lo, var_hi) \
      {\
      dvbcsa_bs_word_t a, b; \
      a = _mm256_load_si256((ptr)); \
      b = _mm256_load_si256((ptr) + 1); \
      a = _mm256_shuffle_epi8(a, _mm256_set_epi8(15,13,11,9,7,5,3,1, 14,12,10,8,6,4,2,0, 15,13,11,9,7,5,3,1, 14,12,10,8,6,4,2,0)); \
      b = _mm256_shuffle_epi8(b, _mm256_set_epi8(15,13,11,9,7,5,3,1, 14,12,10,8,6,4,2,0, 15,13,11,9,7,5,3,1, 14,12,10,8,6,4,2,0)); \
      var_lo = _mm256_unpacklo_epi64(a, b); \
      var_hi = _mm256_unpackhi_epi64(a, b); \
      uint64_t scratch, *swap = (uint64_t *)&var_lo; \
      scratch = swap[1]; \
      swap[1] = swap[2]; \
      swap[2] = scratch; \
      swap = (uint64_t *)&var_hi; \
      scratch = swap[1]; \
      swap[1] = swap[2]; \
      swap[2] = scratch; \
      }
#endif

