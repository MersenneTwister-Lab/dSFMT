/**
 * @file sample.c
 *
 * @brief sample jump using pre-computed jump polynomial.
 *
 * The jump polynomial used in this program was calculated like this:
<pre>
$ ./calc-jump 100000000000000000000 poly.1279.txt
jump polynomial:
2c6b058dca1fbfb57ebf41e67fec066c8828f2bf9414331d2767fa740aba89987685a79114b3543edbc83476e35fd1e52b8b2436932b9d18a728d8c7d7009a31aabed9bf4646909b8138f3e2a05e611c48dd1ce58a46183adabf3314da38599af92720efca1535872e7f85ef916b2c1e41dfe8ea764730f6ba2654ab287a55214bcf08cfae416906e4979108d606819b5d9e5b2f11ce028577f6c3788a9c688f1d64f5ae341eb95169824954
</pre>
*/
/*
 * In this sample, jump polynomial is fixed, then DSFMT_MEXP should be fixed.
 */
#define DSFMT_MEXP 1279
#include <stdio.h>
#include "dSFMT.h"
#include "dSFMT-jump.h"

/*
 * jump step is dSFMT internal state step,
 * for double, 2 * jump step numbers are skipped.
 */
static const int size = 5;
/* 10^20 will be enough */
static const char * jumppoly = "2c6b058dca1fbfb57ebf41e67fec066c8828f2bf"
    "9414331d2767fa740aba89987685a79114b3543edbc83476e35fd1e52b8b2436932"
    "b9d18a728d8c7d7009a31aabed9bf4646909b8138f3e2a05e611c48dd1ce58a4618"
    "3adabf3314da38599af92720efca1535872e7f85ef916b2c1e41dfe8ea764730f6b"
    "a2654ab287a55214bcf08cfae416906e4979108d606819b5d9e5b2f11ce028577f6"
    "c3788a9c688f1d64f5ae341eb95169824954";

int main(int argc, char * argv[])
{
    uint32_t seed = 4321;
    dsfmt_t dsfmt[size];

    dsfmt_init_gen_rand(&dsfmt[0], seed);
    for (int i = 1; i < size; i++) {
	dsfmt[i] = dsfmt[i - 1];
	dSFMT_jump(&dsfmt[i], jumppoly);
    }

    for (int i = 0; i < 10; i++) {
	/* simulating parallel generation */
	printf("%f %f %f %f %f\n",
	       dsfmt_genrand_close_open(&dsfmt[0]),
	       dsfmt_genrand_close_open(&dsfmt[1]),
	       dsfmt_genrand_close_open(&dsfmt[2]),
	       dsfmt_genrand_close_open(&dsfmt[3]),
	       dsfmt_genrand_close_open(&dsfmt[4]));
    }
    return 0;
}
