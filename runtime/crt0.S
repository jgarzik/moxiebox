/* crt0.S -- startup file for moxiebox
 * 
 * Copyright (c) 2008, 2009, 2014  Anthony Green
 * Copyright 2014 BitPay, Inc.
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 */

	.globl	__start
	.weak	_start
	.text
	.type	__start,@function
__start:
_start:
	/* zero fp to allow unwinders to stop */
	xor	$fp, $fp

	/* load sim stack pointer */
	gsr	$sp, 7

	/* zero the bss area */
	ldi.l	$r0, __bss_start__
	xor	$r1, $r1
	ldi.l	$r2, __bss_end__
	sub	$r2, $r0
	jsra	memset

	/* load sim memory descriptor base address */
	gsr	$r0, 6
	sta.l	moxie_memmap, $r0

	/* Set argc and argv (empty). */
	xor	$r0, $r0
	xor	$r1, $r1

	/* Call _init to invoke static constructors, etc.  */
	jsra	_init
	
	/* Call _fini at exit time for static destructors.  */
	ldi.l	$r0, _fini
	jsra	atexit

	jsra	main
	jsra	exit

.Lend:
	.size	__start,(.Lend-__start)
