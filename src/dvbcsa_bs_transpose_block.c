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

void dvbcsa_bs_block_transpose_in (dvbcsa_bs_word_t *out,
                const struct dvbcsa_bs_batch_s *pcks,
                unsigned int offset)
{
  uint32_t *ri = (uint32_t *)out;
  unsigned int i;

  for (i = 0; pcks[i].data; i++)
    if (offset < (pcks[i].len & (unsigned)~0x7))
      {
        dvbcsa_copy_32((uint8_t *)(ri + i), pcks[i].data + offset);
        dvbcsa_copy_32((uint8_t *)(ri + i + BS_BATCH_SIZE), pcks[i].data + offset + 4);
      }

  dvbcsa_bs_matrix_transpose_block(out);
}

void dvbcsa_bs_block_transpose_out (dvbcsa_bs_word_t *in,
                const struct dvbcsa_bs_batch_s *pcks,
                unsigned int offset)
{
  uint32_t *ri = (uint32_t *) in;
  unsigned int i;

  dvbcsa_bs_matrix_transpose_block(in);

  for (i = 0; pcks[i].data; i++)
    if (offset < (pcks[i].len & (unsigned)~0x7))
      {
        dvbcsa_copy_32(pcks[i].data + offset, (uint8_t *)(ri + i));
        dvbcsa_copy_32(pcks[i].data + offset + 4, (uint8_t *)(ri + i + BS_BATCH_SIZE));
      }
}

