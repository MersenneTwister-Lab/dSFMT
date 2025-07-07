#pragma once

#ifndef DSFMT_AVX512_H
#define DSFMT_AVX512_H

#include <immintrin.h>
#include "dSFMT-params.h"

inline static __m512i shift_lung(__m512i lung, int lastLane) {
    __m512i indices[] = {
        _mm512_setr_epi32(12,13,14,15,12,13,14,15,12,13,14,15,12,13,14,15),
        _mm512_setr_epi32(0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3),
        _mm512_setr_epi32(4,5,6,7,4,5,6,7,4,5,6,7,4,5,6,7),
        _mm512_setr_epi32(8,9,10,11,8,9,10,11,8,9,10,11,8,9,10,11)
    };
    return _mm512_permutexvar_epi32(indices[lastLane&3], lung);
}

/**
 * This function represents the recursion formula.
 * @param r output 128-bit
 * @param a a 128-bit part of the internal state array
 * @param b a 128-bit part of the internal state array
 * @param d a 128-bit part of the internal state array (I/O)
 */
inline static void do_recursion_x4_avx512(w128_t* r, w128_t* a, w128_t* b, __m512i* lung)
{
    __m512i v, w, x, y, z;
    const __mmask16 shift128_reverse_mask = 0xFFF0;
    const __m512i shift128_reverse_idx = 
        _mm512_setr_epi32(0,0,0,0, 3,2,1,0, 7,6,5,4, 11,10,9,8);

    const __mmask16 shift384_reverse_mask = 0xF000;
    const __m512i shift384_reverse_idx = 
        _mm512_setr_epi32(0,0,0,0, 0,0,0,0, 0,0,0,0, 3,2,1,0);
    const __m512i expand_lung_idx = _mm512_setr_epi32(15,14,13,12, 12,13,14,15, 15,14,13,12, 12,13,14,15);

    x = _mm512_loadu_epi32(a);
    z = _mm512_slli_epi64(x, DSFMT_SL1);
    z = _mm512_xor_si512(z, _mm512_loadu_epi32(b));

    w = _mm512_maskz_permutexvar_epi32(shift128_reverse_mask,shift128_reverse_idx , z);
    y = _mm512_inserti64x4(_mm512_setzero_si512(), _mm512_castsi512_si256(z), 1);
    v = _mm512_maskz_permutexvar_epi32(shift384_reverse_mask,shift384_reverse_idx , z);
    z = _mm512_ternarylogic_epi32(z, w, y, 0x96);

    y = _mm512_permutexvar_epi32(expand_lung_idx, *lung);
    y = _mm512_ternarylogic_epi32(y, z, v, 0x96);
    *lung = y;

    v = _mm512_srli_epi64(y, DSFMT_SR);
    w = _mm512_and_si512(y, _mm512_broadcast_i32x4(sse2_param_mask.i128));
    v = _mm512_ternarylogic_epi32(v, x, w, 0x96);
    
    _mm512_storeu_epi32(r, v);
}


/**
 * This function represents the recursion formula.
 * @param r output 128-bit
 * @param 1-3x a 128-bit part of the internal state array
 * @param 1-3x a 128-bit part of the internal state array
 * @param 1-3x a 128-bit part of the internal state array (I/O)
 */
inline static void do_recursion_frac_avx512(int nCount, w128_t* r, w128_t* a, w128_t* b, __m512i* lung)
{
    __m512i v, w, x, y, z;
    const __mmask16 shift128_reverse_mask = 0xFFF0;
    const __m512i shift128_reverse_idx = 
        _mm512_setr_epi32(0,0,0,0, 3,2,1,0, 7,6,5,4, 11,10,9,8);
    const __m512i expand_lung_idx = 
        _mm512_setr_epi32(15,14,13,12, 12,13,14,15, 15,14,13,12, 12,13,14,15);

    __mmask16 k = (__mmask16)((1 << (4*nCount))-1);

    x = _mm512_maskz_loadu_epi32(k, a);
    z = _mm512_slli_epi64(x, DSFMT_SL1);
    z = _mm512_xor_si512(z, _mm512_maskz_loadu_epi32(k, b));

    w = _mm512_maskz_permutexvar_epi32(shift128_reverse_mask,shift128_reverse_idx , z);
    v = _mm512_inserti64x4(_mm512_setzero_si512(), _mm512_castsi512_si256(z), 1);
    z = _mm512_ternarylogic_epi32(z, w, v, 0x96);

    y = _mm512_permutexvar_epi32(expand_lung_idx, *lung);
    y = _mm512_xor_si512(y, z);

    *lung = shift_lung(y, nCount);

    v = _mm512_srli_epi64(y, DSFMT_SR);
    w = _mm512_and_si512(y, _mm512_broadcast_i32x4(sse2_param_mask.i128));
    v = _mm512_ternarylogic_epi32(v, x, w, 0x96);
    
    _mm512_mask_storeu_epi32(r, k, v);
}

/**
 * This is the core function for generating pseudorandom numbers.
 * It fills the user-specified array and applies a given conversion.
 * (Unrolled version)
 * @param dsfmt dsfmt state vector.
 * @param array an 128-bit array to be filled by pseudorandom numbers.
 * @param size number of 128-bit pseudorandom numbers to be generated.
 */
static void gen_rand_array(dsfmt_t *dsfmt, w128_t *array,
                                  ptrdiff_t size) {

    ptrdiff_t i, j;
    __m512i lung = _mm512_broadcast_i64x2(dsfmt->status[DSFMT_N].si);

    i = 0;
    const ptrdiff_t loop1_end = DSFMT_N - DSFMT_POS1;
    for (; i < loop1_end - 3; i += 4) {
        do_recursion_x4_avx512(&array[i], &dsfmt->status[i], &dsfmt->status[i + DSFMT_POS1], &lung);
    }
    if (i < loop1_end) {
        do_recursion_frac_avx512(loop1_end-i,
            &array[i], &dsfmt->status[i], &dsfmt->status[i + DSFMT_POS1], &lung);
        i = loop1_end;
    }

    const ptrdiff_t loop2_end = DSFMT_N;
    for (; i < loop2_end - 3; i += 4) {
        do_recursion_x4_avx512(
            &array[i], &dsfmt->status[i], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
    if (i < loop2_end) {
        do_recursion_frac_avx512(loop2_end-i,
            &array[i], &dsfmt->status[i], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
        i = loop2_end;
    }

    const ptrdiff_t loop3_end = size - DSFMT_N;
    for (; i < loop3_end - 3; i += 4) {
        do_recursion_x4_avx512(
            &array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
    if (i < loop3_end) {
        do_recursion_frac_avx512(loop3_end-i,
            &array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
        i= loop3_end;
    }

    const ptrdiff_t loop4_end = 2 * DSFMT_N - size;
    for (j = 0; j < loop4_end-3; j+=4) {
        __m512i vec = _mm512_loadu_epi32((__m512i*)&array[j + size - DSFMT_N]);
        _mm512_storeu_epi32((__m512i*)&dsfmt->status[j], vec);
    }
    if (j < loop4_end) {
        int n = loop4_end-j;
        __mmask16 k = (n<<(4*n))-1;
        __m512i vec = _mm512_maskz_loadu_epi32(k, (__m512i*)&array[j + size - DSFMT_N]);
        _mm512_mask_storeu_epi32((__m512i*)&dsfmt->status[j], k, vec);
        j = loop4_end;
    }

    const ptrdiff_t loop5_end = size;
    for (; i <= loop5_end - 4; i += 4, j += 4) {
        do_recursion_x4_avx512(
            &array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
        _mm512_storeu_epi32((__m512i*)&dsfmt->status[j], _mm512_loadu_epi32((__m512i*)&array[i]));
    }
    if (i < loop5_end) {
        do_recursion_frac_avx512(loop5_end-i,
            &array[i], &array[i - DSFMT_N], &array[i + DSFMT_POS1 - DSFMT_N], &lung);
        __mmask16 k = (1<<(4*(loop5_end-i)))-1;
        __m512i vec = _mm512_maskz_loadu_epi32(k, (__m512i*)&array[i]);
        _mm512_mask_storeu_epi32((__m512i*)&dsfmt->status[j], k, vec);
    }

    dsfmt->status[DSFMT_N].si = _mm512_extracti64x2_epi64(lung, 3);
}

/**
 * This function fills the internal state array with double precision
 * floating point pseudorandom numbers of the IEEE 754 format.
 * @param dsfmt dsfmt state vector.
 */
inline static void dsfmt_gen_rand_all_impl(dsfmt_t *dsfmt) {
    ptrdiff_t i;
    __m512i lung = _mm512_broadcast_i64x2(dsfmt->status[DSFMT_N].si);

    const ptrdiff_t loop1_end = DSFMT_N - DSFMT_POS1;
    for (i = 0; i < loop1_end-3; i += 4) {
        do_recursion_x4_avx512(&dsfmt->status[i], &dsfmt->status[i], &dsfmt->status[i + DSFMT_POS1], &lung);
    }
    if (i < loop1_end) {
        int nCount = loop1_end - i;
        do_recursion_frac_avx512(nCount,
            &dsfmt->status[i], &dsfmt->status[i], &dsfmt->status[i + DSFMT_POS1], &lung);
    }

    const ptrdiff_t loop2_end = DSFMT_N;
    for (i = loop1_end; i < loop2_end-3; i += 4) {
        do_recursion_x4_avx512(&dsfmt->status[i], &dsfmt->status[i], &dsfmt->status[i + DSFMT_POS1 - DSFMT_N], &lung);
    }
    if ( i < loop2_end) {
        int nCount = loop2_end - i;
        do_recursion_frac_avx512(nCount,
            &dsfmt->status[i], &dsfmt->status[i], &dsfmt->status[i + DSFMT_POS1 - DSFMT_N], &lung);
    }

    dsfmt->status[DSFMT_N].si = _mm512_extracti64x2_epi64(lung, 3);
}


#endif /* DSFMT_AVX512_H */

