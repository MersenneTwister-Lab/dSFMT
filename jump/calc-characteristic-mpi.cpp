/**
 * @file calc-characteristic.cpp
 *
 * @brief calculate characteristic polynomial.
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
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <inttypes.h>
#include <stdint.h>
#include "dSFMText.hpp"
#include "dSFMT-calc-jump.hpp"
#include <NTL/GF2X.h>
#include <NTL/vec_GF2.h>
#include <NTL/GF2XFactoring.h>

using namespace dsfmt;
using namespace NTL;
using namespace std;
static void get_lcm_sub(GF2X& lcmpoly, dSFMText& dsfmt);

void calc_minimal(GF2X& minimal, int maxdegree, w128_t outseq[], int bitpos)
{
    uint64_t mask;
    vec_GF2 seq;
    seq.SetLength(2 * maxdegree);
    int idx;
    if (bitpos >= 64) {
	idx = 1;
	mask = UINT64_C(1) << (bitpos - 64);
    } else {
	idx = 0;
	mask = UINT64_C(1) << bitpos;
    }
    for (int i = 0; i < 2 * maxdegree; i++) {
	if (outseq[i].u[idx] & mask) {
	    seq[i] = 1;
	} else {
	    seq[i] = 0;
	}
    }
    MinPolySeq(minimal, seq, maxdegree);
}

void LCM(GF2X& lcm, const GF2X& x, const GF2X& y) {
    GF2X gcd;
    mul(lcm, x, y);
    GCD(gcd, x, y);
    lcm /= gcd;
}

void get_lcm(int rank, int num_process, GF2X& lcmpoly, dSFMText& dsfmt) {
    int maxdegree = dsfmt.get_mamaxdegree();
    dsfmt.seeding(1234);
    get_lcm_sub(lcmpoly, dsfmt);
    int unit = maxdegree / num_process;
    int start = rank * num_process;
    for(int i = start; i < start + unit; i++) {
	dsfmt.init_basis(i);
	get_lcm_sub(lcmpoly, dsfmt);
    }
}

static void get_lcm_sub(GF2X& lcmpoly, dSFMText& dsfmt) {
    GF2X tmp;
    int mamaxdegree = dsfmt.get_mamaxdegree();
    w128_t out_seq[2 * mamaxdegree];
    int i, bitpos;

    for (int i = 0; i < 2 * mamaxdegree; i++) {
	out_seq[i] = dsfmt.next();
    }

    GF2X minimal;
    for (bitpos = 0; bitpos < 128; bitpos++) {
	calc_minimal(minimal, mamaxdegree, out_seq, bitpos);
	LCM(tmp, lcmpoly, minimal);
	lcmpoly = tmp;
    }
}

#if defined(IRRE_CHECK)
static int has_large_irreducible(GF2X& fpoly, int degree) {
    static const GF2X t2(2, 1);
    static const GF2X t1(1, 1);
    GF2X t2m;
    GF2X t;
    GF2X alpha;
    int m;

    t2m = t2;
    if (deg(fpoly) < degree) {
	return 0;
    }
    t = t1;
    t += t2m;

    for (m = 1; deg(fpoly) > degree; m++) {
	for(;;) {
	    GCD(alpha, fpoly, t);
	    if (IsOne(alpha)) {
		break;
	    }
	    fpoly /= alpha;
	    if (deg(fpoly) < degree) {
		return 0;
	    }
	}
	t2m *= t2m;
	t2m %= fpoly;
	add(t, t2m, t1);
    }
    if (deg(fpoly) != degree) {
	return 0;
    }
    return IterIrredTest(fpoly);
}
#endif

int main(int argc, char *argv[]) {
    int rank;
    int num_process;
    int MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);
    if (argc < 6) {
	cout << argv[0] << " mexp pos1 sl1 mask1 mask2" << endl;
	MPI_Finalize();
	return -1;
    }
    int mexp = strtol(argv[1], NULL, 10);
    int pos1 = strtol(argv[2], NULL, 10);
    int sl1 = strtol(argv[3], NULL, 10);
    uint64_t mask[2];
    mask[0] = strtoull(argv[4], NULL, 16);
    mask[1] = strtoull(argv[5], NULL, 16);
    stringstream ss;
    ss << "lcm." << dec << mexp << "." << rank << ".txt";
    string fname;
    ss >> fname;
    //ofstream fout("/tmp/results.txt", ios::trunc );
    ofstream fout(fname.c_str());

    dSFMText dsfmt(mexp, pos1, sl1, mask[0], mask[1]);
    GF2X characteristic(0,1);
    get_lcm(rank, num_process, characteristic, dsfmt);
    GF2X work;
    work = characteristic;
#if defined(IRRE_CHECK)
    if (!has_large_irreducible(characteristic, mexp)) {
        fout << "error? does not have large irreducible" << endl;
	MPI_Finalize();
        return -1;
    }
#endif
    string x;
    polytostring(x, work);
    fout << "#" << dec << mexp;
    fout << "," << dec << pos1;
    fout << "," << dec << sl1;
    fout << "," << hex << mask[0];
    fout << "," << hex << mask[1];
    fout << dec << endl;
    fout << x << endl;
    fout << dec << flush;
    MPI_Finalize();
}
