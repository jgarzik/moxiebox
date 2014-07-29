
noinst_PROGRAMS = sandbox gen-sum

noinst_LIBRARIES = libsandbox.a

libsandbox_a_SOURCES = \
	sandbox.h \
	util.cc

sandbox_SOURCES = \
	elf.cc \
	machine.cc \
	moxie.h \
	moxie.cc \
	sandbox.cc
sandbox_LDADD = libsandbox.a $(ELF_LIBS) $(CRYPTO_LIBS)

gen_sum_SOURCES = gen-sum.cc
gen_sum_LDADD = libsandbox.a $(CRYPTO_LIBS)

