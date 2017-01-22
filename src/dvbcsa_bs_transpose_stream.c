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
  Stream cipher transpose for dvbcsa_bs_word_t sizes = 64, 128, 256, ... bits.
*/

static void dvbcsa_bs_matrix_transpose_64x(dvbcsa_bs_word_t *row)
{
  int j;

#if defined(__i386__)

  /* short of registers  */

  for (j = 0; j < 16; j++)
    {
      dvbcsa_bs_word_t a0, a1, a2, a3;
  
      a0 = row[j + 0];
      a1 = row[j + 16];
      a2 = row[j + 32];
      a3 = row[j + 48];

      BS_SWAP16(a0, a1);
      BS_SWAP16(a2, a3);
      BS_SWAP32(a0, a2);
      BS_SWAP32(a1, a3);

      row[j + 0]  = a0;
      row[j + 16] = a1;
      row[j + 32] = a2;
      row[j + 48] = a3;
    }

  for (j = 0; j < 64; j += 16)
    {
      int i;
      for (i = 0; i < 4; i++)
        {
          dvbcsa_bs_word_t a0, a1, a2, a3;

          a0 = row[j + i + 0];
          a1 = row[j + i + 4];
          a2 = row[j + i + 8];
          a3 = row[j + i + 12];

          BS_SWAP4(a0, a1);
          BS_SWAP4(a2, a3);
          BS_SWAP8(a0, a2);
          BS_SWAP8(a1, a3);

          row[j + i + 0] = a0;
          row[j + i + 4] = a1;
          row[j + i + 8] = a2;
          row[j + i + 12] = a3;
        }
    }

  for (j = 0; j < 64; j += 4)
    {
      dvbcsa_bs_word_t a0, a1, a2, a3;
      
      a0 = row[j + 0];
      a1 = row[j + 1];
      a2 = row[j + 2];
      a3 = row[j + 3];

      BS_SWAP1(a0, a1);
      BS_SWAP1(a2, a3);
      BS_SWAP2(a0, a2);
      BS_SWAP2(a1, a3);

      row[j + 0] = a0;
      row[j + 1] = a1;
      row[j + 2] = a2;
      row[j + 3] = a3;
    }

#else

  for (j = 0; j < 64; j += 8)
    {
      dvbcsa_bs_word_t a0, a1, a2, a3, b0, b1, b2, b3;
      
      a0 = row[j + 0];
      a1 = row[j + 1];
      a2 = row[j + 2];
      a3 = row[j + 3];

      b0 = row[j + 4];
      b1 = row[j + 5];
      b2 = row[j + 6];
      b3 = row[j + 7];

      BS_SWAP1(a0, a1);
      BS_SWAP1(a2, a3);
      BS_SWAP1(b0, b1);
      BS_SWAP1(b2, b3);

      BS_SWAP2(a0, a2);
      BS_SWAP2(a1, a3);
      BS_SWAP2(b0, b2);
      BS_SWAP2(b1, b3);

      BS_SWAP4(a0, b0);
      BS_SWAP4(a1, b1);
      BS_SWAP4(a2, b2);
      BS_SWAP4(a3, b3);

      row[j + 0] = a0;
      row[j + 1] = a1;
      row[j + 2] = a2;
      row[j + 3] = a3;

      row[j + 4] = b0;
      row[j + 5] = b1;
      row[j + 6] = b2;
      row[j + 7] = b3;
    }

  for (j = 0; j < 8; j++)
    {
      dvbcsa_bs_word_t a0, a1, a2, a3, b0, b1, b2, b3;

      a0 = row[j + 0];
      a1 = row[j + 8];
      a2 = row[j + 16];
      a3 = row[j + 24];

      b0 = row[j + 32];
      b1 = row[j + 40];
      b2 = row[j + 48];
      b3 = row[j + 56];

      BS_SWAP8(a0, a1);
      BS_SWAP8(a2, a3);
      BS_SWAP8(b0, b1);
      BS_SWAP8(b2, b3);

      BS_SWAP16(a0, a2);
      BS_SWAP16(a1, a3);
      BS_SWAP16(b0, b2);
      BS_SWAP16(b1, b3);

      BS_SWAP32(a0, b0);
      BS_SWAP32(a1, b1);
      BS_SWAP32(a2, b2);
      BS_SWAP32(a3, b3);

      row[j + 0]  = a0;
      row[j + 8]  = a1;
      row[j + 16] = a2;
      row[j + 24] = a3;
                      
      row[j + 32] = b0;
      row[j + 40] = b1;
      row[j + 48] = b2;
      row[j + 56] = b3;
    }

#endif
}

void dvbcsa_bs_stream_transpose_in(const struct dvbcsa_bs_pkt_buf *pkt_buf,
                                   dvbcsa_bs_word_t *row)
{
  dvbcsa_u64_aliasing_t *p = (dvbcsa_u64_aliasing_t *)row;
  const dvbcsa_bs_block8_t *block = pkt_buf->data;
  int i;

  for (i = 0; i < pkt_buf->n_packets; i++)
    {
      *p++ = block->u64;
      block += BS_PKT_BLOCKS8;
    }

  dvbcsa_bs_matrix_transpose_64x(row);
}

void dvbcsa_bs_stream_transpose_out(struct dvbcsa_bs_pkt_buf *pkt_buf,
                                   unsigned int index, dvbcsa_bs_word_t *row)
{
  dvbcsa_u64_aliasing_t *p = (dvbcsa_u64_aliasing_t *)row;
  dvbcsa_bs_block8_t *block = pkt_buf->data + index / 8;
  int i, n_pkt2;

  dvbcsa_bs_matrix_transpose_64x(row);

  n_pkt2 = pkt_buf->n_packets & (unsigned)~0x1;
  for (i = 0; i < n_pkt2;)
    {
      uint64_t a, b, c, d;
      a = block[0].u64;
      b = p[i + 0];
      c = block[BS_PKT_BLOCKS8].u64;
      d = p[i + 1];
      block[0].u64 = a ^ b;
      block[BS_PKT_BLOCKS8].u64 = c ^ d;
      block += BS_PKT_BLOCKS8 * 2;
      i += 2;
    }
  for (; i < pkt_buf->n_packets; i++)
    {
      block->u64 ^= p[i];
      block += BS_PKT_BLOCKS8;
    }
}

