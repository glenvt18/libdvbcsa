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

#include "dvbcsa/dvbcsa.h"
#include "dvbcsa_bs.h"

#ifdef DVBCSA_USE_ALT_SBOX

#ifdef DVBCSA_ENDIAN_LITTLE
#define BLOCK_ALT_SBOX_LOAD(src, a, b, c, d) \
    a = (src) & 0xff; \
    b = ((src) >>  8) & 0xff; \
    c = ((src) >> 16) & 0xff; \
    d = ((src) >> 24) & 0xff;
#else
#define BLOCK_ALT_SBOX_LOAD(src, a, b, c, d) \
    a = ((src) >> 24) & 0xff; \
    b = ((src) >> 16) & 0xff; \
    c = ((src) >>  8) & 0xff; \
    d = (src) & 0xff;
#endif

#endif

/* SIMD targets which support loading streams of two interleaved bytes */
#ifdef BS_LOAD_DEINTERLEAVE_8

/*
  sbox + permute table - both values at one look-up

  LSB: sbox output
  MSB: permuted sbox output
*/
static const uint16_t dvbcsa_block_sbox_perm[256] =
  {
    0xd43a, 0xd9ea, 0x5168, 0xfdfe, 0xc633, 0x5be9, 0x1888, 0x941a,
    0x8a83, 0xbbcf, 0x4be1, 0xf77f, 0xdcba, 0xc9e2, 0x5438, 0x8412,
    0x59e8, 0xe227, 0x4361, 0x2e95, 0x300c, 0xe436, 0x6be5, 0x4570,
    0xc8a2, 0xa006, 0x8882, 0x757c, 0xa617, 0xcaa3, 0xe026, 0x1349,
    0xfcbe, 0xd57a, 0x736d, 0xa347, 0x0bc1, 0x0751, 0xba8f, 0xcff3,
    0x39cc, 0x975b, 0xe367, 0x7ebd, 0x3bcd, 0x1418, 0x1008, 0x1bc9,
    0xffff, 0x5369, 0xfbef, 0x8203, 0xb14e, 0x1148, 0x914a, 0x2884,
    0xf63f, 0x6cb4, 0x0410, 0x2004, 0x3ddc, 0x6ff5, 0x355c, 0xa9c6,
    0xa416, 0xdaab, 0x78ac, 0x314c, 0x4ff1, 0xd16a, 0xf22f, 0x743c,
    0xd63b, 0x2dd4, 0x2fd5, 0x2c94, 0x0dd0, 0x29c4, 0xc363, 0xc162,
    0x4771, 0x4aa1, 0x5ff9, 0xb34f, 0xf02e, 0xd8aa, 0x2bc5, 0xa556,
    0xcbe3, 0x5639, 0x8e93, 0xb9ce, 0x6365, 0x6164, 0x69e4, 0x1558,
    0x716c, 0x1619, 0x8142, 0x5779, 0x3fdd, 0xf9ee, 0xac96, 0xedf6,
    0x988a, 0x79ec, 0xb41e, 0x2a85, 0x8753, 0x2345, 0xbdde, 0xdebb,
    0xf57e, 0x900a, 0x9c9a, 0x8613, 0xd02a, 0x3e9d, 0x89c2, 0xb55e,
    0x955a, 0xb61f, 0xc432, 0x6635, 0x3c9c, 0x58a8, 0xc773, 0x4430,
    0x5229, 0x763d, 0xebe7, 0x8c92, 0xaa87, 0x961b, 0xd22b, 0x934b,
    0x6aa5, 0xa757, 0xae97, 0x0140, 0x2615, 0xe9e6, 0x7cbc, 0xb00e,
    0xdbeb, 0x8bc3, 0x6434, 0x722d, 0x5cb8, 0x2144, 0x6225, 0x68a4,
    0x341c, 0xabc7, 0xc223, 0x7bed, 0x0c90, 0xf16e, 0x0550, 0x0000,
    0x1e99, 0xbc9e, 0x334d, 0x1fd9, 0x9dda, 0x3a8d, 0xf36f, 0xb75f,
    0xf43e, 0xafd7, 0x4221, 0x6574, 0xa886, 0xbfdf, 0xd36b, 0x2205,
    0xb88e, 0x375d, 0xe637, 0x0611, 0x8dd2, 0x5028, 0x6775, 0xadd6,
    0xeaa7, 0xe777, 0x6024, 0xfebf, 0x4df0, 0x4cb0, 0x8002, 0xeeb7,
    0x5df8, 0x7dfc, 0x0a81, 0x1209, 0x4eb1, 0x0201, 0xe576, 0x0e91,
    0x777d, 0xb20f, 0x19c8, 0x48a0, 0xcdf2, 0x9bcb, 0x5578, 0x4160,
    0x0fd1, 0xeff7, 0x49e0, 0x6eb5, 0x1c98, 0xc022, 0xceb3, 0x4020,
    0x361d, 0xe8a6, 0x9fdb, 0xd77b, 0x1759, 0xbe9f, 0xf8ae, 0x4631,
    0xdffb, 0x8fd3, 0xecb6, 0x99ca, 0x8343, 0xc572, 0xa207, 0x6df4,
    0x1dd8, 0x0341, 0x2414, 0x2755, 0x320d, 0x2554, 0x9a8b, 0x5eb9,
    0x7aad, 0xa146, 0x920b, 0xfaaf, 0x0880, 0x8552, 0x702c, 0xddfa,
    0x388c, 0x1a89, 0xe166, 0x7ffd, 0xccb2, 0x5aa9, 0x9e9b, 0x09c0,
  };

#ifdef DVBCSA_USE_ALT_SBOX

#define BLOCK_SBOX_PERMUTE(in_buf, out_buf) \
    { \
    dvbcsa_u32_aliasing_t *src = (dvbcsa_u32_aliasing_t *)in_buf; \
    dvbcsa_u16_aliasing_t *dst = (dvbcsa_u16_aliasing_t *)out_buf; \
    uint32_t a, b, c, d; \
    uint32_t s; \
    int j; \
    s = *src; \
    for (j = 0; j < BS_BATCH_BYTES/4 * 8 - 1;) \
      { \
        j++; \
        BLOCK_ALT_SBOX_LOAD(s, a, b, c, d) \
        s = src[j]; \
        dst[0] = dvbcsa_block_sbox_perm[a]; \
        dst[1] = dvbcsa_block_sbox_perm[b]; \
        dst[2] = dvbcsa_block_sbox_perm[c]; \
        dst[3] = dvbcsa_block_sbox_perm[d]; \
        dst += 4; \
      } \
      BLOCK_ALT_SBOX_LOAD(s, a, b, c, d) \
      dst[0] = dvbcsa_block_sbox_perm[a]; \
      dst[1] = dvbcsa_block_sbox_perm[b]; \
      dst[2] = dvbcsa_block_sbox_perm[c]; \
      dst[3] = dvbcsa_block_sbox_perm[d]; \
    }

#else

#ifndef BLOCK_SBOX_PERMUTE

#define BLOCK_SBOX_PERMUTE(in_buf, out_buf) \
    { \
    dvbcsa_u8_aliasing_t *src = (dvbcsa_u8_aliasing_t *)in_buf; \
    dvbcsa_u16_aliasing_t *dst = (dvbcsa_u16_aliasing_t *)out_buf; \
    uint8_t a, b, c, d; \
    int j; \
    for (j = 0; j < BS_BATCH_BYTES * 8; j += 4) \
      { \
        a = src[j + 0]; \
        b = src[j + 1]; \
        c = src[j + 2]; \
        d = src[j + 3]; \
        dst[j + 0] = dvbcsa_block_sbox_perm[a]; \
        dst[j + 1] = dvbcsa_block_sbox_perm[b]; \
        dst[j + 2] = dvbcsa_block_sbox_perm[c]; \
        dst[j + 3] = dvbcsa_block_sbox_perm[d]; \
      } \
    }

#endif /* ifndef BLOCK_SBOX_PERMUTE */

#endif /* USE_ALT_SBOX */

#else /* no BS_LOAD_DEINTERLEAVE_8 */

#ifdef DVBCSA_USE_ALT_SBOX

#define BLOCK_SBOX(in_buf, out_buf) \
    { \
    dvbcsa_u32_aliasing_t *src = (dvbcsa_u32_aliasing_t *)in_buf; \
    dvbcsa_u8_aliasing_t *dst = (dvbcsa_u8_aliasing_t *)out_buf; \
    uint32_t a, b, c, d; \
    uint32_t s; \
    int j; \
    s = *src; \
    for (j = 0; j < BS_BATCH_BYTES/4 * 8 - 1;) \
      { \
        j++; \
        BLOCK_ALT_SBOX_LOAD(s, a, b, c, d) \
        s = src[j]; \
        dst[0] = dvbcsa_block_sbox[a]; \
        dst[1] = dvbcsa_block_sbox[b]; \
        dst[2] = dvbcsa_block_sbox[c]; \
        dst[3] = dvbcsa_block_sbox[d]; \
        dst += 4; \
      } \
      BLOCK_ALT_SBOX_LOAD(s, a, b, c, d) \
      dst[0] = dvbcsa_block_sbox[a]; \
      dst[1] = dvbcsa_block_sbox[b]; \
      dst[2] = dvbcsa_block_sbox[c]; \
      dst[3] = dvbcsa_block_sbox[d]; \
    }

#else

#define BLOCK_SBOX(in_buf, out_buf) \
    { \
    dvbcsa_u8_aliasing_t *src = (dvbcsa_u8_aliasing_t *)in_buf; \
    dvbcsa_u8_aliasing_t *dst = (dvbcsa_u8_aliasing_t *)out_buf; \
    uint8_t a, b, c, d; \
    int j; \
    for (j = 0; j < BS_BATCH_BYTES * 8; j += 4) \
      { \
        a = src[j + 0]; \
        b = src[j + 1]; \
        c = src[j + 2]; \
        d = src[j + 3]; \
        dst[j + 0] = dvbcsa_block_sbox[a]; \
        dst[j + 1] = dvbcsa_block_sbox[b]; \
        dst[j + 2] = dvbcsa_block_sbox[c]; \
        dst[j + 3] = dvbcsa_block_sbox[d]; \
      } \
    }

#endif /* DVBCSA_USE_ALT_SBOX */

#define BLOCK_PERMUTE_LOGIC(in, out) \
    { \
    out = BS_OR( \
        BS_OR( \
              BS_OR (BS_SHL (BS_AND (in, BS_VAL8(29)), 1), \
                     BS_SHL (BS_AND (in, BS_VAL8(02)), 6)), \
              BS_OR (BS_SHL (BS_AND (in, BS_VAL8(04)), 3), \
                     BS_SHR (BS_AND (in, BS_VAL8(10)), 2))), \
        BS_OR(       BS_SHR (BS_AND (in, BS_VAL8(40)), 6), \
                     BS_SHR (BS_AND (in, BS_VAL8(80)), 4))); \
    }

#endif /* BS_LOAD_DEINTERLEAVE_8 */

DVBCSA_INLINE static inline void
dvbcsa_bs_block_decrypt_register (const dvbcsa_bs_word_t *block, dvbcsa_bs_word_t *r)
{
  dvbcsa_bs_word_t scratch1[8];
#ifdef BS_LOAD_DEINTERLEAVE_8
  dvbcsa_bs_word_t scratch2[8 * 2];
#else
  dvbcsa_bs_word_t scratch2[8];
#endif
  int i, g;

  r += 8 * 56;

  /* loop over kk[55]..kk[0] */
  for (i = 55; i >= 0; i--)
    {
      dvbcsa_bs_word_t *r6_N = r + 8 * 6;

      r -= 8;   /* virtual shift of registers */

      for (g = 0; g < 8; g++)
        scratch1[g] = BS_XOR(block[i], r6_N[g]);

#ifdef BS_LOAD_DEINTERLEAVE_8
      /* sbox + bit permutation */
      BLOCK_SBOX_PERMUTE(scratch1, scratch2);
#else
      /* only sbox */
      BLOCK_SBOX(scratch1, scratch2);
#endif

      for (g = 0; g < 8; g++)
        {
          dvbcsa_bs_word_t sbox_out, perm_out, w, tmp1, tmp2, tmp3, tmp4;

#ifdef BS_LOAD_DEINTERLEAVE_8
          BS_LOAD_DEINTERLEAVE_8(scratch2 + g * 2, sbox_out, perm_out);
#else
          sbox_out = scratch2[g];
          BLOCK_PERMUTE_LOGIC(sbox_out, perm_out);
#endif
          /*
              w = r[8 * 8 + g] ^ sbox_out;
              r[8 * 0 + g] = w;
              r[8 * 2 + g] ^= w;
              r[8 * 3 + g] ^= w;
              r[8 * 4 + g] ^ =w;
              r[8 * 6 + g] ^= perm_out;
          */

          w = r[8 * 8 + g];
          tmp1 = r[8 * 2 + g];
          tmp2 = r[8 * 3 + g];
          tmp3 = r[8 * 4 + g];
          w = BS_XOR(w, sbox_out);
          tmp4 = r[8 * 6 + g];
          tmp1 = BS_XOR(tmp1, w);
          tmp2 = BS_XOR(tmp2, w);
          tmp3 = BS_XOR(tmp3, w);
          r[8 * 0 + g] = w;
          r[8 * 2 + g] = tmp1;
          tmp4 = BS_XOR(tmp4, perm_out);
          r[8 * 3 + g] = tmp2;
          r[8 * 4 + g] = tmp3;
          r[8 * 6 + g] = tmp4;
        }
    }
}

DVBCSA_INLINE static inline void
dvbcsa_bs_block_decrypt_block(const struct dvbcsa_bs_key_s *key,
                              struct dvbcsa_bs_pkt_buf *pkt_buf,
                              unsigned int offset)
{
  dvbcsa_bs_word_t r[8 * (8 + 56)];

  dvbcsa_bs_block_transpose_in(r + 8 * 56, pkt_buf, offset);
  dvbcsa_bs_block_decrypt_register(key->block, r);
  dvbcsa_bs_block_transpose_out_and_xor(r, pkt_buf, offset, 0);
}

void dvbcsa_bs_block_decrypt_batch(const struct dvbcsa_bs_key_s *key,
                                   struct dvbcsa_bs_pkt_buf *pkt_buf,
                                   unsigned int maxlen)
{
  unsigned int i;

  for (i = 0; i < maxlen; i += 8)
    dvbcsa_bs_block_decrypt_block(key, pkt_buf, i);
}

DVBCSA_INLINE static inline void
dvbcsa_bs_block_encrypt_register (const dvbcsa_bs_word_t *block, dvbcsa_bs_word_t *r)
{
  dvbcsa_bs_word_t scratch1[8];
#ifdef BS_LOAD_DEINTERLEAVE_8
  dvbcsa_bs_word_t scratch2[8 * 2];
#else
  dvbcsa_bs_word_t scratch2[8];
#endif
  int i, g;

  /* loop over kk[55]..kk[0] */
  for (i = 0; i < 56; i++)
    {
      dvbcsa_bs_word_t *r7_N = r + 8 * 7;

      r += 8;   /* virtual shift of registers */

      for (g = 0; g < 8; g++)
         scratch1[g] = BS_XOR(block[i], r7_N[g]);

#ifdef BS_LOAD_DEINTERLEAVE_8
      /* sbox + bit permutation */
      BLOCK_SBOX_PERMUTE(scratch1, scratch2);
#else
      /* only sbox */
      BLOCK_SBOX(scratch1, scratch2);
#endif

      for (g = 0; g < 8; g++)
        {
          dvbcsa_bs_word_t sbox_out, perm_out, w, tmp1, tmp2, tmp3, tmp4;

#ifdef BS_LOAD_DEINTERLEAVE_8
          BS_LOAD_DEINTERLEAVE_8(scratch2 + g * 2, sbox_out, perm_out);
#else
          sbox_out = scratch2[g];
          BLOCK_PERMUTE_LOGIC(sbox_out, perm_out);
#endif
          /*
              w = r[-8 * 1 + g];
              r[8 * 7 + g] = w ^ sbox_out;
              r[8 * 1 + g] ^= w;
              r[8 * 2 + g] ^= w;
              r[8 * 3 + g] ^ =w;
              r[8 * 5 + g] ^= perm_out;
          */

          w = r[-8 * 1 + g];
          tmp1 = r[8 * 1 + g];
          tmp2 = r[8 * 2 + g];
          tmp3 = r[8 * 3 + g];
          sbox_out = BS_XOR(sbox_out, w);
          tmp4 = r[8 * 5 + g];
          tmp1 = BS_XOR(tmp1, w);
          tmp2 = BS_XOR(tmp2, w);
          tmp3 = BS_XOR(tmp3, w);
          r[8 * 7 + g] = sbox_out;
          r[8 * 1 + g] = tmp1;
          tmp4 = BS_XOR(tmp4, perm_out);
          r[8 * 2 + g] = tmp2;
          r[8 * 3 + g] = tmp3;
          r[8 * 5 + g] = tmp4;
        }
    }
}

DVBCSA_INLINE static inline void
dvbcsa_bs_block_encrypt_block(const struct dvbcsa_bs_key_s *key,
                              struct dvbcsa_bs_pkt_buf *pkt_buf,
                              unsigned int offset)
{
  dvbcsa_bs_word_t r[8 * (8 + 56)];

  dvbcsa_bs_block_transpose_in(r, pkt_buf, offset);
  dvbcsa_bs_block_encrypt_register(key->block, r);
  dvbcsa_bs_block_transpose_out_and_xor(r + 8 * 56, pkt_buf, offset, 1);
}

void dvbcsa_bs_block_encrypt_batch(const struct dvbcsa_bs_key_s *key,
                                   struct dvbcsa_bs_pkt_buf *pkt_buf,
                                   unsigned int maxlen)
{
  int   i;

  for (i = maxlen - 8; i >= 0; i -= 8)
    dvbcsa_bs_block_encrypt_block(key, pkt_buf, i);
}

