
sandbox execution environment
=============================

sandbox provides a "bare metal" execution environment similar to that
found in an operating system kernel.  This is a highly specialized
environment.  Applications will not find any of the normal POSIX APIs
available.


Phase 1: address space preparation (code + data)
----------------------------------------------------------------

* An empty 32-bit virtual address space is initialized.
* One or more 32-bit little endian ELF Moxie program binaries is
  loaded into the address space, at virtual addresses specified in ELF.
  This memory addressible, read-only.
* Zero or more data files are read into the virtual address space,
  in consecutive aligned virtual addresses following the ELF binaries.
* 64K stack allocated. Location stored in special register 7.
* List of memory descriptors built. Location in special register 6.

This prepared 32-bit address space is the input into the program being
executed.


Phase 2: program execution
--------------------------
Moxie simulator begins execution of the Moxie program, by jumping
to the start address specified in the ELF executable.

At program start, one of the first tasks is to walk the in-memory
moxie_memmap array to determine its input data.

To allocate heap memory, call mmap() with the MAP_ANONYMOUS flag.

Execution is single thread, single pipeline.  Execution ends when
the runtime-settable CPU budget is exhausted, or the program exits
via _exit().


Phase 3: collect output
-----------------------
The program prepares its output in a contiguous memory buffer.  The
pointer (virtual address) to this buffer is stored in special register
#6.  The length of the buffer is stored in special register #7.

The setreturn() function handles this task.

