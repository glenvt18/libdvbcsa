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

#if defined(DVBCSA_DEBUG)
#include <stdio.h>
#endif

#include "dvbcsa/dvbcsa.h"
#include "dvbcsa_bs.h"

#ifdef HAVE_MM_MALLOC
# include <mm_malloc.h>
#endif

#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif

static void dvbcsa_pkt_buf_load(struct dvbcsa_bs_pkt_buf *pkt_buf,
                                const struct dvbcsa_bs_batch_s *pcks,
                                unsigned int maxlen)
{
  int i;
  dvbcsa_bs_block8_t *block = pkt_buf->data;

  for (i = 0; pcks[i].data; i++, block += BS_PKT_BLOCKS8)
    {
      pkt_buf->len8[i] = pcks[i].len & (unsigned)~0x7;
      memcpy(block, pcks[i].data, pcks[i].len);
    }

  pkt_buf->n_packets = i;
  pkt_buf->maxlen = maxlen;
}

static void dvbcsa_pkt_buf_store(const struct dvbcsa_bs_pkt_buf *pkt_buf,
                                const struct dvbcsa_bs_batch_s *pcks)
{
  int i;
  const dvbcsa_bs_block8_t *block = pkt_buf->data;

  for (i = 0; pcks[i].data; i++, block += BS_PKT_BLOCKS8)
    memcpy(pcks[i].data, block, pcks[i].len);
}

void dvbcsa_bs_decrypt(const struct dvbcsa_bs_key_s *key,
                       const struct dvbcsa_bs_batch_s *pcks,
                       unsigned int maxlen)
{
  struct dvbcsa_bs_pkt_buf pbuf;

#ifdef HAVE_ASSERT_H
  assert(maxlen % 8 == 0);
  assert(maxlen <= DVBCSA_BS_MAX_PACKET_LEN);
#endif

  dvbcsa_pkt_buf_load(&pbuf, pcks, maxlen);

#ifndef DVBCSA_DISABLE_STREAM
  dvbcsa_bs_stream_cipher_batch(key, &pbuf, maxlen);
#endif
#ifndef DVBCSA_DISABLE_BLOCK
  dvbcsa_bs_block_decrypt_batch(key, &pbuf, maxlen);
#endif

  BS_EMPTY ();                  // restore CPU multimedia state

  dvbcsa_pkt_buf_store(&pbuf, pcks);
}

void dvbcsa_bs_encrypt(const struct dvbcsa_bs_key_s *key,
                       const struct dvbcsa_bs_batch_s *pcks,
                       unsigned int maxlen)
{
  struct dvbcsa_bs_pkt_buf pbuf;

#ifdef HAVE_ASSERT_H
  assert(maxlen % 8 == 0);
  assert(maxlen <= DVBCSA_BS_MAX_PACKET_LEN);
#endif

  dvbcsa_pkt_buf_load(&pbuf, pcks, maxlen);

#ifndef DVBCSA_DISABLE_BLOCK
  dvbcsa_bs_block_encrypt_batch(key, &pbuf, maxlen);
#endif
#ifndef DVBCSA_DISABLE_STREAM
  dvbcsa_bs_stream_cipher_batch(key, &pbuf, maxlen);
#endif

  BS_EMPTY ();                  // restore CPU multimedia state

  dvbcsa_pkt_buf_store(&pbuf, pcks);
}

struct dvbcsa_bs_key_s * dvbcsa_bs_key_alloc(void)
{
  void *p;

#if defined(HAVE_MM_MALLOC)
  p = _mm_malloc(sizeof (struct dvbcsa_bs_key_s), sizeof(dvbcsa_bs_word_t));

#elif defined(HAVE_POSIX_MEMALIGN)
  p = posix_memalign(&p, sizeof(dvbcsa_bs_word_t) > sizeof(void *)
                        ? sizeof(dvbcsa_bs_word_t) : sizeof(void *),
                        sizeof (struct dvbcsa_bs_key_s)) ? NULL : p;

#else
  p = malloc(sizeof (struct dvbcsa_bs_key_s));
# warning Using malloc instead of posix_memalign may raise alignment issues
# ifdef HAVE_ASSERT_H
  assert((uintptr_t)p % sizeof(dvbcsa_bs_word_t) == 0);
# endif
#endif

  return (struct dvbcsa_bs_key_s *)p;
}

void dvbcsa_bs_key_free(struct dvbcsa_bs_key_s *key)
{
#ifdef HAVE_MM_MALLOC
  _mm_free(key);
#else
  free(key);
#endif
}

unsigned int dvbcsa_bs_batch_size (void)
{
  return BS_BATCH_SIZE;
}

#if defined(DVBCSA_DEBUG)
void
worddump (const char *str, const void *data, size_t len, size_t ws)
{
  const uint8_t *p = data;
  uint32_t i;

  printf ("- %s - %zu bytes\n", str, len);
  for (i = 0; i < len; i++)
    {
      const char *s;

      if ((i + 1) % 32 == 0)
        s = "\n";
      else if ((i + 1) % ws == 0)
        s = " ";
      else
        s = "";

      printf("%02x%s", p[i ^ (ws - 1)], s);
    }

  if (i % 32)
    putchar ('\n');
}
#endif

