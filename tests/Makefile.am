
TESTS = run-basic.sh run-sha256.sh

EXTRA_DIST = run-basic.sh run-sha256.sh

noinst_PROGRAMS = \
	basic sha256-test

basic_SOURCES = basic.c

basic.o:	basic.c
	$(MOX_GCC) -I$(top_srcdir)/runtime -Wall -O2 -mel -static -nostdlib -c $<
basic$(EXEEXT):	basic.o ../runtime/libsandboxrt.a
	$(MOX_GCC) -Wall -O2 -mel -static -nostdlib -o $@ basic.o ../runtime/libsandboxrt.a

sha256_test_SOURCES = sha256.h sha256.c sha256-test.c

sha256.o:	sha256.c
	$(MOX_GCC) -I$(top_srcdir)/runtime -Wall -O2 -mel -static -nostdlib -c $<
sha256-test.o:	sha256-test.c
	$(MOX_GCC) -I$(top_srcdir)/runtime -Wall -O2 -mel -static -nostdlib -c $<
sha256-test$(EXEEXT):	sha256.o sha256-test.o ../runtime/libsandboxrt.a
	$(MOX_GCC) -Wall -O2 -mel -static -nostdlib -o $@ sha256.o sha256-test.o ../runtime/libsandboxrt.a
