/**
 * @file sample.c
 *
 * @brief sample jump using pre-computed jump polynomial.
 *
 * The jump polynomial used in this program was calculated like this:
<pre>
$ ./calc-jump 10000 lcm.521.txt
jump polynomial:
61cfe6581dea7caf5009f3366e7ba5c81e78da9f2700af2ad3cddc005b1b542bf9295a1a81c7d41ebae4edd16780347d604da7b861074aa7abadbfb4a5cbf9ea0d6a65bbd
</pre>
* This jump step is too small for actual use, just for check.
* Use larger step for your application.
*/
/*
 * In this sample, jump polynomial is fixed, then DSFMT_MEXP should be fixed.
 */
#define DSFMT_MEXP 521
#include <stdio.h>
#include "dSFMT.h"
#include "dSFMT-jump.h"

/*
 * jump step is dSFMT internal state step,
 * for double, 2 * jump step numbers are skipped.
 */
static const int step = 2 * 2000;
static const int size = 5;
static const char * jump2000  = "19976402611e3971602315ac42c1362b69367b411"
    "1785f40aa0ab67ce3e41e8d60eb1f3b012ed9854d7b594094b4b58670b30b9b1ea18a"
    "dba31d715592b6595195e01d458";
static const char * jump8000 = "b615c968ff87afc24e13d91dc2c27737a396667f6e"
    "b2979039b0de4d622a169d4d0b8200d2ee3b86d5f4528573abb4e55330bb0f002d66b"
    "762f42956f74a38e1cb5dbee2c";

int main(int argc, char * argv[])
{
    uint32_t seed = 1234;
    dsfmt_t dsfmt[size];
    dsfmt_t serial;
    uint32_t array[size * step];

    /* set up initial position of six sfmt instances
     * sfmt[0]: position 0 -- 2000, jump, 10000 -- 12000
     * sfmt[1]: position 2000 -- 4000, jump, 12000 -- 14000
     * sfmt[2]: position 4000 -- 6000
     * sfmt[3]: position 6000 -- 8000
     * sfmt[4]: position 8000 -- 10000
     */
    dsfmt_init_gen_rand(&dsfmt[0], seed);
    for (int i = 1; i < size; i++) {
	dsfmt[i] = dsfmt[i - 1];
	dSFMT_jump(&dsfmt[i], jump2000);
    }
    /* initialize serial generator */
    dsfmt_init_gen_rand(&serial, seed);
    /* generate and compare with serial generation */
    for (int i = 0; i < 10; i++) {
	/* simulating parallel generation */
	for (int j = 0; j < step; j++) {
	    array[j + 0 * step] = dsfmt_genrand_uint32(&dsfmt[0]);
	    array[j + 1 * step] = dsfmt_genrand_uint32(&dsfmt[1]);
	    array[j + 2 * step] = dsfmt_genrand_uint32(&dsfmt[2]);
	    array[j + 3 * step] = dsfmt_genrand_uint32(&dsfmt[3]);
	    array[j + 4 * step] = dsfmt_genrand_uint32(&dsfmt[4]);
	}
	/* checking */
	printf("comparing with serial generation %d to %d",
	       i * 10000, (i + 1) * 10000 -1);
	for (int j = 0; j < size * step; j++) {
	    if (dsfmt_genrand_uint32(&serial) != array[j]) {
		printf("something wrong! i=%d, j=%d\n", i, j);
		return -1;
	    }
	}
	printf(" OK\n");
	/* simulating parallel jump */
	dSFMT_jump(&dsfmt[0], jump8000);
	dSFMT_jump(&dsfmt[1], jump8000);
	dSFMT_jump(&dsfmt[2], jump8000);
	dSFMT_jump(&dsfmt[3], jump8000);
	dSFMT_jump(&dsfmt[4], jump8000);
    }
    return 0;
}
