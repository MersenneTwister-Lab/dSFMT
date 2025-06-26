#include <stdint.h>
#include <immintrin.h>
#include "xoshiro256.h"







union XOSHIRO256_X8_AVX512_STATE {
    __m512i state512[4];
    uint64_t state64[32];
    uint32_t state32[64];
};
typedef union XOSHIRO256_X8_AVX512_STATE xoshiro256_x8_avx512_state_t;


xoshiro256_x8_avx512_state_t transpose(uint64_t states[32])
{
    __m256i vdx = _mm256_setr_epi32(0, 32, 64, 96, 128, 160, 192, 224);        
    xoshiro256_x8_avx512_state_t result = {
        _mm512_i32logather_epi64(vdx, &states[0], 1),
        _mm512_i32logather_epi64(vdx, &states[1], 1),
        _mm512_i32logather_epi64(vdx, &states[2], 1),
        _mm512_i32logather_epi64(vdx, &states[3], 1)
    };
    return result;
}

inline void next_common_uint64x8_avx512(xoshiro256_x8_avx512_state_t* state) {
    __m512i s0, s1, s2, s3;
    __m512i t;
    __m512i u0, u1;
    __m512i s0 = state->state512[0];
    __m512i s1 = state->state512[1];
    __m512i s2 = state->state512[2];
    __m512i s3 = state->state512[3];
    __m512i t  = _mm512_slli_epi64(s1, 17);
    state->state512[1] = _mm512_ternarylogic_epi64(s1, s2, s0, 0x96);
    state->state512[2] = _mm512_ternarylogic_epi64(t, s2, s0, 0x96);
    state->state512[0] = _mm512_ternarylogic_epi64(s0, s1, s3, 0x96);
    s3 = _mm512_xor_epi64(s1, s3);
    state->state512[3] = _mm512_rol_epi64(s3, 45);
}

__m512i next_uint64x8_pp(xoshiro256_x8_avx512_state_t* state) {
    __m512i v = _mm512_add_epi64(state->state512[0], state->state512[3]);
    v = _mm512_rol_epi64(v, 23);
    v = _mm512_add_epi64(v, state->state512[0]);
    next_common_uint64x8_avx512(state);
    return v;
}

__m512i next_uint64x8_ss(xoshiro256_x8_avx512_state_t* state) {
    __m512i v = state->state512[0];
    v = _mm512_add_epi64(v, _mm512_slli_epi64(v, 2)); // x5
    v = _mm512_rol_epi64(v, 7);
    v = _mm512_add_epi64(v, _mm512_slli_epi64(v, 3)); // x9
    next_common_uint64x8_avx512(state);
    return v;
}
__m512i next_uint64x8_p(xoshiro256_x8_avx512_state_t* state)
{
    __m512i v = _mm512_add_epi64(state->state512[0], state->state512[3]);
    next_common_uint64x8_avx512(state);
    return v;
}

__m512d next_float64x8_p(xoshiro256_x8_avx512_state_t* state)
{
    __m512i v = next_uint64x8_p(state);
    __m512d vd = _mm512_mul_pd(_mm512_cvtepu64_pd(v), 
        _mm512_set1_pd(1.0/18446744073709551616.0));
        __m512d vd = _mm512_sub_pd(_mm512_castsi512_pd(v), _mm512_set1_pd(1.0));
    return vd;
}


