
noinst_LIBRARIES = libsandboxrt.a

libsandboxrt_a_SOURCES = \
	crt0.S \
	setreturn.S \
	sys-_exit.S \
	sys-mmap.S \
	memchr.c \
	memcmp.c \
	memcpy.c \
	memmap.c \
	memset.c \
	strchr.c \
	strcmp.c \
	strcpy.c \
	strlen.c \
	strncpy.c \
	str-two-way.h \
	strstr.c \
	sandboxrt.h

MOX_LIB_OBJS = \
	crt0.o \
	memchr.o \
	memcmp.o \
	memcpy.o \
	memmap.o \
	memset.o \
	setreturn.o \
	strchr.o \
	strcmp.o \
	strcpy.o \
	strlen.o \
	strncpy.o \
	strstr.o \
	sys-_exit.o \
	sys-mmap.o

libsandboxrt.a: $(MOX_LIB_OBJS)
	$(MOX_AR) rvs libsandboxrt.a \
		$(MOX_LIB_OBJS)

crt0.o:	crt0.S
	$(MOX_AS) -o $@ $<
setreturn.o:	setreturn.S
	$(MOX_AS) -o $@ $<
sys-_exit.o:	sys-_exit.S
	$(MOX_AS) -o $@ $<
sys-mmap.o:	sys-mmap.S
	$(MOX_AS) -o $@ $<

memchr.o:	memchr.c
	$(MOX_GCC) -Wall -O2 -c $<
memcmp.o:	memcmp.c
	$(MOX_GCC) -Wall -O2 -c $<
memcpy.o:	memcpy.c
	$(MOX_GCC) -Wall -O2 -c $<
memmap.o:	memmap.c
	$(MOX_GCC) -Wall -O2 -c $<
memset.o:	memset.c
	$(MOX_GCC) -Wall -O2 -c $<
strchr.o:	strchr.c
	$(MOX_GCC) -Wall -O2 -c $<
strcmp.o:	strcmp.c
	$(MOX_GCC) -Wall -O2 -c $<
strcpy.o:	strcpy.c
	$(MOX_GCC) -Wall -O2 -c $<
strncpy.o:	strncpy.c
	$(MOX_GCC) -Wall -O2 -c $<
strstr.o:	strstr.c
	$(MOX_GCC) -Wall -O2 -c $<
strlen.o:	strlen.c
	$(MOX_GCC) -Wall -O2 -c $<
