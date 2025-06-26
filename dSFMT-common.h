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
 * All rights reserved.
 *
 * The 3-clause BSD License is applied to this software, see
 * LICENSE.txt
 */
#ifndef DSFMT_COMMON_H
#define DSFMT_COMMON_H

#include "dSFMT.h"

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

#if defined(HAVE_ALTIVEC)
inline static void do_recursion(w128_t *r, w128_t *a, w128_t * b,
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
#elif defined(HAVE_SSE2)
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
#if defined(__AVX512VL__)
    y = _mm_ternarylogic_epi32(y, z, b->si, 0x96);
#else
    z = _mm_xor_si128(z, b->si);
    y = _mm_xor_si128(y, z);
#endif
    v = _mm_srli_epi64(y, DSFMT_SR);
    w = _mm_and_si128(y, sse2_param_mask.i128);
#if defined(__AVX512VL__)
    v = _mm_ternarylogic_epi32(v, x, w, 0x96);
#else
    v = _mm_xor_si128(v, x);
    v = _mm_xor_si128(v, w);
#endif
    r->si = v;
    u->si = y;
}

/**
 * This function represents the recursion formula.
 * @param a a 128-bit part of the internal state array
 * @param b a 128-bit part of the internal state array
 * @param u 128-bit (expand to 256-bit) part of the internal state array (I/O)
 * @return output 256-bit
 */

inline static void do_recursion_x2(w128_t *r, w128_t *a, w128_t *b, w128_t *u) {
    __m128i v1, w1, x1, y1, z1;
    __m128i v2, w2, x2, y2, z2;

    x1 = a[0].si;
    x2 = a[1].si;

    z1 = _mm_slli_epi64(x1, DSFMT_SL1);
    z2 = _mm_slli_epi64(x2, DSFMT_SL1);

    z1 = _mm_xor_si128(z1, b[0].si);
    z2 = _mm_xor_si128(z2, b[1].si);
    
    v1 = _mm_shuffle_epi32(z1, SSE2_SHUFF);
    z2 = _mm_xor_si128(z2, v1);

    y2 = u->si;
    y1 = _mm_shuffle_epi32(y2, SSE2_SHUFF);
    y1 = _mm_xor_si128(y1, z1);
    y2 = _mm_xor_si128(y2, z2);
    u->si = y2;

    v1 = _mm_srli_epi64(y1, DSFMT_SR);
    v2 = _mm_srli_epi64(y2, DSFMT_SR);
    w1 = _mm_and_si128(y1, sse2_param_mask.i128);
    w2 = _mm_and_si128(y2, sse2_param_mask.i128);
    v1 = _mm_xor_si128(v1, x1);
    v2 = _mm_xor_si128(v2, x2);
    v1 = _mm_xor_si128(v1, w1);
    v2 = _mm_xor_si128(v2, w2);

    r[0].si = v1;
    r[1].si = v2;
}

inline static void do_recursion_x3(w128_t *r, w128_t *a, w128_t *b, w128_t *u) {
    do_recursion_x2(r, a, b, u);
    do_recursion(r+2, a+2, b+2, u);
}

#if DSFMT_MEXP == 1279
#define DSFMT_RECURSION_X3
#elif DSFMT_N - DSFMT_POS1 > 1 
#define DSFMT_RECURSION_X2
#endif
 
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
inline static void do_recursion(w128_t *r, w128_t *a, w128_t * b,
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
#endif
