#pragma once
/**
 * @file dSFMT-neon.h
 *
 * @brief SIMD oriented Fast Mersenne Twister(SFMT) pseudorandom
 * number generator with jump function. This file includes common functions
 * used in random number generation and jump.
 *
 * @author Masaki Ota
 *
 * Copyright (C) 2025 Masaki Ota.
 *
 * The 3-clause BSD License is applied to this software, see
 * LICENSE.txt
 */
#ifndef DSFMT_NEON_H
#define DSFMT_NEON_H

#include <arm_neon.h>
#include "dSFMT-params.h"

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

#if DSFMT_MEXP == 521
/* nothing */
#elif DSFMT_MEXP == 1279
#define DSFMT_RECURSION_X3
#else
#define DSFMT_RECURSION_X4
#define DSFMT_RECURSION_X2
#endif

/**
* do_recursion_neon
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion(w128_t *r, w128_t *a, w128_t *b, w128_t *u) 
{
    uint64x2_t lung, lung_rev;
    uint64x2_t v, w, x, y, z;
    x = vld1q_u64((void*)a);
    v = vld1q_u64((void*)b);
    z = vshlq_n_u64(x, DSFMT_SL1);

    lung = vld1q_u64((void*)u);
    lung_rev = word_reverse(lung);

#if defined(HAVE_SHA3)
    y = veor3q_u64(z, v, lung_rev);
#else
    w = veorq_u64(x, NEON_DSFMT_MSK);
    z = veorq_u64(z, v);
    y = veorq_u64(z, lung_rev);
#endif
    vst1q_u64((void*)u, y);

    v = vshrq_n_u64(y, DSFMT_SR);
#if defined(HAVE_SHA3)
    w = vbcaxq_u64(x, y, NEON_DSFMT_MSK_NEG);
#else
    w = vbslq_u64(y, w, x);
#endif
    v = veorq_u64(v, w);
    vst1q_u64((void*)r, v);
} 

/**
* do_recursion_x2
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_x2(w128_t *r, w128_t *a, w128_t *b, w128_t *u) 
{
    uint64x2_t lung, lung_rev;
    uint64x2x2_t v, w, x, y, z;

    x = vld1q_u64_x2((void*)a);
    v = vld1q_u64_x2((void*)b);

    z.val[0] = vshlq_n_u64(x.val[0], DSFMT_SL1);
    z.val[0] = veorq_u64(z.val[0], v.val[0]);
    z.val[1] = vshlq_n_u64(x.val[1], DSFMT_SL1);
    z.val[1] = veorq_u64(z.val[1], v.val[1]);

    v.val[0] = word_reverse(z.val[0]);


#if defined(HAVE_SHA3)
    lung = vld1q_u64((void*)u);
    lung_rev = word_reverse(lung);
    y.val[0] = veorq_u64(z.val[0], lung_rev);
    y.val[1] = veor3q_u64(z.val[1], v.val[0], lung);
#else
    w.val[0] = veorq_u64(x.val[0], NEON_DSFMT_MSK);
    w.val[1] = veorq_u64(x.val[1], NEON_DSFMT_MSK);
    z.val[1] = veorq_u64(z.val[1], v.val[0]);

    lung = vld1q_u64((void*)u);
    lung_rev = word_reverse(lung);
    y.val[0] = veorq_u64(z.val[0], lung_rev);
    y.val[1] = veorq_u64(z.val[1], lung);
#endif
    vst1q_u64((void*)u, y.val[1]);

    v.val[0] = vshrq_n_u64(y.val[0], DSFMT_SR);
    v.val[1] = vshrq_n_u64(y.val[1], DSFMT_SR);

#if defined(HAVE_SHA3)
    w.val[0] = vbcaxq_u64(x.val[0], y.val[0], NEON_DSFMT_MSK_NEG);
    w.val[1] = vbcaxq_u64(x.val[1], y.val[1], NEON_DSFMT_MSK_NEG);
#else
    w.val[0] = vbslq_u64(y.val[0], w.val[0], x.val[0]);
    w.val[1] = vbslq_u64(y.val[1], w.val[1], x.val[1]);
#endif

    v.val[0] = veorq_u64(v.val[0], w.val[0]);
    v.val[1] = veorq_u64(v.val[1], w.val[1]);

    vst1q_u64_x2((void*)r, v);
} 


/**
* do_recursion_x4
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_x4(w128_t *r, w128_t *a, w128_t *b, w128_t *u) 
{
    uint64x2_t lung, lung_rev;
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

#if defined (HAVE_SHA3)
    y.val[2] = veor3q_u64(z.val[2], v.val[1], z.val[0]);
    y.val[3] = veor3q_u64(z.val[3], v.val[2], z.val[1]);

    lung = vld1q_u64((void*)u);
    lung_rev = word_reverse(lung);
    y.val[0] = veorq_u64(z.val[0], lung_rev);
    y.val[1] = veor3q_u64(z.val[1], v.val[0], lung);
    y.val[2] = veorq_u64(y.val[2], lung_rev);
    y.val[3] = veor3q_u64(y.val[3], v.val[0], lung);
    vst1q_u64((void*)u, y.val[3]);
#else
    w.val[0] = veorq_u64(x.val[0], NEON_DSFMT_MSK);
    w.val[1] = veorq_u64(x.val[1], NEON_DSFMT_MSK);
    w.val[2] = veorq_u64(x.val[2], NEON_DSFMT_MSK);
    w.val[3] = veorq_u64(x.val[3], NEON_DSFMT_MSK);

    z.val[1] = veorq_u64(z.val[1], v.val[0]);
    z.val[2] = veorq_u64(z.val[2], v.val[1]);
    z.val[2] = veorq_u64(z.val[2], z.val[0]);
    z.val[3] = veorq_u64(z.val[3], v.val[2]);
    z.val[3] = veorq_u64(z.val[3], z.val[1]);

    lung = vld1q_u64((void*)u);
    lung_rev = word_reverse(lung);
    y.val[0] = veorq_u64(z.val[0], lung_rev);
    y.val[1] = veorq_u64(z.val[1], lung);
    y.val[2] = veorq_u64(z.val[2], lung_rev);
    y.val[3] = veorq_u64(z.val[3], lung);
    vst1q_u64((void*)u, y.val[3]);
#endif

    v.val[0] = vshrq_n_u64(y.val[0], DSFMT_SR);
    v.val[1] = vshrq_n_u64(y.val[1], DSFMT_SR);
    v.val[2] = vshrq_n_u64(y.val[2], DSFMT_SR);
    v.val[3] = vshrq_n_u64(y.val[3], DSFMT_SR);
 
#if defined(HAVE_SHA3)
    w.val[0] = vbcaxq_u64(x.val[0], y.val[0], NEON_DSFMT_MSK_NEG);
    w.val[1] = vbcaxq_u64(x.val[1], y.val[1], NEON_DSFMT_MSK_NEG);
    w.val[2] = vbcaxq_u64(x.val[2], y.val[2], NEON_DSFMT_MSK_NEG);
    w.val[3] = vbcaxq_u64(x.val[3], y.val[3], NEON_DSFMT_MSK_NEG);
#else
    w.val[0] = vbslq_u64(y.val[0], w.val[0], x.val[0]);
    w.val[1] = vbslq_u64(y.val[1], w.val[1], x.val[1]);
    w.val[2] = vbslq_u64(y.val[2], w.val[2], x.val[2]);
    w.val[3] = vbslq_u64(y.val[3], w.val[3], x.val[3]);
#endif

    v.val[0] = veorq_u64(v.val[0], w.val[0]);
    v.val[1] = veorq_u64(v.val[1], w.val[1]);
    v.val[2] = veorq_u64(v.val[2], w.val[2]);
    v.val[3] = veorq_u64(v.val[3], w.val[3]);

    vst1q_u64_x4((void*)r, v);
} 

/**
* do_recursion_x3
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_x3(w128_t *r, w128_t *a, w128_t *b, w128_t *u) 
{
    uint64x2_t lung, lung_rev;
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

#if defined (HAVE_SHA3)
    y.val[2] = veor3q_u64(z.val[2], v.val[1], z.val[0]);

    lung = vld1q_u64((void*)u);
    lung_rev = word_reverse(lung);
    y.val[0] = veorq_u64(z.val[0], lung_rev);
    y.val[1] = veor3q_u64(z.val[1], v.val[0], lung);
    y.val[2] = veorq_u64(y.val[2], lung_rev);
    vst1q_u64((void*)u, y.val[2]);
#else
    w.val[0] = veorq_u64(x.val[0], NEON_DSFMT_MSK);
    w.val[1] = veorq_u64(x.val[1], NEON_DSFMT_MSK);
    w.val[2] = veorq_u64(x.val[2], NEON_DSFMT_MSK);

    z.val[1] = veorq_u64(z.val[1], v.val[0]);
    z.val[2] = veorq_u64(z.val[2], v.val[1]);
    z.val[2] = veorq_u64(z.val[2], z.val[0]);

    lung = vld1q_u64((void*)u);
    lung_rev = word_reverse(lung);
    y.val[0] = veorq_u64(z.val[0], lung_rev);
    y.val[1] = veorq_u64(z.val[1], lung);
    y.val[2] = veorq_u64(z.val[2], lung_rev);
    vst1q_u64((void*)u, y.val[2]);
#endif

    v.val[0] = vshrq_n_u64(y.val[0], DSFMT_SR);
    v.val[1] = vshrq_n_u64(y.val[1], DSFMT_SR);
    v.val[2] = vshrq_n_u64(y.val[2], DSFMT_SR);
 
#if defined(HAVE_SHA3)
    w.val[0] = vbcaxq_u64(x.val[0], y.val[0], NEON_DSFMT_MSK_NEG);
    w.val[1] = vbcaxq_u64(x.val[1], y.val[1], NEON_DSFMT_MSK_NEG);
    w.val[2] = vbcaxq_u64(x.val[2], y.val[2], NEON_DSFMT_MSK_NEG);
#else
    w.val[0] = vbslq_u64(y.val[0], w.val[0], x.val[0]);
    w.val[1] = vbslq_u64(y.val[1], w.val[1], x.val[1]);
    w.val[2] = vbslq_u64(y.val[2], w.val[2], x.val[2]);
#endif

    v.val[0] = veorq_u64(v.val[0], w.val[0]);
    v.val[1] = veorq_u64(v.val[1], w.val[1]);
    v.val[2] = veorq_u64(v.val[2], w.val[2]);

    vst1q_u64_x3((void*)r, v);
} 
#endif
