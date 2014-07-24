
This is a sandbox, in the spirit of Cloudius Systems' OSv.

The goal is to provide a secure, sandboxed execution mechanism that
enables deterministic input, processing and output.  Execution is
separated out into distinct phases:

1. Prepare and load hash-sealed program executables, data.
2. Execute program as a black box, with no I/O capability.
   Runs until exit or CPU budget exhausted.
3. Gather processed data, if any.

A single thread of execution pre-loads necessary data, runs a 32-bit
little endian Moxie ELF binary.  This program is built using the
"moxie-elf" target in upstream binutils and gcc.  You will need to build
and install moxie binutils+gcc cross-compiler toolset.
```
	git clone git://sourceware.org/git/binutils-gdb.git
	git clone git://gcc.gnu.org/git/gcc.git
```
From the Moxie program's point of view, it is running as root and is
essentially the entire operating system kernel + application, all in
a single wrapper.

From the sandbox's point of view, the application is running as an
unpriv'd application with only the ability to access data within the
hand-built memory map.

More info about the Moxie architecture may be found at
http://moxielogic.org/wiki/index.php/Main_Page and
http://moxielogic.org/blog/  The main code repository is
https://github.com/atgreen/moxie-cores.

