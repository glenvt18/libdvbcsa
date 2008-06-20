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

static void
dvbcsa_bs_key_schedule_stream (const dvbcsa_cw_t ck,	// [In]  ck[0]-ck[7]   8 bytes   | Key.
			       uint32_t *iA,	// [Out] iA[0]-iA[7]   8 nibbles | Key schedule.
			       uint32_t *iB)	// [Out] iB[0]-iB[7]   8 nibbles | Key schedule.
{
  iA[0] = (ck[0] >> 4) & 0xf;
  iA[1] = (ck[0]) & 0xf;
  iA[2] = (ck[1] >> 4) & 0xf;
  iA[3] = (ck[1]) & 0xf;
  iA[4] = (ck[2] >> 4) & 0xf;
  iA[5] = (ck[2]) & 0xf;
  iA[6] = (ck[3] >> 4) & 0xf;
  iA[7] = (ck[3]) & 0xf;
  iB[0] = (ck[4] >> 4) & 0xf;
  iB[1] = (ck[4]) & 0xf;
  iB[2] = (ck[5] >> 4) & 0xf;
  iB[3] = (ck[5]) & 0xf;
  iB[4] = (ck[6] >> 4) & 0xf;
  iB[5] = (ck[6]) & 0xf;
  iB[6] = (ck[7] >> 4) & 0xf;
  iB[7] = (ck[7]) & 0xf;
}

static void
dvbcsa_bs_key_schedule_block (const dvbcsa_cw_t ck,	// [In]  ck[0]-ck[7]   8 bytes | Key.
			      uint8_t * kk)	// [Out] kk[0]-kk[55] 56 bytes | Key schedule.
{
  static const uint8_t key_perm[0x40] = {
    0x12, 0x24, 0x09, 0x07, 0x2A, 0x31, 0x1D, 0x15,
    0x1C, 0x36, 0x3E, 0x32, 0x13, 0x21, 0x3B, 0x40,
    0x18, 0x14, 0x25, 0x27, 0x02, 0x35, 0x1B, 0x01,
    0x22, 0x04, 0x0D, 0x0E, 0x39, 0x28, 0x1A, 0x29,
    0x33, 0x23, 0x34, 0x0C, 0x16, 0x30, 0x1E, 0x3A,
    0x2D, 0x1F, 0x08, 0x19, 0x17, 0x2F, 0x3D, 0x11,
    0x3C, 0x05, 0x38, 0x2B, 0x0B, 0x06, 0x0A, 0x2C,
    0x20, 0x3F, 0x2E, 0x0F, 0x03, 0x26, 0x10, 0x37,
  };

  int i, j, k;
  int bit[64];
  int newbit[64];
  int kb[7][8];

  // 56 steps
  // 56 key bytes kk(55)..kk(0) by key schedule from ck

  // kb(6,0) .. kb(6,7) = ck(0) .. ck(7)
  kb[6][0] = ck[0];
  kb[6][1] = ck[1];
  kb[6][2] = ck[2];
  kb[6][3] = ck[3];
  kb[6][4] = ck[4];
  kb[6][5] = ck[5];
  kb[6][6] = ck[6];
  kb[6][7] = ck[7];

  // calculate kb[5] .. kb[0]
  for (i = 5; i >= 0; i--)
    {
      // 64 bit perm on kb
      for (j = 0; j < 8; j++)
	{
	  for (k = 0; k < 8; k++)
	    {
	      bit[j * 8 + k] = (kb[i + 1][j] >> (7 - k)) & 1;
	      newbit[key_perm[j * 8 + k] - 1] = bit[j * 8 + k];
	    }
	}
      for (j = 0; j < 8; j++)
	{
	  kb[i][j] = 0;
	  for (k = 0; k < 8; k++)
	    {
	      kb[i][j] |= newbit[j * 8 + k] << (7 - k);
	    }
	}
    }

  // xor to give kk
  for (i = 0; i < 7; i++)
    {
      for (j = 0; j < 8; j++)
	{
	  kk[i * 8 + j] = kb[i][j] ^ i;
	}
    }

}

void
dvbcsa_bs_key_set (const dvbcsa_cw_t cw, struct dvbcsa_bs_key_s *key)
{
  // could be made faster, but is not run often
  int bi, by;
  int i, j;

  // key
  memcpy (key->ck, cw, 8);

  // precalculations for stream

  dvbcsa_bs_key_schedule_stream (key->ck, key->iA, key->iB);

  for (by = 0; by < 8; by++)
    for (bi = 0; bi < 8; bi++)
      key->ck_g[by][bi] = (key->ck[by] & (1 << bi)) ? BS_VAL8(ff) : BS_VAL8(00);

  for (by = 0; by < 8; by++)
    for (bi = 0; bi < 4; bi++)
      {
	key->iA_g[by][bi] =
	  (key->iA[by] & (1 << bi)) ? BS_VAL8(ff) : BS_VAL8(00);
	key->iB_g[by][bi] =
	  (key->iB[by] & (1 << bi)) ? BS_VAL8(ff) : BS_VAL8(00);
      }

  // precalculations for block

  dvbcsa_bs_key_schedule_block (key->ck, key->kk);

  for (i = 0; i < 56; i++)
    for (j = 0; j < BS_BATCH_BYTES; j++)
      *(((uint8_t *) & key->kkmulti[i]) + j) = key->kk[i];
}

