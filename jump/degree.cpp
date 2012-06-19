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
#include <fstream>
#include <inttypes.h>
#include <stdint.h>
#include <dSFMT-calc-jump.hpp>
#include <NTL/GF2X.h>
#include <NTL/vec_GF2.h>
#include <NTL/GF2XFactoring.h>

using namespace NTL;
using namespace std;
using namespace dsfmt;

static void read_file(GF2X& lcmpoly, long line_no, const string& file);

int main(int argc, char *argv[]) {
    if (argc < 2) {
	cout << argv[0] << " filename" << endl;
	return -1;
    }
    string filename = argv[1];
    GF2X poly;
    read_file(poly, 0, filename);

    cout << "degree = " << dec << deg(poly) << endl;
}

static void read_file(GF2X& lcmpoly, long line_no, const string& file)
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
    stringtopoly(lcmpoly, line);
}
