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

#include <stdio.h>

#include <dvbcsa/dvbcsa.h>
#include "dvbcsa_bs.h"
#include "dvbcsa_bs_transpose.h"

void _testeq(const int i, uint8_t a, uint8_t b, const char *sa, const char *sb, const char *file, const int line, const char *func)
{
        if ( a==b )
                return;
        printf("Test failed in %s:%d (%s), %s (%x) == %s (%x), i=%d\n", file, line, func, sa, a, sb, b, i);
//      abort();
}

#define testeq(i, a, b) _testeq(i, a, b, #a, #b, __FILE__, __LINE__, __FUNCTION__)

#define TEST_BS8(xx)                                                            \
        do {                                                                    \
                dvbcsa_bs_word_t aaa = BS_VAL8(xx);                             \
                for (i = 0; i < BS_BATCH_BYTES; i++)                            \
                        testeq(i, (uint8_t)BS_EXTRACT8(aaa, i), 0x##xx);        \
        } while(0)

#define TEST_BS8_16(N)                                                                                             \
        TEST_BS8(N##0);                                                                                                    \
        TEST_BS8(N##1);                                                                                                    \
        TEST_BS8(N##2);                                                                                                    \
        TEST_BS8(N##3);                                                                                                    \
        TEST_BS8(N##4);                                                                                                    \
        TEST_BS8(N##5);                                                                                                    \
        TEST_BS8(N##6);                                                                                                    \
        TEST_BS8(N##7);                                                                                                    \
        TEST_BS8(N##8);                                                                                                    \
        TEST_BS8(N##9);                                                                                                    \
        TEST_BS8(N##a);                                                                                                    \
        TEST_BS8(N##b);                                                                                                    \
        TEST_BS8(N##c);                                                                                                    \
        TEST_BS8(N##d);                                                                                                    \
        TEST_BS8(N##e);                                                                                                    \
        TEST_BS8(N##f)

void print_dvbcsa_word(dvbcsa_bs_word_t v)
{
        int i;
        for (i = 0; i < BS_BATCH_BYTES; i++)
                printf("%02x", (uint8_t)BS_EXTRACT8(v, BS_BATCH_BYTES-1-i));
}

#define vec_testeq(a, b) _vec_testeq(a, b, __FILE__, __LINE__, __FUNCTION__)

void _vec_testeq(dvbcsa_bs_word_t l, dvbcsa_bs_word_t r, const char *file, const int line, const char *func)
{
        int equal = 1, i;
        for (i = 0; i < BS_BATCH_BYTES; i++)
                equal &= (uint8_t)BS_EXTRACT8(l, i) == (uint8_t)BS_EXTRACT8(r, i);
        if (equal)
                return;
        printf("Expected "); print_dvbcsa_word(l); printf("\n");
        printf("Got      "); print_dvbcsa_word(r); printf("\n");
        printf("Error in %s: %s:%d. Inequality\n", func, file, line);
        abort();
}

int
main            (void)
{
  unsigned int i;

  dvbcsa_bs_word_t c, b, a;/* = BS_VAL8(5a);

  for (i = 0; i < BS_BATCH_BYTES; i++)
  testeq(i, (uint8_t)BS_EXTRACT8(a, i), 0x5a);*/

  TEST_BS8(5a);
  TEST_BS8(80);
  TEST_BS8(10);
  TEST_BS8(ff);
  TEST_BS8(00);

/*   TEST_BS8_16(0); */
/*   TEST_BS8_16(1); */
/*   TEST_BS8_16(2); */
/*   TEST_BS8_16(3); */
/*   TEST_BS8_16(4); */
/*   TEST_BS8_16(5); */
/*   TEST_BS8_16(6); */
/*   TEST_BS8_16(7); */
/*   TEST_BS8_16(8); */
/*   TEST_BS8_16(9); */
/*   TEST_BS8_16(a); */
/*   TEST_BS8_16(b); */
/*   TEST_BS8_16(c); */
/*   TEST_BS8_16(d); */
/*   TEST_BS8_16(e); */
/*   TEST_BS8_16(f); */

  a = BS_VAL16(1234);

  for (i = 0; i < BS_BATCH_BYTES; i += 2)
    {
      testeq(i, (uint8_t)BS_EXTRACT8(a, i), 0x34);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 1), 0x12);
    }

  vec_testeq(BS_VAL16(1234), a);

  a = BS_VAL32(12345678);

  for (i = 0; i < BS_BATCH_BYTES; i += 4)
    {
      testeq(i, (uint8_t)BS_EXTRACT8(a, i), 0x78);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 1), 0x56);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 2), 0x34);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 3), 0x12);
    }

  vec_testeq(BS_VAL32(12345678), a);

#if BS_BATCH_SIZE > 32

  a = BS_VAL64(1234567887654321);

  for (i = 0; i < BS_BATCH_BYTES; i += 8)
    {
      testeq(i, (uint8_t)BS_EXTRACT8(a, i), 0x21);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 1), 0x43);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 2), 0x65);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 3), 0x87);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 4), 0x78);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 5), 0x56);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 6), 0x34);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 7), 0x12);
    }

  vec_testeq(BS_VAL64(1234567887654321), a);

#endif

#if BS_BATCH_SIZE > 64

  a = BS_VAL(0xabcdef1331fedcbaLL,
             0x1234567887654321LL);

  for (i = 0; i < BS_BATCH_BYTES; i += 16)
    {
      testeq(i, (uint8_t)BS_EXTRACT8(a, i), 0x21);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 1), 0x43);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 2), 0x65);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 3), 0x87);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 4), 0x78);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 5), 0x56);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 6), 0x34);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 7), 0x12);

      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 8), 0xba);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 9), 0xdc);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 10), 0xfe);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 11), 0x31);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 12), 0x13);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 13), 0xef);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 14), 0xcd);
      testeq(i, (uint8_t)BS_EXTRACT8(a, i + 15), 0xab);
    }

#endif

  /* test AND */

  a = BS_VAL8(5a);
  b = BS_VAL8(1f);
  c = BS_AND(a, b);
  vec_testeq(BS_VAL8(1a), c);

  /* test OR */

  a = BS_VAL8(5a);
  b = BS_VAL8(1f);
  c = BS_OR(a, b);
  vec_testeq(BS_VAL8(5f), c);

  /* test XOR */

  a = BS_VAL8(5a);
  b = BS_VAL8(1f);
  c = BS_XOR(a, b);
  vec_testeq(BS_VAL8(45), c);

  /* test NOT */

  a = BS_VAL8(5a);
  c = BS_NOT(a);
  vec_testeq(BS_VAL8(a5), c);

  /* test SHL */

  a = BS_VAL8(01);
  b = BS_SHL(a, 4);
  vec_testeq(BS_VAL8(10), b);

  a = BS_VAL16(0001);
  b = BS_SHL(a, 13);
  vec_testeq(BS_VAL16(2000), b);

  /* Test SHR */

  a = BS_VAL8(10);
  b = BS_SHR(a, 4);
  vec_testeq(BS_VAL8(01), b);

  a = BS_VAL8(80);
  b = BS_SHR(a, 4);
  vec_testeq(BS_VAL8(08), b);

  a = BS_VAL8(80);
  b = BS_SHR(a, 7);
  vec_testeq(BS_VAL8(01), b);

  a = BS_VAL16(2000);
  b = BS_SHR(a, 13);
  vec_testeq(BS_VAL16(0001), b);

#ifdef BS_LOAD_DEINTERLEAVE_8

#ifndef DVBCSA_USE_AVX2
  /* test BS_LOAD_DEINTERLEAVE_8 */

  {
  /* block cipher sbox + perm */
  dvbcsa_bs_word_t d, buf_in[2], buf_out[4];
  int i;
  dvbcsa_u8_aliasing_t *src, *dst;

  a = BS_VAL(0x0001020304050607LL, 0x08090a0b0c0d0e0fLL);
  b = BS_VAL(0x8f8e8d8c8b8a8988LL, 0x8786858483828180LL);
  buf_in[0] = a;
  buf_in[1] = b;
  src = (dvbcsa_u8_aliasing_t *)buf_in;
  dst = (dvbcsa_u8_aliasing_t *)buf_out;
  for (i = 0; i < BS_BATCH_BYTES * 2; i++)
    {
      dst[i * 2] = src[i];
      dst[i * 2 + 1] = src[i] + 0x10;
    }
  BS_LOAD_DEINTERLEAVE_8(buf_out, c, d);
  vec_testeq(a, c);
  vec_testeq(BS_OR(a, BS_VAL8(10)), d);
  BS_LOAD_DEINTERLEAVE_8(buf_out + 2, c, d);
  vec_testeq(b, c);
  vec_testeq(BS_OR(b, BS_VAL8(10)), d);
  }
#endif

#endif

  /* test 2x2 matrix transpose */

  {
  dvbcsa_bs_word_t a, b, c, d;

#if BS_BATCH_SIZE > 32
  a = BS_VAL64(3132333435363738);
  b = BS_VAL64(494a4b4c4d4e4f40);
#ifdef DVBCSA_ENDIAN_LITTLE
  c = BS_VAL64(4d4e4f4035363738);
  d = BS_VAL64(494a4b4c31323334);
#else
  c = BS_VAL64(31323334494a4b4c);
  d = BS_VAL64(353637384d4e4f40);
#endif
  BS_SWAP32(a, b);
  vec_testeq(a, c);
  vec_testeq(b, d);
#endif

  a = BS_VAL32(12345678);
  b = BS_VAL32(9abcdef0);
#ifdef DVBCSA_ENDIAN_LITTLE
  c = BS_VAL32(def05678);
  d = BS_VAL32(9abc1234);
#else
  c = BS_VAL32(12349abc);
  d = BS_VAL32(5678def0);
#endif
  BS_SWAP16(a, b);
  vec_testeq(a, c);
  vec_testeq(b, d);

  a = BS_VAL32(12345678);
  b = BS_VAL32(9abcdef0);
#ifdef DVBCSA_ENDIAN_LITTLE
  c = BS_VAL32(bc34f078);
  d = BS_VAL32(9a12de56);
#else
  c = BS_VAL32(129a56de);
  d = BS_VAL32(34bc78f0);
#endif
  BS_SWAP8(a, b);
  vec_testeq(a, c);
  vec_testeq(b, d);

  a = BS_VAL32(12345678);
  b = BS_VAL32(9abcdef0);
  c = BS_VAL32(a2c4e608);
  d = BS_VAL32(91b3d5f7);
  BS_SWAP4(a, b);
  vec_testeq(a, c);
  vec_testeq(b, d);

  a = BS_VAL32(ff00ff00);
  b = BS_VAL32(00ff00ff);
  c = BS_VAL32(33cc33cc);
  d = BS_VAL32(33cc33cc);
  BS_SWAP2(a, b);
  vec_testeq(a, c);
  vec_testeq(b, d);

  a = BS_VAL32(ff00ff00);
  b = BS_VAL32(00ff00ff);
  c = BS_VAL32(55aa55aa);
  d = BS_VAL32(55aa55aa);
  BS_SWAP1(a, b);
  vec_testeq(a, c);
  vec_testeq(b, d);

  }

  puts("Ok");
  return 0;
}

