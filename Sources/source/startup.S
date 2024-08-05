// ------------------------------------------------------------
// Armv8-A MPCore EL3 AArch64 Startup Code
//
// Basic Vectors, MMU, caches and GICv2 initialization in EL3 AArch64
//
// Exits in EL3 AArch64
//
// Copyright (c) 2014-2017 Arm Limited (or its affiliates). All rights reserved.
// Use, modification and redistribution of this file is subject to your possession of a
// valid End User License Agreement for the Arm Product of which these examples are part of
// and your compliance with all applicable terms and conditions of such licence agreement.
// ------------------------------------------------------------

#include "v8_mmu.h"
#include "v8_system.h"

#define COUNTER_FREQUENCY  250000000



	.section .StartUp, "ax"
	.balign 4


	.global el1_vectors
	.global el2_vectors
	.global el3_vectors

	.global InvalidateUDCaches
	.global ZeroBlock

	.global SetPrivateIntSecurityBlock
	.global SetSPISecurityAll
	.global SetPrivateIntPriority

	.global SetIrqGroup
	.global SetBlockGroup
	.global SetSPIGroup
	.global EnableIRQ
	.global TestIRQ
	.global EnableGICD
	.global EnableGICC
	.global SetIRQPriority
	.global SetPriorityMask
	.global ClearSGI
	.global EnableSecureFIQ
	.global DisableCachesEL1

	.global bootblock_main

	.global __ttb0_l1
	.global __el3_stack
	.global __stack

// ------------------------------------------------------------

	.global start64
	.type start64, "function"
start64:



	// bl  ZeroRegs

	//
	// program the VBARs
	//
	ldr x1, =el1_vectors
	msr VBAR_EL1, x1

	ldr x1, =el2_vectors
	msr VBAR_EL2, x1

	ldr x1, =el3_vectors
	msr VBAR_EL3, x1

	MOV X0, XZR
	MOV X1, XZR
	MOV X2, XZR
	MOV X3, XZR
	MOV X4, XZR
	MOV X5, XZR
	MOV X6, XZR
	MOV X7, XZR
	MOV X8, XZR
	MOV X9, XZR
	MOV X10, XZR
	MOV X11, XZR
	MOV X12, XZR
	MOV X13, XZR
	MOV X14, XZR
	MOV X15, XZR
	MOV X16, XZR
	MOV X17, XZR
	MOV X18, XZR
	MOV X19, XZR
	MOV X20, XZR
	MOV X21, XZR
	MOV X22, XZR
	MOV X23, XZR
	MOV X24, XZR
	MOV X25, XZR
	MOV X26, XZR
	MOV X27, XZR
	MOV X28, XZR
	MOV X29, XZR
	MOV X30, XZR

	// Set LR to a known address.
	ldr x0, =start64
	msr elr_el3, x0
	msr elr_el2, x0
	msr elr_el1, x0


	// clear FPU\NEON regs:
	MSR  CPTR_EL3, XZR
	MSR  CPTR_EL2, XZR
	FMOV D0, XZR
	FMOV D1, XZR
	FMOV D2, XZR
	FMOV D3, XZR
	FMOV D4, XZR
	FMOV D5, XZR
	FMOV D6, XZR
	FMOV D7, XZR
	FMOV D8, XZR
	FMOV D9, XZR
	FMOV D10, XZR
	FMOV D11, XZR
	FMOV D12, XZR
	FMOV D13, XZR
	FMOV D14, XZR
	FMOV D15, XZR
	FMOV D16, XZR
	FMOV D17, XZR
	FMOV D18, XZR
	FMOV D19, XZR
	FMOV D20, XZR
	//
	//	Route aborts, FIQ and IRQ to EL3
	//
	mov x3, #(SCR_EL3_EA | SCR_EL3_FIQ | SCR_EL3_IRQ | SCR_EL3_RW)
	msr SCR_EL3, x3

	// extract the core number from MPIDR_EL1 and store it in
	// x19 (defined by the AAPCS as callee-saved), so we can re-use
	// the number later
	//
	bl GetCPUID
	mov x19, x0

	//
	// Don't trap SIMD, floating point or accesses to CPACR
	//
	msr CPTR_EL3, xzr

	//
	// SCTLR_ELx may come out of reset with UNKNOWN values so we will
	// set the fields to 0 except, possibly, the endianess field(s).
	//
#ifdef __ARM_BIG_ENDIAN
	mov x0, #(SCTLR_ELx_EE | SCTLR_EL1_E0E)
#else
	mov x0, #0
#endif
	msr SCTLR_EL3, x0


	//
	// Configure ACTLR_EL[3]
	// ----------------------
	//
	// Controls acces to auxliary registers in lower levels
	//
	msr ACTLR_EL3, xzr

	//
	// configure CPUECTLR_EL1
	//
	// These bits are IMP DEF, so need to different for different
	// processors
	//
	// SMPEN - bit 6 - Enables the processor to receive cache
	//                 and TLB maintenance operations
	//
	// Note: For Cortex-A57/53 SMPEN should be set before enabling
	//       the caches and MMU, or performing any cache and TLB
	//       maintenance operations.
	//
	//       This register has a defined reset value, so we use a
	//       read-modify-write sequence to set SMPEN
	//
	mrs x0, S3_1_c15_c2_1  // Read EL1 CPU Extended Control Register
	orr x0, x0, #(1 << 6)  // Set the SMPEN bit
	msr S3_1_c15_c2_1, x0  // Write EL1 CPU Extended Control Register

	isb


	//
	// That is the last of the control settings for now
	//
	// Note: no ISB after all these changes, as registers wont be
	// accessed until after an exception return, which is itself a
	// context synchronisation event
	//

	//
	// Setup some EL3 stack space, ready for calling some subroutines, below.
	//
	// Stack space allocation is CPU-specific, so use CPU
	// number already held in x19
	//
	// 2^13 bytes per CPU for the EL3 stacks
	//
	mov x0, #0
	ldr w0, =__el3_stack
	sub w0, w0, w19, lsl #13
	mov sp, x0
	MSR sp_el2, x0
	MSR sp_el1, x0
	MSR sp_el0, x0
	

	//
	// SGI #15 is assigned to group1 - non secure interruprs
	//
	mov w0, #15
	mov w1, #1
	bl SetIrqGroup

	//
	// While we are in the Secure World, set the priority mask low enough
	// for it to be writable in the Non-Secure World
	//
	mov w0, #0x1F
	bl  SetPriorityMask

	//
	// Enable floating point
	//
	mov x0, #CPACR_EL1_FPEN
	msr CPACR_EL1, x0

	// Invalidate caches and TLBs for all stage 1
	// translations used at EL1
	//
	// Cortex-A processors automatically invalidate their caches on reset
	// (unless suppressed with the DBGL1RSTDISABLE or L2RSTDISABLE pins).
	// It is therefore not necessary for software to invalidate the caches
	// on startup, however, this is done here in case of a warm reset.
	bl  InvalidateUDCaches
	tlbi VMALLE1


	//
	// Set TTBR0 Base address
	//
	// The CPUs share one set of translation tables that are
	// generated by CPU0 at run-time
	//
	//
	ldr x1, =__ttb0_l1
	msr TTBR0_EL3, x1


	//
	// Set up memory attributes
	//
	// These equate to:
	//
	// 0 -> 0b01000100 = 0x00000044 = Normal, Inner/Outer Non-Cacheable
	// 1 -> 0b11111111 = 0x0000ff00 = Normal, Inner/Outer WriteBack Read/Write Allocate
	// 2 -> 0b00000100 = 0x00040000 = Device-nGnRE
	//
	mov  x1, #0xff44
	movk x1, #4, LSL #16    // equiv to: movk x1, #0x0000000000040000
	msr MAIR_EL3, x1


	//
	// Set up TCR_EL3
	//
	// We are using only TTBR0 (EPD1 = 1), and the page table entries:
	//  - are using an 8-bit ASID from TTBR0
	//  - have a 4K granularity (TG0 = 0b00)
	//  - are outer-shareable (SH0 = 0b10)
	//  - are using Inner & Outer WBWA Normal memory ([IO]RGN0 = 0b01)
	//  -  added : PS 1 36 bits address space
	//  - map
	//      + 36 bits of VA space (T0SZ = 0x20 - (region size 2^(64-32) = 2^32))
	//      + into a 32-bit PA space (PS = 0b00)
	//
	//     28   24   20   16   12    8    4    0
	//  +----+----+----+----+----+----+----+----+
	//  |    |    |    |    |    |OOII|    |    |
	//  |R   |   R|R R |R   |    |RRRR| RT |   T|
	//  |E   |   E|E ET|E   |TTSS|GGGG| E0 |   0|
	//  |S   |   S|S SB|S  P|GGHH|NNNN| SS |   S|
	//  |1---|---0|1-0I|0--S|0000|0000|-0Z-|---Z|
	//
	//   0000 0000 0000 0001 0010 0101 0001 1100
	//
	// 0x  0    0    0    0    2    5    2    0
	//
	ldr x1, =0x2520
	msr TCR_EL3, x1
	isb


//  Initialize CNTFRQ 
	ldr	x11, =COUNTER_FREQUENCY
	msr	cntfrq_el0, x11

	//
	// x19 already contains the CPU number, so branch to secondary
	// code if we _re not on CPU0
	//

	cbnz x19, el3_secondary

	//
	// Fall through to primary code
	//


//
// ------------------------------------------------------------
//
// EL3 - primary CPU init code
//
// This code is run on CPU0, while the other CPUs are in the
// holding pen
//



	.global el3_primary
	.type el3_primary, "function"
el3_primary:

	// Disable interrupts. not using in BootBlock:
//			bl DisableGICD

	//
	// We _re now on the primary processor, so turn GIC distributor
	// and CPU interface
	bl  EnableGICD

	//
	// Enabling secure FIQ will generate FIQ for Group 0 interrupts; otherwise IRQ will be generated
	//
	// bl 	EnableSecureFIQ

	//
	// Enable GIC CPU interface
	//
	bl  EnableGICC

	//
	// Enable the MMU
	//
		//	mrs x1, SCTLR_EL3
		//	orr x1, x1, #SCTLR_ELx_M
		//	bic x1, x1, #SCTLR_ELx_A // Disable alignment fault checking.  To enable, change bic to orr
		//	orr x1, x1, #SCTLR_ELx_C
		//	orr x1, x1, #SCTLR_ELx_I
		//	msr SCTLR_EL3, x1
		//	isb

	// Zero the bss
	ldr x0, =__bss_start__ // Start of block
	ldr x2, =__bss_end__   // End of block
	sub x1, x2, x0         // Length of block
	bl  ZeroBlock

	// bl  DisableCachesEL1

	// Branch to core0 main funtion
	b bootblock_main







// ------------------------------------------------------------
// EL3 - secondary CPU init code
//
// This code is run on CPUs 1, 2, 3 etc....
// ------------------------------------------------------------

	.section .StartUp_secondary, "ax"
	.global el3_secondary
	.type el3_secondary, "function"
el3_secondary:



#ifdef WAKE_ON_SGI
	//
	// the primary CPU is going to use SGI 15 as a wakeup event
	// to let us know when it is OK to proceed, so prepare for
	// receiving that interrupt
	//
	// NS interrupt priorities run from 0 to 15, with 15 being
	// too low a priority to ever raise an interrupt, so let _s
	// use 14
	//
	mov w0, #15
	mov w1, #(14 << 1) // we _re in NS world, so adjustment is needed
	bl  SetIRQPriority
	
	mov w0, #15
	bl  EnableIRQ
	
	mov w0, #(15 << 1)
	bl  SetPriorityMask

#endif

	//
	// Enabling secure FIQ will generate FIQ for Group 0 interrupts; otherwise IRQ will be generated
	//
	//bl	EnableSecureFIQ

	//
	// Enable GIC CPU interface
	//
	bl  EnableGICC
	
	//
	// wait for our interrupt to arrive
	//





	msr   cptr_el3, xzr		/* Disable coprocessor traps to EL3 */
	mov   x6, #CPTR_EL2_RES1
	msr   cptr_el2, x6		/* Disable coprocessor traps to EL2 */

	/* Initialize Generic Timers */
	msr   cntvoff_el2, xzr

	/* Initialize SCTLR_EL2
	 *
	 * setting RES1 bits (29,28,23,22,18,16,11,5,4) to 1
	 * and RES0 bits (31,30,27,26,24,21,20,17,15-13,10-6) +
	 * EE,WXN,I,SA,C,A,M to 0
	 */
	ldr	x6, =(SCTLR_EL2_RES1 | SCTLR_EL2_EE_LE |\
			SCTLR_EL2_WXN_DIS | SCTLR_EL2_ICACHE_DIS |\
			SCTLR_EL2_SA_DIS | SCTLR_EL2_DCACHE_DIS |\
			SCTLR_EL2_ALIGN_DIS | SCTLR_EL2_MMU_DIS)
	msr	sctlr_el2, x6

	mov	x6, sp
	msr	sp_el2, x6		/* Migrate SP */
	mrs	x6, vbar_el3
	msr	vbar_el2, x6		/* Migrate VBAR */

	/*
	 * The next lower exception level is AArch64, 64bit EL2 | HCE |
	 * RES1 (Bits[5:4]) | Non-secure EL0/EL1.
	 * and the SMD depends on requirements.
	 */
	ldr	x6, =(SCR_EL3_RW_AARCH64 | SCR_EL3_HCE_EN |\
			SCR_EL3_SMD_DIS | SCR_EL3_RES1 | SCR_EL3_FIQ_EN | SCR_EL3_IRQ_EN |\
			SCR_EL3_NS_EN)
	msr	scr_el3, x6

	/* Return to the EL2_SP2 mode from EL3 */
	ldr	x6, =(SPSR_EL_DEBUG_MASK | SPSR_EL_SERR_MASK |\
			SPSR_EL_M_AARCH64 )

	msr	spsr_el3, x6

	/* read SCRPAD10 (address 0xF0800E00, SCRPAD_10_41(cpu_number*2)) register. If not zero, core will branch to this address */	
	mov   x1, x19
	mov   x2, #8
	mul   x1, x1, x2
	add   x1, x1, #0x8
	mov   x7, #0xE00
	movk  x7, #0xF080, lsl #0x10
	add   x7, x1, x7

	/* clear SCRPAD first */
	mov   x0, #0
	str   x0, [x7]
	
wait_loop_wfe:

	dsb SY      // Clear all pending data accesses

	// wfe

	ldr   w8, [x7]
	cbz   w8, wait_loop_wfe

    blr	  x8


	// should not get here:
    b wait_loop_wfe

	


	.end section StartUp









