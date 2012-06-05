/**
 * @file dSFMT-jump.c
 *
 * @brief do jump using jump polynomial.
 *
 * @author Mutsuo Saito (Hiroshima University)
 * @author Makoto Matsumoto (The University of Tokyo)
 *
 * Copyright (C) 2012 Mutsuo Saito, Makoto Matsumoto,
 * Hiroshima University and The University of Tokyo.
 * All rights reserved.
 *
 * The 3-clause BSD License is applied to this software, see
 * LICENSE.txt
 */

#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "dSFMT-params.h"
#include "dSFMT.h"
#include "dSFMT-jump.h"
#include "dSFMT-common.h"

#if defined(__cplusplus)
extern "C" {
#endif

    static const uint64_t fix[] = {
	UINT64_C(0x00069a07426b5843), UINT64_C(0x0003ef4acb14fc95),
	UINT64_C(0x000caae34490ca9c), UINT64_C(0x00047ebdee94edf9),
	UINT64_C(0x000ddf318b9c6bac), UINT64_C(0x000488deb751f65b),
	UINT64_C(0x00053752bd35283c), UINT64_C(0x000a1135d25a0235),
	UINT64_C(0x05e9c8d9cbe6467d), UINT64_C(0x602eae9da43ea01f)
    };

    inline static void next_state(dsfmt_t * dsfmt);

#if defined(HAVE_SSE2)
/**
 * add internal state of src to dest as F2-vector.
 * @param dest destination state
 * @param src source state
 */
    inline static void add(dsfmt_t *dest, dsfmt_t *src) {
	int dp = dest->idx / 2;
	int sp = src->idx / 2;
	int diff = (sp - dp + DSFMT_N) % DSFMT_N;
	int p;
	int i;
	for (i = 0; i < DSFMT_N - diff; i++) {
	    p = i + diff;
	    dest->status[i].si
		= _mm_xor_si128(dest->status[i].si, src->status[p].si);
	}
	for (i = DSFMT_N - diff; i < DSFMT_N; i++) {
	    p = i + diff - DSFMT_N;
	    dest->status[i].si
		= _mm_xor_si128(dest->status[i].si, src->status[p].si);
	}
	dest->status[DSFMT_N].si
	    = _mm_xor_si128(dest->status[DSFMT_N].si,
			    src->status[DSFMT_N].si);
    }
#else
    inline static void add(dsfmt_t *dest, dsfmt_t *src) {
	int dp = dest->idx / 2;
	int sp = src->idx / 2;
	int diff = (sp - dp + DSFMT_N) % DSFMT_N;
	int p;
	int i;
	for (i = 0; i < DSFMT_N - diff; i++) {
	    p = i + diff;
	    dest->status[i].u[0] ^= src->status[p].u[0];
	    dest->status[i].u[1] ^= src->status[p].u[1];
	}
	for (; i < DSFMT_N; i++) {
	    p = i + diff - DSFMT_N;
	    dest->status[i].u[0] ^= src->status[p].u[0];
	    dest->status[i].u[1] ^= src->status[p].u[1];
	}
	dest->status[DSFMT_N].u[0] ^= src->status[DSFMT_N].u[0];
	dest->status[DSFMT_N].u[1] ^= src->status[DSFMT_N].u[1];
    }
#endif

/**
 * calculate next state
 * @param sfmt SFMT internal state
 */
    inline static void next_state(dsfmt_t * dsfmt) {
	int idx = (dsfmt->idx / 2) % DSFMT_N;
	w128_t * lung;
	w128_t * pstate = &dsfmt->status[0];

	lung = &pstate[DSFMT_N];
	do_recursion(&pstate[idx],
		     &pstate[idx],
		     &pstate[(idx + DSFMT_POS1) % DSFMT_N],
		     lung);
	dsfmt->idx = (dsfmt->idx + 2) % DSFMT_N64;
    }

    inline static void add_fix(dsfmt_t * dsfmt) {
	int i;
	uint64_t *psfmt;

	psfmt = &dsfmt->status[0].u[0];
	for (i = 0; i < (DSFMT_N + 1) * 2; i++) {
	    psfmt[i] ^= fix[i];
	}
    }

    inline static void setup_fix(dsfmt_t * dsfmt) {
	int i;
	uint64_t *psfmt;

	psfmt = &dsfmt->status[0].u[0];
	for (i = 0; i < (DSFMT_N + 1) * 2; i++) {
	    psfmt[i] ^= fix[i];
	}
	for (i = 0; i < DSFMT_N * 2; i++) {
	    psfmt[i] &= DSFMT_LOW_MASK;
	}
    }

    inline static void teardown_fix(dsfmt_t * dsfmt) {
	int i;
	uint64_t *psfmt;

	psfmt = &dsfmt->status[0].u[0];
	for (i = 0; i < (DSFMT_N + 1) * 2; i++) {
	    psfmt[i] ^= fix[i];
	}
	for (i = 0; i < DSFMT_N * 2; i++) {
	    psfmt[i] |= DSFMT_HIGH_CONST;
	}
    }

    inline static void add_fix_dummy(dsfmt_t * dsfmt) {
	int i;
	uint64_t *psfmt;

	psfmt = &dsfmt->status[0].u[0];
	for (i = 0; i < DSFMT_N * 2; i++) {
	    if ((psfmt[i] & DSFMT_HIGH_CONST) == 0) {
		psfmt[i] |= DSFMT_HIGH_CONST;
	    }
	}
    }

    inline static void set_constant(dsfmt_t * dsfmt) {
	int i;
	uint64_t *psfmt;

	psfmt = &dsfmt->status[0].u[0];
	for (i = 0; i < DSFMT_N * 2; i++) {
	    psfmt[i] &= DSFMT_LOW_MASK;
	    psfmt[i] |= DSFMT_HIGH_CONST;
	}
    }

/**
 * jump ahead using jump_string
 * @param dsfmt dSFMT internal state input and output.
 * @param jump_string string which represents jump polynomial.
 */
    void dSFMT_jump(dsfmt_t * dsfmt, const char * jump_string) {
	dsfmt_t work;
	int index = dsfmt->idx;
	int bits;
	memset(&work, 0, sizeof(dsfmt_t));
	//setup_fix(dsfmt);
	dsfmt->idx = DSFMT_N64;

	for (int i = 0; jump_string[i] != '\0'; i++) {
	    bits = jump_string[i];
	    assert(isxdigit(bits));
	    bits = tolower(bits);
	    if (bits >= 'a' && bits <= 'f') {
		bits = bits - 'a' + 10;
	    } else {
		bits = bits - '0';
	    }
	    bits = bits & 0x0f;
	    for (int j = 0; j < 4; j++) {
		if ((bits & 1) != 0) {
		    add(&work, dsfmt);
		}
		next_state(dsfmt);
		bits = bits >> 1;
	    }
	}
	*dsfmt = work;
	//add_fix_dummy(dsfmt);
	dsfmt->idx = index;
    }

#if defined(__cplusplus)
}
#endif
