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

    (c) 2006-2008 Alexandre Becoulet <alexandre.becoulet@free.fr>

*/

#ifndef DVBCSA_PV_H_
# define DVBCSA_PV_H_

#include "config.h"

#if STDC_HEADERS
# include <stdlib.h>
# include <stddef.h>
#else
# if HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif
#if HAVE_STRING_H
# if !STDC_HEADERS && HAVE_MEMORY_H
#  include <memory.h>
# endif
# include <string.h>
#endif
#if HAVE_STRINGS_H
# include <strings.h>
#endif
#if HAVE_INTTYPES_H
# include <inttypes.h>
#else
# if HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif

#if !defined(DVBCSA_DEBUG) && defined(__GNUC__)
#define DVBCSA_INLINE __attribute__ ((always_inline))
#else
#define DVBCSA_INLINE
#endif

/** types used for type-punning */
#ifdef HAVE_ATTRIBUTE_MAY_ALIAS
typedef uint8_t  __attribute__((may_alias)) dvbcsa_u8_aliasing_t;
typedef uint16_t __attribute__((may_alias)) dvbcsa_u16_aliasing_t;
typedef uint32_t __attribute__((may_alias)) dvbcsa_u32_aliasing_t;
typedef uint64_t __attribute__((may_alias)) dvbcsa_u64_aliasing_t;
#else
typedef uint8_t  dvbcsa_u8_aliasing_t;
typedef uint16_t dvbcsa_u16_aliasing_t;
typedef uint32_t dvbcsa_u32_aliasing_t;
typedef uint64_t dvbcsa_u64_aliasing_t;
#endif

void worddump (const char *str, const void *data, size_t len, size_t ws);

#define DVBCSA_DATA_SIZE        8
#define DVBCSA_KEYSBUFF_SIZE    56
#define DVBCSA_CWBITS_SIZE      64

typedef uint8_t                 dvbcsa_block_t[DVBCSA_DATA_SIZE];
typedef uint8_t                 dvbcsa_keys_t[DVBCSA_KEYSBUFF_SIZE];

struct dvbcsa_key_s
{
  dvbcsa_cw_t           cw;
  dvbcsa_cw_t           cws;    /* nibble swapped CW */
  dvbcsa_keys_t         sch;
};

extern const uint8_t dvbcsa_block_sbox[256];

static const uint8_t            csa_block_perm_ecm[256] =
  {
    0x00,0x02,0x80,0x82,0x20,0x22,0xa0,0xa2, 0x04,0x06,0x84,0x86,0x24,0x26,0xa4,0xa6,
    0x40,0x42,0xc0,0xc2,0x60,0x62,0xe0,0xe2, 0x44,0x46,0xc4,0xc6,0x64,0x66,0xe4,0xe6,
    0x01,0x03,0x81,0x83,0x21,0x23,0xa1,0xa3, 0x05,0x07,0x85,0x87,0x25,0x27,0xa5,0xa7,
    0x41,0x43,0xc1,0xc3,0x61,0x63,0xe1,0xe3, 0x45,0x47,0xc5,0xc7,0x65,0x67,0xe5,0xe7,
    0x08,0x0a,0x88,0x8a,0x28,0x2a,0xa8,0xaa, 0x0c,0x0e,0x8c,0x8e,0x2c,0x2e,0xac,0xae,
    0x48,0x4a,0xc8,0xca,0x68,0x6a,0xe8,0xea, 0x4c,0x4e,0xcc,0xce,0x6c,0x6e,0xec,0xee,
    0x09,0x0b,0x89,0x8b,0x29,0x2b,0xa9,0xab, 0x0d,0x0f,0x8d,0x8f,0x2d,0x2f,0xad,0xaf,
    0x49,0x4b,0xc9,0xcb,0x69,0x6b,0xe9,0xeb, 0x4d,0x4f,0xcd,0xcf,0x6d,0x6f,0xed,0xef,
    0x10,0x12,0x90,0x92,0x30,0x32,0xb0,0xb2, 0x14,0x16,0x94,0x96,0x34,0x36,0xb4,0xb6,
    0x50,0x52,0xd0,0xd2,0x70,0x72,0xf0,0xf2, 0x54,0x56,0xd4,0xd6,0x74,0x76,0xf4,0xf6,
    0x11,0x13,0x91,0x93,0x31,0x33,0xb1,0xb3, 0x15,0x17,0x95,0x97,0x35,0x37,0xb5,0xb7,
    0x51,0x53,0xd1,0xd3,0x71,0x73,0xf1,0xf3, 0x55,0x57,0xd5,0xd7,0x75,0x77,0xf5,0xf7,
    0x18,0x1a,0x98,0x9a,0x38,0x3a,0xb8,0xba, 0x1c,0x1e,0x9c,0x9e,0x3c,0x3e,0xbc,0xbe,
    0x58,0x5a,0xd8,0xda,0x78,0x7a,0xf8,0xfa, 0x5c,0x5e,0xdc,0xde,0x7c,0x7e,0xfc,0xfe,
    0x19,0x1b,0x99,0x9b,0x39,0x3b,0xb9,0xbb, 0x1d,0x1f,0x9d,0x9f,0x3d,0x3f,0xbd,0xbf,
    0x59,0x5b,0xd9,0xdb,0x79,0x7b,0xf9,0xfb, 0x5d,0x5f,0xdd,0xdf,0x7d,0x7f,0xfd,0xff
};

void dvbcsa_block_decrypt (const dvbcsa_keys_t key, const dvbcsa_block_t in, dvbcsa_block_t out);
void dvbcsa_block_encrypt (const dvbcsa_keys_t key, const dvbcsa_block_t in, dvbcsa_block_t out);

void dvbcsa_stream_xor (const dvbcsa_cw_t cw, const dvbcsa_block_t iv,
                        uint8_t *stream, unsigned int len);

void dvbcsa_key_schedule_block(const dvbcsa_cw_t cw, uint8_t * kk);
void dvbcsa_key_schedule_block_ecm(const unsigned char ecm, const dvbcsa_cw_t cw, uint8_t * kk);

DVBCSA_INLINE static inline void
dvbcsa_xor_64 (uint8_t *b, const uint8_t *a)
{
  uint64_t ia, ib;
  memcpy(&ia, a, 8);
  memcpy(&ib, b, 8);
  ib ^= ia;
  memcpy(b, &ib, 8);
}

DVBCSA_INLINE static inline uint32_t
dvbcsa_load_le32(const uint8_t *p)
{
#if defined(DVBCSA_ENDIAN_LITTLE)
  uint32_t i;
  memcpy(&i, p, 4);
  return i;
#else
  return ((uint32_t)p[3] << 24) |
         ((uint32_t)p[2] << 16) |
         ((uint32_t)p[1] << 8 ) |
          (uint32_t)p[0];
#endif
}

DVBCSA_INLINE static inline uint64_t
dvbcsa_load_le64(const uint8_t *p)
{
#if defined(DVBCSA_ENDIAN_LITTLE)
  uint64_t i;
  memcpy(&i, p, 8);
  return i;
#else
  return (uint64_t)( ((uint64_t)p[7] << 56) |
                     ((uint64_t)p[6] << 48) |
                     ((uint64_t)p[5] << 40) |
                     ((uint64_t)p[4] << 32) |
                     ((uint64_t)p[3] << 24) |
                     ((uint64_t)p[2] << 16) |
                     ((uint64_t)p[1] << 8 ) |
                      (uint64_t)p[0]
                     );
#endif
}

DVBCSA_INLINE static inline uint64_t
dvbcsa_load_le64_ecm(const unsigned char ecm, const uint8_t *p)
{
  dvbcsa_block_t W;
  memcpy(W, p, sizeof(W));
  if (ecm == 4)
  {
    W[0] = csa_block_perm_ecm[p[0]];
    W[4] = csa_block_perm_ecm[p[4]];
  }
#if defined(DVBCSA_ENDIAN_LITTLE)
  uint64_t i;
  memcpy(&i, W, 8);
  return i;
#else
  return (uint64_t)( ((uint64_t)W[7] << 56) |
                     ((uint64_t)W[6] << 48) |
                     ((uint64_t)W[5] << 40) |
                     ((uint64_t)W[4] << 32) |
                     ((uint64_t)W[3] << 24) |
                     ((uint64_t)W[2] << 16) |
                     ((uint64_t)W[1] << 8 ) |
                      (uint64_t)W[0]
                     );
#endif
}

DVBCSA_INLINE static inline void
dvbcsa_store_le64(uint8_t *p, const uint64_t w)
{
#if defined(DVBCSA_ENDIAN_LITTLE)
  memcpy(p, &w, 8);
#else
  p[7] = (w >> 56);
  p[6] = (w >> 48);
  p[5] = (w >> 40);
  p[4] = (w >> 32);
  p[3] = (w >> 24);
  p[2] = (w >> 16);
  p[1] = (w >> 8);
  p[0] = (w);
#endif
}


#endif

