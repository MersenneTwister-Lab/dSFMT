#ifndef DSFMTEXT_HPP
#define DSFMTEXT_HPP
/**
 * @file dSFMText.hpp
 *
 * @brief exteted dSFMT class for calculating characteristic polynomial.
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
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

namespace dsfmt {
    using namespace std;

    union w128_t {
	uint64_t u[2];
	uint32_t u32[4];
    };

    class dSFMText {
    public:
	/**
	 * Constructor from parameters.
	 */
	dSFMText(int mexp, int pos1, int sl1,
		  uint64_t mask1, uint64_t mask2) {
		this->mexp = mexp;
		this->sl1 = sl1;
		this->sr = 12;
		this->pos1 = pos1;
		this->mask1 = mask1;
		this->mask2 = mask2;
		this->size = calc_size(mexp);
		this->maxdegree = (this->size -1) * 52 * 2 + 128;
		this->state = new w128_t[size];
		this->index = 0;
		this->bitpos = 0;
		for (int i = 0; i < size; i++) {
		    state[i].u[0] = 0;
		    state[i].u[1] = 0;
		}
#if defined(DEBUG)
		cout << "mexp:" << dec << this->mexp << endl;
		cout << "pos1:" << dec << this->pos1 << endl;
		cout << "sl1:" << dec << this->sl1 << endl;
		cout << "sr:" << dec << this->sr << endl;
		cout << "mask1:" << hex << this->mask1 << endl;
		cout << "mask2:" << hex << this->mask2 << endl;
		cout << "size:" << dec << this->size << endl;
		cout << "maxdegree:" << dec << this->maxdegree << endl;
#endif
	}

	/**
	 * release internal state.
	 */
	~dSFMText() {
	    delete[] state;
	}
	int get_mexp() {
	    return mexp;
	}
	int get_maxdegree() {
	    return maxdegree;
	}
	int get_mamaxdegree() {
	    return 128 * size;
	}
	void init_basis() {
	    init_basis(0);
	}
#if 0
	void init_basis(int affine) {
	    if (bitpos >= maxdegree) {
		throw new logic_error("exceed max degree");
	    }
	    int i, j, k;
	    uint64_t mask;
	    if (bitpos >= maxdegree - 128) {
		k = bitpos % 64;
		j = (bitpos / 64) % 2;
		i = size - 1;
	    } else {
		k = bitpos % 52;
		j = (bitpos / 52) % 2;
		i = bitpos / (52 * 2);
 	    }
	    memset(state, 0, sizeof(w128_t) * size);
	    mask = UINT64_C(1) << k;
	    state[i].u[j] = mask;
	    bitpos++;
	    if (affine) {
		setup_high();
	    }
	}
#else
	void init_basis(int affine) {
//	    if (bitpos >= maxdegree) {
//		throw new logic_error("exceed max degree");
//	    }
	    int i, j, k;
	    uint64_t mask;
	    k = bitpos % 64;
	    j = (bitpos / 64) % 2;
	    i = bitpos / (64 * 2);
	    memset(state, 0, sizeof(w128_t) * size);
	    mask = UINT64_C(1) << k;
	    state[i].u[j] = mask;
	    bitpos++;
	    if (affine) {
		setup_high();
	    }
	}
#endif
	/**
	 * generate next 128 bits.
	 * @returns 128-bit number.
	 */
	w128_t next() {
	    if (index >= size - 1) {
		index = 0;
	    }
	    do_recursion();
	    w128_t result = state[index];
	    index++;
	    if (index >= size - 1) {
		index = 0;
	    }
	    return result;
	}

	/**
	 * generate next 128 bits.
	 * @returns 128-bit number.
	 */
	w128_t next_add() {
	    if (index >= size - 1) {
		index = 0;
	    }
	    do_recursion();
	    w128_t result = state[index];
	    index++;
	    if (index >= size - 1) {
		index = 0;
	    }
	    add_const();
	    return result;
	}

	int next(uint64_t mask[2]) {
	    w128_t num = next();
	    if ((num.u[0] & mask[0]) != UINT64_C(0)) {
		return 1;
	    } else if ((num.u[1] & mask[1]) != UINT64_C(0)) {
		return 1;
	    } else {
		return 0;
	    }
	}
	void seeding(uint32_t seed) {
	    seeding(seed, 0);
	}
	void seeding(uint32_t seed, int affine) {
	    int i;
	    uint32_t *psfmt;

	    psfmt = &state[0].u32[0];
	    psfmt[0] = seed;
	    for (i = 1; i < size * 4; i++) {
		psfmt[i] = 1812433253UL
		    * (psfmt[i - 1] ^ (psfmt[i - 1] >> 30)) + i;
	    }
	    uint64_t mask = 0;
	    if (affine) {
		mask = UINT64_C(0x3ff0000000000000);
	    } else {
		mask = 0;
	    }
#if 0
	    uint64_t *psfmt64 = &state[0].u[0];
	    for (int i = 0; i < size * 2 - 2; i++) {
		psfmt64[i] &= UINT64_C(0x000fffffffffffff);
		psfmt64[i] |= mask;
	    }
#endif
	    index = size - 1;
	}
	void add(dSFMText& that) {
	    int diff = (that.index - index + size -1) % (size - 1);
	    for (int i = 0; i < size - 1; i++) {
		state[i].u[0] ^= that.state[(i + diff) % (size - 1)].u[0];
		state[i].u[1] ^= that.state[(i + diff) % (size - 1)].u[1];
	    }
	    state[size - 1].u[0] ^= that.state[size - 1].u[0];
	    state[size - 1].u[1] ^= that.state[size - 1].u[1];
	}
	void add_const() {
	    state[(index + size - 2) % (size - 1)].u[0]
		^= UINT64_C(0x0003ff0000000000);
	    state[(index + size - 2) % (size - 1)].u[1]
		^= UINT64_C(0x0003ff0000000000);
	    state[size - 1].u[0] ^= UINT64_C(0x3ff0000000000000);
	    state[size - 1].u[1] ^= UINT64_C(0x3ff0000000000000);
	}
	void setup_constants() {
	    state[size - 2].u[0] = UINT64_C(0x0003ff0000000000);
	    state[size - 2].u[1] = UINT64_C(0x0003ff0000000000);
	    state[size - 1].u[0] = UINT64_C(0x3ff0000000000000);
	    state[size - 1].u[1] = UINT64_C(0x3ff0000000000000);
#if 0
	    for (int i = 0; i < size - 1; i++) {
		state[i].u[0] &= UINT64_C(0x000fffffffffffff);
		state[i].u[0] |= UINT64_C(0x3ff0000000000000);
		state[i].u[1] &= UINT64_C(0x000fffffffffffff);
		state[i].u[1] |= UINT64_C(0x3ff0000000000000);
	    }
#endif
	}
	void setup_high() {
	    for (int i = 0; i < size - 1; i++) {
		state[i].u[0] &= UINT64_C(0x000fffffffffffff);
		state[i].u[0] |= UINT64_C(0x3ff0000000000000);
		state[i].u[1] &= UINT64_C(0x000fffffffffffff);
		state[i].u[1] |= UINT64_C(0x3ff0000000000000);
	    }
	}
	void print(ostream& out) {
	    out << "index:" << dec << index << endl;
	    for (int i = 0; i < size; i++) {
		out << hex << setw(16) << setfill('0') << state[i].u[0] << " ";
		out << hex << setw(16) << setfill('0') << state[i].u[1] << endl;
	    }
	}
    private:
	int mexp;
	int maxdegree;
	int sl1;
	int sr;
	int pos1;
	int size;
	int index;
	int bitpos;
	uint64_t mask1;
	uint64_t mask2;
	w128_t * state;

	int calc_size(int mexp) {
	    int size = mexp - 128;
	    size = size / (52 * 2) + 2;
	    return size;
	}

	void do_recursion() {
	    uint64_t t0, t1, L0, L1;
	    int pos = (index + pos1) % (size - 1);
	    w128_t * lung = &state[size -1];
	    t0 = state[index].u[0];
	    t1 = state[index].u[1];
	    L0 = lung->u[0];
	    L1 = lung->u[1];
	    lung->u[0] = (t0 << sl1) ^ (L1 >> 32) ^ (L1 << 32)
		^ state[pos].u[0];
	    lung->u[1] = (t1 << sl1) ^ (L0 >> 32) ^ (L0 << 32)
		^ state[pos].u[1];
	    state[index].u[0] = (lung->u[0] >> sr) ^ (lung->u[0] & mask1) ^ t0;
	    state[index].u[1] = (lung->u[1] >> sr) ^ (lung->u[1] & mask2) ^ t1;
	}
    };
}
#endif
