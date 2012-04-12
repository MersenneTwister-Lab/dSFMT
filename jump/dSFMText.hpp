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
#include <stdexcept>
#include <cstdint>
#include <cinttypes>

namespace dsfmt {
    using namespace std;
    using namespace NTL;

    union w128_t {
	uint64_t u[2];
	uint32_t u32[4];
    };

    class dsfmt_ext {
    public:
	/**
	 * Constructor from parameters.
	 */
	dsfmt_ext(int mexp, int sl1, int sr, int pos1,
		  uint64_t mask1, uint64_t mask2) {
		this.mexp = mexp;
		this.sl1 = sl1;
		this.sr = sr;
		this.pos1 = pos1;
		this.mask1 = mask1;
		this.mask2 = mask2;
		this.size = calc_size(mexp);
		this.maxdegree = (this.size - 1) * 52 * 2 + 128;
		this.state = new w128_t[size];
		this.index = 0;
		this.bitpos = 0;
	};

	/**
	 * release internal state.
	 */
	~dsfmt_ext() {
	    delete[] state;
	};

	void init_basis() {
	};

	/**
	 * generate next 128 bits.
	 * @returns 128-bit number.
	 */
	w128_t next() {
	    do_recursion();
	    w128_t result = state[index];
	    index++;
	    if (index >= size - 1) {
		index = 0;
	    }
	    return result;
	};

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
	    size = size / (52 * 2) + 1;
	    return size;
	};

	void init_basis() {
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
		k = bit_pos % 52;
		j = (bit_pos / 52) % 2;
		i = bit_pos / (52 * 2);
 	    }
	    memset(state, 0, sizeof(w128_t) * size);
	    mask = UINT64_C(1) << k;
	    status[i].u[j] = mask;
	    bitpos++;
	};

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
	    lung->.u[1] = (t1 << sl1) ^ (L0 >> 32) ^ (L0 << 32)
		^ state[pos].u[1];
	    state[index].u[0] = (lung->u[0] >> sr) ^ (lung->u[0] & msk1) ^ t0;
	    state[index].u[1] = (lung->u[1] >> sr) ^ (lung->u[1] & msk2) ^ t1;
	};
    }
#endif
