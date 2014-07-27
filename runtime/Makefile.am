
noinst_LIBRARIES = libsandboxrt.a
noinst_PROGRAMS = test1

libsandboxrt_a_SOURCES = \
	crt0.S \
	setreturn.S \
	sys-_exit.S \
	sys-mmap.S \
	memmap.c \
	sandboxrt.h

test1_SOURCES = test1.c

MOX_LIB_OBJS = \
	crt0.o \
	memmap.o \
	setreturn.o \
	sys-_exit.o \
	sys-mmap.o

libsandboxrt.a: $(MOX_LIB_OBJS)
	$(MOX_AR) rvs --target=elf32-littlemoxie libsandboxrt.a \
		$(MOX_LIB_OBJS)

crt0.o:	crt0.S
	$(MOX_AS) -EL -o $@ $<
setreturn.o:	setreturn.S
	$(MOX_AS) -EL -o $@ $<
sys-_exit.o:	sys-_exit.S
	$(MOX_AS) -EL -o $@ $<
sys-mmap.o:	sys-mmap.S
	$(MOX_AS) -EL -o $@ $<

memmap.o:	memmap.c
	$(MOX_GCC) -Wall -O2 -mel -static -nostdlib -c $<
test1.o:	test1.c
	$(MOX_GCC) -Wall -O2 -mel -static -nostdlib -c $<
test1$(EXEEXT):	test1.o libsandboxrt.a
	$(MOX_GCC) -Wall -O2 -mel -static -nostdlib -o $@ test1.o libsandboxrt.a

