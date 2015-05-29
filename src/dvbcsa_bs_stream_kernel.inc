#ifdef DVBCSA_BS_STREAM_KERNEL_INIT

static void DVBCSA_INLINE inline
dvbcsa_bs_stream_sbox1(dvbcsa_bs_word_t fa, dvbcsa_bs_word_t fb,
                       dvbcsa_bs_word_t fc, dvbcsa_bs_word_t fd,
                       dvbcsa_bs_word_t fe,
                       dvbcsa_bs_word_t *sa, dvbcsa_bs_word_t *sb)
{
  dvbcsa_bs_word_t tmp0, tmp1, tmp2, tmp3;

  tmp0 = BS_XOR (fa, BS_XOR (fb, BS_NOT (BS_OR (BS_XOR (BS_OR (fa, fb), fc), BS_XOR (fc, fd)))));
  tmp1 = BS_XOR (BS_OR (fa, fb), BS_NOT (BS_AND (fc, BS_OR (fa, BS_XOR (fb, fd)))));
  tmp2 = BS_XOR (fa, BS_XOR (BS_AND (fb, fd), BS_OR (BS_AND (fa, fd), fc)));
  tmp3 = BS_XOR (BS_AND (fa, fc), BS_XOR (fa, BS_OR (BS_AND (fa, fb), fd)));

  *sa = BS_XOR (tmp0, BS_AND (fe, tmp1));
  *sb = BS_XOR (tmp2, BS_AND (fe, tmp3));
}

static void DVBCSA_INLINE inline
dvbcsa_bs_stream_sbox2(dvbcsa_bs_word_t fa, dvbcsa_bs_word_t fb,
                       dvbcsa_bs_word_t fc, dvbcsa_bs_word_t fd,
                       dvbcsa_bs_word_t fe,
                       dvbcsa_bs_word_t *sa, dvbcsa_bs_word_t *sb)
{
  dvbcsa_bs_word_t tmp0, tmp1, tmp2, tmp3;

  tmp0 = BS_XOR (fa, BS_XOR (BS_AND (fb, BS_OR (fc, fd)), BS_XOR (fc, BS_NOT (fd))));
  tmp1 = BS_OR (BS_AND (fa, BS_XOR (fb, fd)), BS_AND (BS_OR (fa, fb), fc));
  tmp2 = BS_XOR (BS_AND (fb, fd), BS_OR (BS_AND (fa, fd), BS_XOR (fb, BS_NOT (fc))));
  tmp3 = BS_OR (BS_AND (fa, fd), BS_XOR (fa, BS_XOR (fb, BS_AND (fc, fd))));

  *sa = BS_XOR (tmp0, BS_AND (fe, tmp1));
  *sb = BS_XOR (tmp2, BS_AND (fe, tmp3));
}

static void DVBCSA_INLINE inline
dvbcsa_bs_stream_sbox3(dvbcsa_bs_word_t fa, dvbcsa_bs_word_t fb,
                       dvbcsa_bs_word_t fc, dvbcsa_bs_word_t fd,
                       dvbcsa_bs_word_t fe,
                       dvbcsa_bs_word_t *sa, dvbcsa_bs_word_t *sb)
{
  dvbcsa_bs_word_t tmp0, tmp1, tmp2;

  tmp0 = BS_XOR (fa, BS_XOR (fb, BS_XOR (BS_AND (fc, BS_OR (fa, fd)), fd)));
  tmp1 = BS_XOR (BS_AND (fa, fc), BS_OR (BS_XOR (fa, fd), BS_XOR (BS_OR (fb, fc), BS_NOT (fd))));
  tmp2 = BS_XOR (fa, BS_XOR (BS_AND (BS_XOR (fb, fc), fd), fc));

  *sa = BS_XOR (tmp0, BS_AND (BS_NOT (fe), tmp1));
  *sb = BS_XOR (tmp2, fe);
}

static void DVBCSA_INLINE inline
dvbcsa_bs_stream_sbox4(dvbcsa_bs_word_t fa, dvbcsa_bs_word_t fb,
                       dvbcsa_bs_word_t fc, dvbcsa_bs_word_t fd,
                       dvbcsa_bs_word_t fe,
                       dvbcsa_bs_word_t *sa, dvbcsa_bs_word_t *sb)
{
  dvbcsa_bs_word_t tmp0, tmp1, tmp2;

  tmp0 = BS_XOR (fa, BS_OR (BS_AND (fc, BS_XOR (fa, fd)), BS_XOR (fb, BS_OR (fc, BS_NOT (fd)))));
  tmp1 = BS_XOR (BS_AND (fa, fb), BS_XOR (fb, BS_XOR (BS_AND (BS_OR (fa, fc), fd), fc)));
  tmp2 = BS_XOR (fa, BS_OR (BS_AND (fb, fc), BS_XOR (BS_OR (BS_AND (fa, BS_XOR (fb, fd)), fc), fd)));

  *sa = BS_XOR (tmp0, BS_AND (fe, BS_XOR (tmp1, tmp0)));
  *sb = BS_XOR (BS_XOR (*sa, tmp2), fe);
}

static void DVBCSA_INLINE inline
dvbcsa_bs_stream_sbox5(dvbcsa_bs_word_t fa, dvbcsa_bs_word_t fb,
                       dvbcsa_bs_word_t fc, dvbcsa_bs_word_t fd,
                       dvbcsa_bs_word_t fe,
                       dvbcsa_bs_word_t *sa, dvbcsa_bs_word_t *sb)
{
  dvbcsa_bs_word_t tmp0, tmp1, tmp2, tmp3;

  tmp0 = BS_OR (BS_XOR (BS_AND (fa, BS_OR (fb, fc)), fb), BS_XOR (BS_OR (BS_XOR (fa, fc), fd), BS_VAL8(ff)));
  tmp1 = BS_XOR (fb, BS_AND (BS_XOR (fc, fd), BS_XOR (fc, BS_OR (fb, BS_XOR (fa, fd)))));
  tmp2 = BS_XOR (BS_AND (fa, fc), BS_XOR (fb, BS_AND (BS_OR (fb, BS_XOR (fa, fc)), fd)));
  tmp3 = BS_OR (BS_AND (BS_XOR (fa, fb), BS_XOR (fc, BS_VAL8(ff))), fd);

  *sa = BS_XOR (tmp0, BS_AND (fe, tmp1));
  *sb = BS_XOR (tmp2, BS_AND (fe, tmp3));
}

static void DVBCSA_INLINE inline
dvbcsa_bs_stream_sbox6(dvbcsa_bs_word_t fa, dvbcsa_bs_word_t fb,
                       dvbcsa_bs_word_t fc, dvbcsa_bs_word_t fd,
                       dvbcsa_bs_word_t fe,
                       dvbcsa_bs_word_t *sa, dvbcsa_bs_word_t *sb)
{
  dvbcsa_bs_word_t tmp0, tmp1, tmp2, tmp3;

  tmp0 = BS_XOR (BS_AND (BS_AND (fa, fc), fd), BS_XOR (BS_AND (fb, BS_OR (fa, fd)), fc));
  tmp1 = BS_NOT (BS_AND (BS_XOR (fa, fc), fd));
  tmp2 = BS_XOR (BS_AND (fa, BS_OR (fb, fc)), BS_XOR (fb, BS_OR (BS_AND (fb, fc), fd)));
  tmp3 = BS_AND (fc, BS_XOR (BS_AND (fa, BS_XOR (fb, fd)), BS_OR (fb, fd)));

  *sa = BS_XOR (tmp0, BS_AND (fe, tmp1));
  *sb = BS_XOR (tmp2, BS_AND (fe, tmp3));
}

static void DVBCSA_INLINE inline
dvbcsa_bs_stream_sbox7(dvbcsa_bs_word_t fa, dvbcsa_bs_word_t fb,
                       dvbcsa_bs_word_t fc, dvbcsa_bs_word_t fd,
                       dvbcsa_bs_word_t fe,
                       dvbcsa_bs_word_t *sa, dvbcsa_bs_word_t *sb)
{
  dvbcsa_bs_word_t tmp0, tmp1, tmp2, tmp3;

  tmp0 = BS_XOR (fb, BS_OR (BS_AND (fc, fd), BS_XOR (fa, BS_XOR (fc, fd))));
  tmp1 = BS_AND (BS_OR (fb, fd), BS_OR (BS_AND (fa, fc), BS_XOR (fb, BS_XOR (fc, fd))));
  tmp2 = BS_XOR (BS_OR (fa, fb), BS_XOR (BS_AND (fc, BS_OR (fb, fd)), fd));
  tmp3 = BS_OR (fd, BS_XOR (BS_AND (fa, fc), BS_VAL8(ff)));

  *sa = BS_XOR (tmp0, BS_AND (fe, tmp1));
  *sb = BS_XOR (tmp2, BS_AND (fe, tmp3));
}

static void
dvbcsa_bs_stream_cipher_kernel_init(struct dvbcsa_bs_stream_regs_s *regs)

#else

static void
dvbcsa_bs_stream_cipher_kernel(struct dvbcsa_bs_stream_regs_s *regs)

#endif

{
  dvbcsa_bs_word_t extra_B[4];
  dvbcsa_bs_word_t (*A)[4], (*B)[4];
  int i, j, b;

  A = regs->A + 32;
  B = regs->B + 32;

  for (i = 0; i < 8; i++)
    {
      for (j = 0; j < 4; j++)
        {
          // use 4x4 xor to produce extra nibble for T3

          extra_B[3] = BS_XOR (BS_XOR (BS_XOR (B[2][0], B[5][1]), B[6][2]), B[8][3]);
          extra_B[2] = BS_XOR (BS_XOR (BS_XOR (B[5][0], B[7][1]), B[2][3]), B[3][2]);
          extra_B[1] = BS_XOR (BS_XOR (BS_XOR (B[4][3], B[7][2]), B[3][0]), B[4][1]);
          extra_B[0] = BS_XOR (BS_XOR (BS_XOR (B[8][2], B[5][3]), B[2][1]), B[7][0]);

          // T1 = xor all inputs
          // in1, in2, D are only used in T1 during initialisation, not generation
          for (b = 0; b < 4; b++)
            {
              A[-1][b] = BS_XOR (A[9][b], regs->X[b]);
#ifdef DVBCSA_BS_STREAM_KERNEL_INIT
              //A[-1][b] = BS_XOR (BS_XOR (A[-1][b], D[b]), ((j % 2) ? in2[b] : in1[b]));
              A[-1][b] = BS_XOR (BS_XOR (A[-1][b], regs->D[b]), ((j % 2) ? regs->sb[8 * i + b] : regs->sb[8 * i + 4 + b]));
#endif
            }

          // T2 =  xor all inputs
          // in1, in2 are only used in T1 during initialisation, not generation
          // if p=0, use this, if p=1, rotate the result left
          for (b = 0; b < 4; b++)
            {
              B[-1][b] = BS_XOR (BS_XOR (B[6][b], B[9][b]), regs->Y[b]);
#ifdef DVBCSA_BS_STREAM_KERNEL_INIT
              //B[-1][b] = BS_XOR (B[-1][b], ((j % 2) ? in1[b] : in2[b]));
              B[-1][b] = BS_XOR (B[-1][b], ((j % 2) ? regs->sb[8 * i + 4 + b]: regs->sb[8 * i + b]));
#endif
            }

          // if p=1, rotate left (yes, this is what we're doing)
          {
          dvbcsa_bs_word_t tmp0, tmp1, tmp2, tmp3, tmp4;

          tmp3 = B[-1][3];
          tmp2 = B[-1][2];
          tmp4 = regs->p;
          tmp1 = B[-1][1];
          tmp0 = B[-1][0];
          B[-1][3] = BS_XOR (tmp3, BS_AND (BS_XOR (tmp3, tmp2), tmp4));
          B[-1][2] = BS_XOR (tmp2, BS_AND (BS_XOR (tmp2, tmp1), tmp4));
          B[-1][1] = BS_XOR (tmp1, BS_AND (BS_XOR (tmp1, tmp0), tmp4));
          B[-1][0] = BS_XOR (tmp0, BS_AND (BS_XOR (tmp0, tmp3), tmp4));
          }

          // T3 = xor all inputs
          //   D = E ^ Z ^ extra_B
          // T4 = sum, carry of Z + E + r
          //   also E' = F
          {
          dvbcsa_bs_word_t tmp0, tmp1, tmp2, tmp3, tmp4;

          tmp0 = BS_XOR (regs->Z[0], regs->E[0]);
          tmp1 = BS_AND (regs->Z[0], regs->E[0]);
          tmp2 = regs->F[0];
          regs->F[0] = BS_XOR (regs->E[0], BS_AND (regs->q, BS_XOR (regs->Z[0], regs->r)));
          regs->D[0] = BS_XOR (extra_B[0], tmp0);
          tmp3 = BS_AND (tmp0, regs->r);
          regs->E[0] = tmp2;
          tmp4 = BS_OR (tmp1, tmp3);

          tmp0 = BS_XOR (regs->Z[1], regs->E[1]);
          tmp1 = BS_AND (regs->Z[1], regs->E[1]);
          tmp2 = regs->F[1];
          regs->F[1] = BS_XOR (regs->E[1], BS_AND (regs->q, BS_XOR (regs->Z[1], tmp4)));
          regs->D[1] = BS_XOR (extra_B[1], tmp0);
          tmp3 = BS_AND (tmp0, tmp4);
          regs->E[1] = tmp2;
          tmp4 = BS_OR (tmp1, tmp3);

          tmp0 = BS_XOR (regs->Z[2], regs->E[2]);
          tmp1 = BS_AND (regs->Z[2], regs->E[2]);
          tmp2 = regs->F[2];
          regs->F[2] = BS_XOR (regs->E[2], BS_AND (regs->q, BS_XOR (regs->Z[2], tmp4)));
          regs->D[2] = BS_XOR (extra_B[2], tmp0);
          tmp3 = BS_AND (tmp0, tmp4);
          regs->E[2] = tmp2;
          tmp4 = BS_OR (tmp1, tmp3);

          tmp0 = BS_XOR (regs->Z[3], regs->E[3]);
          tmp1 = BS_AND (regs->Z[3], regs->E[3]);
          tmp2 = regs->F[3];
          regs->F[3] = BS_XOR (regs->E[3], BS_AND (regs->q, BS_XOR (regs->Z[3], tmp4)));
          regs->D[3] = BS_XOR (extra_B[3], tmp0);
          tmp3 = BS_AND (tmp0, tmp4);
          regs->E[3] = tmp2;
          regs->r = BS_XOR (regs->r, BS_AND (regs->q, BS_XOR (BS_OR (tmp1, tmp3), regs->r)));   // ultimate carry
          }

          dvbcsa_bs_stream_sbox1(A[0][2], A[5][1], A[6][3], A[8][0], A[3][0], &regs->X[0], &regs->Z[2]);
          dvbcsa_bs_stream_sbox2(A[2][2], A[5][3], A[6][0], A[8][1], A[1][1], &regs->X[1], &regs->Z[3]);
          dvbcsa_bs_stream_sbox3(A[1][0], A[4][1], A[4][3], A[5][2], A[0][3], &regs->Y[0], &regs->X[2]);
          dvbcsa_bs_stream_sbox4(A[0][1], A[1][3], A[3][2], A[7][0], A[2][3], &regs->Y[1], &regs->X[3]);
          dvbcsa_bs_stream_sbox5(A[3][3], A[5][0], A[7][1], A[8][2], A[4][2], &regs->Z[0], &regs->Y[2]);
          dvbcsa_bs_stream_sbox6(A[3][1], A[4][0], A[6][2], A[8][3], A[2][1], &regs->Z[1], &regs->Y[3]);
          dvbcsa_bs_stream_sbox7(A[2][0], A[6][1], A[7][2], A[7][3], A[1][2], &regs->p, &regs->q);

          A--;
          B--;

#ifndef DVBCSA_BS_STREAM_KERNEL_INIT

          // require 4 loops per output byte
          // 2 output bits are a function of the 4 bits of D
          // xor 2 by 2
          regs->sb[i * 8 + 7 - 2 * j] = BS_XOR (regs->D[2], regs->D[3]);
          regs->sb[i * 8 + 6 - 2 * j] = BS_XOR (regs->D[0], regs->D[1]);

#endif
        }  // INTERNAL LOOP
    }   // EXTERNAL LOOP

    for (i = 0; i < 10; i++)
        for (b = 0; b < 4; b++)
            regs->A[32 + i][b] = regs->A[i][b];
    for (i = 0; i < 10; i++)
        for (b = 0; b < 4; b++)
            regs->B[32 + i][b] = regs->B[i][b];
}

