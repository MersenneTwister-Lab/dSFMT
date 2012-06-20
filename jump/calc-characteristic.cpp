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
#include <iostream>
#include <inttypes.h>
#include <stdint.h>
#include <time.h>
#include "dSFMText.hpp"
#include "dSFMT-calc-jump.hpp"
#include <NTL/GF2X.h>
#include <NTL/vec_GF2.h>
#include <NTL/GF2XFactoring.h>

using namespace dsfmt;
using namespace NTL;
using namespace std;

void calc_minimal(GF2X& minimal, int maxdegree, w128_t outseq[], int pos)
{
    uint64_t mask = 0;
    vec_GF2 seq;
    seq.SetLength(2 * maxdegree);
    int idx = 0;

    if (pos >= 104 || pos < 0) {
	cerr << "pos error:" << dec << pos << endl;
	exit(1);
    } else if (pos >= 52) {
	idx = 1;
	mask = UINT64_C(1) << (pos - 52);
    } else {
	idx = 0;
	mask = UINT64_C(1) << pos;
    }
    if (idx < 0 || idx >= 2) {
	cerr << "idx error:" << dec << idx << endl;
	exit(1);
    }
    for (int i = 0; i < 2 * maxdegree; i++) {
	if ((outseq[i].u[idx] & mask) != 0) {
	    seq[i] = 1;
	} else {
	    seq[i] = 0;
	}
    }
    MinPolySeq(minimal, seq, maxdegree);
#if defined(DEBUG)
    cout << "deg(minimal) = " << dec << deg(minimal) << endl;
#endif
}

void LCM(GF2X& lcm, const GF2X& x, const GF2X& y) {
    GF2X gcd;
    mul(lcm, x, y);
    GCD(gcd, x, y);
//void DivRem(GF2X& q, GF2X& r, const GF2X& a, const GF2X& b);
// q = a/b, r = a%b
    GF2X r;
    DivRem(lcm, r, lcm, gcd);
    if (deg(r) != -1) {
	cerr << "LCM error: deg(r) = " << dec << deg(r);
	cerr << "r:" << r << endl;
	exit(1);
    }
}

static void get_lcm_sub(GF2X& lcmpoly, dSFMText& dsfmt) {
    GF2X tmp(1,1);
    int maxdegree = dsfmt.get_maxdegree();
    w128_t out_seq[2 * maxdegree];

    for (int i = 0; i < 2 * maxdegree; i++) {
	out_seq[i] = dsfmt.next();
    }
    GF2X minimal;
    GF2X t1;
    SetCoeff(t1, 0, 1);
    SetCoeff(t1, 1, 1);
    for (int pos = 0; pos < 104; pos++) {
	calc_minimal(minimal, maxdegree, out_seq, pos);
	LCM(tmp, lcmpoly, minimal);
	lcmpoly = tmp;
	if (divide(lcmpoly, t1)) {
	    return;
	}
    }
#if defined(DEBUG)
    cout << "deg(lcmpoly) = " << dec << deg(lcmpoly) << endl;
#endif
}

void get_lcm(GF2X& lcmpoly, dSFMText& dsfmt) {
    int maxdegree = dsfmt.get_maxdegree();
    GF2X t1;
    SetCoeff(t1, 0, 1);
    SetCoeff(t1, 1, 1);

    //uint32_t time = (uint32_t)clock();
    dsfmt.seeding(1234);
    get_lcm_sub(lcmpoly, dsfmt);
    if (deg(lcmpoly) >= maxdegree) {
	return;
    }
    if (divide(lcmpoly, t1)) {
	return;
    }
    for(int i = 0; i < maxdegree; i++) {
	dsfmt.init_basis(i);
	get_lcm_sub(lcmpoly, dsfmt);
	if (deg(lcmpoly) >= maxdegree) {
	    return;
	}
	if (divide(lcmpoly, t1)) {
	    return;
	}
    }
}


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

void check_fix(dSFMText & fix)
{
    fix.setup_high();
    cout << "before:";
    fix.print(cout);
    fix.next();
    cout << "after 1 step:";
    fix.print(cout);
    fix.next();
    cout << "after 2 step:";
    fix.print(cout);
    fix.next();
    cout << "after 3 step:";
    fix.print(cout);
}

void check_fix2(dSFMText & fix)
{
    cout << "before:";
    fix.print(cout);
    fix.next_add();
    cout << "after 1 step:";
    fix.print(cout);
    fix.next_add();
    cout << "after 2 step:";
    fix.print(cout);
    fix.next_add();
    cout << "after 3 step:";
    fix.print(cout);
}

void calc_fix(GF2X& inv, int mexp, int pos1, int sl1,
	      uint64_t mask1, uint64_t mask2)
{
    dSFMText fix(mexp, pos1, sl1, mask1, mask2);
    //fix.setup_high();
    //fix.next();
    //cout << "setup0:";
    //fix.print(cout);
    fix.setup_constants();
#if defined(DEBUG)
    cout << "setup:";
    fix.print(cout);
#endif
    dSFMText work(mexp, pos1, sl1, mask1, mask2);
#if defined(DEBUG)
    cout << "zero:";
    work.print(cout);
#endif
    for (long i = 0; i <= deg(inv); i++) {
	if (IsOne(coeff(inv, i))) {
	    work.add(fix);
#if defined(DEBUG) && 0
    cout << "work:";
    work.print(cout);
#endif
	}
	fix.next();
    }
    cout << "fix:";
    work.print(cout);
    check_fix(work);
}

void check_const(GF2X& lcm, int mexp, int pos1, int sl1,
	      uint64_t mask1, uint64_t mask2)
{
    dSFMText normal(mexp, pos1, sl1, mask1, mask2);
    dSFMText add(mexp, pos1, sl1, mask1, mask2);
    normal.seeding(123);
    add.seeding(123);

    normal.setup_high();
    normal.next();
    add.next_add();
    cout << "normal:";
    normal.print(cout);
    cout << "add:";
    add.print(cout);
    for (int i = 0; i < 5; i++) {
	normal.next();
	add.next_add();
    }
    cout << "normal:";
    normal.print(cout);
    cout << "add:";
    add.print(cout);
}

void check_gcd(GF2X& d, GF2X& s, GF2X& inv, GF2X& work, GF2X& t1)
{
//    XGCD(d, s, inv, work, t1);
//    check_gcd(d, s, inv, work, t1);
//void XGCD(GF2X& d, GF2X& s, GF2X& t, const GF2X& a, const GF2X& b);
// d = gcd(a,b), a s + b t = d
    if (deg(d) != 0) {
	cout << "check_gcd d != 1:" << dec << deg(d) << endl;
	return;
    }
    GF2X p;
    GF2X q;
    mul(p, s, work);
    mul(q, inv, t1);
    p += q;
    if (p != d) {
	cout << "check_gcd p != d, p = " << p << endl;
	cout << "check_gcd p != d, d = " << d << endl;
    } else {
	cout << "check_gcd OK d:" << d << endl;
    }
}
int main(int argc, char *argv[]) {
    if (argc < 6) {
	cout << argv[0] << " mexp pos1 sl1 mask1 mask2" << endl;
	return -1;
    }
    int mexp = strtol(argv[1], NULL, 10);
    int pos1 = strtol(argv[2], NULL, 10);
    int sl1 = strtol(argv[3], NULL, 10);
    uint64_t mask[2];
    mask[0] = strtoull(argv[4], NULL, 16);
    mask[1] = strtoull(argv[5], NULL, 16);
#if defined(DEBUG)
    cout << "mexp:" << dec << mexp << endl;
    cout << "pos1:" << dec << pos1 << endl;
    cout << "sl1:" << dec << sl1 << endl;
    cout << "mask1:" << hex<< mask[0] << endl;
    cout << "mask2:" << hex << mask[1] << endl;
#endif
    dSFMText dsfmt(mexp, pos1, sl1, mask[0], mask[1]);
    GF2X characteristic(0,1);
    get_lcm(characteristic, dsfmt);
#if defined(DEBUG)
    cout << "degree:" << dec << deg(characteristic) << endl;
    cout << "maxdegree = " << dec << dsfmt.get_mamaxdegree() << endl;
    cout << characteristic << endl;
#endif
    GF2X work;
    work = characteristic;
#if defined(DEBUG)
    cout << "degree:" << deg(characteristic) << endl;
    cout << characteristic << endl;
#endif
    if (!has_large_irreducible(characteristic, mexp)) {
        cout << "error?" << endl;
        return -1;
    }
#if 0
    GF2X remain = work / characteristic;
    vec_pair_GF2X_long factors;
    CanZass(factors, remain);
    cout << "degree of work:" << dec << deg(work) << endl;
    cout << "degree of remain:" << dec << deg(remain) << endl;
    cout << "=== factor of remain ===" << endl;
    for (int i = 0; i < factors.length(); i++) {
	cout << factors[i].a;
	cout << ":";
	cout << dec << deg(factors[i].a);
	cout << ":";
	cout << dec << factors[i].b << endl;
    }
#endif
    GF2X d, s, inv;
    GF2X t1;
    SetCoeff(t1, 0, 1);
    SetCoeff(t1, 1, 1);
    XGCD(d, s, inv, work, t1);
    check_gcd(d, s, inv, work, t1);
// d = gcd(a,b), a s + b t = d
//    MulMod(s, inv, t1, work);
#if defined(DEBUG)
    cout << "deg work:" << dec << deg(work) << endl;
    cout << "deg inv:" << dec << deg(inv) << endl;
#endif
    string x;
//    cout << "# deg = " << dec << deg(work) << endl;
    polytostring(x, work);
    cout << "#" << dec << mexp;
    cout << "," << dec << pos1;
    cout << "," << dec << sl1;
    cout << "," << hex << mask[0];
    cout << "," << hex << mask[1];
    cout << dec << endl;
    cout << x << endl;
    cout << dec << flush;
    if (deg(d) != 0) {
	cout << "doesn't have fixpoint." << endl;
	return 0;
    }
#if 0
    string y;
    polytostring(y, inv);
    if (deg(d) == 0) {
	cout << y << endl;
    } else {
	cout << "can't finf inv d=" << d << endl;
    }
#endif
    calc_fix(inv, mexp, pos1, sl1, mask[0], mask[1]);
    //check_const(inv, mexp, pos1, sl1, mask[0], mask[1]);
}
