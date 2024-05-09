//
// Defines for v8 System Registers
//
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
// Use, modification and redistribution of this file is subject to your possession of a
// valid End User License Agreement for the Arm Product of which these examples are part of 
// and your compliance with all applicable terms and conditions of such licence agreement.
//

#ifndef V8_SYSTEM_H
#define V8_SYSTEM_H

//
// AArch64 SPSR
//
#define AARCH64_SPSR_EL3h 0b1101
#define AARCH64_SPSR_EL3t 0b1100
#define AARCH64_SPSR_EL2h 0b1001
#define AARCH64_SPSR_EL2t 0b1000
#define AARCH64_SPSR_EL1h 0b0101
#define AARCH64_SPSR_EL1t 0b0100
#define AARCH64_SPSR_EL0t 0b0000
#define AARCH64_SPSR_RW (1 << 4)
#define AARCH64_SPSR_F  (1 << 6)
#define AARCH64_SPSR_I  (1 << 7)
#define AARCH64_SPSR_A  (1 << 8)
#define AARCH64_SPSR_D  (1 << 9)
#define AARCH64_SPSR_IL (1 << 20)
#define AARCH64_SPSR_SS (1 << 21)
#define AARCH64_SPSR_V  (1 << 28)
#define AARCH64_SPSR_C  (1 << 29)
#define AARCH64_SPSR_Z  (1 << 30)
#define AARCH64_SPSR_N  (1 << 31)

//
// Multiprocessor Affinity Register
//
#define MPIDR_EL1_AFF3_LSB 32
#define MPIDR_EL1_U  (1 << 30)
#define MPIDR_EL1_MT (1 << 24)
#define MPIDR_EL1_AFF2_LSB 16
#define MPIDR_EL1_AFF1_LSB  8
#define MPIDR_EL1_AFF0_LSB  0
#define MPIDR_EL1_AFF_WIDTH 8

//
// Data Cache Zero ID Register
//
#define DCZID_EL0_BS_LSB   0
#define DCZID_EL0_BS_WIDTH 4
#define DCZID_EL0_DZP_LSB  5
#define DCZID_EL0_DZP (1 << 5)

//
// System Control Register
//
#define SCTLR_EL1_UCI     (1 << 26)  // Enables EL0 access to the DC CVAU, DC CIVAC, DC CVAC and IC IVAU instructions in the AArch64 Execution state.
#define SCTLR_ELx_EE      (1 << 25)  // Exception endianness. 
#define SCTLR_EL1_E0E     (1 << 24)  // Endianness of explicit data access at EL0.
#define SCTLR_ELx_WXN     (1 << 19)  // Write permission implies Execute Never (XN).
#define SCTLR_EL1_nTWE    (1 << 18)  // WFE non-trapping.
#define SCTLR_EL1_nTWI    (1 << 16)  // WFI non-trapping.
#define SCTLR_EL1_UCT     (1 << 15)  // Enables EL0 access to the CTR_EL0 register in AArch64 Execution state
#define SCTLR_EL1_DZE     (1 << 14)  // Enables access to the DC ZVA instruction at EL0. 
#define SCTLR_ELx_I       (1 << 12)  // Instruction cache enable. 
#define SCTLR_EL1_UMA     (1 << 9)   // User Mask Access. Controls access to interrupt masks from EL0, when EL0 is using AArch64.
#define SCTLR_EL1_SED     (1 << 8)   // SETEND instruction disable
#define SCTLR_EL1_ITD     (1 << 7)   // IT instruction disable. 
#define SCTLR_EL1_THEE    (1 << 6) 
#define SCTLR_EL1_CP15BEN (1 << 5)  // CP15 barrier enable
#define SCTLR_EL1_SA0     (1 << 4)  // Enable EL0 stack alignment check. 
#define SCTLR_ELx_SA      (1 << 3)  // Enable SP alignment check
#define SCTLR_ELx_C       (1 << 2)  // cache
#define SCTLR_ELx_A       (1 << 1)  // allignment check
#define SCTLR_ELx_M       (1 << 0)  // MMU

//
// Architectural Feature Access Control Register
//
#define CPACR_EL1_TTA     (1 << 28)
#define CPACR_EL1_FPEN    (3 << 20)

//
// Architectural Feature Trap Register
//
#define CPTR_ELx_TCPAC (1 << 31)
#define CPTR_ELx_TTA   (1 << 20)
#define CPTR_ELx_TFP   (1 << 10)

//
// Secure Configuration Register
//
#define SCR_EL3_TWE  (1 << 13)
#define SCR_EL3_TWI  (1 << 12)
#define SCR_EL3_ST   (1 << 11)
#define SCR_EL3_RW   (1 << 10)
#define SCR_EL3_SIF  (1 << 9)
#define SCR_EL3_HCE  (1 << 8)
#define SCR_EL3_SMD  (1 << 7)
#define SCR_EL3_EA   (1 << 3)
#define SCR_EL3_FIQ  (1 << 2)
#define SCR_EL3_IRQ  (1 << 1)
#define SCR_EL3_NS   (1 << 0)

//
// Hypervisor Configuration Register
//
#define HCR_EL2_ID   (1 << 33)
#define HCR_EL2_CD   (1 << 32)
#define HCR_EL2_RW   (1 << 31)
#define HCR_EL2_TRVM (1 << 30)
#define HCR_EL2_HVC  (1 << 29)
#define HCR_EL2_TDZ  (1 << 28)

//
// Reset Management register
//
#define RMR_EL3_RR		(1 << 1)
#define RMR_EL3_AA64	(1 << 0)

// L2 Auxillary 
#define L2ACTLR_EL1_L2DEIEN		(1 << 29)


#define CPTR_EL2_RES1		(3 << 12 | 0x3ff)           /* Reserved, RES1 */

/*
 * SCTLR_EL2 bits definitions
 */
#define SCTLR_EL2_RES1		(3 << 28 | 3 << 22 | 1 << 18 | 1 << 16 | 1 << 11 | 3 << 4)	    /* Reserved, RES1 */
#define SCTLR_EL2_EE_LE		(0 << 25) /* Exception Little-endian          */
#define SCTLR_EL2_WXN_DIS	(0 << 19) /* Write permission is not XN       */
#define SCTLR_EL2_ICACHE_DIS	(0 << 12) /* Instruction cache disabled       */
#define SCTLR_EL2_SA_DIS	(0 << 3)  /* Stack Alignment Check disabled   */
#define SCTLR_EL2_DCACHE_DIS	(0 << 2)  /* Data cache disabled              */
#define SCTLR_EL2_ALIGN_DIS	(0 << 1)  /* Alignment check disabled         */
#define SCTLR_EL2_MMU_DIS	(0)       /* MMU disabled                     */

/*
 * SCR_EL3 bits definitions
 */
#define SCR_EL3_RW_AARCH64	(1 << 10) /* Next lower level is AArch64     */
#define SCR_EL3_RW_AARCH32	(0 << 10) /* Lower lowers level are AArch32  */
#define SCR_EL3_HCE_EN		(1 << 8)  /* Hypervisor Call enable          */
#define SCR_EL3_SMD_DIS		(1 << 7)  /* Secure Monitor Call disable     */
#define SCR_EL3_RES1		(3 << 4)  /* Reserved, RES1                  */
#define SCR_EL3_EA_EN		(1 << 3)  /* External aborts taken to EL3    */
#define SCR_EL3_FIQ_EN		(1 << 2)  /* FIQ are taken by EL3    */
#define SCR_EL3_IRQ_EN		(1 << 1)  /* IRQ are taken by EL3     */
#define SCR_EL3_NS_EN		(1 << 0)  /* EL0 and EL1 in Non-scure state  */

/*
 * SPSR_EL3/SPSR_EL2 bits definitions
 */
#define SPSR_EL_END_LE		(0 << 9)  /* Exception Little-endian          */
#define SPSR_EL_DEBUG_MASK	(1 << 9)  /* Debug exception masked           */
#define SPSR_EL_ASYN_MASK	(1 << 8)  /* Asynchronous data abort masked   */
#define SPSR_EL_SERR_MASK	(1 << 8)  /* System Error exception masked    */
#define SPSR_EL_IRQ_MASK	(1 << 7)  /* IRQ exception masked             */
#define SPSR_EL_FIQ_MASK	(1 << 6)  /* FIQ exception masked             */
#define SPSR_EL_T_A32		(0 << 5)  /* AArch32 instruction set A32      */
#define SPSR_EL_M_AARCH64	(0 << 4)  /* Exception taken from AArch64     */
#define SPSR_EL_M_AARCH32	(1 << 4)  /* Exception taken from AArch32     */
#define SPSR_EL_M_SVC		(0x3)     /* Exception taken from SVC mode    */
#define SPSR_EL_M_HYP		(0xa)     /* Exception taken from HYP mode    */
#define SPSR_EL_M_EL1H		(5)       /* Exception taken from EL1h mode   */
#define SPSR_EL_M_EL2H		(9)       /* Exception taken from EL2h mode   */


// use this list to access system registers with MSR commands:

// S3_0_C0_C0_0 - Main ID Register
// S3_0_C0_C0_1 - Cache Type Register
// S3_0_C0_C0_2 - TCM Type Register
// S3_0_C0_C1_0 - Control Register
// S3_0_C0_C1_1 - Auxiliary Control Register
// S3_0_C0_C1_2 - Secure Configuration Register
// S3_0_C0_C1_3 - Secure Debug Configuration Register
// S3_0_C0_C2_0 - Translation Control Register
// S3_0_C0_C2_1 - Translation Table Base Register 0
// S3_0_C0_C2_2 - Translation Table Base Register 1
// S3_0_C0_C2_3 - Translation Table Base Register 2
// S3_0_C0_C2_4 - Translation Table Base Register 3
// S3_0_C0_C3_0 - Domain Access Control Register
// S3_0_C0_C3_1 - Fault Address Register
// S3_0_C0_C4_0 - TLB Type Register
// S3_0_C0_C5_0 - Cache Size ID Register
// S3_0_C0_C5_1 - Cache Size Selection Register
// S3_0_C0_C6_0 - Instruction Set Attributes Register
// S3_0_C0_C6_1 - Memory Model Feature Register
// S3_0_C0_C6_2 - Debug Feature Register
// S3_0_C0_C6_3 - Auxiliary Feature Register
// S3_0_C0_C7_0 - Cache Level ID Register
// S3_0_C0_C7_1 - Cache Size ID Register
// S3_0_C0_C7_2 - Cache Type Register
// S3_0_C0_C7_3 - Cache Level Type Register
// S3_0_C0_C8_0 - TLB Lockdown Register
// S3_0_C0_C9_0 - TLB Lockdown Register 2
// S3_0_C0_C10_0 - TLB Lockdown Register 3
// S3_0_C0_C11_0 - TLB Lockdown Register 4
// S3_0_C0_C12_0 - Process ID Register
// S3_0_C0_C12_1 - Context ID Register
// S3_0_C0_C12_2 - Virtual Process ID Register
// S3_0_C0_C13_0 - Virtual Machine ID Register
// S3_0_C0_C13_1 - Virtual Machine Control Register
// S3_0_C0_C13_2 - Virtual Machine Feature Register
// S3_0_C1_C0_0 - Secure Configuration Register
// S3_0_C1_C0_1 - Secure Debug Configuration Register
// S3_0_C1_C0_2 - Virtualization Control Register
// S3_0_C1_C0_3 - Virtualization Feature Register
// S3_0_C1_C1_0 - Virtualization Translation Control Register
// S3_0_C1_C1_1 - Virtualization Translation Table Base Register 0
// S3_0_C1_C1_2 - Virtualization Translation Table Base Register 1
// S3_0_C1_C1_3 - Virtualization Translation Table Base Register 2
// S3_0_C1_C1_4 - Virtualization Translation Table Base Register 3
// S3_0_C1_C2_0 - Virtualization Memory Control Register

// S3_1_C0_C0_0 - Performance Monitor Control Register
// S3_1_C0_C0_1 - Performance Monitor Count Enable Set Register
// S3_1_C0_C0_2 - Performance Monitor Event Counter Selection Register
// S3_1_C0_C0_3 - Performance Monitor Count Enable Clear Register
// S3_1_C0_C1_0 - Performance Monitor Overflow Flag Status Register
// S3_1_C0_C1_1 - Performance Monitor Software Increment Register
// S3_1_C0_C1_2 - Performance Monitor Count Register 0
// S3_1_C0_C1_3 - Performance Monitor Count Register 1
// S3_1_C0_C1_4 - Performance Monitor Count Register 2
// S3_1_C0_C1_5 - Performance Monitor Count Register 3
// S3_1_C0_C2_0 - Performance Monitor Event Type Select Register
// S3_1_C0_C3_0 - Performance Monitor Event Count Register 0
// S3_1_C0_C3_1 - Performance Monitor Event Count Register 1
// S3_1_C0_C3_2 - Performance Monitor Event Count Register 2
// S3_1_C0_C3_3 - Performance Monitor Event Count Register 3
// S3_1_C0_C4_0 - Context ID Register
// S3_1_C0_C5_0 - User Read/Write Access Register
// S3_1_C0_C5_1 - User Read/Write Access Register
// S3_1_C0_C5_2 - User Read/Write Access Register
// S3_1_C0_C5_3 - User Read/Write Access Register
// S3_1_C0_C5_4 - User Read/Write Access Register
// S3_1_C0_C5_5 - User Read/Write Access Register
// S3_1_C0_C6_0 - Hypervisor Configuration Register
// S3_1_C0_C6_1 - Hypervisor Debug Configuration Register
// S3_1_C0_C6_2 - Hypervisor Auxiliary Configuration Register
// S3_1_C0_C6_3 - Hypervisor Memory Management Feature Register
// S3_1_C0_C6_4 - Hypervisor Auxiliary Feature Register
// S3_1_C0_C7_0 - Virtualization Interrupt Control Register
// S3_1_C0_C7_1 - Virtualization Interrupt Priority Register
// S3_1_C0_C7_2 - Virtualization Interrupt Acknowledge Register
// S3_1_C0_C8_0 - Hypervisor TLB Lockdown Register
// S3_1_C0_C9_0 - Hypervisor TLB Lockdown Register 2
// S3_1_C0_C10_0 - Hypervisor TLB Lockdown Register 3
// S3_1_C0_C11_0 - Hypervisor TLB Lockdown Register 4
// S3_1_C0_C12_0 - Non-Secure Access Control Register
// S3_1_C0_C12_1 - Non-Secure Debug Configuration Register
// S3_1_C0_C12_2 - Virtualization Exception Syndrome Register
// S3_1_C0_C13_0 - Virtualization Event Register
// S3_1_C0_C13_1 - Virtualization Exception Register
// S3_1_C0_C13_2 - Virtualization Instruction Syndrome Register
// S3_1_C0_C14_0 - Hypervisor Fault Status Register
// S3_1_C0_C14_1 - Hypervisor Fault Address Register


// S3_1_C0_C15_0 - Secure Configuration Register
// S3_1_C1_C0_0 - Auxiliary Control Register
// S3_1_C1_C0_1 - Secure Debug Configuration Register
// S3_1_C1_C0_2 - Secure Auxiliary Control Register
// S3_1_C1_C0_3 - Secure Memory Protection Control Register
// S3_1_C1_C0_4 - Non-Secure Memory Protection Control Register
// S3_1_C1_C1_0 - Secure Monitor Call Register
// S3_1_C1_C2_0 - Secure Context ID Register
// S3_1_C1_C3_0 - Secure User Read/Write Access Register
// S3_1_C1_C4_0 - Secure Configuration Register 2
// S3_1_C1_C6_0 - Hypervisor System Trap Register
// S3_1_C1_C7_0 - Hypervisor Debug Trap Register
// S3_1_C1_C8_0 - Hypervisor Fault Handling Register
// S3_1_C1_C9_0 - Hypervisor System Control Register
// S3_1_C1_C10_0 - Hypervisor Configuration Base Register
// S3_1_C1_C11_0 - Hypervisor Auxiliary Trap Handling Register
// S3_1_C1_C12_0 - Secure Debug Exception Catch Control Register
// S3_1_C1_C12_1 - Secure Debug Exception Catch Address Register
// S3_1_C1_C12_2 - Secure Debug Exception Catch Data Register
// S3_1_C1_C13_0 - Secure Debug Control Register
// S3_1_C1_C13_1 - Secure Debug Access Control Register
// S3_1_C1_C13_2 - Secure Debug Authentication Register
// S3_1_C1_C14_0 - Monitor Vector Base Address Register
// S3_1_C1_C15_0 - Interrupt Configuration Register

// S3_1_C7_C10_4 - Clean data cache to the Point of Persistence by VA to the Point of Coherency by VA
// S3_1_C7_C10_6 - Clean and invalidate data cache to the Point of Persistence by VA to the Point of Coherency by VA
// S3_1_C7_C14_1 - Invalidate instruction cache to the Point of Coherency by VA
// S3_1_C7_C10_5 - Invalidate data cache to the Point of Coherency by VA
// S3_1_C7_C5_0 - Data Synchronization Barrier Operation
// S3_1_C7_C10_1 - Invalidate data cache by VA to the Point of Coherency by VA
// S3_1_C7_C11_1 - Invalidate data cache to the Point of Persistence by set/way
// S3_1_C7_C11_2 - Clean data cache to the Point of Persistence by set/way

#endif // V8_SYSTEM_H


