#!/bin/sh -x

moxie-elf-as -EL -o crt0.o crt0.S
moxie-elf-as -EL -o sys-exit.o sys-exit.S
moxie-elf-ar rvs --target=elf32-littlemoxie libsandboxrt.a crt0.o sys-exit.o
