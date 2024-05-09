//
// Simple utility routines for baremetal v8 code
//
// Copyright (c) 2013-2017 Arm Limited (or its affiliates). All rights reserved.
// Use, modification and redistribution of this file is subject to your possession of a
// valid End User License Agreement for the Arm Product of which these examples are part of 
// and your compliance with all applicable terms and conditions of such licence agreement.
//

#include "v8_system.h"

    .text
    .cfi_sections .debug_frame  // put stack frame info into .debug_frame instead of .eh_frame

//
// void *ZeroBlock(void *blockPtr, unsigned int nBytes)
//
// Zero fill a block of memory
//   Fill memory pages or similar structures with zeros.
//   The byte count must be a multiple of the block fill size (16 bytes)
//
//  Inputs:
//    blockPtr - base address of block to fill
//    nBytes - block size, in bytes
//
//  Returns:
//    pointer to just filled block, NULL if nBytes is
//  incompatible with block fill size
//
    .global ZeroBlock
    .type ZeroBlock, "function"
    .cfi_startproc
ZeroBlock:

    //
    // we fill data by steam, 16 bytes at a time: check that
    // blocksize is a multiple of that
    //
    ubfx x2, x1, #0, #4
    cbnz x2, incompatible
    
    //
    // if blocksize = 0 skip the loop and return
    //
    cbz x1, skip_loop

    //
    // we already have one register full of zeros, get another
    //
    mov x3, x2

    //
    // OK, set temporary pointer and away we go
    //
    add x0, x0, x1

loop0:
    subs x1, x1, #16
    stp  x2, x3, [x0, #-16]!
    b.ne loop0

    //
    // that's all - x0 will be back to its start value
    //
skip_loop:
    ret

    //
    // parameters are incompatible with block size - return
    // an indication that this is so
    //
incompatible:
    mov x0,#0
    ret
    .cfi_endproc


//
// void *ZeroRegs(void)
//
// Zero all the registers
//
//  Inputs:
//    None
//
//  Returns:
//    None
//
    .global ZeroRegs
    .type ZeroRegs, "function"
    .cfi_startproc
    
ZeroRegs:

	MOV X0, #0
	MOV X1, #0
	MOV X2, #0
	MOV X3, #0
	MOV X4, #0
	MOV X5, #0
	MOV X6, #0
	MOV X7, #0
	MOV X8, #0
	MOV X9, #0
	MOV X10, #0
	MOV X11, #0
	MOV X12, #0
	MOV X13, #0
	MOV X14, #0
	MOV X15, #0
	MOV X16, #0
	MOV X17, #0
	MOV X18, #0
	MOV X19, #0
	MOV X20, #0
	MOV X21, #0
	MOV X22, #0
	MOV X23, #0
	MOV X24, #0
	MOV X25, #0
	MOV X26, #0
	MOV X27, #0
	MOV X28, #0
	MOV X29, #0
	MOV X30, #0

    ret
    .cfi_endproc

