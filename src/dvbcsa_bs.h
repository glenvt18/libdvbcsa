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

#ifndef DVBCSA_BS_H_
#define DVBCSA_BS_H_

#include "dvbcsa/dvbcsa.h"
#include "dvbcsa_pv.h"

#if defined(DVBCSA_USE_UINT64)
# include "dvbcsa_bs_uint64.h"

#elif defined(DVBCSA_USE_UINT32)
# include "dvbcsa_bs_uint32.h"

#elif defined(DVBCSA_USE_MMX)
# include "dvbcsa_bs_mmx.h"

#elif defined(DVBCSA_USE_SSE)
# include "dvbcsa_bs_sse.h"

#elif defined(DVBCSA_USE_SSSE3)
# include "dvbcsa_bs_sse.h"

#elif defined(DVBCSA_USE_AVX2)
# include "dvbcsa_bs_avx2.h"

#elif defined(DVBCSA_USE_ALTIVEC)
# include "dvbcsa_bs_altivec.h"

#elif defined(DVBCSA_USE_NEON)
# include "dvbcsa_bs_neon.h"

#else
# error No dvbcsa word size defined
#endif

#define DVBCSA_UNROLL2(op...) op op
#define DVBCSA_UNROLL4(op...) DVBCSA_UNROLL2(op) DVBCSA_UNROLL2(op)
#define DVBCSA_UNROLL8(op...) DVBCSA_UNROLL4(op) DVBCSA_UNROLL4(op)
#define DVBCSA_UNROLL16(op...) DVBCSA_UNROLL8(op) DVBCSA_UNROLL8(op)

struct dvbcsa_bs_key_s
{
  dvbcsa_bs_word_t      block[DVBCSA_KEYSBUFF_SIZE];
  dvbcsa_bs_word_t      stream[DVBCSA_CWBITS_SIZE];
};

#if DVBCSA_BS_MAX_PACKET_LEN % 8 != 0
#error DVBCSA_BS_MAX_PACKET_LEN must be a multiple of 8
#endif

#define BS_PKT_BLOCKS8  DVBCSA_BS_MAX_PACKET_LEN / 8

typedef union {
  uint64_t u64;
  uint32_t u32[2];
} dvbcsa_bs_block8_t;

struct dvbcsa_bs_pkt_buf {
  int n_packets;
  unsigned int maxlen;
  unsigned int len8[BS_BATCH_SIZE];
  dvbcsa_bs_block8_t data[BS_BATCH_SIZE * BS_PKT_BLOCKS8];
};

void dvbcsa_bs_stream_cipher_batch(const struct dvbcsa_bs_key_s *key,
                                   struct dvbcsa_bs_pkt_buf *pkt_buf,
                                   unsigned int maxlen);

void dvbcsa_bs_block_decrypt_batch(const struct dvbcsa_bs_key_s *key,
                                   struct dvbcsa_bs_pkt_buf *pkt_buf,
                                   unsigned int maxlen);

void dvbcsa_bs_block_encrypt_batch(const struct dvbcsa_bs_key_s *key,
                                   struct dvbcsa_bs_pkt_buf *pkt_buf,
                                   unsigned int maxlen);

void dvbcsa_bs_block_transpose_in(dvbcsa_bs_word_t *out, const struct dvbcsa_bs_pkt_buf *pkt_buf,
                                   unsigned int offset);

void dvbcsa_bs_block_transpose_out_and_xor(dvbcsa_bs_word_t *in, struct dvbcsa_bs_pkt_buf *pkt_buf,
                                   unsigned int offset, int encrypt);

void dvbcsa_bs_stream_transpose_out(struct dvbcsa_bs_pkt_buf *pkt_buf,
                                   unsigned int index, dvbcsa_bs_word_t *row);

void dvbcsa_bs_stream_transpose_in(const struct dvbcsa_bs_pkt_buf *pkt_buf,
                                   dvbcsa_bs_word_t *row);


#endif

