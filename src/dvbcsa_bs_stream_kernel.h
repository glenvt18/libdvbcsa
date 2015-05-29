#ifndef DVBCSA_BS_STREAM_KERNEL_H_
#define DVBCSA_BS_STREAM_KERNEL_H_

#include "dvbcsa_bs.h"

struct dvbcsa_bs_stream_regs_s {
  dvbcsa_bs_word_t A[32 + 10][4];
  dvbcsa_bs_word_t B[32 + 10][4];
  dvbcsa_bs_word_t X[4];
  dvbcsa_bs_word_t Y[4];
  dvbcsa_bs_word_t Z[4];
  dvbcsa_bs_word_t D[4];
  dvbcsa_bs_word_t E[4];
  dvbcsa_bs_word_t F[4];
  dvbcsa_bs_word_t sb[64];
  dvbcsa_bs_word_t p;
  dvbcsa_bs_word_t q;
  dvbcsa_bs_word_t r;
};

#endif

