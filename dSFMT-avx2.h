#pragma once
/**
 * @file dSFMT-avx2.h
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
#ifndef DSFMT_AVX2_H
#define DSFMT_AVX2_H

#include <immintrin.h>
#include "dSFMT-params.h"
 
/**
 * This function represents the recursion formula.
 * @param a a 128-bit part of the internal state array
 * @param b a 128-bit part of the internal state array
 * @param u 128-bit (expand to 256-bit) part of the internal state array (I/O)
 * @return output 256-bit
 */
inline static void do_recursion_x2_avx2(w128_t *r, w128_t *a, w128_t *b, __m256i *u) {
    __m256i v, w, x, y, z;
    __m256i lung_shuf_idx = _mm256_setr_epi32(7, 6, 5, 4, 4, 5, 6, 7);
    __m256i word_reverse_idx = _mm256_setr_epi32(0, 1, 2, 3, 3, 2, 1, 0);
    __m256i dsfmt_mask = _mm256_broadcastsi128_si256(sse2_param_mask.i128);
    __m256i z_mask = _mm256_zextsi128_si256(_mm_set1_epi8(0xFF));

    x = _mm256_loadu_si256((__m256i*)a);
    z = _mm256_slli_epi64(x, DSFMT_SL1);
    z = _mm256_xor_si256(z, _mm256_loadu_si256((__m256i*)b));

    v = _mm256_permutevar8x32_epi32(z, word_reverse_idx);
    z = _mm256_andnot_si256(z_mask, z);
    z = _mm256_xor_si256(z, v);

    w = *u;
#if 0
    y = _mm256_permutevar8x32_epi32(w, lung_shuf_idx);
    *u = _mm256_xor_si256(w, z);
    y = _mm256_xor_si256(y, z); 
#else
    y = _mm256_permutevar8x32_epi32(w, lung_shuf_idx);
    y = _mm256_xor_si256(y, z);
    *u = y;

#endif
    v = _mm256_srli_epi64(y, DSFMT_SR);
    w = _mm256_and_si256(y, dsfmt_mask);

#if defined(HAVE_AVX512)
    v = _mm256_ternarylogic_epi32(v, w, x, 0x96);
#else    
    v = _mm256_xor_si256(v, x);
    v = _mm256_xor_si256(v, w);
#endif
    _mm256_storeu_si256((__m256i*)r, v);
}



/**
 * This function represents the recursion formula.
 * @param r output 128-bit
 * @param a a 128-bit part of the internal state array
 * @param b a 128-bit part of the internal state array
 * @param d a 128-bit part of the internal state array (I/O)
 */
inline static void do_recursion_x1_avx2(w128_t *r, w128_t* a, w128_t *b, __m256i *u) {
    __m128i v, w, x, y, z;
    __m256i lung_shuf_idx = _mm256_setr_epi32(7, 6, 5, 4, 4, 5, 6, 7);
    __m256i lung_extend;

    x = a->si;
    z = _mm_slli_epi64(x, DSFMT_SL1);

    lung_extend = _mm256_permutevar8x32_epi32(*u, lung_shuf_idx);
    y = _mm256_castsi256_si128(lung_extend);
#if defined(HAVE_AVX512)
    y = _mm_ternarylogic_epi32(y, z, b->si, 0x96);
#else
    z = _mm_xor_si128(z, b->si);
    y = _mm_xor_si128(y, z);
#endif
    lung_extend = _mm256_broadcastsi128_si256(y);
    *u = lung_extend;

    v = _mm_srli_epi64(y, DSFMT_SR);
    w = _mm_and_si128(y, sse2_param_mask.i128);
#if defined(HAVE_AVX512)
    v = _mm_ternarylogic_epi32(v, w, x, 0x96);
#else
    v = _mm_xor_si128(v, x);
    v = _mm_xor_si128(v, w);
#endif
    
    r->si = v;
}





#define MAKE_PAIR(a, b) _mm256_loadu2_m128i((__m128i*)(b), (__m128i*)(a))
#define LD256(a) _mm256_loadu_si256((__m256i*)(a))
#define LD128(a) _mm_loadu_si128((__m128i*)(a))

#define ST256(addr, v) _mm256_storeu_si256((__m256i*)(addr), (v));
#define ST128(addr, v) _mm_storeu_si128((__m128i*)(addr), (v));

/**
 * This is the core function for generating pseudorandom numbers.
 * It fills the user-specified array and applies a given conversion.
 * (Unrolled version)
 * @param dsfmt dsfmt state vector.
 * @param array an 128-bit array to be filled by pseudorandom numbers.
 * @param size number of 128-bit pseudorandom numbers to be generated.
 */
static inline __attribute__((always_inline, unused)) 
void gen_rand_array(dsfmt_t *dsfmt, w128_t *array,
                                  ptrdiff_t size) {

    ptrdiff_t i, j;

    __m256i lung = _mm256_broadcastsi128_si256(dsfmt->status[DSFMT_N].si);

    i = 0;

    const ptrdiff_t loop1_end = DSFMT_N - DSFMT_POS1;
    for (; i <= loop1_end - 2; i += 2) {
        do_recursion_x2_avx2(&array[i], &dsfmt->status[i], &dsfmt->status[i + DSFMT_POS1], &lung);
    }
    if (i < loop1_end) {
        do_recursion_x1_avx2(&array[i], &dsfmt->status[i], &dsfmt->status[i + DSFMT_POS1], &lung);
        i++;
    }

    const ptrdiff_t loop2_end = DSFMT_N;
    for (; i <= loop2_end - 2; i += 2) {
        do_recursion_x2_avx2(&array[i], &dsfmt->status[i], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
    if (i < loop2_end) {
        do_recursion_x1_avx2(&array[i], &dsfmt->status[i], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
        i++;
    }

    const ptrdiff_t loop3_end = size - DSFMT_N;
    for (; i <= loop3_end - 2; i += 2) {
        do_recursion_x2_avx2(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
    if (i < loop3_end) {
        do_recursion_x1_avx2(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
        i++;
    }

    const ptrdiff_t loop4_end = 2 * DSFMT_N - size;
    for (j = 0; j < loop4_end-1; j+=2) {
        __m256i vec = _mm256_loadu_si256((__m256i*)&array[j + size - DSFMT_N]);
        _mm256_storeu_si256((__m256i*)&dsfmt->status[j], vec);
    }
    if (j < loop4_end) {
        dsfmt->status[j] = array[j + size - DSFMT_N];
        j++;
    }

    const ptrdiff_t loop5_end = size;
    for (; i <= loop5_end - 2; i += 2, j += 2) {
        do_recursion_x2_avx2(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
        _mm256_storeu_si256((__m256i*)&dsfmt->status[j], _mm256_loadu_si256((__m256i*)&array[i]));
    }
    if (i < loop5_end) {
        do_recursion_x1_avx2(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
        dsfmt->status[j] = array[i];
        i++, j++;
    }

    dsfmt->status[DSFMT_N].si =  _mm256_extracti128_si256(lung, 1);
}

/**
 * This function fills the internal state array with double precision
 * floating point pseudorandom numbers of the IEEE 754 format.
 * @param dsfmt dsfmt state vector.
 */
inline static void dsfmt_gen_rand_all_impl(dsfmt_t *dsfmt) {
    int i;
    __m256i lung = _mm256_broadcastsi128_si256(dsfmt->status[DSFMT_N].si);

    i = 0;
    
    for (; i <  DSFMT_N - 1; i += 2) {
        if (i < DSFMT_N - DSFMT_POS1 - 1) {
            do_recursion_x2_avx2(&dsfmt->status[i], 
                &dsfmt->status[i], &dsfmt->status[i + DSFMT_POS1], &lung);
        } else if (i == DSFMT_N - DSFMT_POS1 - 1) {
            w128_t tmp[2];
            tmp[0] = dsfmt->status[i + DSFMT_POS1];
            tmp[1] = dsfmt->status[0];
            do_recursion_x2_avx2(&dsfmt->status[i], 
                &dsfmt->status[i], tmp, &lung);
        } else {
            do_recursion_x2_avx2(&dsfmt->status[i], &dsfmt->status[i],
                        &dsfmt->status[i + DSFMT_POS1 - DSFMT_N], &lung);
        }
    }
    if (i < DSFMT_N) {
        do_recursion_x1_avx2(&dsfmt->status[i], &dsfmt->status[i],
                     &dsfmt->status[i + DSFMT_POS1 - DSFMT_N], &lung);
    }

    ST128(&dsfmt->status[DSFMT_N], _mm256_extracti128_si256(lung, 1));

}


#endif /* DSFMT_AVX2_H */

