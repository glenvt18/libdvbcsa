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
#include "dvbcsa_bs_transpose.h"

/*
  Block cipher transpose
*/

DVBCSA_INLINE static inline void 
dvbcsa_bs_matrix_transpose_block(dvbcsa_bs_word_t *row)
{
  int j;

  for (j = 0; j < 64; j += 32)
    {
      int i;
      for (i = 0; i < 8; i++)
        {
          dvbcsa_bs_word_t a0, a1, a2, a3;

          a0 = row[j + i + 0];
          a1 = row[j + i + 8];
          a2 = row[j + i + 16];
          a3 = row[j + i + 24];

          BS_SWAP8(a0, a1);
          BS_SWAP8(a2, a3);
          BS_SWAP16(a0, a2);
          BS_SWAP16(a1, a3);

          row[j + i + 0] = a0;
          row[j + i + 8] = a1;
          row[j + i + 16] = a2;
          row[j + i + 24] = a3;
        }
    }
}

void dvbcsa_bs_block_transpose_in(dvbcsa_bs_word_t *out,
                const struct dvbcsa_bs_pkt_buf *pkt_buf,
                unsigned int offset)
{
  dvbcsa_u32_aliasing_t *ri = (dvbcsa_u32_aliasing_t *)out;
  const dvbcsa_bs_block8_t *block = pkt_buf->data + offset / 8;
  int i;

  int n_pkt4 = pkt_buf->n_packets & (unsigned)~0x3;
  for (i = 0; i < n_pkt4;)
    {
      uint32_t a, b, c, d;
      DVBCSA_UNROLL2(
        a = block[0].u32[0];
        b = block[0].u32[1];
        c = block[BS_PKT_BLOCKS8].u32[0];
        d = block[BS_PKT_BLOCKS8].u32[1];
        ri[i] = a;
        ri[i + BS_BATCH_SIZE] = b;
        ri[i + 1] = c;
        ri[i + 1 + BS_BATCH_SIZE] = d;
        i += 2;
        block += BS_PKT_BLOCKS8 * 2;
      )
    }
  for (; i < pkt_buf->n_packets; i++, block += BS_PKT_BLOCKS8)
    {
      ri[i] = block->u32[0];
      ri[i + BS_BATCH_SIZE] = block->u32[1];
    }

  dvbcsa_bs_matrix_transpose_block(out);
}

/* output transpose and chained cipher XOR 2-in-1 */

void dvbcsa_bs_block_transpose_out_and_xor(dvbcsa_bs_word_t *in,
                struct dvbcsa_bs_pkt_buf *pkt_buf,
                unsigned int offset, int encrypt)
{
  dvbcsa_u32_aliasing_t *ri = (dvbcsa_u32_aliasing_t *)in;
  dvbcsa_bs_block8_t *block = pkt_buf->data + offset / 8;
  int i;

  dvbcsa_bs_matrix_transpose_block(in);

  if (offset == 0)
    {
      for (i = 0; i < pkt_buf->n_packets; i++, block += BS_PKT_BLOCKS8)
        if (pkt_buf->len8[i])
          {
            block->u32[0] = ri[i];
            block->u32[1] = ri[i + BS_BATCH_SIZE];
          }
    }
  else if (encrypt)
    {
      int n_pkt4 = pkt_buf->n_packets & (unsigned)~0x3;
      for (i = 0; i < n_pkt4;)
        {
          DVBCSA_UNROLL4(
            if (offset < pkt_buf->len8[i])
              {
                dvbcsa_bs_block8_t b;
                b.u32[0] = ri[i];
                b.u32[1] = ri[i + BS_BATCH_SIZE];
                block[-1].u64 ^= b.u64;
                block[0].u64 = b.u64;
              }
            i++;
            block += BS_PKT_BLOCKS8;
          )
        }
      for (; i < pkt_buf->n_packets; i++, block += BS_PKT_BLOCKS8)
        if (offset < pkt_buf->len8[i])
          {
            dvbcsa_bs_block8_t b;
            b.u32[0] = ri[i];
            b.u32[1] = ri[i + BS_BATCH_SIZE];
            block[-1].u64 ^= b.u64;
            block[0].u64 = b.u64;
          }
    }
  else
    {
      int n_pkt4 = pkt_buf->n_packets & (unsigned)~0x3;
      for (i = 0; i < n_pkt4;)
        {
          DVBCSA_UNROLL4(
            if (offset < pkt_buf->len8[i])
              {
                dvbcsa_bs_block8_t b;
                b.u32[0] = ri[i];
                b.u32[1] = ri[i + BS_BATCH_SIZE];
                block[-1].u64 ^= block[0].u64;
                block[0].u64 = b.u64;
              }
            i++;
            block += BS_PKT_BLOCKS8;
          )
        }
      for (; i < pkt_buf->n_packets; i++, block += BS_PKT_BLOCKS8)
        if (offset < pkt_buf->len8[i])
          {
            dvbcsa_bs_block8_t b;
            b.u32[0] = ri[i];
            b.u32[1] = ri[i + BS_BATCH_SIZE];
            block[-1].u64 ^= block[0].u64;
            block[0].u64 = b.u64;
          }
    }
}
