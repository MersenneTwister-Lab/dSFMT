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
#if defined(__AVX__)
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

#elif defined(__aarch64__)
  #if !defined(ARM64_NO_NEON)
    #define HAVE_NEON
    #if defined(__ARM_FEATURE_SHA3) && !defined(ARM64_NO_SHA3)
      #define HAVE_SHA3
    #endif 
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
#elif defined(HAVE_NEON) 
#include <arm_neon.h>
/**
* @brief word_reverse
* @param a 128-bit (4x 32-bit) array
* @return reversed array (32-bit)
*/
inline static uint64x2_t word_reverse(uint64x2_t a)
{
    uint8x16_t WORD_REV = {12,13,14,15, 8,9,10,11, 4,5,6,7, 0,1,2,3};
    uint8x16_t pat = vreinterpretq_u8_u64(a);
    uint8x16_t rev = vqtbl1q_u8(pat, WORD_REV);
    return vreinterpretq_u64_u8(rev);
} 

const uint64x2_t NEON_DSFMT_MSK = {DSFMT_MSK1, DSFMT_MSK2};
const uint64x2_t NEON_DSFMT_MSK_NEG = {~DSFMT_MSK1, ~DSFMT_MSK2};

/**
* do_recursion_neon
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_neon(w128_t *r, w128_t *a, w128_t *b, w128_t *lung) 
{
    uint64x2_t v, w, x, y, z;
    x = vld1q_u64((void*)a);
    v = vld1q_u64((void*)b);
    w = veorq_u64(x, NEON_DSFMT_MSK);
    z = vshlq_n_u64(x, DSFMT_SL1);
    z = veorq_u64(z, v);
    y = word_reverse(lung->vqi64);
    y = veorq_u64(y, z);
    lung->vqi64 = y;
    v = vshrq_n_u64(y, DSFMT_SR);
    w = vbslq_u64(y, w, x);
    v = veorq_u64(v, w);
    vst1q_u64( (void*)r, v);
} 

/**
* do_recursion_x2_neon
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_x2_neon(w128_t *r, w128_t *a, w128_t *b, w128_t *lung) 
{
    uint64x2x2_t v, w, x, y, z;

    x = vld1q_u64_x2((void*)a);
    v = vld1q_u64_x2((void*)b);
    w.val[0] = veorq_u64(x.val[0], NEON_DSFMT_MSK);
    w.val[1] = veorq_u64(x.val[1], NEON_DSFMT_MSK);

    z.val[0] = vshlq_n_u64(x.val[0], DSFMT_SL1);
    z.val[1] = vshlq_n_u64(x.val[1], DSFMT_SL1);

    z.val[0] = veorq_u64(z.val[0], v.val[0]);
    z.val[1] = veorq_u64(z.val[1], v.val[1]);

    v.val[0] = word_reverse(z.val[0]);

    y.val[0] = word_reverse(lung->vqi64);
    y.val[1] = lung->vqi64;

    y.val[0] = veorq_u64(y.val[0], z.val[0]);
    y.val[1] = veorq_u64(y.val[1], veorq_u64(z.val[1], v.val[0]));

    lung->vqi64 = y.val[1];

    v.val[0] = vshrq_n_u64(y.val[0], DSFMT_SR);
    v.val[1] = vshrq_n_u64(y.val[1], DSFMT_SR);

    w.val[0] = vbslq_u64(y.val[0], w.val[0], x.val[0]);
    w.val[1] = vbslq_u64(y.val[1], w.val[1], x.val[1]);

    v.val[0] = veorq_u64(v.val[0], w.val[0]);
    v.val[1] = veorq_u64(v.val[1], w.val[1]);

    vst1q_u64_x2( (void*)r, v);
} 

/**
* do_recursion_x3_neon
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_x3_neon(w128_t *r, w128_t *a, w128_t *b, w128_t *lung) 
{
    uint64x2x3_t v, w, x, y, z;

    x = vld1q_u64_x3((void*)a);
    v = vld1q_u64_x3((void*)b);
    w.val[0] = veorq_u64(x.val[0], NEON_DSFMT_MSK);
    w.val[1] = veorq_u64(x.val[1], NEON_DSFMT_MSK);
    w.val[2] = veorq_u64(x.val[2], NEON_DSFMT_MSK);

    z.val[0] = vshlq_n_u64(x.val[0], DSFMT_SL1);
    z.val[1] = vshlq_n_u64(x.val[1], DSFMT_SL1);
    z.val[2] = vshlq_n_u64(x.val[2], DSFMT_SL1);

    z.val[0] = veorq_u64(z.val[0], v.val[0]);
    z.val[1] = veorq_u64(z.val[1], v.val[1]);
    z.val[2] = veorq_u64(z.val[2], v.val[2]);

    v.val[0] = word_reverse(z.val[0]);
    v.val[1] = word_reverse(z.val[1]);

    y.val[0] = word_reverse(lung->vqi64);
    y.val[1] = lung->vqi64;

    y.val[0] = veorq_u64(y.val[0], z.val[0]);
    y.val[1] = veorq_u64(y.val[1], veorq_u64(z.val[1], v.val[0]));
    y.val[2] = veorq_u64(y.val[0], veorq_u64(z.val[2], v.val[1]));

    lung->vqi64 = y.val[2];

    v.val[0] = vshrq_n_u64(y.val[0], DSFMT_SR);
    v.val[1] = vshrq_n_u64(y.val[1], DSFMT_SR);
    v.val[2] = vshrq_n_u64(y.val[2], DSFMT_SR);

    w.val[0] = vbslq_u64(y.val[0], w.val[0], x.val[0]);
    w.val[1] = vbslq_u64(y.val[1], w.val[1], x.val[1]);
    w.val[2] = vbslq_u64(y.val[2], w.val[2], x.val[2]);

    v.val[0] = veorq_u64(v.val[0], w.val[0]);
    v.val[1] = veorq_u64(v.val[1], w.val[1]);
    v.val[2] = veorq_u64(v.val[2], w.val[2]);

    vst1q_u64_x3( (void*)r, v);
} 

/**
* do_recursion_x4_neon
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_x4_neon(w128_t *r, w128_t *a, w128_t *b, w128_t *lung) 
{
    uint64x2x4_t v, w, x, y, z;

    x = vld1q_u64_x4((void*)a);
    v = vld1q_u64_x4((void*)b);
    w.val[0] = veorq_u64(x.val[0], NEON_DSFMT_MSK);
    w.val[1] = veorq_u64(x.val[1], NEON_DSFMT_MSK);
    w.val[2] = veorq_u64(x.val[2], NEON_DSFMT_MSK);
    w.val[3] = veorq_u64(x.val[3], NEON_DSFMT_MSK);

    z.val[0] = vshlq_n_u64(x.val[0], DSFMT_SL1);
    z.val[1] = vshlq_n_u64(x.val[1], DSFMT_SL1);
    z.val[2] = vshlq_n_u64(x.val[2], DSFMT_SL1);
    z.val[3] = vshlq_n_u64(x.val[3], DSFMT_SL1);

    z.val[0] = veorq_u64(z.val[0], v.val[0]);
    z.val[1] = veorq_u64(z.val[1], v.val[1]);
    z.val[2] = veorq_u64(z.val[2], v.val[2]);
    z.val[3] = veorq_u64(z.val[3], v.val[3]);

    v.val[0] = word_reverse(z.val[0]);
    v.val[1] = word_reverse(z.val[1]);
    v.val[2] = word_reverse(z.val[2]);

    y.val[0] = word_reverse(lung->vqi64);
    y.val[1] = lung->vqi64;

    y.val[0] = veorq_u64(y.val[0], z.val[0]);
    y.val[1] = veorq_u64(y.val[1], veorq_u64(z.val[1], v.val[0]));
    y.val[2] = veorq_u64(y.val[0], veorq_u64(z.val[2], v.val[1]));
    y.val[3] = veorq_u64(y.val[1], veorq_u64(z.val[3], v.val[2]));

    lung->vqi64 = y.val[3];

    v.val[0] = vshrq_n_u64(y.val[0], DSFMT_SR);
    v.val[1] = vshrq_n_u64(y.val[1], DSFMT_SR);
    v.val[2] = vshrq_n_u64(y.val[2], DSFMT_SR);
    v.val[3] = vshrq_n_u64(y.val[3], DSFMT_SR);

    w.val[0] = vbslq_u64(y.val[0], w.val[0], x.val[0]);
    w.val[1] = vbslq_u64(y.val[1], w.val[1], x.val[1]);
    w.val[2] = vbslq_u64(y.val[2], w.val[2], x.val[2]);
    w.val[3] = vbslq_u64(y.val[3], w.val[3], x.val[3]);

    v.val[0] = veorq_u64(v.val[0], w.val[0]);
    v.val[1] = veorq_u64(v.val[1], w.val[1]);
    v.val[2] = veorq_u64(v.val[2], w.val[2]);
    v.val[3] = veorq_u64(v.val[3], w.val[3]);

    vst1q_u64_x4( (void*)r, v);
} 

#if defined(HAVE_SHA3)
/**
* do_recursion_sha3
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_sha3(w128_t *r, w128_t *a, w128_t *b, w128_t *lung) 
{
    uint64x2_t v, w, x, y, z;
    x = vld1q_u64((void*)a);
    v = vld1q_u64((void*)b);
    z = vshlq_n_u64(x, DSFMT_SL1);
    z = veorq_u64(z, v);
    y = word_reverse(lung->vqi64);
    y = veorq_u64(y, z);
    lung->vqi64 = y;
    v = vshrq_n_u64(y, DSFMT_SR);
    w = vbcaxq_u64(x, y, NEON_DSFMT_MSK_NEG);
    v = veorq_u64(v, w);
    vst1q_u64( (void*)r, v);
} 

/**
* do_recursion_x2_sha3
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_x2_sha3(w128_t *r, w128_t *a, w128_t *b, w128_t *lung) 
{
    uint64x2x2_t v, w, x, y, z;

    x = vld1q_u64_x2((void*)a);
    v = vld1q_u64_x2((void*)b);

    z.val[0] = vshlq_n_u64(x.val[0], DSFMT_SL1);
    z.val[1] = vshlq_n_u64(x.val[1], DSFMT_SL1);

    z.val[0] = veorq_u64(z.val[0], v.val[0]);
    z.val[1] = veorq_u64(z.val[1], v.val[1]);

    v.val[0] = word_reverse(z.val[0]);

    y.val[0] = word_reverse(lung->vqi64);
    y.val[1] = lung->vqi64;

    y.val[0] = veorq_u64(y.val[0], z.val[0]);
    y.val[1] = veor3q_u64(y.val[1], z.val[1], v.val[0]);

    lung->vqi64 = y.val[1];

    v.val[0] = vshrq_n_u64(y.val[0], DSFMT_SR);
    v.val[1] = vshrq_n_u64(y.val[1], DSFMT_SR);

    w.val[0] = vbcaxq_u64(x.val[0], y.val[0], NEON_DSFMT_MSK_NEG);
    w.val[1] = vbcaxq_u64(x.val[1], y.val[1], NEON_DSFMT_MSK_NEG);

    v.val[0] = veorq_u64(v.val[0], w.val[0]);
    v.val[1] = veorq_u64(v.val[1], w.val[1]);

    vst1q_u64_x2( (void*)r, v);
} 

/**
* do_recursion_x3_sha3
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_x3_sha3(w128_t *r, w128_t *a, w128_t *b, w128_t *lung) 
{
    uint64x2x3_t v, w, x, y, z;

    x = vld1q_u64_x3((void*)a);
    v = vld1q_u64_x3((void*)b);

    z.val[0] = vshlq_n_u64(x.val[0], DSFMT_SL1);
    z.val[1] = vshlq_n_u64(x.val[1], DSFMT_SL1);
    z.val[2] = vshlq_n_u64(x.val[2], DSFMT_SL1);

    z.val[0] = veorq_u64(z.val[0], v.val[0]);
    z.val[1] = veorq_u64(z.val[1], v.val[1]);
    z.val[2] = veorq_u64(z.val[2], v.val[2]);

    v.val[0] = word_reverse(z.val[0]);
    v.val[1] = word_reverse(z.val[1]);

    y.val[0] = word_reverse(lung->vqi64);
    y.val[1] = lung->vqi64;

    y.val[0] = veorq_u64(y.val[0], z.val[0]);
    y.val[1] = veor3q_u64(y.val[1], z.val[1], v.val[0]);
    y.val[2] = veor3q_u64(y.val[0], z.val[2], v.val[1]);

    lung->vqi64 = y.val[2];

    v.val[0] = vshrq_n_u64(y.val[0], DSFMT_SR);
    v.val[1] = vshrq_n_u64(y.val[1], DSFMT_SR);
    v.val[2] = vshrq_n_u64(y.val[2], DSFMT_SR);

    w.val[0] = vbcaxq_u64(x.val[0], y.val[0], NEON_DSFMT_MSK_NEG);
    w.val[1] = vbcaxq_u64(x.val[1], y.val[1], NEON_DSFMT_MSK_NEG);
    w.val[2] = vbcaxq_u64(x.val[2], y.val[2], NEON_DSFMT_MSK_NEG);

    v.val[0] = veorq_u64(v.val[0], w.val[0]);
    v.val[1] = veorq_u64(v.val[1], w.val[1]);
    v.val[2] = veorq_u64(v.val[2], w.val[2]);

    vst1q_u64_x3( (void*)r, v);
} 

/**
* do_recursion_x4_sha3
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_x4_sha3(w128_t *r, w128_t *a, w128_t *b, w128_t *lung) 
{
    uint64x2x4_t v, w, x, y, z;

    x = vld1q_u64_x4((void*)a);
    v = vld1q_u64_x4((void*)b);

    z.val[0] = vshlq_n_u64(x.val[0], DSFMT_SL1);
    z.val[1] = vshlq_n_u64(x.val[1], DSFMT_SL1);
    z.val[2] = vshlq_n_u64(x.val[2], DSFMT_SL1);
    z.val[3] = vshlq_n_u64(x.val[3], DSFMT_SL1);

    z.val[0] = veorq_u64(z.val[0], v.val[0]);
    z.val[1] = veorq_u64(z.val[1], v.val[1]);
    z.val[2] = veorq_u64(z.val[2], v.val[2]);
    z.val[3] = veorq_u64(z.val[3], v.val[3]);

    v.val[0] = word_reverse(z.val[0]);
    v.val[1] = word_reverse(z.val[1]);
    v.val[2] = word_reverse(z.val[2]);

    y.val[0] = word_reverse(lung->vqi64);
    y.val[1] = lung->vqi64;

    y.val[0] = veorq_u64(y.val[0], z.val[0]);
    y.val[1] = veor3q_u64(y.val[1], z.val[1], v.val[0]);
    y.val[2] = veor3q_u64(y.val[0], z.val[2], v.val[1]);
    y.val[3] = veor3q_u64(y.val[1], z.val[3], v.val[2]);

    lung->vqi64 = y.val[3];

    v.val[0] = vshrq_n_u64(y.val[0], DSFMT_SR);
    v.val[1] = vshrq_n_u64(y.val[1], DSFMT_SR);
    v.val[2] = vshrq_n_u64(y.val[2], DSFMT_SR);
    v.val[3] = vshrq_n_u64(y.val[3], DSFMT_SR);

    w.val[0] = vbcaxq_u64(x.val[0], y.val[0], NEON_DSFMT_MSK_NEG);
    w.val[1] = vbcaxq_u64(x.val[1], y.val[1], NEON_DSFMT_MSK_NEG);
    w.val[2] = vbcaxq_u64(x.val[2], y.val[2], NEON_DSFMT_MSK_NEG);
    w.val[3] = vbcaxq_u64(x.val[3], y.val[3], NEON_DSFMT_MSK_NEG);

    v.val[0] = veorq_u64(v.val[0], w.val[0]);
    v.val[1] = veorq_u64(v.val[1], w.val[1]);
    v.val[2] = veorq_u64(v.val[2], w.val[2]);
    v.val[3] = veorq_u64(v.val[3], w.val[3]);

    vst1q_u64_x4( (void*)r, v);
} 
#endif /* HAVE_SHA3 */

#define DO_RECURSION_X2
#define DO_RECURSION_X3
#define DO_RECURSION_X4

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
