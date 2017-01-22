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

#ifndef DVBCSA_NEON_H_
#define DVBCSA_NEON_H_

# include <arm_neon.h>

typedef uint64x2_t dvbcsa_bs_word_t;

#define BS_BATCH_SIZE 128
#define BS_BATCH_BYTES 16

#define BS_VAL(n, m)    vcombine_u64((uint64x1_t)(m), (uint64x1_t)(n))
#define BS_VAL64(n)     vdupq_n_u64(0x##n##ULL)
#define BS_VAL32(n)     vreinterpretq_u64_u32(vdupq_n_u32(0x##n))
#define BS_VAL16(n)     vreinterpretq_u64_u16(vdupq_n_u16(0x##n))
#define BS_VAL8(n)      vreinterpretq_u64_u8(vdupq_n_u8(0x##n))

#define BS_AND(a, b)    vandq_u64 ((a), (b))
#define BS_OR(a, b)     vorrq_u64 ((a), (b))
#define BS_XOR(a, b)    veorq_u64 ((a), (b))
#define BS_NOT(a)       vreinterpretq_u64_u8(vmvnq_u8(vreinterpretq_u8_u64(a)))

#define BS_SHL(a, n)    vshlq_n_u64 ((a), n)
#define BS_SHR(a, n)    vshrq_n_u64 ((a), n)
#define BS_EXTRACT8(a, n) (((dvbcsa_u8_aliasing_t *)&(a))[n])

#define BS_EMPTY()

/* 2x2 matrix transpose */
#ifdef BS_SWAP32_LE
#undef BS_SWAP32_LE
#endif
#define BS_SWAP32_LE(t, b) \
    { \
    uint32x4x2_t tmp = vtrnq_u32(vreinterpretq_u32_u64(t), vreinterpretq_u32_u64(b)); \
    t = vreinterpretq_u64_u32(tmp.val[0]); \
    b = vreinterpretq_u64_u32(tmp.val[1]); \
    }

#ifdef BS_SWAP16_LE
#undef BS_SWAP16_LE
#endif
#define BS_SWAP16_LE(t, b) \
    { \
    uint16x8x2_t tmp = vtrnq_u16(vreinterpretq_u16_u64(t), vreinterpretq_u16_u64(b)); \
    t = vreinterpretq_u64_u16(tmp.val[0]); \
    b = vreinterpretq_u64_u16(tmp.val[1]); \
    }

#ifdef BS_SWAP8_LE
#undef BS_SWAP8_LE
#endif
#define BS_SWAP8_LE(t, b) \
    { \
    uint8x16x2_t tmp = vtrnq_u8(vreinterpretq_u8_u64(t), vreinterpretq_u8_u64(b)); \
    t = vreinterpretq_u64_u8(tmp.val[0]); \
    b = vreinterpretq_u64_u8(tmp.val[1]); \
    }

/* block cipher 2-word load with byte-deinterleaving */
#define BS_LOAD_DEINTERLEAVE_8(ptr, var_lo, var_hi) \
      {\
      uint8x16x2_t tmp = vld2q_u8((unsigned char *)(ptr));\
      var_lo = vreinterpretq_u64_u8(tmp.val[0]);\
      var_hi = vreinterpretq_u64_u8(tmp.val[1]);\
      }

#endif

