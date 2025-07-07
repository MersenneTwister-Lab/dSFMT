#pragma once
/**
 * @file dSFMT-common.h
 *
 * @brief SIMD oriented Fast Mersenne Twister(SFMT) pseudorandom
 * number generator with jump function. This file includes common functions
 * used in random number generation and jump.
 *
 * @author Mutsuo Saito (Hiroshima University)
 * @author Makoto Matsumoto (The University of Tokyo)
 *
 * Copyright (C) 2006, 2007 Mutsuo Saito, Makoto Matsumoto and Hiroshima
 * University.
 * Copyright (C) 2012 Mutsuo Saito, Makoto Matsumoto, Hiroshima
 * University and The University of Tokyo.
 * Copyright (C) 2018, 2020 Masaki Ota. All rights reserved.
 * All rights reserved.
 *
 * The 3-clause BSD License is applied to this software, see
 * LICENSE.txt
 */
#ifndef DSFMT_COMMON_H
#define DSFMT_COMMON_H

#include "dSFMT.h"

/*
 * NOTE: This macro mitigates performance degradation caused by
 *  the high latency of vperm* instructions.
 */
#define DSFMT_LUNG_SHORT_CUT

#if defined(HAVE_SSE2)
#  include <emmintrin.h>
#if defined(__AVX2__)
#  include <immintrin.h>
#endif
union X128I_T {
    uint64_t u[2];
    __m128i  i128;
};
union X128D_T {
    double d[2];
    __m128d d128;
};
/** mask data for sse2 */
static const union X128I_T sse2_param_mask = {{DSFMT_MSK1, DSFMT_MSK2}};

#elif defined(__aarch64__) && !defined(ARM64_NO_NEON)
#define HAVE_NEON
#if defined(__ARM_FEATURE_SHA3) 
#define HAVE_SHA3
#endif 
#endif

#if defined(__aarch64__) && defined(HAVE_NEON)
#include <arm_neon.h>
union X128I_T {
    uint64_t u[2];
    uint64x2_t i128;
};
union X128D_T {
    double d[2];
    float64x2_t d128;
};
/** mask data for NEON */
static const union X128I_T neon_param_mask = {{DSFMT_MSK1, DSFMT_MSK2}};
#endif


#if defined(HAVE_ALTIVEC)
static inline  void do_recursion(w128_t *r, w128_t *a, w128_t * b,
				w128_t *lung) {
    const vector unsigned char sl1 = ALTI_SL1;
    const vector unsigned char sl1_perm = ALTI_SL1_PERM;
    const vector unsigned int sl1_msk = ALTI_SL1_MSK;
    const vector unsigned char sr1 = ALTI_SR;
    const vector unsigned char sr1_perm = ALTI_SR_PERM;
    const vector unsigned int sr1_msk = ALTI_SR_MSK;
    const vector unsigned char perm = ALTI_PERM;
    const vector unsigned int msk1 = ALTI_MSK;
    vector unsigned int w, x, y, z;

    z = a->s;
    w = lung->s;
    x = vec_perm(w, (vector unsigned int)perm, perm);
    y = vec_perm(z, (vector unsigned int)sl1_perm, sl1_perm);
    y = vec_sll(y, sl1);
    y = vec_and(y, sl1_msk);
    w = vec_xor(x, b->s);
    w = vec_xor(w, y);
    x = vec_perm(w, (vector unsigned int)sr1_perm, sr1_perm);
    x = vec_srl(x, sr1);
    x = vec_and(x, sr1_msk);
    y = vec_and(w, msk1);
    z = vec_xor(z, y);
    r->s = vec_xor(z, x);
    lung->s = w;
}

// note : AVX-512 not passed
#elif defined(HAVE_AVX512) && DSFMT_MEXP > 1279
#include "dSFMT-avx512.h"
#define DSFMT_GEN_RAND_CUSTOM

#elif defined(HAVE_AVX2) && DSFMT_MEXP > 1279
#include "dSFMT-avx2.h"
#define DSFMT_GEN_RAND_CUSTOM

#elif defined(HAVE_SSE2)
#if DSFMT_MEXP == 1279
#define DSFMT_RECURSION_X3
#elif DSFMT_N - DSFMT_POS1 > 1 
#define DSFMT_RECURSION_X2
#endif

/**
 * This function represents the recursion formula.
 * @param r output 128-bit
 * @param a a 128-bit part of the internal state array
 * @param b a 128-bit part of the internal state array
 * @param d a 128-bit part of the internal state array (I/O)
 */
inline static void do_recursion(w128_t *r, w128_t *a, w128_t *b, w128_t *u) {
    __m128i v, w, x, y, z;

    x = a->si;
    z = _mm_slli_epi64(x, DSFMT_SL1);
    y = _mm_shuffle_epi32(u->si, SSE2_SHUFF);
#if defined(HAVE_AVX512)
    y = _mm_ternarylogic_epi32(y, z, b->si, 0x96);
#else
    z = _mm_xor_si128(z, b->si);
    y = _mm_xor_si128(y, z);
#endif
    u->si = y;

    v = _mm_srli_epi64(y, DSFMT_SR);
    w = _mm_and_si128(y, sse2_param_mask.i128);
#if defined(HAVE_AVX512)
    v = _mm_ternarylogic_epi32(v, x, w, 0x96);
#else
    v = _mm_xor_si128(v, x);
    v = _mm_xor_si128(v, w);
#endif
    r->si = v;
}

static inline void do_recursion_x2(w128_t *r, w128_t *a, w128_t *b, w128_t *u) {
    __m128i v1, w1, x1, y1, z1;
    __m128i v2, w2, x2, y2, z2;

    x1 = a[0].si;
    x2 = a[1].si;

    z1 = _mm_slli_epi64(x1, DSFMT_SL1);
    z2 = _mm_slli_epi64(x2, DSFMT_SL1);

    z1 = _mm_xor_si128(z1, b[0].si);
    z2 = _mm_xor_si128(z2, b[1].si);
    
    y2 = u->si;
    v1 = _mm_shuffle_epi32(z1, SSE2_SHUFF);
    y1 = _mm_shuffle_epi32(y2, SSE2_SHUFF);

    y1 = _mm_xor_si128(y1, z1);
#if defined(HAVE_AVX512)
    y2 = _mm_ternarylogic_epi32(y2, z2, v1, 0x96);
#else
    z2 = _mm_xor_si128(z2, v1);
    y2 = _mm_xor_si128(y2, z2);
#endif
    u->si = y2;

    v1 = _mm_srli_epi64(y1, DSFMT_SR);
    v2 = _mm_srli_epi64(y2, DSFMT_SR);
    w1 = _mm_and_si128(y1, sse2_param_mask.i128);
    w2 = _mm_and_si128(y2, sse2_param_mask.i128);

#if defined(HAVE_AVX512)
    v1 = _mm_ternarylogic_epi32(v1, x1, w1, 0x96);
    v2 = _mm_ternarylogic_epi32(v2, x2, w2, 0x96);
#else
    v1 = _mm_xor_si128(v1, x1);
    v2 = _mm_xor_si128(v2, x2);
    v1 = _mm_xor_si128(v1, w1);
    v2 = _mm_xor_si128(v2, w2);
#endif
    r[0].si = v1;
    r[1].si = v2;
}

static inline  void do_recursion_x3(w128_t *r, w128_t *a, w128_t *b, w128_t *u) {
    do_recursion_x2(r, a, b, u);
    do_recursion(r+2, a+2, b+2, u);
} 

#elif defined(HAVE_NEON)
#include "dSFMT-neon.h"
#else
/**
 * This function represents the recursion formula.
 * @param r output 128-bit
 * @param a a 128-bit part of the internal state array
 * @param b a 128-bit part of the internal state array
 * @param lung a 128-bit part of the internal state array (I/O)
 */
static inline  void do_recursion(w128_t *r, w128_t *a, w128_t * b,
				w128_t *lung) {
    uint64_t t0, t1, L0, L1;

    t0 = a->u[0];
    t1 = a->u[1];
    L0 = lung->u[0];
    L1 = lung->u[1];
    lung->u[0] = (t0 << DSFMT_SL1) ^ (L1 >> 32) ^ (L1 << 32) ^ b->u[0];
    lung->u[1] = (t1 << DSFMT_SL1) ^ (L0 >> 32) ^ (L0 << 32) ^ b->u[1];
    r->u[0] = (lung->u[0] >> DSFMT_SR) ^ (lung->u[0] & DSFMT_MSK1) ^ t0;
    r->u[1] = (lung->u[1] >> DSFMT_SR) ^ (lung->u[1] & DSFMT_MSK2) ^ t1;
}
#endif


#if !defined(DSFMT_GEN_RAND_CUSTOM)
/**
 * This function fills the internal state array with double precision
 * floating point pseudorandom numbers of the IEEE 754 format.
 * @param dsfmt dsfmt state vector.
 */
static inline  void dsfmt_gen_rand_all_impl(dsfmt_t *dsfmt) {
    int i;

    w128_t lung;
    lung = dsfmt->status[DSFMT_N];

    i = 0;

    const int loop1_end = DSFMT_N - DSFMT_POS1;
    const int loop2_end = DSFMT_N;
    
#if defined(DSFMT_RECURSION_X4)
    for (; i < loop1_end - 3; i += 4) {
        do_recursion_x4(&dsfmt->status[i], &dsfmt->status[i],
                        &dsfmt->status[i + DSFMT_POS1], &lung);
    }
    if (i < loop1_end) {
        w128_t b[4];
        switch (loop1_end & 3)
        { 
            case 1:
                b[0] = dsfmt->status[i + DSFMT_POS1];
                b[1] = dsfmt->status[0];
                b[2] = dsfmt->status[1];
                b[3] = dsfmt->status[2];
                break;
            case 2:
                b[0] = dsfmt->status[i + DSFMT_POS1];
                b[1] = dsfmt->status[i + DSFMT_POS1+1];
                b[2] = dsfmt->status[0];
                b[3] = dsfmt->status[1];
                break;
            case 3:
                b[0] = dsfmt->status[i + DSFMT_POS1];
                b[1] = dsfmt->status[i + DSFMT_POS1+1];
                b[2] = dsfmt->status[i + DSFMT_POS1+2];
                b[3] = dsfmt->status[0];
                break;
        }
        do_recursion_x4(&dsfmt->status[i], &dsfmt->status[i], &b[0], &lung);
        i+=4;
    }

    for (; i < loop2_end - 3; i += 4) {
        do_recursion_x4(&dsfmt->status[i], &dsfmt->status[i],
                        &dsfmt->status[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
    for (; i < loop2_end; i++) {
        do_recursion(&dsfmt->status[i], &dsfmt->status[i],
                     &dsfmt->status[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
#elif defined(DSFMT_RECURSION_X2)
    for (; i < loop1_end - 1; i += 2) {
        do_recursion_x2(&dsfmt->status[i], &dsfmt->status[i],
                        &dsfmt->status[i + DSFMT_POS1], &lung);
    }
    if ((DSFMT_N - DSFMT_POS1) & 1)
    {
        w128_t b[2];
        b[0] = dsfmt->status[i + DSFMT_POS1];
        b[1] = dsfmt->status[0];
        do_recursion_x2(&dsfmt->status[i], &dsfmt->status[i], b, &lung);
        i+=2;
    }
    for (; i < loop2_end - 1; i += 2) {
        do_recursion_x2(&dsfmt->status[i], &dsfmt->status[i],
                        &dsfmt->status[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
    if (i < loop2_end) {
        do_recursion(&dsfmt->status[i], &dsfmt->status[i],
                     &dsfmt->status[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
#elif defined(DSFMT_RECURSION_X3)
    for (; i < loop1_end; i += 3) {
        do_recursion_x3(&dsfmt->status[i], &dsfmt->status[i],
                        &dsfmt->status[i + DSFMT_POS1], &lung);
    }
    for (; i < loop2_end; i += 3) {
        do_recursion_x3(&dsfmt->status[i], &dsfmt->status[i],
                        &dsfmt->status[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
#else
    for (; i < loop1_end; i++) {
        do_recursion(&dsfmt->status[i], &dsfmt->status[i],
                     &dsfmt->status[i + DSFMT_POS1], &lung);
    }
    for (; i < loop2_end; i++) {
        do_recursion(&dsfmt->status[i], &dsfmt->status[i],
                     &dsfmt->status[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
#endif

    dsfmt->status[DSFMT_N] = lung;
}
/*
 * This is the core function for generating pseudorandom numbers.
 * It fills the user-specified array and applies a given conversion.
 * (Unrolled version)
 * @param dsfmt dsfmt state vector.
 * @param array an 128-bit array to be filled by pseudorandom numbers.
 * @param size number of 128-bit pseudorandom numbers to be generated.
 * @param converter a function pointer for value conversion.
 */
static void gen_rand_array(dsfmt_t *dsfmt, w128_t *array, ptrdiff_t size) {
    ptrdiff_t i, j;
 
    w128_t lung = dsfmt->status[DSFMT_N];
    i = 0;
    const ptrdiff_t loop1_end = DSFMT_N - DSFMT_POS1;
#if defined(DSFMT_RECURSION_X4)
    for (; i <= loop1_end - 4; i += 4) {
        do_recursion_x4(&array[i], &dsfmt->status[i], &dsfmt->status[i + DSFMT_POS1], &lung);
    }
#endif
#if defined(DSFMT_RECURSION_X3)
    for (; i <= loop1_end - 3; i += 3) {
        do_recursion_x3(&array[i], &dsfmt->status[i], &dsfmt->status[i + DSFMT_POS1], &lung);
    }
#endif
#if defined(DSFMT_RECURSION_X2)
    for (; i <= loop1_end - 2; i += 2) {
        do_recursion_x2(&array[i], &dsfmt->status[i], &dsfmt->status[i + DSFMT_POS1], &lung);
    }
#endif
    for (; i < loop1_end; i++) {
        do_recursion(&array[i], &dsfmt->status[i], &dsfmt->status[i + DSFMT_POS1], &lung);
    }

    const ptrdiff_t loop2_end = DSFMT_N;

#if defined(DSFMT_RECURSION_X4)
    for (; i <= loop2_end - 4; i += 4) {
        do_recursion_x4(&array[i], &dsfmt->status[i], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
#endif
#if defined(DSFMT_RECURSION_X3)
    for (; i <= loop1_end - 3; i += 3) {
        do_recursion_x3(&array[i], &dsfmt->status[i], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
#endif
#if defined(DSFMT_RECURSION_X2)
    for (; i <= loop2_end - 2; i += 2) {
        do_recursion_x2(&array[i], &dsfmt->status[i], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
#endif
    for (; i < loop2_end; i++) {
        do_recursion(&array[i], &dsfmt->status[i], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
    }

    const ptrdiff_t loop3_end = size - DSFMT_N;
#if defined(DSFMT_RECURSION_X4)
    for (; i <= loop3_end - 4; i += 4) {
        do_recursion_x4(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
#endif
#if defined(DSFMT_RECURSION_X3)
    for (; i <= loop3_end - 3; i += 3) {
        do_recursion_x3(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
#endif
#if defined(DSFMT_RECURSION_X2)
    for (; i <= loop3_end - 2; i += 2) {
        do_recursion_x2(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
#endif
    for (; i < loop3_end; i++) {
        do_recursion(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
    }

    const ptrdiff_t loop4_end = 2 * DSFMT_N - size;
    for (j = 0; j < loop4_end; j++) {
        dsfmt->status[j] = array[j + size - DSFMT_N];
    }

    const ptrdiff_t loop5_end = size;
#if defined(DSFMT_RECURSION_X4)
    for (; i <= loop5_end - 4; i += 4, j += 4) {
        do_recursion_x4(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
        dsfmt->status[j]     = array[i];
        dsfmt->status[j + 1] = array[i + 1];
        dsfmt->status[j + 2] = array[i + 2];
        dsfmt->status[j + 3] = array[i + 3];
    }
#endif
#if defined(DSFMT_RECURSION_X3)
    for (; i <= loop5_end - 3; i += 3, j += 3) {
        do_recursion_x3(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
        dsfmt->status[j]     = array[i];
        dsfmt->status[j + 1] = array[i + 1];
        dsfmt->status[j + 2] = array[i + 2];
    }
#endif
#if defined(DSFMT_RECURSION_X2)
    for (; i <= loop5_end - 2; i += 2, j += 2) {
        do_recursion_x2(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
        dsfmt->status[j]     = array[i];
        dsfmt->status[j + 1] = array[i + 1];   
    }
#endif
    for (; i < loop5_end; i++, j++) {
        do_recursion(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
        dsfmt->status[j] = array[i];   
    }
    dsfmt->status[DSFMT_N] = lung;
}
#endif //DSFMT_GEN_RAND_CUSTOM
#endif
