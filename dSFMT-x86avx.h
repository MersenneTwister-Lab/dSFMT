#pragma once

#ifndef DSFMT_X86AVX_H
#define DSFMT_X86AVX_H

#include <immintrin.h>
#include <emmintrin.h>
#include "dSFMT-params.h"

/**
 * This function represents the recursion formula.
 * @param a a 128-bit part of the internal state array
 * @param b a 128-bit part of the internal state array
 * @param u 128-bit (expand to 256-bit) part of the internal state array (I/O)
 * @return output 256-bit
 */
inline static __m256i do_recursion_x2_avx2(__m256i a, __m256i b, __m256i *u) {
    __m256i v, w, x, y, z;
    __m256i lung_shuf_idx = _mm256_setr_epi32(7, 6, 5, 4, 4, 5, 6, 7);
    __m256i word_reverse_idx = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
    __m256i z_mask = _mm256_setr_epi32(0, 0, 0, 0, -1, -1, -1, -1);
    __m256i dsfmt_mask = _mm256_broadcastsi128_si256(_mm_set_epi64x(DSFMT_MSK2, DSFMT_MSK1));

    x = a;
    z = _mm256_slli_epi64(x, DSFMT_SL1);
    z = _mm256_xor_si256(z, b);

    v = _mm256_permutevar8x32_epi32(z, word_reverse_idx);
    z = _mm256_and_si256(z, z_mask);
    z = _mm256_xor_si256(z, v);

    /*lungの計算ショートカット*/
    y = _mm256_load_si256((__m256i*)u);
    _mm256_store_si256((__m256i*)u, _mm256_xor_si256(z, y));

    y = _mm256_permutevar8x32_epi32(y, lung_shuf_idx);
    y = _mm256_xor_si256(y, z); 

    v = _mm256_srli_epi64(y, DSFMT_SR);
    w = _mm256_and_si256(y, dsfmt_mask);
    v = _mm256_xor_si256(v, x);
    v = _mm256_xor_si256(v, w);
    _mm256_store_si256((__m256i*)r, v);
}

/**
 * This function represents the recursion formula.
 * @param r output 128-bit
 * @param a a 128-bit part of the internal state array
 * @param b a 128-bit part of the internal state array
 * @param d a 128-bit part of the internal state array (I/O)
 */
inline static __m128i do_recursion_last(__m128i a, __m128i b, __m128i* u) {
    __m128i v, w, x, y, z;
    __m128i dsfmt_mask = _mm_set_epi64x(DSFMT_MSK2, DSFMT_MSK1);

    x = a;
    z = _mm_slli_epi64(x, DSFMT_SL1);
    y = _mm_shuffle_epi32(*u, _MM_SHUFFLE(0,1,2,3));
    z = _mm_xor_si128(z, b);
    y = _mm_xor_si128(y, z);
    *u = y;

    v = _mm_srli_epi64(y, DSFMT_SR);
    w = _mm_and_si128(y, dsfmt_mask);
    v = _mm_xor_si128(v, x);
    v = _mm_xor_si128(v, w);
    return v;
}



//#define DO_RECURSION_X4
//#define DO_RECURSION_X3
#define DO_RECURSION_X2


typedef void (*avx_converter_t)(__m256d *);

inline static void* no_convert_avx(__m256d* w) {}
inline static void* convert_c0o1_avx(__m256d* w) {
    *w = _mm256_add_pd(w, _mm256_set1_pd(-1.0));
}

inline static void* convert_o0c1_avx(__m256d* w) {
    *w = _mm256_sub_pd(_mm256_set1_pd(2.0), w);
}
inline static void* convert_o0o1_avx(__m256d* w) {
    __m256d mask = _mm256_castsi256_pd(_mm256_set1_epi64x(1));
    *w = _mm256_add_pd(_mm256_or_pd(w, mask), _mm256_set1_pd(-1.0));
}

#define MAKE_PAIR(a, b) _mm256_set_m128i((b).si, (a).si)
#define LD256(a) _mm256_loadu_si256((__m256i*)(a))
#define LD128(a) _mm_loadu_si128((__m128i*)(a))

#define ST256(addr, v) _mm256_storeu_si256(((__m256i*)(addr), (v));
#define ST128(addr, v) _mm_storeu_si128(((__m256i*)(addr), (v));

/**
 * This is the core function for generating pseudorandom numbers.
 * It fills the user-specified array and applies a given conversion.
 * (Unrolled version)
 * @param dsfmt dsfmt state vector.
 * @param array an 128-bit array to be filled by pseudorandom numbers.
 * @param size number of 128-bit pseudorandom numbers to be generated.
 * @param converter a function pointer for value conversion.
 */
inline static void gen_rand_array_avx_core(dsfmt_t *dsfmt, w128_t *array,
                                ptrdiff_t size, avx_converter_t converter) {
    ptrdiff_t i, j;

    __m256i lung = _mm256_broadcastsi128_si256(dsfmt->status[DSFMT_N].si);

    const ptrdiff_t loop1_end = DSFMT_N - DSFMT_POS1;
    for (; i < loop1_end - 1; i += 2) {
        __m256i result = do_recursion_x2_avx2(
            LD256(&dsfmt->status[i]),
            LD256(&dsfmt->status[i + DSFMT_POS1]), 
            &lung);
        ST256(&array[i], result);
    }
    if (loop1_end & 1) {
        __m256i result = do_recursion_x2_avx2(
            LD256(&dsfmt->status[i]), 
            MAKE_PAIR(&dsfmt->status[DSFMT_N-1], &array[0]),
            &lung);
        ST256(&array[i], result);
        i+=2;
    }
    // ループ2: 状態ベクトル後半と生成済み配列の一部を使って生成
    const ptrdiff_t loop2_end = DSFMT_N;
    for (; i < loop2_end - 1; i += 2) {
        __m256i result = do_recursion_x2_avx2(
            LD256(&dsfmt->status[i]), 
            LD256(&array[i + DSFMT_POS1 - DSFMT_N]),
            &lung);
        ST256(&array[i], result);
    }
    if (loop2_end & 1) {
        __m256i result = do_recursion_x2_avx2(
            MAKE_PAIR(&dsfmt->status[i], &array[0]), 
            LD256(&array[i + DSFMT_POS1 - DSFMT_N]),
            &lung);
        ST256(&array[i], result);
        i+=2;
    }

    // ループ3: メインの生成ループ
    const ptrdiff_t loop3_end = (size - DSFMT_N) & ~1;
    for (; i < loop3_end; i += 2) {
        __m256i result = do_recursion_x2_avx2(
            LD256(&array[i - DSFMT_N]), 
            LD256(&array[i + DSFMT_POS1 - DSFMT_N]),
            &lung);
        ST256(&array[i], result);
        converter(&array[i - DSFMT_N]);
    }

    // ループ4: 次の状態ベクトルを準備 (前半)
    const ptrdiff_t loop4_end = 2 * DSFMT_N - size;
    for (j = 0; j < loop4_end-1; j+=2) {
        ST256(&dsfmt->status[j], LD256[&array[j + size - DSFMT_N]]);
    }

    // ループ5: 配列の残りを生成しつつ、次の状態ベクトルを準備 (後半)
    for (; i < size - 1; i += 2, j += 2) {
        __m256i result = do_recursion_x2_avx2(
            LD256(&array[i - DSFMT_N]), 
            LD256(&array[i + DSFMT_POS1 - DSFMT_N]),
            &lung);
        ST256(&array[i], result);
        converter(&array[i - DSFMT_N]);
    }
    if (size & 1) {
        __m128i lung128 = _mm256_extracti128_si256(lung, 1);
        __m128i result = do_recursion_last(
            LD128(&array[i - DSFMT_N]), 
            LD128(&array[i + DSFMT_POS1 - DSFMT_N]),
            &lung128);
        ST128(&array[i], result);
        ST128(&dsfmt->status[DSFMT_N], lung128);
    } else {
        ST128(&dsfmt->status[DSFMT_N], _mm256_extracti128_si256(lung, 1));
    }

    // ループ6: 生成された配列の末尾部分に変換を適用
    for (i = loop3_end; i < size; i+= 2) {
        converter(&array[i]);
    }
    if (size & 1) {
        __m256d data = _mm256_castpd128_pd256(_mm_loadu_ps(&array[i]))
        converter(&data);
        _mm_storeu_pd((double*)&array[i], _mm256_castpd256_pd128(data));
    } 
}

/**
 * This function fills the internal state array with double precision
 * floating point pseudorandom numbers of the IEEE 754 format.
 * @param dsfmt dsfmt state vector.
 */
void dsfmt_gen_rand_all_avx(dsfmt_t *dsfmt) {
    int i;
    __m256i lung = _mm256_broadcastsi128_si256(dsfmt->status[DSFMT_N].si);

    i = 0;

    // ループ1: 状態ベクトル前半の生成
    for (; i < DSFMT_N - DSFMT_POS1 - 1; i += 2) {
        __m256i result = do_recursion_x2_avx2(
            LD256(&dsfmt->status[i]),
            LD256(&dsfmt->status[i + DSFMT_POS1]), 
            &lung);
        ST256(&dsfmt->status[i], result);
    }
    if ((DSFMT_N - DSFMT_POS1) & 1) {
        __m256i result = do_recursion_x2_avx2(
            LD256(&dsfmt->status[i]), 
            MAKE_PAIR(&dsfmt->status[DSFMT_N-1], &dsfmt->status[0]),
            &lung);
        ST256(&dsfmt->status[i], result);
        i += 2;
    }
    // ループ2: 状態ベクトル後半生成
    for (; i < DSFMT_N - 1; i += 2) {
        __m256i result = do_recursion_x2_avx2(
            LD256(&dsfmt->status[i]), 
            LD256(&dsfmt->status[i + DSFMT_POS1 - DSFMT_N]),
            &lung);
        ST256(&dsfmt->status[i], result);
    }
    if (DSFMT_N & 1) {
        __m128i lung128 = _mm256_extracti128_si256(lung, 1);
        __m128i result = do_recursion_avx2(
            LD256(&dsfmt->status[DSFMT_N-1]), 
            LD256(&array[i + DSFMT_POS1 - DSFMT_N]),
            &lung128);
        ST128(&dsfmt->status[DSFMT_N-1], result);
        ST128(&dsfmt->status[DSFMT_N], lung128);

    } else {
                ST128(&dsfmt->status[DSFMT_N], lung128);
    } else {
        ST128(&dsfmt->status[DSFMT_N], _mm256_extracti128_si256(lung, 1));
    }

}

#endif /* DSFMT_ARM64NEON_H */

