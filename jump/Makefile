#for GNU make

#DDEBUG = -O0 -g -DDEBUG=1
CPPOPT = -D__STDC_CONSTANT_MACROS=1 -D__STDC_FORMAT_MACROS=1
CCOPT = -std=c99 -Wall -Wmissing-prototypes
CC = gcc -O9 -I../ $(DDEBUG) $(DMEXP) $(DPG)
CPP = g++ -Wall -O9 -I../ $(DDEBUG) $(DMEXP) $(DPG)
OMP = -fopenmp

LINKOPT = -lntl -lgf2x

calc-characteristic: calc-characteristic.cpp dSFMText.hpp
	${CPP} ${CPPOPT} -o $@ calc-characteristic.cpp ${LINKOPT}

calc-characteristic-mpi: calc-characteristic-mpi.cpp dSFMText.hpp
	${CPP} ${CPPOPT} ${OMP} -o $@ calc-characteristic-mpi.cpp ${LINKOPT}

calc-jump:calc-jump.cpp dSFMT-calc-jump.hpp
	${CPP} ${CPPOPT} -o $@ calc-jump.cpp ${LINKOPT}

factorization:factorization.cpp
	${CPP} ${CPPOPT} -o $@ factorization.cpp ${LINKOPT}

degree:degree.cpp
	${CPP} ${CPPOPT} -o $@ degree.cpp ${LINKOPT}

test-jump-M521: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPOPT} -DDSFMT_MEXP=521 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M1279: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPOPT} -DDSFMT_MEXP=1279 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M2203: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPOPT} -DDSFMT_MEXP=2203 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M4253: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPOPT} -DDSFMT_MEXP=4253 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M11213: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPOPT} -DDSFMT_MEXP=11213 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M19937: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPOPT} -DDSFMT_MEXP=19937 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M44497: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPOPT} -DDSFMT_MEXP=44497 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M86243: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPOPT} -DDSFMT_MEXP=86243 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M132049: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPOPT} -DDSFMT_MEXP=132049 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M216091: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPOPT} -DDSFMT_MEXP=216091 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M4253-omp: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPOPT} -fopenmp -DDSFMT_MEXP=4253 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

sample: sample.c ../dSFMT.c dSFMT-jump.c ../dSFMT-common.h dSFMT-jump.h
	${CC} ${CCOPT} -DDSFMT_MEXP=521 -DHAVE_SSE2 -o $@ \
	sample.c ../dSFMT.c dSFMT-jump.c

sample2: sample2.c ../dSFMT.c dSFMT-jump.c ../dSFMT-common.h dSFMT-jump.h
	${CC} ${CCOPT} -DDSFMT_MEXP=1279 -DHAVE_SSE2 -o $@ \
	sample2.c ../dSFMT.c dSFMT-jump.c

.cpp.o:
	${CPP} -c $<

.c.o:
	${CC} -c $<

clean:
	rm -rf *.o *~ *.dSYM
