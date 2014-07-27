
noinst_PROGRAMS = sandbox

SUBDIRS = runtime

sandbox_SOURCES = \
	elf.cc \
	machine.cc \
	moxie.h \
	moxie.cc \
	sandbox.cc \
	sandbox.h \
	util.cc
sandbox_LDADD = $(ELF_LIBS) $(CRYPTO_LIBS)

