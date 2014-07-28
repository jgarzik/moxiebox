
TESTS = \
	run-basic.sh \
	run-exit.sh

EXTRA_DIST = \
	run-basic.sh \
	run-exit.sh

noinst_PROGRAMS = \
	basic \
	exit0 \
	exit1

basic_SOURCES = basic.c

basic.o:	basic.c
	$(MOX_GCC) -I$(top_srcdir)/runtime -Wall -O2 -c $<
basic$(EXEEXT):	basic.o ../runtime/libsandboxrt.a
	$(MOX_GCC) -Wall -O2 -o $@ basic.o

exit0_SOURCES = exit0.c

exit0.o:	exit0.c
	$(MOX_GCC) -I$(top_srcdir)/runtime -Wall -O2 -mel -static -nostdlib -c $<
exit0$(EXEEXT):	exit0.o ../runtime/libsandboxrt.a
	$(MOX_GCC) -Wall -O2 -mel -static -nostdlib -o $@ exit0.o ../runtime/libsandboxrt.a

exit1_SOURCES = exit1.c

exit1.o:	exit1.c
	$(MOX_GCC) -I$(top_srcdir)/runtime -Wall -O2 -mel -static -nostdlib -c $<
exit1$(EXEEXT):	exit1.o ../runtime/libsandboxrt.a
	$(MOX_GCC) -Wall -O2 -mel -static -nostdlib -o $@ exit1.o ../runtime/libsandboxrt.a

