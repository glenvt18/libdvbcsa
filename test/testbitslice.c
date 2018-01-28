/*

    This file is part of libdvbcsa.

    libdvbcsa is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    libdvbcsa is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libdvbcsa; if not, write to the Free Software Foundation,
    Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <time.h>
#include <stdio.h>

#include <dvbcsa/dvbcsa.h>
#include "dvbcsa_pv.h"

#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif

#define TS_SIZE         184     /* stream size generation */


static unsigned int  gs;
static uint8_t       cw[8] = {0x12, 0x34, 0x56, 0x78, 0x1e, 0xd4, 0xf6, 0xab};
static int           n_tests, n_failed;


static void
hexdump(const char *str, const void *data, uint32_t len)
{
  const uint8_t *p = (const uint8_t *)data;
  uint32_t i;

  printf ("- %s - %u bytes\n", str, len);
  for (i = 0; i < len; i++)
    printf ("0x%02x,%c", p[i], (i + 1) % 16 ? ' ' : '\n');
  puts ("");
}

static void
init_data(void)
{
  /*  srand(time(0)); */

  n_tests = 0;
  n_failed = 0;

  gs = dvbcsa_bs_batch_size();
}

static struct dvbcsa_bs_batch_s *
generate_packets(unsigned int minlen, unsigned int maxlen,
                 int random_size, int pattern,
                 unsigned n_packets)
{
  unsigned int i, curlen = minlen;
  unsigned int rnd;
  uint8_t *data;
  struct dvbcsa_bs_batch_s *pcks;

  printf(" - Generating batch with %i packets, size range: %d-%d%s, pattern: 0x%02x%s\n",
    n_packets, minlen, maxlen, (random_size)? " (random)" : "", pattern & 0xff,
    (pattern < 0)? " (random)" : "");

  data = (uint8_t *)malloc(n_packets * TS_SIZE);
  pcks = (struct dvbcsa_bs_batch_s *)malloc((n_packets + 2) * sizeof(struct dvbcsa_bs_batch_s));

#ifdef HAVE_ASSERT_H
  assert(pcks != NULL);
  assert(data != NULL);
#endif

  /* store data buffer in pcks[-1] */
  pcks[0].data = data;
  pcks[0].len = n_packets * TS_SIZE;
  pcks++;

  for (i = 0; i < n_packets; i++)
    {
      if (pattern < 0)
        rnd = rand() & 0xff;
      else
        rnd = pattern & 0xff;

      if (random_size)
        {
          int offs, len;

          len = minlen + rand() % (maxlen - minlen + 1);
          offs = rand() % (TS_SIZE - len + 1);
          pcks[i].data = data + i * TS_SIZE + offs;
          pcks[i].len = len;
        }
      else
        {
          if (curlen > maxlen)
            curlen = minlen;
          pcks[i].data = data + i * TS_SIZE + (TS_SIZE - curlen);
          pcks[i].len = curlen;
          curlen++;
        }
      memset(pcks[i].data, rnd, pcks[i].len);
    }

  pcks[i].data = NULL;

  return pcks;
}

static void
free_packets(struct dvbcsa_bs_batch_s *pcks)
{
  free(pcks[-1].data);
  free(pcks - 1);
}

static struct dvbcsa_bs_batch_s *
clone_packets(const struct dvbcsa_bs_batch_s *pcks)
{
  unsigned int i, n_packets;
  uint8_t *data;
  struct dvbcsa_bs_batch_s *clone;

  for (n_packets = 0; pcks[n_packets].data; n_packets++);

  data = (uint8_t *)malloc(n_packets * TS_SIZE);
  clone = (struct dvbcsa_bs_batch_s *)malloc((n_packets + 2) * sizeof(struct dvbcsa_bs_batch_s));

#ifdef HAVE_ASSERT_H
  assert(clone != NULL);
  assert(data != NULL);
#endif

  memcpy(data, pcks[-1].data, n_packets * TS_SIZE);
  clone[0].data = data;
  clone[0].len = n_packets * TS_SIZE;
  clone++;

  for (i = 0; i < n_packets; i++)
    {
      int offs = pcks[i].data - (pcks[-1].data + i * TS_SIZE);
#ifdef HAVE_ASSERT_H
      assert(offs >= 0 && offs + pcks[i].len <= TS_SIZE);
#endif
      clone[i].data = data + i * TS_SIZE + offs;
      clone[i].len = pcks[i].len;
    }

   clone[i].data = NULL;

   return clone;
}

static int
check_packets(const struct dvbcsa_bs_batch_s *got,
              const struct dvbcsa_bs_batch_s *expected)
{
  unsigned int i;

  for (i = 0; got[i].data; i++);

  printf(" - Checking results (%d pkts)...", i);

  for (i = 0; got[i].data; i++)
    {
      if (got[i].len != expected[i].len)
        {
          printf("\nlen[%u] mismatch: %u != %u\n", i, got[i].len, expected[i].len);
          return -1;
        }

      if (expected[i].data == NULL)
        {
          printf("\nexpected[%u] data is NULL\n", i);
          return -1;
        }

      if (memcmp(got[i].data, expected[i].data, got[i].len))
        {
          printf ("\n - #%u Failed !\n", i);
          hexdump("failed", got[i].data, got[i].len);
          return -1;
        }
    }

  if (expected[i].data != NULL)
    {
      printf("\nexpected[%u] data is not NULL\n", i);
      return -1;
    }

  printf("Ok\n");
  return 0;
}

static void
decrypt_packets(const dvbcsa_cw_t cw, struct dvbcsa_bs_batch_s *pcks)
{
  unsigned int i;
  struct dvbcsa_key_s *key = dvbcsa_key_alloc();

#ifdef HAVE_ASSERT_H
  assert(key != NULL);
#endif
  dvbcsa_key_set(cw, key);

  for (i = 0; pcks[i].data; i++)
    dvbcsa_decrypt(key, pcks[i].data, pcks[i].len);

  dvbcsa_key_free(key);
}

static void
encrypt_packets(const dvbcsa_cw_t cw, struct dvbcsa_bs_batch_s *pcks)
{
  unsigned int i;
  struct dvbcsa_key_s *key = dvbcsa_key_alloc();

#ifdef HAVE_ASSERT_H
  assert(key != NULL);
#endif
  dvbcsa_key_set(cw, key);

  for (i = 0; pcks[i].data; i++)
    dvbcsa_encrypt(key, pcks[i].data, pcks[i].len);

  dvbcsa_key_free(key);
}

static int
run_test(unsigned int minlen, unsigned int maxlen,
         int random_size, int pattern)
{
  int err = 0;
  struct dvbcsa_bs_batch_s *pcks, *orig;

  struct dvbcsa_bs_key_s *ffkey = dvbcsa_bs_key_alloc();

#ifdef HAVE_ASSERT_H
  assert(ffkey != NULL);
#endif

  dvbcsa_bs_key_set(cw, ffkey);

  orig = generate_packets(minlen, maxlen, random_size, pattern, gs);
  pcks = clone_packets(orig);

  /* Bitslice decrypt test */

  printf(" - Encrypting each packet using dvbcsa_encrypt()\n");
  encrypt_packets(cw, pcks);
  printf(" - Decrypting batch using _bitslice_ dvbcsa_bs_decrypt()\n");
  dvbcsa_bs_decrypt(ffkey, pcks, 184);
  err = check_packets(pcks, orig);

  /* Reset pcks */

  free_packets(pcks);
  pcks = clone_packets(orig);

  /* Bitslice encrypt test */

  printf(" - Decrypting each packet using dvbcsa_decrypt()\n");
  decrypt_packets(cw, pcks);
  printf(" - Encrypting batch using _bitslice_ dvbcsa_bs_encrypt()\n");
  dvbcsa_bs_encrypt(ffkey, pcks, 184);
  err |= check_packets(pcks, orig);

  if (!err)
    printf(" - Ok !\n");

  dvbcsa_bs_key_free(ffkey);
  free_packets(orig);
  free_packets(pcks);

  if (err)
    n_failed++;

  n_tests++;

  return err;
}

static void
make_mx_cw(struct dvbcsa_bs_mx_stream_s *st)
{
  unsigned int i;

  /* TODO generate random-like keys */
  for (i = 0; i < sizeof(dvbcsa_cw_t); i++)
    st->cw[i] = st->first_slot + i + st->n_slots;
}

static int
run_test_mx(struct dvbcsa_bs_mx_stream_s *streams,
            unsigned int n_streams)
{
  unsigned int i;
  int err = 0;
  struct dvbcsa_bs_batch_s **pcks, **orig;
  struct dvbcsa_bs_key_s *mx_key = dvbcsa_bs_key_alloc();

#ifdef HAVE_ASSERT_H
  assert(mx_key != NULL);
#endif
  pcks = (struct dvbcsa_bs_batch_s **)malloc(n_streams * sizeof(*pcks));
  orig = (struct dvbcsa_bs_batch_s **)malloc(n_streams * sizeof(*pcks));
#ifdef HAVE_ASSERT_H
  assert(pcks != NULL);
  assert(orig != NULL);
#endif

  printf(" = Testing streams ");
  for (i = 0; i < n_streams; i++)
    {
      if (i > 0)
        printf(", ");

      if (streams[i].n_slots > 1)
        printf("%u-%u", streams[i].first_slot,
               streams[i].first_slot + streams[i].n_slots - 1);

      else if (streams[i].n_slots == 1)
        printf("%u", streams[i].first_slot);
    }
  printf("\n");

  for (i = 0; i < n_streams; i++)
    {
      unsigned int n_packets;
      struct dvbcsa_bs_mx_stream_s *st = &streams[i];

      n_packets = st->n_slots * dvbcsa_bs_mx_slot_size();
      orig[i] = generate_packets(100, TS_SIZE, 1, -1, n_packets);
      pcks[i] = clone_packets(orig[i]);
      st->pcks = pcks[i];

      make_mx_cw(st);
      dvbcsa_bs_key_set_mx(mx_key, st);
    }

  /* Bitslice decrypt_mx test */

  printf(" - Encrypting each stream using dvbcsa_encrypt()\n");

  for (i = 0; i < n_streams; i++)
    encrypt_packets(streams[i].cw, streams[i].pcks);

  printf(" - Decrypting streams using _bitslice_ dvbcsa_bs_decrypt_mx()\n");
  dvbcsa_bs_decrypt_mx(mx_key, streams, n_streams, TS_SIZE);

  for (i = 0; i < n_streams; i++)
    err |= check_packets(streams[i].pcks, orig[i]);

  /* Reset pcks */

  for (i = 0; i < n_streams; i++)
    {
      free_packets(pcks[i]);
      pcks[i] = clone_packets(orig[i]);
      streams[i].pcks = pcks[i];
    }

  /* Bitslice encrypt_mx test */

  printf(" - Decrypting each stream using dvbcsa_decrypt()\n");

  for (i = 0; i < n_streams; i++)
    decrypt_packets(streams[i].cw, streams[i].pcks);

  printf(" - Encrypting streams using _bitslice_ dvbcsa_bs_encrypt_mx()\n");
  dvbcsa_bs_encrypt_mx(mx_key, streams, n_streams, TS_SIZE);
  for (i = 0; i < n_streams; i++)
    err |= check_packets(streams[i].pcks, orig[i]);

  if (!err)
    printf(" - Ok !\n");

  dvbcsa_bs_key_free(mx_key);
  free(orig);
  free(pcks);

  if (err)
    n_failed++;

  n_tests++;

  return err;
}

int
main(void)
{
  unsigned int minl;
  int i;
  struct dvbcsa_bs_mx_stream_s *streams;
  int max_slots;

  printf("* DVBCSA test *\n");

  init_data();

  run_test(TS_SIZE, TS_SIZE, 0, 0xa5);

  run_test(100, TS_SIZE, 1, -1);

  for (minl = 0; minl < TS_SIZE; minl += gs)
    {
      unsigned int maxl = minl + gs - 1;
      if (maxl > TS_SIZE)
        maxl = TS_SIZE;

      run_test(minl, maxl, 0, -1);
    }

  /* Multi-stream tests */

  printf("* DVBCSA multi-stream test *\n");

  max_slots = dvbcsa_bs_mx_slots();
  streams = (struct dvbcsa_bs_mx_stream_s *)malloc(
             max_slots * sizeof(struct dvbcsa_bs_mx_stream_s));
#ifdef HAVE_ASSERT_H
  assert(streams != NULL);
#endif

  streams[0].first_slot = 0;
  streams[0].n_slots = max_slots;
  run_test_mx(streams, 1);

  streams[0].first_slot = 2;
  streams[0].n_slots = 1;
  run_test_mx(streams, 1);

  streams[0].first_slot = 0;
  streams[0].n_slots = 1;
  streams[1].first_slot = 1;
  streams[1].n_slots = max_slots - 1;
  run_test_mx(streams, 2);

  streams[0].first_slot = max_slots - 1;
  streams[0].n_slots = 1;
  streams[1].first_slot = 0;
  streams[1].n_slots = max_slots - 1;
  run_test_mx(streams, 2);

  for (i = 0; i < max_slots; i++)
    {
      streams[i].first_slot = i;
      streams[i].n_slots = 1;
    }
  run_test_mx(streams, max_slots);

  for (i = 0; i < 2; i++)
    {
      streams[i].first_slot = i * max_slots / 2;
      streams[i].n_slots = max_slots / 2;
    }
  run_test_mx(streams, 2);

  for (i = 0; i < 4; i++)
    {
      streams[i].first_slot = i * max_slots / 4;
      streams[i].n_slots = max_slots / 4;
    }
  run_test_mx(streams, 4);

  free(streams);

  printf("=======================\n");

  if (n_failed)
    printf("%d out of %d tests FAILED.\n", n_failed, n_tests);
  else
    printf("OK! All tests passed.\n");

  return (n_failed != 0);
}

