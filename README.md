
This is a sandbox, in the spirit of Cloudius Systems' OSv.

The goal is to provide a secure, sandboxed execution mechanism that
enables deterministic input, processing and output.  Execution is
separated out into distinct phases:

1. Prepare and load hash-sealed program executables, data.
2. Execute program as a black box, with no I/O capability.
   Runs until exit or CPU budget exhausted (or CPU exception).
3. Gather processed data, if any.

A single thread of execution pre-loads necessary data, then simulates a
32-bit little endian Moxie CPU, running the loaded code.

This program is built using the "moxiebox" target in upstream binutils
and gcc.  A reduced (C-only) gcc toolchain is therefore available for
immediate use by developers.

You will need to build and install moxie binutils+gcc cross-compiler
toolset:

```bash
    cd contrib
    ./download-tools-sources.sh
    ./build-moxiebox-tools.sh
```

From the Moxie program's point of view, it is a single thread running
as root and is essentially the entire operating system kernel +
application, all in a single wrapper.

From the sandbox's point of view, the application is running as an
unpriv'd application with only the ability to access data within the
hand-built memory map.

More info about the Moxie architecture may be found at
http://moxielogic.org/blog/pages/architecture.html and
http://moxielogic.org/blog/ . The main code repository is
https://github.com/atgreen/moxie-cores.

Example usage:

	./sandbox \
		-e runtime/test1 \
		-d mydata.json \
		-d mydata2.dat \
		-o file.out

If you specify the -g <port> option, then sandbox will wait for a GDB
connection on the given port.  For example, run sandbox like so:

    $ ./sandbox -e ./basic -g 9999
    ep 00001000
    ro 00000f6c-00001536 elf0
    ro 00001538-00001540 elf1
    rw 00001640-00001ab4 elf2
    rw 00002ab4-00012ab4 stack
    ro 00013ab4-00013b74 mapdesc
    
And, in a separate console, run GDB to connect to sandbox using the
`target remote` command like so:

    $ moxiebox-gdb basic
    GNU gdb (GDB) 7.8.50.20141227-cvs
    Copyright (C) 2014 Free Software Foundation, Inc.
    License GPLv3+: GNU GPL version 3 or later http://gnu.org/licenses/gpl.html
    This is free software: you are free to change and redistribute it.
    There is NO WARRANTY, to the extent permitted by law. Type "show copying"
    and "show warranty" for details.
    This GDB was configured as "--host=x86_64-unknown-linux-gnu --target=moxie-elf".
    Type "show configuration" for configuration details.
    For bug reporting instructions, please see:
    http://www.gnu.org/software/gdb/bugs/.
    Find the GDB manual and other documentation resources online at:
    http://www.gnu.org/software/gdb/documentation/.
    For help, type "help".
    Type "apropos word" to search for commands related to "word"...
    Reading symbols from basic...done.
    (gdb) target remote localhost:9999
    Remote debugging using localhost:9999
    0x00001000 in __start ()
    (gdb) b main
    Breakpoint 1 at 0x14c8
    (gdb) c
    Continuing.
    
    Breakpoint 1, 0x000014c8 in main ()
    (gdb) x/4i $pc
    => 0x14c8 : xor $r5, $r5
    0x14ca : mov $r4, $r5
    0x14cc : lda.l $r3, 0x1644
    0x14d2 : lda.l $r2, 0x1648
    (gdb)
