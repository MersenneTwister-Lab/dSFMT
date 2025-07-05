#pragma once

#ifndef DSFMT_AVX512_H
#define DSFMT_AVX512_H

#include <immintrin.h>
#include <emmintrin.h>
#include "dSFMT-params.h"
#include <string.h>


#if DSFMT_MEXP == 521
#define DSFMT_LOOP_STEP (1)
#elif DSFMT_MEXP == 1279
#define DSFMT_LOOP_STEP (3)
#elif DSFMT_MEXP > 1279
#define DSFMT_LOOP_STEP (4)
#endif 


#if DSFMT_LOOP_STEP >= 3

inline static __m512i expand_lung(__m512i lung, int lastLane) {
    switch (lastLane) {
        case 1: return _mm512_permutexvar_epi32(
            _mm512_setr_epi32(3,2,1,0, 0,1,2,3, 3,2,1,0, 0,1,2,3), lung);
        case 2: return _mm512_permutexvar_epi32(
            _mm512_setr_epi32(7,6,5,4, 4,5,6,7, 7,6,5,4, 4,5,6,7),lung);
        case 3: return _mm512_permutexvar_epi32(
            _mm512_setr_epi32(11,10,9,8, 8,9,10,11, 11,10,9,8, 8,9,10,11),lung);
        default: return _mm512_permutexvar_epi32(
            _mm512_setr_epi32(15,14,13,12, 12,13,14,15, 15,14,13,12, 12,13,14,15),lung);
    }
}

/**
 * This function represents the recursion formula.
 * @param r output 128-bit
 * @param a a 128-bit part of the internal state array
 * @param b a 128-bit part of the internal state array
 * @param d a 128-bit part of the internal state array (I/O)
 */
inline static __m512i do_recursion_x4_avx512(
    w128_t* r, 
    w128_t* a, 
    w128_t* b, 
    __m512i* lung, 
    int nStep 
{
    __mmask16 predicate = (1 << (nStep*4)) - 1;

    __m512i v, w, x, y, z;
    const __mmask16 shift128_reverse_mask = 0xFFF0;
  
    const __m512i shift128_reverse_idx = 
        _mm512_setr_epi32(0, 0, 0, 0, 3,2,1,0, 7,6,5,4, 11,10,9,8);
    const __m512i expand_lung_idx = 
        _mm512_setr_epi32(15,14,13,12, 12,13,14,15, 15,14,13,12, 12,13,14,15);
    
    x = _mm512_maskz_expandloadu_epi32(predicate, a);
    z = _mm512_slli_epi64(x, DSFMT_SL1);
    z = _mm512_xor_si512(z, _mm512_maskz_expandloadu_epi32(predicate, b));

    w = _mm512_maskz_permutexvar_epi32(shift128_reverse_mask,shift128_reverse_idx , z);
    z = _mm512_xor_si512(z, w);

    w = _mm512_setzero_si512();
    w = _mm512_inserti64x4(w, _mm512_castsi512_si256(z), 1);
    z = _mm512_xor_si512(z, w);

    y = _mm512_ternarylogic_epi32(*lung, x, z, 0x96);
    *lung = expand_lung(y, DSFMT_LOOP_STEP);

    v = _mm512_srli_epi64(y, DSFMT_SR);
    w = _mm512_and_si512(y, _mm512_broadcast_i32x4(sse2_param_mask.i128));
    v = _mm512_ternarylogic_epi32(v, a, w, 0x96);
    
    _mm512_mask_storeu_epi32(r, predicate, v);
}


typedef void (*avx_converter_t)(w128_t *, int);

#define CALC_MASK_PD(n) (1<<((n)*2))-1

inline static void no_convert_avx(w128_t* w, int) {}

inline static void convert_c0o1_avx512(w128_t* w, int n) {
    __m512d v = _mm512_maskz_loadu_pd(CALC_MASK_PD(n), w);
    v = _mm512_add_pd(v, _mm512_set1_pd(-1.0));
    _mm512_mask_store_pd(w, CALC_MASK_PD(n), v);
}

inline static void convert_o0c1_avx512(w128_t* w, int n) {
    __m512d v = _mm512_maskz_loadu_pd(CALC_MASK_PD(n), w);
    v = _mm256_sub_pd(_mm256_set1_pd(2.0), v);
    _mm512_mask_store_pd(w, CALC_MASK_PD(n), v);
}

inline static void convert_o0o1_avx512(w128_t* w, int n) {
    __m512d v = _mm512_maskz_loadu_pd(CALC_MASK_PD(n), w);
    __m512d mask = _mm512_castsi512_pd(_mm512_set1_epi64x(1));
    v = _mm512_or_pd(v, mask);
    v = _mm512_add_pd(v, _mm256_set1_pd(-1.0));
    _mm512_mask_store_pd(w, CALC_MASK_PD(n), v);
}


/**
 * This is the core function for generating pseudorandom numbers.
 * It fills the user-specified array and applies a given conversion.
 * (Unrolled version)
 * @param dsfmt dsfmt state vector.
 * @param array an 128-bit array to be filled by pseudorandom numbers.
 * @param size number of 128-bit pseudorandom numbers to be generated.
 * @param converter a function pointer for value conversion.
 */
inline static void gen_rand_array_avx512_core(dsfmt_t *dsfmt, 
    w128_t *array,
    ptrdiff_t size, 
    avx_converter_t converter,
) {
    unsigned int pred = (1 << (DSFMT_LOOP_STEP*4))  - 1;

    ptrdiff_t i, j;

    __m512i lung = expand_lung(_mm512_castsi128_si512(dsfmt->status[DSFMT_N].si), 1);

    int loop_end = DSFMT_N - DSFMT_POS1;
    for (i = 0; i < loop_end; i += DSFMT_LOOP_STEP) {
        int nCount = (i <= loop_end - DSFMT_LOOP_STEP) ? DSFMT_LOOP_STEP : loop_end - i;
        do_recursion_x4_avx512(&array[i], &dsfmt->status[i], &dsfmt->status[i + DSFMT_POS1], &lung, nCount);
    }
    i = loop_end;

    loop_end = DSFMT_N;
    for (; i < loop_end; i += DSFMT_LOOP_STEP) {
        int nCount = (i <= loop_end - DSFMT_LOOP_STEP) ? DSFMT_LOOP_STEP : loop_end - i;
        do_recursion_x4_avx512(&array[i], &dsfmt->status[i], &array[i + DSFMT_POS1 - DSFMT_N], &lung, nCount);
    }
    i = loop_end;

    loop_end = size - DSFMT_N;
    for (; i < loop_end; i += nCount) {
        int nCount = (i <= loop_end - DSFMT_LOOP_STEP) ? DSFMT_LOOP_STEP : loop_end - i;
        do_recursion_x4_avx512(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung, nCount);
        converter(&array[i - DSFMT_N], nCount);
    }
    i = loop_end;

    //memcpy
    memcpy(&dsfmt->status[j], &array[j + size - DSFMT_N], sizeof(w128_t) * (2 * DSFMT_N - size));

    // ループ5: 配列の残りを生成しつつ、次の状態ベクトルを準備 (後半)
    loop_end = size;
    for (i = size - DSFMT_N, j = 2 * DSFMT_N - size;
         i < loop_end; 
         i += DSFMT_LOOP_STEP, j += DSFMT_LOOP_STEP) {
        int nCount = (i <= loop_end - DSFMT_LOOP_STEP) ? DSFMT_LOOP_STEP : loop_end - i;
        do_recursion_x4_avx512(&array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung, nCount);
        converter(&array[i - DSFMT_N], nCount);
    }

    dsfmt->status[DSFMT_N].si =  _mm512_extracti128_si512(lung, 3);

    // ループ6: 生成された配列の末尾部分に変換を適用
    for (i = size - DSFMT_N; i < size; i += 4) {
        int nCount = (i <= size - 4) ? 4 : size - i;
        converter(&array[i], nCount);
    }
}

#endif //DSFMT_LOOP_STEP >= 3


#endif /* DSFMT_X86AVX_H */

