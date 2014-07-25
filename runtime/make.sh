#!/bin/sh -x

moxie-elf-as -EL -o crt0.o crt0.S
moxie-elf-as -EL -o sys-exit.o sys-exit.S
moxie-elf-as -EL -o sys-mmap.o sys-mmap.S
moxie-elf-gcc -Wall -O -mel -static -nostdlib -c memmap.c
moxie-elf-ar rvs --target=elf32-littlemoxie libsandboxrt.a \
	crt0.o \
	memmap.o \
	sys-exit.o \
	sys-mmap.o

moxie-elf-gcc -Wall -O -mel -static -nostdlib -c test1.c
moxie-elf-gcc -Wall -O -mel -static -nostdlib -o test1 test1.o libsandboxrt.a

