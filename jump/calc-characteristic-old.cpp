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
#include "dSFMText.hpp"
#include "dSFMT-calc-jump.hpp"
#include <NTL/GF2X.h>
#include <NTL/vec_GF2.h>
#include <NTL/GF2XFactoring.h>

using namespace dsfmt;
using namespace NTL;
using namespace std;

void calc_minimal(GF2X& minimal, dSFMText& dsfmt, int bitpos)
{
    uint64_t mask[2];
    if (bitpos >= 52) {
	mask[0] = 0;
	mask[1] = UINT64_C(1) << (bitpos - 52);
    } else {
	mask[0] = UINT64_C(1) << bitpos;
	mask[1] = 0;
    }
    int maxdegree = dsfmt.get_mamaxdegree();
    vec_GF2 seq;
    seq.SetLength(2 * maxdegree);
    for (int i = 0; i < 2 * maxdegree; i++) {
	seq[i] = dsfmt.next(mask);
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
    lcm /= gcd;
}

void get_lcm(GF2X& lcmpoly, dSFMText& dsfmt) {
    GF2X minimal;
    GF2X tmp;
    int maxdegree = dsfmt.get_mamaxdegree();
    dsfmt.seeding(1234);
//    dsfmt.set_high()
//    dsfmt.seeding(1234, 1);
    for (int bitpos = 0; bitpos < 52 * 2; bitpos++) {
	calc_minimal(minimal, dsfmt, bitpos);
	LCM(tmp, lcmpoly, minimal);
	lcmpoly = tmp;
#if defined(DEBUG)
    cout << "deg(lcm) = " << dec << deg(lcmpoly) << endl;
#endif
#if 0
	if (deg(lcmpoly) == maxdegree) {
	    return;
	}
	if (deg(lcmpoly) > maxdegree) {
	    return;
	}
#endif
    }
#if defined(DEBUG)
    cout << "deg(lcm) = " << dec << deg(lcmpoly) << endl;
#endif
#if 1
    for(int i = 0; i < maxdegree; i++) {
	dsfmt.init_basis();
	for (int bitpos = 0; bitpos < 52 * 2; bitpos++) {
	    calc_minimal(minimal, dsfmt, bitpos);
	    LCM(tmp, lcmpoly, minimal);
	    lcmpoly = tmp;
#if 0
	    if (deg(lcmpoly) == maxdegree) {
		return;
	    }
#endif
	}
    }
#endif
#if 0
    cerr << "can't find lcm deg=" << dec << deg(lcmpoly) << endl;
    cerr << "maxdegree = " << dec << maxdegree << endl;
    throw new logic_error("can't find lcm");
#endif
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

void calc_fix(GF2X& lcm, int mexp, int pos1, int sl1,
	      uint64_t mask1, uint64_t mask2)
{
    dSFMText fix(mexp, pos1, sl1, mask1, mask2);
    //fix.setup_high();
    //fix.next();
    //cout << "setup0:";
    //fix.print(cout);
    fix.setup_constants();
    cout << "setup:";
    fix.print(cout);
    dSFMText work(mexp, pos1, sl1, mask1, mask2);
    cout << "zero:";
    work.print(cout);
    for (int i = 0; i <= deg(lcm); i++) {
	if (IsOne(coeff(lcm, i))) {
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
    check_fix2(work);
#if 0
    cout << "* (t+1)" << endl;
    GF2X t1(1,1);
    dSFMText work2(mexp, pos1, sl1, mask1, mask2);
    cout << "work2:";
    work2.print(cout);
    for (int i = 0; i <= deg(t1); i++) {
	if (IsOne(coeff(t1, i))) {
	    work2.add(work);
#if defined(DEBUG)
    cout << "work2:";
    work2.print(cout);
#endif
	}
	work.next();
    }
    cout << "work2:";
    work2.print(cout);
#endif
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
    cerr << "maxdegree = " << dec << dsfmt.get_mamaxdegree() << endl;
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
    cout << "=== factor of remain ===" << endl;
    for (int i = 0; i < factors.length(); i++) {
	cout << factors[i].a;
	cout << ":";
	cout << factors[i].b << endl;
    }
    cout << "=== factor of remain ===" << endl;
#endif
    GF2X d, s, inv;
    GF2X t1(1,1);
    XGCD(d, s, inv, work, t1);
// d = gcd(a,b), a s + b t = d
    MulMod(s, inv, t1, work);
    if (deg(s) != 0) {
	cout << "inv is not inv." << endl;
    }

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
#if 0
    string y;
    polytostring(y, inv);
    if (deg(d) == 0) {
	cout << y << endl;
    } else {
	cout << "can't finf inv d=" << d << endl;
    }
    calc_fix(inv, mexp, pos1, sl1, mask[0], mask[1]);
    //check_const(inv, mexp, pos1, sl1, mask[0], mask[1]);
#endif
}
