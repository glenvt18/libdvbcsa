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

    (c) 2011 Erik Tews <e_tews at cdc.informatik.tu-darmstadt.de>
*/

#include <time.h>
#include <stdio.h>
#include <sys/time.h>

#include <dvbcsa/dvbcsa.h>
#include "dvbcsa_pv.h"

#ifdef __linux__
#include <sched.h>
#endif


#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif

#define KEYS 5000000


static void
print_result(const struct timeval *t0, const struct timeval *t1)
{
  printf("  - %u keys proceded, %.3f Mkeys/s\n\n", KEYS,
         (float)(KEYS) / (float)((t1->tv_sec * 1000000 + t1->tv_usec) - (t0->tv_sec * 1000000 + t0->tv_usec))
         );
}

int
main (void)
{
  struct dvbcsa_bs_key_s *ffkey = dvbcsa_bs_key_alloc();
  unsigned int i, j;
  struct timeval        t0, t1;
  unsigned int max_slots = dvbcsa_bs_mx_slots();
  struct dvbcsa_bs_mx_stream_s stream;

  memset(&stream, 0, sizeof(stream));

#ifdef HAVE_ASSERT_H
  assert(ffkey != NULL);
#endif

#ifdef __linux__
  struct sched_param sp = { .sched_priority = 1 };

  sched_setscheduler (0, SCHED_FIFO, &sp);
#endif

  printf("* DVBCSA multi-stream key setup benchmark. *\n");

  printf("\n  Worst case: %u streams, %u key slots\n",
         max_slots, max_slots);
  gettimeofday(&t0, NULL);
  for (i = 0; i < KEYS; i++)
    {
      for (j = 0; j < max_slots; j++)
        {
          stream.cw[0] = j + (i);
          stream.cw[1] = j + (i>>8);
          stream.cw[2] = j + (i>>16);
          stream.cw[3] = j + (i>>24);
          stream.first_slot = j;
          stream.n_slots = 1;

          dvbcsa_bs_key_set_mx(ffkey, &stream);
        }
    }
  gettimeofday(&t1, NULL);
  print_result(&t0, &t1);

  printf("\n  Single stream, %u key slots\n",
         max_slots);
  gettimeofday(&t0, NULL);
  for (i = 0; i < KEYS; i++)
    {
      stream.cw[0] = i;
      stream.cw[1] = i>>8;
      stream.cw[2] = i>>16;
      stream.cw[3] = i>>24;
      stream.first_slot = 0;
      stream.n_slots = max_slots;

      dvbcsa_bs_key_set_mx(ffkey, &stream);
    }
  gettimeofday(&t1, NULL);
  print_result(&t0, &t1);

  printf("\n  Single stream, 1 key slot\n");
  gettimeofday(&t0, NULL);
  for (i = 0; i < KEYS; i++)
    {
      stream.cw[0] = i;
      stream.cw[1] = i>>8;
      stream.cw[2] = i>>16;
      stream.cw[3] = i>>24;
      stream.first_slot = 0;
      stream.n_slots = 1;

      dvbcsa_bs_key_set_mx(ffkey, &stream);
    }
  gettimeofday(&t1, NULL);
  print_result(&t0, &t1);

  dvbcsa_bs_key_free(ffkey);

  return (0);
}

