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
  dvbcsa_bs_word_t s1a, s1b, s2a, s2b, s3a, s3b, s4a, s4b, s5a, s5b, s6a, s6b, s7a, s7b;
  dvbcsa_bs_word_t next_E[4];
  dvbcsa_bs_word_t tmp0, tmp1, tmp3, tmp4;
  dvbcsa_bs_word_t (*A)[4], (*B)[4];
  int i, j, b;

  A = regs->A + 32;
  B = regs->B + 32;

  for (i = 0; i < 8; i++)
    {
      for (j = 0; j < 4; j++)
        {
          dvbcsa_bs_stream_sbox1(A[0][2], A[5][1], A[6][3], A[8][0], A[3][0], &s1a, &s1b);
          dvbcsa_bs_stream_sbox2(A[2][2], A[5][3], A[6][0], A[8][1], A[1][1], &s2a, &s2b);
          dvbcsa_bs_stream_sbox3(A[1][0], A[4][1], A[4][3], A[5][2], A[0][3], &s3a, &s3b);
          dvbcsa_bs_stream_sbox4(A[0][1], A[1][3], A[3][2], A[7][0], A[2][3], &s4a, &s4b);
          dvbcsa_bs_stream_sbox5(A[3][3], A[5][0], A[7][1], A[8][2], A[4][2], &s5a, &s5b);
          dvbcsa_bs_stream_sbox6(A[3][1], A[4][0], A[6][2], A[8][3], A[2][1], &s6a, &s6b);
          dvbcsa_bs_stream_sbox7(A[2][0], A[6][1], A[7][2], A[7][3], A[1][2], &s7a, &s7b);

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
          tmp3 = B[-1][3];
          B[-1][3] = BS_XOR (B[-1][3], BS_AND (BS_XOR (B[-1][3], B[-1][2]), regs->p));
          B[-1][2] = BS_XOR (B[-1][2], BS_AND (BS_XOR (B[-1][2], B[-1][1]), regs->p));
          B[-1][1] = BS_XOR (B[-1][1], BS_AND (BS_XOR (B[-1][1], B[-1][0]), regs->p));
          B[-1][0] = BS_XOR (B[-1][0], BS_AND (BS_XOR (B[-1][0], tmp3), regs->p));

          // T3 = xor all inputs
          for (b = 0; b < 4; b++)
            regs->D[b] = BS_XOR (BS_XOR (regs->E[b], regs->Z[b]), extra_B[b]);

          // T4 = sum, carry of Z + E + r
          for (b = 0; b < 4; b++)
            next_E[b] = regs->F[b];

          tmp0 = BS_XOR (regs->Z[0], regs->E[0]);
          tmp1 = BS_AND (regs->Z[0], regs->E[0]);
          regs->F[0] = BS_XOR (regs->E[0], BS_AND (regs->q, BS_XOR (regs->Z[0], regs->r)));
          tmp3 = BS_AND (tmp0, regs->r);
          tmp4 = BS_OR (tmp1, tmp3);

          tmp0 = BS_XOR (regs->Z[1], regs->E[1]);
          tmp1 = BS_AND (regs->Z[1], regs->E[1]);
          regs->F[1] = BS_XOR (regs->E[1], BS_AND (regs->q, BS_XOR (regs->Z[1], tmp4)));
          tmp3 = BS_AND (tmp0, tmp4);
          tmp4 = BS_OR (tmp1, tmp3);

          tmp0 = BS_XOR (regs->Z[2], regs->E[2]);
          tmp1 = BS_AND (regs->Z[2], regs->E[2]);
          regs->F[2] = BS_XOR (regs->E[2], BS_AND (regs->q, BS_XOR (regs->Z[2], tmp4)));
          tmp3 = BS_AND (tmp0, tmp4);
          tmp4 = BS_OR (tmp1, tmp3);

          tmp0 = BS_XOR (regs->Z[3], regs->E[3]);
          tmp1 = BS_AND (regs->Z[3], regs->E[3]);
          regs->F[3] = BS_XOR (regs->E[3], BS_AND (regs->q, BS_XOR (regs->Z[3], tmp4)));
          tmp3 = BS_AND (tmp0, tmp4);
          regs->r = BS_XOR (regs->r, BS_AND (regs->q, BS_XOR (BS_OR (tmp1, tmp3), regs->r)));   // ultimate carry

          for (b = 0; b < 4; b++)
            regs->E[b] = next_E[b];

          A--;
          B--;

          regs->X[0] = s1a;
          regs->X[1] = s2a;
          regs->X[2] = s3b;
          regs->X[3] = s4b;
          regs->Y[0] = s3a;
          regs->Y[1] = s4a;
          regs->Y[2] = s5b;
          regs->Y[3] = s6b;
          regs->Z[0] = s5a;
          regs->Z[1] = s6a;
          regs->Z[2] = s1b;
          regs->Z[3] = s2b;

          regs->p = s7a;
          regs->q = s7b;

          // require 4 loops per output byte
          // 2 output bits are a function of the 4 bits of D
          // xor 2 by 2
          regs->cb[i * 8 + 7 - 2 * j] = BS_XOR (regs->D[2], regs->D[3]);
          regs->cb[i * 8 + 6 - 2 * j] = BS_XOR (regs->D[0], regs->D[1]);
        }  // INTERNAL LOOP
    }   // EXTERNAL LOOP

    for (i = 0; i < 10; i++)
        for (b = 0; b < 4; b++)
            regs->A[32 + i][b] = regs->A[i][b];
    for (i = 0; i < 10; i++)
        for (b = 0; b < 4; b++)
            regs->B[32 + i][b] = regs->B[i][b];
}

