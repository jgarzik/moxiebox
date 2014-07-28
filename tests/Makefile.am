
TESTS = run-basic.sh

EXTRA_DIST = run-basic.sh

noinst_PROGRAMS = \
	basic

basic_SOURCES = basic.c

basic.o:	basic.c
	$(MOX_GCC) -I$(top_srcdir)/runtime -Wall -O2 -c $<
basic$(EXEEXT):	basic.o ../runtime/libsandboxrt.a
	$(MOX_GCC) -Wall -O2 -o $@ basic.o

