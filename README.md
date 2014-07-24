
This is a sandbox, in the spirit of Cloudius Systems' OSv.

The goal is to provide a secure, sandboxed execution mechanism that
enables deterministic input, processing and output.  Execution is
separately out into distinct phases:

1. Prepare and load hash-sealed program executables, data.
2. Execute program as a black box, with no I/O capability.
   Runs until exit or CPU budget exhausted.
3. Gather processed data, if any.

A single thread of execution pre-loads necessary data, runs a 32-bit
little endian Moxie ELF binary until it exits or a cycle budget is
exhausted.  Output, if any, is noted.

From the Moxie program's point of view, it is running as root and is
essentially the entire operating system kernel + application, all in
a single wrapper.

From the sandbox's point of view, the application is running as an
unpriv'd application with only the ability to access data within the
hand-built memory map.

