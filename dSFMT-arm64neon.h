#pragma once

#ifndef DSFMT_ARM64NEON_H
#define DSFMT_ARM64NEON_H

#include <arm_neon.h>
#include "dSFMT-params.h"
//#include "dSFMT-common.h"

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
inline static void do_recursion_neon(w128_t *r, w128_t *a, w128_t *b, w128_t *u) 
{
    uint64x2_t lung, lung_rev;
    uint64x2_t v, w, x, y, z;
    x = vld1q_u64((void*)a);
    v = vld1q_u64((void*)b);
    w = veorq_u64(x, NEON_DSFMT_MSK);
    z = vshlq_n_u64(x, DSFMT_SL1);
    z = veorq_u64(z, v);
    lung = vld1q_u64((void*)u);
    lung_rev = word_reverse(lung);
    y = veorq_u64(z, lung_rev);
    vst1q_u64((void*)u, y);
    v = vshrq_n_u64(y, DSFMT_SR);
    w = vbslq_u64(y, w, x);
    v = veorq_u64(v, w);
    vst1q_u64((void*)r, v);
} 


/**
* do_recursion_x2_neon
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_x2_neon(w128_t *r, w128_t *a, w128_t *b, w128_t *u) 
{
    uint64x2_t lung, lung_rev;
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

    z.val[1] = veorq_u64(z.val[1], v.val[0]);


    lung = vld1q_u64((void*)u);
    lung_rev = word_reverse(lung);
    y.val[0] = veorq_u64(z.val[0], lung_rev);
    y.val[1] = veorq_u64(z.val[1], lung);
    vst1q_u64((void*)u, y.val[1]);

    v.val[0] = vshrq_n_u64(y.val[0], DSFMT_SR);
    v.val[1] = vshrq_n_u64(y.val[1], DSFMT_SR);

    w.val[0] = vbslq_u64(y.val[0], w.val[0], x.val[0]);
    w.val[1] = vbslq_u64(y.val[1], w.val[1], x.val[1]);

    v.val[0] = veorq_u64(v.val[0], w.val[0]);
    v.val[1] = veorq_u64(v.val[1], w.val[1]);

    vst1q_u64_x2((void*)r, v);
} 


/**
* do_recursion_x3_neon
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_x3_neon(w128_t *r, w128_t *a, w128_t *b, w128_t *u) 
{
    uint64x2_t lung, lung_rev;
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

    z.val[1] = veorq_u64(z.val[1], v.val[0]);z.val[2] = veorq_u64(z.val[2], v.val[1]);


    lung = vld1q_u64((void*)u);
    lung_rev = word_reverse(lung);
    y.val[0] = veorq_u64(z.val[0], lung_rev);
    y.val[1] = veorq_u64(z.val[1], lung);
    y.val[2] = veorq_u64(z.val[2], lung_rev);
    vst1q_u64((void*)u, y.val[2]);

    v.val[0] = vshrq_n_u64(y.val[0], DSFMT_SR);
    v.val[1] = vshrq_n_u64(y.val[1], DSFMT_SR);
    v.val[2] = vshrq_n_u64(y.val[2], DSFMT_SR);

    w.val[0] = vbslq_u64(y.val[0], w.val[0], x.val[0]);
    w.val[1] = vbslq_u64(y.val[1], w.val[1], x.val[1]);
    w.val[2] = vbslq_u64(y.val[2], w.val[2], x.val[2]);

    v.val[0] = veorq_u64(v.val[0], w.val[0]);
    v.val[1] = veorq_u64(v.val[1], w.val[1]);
    v.val[2] = veorq_u64(v.val[2], w.val[2]);

    vst1q_u64_x3((void*)r, v);
} 


/**
* do_recursion_x4_neon
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_x4_neon(w128_t *r, w128_t *a, w128_t *b, w128_t *u) 
{
    uint64x2_t lung, lung_rev;
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

    z.val[1] = veorq_u64(z.val[1], v.val[0]);z.val[2] = veorq_u64(z.val[2], v.val[1]);z.val[3] = veorq_u64(z.val[3], v.val[2]);

    z.val[3] = veorq_u64(z.val[3], z.val[1]);

    lung = vld1q_u64((void*)u);
    lung_rev = word_reverse(lung);
    y.val[0] = veorq_u64(z.val[0], lung_rev);
    y.val[1] = veorq_u64(z.val[1], lung);
    y.val[2] = veorq_u64(z.val[2], lung_rev);
    y.val[3] = veorq_u64(z.val[3], lung);
    vst1q_u64((void*)u, y.val[3]);

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

    vst1q_u64_x4((void*)r, v);
} 


/**
* do_recursion_sha3
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_sha3(w128_t *r, w128_t *a, w128_t *b, w128_t *u) 
{
    uint64x2_t lung, lung_rev;
    uint64x2_t v, w, x, y, z;
    x = vld1q_u64((void*)a);
    v = vld1q_u64((void*)b);
    z = vshlq_n_u64(x, DSFMT_SL1);
    z = veorq_u64(z, v);
    lung = vld1q_u64((void*)u);
    lung_rev = word_reverse(lung);
    y = veorq_u64(z, lung_rev);
    vst1q_u64((void*)u, y);
    v = vshrq_n_u64(y, DSFMT_SR);
    w = vbcaxq_u64(x, y, NEON_DSFMT_MSK_NEG);
    v = veorq_u64(v, w);
    vst1q_u64((void*)r, v);
} 


/**
* do_recursion_x2_sha3
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_x2_sha3(w128_t *r, w128_t *a, w128_t *b, w128_t *u) 
{
    uint64x2_t lung, lung_rev;
    uint64x2x2_t v, w, x, y, z;

    x = vld1q_u64_x2((void*)a);
    v = vld1q_u64_x2((void*)b);


    z.val[0] = vshlq_n_u64(x.val[0], DSFMT_SL1);
    z.val[1] = vshlq_n_u64(x.val[1], DSFMT_SL1);

    z.val[0] = veorq_u64(z.val[0], v.val[0]);
    z.val[1] = veorq_u64(z.val[1], v.val[1]);

    v.val[0] = word_reverse(z.val[0]);

    lung = vld1q_u64((void*)u);
    lung_rev = word_reverse(lung);
    y.val[0] = veorq_u64(z.val[0], lung_rev);
    y.val[1] = veor3q_u64(z.val[1], v.val[0], lung);
    vst1q_u64((void*)u, y.val[1]);

    v.val[0] = vshrq_n_u64(y.val[0], DSFMT_SR);
    v.val[1] = vshrq_n_u64(y.val[1], DSFMT_SR);

    w.val[0] = vbcaxq_u64(x.val[0], y.val[0], NEON_DSFMT_MSK_NEG);
    w.val[1] = vbcaxq_u64(x.val[1], y.val[1], NEON_DSFMT_MSK_NEG);

    v.val[0] = veorq_u64(v.val[0], w.val[0]);
    v.val[1] = veorq_u64(v.val[1], w.val[1]);

    vst1q_u64_x2((void*)r, v);
} 


/**
* do_recursion_x3_sha3
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_x3_sha3(w128_t *r, w128_t *a, w128_t *b, w128_t *u) 
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

    y.val[2] = veor3q_u64(z.val[2], v.val[1], z.val[0]);
    lung = vld1q_u64((void*)u);
    lung_rev = word_reverse(lung);
    y.val[0] = veorq_u64(z.val[0], lung_rev);
    y.val[1] = veor3q_u64(z.val[1], v.val[0], lung);
    y.val[2] = veorq_u64(y.val[2], lung_rev);
    vst1q_u64((void*)u, y.val[2]);

    v.val[0] = vshrq_n_u64(y.val[0], DSFMT_SR);
    v.val[1] = vshrq_n_u64(y.val[1], DSFMT_SR);
    v.val[2] = vshrq_n_u64(y.val[2], DSFMT_SR);

    w.val[0] = vbcaxq_u64(x.val[0], y.val[0], NEON_DSFMT_MSK_NEG);
    w.val[1] = vbcaxq_u64(x.val[1], y.val[1], NEON_DSFMT_MSK_NEG);
    w.val[2] = vbcaxq_u64(x.val[2], y.val[2], NEON_DSFMT_MSK_NEG);

    v.val[0] = veorq_u64(v.val[0], w.val[0]);
    v.val[1] = veorq_u64(v.val[1], w.val[1]);
    v.val[2] = veorq_u64(v.val[2], w.val[2]);

    vst1q_u64_x3((void*)r, v);
} 


/**
* do_recursion_x4_sha3
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
inline static void do_recursion_x4_sha3(w128_t *r, w128_t *a, w128_t *b, w128_t *u) 
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

    y.val[2] = veor3q_u64(z.val[2], v.val[1], z.val[0]);
    y.val[3] = veor3q_u64(z.val[3], v.val[2], z.val[1]);
    lung = vld1q_u64((void*)u);
    lung_rev = word_reverse(lung);
    y.val[0] = veorq_u64(z.val[0], lung_rev);
    y.val[1] = veor3q_u64(z.val[1], v.val[0], lung);
    y.val[2] = veorq_u64(y.val[2], lung_rev);
    y.val[3] = veor3q_u64(y.val[3], v.val[0], lung);
    vst1q_u64((void*)u, y.val[3]);

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

    vst1q_u64_x4((void*)r, v);
} 


#define DO_RECURSION_X4
//#define DO_RECURSION_X3
#define DO_RECURSION_X2


#endif /* DSFMT_ARM64NEON_H */

