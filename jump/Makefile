#for GNU make

#DDEBUG = -O0 -g -DDEBUG=1
CPPOPT = -D__STDC_CONSTANT_MACROS=1
CC = gcc -Wall -O9 -I../ -Wmissing-prototypes $(DDEBUG) $(DMEXP) $(DPG)
CPP = g++ -Wall -O9 -I../ $(DDEBUG) $(DMEXP) $(DPG)

LINKOPT = -lntl -lgf2x

calc-characteristic: calc-characteristic.cpp dSFMText.hpp
	${CPP} ${CPPOPT} -o $@ calc-characteristic.cpp ${LINKOPT}

test-jump-M521: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPFLAGS} -DDSFMT_MEXP=521 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M1279: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPFLAGS} -DDSFMT_MEXP=1279 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M2203: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPFLAGS} -DDSFMT_MEXP=2203 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M4253: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPFLAGS} -DDSFMT_MEXP=4253 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M11213: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPFLAGS} -DDSFMT_MEXP=11213 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M4253-omp: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPFLAGS} -fopenmp -DDSFMT_MEXP=4253 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

.cpp.o:
	${CPP} -c $<

.c.o:
	${CC} -c $<

clean:
	rm -rf *.o *~ *.dSYM
