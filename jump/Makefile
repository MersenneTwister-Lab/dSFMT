#for GNU make

#DDEBUG = -O0 -g -DDEBUG=1
# uncomment next line if you installed NTL with gf2x
#LIBGF2X = -lgf2x
# uncomment next line if you installed NTL with gmp
#LIBGMP = -lgmp

DEFINES = -D__STDC_CONSTANT_MACROS=1 -D__STDC_FORMAT_MACROS=1
CPP = g++
#CPP = icc
GPPFLAGS = -msse2
#ICCPPFLAGS = -mssse3

CC = gcc
#CC = icc -fast
GCCFLAGS = -msse2 --param max-inline-insns-single=1800
#ICCFLAGS = -fast
OPTI = -O3 -finline-functions -fomit-frame-pointer
CCSTD = -std=c99
WARN = -Wall #-Winline
CCFLAGS = -I../ -Wmissing-prototypes $(GCCFLAGS) $(ICCFLAGS) $(OPTI) \
	$(WARN) $(CCSTD) $(DDEBUG)
CPPFLAGS = -I../ $(OPTI) $(GPPFLAGS) $(ICCPPFLAGS) $(WARN) $(DEFINES) $(DDEBUG)

LINKOPT = -lntl $(LIBGF2X) $(LIBGMP)

ALL_JUMP_TARGET = calc-jump test-jump-M521 test-jump-M1279 test-jump-M2203 \
test-jump-M4253 test-jump-M11213 test-jump-M19937 test-jump-M44497 \
test-jump-M86243 test-jump-M132049 test-jump-M216091

#
# jump
#
all:calc-jump

check: ${ALL_JUMP_TARGET}
	./check-jump.sh

calc-jump:calc-jump.cpp dSFMT-calc-jump.hpp
	${CPP} ${CPPFLAGS} -o $@ calc-jump.cpp ${LINKOPT}

#
# sample
#
sample1: sample1.c ../dSFMT.c dSFMT-jump.c ../dSFMT-common.h dSFMT-jump.h
	${CC} ${CCFLAGS} -DDSFMT_MEXP=1279 -DHAVE_SSE2 -o $@ \
	sample1.c ../dSFMT.c dSFMT-jump.c

sample2: sample2.c ../dSFMT.c dSFMT-jump.c ../dSFMT-common.h dSFMT-jump.h
	${CC} ${CCFLAGS} -DDSFMT_MEXP=521 -DHAVE_SSE2 -o $@ \
	sample2.c ../dSFMT.c dSFMT-jump.c

#
# test
#
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

test-jump-M19937: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPFLAGS} -DDSFMT_MEXP=19937 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M44497: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPFLAGS} -DDSFMT_MEXP=44497 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M86243: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPFLAGS} -DDSFMT_MEXP=86243 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M132049: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPFLAGS} -DDSFMT_MEXP=132049 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

test-jump-M216091: test-jump.cpp ../dSFMT.c dSFMT-jump.c dSFMT-calc-jump.hpp \
	../dSFMT-common.h dSFMT-jump.h
	${CPP} ${CPPFLAGS} -DDSFMT_MEXP=216091 -DHAVE_SSE2 -o $@ \
	test-jump.cpp ../dSFMT.c dSFMT-jump.c ${LINKOPT}

.cpp.o:
	${CPP} -c $<

.c.o:
	${CC} -c $<

clean:
	rm -rf *.o *~ *.dSYM

clean-all:
	rm -f $(ALL_JUMP_TARGET)
