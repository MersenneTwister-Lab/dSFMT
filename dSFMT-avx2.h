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

    y = *u;
    y = _mm256_permutevar8x32_epi32(y, lung_shuf_idx);
    y = _mm256_xor_si256(y, z); 
    *u = y;

    v = _mm256_srli_epi64(y, DSFMT_SR);
    w = _mm256_and_si256(y, dsfmt_mask);
    v = _mm256_xor_si256(v, x);
    v = _mm256_xor_si256(v, w);
    _mm256_storeu_si256((__m256i*)r, v);
}

/**
 * This function represents the recursion formula.
 * @param r output 128-bit
 * @param a a 128-bit part of the internal state array
 * @param b a 128-bit part of the internal state array
 * @param d a 128-bit part of the internal state array (I/O)
 */
inline static void do_recursion_x1_avx2(w128_t *r, w128_t *a, w128_t *b, __m256i *u) {
    __m128i v, w, x, y, z;
    __m256i lung_shuf_idx = _mm256_setr_epi32(7, 6, 5, 4, 4, 5, 6, 7);
    __m256i lung_extend;

    x = a->si;
    z = _mm_slli_epi64(x, DSFMT_SL1);
    z = _mm_xor_si128(z, b->si);

    lung_extend = _mm256_permutevar8x32_epi32(*u, lung_shuf_idx);
    y = _mm256_castsi256_si128(lung_extend);
    y = _mm_xor_si128(y, z);
    lung_extend = _mm256_inserti128_si256(lung_extend, y, 1);)
    *u = lung_extend;

    v = _mm_srli_epi64(y, DSFMT_SR);
    w = _mm_and_si128(y, sse2_param_mask.i128);
    v = _mm_xor_si128(v, x);
    v = _mm_xor_si128(v, w);
    r->si = v;
}



inline static void no_convert_avx(const double* w) {}

inline static void convert_c0o1_avx256(const double* w) {
    __m256d result = _mm256_add_pd(_mm256_loadu_pd(w), _mm256_set1_pd(-1.0));
    _mm256_storeu_pd((double*)w, result);
}
inline static void convert_o0c1_avx256(const double* w) {
    __m256d result = _mm256_sub_pd(_mm256_set1_pd(2.0), _mm256_loadu_pd(w));
    _mm256_storeu_pd((double*)w, result);
}
inline static void convert_o0o1_avx256(const double* w) {
    __m256d mask = _mm256_castsi256_pd(_mm256_set1_epi64x(1));
    __m256d result = _mm256_or_pd(_mm256_loadu_pd(w), mask);
    result = _mm256_add_pd(result, _mm256_set1_pd(-1.0));
    _mm256_storeu_pd((double*)w, result);
}

inline static void convert_c0o1_avx128(const double* w) {
    __m128d result = _mm_add_pd(_mm_loadu_pd(w), _mm_set1_pd(-1.0));
    _mm_storeu_pd((double*)w, result);
}
inline static void convert_o0c1_avx128(const double* w) {
    __m128d result = _mm_sub_pd(_mm_set1_pd(2.0), _mm_loadu_pd(w));
    _mm_storeu_pd((double*)w, result);
}
inline static void convert_o0o1_avx128(const double* w) {
    __m128d mask = _mm_castsi128_pd(_mm_set1_epi64x(1));
    __m128d result = _mm_or_pd(_mm_loadu_pd(w), mask);    
    result = _mm_add_pd(result, _mm_set1_pd(-1.0));
    _mm_storeu_pd((double*)w, result);
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
 * @param converter a function pointer for value conversion.
 */

inline static void gen_rand_array(dsfmt_t *dsfmt, w128_t *array,
                                ptrdiff_t size, enum DSFMT_CONVERT_TYPE convType) {
    ptrdiff_t i, j;

    typedef void (*avx_converter_t)(const double *);
    avx_converter_t conv128 = no_convert_avx, conv256 = no_convert_avx;
    switch (convType)
    {
        case CONVERT_CLOSE0_OPEN1:
            conv128 = convert_c0o1_avx128;
            conv256 = convert_c0o1_avx256;
            break;
        case CONVERT_OPEN0_CLOSE1:
            conv128 = convert_o0c1_avx128;
            conv256 = convert_o0c1_avx256;
            break;
        case CONVERT_OPEN0_OPEN1:
            conv128 = convert_o0o1_avx128;
            conv256 = convert_o0o1_avx256;
            break;
        default: //NO_CONVERT_CLOSE1_OPEN2:
            break;
    }

    __m256i lung = _mm256_broadcastsi128_si256(dsfmt->status[DSFMT_N].si);
    i = 0;

    const ptrdiff_t loop1_end = DSFMT_N - DSFMT_POS1;
    for (; i <= loop1_end - 2; i += 2) {
        do_recursion_x2_avx2(&array[i], &dsfmt->status[i], &dsfmt->status[i + DSFMT_POS1], &lung);
    }

    if ((DSFMT_N - DSFMT_POS1) & 1)
    {
        w128_t b[2];
        b[0] = dsfmt->status[i + DSFMT_POS1];
        b[1] = dsfmt->status[0];
        do_recursion_x2_avx2(&dsfmt->status[i], &dsfmt->status[i], b, &lung);
        i+=2;
    }
    // ループ2: 状態ベクトル後半と生成済み配列の一部を使って生成
    const ptrdiff_t loop2_end = DSFMT_N;
    for (; i <= loop2_end - 2; i += 2) {
        do_recursion_x2_avx2(&array[i], &dsfmt->status[i], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
    for (; i < loop2_end; i++) {
        do_recursion_x1_avx2(&array[i], &dsfmt->status[i], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
    }



    // ループ3: メインの生成ループ
    const ptrdiff_t loop3_end = size - DSFMT_N;
    for (; i <= loop3_end - 2; i += 2) {
        do_recursion_x2_avx2(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
        conv256((double*)&array[i - DSFMT_N]);
    }
    for (; i < loop3_end; i++) {
        do_recursion_x1_avx2(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
        conv128((double*)&array[i - DSFMT_N]);
    }

    // ループ4: 次の状態ベクトルを準備 (前半)
    const ptrdiff_t loop4_end = 2 * DSFMT_N - size;
    for (j = 0; j < loop4_end; j++) {
        dsfmt->status[j] = array[j + size - DSFMT_N];
    }

    // ループ5: 配列の残りを生成しつつ、次の状態ベクトルを準備 (後半)
    const ptrdiff_t loop5_end = size;
    for (; i <= loop5_end - 2; i += 2, j += 2) {
        do_recursion_x2_avx2(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
        dsfmt->status[j]     = array[i];
        dsfmt->status[j + 1] = array[i + 1];
        conv256((double*)&array[i - DSFMT_N]);
    }
    for (; i < loop5_end; i++, j++) {
        do_recursion_x1_avx2(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
        dsfmt->status[j] = array[i];
        conv128((double*)&array[i - DSFMT_N]);
    }

    // ループ6: 生成された配列の末尾部分に変換を適用
    for ( i = size - DSFMT_N; i < size; i++) {
        conv128((double*)&array[i]);
    }
    dsfmt->status[DSFMT_N].si = _mm256_extracti128_si256(lung, 1);
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
    
    for (; i <  DSFMT_N - DSFMT_POS1 - 1; i += 2) {
        do_recursion_x2_avx2(&dsfmt->status[i], &dsfmt->status[i],
                        &dsfmt->status[i + DSFMT_POS1], &lung);
    }
    if ((DSFMT_N - DSFMT_POS1) & 1)
    {
        w128_t b[2];
        b[0] = dsfmt->status[i + DSFMT_POS1];
        b[1] = dsfmt->status[0];
        do_recursion_x2_avx2(&dsfmt->status[i], &dsfmt->status[i], b, &lung);
        i+=2;
    }
    for (; i < DSFMT_N - 1; i += 2) {
        do_recursion_x2_avx2(&dsfmt->status[i], &dsfmt->status[i],
                        &dsfmt->status[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
    if (i < DSFMT_N) {
        do_recursion_x1_avx2(&dsfmt->status[i], &dsfmt->status[i],
                     &dsfmt->status[i + DSFMT_POS1 - DSFMT_N], &lung);
    }

    ST128(&dsfmt->status[DSFMT_N], _mm256_extracti128_si256(lung, 1));

}


#endif /* DSFMT_AVX2_H */

