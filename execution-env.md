
sandbox execution environment
=============================

sandbox provides a "bare metal" execution environment similar to that
found in an operating system kernel.  This is a highly specialized
environment.  Applications will not find any of the normal POSIX APIs
available.


Phase 1: code and data prep
---------------------------

* An empty 32-bit virtual address space is initialized.
* One or more 32-bit little endian ELF Moxie program binaries is
  loaded into the address space, at virtual addresses specified in ELF.
  This memory addressible, read-only.
* Zero or more data files are read into the virtual address space,
  in consecutive aligned virtual addresses following the ELF binaries.

This prepared 32-bit address space is the input into the program being
executed.


Phase 2: program execution
--------------------------
Moxie simulator begins execution of the Moxie program, by jumping
to the start address specified in the ELF executable.

Execution ends when the runtime-settable CPU budget is exhausted,
or the program exits.


Phase 3: collect output
-----------------------
TBD

