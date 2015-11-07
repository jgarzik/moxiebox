
sandbox execution environment
=============================

sandbox provides a "bare metal" execution environment similar to that
found in an operating system kernel.  This is a highly specialized
environment.  Applications will not find any of the normal POSIX APIs
available.


Phase 1: address space preparation (code + data)
----------------------------------------------------------------

* An empty 32-bit virtual address space is initialized.
* One or more 32-bit little endian ELF Moxie program binaries are
  loaded into the address space, at virtual addresses specified in ELF.
  This memory is addressible, read-only.
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
pointer (virtual address) to this buffer is stored in special
register #6.  The length of the buffer is stored in special register #7.

The setreturn() function handles this task.


ABI Summary
-----------
* 32 bit, little endian Moxie CPU (simulated)
* Linux-ELF executable code
* Zero or more data files, placed into the program's memory map.
* Runtime environment:
	* moxie_memmap - Global variable, pointer to list of
	  struct moxie_memory_map_ent, which describes the
	  execution environment's input data.
	* setreturn(3) - Pointer to environment's output data buffer.
	  This is the data returned from the sandbox to the user.
	* stdlib.h: abort(3), exit(3)
	* string.h: memchr(3), memcmp(3), memcpy(3), memset(3)
	* string.h: strchr(3), strcmp(3), strcpy(3), strlen(3), strncpy(3), strcpy(3), strstr(3)
* Runtime environment - crypto:
	* sha256: sha256_init(), sha256_update(), sha256_final()
* System calls:
	* mmap(2) - MAP_PRIVATE|MAP_ANONYMOUS to allocate heap memory.
	* _exit(2) - End process

