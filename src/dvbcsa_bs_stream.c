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
#include "dvbcsa_bs_stream_kernel.h"

#define DVBCSA_BS_STREAM_KERNEL_INIT 
#include "dvbcsa_bs_stream_kernel.inc"
#undef DVBCSA_BS_STREAM_KERNEL_INIT 
#include "dvbcsa_bs_stream_kernel.inc"

void
dvbcsa_bs_stream_cipher_batch(const struct dvbcsa_bs_key_s *key,
                              struct dvbcsa_bs_pkt_buf *pkt_buf,
                              unsigned int maxlen)
{
  struct dvbcsa_bs_stream_regs_s regs;

  int i, b;
  unsigned int h;

  dvbcsa_bs_stream_transpose_in(pkt_buf, regs.sb);

  for (b = 0; b < 4; b++)
    {
      for (i = 0; i < 8; i++)
        {
          regs.A[32 + i][b] = key->stream[b + i * 4];
          regs.B[32 + i][b] = key->stream[b + i * 4 + 32];
        }

      // all other regs = 0
      regs.A[32 + 8][b] = BS_VAL8(00);
      regs.A[32 + 9][b] = BS_VAL8(00);
      regs.B[32 + 8][b] = BS_VAL8(00);
      regs.B[32 + 9][b] = BS_VAL8(00);

      regs.X[b] = BS_VAL8(00);
      regs.Y[b] = BS_VAL8(00);
      regs.Z[b] = BS_VAL8(00);
      regs.D[b] = BS_VAL8(00);
      regs.E[b] = BS_VAL8(00);
      regs.F[b] = BS_VAL8(00);
    }

  regs.p = BS_VAL8(00);
  regs.q = BS_VAL8(00);
  regs.r = BS_VAL8(00);

  /* Stream INIT */

  dvbcsa_bs_stream_cipher_kernel_init(&regs);


  /* Stream GEN */

  for (h = 8; h < maxlen; h += 8)
    {
      dvbcsa_bs_stream_cipher_kernel(&regs);
      dvbcsa_bs_stream_transpose_out(pkt_buf, h, regs.sb);
    }

}

