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
/*
#define BS_LOAD_DEINTERLEAVE_8(ptr, var_lo, var_hi) \
      {\
      dvbcsa_bs_word_t a, b; \
      a = _mm256_load_si256((ptr)); \
      b = _mm256_load_si256((ptr) + 1); \
      a = _mm256_shuffle_epi8(a, _mm256_set_epi8(15,13,11,9,7,5,3,1, 14,12,10,8,6,4,2,0, 15,13,11,9,7,5,3,1, 14,12,10,8,6,4,2,0)); \
      b = _mm256_shuffle_epi8(b, _mm256_set_epi8(15,13,11,9,7,5,3,1, 14,12,10,8,6,4,2,0, 15,13,11,9,7,5,3,1, 14,12,10,8,6,4,2,0)); \
      var_lo = _mm256_unpacklo_epi64(a, b); \
      var_hi = _mm256_unpackhi_epi64(a, b); \
      var_lo = _mm256_permute4x64_epi64(var_lo, 0xD8); \
      var_hi = _mm256_permute4x64_epi64(var_hi, 0xD8); \
      }
*/

static inline void block_sbox_avx1(dvbcsa_bs_word_t *src, dvbcsa_bs_word_t *dst) {
	int j;
	dvbcsa_bs_word_t a, i, b, res, lsb_mask = _mm256_set_epi32(0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff);
	for (j = 0; j < 8; j++) {
		i = _mm256_load_si256(src + j);

		a = BS_AND(i, lsb_mask);					//a = src[j+N]
		b = _mm256_i32gather_epi32(dvbcsa_block_sbox, a, 1);		//b = dvbcsa_block_sbox
		res = BS_AND(b, lsb_mask);

		a = _mm256_srli_epi32(i, 8);
		a = BS_AND(a, lsb_mask);
		b = _mm256_i32gather_epi32(dvbcsa_block_sbox, a, 1);
		b = BS_AND(b, lsb_mask);
		b = _mm256_slli_epi32(b, 8);
		res = BS_OR(res, b);

		a = _mm256_srli_epi32(i, 16);
		a = BS_AND(a, lsb_mask);
		b = _mm256_i32gather_epi32(dvbcsa_block_sbox, a, 1);
		b = BS_AND(b, lsb_mask);
		b = _mm256_slli_epi32(b, 16);
		res = BS_OR(res, b);

		a = _mm256_srli_epi32(i, 24);
		a = BS_AND(a, lsb_mask);
		b = _mm256_i32gather_epi32(dvbcsa_block_sbox, a, 1);
		b = BS_AND(b, lsb_mask);
		b = _mm256_slli_epi32(b, 24);
		res = BS_OR(res, b);

		_mm256_store_si256(dst + j, res);
	}
}

static inline void block_sbox_avx2(dvbcsa_bs_word_t *src, dvbcsa_bs_word_t *dst) {
	int j;
	dvbcsa_bs_word_t a, i, b, res, lsb_mask = _mm256_set_epi32(0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff);
	for (j = 0; j < 8; j++) {
		i = _mm256_load_si256(src + j);

		a = BS_AND(i, lsb_mask);
		b = _mm256_i32gather_epi32(dvbcsa_block_sbox, a, 1);
		res = BS_AND(b, lsb_mask);

		i = _mm256_srli_epi32(i, 8);
		a = BS_AND(i, lsb_mask);
		b = _mm256_i32gather_epi32(dvbcsa_block_sbox, a, 1);
		b = BS_AND(b, lsb_mask);
		b = _mm256_slli_epi32(b, 8);
		res = BS_OR(res, b);

		i = _mm256_srli_epi32(i, 8);
		a = BS_AND(i, lsb_mask);
		b = _mm256_i32gather_epi32(dvbcsa_block_sbox, a, 1);
		b = BS_AND(b, lsb_mask);
		b = _mm256_slli_epi32(b, 16);
		res = BS_OR(res, b);

		a = _mm256_srli_epi32(i, 8);
		b = _mm256_i32gather_epi32(dvbcsa_block_sbox, a, 1);
		b = _mm256_slli_epi32(b, 24);
		res = BS_OR(res, b);

		_mm256_store_si256(dst + j, res);
	}
}

static inline void block_sbox_avx3(dvbcsa_bs_word_t *src, dvbcsa_bs_word_t *dst) {
	int j;
	dvbcsa_bs_word_t a, i, b, res, lsb_mask = _mm256_set_epi32(0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff);
	for (j = 0; j < 8; j++) {
		i = _mm256_load_si256(src + j);

		a = BS_AND(i, lsb_mask);
		b = _mm256_i32gather_epi32(dvbcsa_block_sbox, a, 1);
		res = BS_AND(b, lsb_mask);

		a = _mm256_shuffle_epi8(i, _mm256_set_epi8(0x80,0x80,0x80,13,0x80,0x80,0x80,9,0x80,0x80,0x80,5,0x80,0x80,0x80,1, 0x80,0x80,0x80,13,0x80,0x80,0x80,9,0x80,0x80,0x80,5,0x80,0x80,0x80,1));
		b = _mm256_i32gather_epi32(dvbcsa_block_sbox, a, 1);
		b = _mm256_shuffle_epi8(b, _mm256_set_epi8(0x80,0x80,12,0x80,0x80,0x80,8,0x80,0x80,0x80,4,0x80,0x80,0x80,0,0x80, 0x80,0x80,12,0x80,0x80,0x80,8,0x80,0x80,0x80,4,0x80,0x80,0x80,0,0x80));
		res = BS_OR(res, b);

		a = _mm256_shuffle_epi8(i, _mm256_set_epi8(0x80,0x80,0x80,14,0x80,0x80,0x80,10,0x80,0x80,0x80,6,0x80,0x80,0x80,2, 0x80,0x80,0x80,14,0x80,0x80,0x80,10,0x80,0x80,0x80,6,0x80,0x80,0x80,2));
		b = _mm256_i32gather_epi32(dvbcsa_block_sbox, a, 1);
		b = _mm256_shuffle_epi8(b, _mm256_set_epi8(0x80,12,0x80,0x80,0x80,8,0x80,0x80,0x80,4,0x80,0x80,0x80,0,0x80,0x80, 0x80,12,0x80,0x80,0x80,8,0x80,0x80,0x80,4,0x80,0x80,0x80,0,0x80,0x80));
		res = BS_OR(res, b);

		a = _mm256_shuffle_epi8(i, _mm256_set_epi8(0x80,0x80,0x80,15,0x80,0x80,0x80,11,0x80,0x80,0x80,7,0x80,0x80,0x80,3, 0x80,0x80,0x80,15,0x80,0x80,0x80,11,0x80,0x80,0x80,7,0x80,0x80,0x80,3));
		b = _mm256_i32gather_epi32(dvbcsa_block_sbox, a, 1);
		b = _mm256_slli_epi32(b, 24);
		res = BS_OR(res, b);

		_mm256_store_si256(dst + j, res);
	}
}

// BS_LOAD_DEINTERLEAVE_8 replacement that only loads data that are already uninterleaved

#define BS_LOAD_DEINTERLEAVE_8(ptr, var_lo, var_hi) \
      {\
      var_lo = _mm256_load_si256((ptr)); \
      var_hi = _mm256_load_si256((ptr) + 1); \
      }

extern const uint16_t dvbcsa_block_sbox_perm[256];
static inline void block_sbox_permute_interleave_avx(dvbcsa_bs_word_t *src, dvbcsa_bs_word_t *dst) {
	int j;
	dvbcsa_bs_word_t a, i, b, res1, res2;
	dvbcsa_bs_word_t lsb_mask = _mm256_set_epi32(0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff);
	dvbcsa_bs_word_t lsw_mask = _mm256_set_epi32(0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff);
	for (j = 0; j < 8; j++) {
		i = _mm256_load_si256(src + j);

		//part 1
		a = BS_AND(i, lsb_mask);
		b = _mm256_i32gather_epi32(dvbcsa_block_sbox_perm, a, 2);
		res1 = BS_AND(b, lsw_mask);

		a = _mm256_shuffle_epi8(i, _mm256_set_epi8(0x80,0x80,0x80,13,0x80,0x80,0x80,9,0x80,0x80,0x80,5,0x80,0x80,0x80,1, 0x80,0x80,0x80,13,0x80,0x80,0x80,9,0x80,0x80,0x80,5,0x80,0x80,0x80,1));
		b = _mm256_i32gather_epi32(dvbcsa_block_sbox_perm, a, 2);
		b = _mm256_slli_epi32(b, 16);
		res1 = BS_OR(res1, b);

		//part 2
		a = _mm256_shuffle_epi8(i, _mm256_set_epi8(0x80,0x80,0x80,14,0x80,0x80,0x80,10,0x80,0x80,0x80,6,0x80,0x80,0x80,2, 0x80,0x80,0x80,14,0x80,0x80,0x80,10,0x80,0x80,0x80,6,0x80,0x80,0x80,2));
		b = _mm256_i32gather_epi32(dvbcsa_block_sbox_perm, a, 2);
		res2 = BS_AND(b, lsw_mask);

		a = _mm256_shuffle_epi8(i, _mm256_set_epi8(0x80,0x80,0x80,15,0x80,0x80,0x80,11,0x80,0x80,0x80,7,0x80,0x80,0x80,3, 0x80,0x80,0x80,15,0x80,0x80,0x80,11,0x80,0x80,0x80,7,0x80,0x80,0x80,3));
		b = _mm256_i32gather_epi32(dvbcsa_block_sbox_perm, a, 2);
		b = _mm256_slli_epi32(b, 16);
		res2 = BS_OR(res2, b);

		//unpack
		a = _mm256_unpacklo_epi32(res1, res2);
		b = _mm256_unpackhi_epi32(res1, res2);

		a = _mm256_shuffle_epi8(res1, _mm256_set_epi8(15,13,11,9,7,5,3,1, 14,12,10,8,6,4,2,0, 15,13,11,9,7,5,3,1, 14,12,10,8,6,4,2,0));
		b = _mm256_shuffle_epi8(res2, _mm256_set_epi8(15,13,11,9,7,5,3,1, 14,12,10,8,6,4,2,0, 15,13,11,9,7,5,3,1, 14,12,10,8,6,4,2,0));
		res1 = _mm256_unpacklo_epi16(a, b);
		res2 = _mm256_unpackhi_epi16(a, b);

		_mm256_store_si256(dst + 2*j, res1);
		_mm256_store_si256(dst + 2*j + 1, res2);
	}
}

#endif

