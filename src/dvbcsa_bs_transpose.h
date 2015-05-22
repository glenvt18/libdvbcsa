#ifndef DVBCSA_BS_TRANSPOSE_H_
#define DVBCSA_BS_TRANSPOSE_H_

#include "dvbcsa_bs.h"

/*
    2x2 matrix transpose swap operation:

    t = |a b  c d|  =>  |f b  h d|
    b = |e f  g h|  =>  |e a  g c|

    'a' and 'e' are MSB of dvbcsa_bs_word_t.
    (little-endian transpose)

    tmp = (b ^ (t>>j)) & m;
    b = b ^ tmp;
    t = t ^ (tmp<<j);
*/

#define BS_SWAP_BITS_LE(t, b, shift, mask) \
    { \
    dvbcsa_bs_word_t tmp; \
    tmp = BS_AND(BS_XOR(BS_SHR(t, shift), b), mask); \
    (b) = BS_XOR((b), tmp); \
    (t) = BS_XOR((t), BS_SHL(tmp, shift)); \
    }

#ifndef BS_SWAP32_LE
#define BS_SWAP32_LE(t, b) BS_SWAP_BITS_LE(t, b, 32, BS_VAL64(00000000ffffffff))
#endif

#ifndef BS_SWAP16_LE
#define BS_SWAP16_LE(t, b) BS_SWAP_BITS_LE(t, b, 16, BS_VAL32(0000ffff))
#endif

#ifndef BS_SWAP8_LE
#define BS_SWAP8_LE(t, b) BS_SWAP_BITS_LE(t, b, 8, BS_VAL16(00ff))
#endif

#ifndef BS_SWAP4_LE
#define BS_SWAP4_LE(t, b) BS_SWAP_BITS_LE(t, b, 4, BS_VAL8(0f))
#endif

#ifndef BS_SWAP2_LE
#define BS_SWAP2_LE(t, b) BS_SWAP_BITS_LE(t, b, 2, BS_VAL8(33))
#endif

#ifndef BS_SWAP1_LE
#define BS_SWAP1_LE(t, b) BS_SWAP_BITS_LE(t, b, 1, BS_VAL8(55))
#endif

#define BS_SWAP4(t, b)   BS_SWAP4_LE(t, b)
#define BS_SWAP2(t, b)   BS_SWAP2_LE(t, b)
#define BS_SWAP1(t, b)   BS_SWAP1_LE(t, b)

#ifdef DVBCSA_ENDIAN_LITTLE

#define BS_SWAP32(t, b)  BS_SWAP32_LE(t, b)
#define BS_SWAP16(t, b)  BS_SWAP16_LE(t, b)
#define BS_SWAP8(t, b)   BS_SWAP8_LE(t, b)

#else

#define BS_SWAP32(t, b)  BS_SWAP32_LE(b, t)
#define BS_SWAP16(t, b)  BS_SWAP16_LE(b, t)
#define BS_SWAP8(t, b)   BS_SWAP8_LE(b, t)

#endif

#endif

