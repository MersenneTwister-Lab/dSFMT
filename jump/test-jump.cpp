/**
 * @file test-jump.cpp
 *
 * @brief test jump function.
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
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <inttypes.h>
#include <stdint.h>
#include <time.h>
#include <NTL/GF2X.h>
#include <NTL/vec_GF2.h>
#include <NTL/ZZ.h>
#include "dSFMT-calc-jump.hpp"
#include "dSFMT-jump.h"

using namespace NTL;
using namespace std;
using namespace dsfmt;

static void read_file(GF2X& characteristic, int line_no, const string& file);
static int test(dsfmt_t * dsfmt, GF2X& poly);
static int check(dsfmt_t *a, dsfmt_t *b, int verbose);
static void print_state(dsfmt_t *a, dsfmt_t * b);
static void print_state_line(w128_t *a, w128_t *b);
static void print_sequence(dsfmt_t *a, dsfmt_t * b);
static int speed(dsfmt_t * dsfmt, GF2X& characteristic);

int main(int argc, char * argv[]) {
    if (argc <= 1) {
	printf("%s -s|-c [prefix]\n", argv[0]);
	return -1;
    }
    string prefix = "poly.";
    if (argc >= 3) {
	prefix = argv[2];
    }
    stringstream ss_file;
    GF2X characteristic;
    ss_file << prefix << DSFMT_MEXP << ".txt";
    read_file(characteristic, 0, ss_file.str());
    dsfmt_t dsfmt;
    if (argv[1][1] == 's') {
	return speed(&dsfmt, characteristic);
    } else {
	return test(&dsfmt, characteristic);
    }
}

static int speed(dsfmt_t * dsfmt, GF2X& characteristic)
{
    uint32_t seed = 1234;
    long step = 10000;
    int exp = 4;
    ZZ test_count;
    string jump_string;
    clock_t start;
    double elapsed1;
    double elapsed2;

    dsfmt_init_gen_rand(dsfmt, seed);
    test_count = step;
    for (int i = 0; i < 10; i++) {
	start = clock();
	calc_jump(jump_string, test_count, characteristic);
	elapsed1 = clock() - start;
	elapsed1 = elapsed1 * 1000 / CLOCKS_PER_SEC;
	cout << "mexp "
	     << setw(5)
	     << DSFMT_MEXP
	     << " jump 10^"
	     << setfill('0') << setw(2)
	     << exp
	     << " steps  calc_jump:"
	     << setfill(' ') << setiosflags(ios::fixed)
	     << setw(6) << setprecision(3)
	     << elapsed1
	     << "ms"
	     << endl;
	start = clock();

	for (int j = 0; j < 10; j++) {
	    dSFMT_jump(dsfmt, jump_string.c_str());
	}
	elapsed2 = clock() - start;
	elapsed2 = elapsed2 * 1000 / 10 / CLOCKS_PER_SEC;
	cout << "mexp "
	     << setw(5)
	     << DSFMT_MEXP
	     << " jump 10^"
	     << setfill('0') << setw(2)
	     << exp
	     << " steps dSFMT_jump:"
	     << setfill(' ') << setiosflags(ios::fixed)
	     << setw(6) << setprecision(3)
	     << elapsed2
	     << "ms"
	     << endl;
	test_count *= 100;
	exp += 2;
    }
    return 0;
}

static void read_file(GF2X& characteristic, int line_no, const string& file)
{
    ifstream ifs(file.c_str());
    string line;
    for (int i = 0; i < line_no; i++) {
	ifs >> line;
	ifs >> line;
    }
    if (ifs) {
	ifs >> line;
	line = "";
	ifs >> line;
    }
    stringtopoly(characteristic, line);
#if defined(DEBUG)
    cout << "line = " << line << endl;
    cout << "deg = " << deg(characteristic) << endl;
    cout << "cha = " << characteristic << endl;
    string x;
    polytostring(x, characteristic);
    cout << "x = " << x << endl;
#endif
}

static int check(dsfmt_t *a, dsfmt_t *b, int verbose)
{
    int check = 0;
    for (int i = 0; i < 100; i++) {
	double x = dsfmt_genrand_close_open(a);
	double y = dsfmt_genrand_close_open(b);
	if (x != y) {
	    print_state(a, b);
	    print_sequence(a, b);
	    check = 1;
	    break;
	}
    }
    if (check == 0) {
	if (verbose) {
	    printf("OK!\n");
	}
    } else {
	printf("NG!\n");
    }
    return check;
}

static void print_state_line(w128_t *a, w128_t *b)
{
    printf("[");
    for (int j = 0; j < 2; j++) {
	printf("%016"PRIx64, a->u[j]);
	if (j == 0) {
	    printf(" ");
	} else {
	    printf("]");
	}
    }
    printf("[");
    for (int j = 0; j < 2; j++) {
	printf("%016"PRIx64, b->u[j]);
	if (j == 0) {
	    printf(" ");
	} else {
	    printf("]");
	}
    }
    printf("\n");
}

static void print_state(dsfmt_t *a, dsfmt_t * b)
{
    printf("idx = %d                            idx = %d\n",
	   a->idx, b->idx);
    for (int i = 0; (i < 10) && (i < DSFMT_N); i++) {
	print_state_line(&a->status[i], &b->status[i]);
    }
    print_state_line(&a->status[DSFMT_N], &b->status[DSFMT_N]);
}

static void print_sequence(dsfmt_t *a, dsfmt_t * b)
{
    for (int i = 0; i < 25; i++) {
	double c, d;
	c = dsfmt_genrand_close_open(a);
	d = dsfmt_genrand_close_open(b);
	printf("[%1.15f %1.15f]\n", c, d);
    }
}

static int test(dsfmt_t * dsfmt, GF2X& characteristic)
{
    dsfmt_t new_dsfmt_z;
    dsfmt_t * new_dsfmt = &new_dsfmt_z;
    uint32_t seed[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    long steps[] = {1, 2, DSFMT_N + 1,
		    DSFMT_N * 128 - 1,
		    DSFMT_N * 128 + 1,
		    3003,
		    3004,
		    200004,
		    200005,
		    10000005,
		    10000006};
    int steps_size = sizeof(steps) / sizeof(long);
    ZZ test_count;
    string jump_string;

    dsfmt_init_gen_rand(dsfmt, seed[0]);
    dsfmt_genrand_close_open(dsfmt);
    /* plus jump */
    for (int index = 0; index < steps_size; index++) {
//	dsfmt_init(dsfmt, seed[index]);
	test_count = steps[index];
	cout << "mexp " << dec << DSFMT_MEXP << " jump "
	     << test_count << " steps" << endl;
	*new_dsfmt = *dsfmt;
	for (long i = 0; i < steps[index] * 2; i++) {
	    dsfmt_genrand_uint32(dsfmt);
	}
	calc_jump(jump_string, test_count, characteristic);
#if defined(DEBUG)
	cout << "jump string:" << jump_string << endl;
	cout << "before jump:" << endl;
	print_state(new_dsfmt, dsfmt);
#endif
	dSFMT_jump(new_dsfmt, jump_string.c_str());
#if defined(DEBUG)
	cout << "after jump:" << endl;
	print_state(new_dsfmt, dsfmt);
#endif
	if (check(new_dsfmt, dsfmt, 1)) {
	    return 1;
	}
    }
    dsfmt_t rnd;
    dsfmt_init_gen_rand(&rnd, (uint32_t)clock());
    for (int index = 0; index < 100; index++) {
	dsfmt_init_gen_rand(dsfmt, dsfmt_genrand_uint32(&rnd));
	test_count = dsfmt_genrand_uint32(&rnd) % 100000;
	*new_dsfmt = *dsfmt;
	for (long i = 0; i < test_count * 2; i++) {
	    dsfmt_genrand_uint32(dsfmt);
	}
	calc_jump(jump_string, test_count, characteristic);
	dSFMT_jump(new_dsfmt, jump_string.c_str());
	if (check(new_dsfmt, dsfmt, 0)) {
	    cout << "mexp " << dec << DSFMT_MEXP << " jump "
		 << test_count << " steps ";
	    cout << "check NG!" << endl;
	    return 1;
	}
    }
    return 0;
}
