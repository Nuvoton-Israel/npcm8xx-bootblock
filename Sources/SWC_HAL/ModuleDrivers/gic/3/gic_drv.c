/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   gic_drv.c                                                                */
/* This file contains Nested Vectored Interrupt Controller (GIC) driver implementation */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                                 INCLUDES                                                */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#include __CHIP_H_FROM_DRV()

#include "gic_drv.h"
#include "gic_regs.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Module Dependencies                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                           TYPES & DEFINITIONS                                           */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Maximum number of interrupts supported by ARMv7-A GIC architecture                                      */
/*---------------------------------------------------------------------------------------------------------*/
#define GIC_MAX_NUM_OF_INTERRUPTS                 1020

/*---------------------------------------------------------------------------------------------------------*/
/* Maximum number of exceptions (traps + interrupts) supported by ARMv7-M GIC architecture                 */
/*---------------------------------------------------------------------------------------------------------*/
#define GIC_MAX_NUM_OF_EXCEPTIONS                 (GIC_MAX_NUM_OF_INTERRUPTS + GIC_TRAP_NUM)

/*---------------------------------------------------------------------------------------------------------*/
/* Number of interrupts groups. Each groups contains 32 interrupts, total of 16 groups for 496 interrupts  */
/*---------------------------------------------------------------------------------------------------------*/
#define GIC_NUM_OF_INTERRUPT_GROUPS               (GIC_MAX_NUM_OF_EXCEPTIONS / 32)

/*---------------------------------------------------------------------------------------------------------*/
/* Number of implemented exceptions (traps + interrupts)                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define GIC_NUM_OF_EXCEPTIONS                     (GIC_INTERRUPT_NUM + GIC_TRAP_NUM)

/*---------------------------------------------------------------------------------------------------------*/
/* Vrify the number of implemented exceptions is no more than the maximum number of exceptions supported   */
/*---------------------------------------------------------------------------------------------------------*/
#if (GIC_NUM_OF_EXCEPTIONS > GIC_MAX_NUM_OF_EXCEPTIONS)
#error "Unsupported number of exceptions"
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* The Vector table must be naturally aligned to a power of two whose alignment value is greater than or   */
/* equal to (Number of Exceptions supported x 4), with a minimum alignment of 128 bytes                    */
/* Quated from ARM DDI 0403D                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define GIC_VECTOR_TABLE_ALIGNMENT                ((GIC_NUM_OF_EXCEPTIONS <= (32 << 0)) ? (32 << 2) :    \
                                                   ((GIC_NUM_OF_EXCEPTIONS <= (32 << 1)) ? (32 << 3) :    \
                                                   ((GIC_NUM_OF_EXCEPTIONS <= (32 << 2)) ? (32 << 4) :    \
                                                   ((GIC_NUM_OF_EXCEPTIONS <= (32 << 3)) ? (32 << 5) :    \
                                                                                            (32 << 6)))))

/*---------------------------------------------------------------------------------------------------------*/
/* Extract Interupt Group number and input offset given the input number                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define GIC_GET_INTERRUPT_INPUT_NUM(gic_num)     (UINT8)(gic_num % 32)
#define GIC_GET_INTERRUPT_GROUP_NUM(gic_num)     (UINT8)(gic_num / 32)

/*---------------------------------------------------------------------------------------------------------*/
/* Extract Priority Group number and input offset given the input number                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define GIC_GET_PRIORITY_INPUT_NUM(gic_num)      (UINT8)(gic_num % 4)
#define GIC_GET_PRIORITY_GROUP_NUM(gic_num)      (UINT8)(gic_num / 4)

/*---------------------------------------------------------------------------------------------------------*/
/* Macro for defining TRAP routing functions                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define TRAP_ROUTER_FUNCTION(HANDLER, INT_NUM)                  \
_ISR_ void HANDLER ()                                           \
{                                                               \
    (GIC_SW_DispatchTable[INT_NUM])(INT_NUM);                  \
}

/*---------------------------------------------------------------------------------------------------------*/
/* Hardware dispach table entry type                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
typedef void (* _ISR_ INTERNAL_HANDLER_T)(void);



#ifndef EXTERNAL_DISPATCH_TABLE

/*---------------------------------------------------------------------------------------------------------*/
/* Use the gic driver internal dispatch table,                                                             */
/* otherwise assume that the table is already defined outside of this driver.                              */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                       LOCAL FUNCTIONS DECLARATION                                       */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* TRAP Handler                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************/
/*    GIC Trap List for ARM Cortex-A35                                                                      */
/***********************************************************************************************************/

/* Reserved trap -  Reserved trap 0 */
_ISR_ void GIC_TrapRouting_RES0_L (void);

/* Reset trap -  Reset trap 1 */
_ISR_ void GIC_TrapRouting_RST_L  (void);

/* Undefined Instruction trap */
_ISR_ void GIC_TrapRouting_UNDEF_L (void);

/* Software Interrupt trap */
_ISR_ void GIC_TrapRouting_SWI_L (void);

/* Prefetch Abort trap */
_ISR_ void GIC_TrapRouting_PABT_L (void);

/* Data Abort trap */
_ISR_ void GIC_TrapRouting_DABT_L (void);

/* Hypervisor Call trap */
_ISR_ void GIC_TrapRouting_HVC_L (void);

/* IRQ trap - Normal Interrupt Request */
_ISR_ void GIC_TrapRouting_IRQ_L (void);

/* FIQ trap - Fast Interrupt Request */
_ISR_ void GIC_TrapRouting_FIQ_L (void);

/* System Error trap */
_ISR_ void GIC_TrapRouting_SYSERR_L (void);

/* Spurious Interrupt trap - occurs when an interrupt is cleared but still acknowledged */
_ISR_ void GIC_TrapRouting_SPURIOUS_L (void);

/* Secure Monitor Call trap */
_ISR_ void GIC_TrapRouting_SMC_L (void);

/* External Abort trap - external memory error or external bus error */
_ISR_ void GIC_TrapRouting_EXT_ABT_L (void);

/* Virtual IRQ trap - specific to virtualization */
_ISR_ void GIC_TrapRouting_VIRQ_L (void);

/* Non-Maskable Interrupt trap */
_ISR_ void GIC_TrapRouting_NMI_L (void);

/* Debug Event trap - for handling debug exceptions */
_ISR_ void GIC_TrapRouting_DEBUG_L (void);



/*---------------------------------------------------------------------------------------------------------*/
/* DEFAULT SW HANDLER                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
static void GIC_Default_Handler_L (UINT32 int_num);


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                          CONSTANTS & VARIABLES                                          */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Dispatch Table                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
_ALIGN_(GIC_VECTOR_TABLE_ALIGNMENT, static const INTERNAL_HANDLER_T GIC_DispatchTable[GIC_TABLE_SIZE]) =
{
    /* Traps */
    /*  0 */    GIC_TrapRouting_RES0_L,
    /*  1 */    GIC_TrapRouting_RST_L ,
    /*  2 */    GIC_TrapRouting_UNDEF_L,
    /*  3 */    GIC_TrapRouting_SWI_L,
    /*  4 */    GIC_TrapRouting_PABT_L,
    /*  5 */    GIC_TrapRouting_DABT_L,
    /*  6 */    GIC_TrapRouting_HVC_L,
    /*  7 */    GIC_TrapRouting_IRQ_L,
    /*  8 */    GIC_TrapRouting_FIQ_L,
    /*  9 */    GIC_TrapRouting_SYSERR_L,
    /* 10 */    GIC_TrapRouting_SPURIOUS_L,
    /* 11 */    GIC_TrapRouting_SMC_L,
    /* 12 */    GIC_TrapRouting_EXT_ABT_L,
    /* 13 */    GIC_TrapRouting_VIRQ_L,
    /* 14 */    GIC_TrapRouting_NMI_L,
    /* 15 */    GIC_TrapRouting_DEBUG_L,
    /* Interrupts */
    /*  0 */    GIC_ISR,
    /*  1 */    GIC_ISR,
    /*  2 */    GIC_ISR,
    /*  3 */    GIC_ISR,
    /*  4 */    GIC_ISR,
    /*  5 */    GIC_ISR,
    /*  6 */    GIC_ISR,
    /*  7 */    GIC_ISR,
    /*  8 */    GIC_ISR,
    /*  9 */    GIC_ISR,
    /* 10 */    GIC_ISR,
    /* 11 */    GIC_ISR,
    /* 12 */    GIC_ISR,
    /* 13 */    GIC_ISR,
    /* 14 */    GIC_ISR,
    /* 15 */    GIC_ISR,
    /* 16 */    GIC_ISR,
    /* 17 */    GIC_ISR,
    /* 18 */    GIC_ISR,
    /* 19 */    GIC_ISR,
    /* 20 */    GIC_ISR,
    /* 21 */    GIC_ISR,
    /* 22 */    GIC_ISR,
    /* 23 */    GIC_ISR,
    /* 24 */    GIC_ISR,
    /* 25 */    GIC_ISR,
    /* 26 */    GIC_ISR,
    /* 27 */    GIC_ISR,
    /* 28 */    GIC_ISR,
    /* 29 */    GIC_ISR,
    /* 30 */    GIC_ISR,
    /* 31 */    GIC_ISR,
    /* 32 */    GIC_ISR,
    /* 33 */    GIC_ISR,
    /* 34 */    GIC_ISR,
    /* 35 */    GIC_ISR,
    /* 36 */    GIC_ISR,
    /* 37 */    GIC_ISR,
    /* 38 */    GIC_ISR,
    /* 39 */    GIC_ISR,
    /* 40 */    GIC_ISR,
    /* 41 */    GIC_ISR,
    /* 42 */    GIC_ISR,
    /* 43 */    GIC_ISR,
    /* 44 */    GIC_ISR,
    /* 45 */    GIC_ISR,
    /* 46 */    GIC_ISR,
    /* 47 */    GIC_ISR,
    /* 48 */    GIC_ISR,
    /* 49 */    GIC_ISR,
    /* 50 */    GIC_ISR,
    /* 51 */    GIC_ISR,
    /* 52 */    GIC_ISR,
    /* 53 */    GIC_ISR,
    /* 54 */    GIC_ISR,
    /* 55 */    GIC_ISR,
    /* 56 */    GIC_ISR,
    /* 57 */    GIC_ISR,
    /* 58 */    GIC_ISR,
    /* 59 */    GIC_ISR,
    /* 60 */    GIC_ISR,
    /* 61 */    GIC_ISR,
    /* 62 */    GIC_ISR,
    /* 63 */    GIC_ISR,
};

#ifndef STATIC_DISPATCH_TABLE
static HANDLER_T GIC_SW_DispatchTable[GIC_TABLE_SIZE];
#else
const static HANDLER_T GIC_SW_DispatchTable[GIC_TABLE_SIZE];
{
    /* Traps */
    /*  0 */    GIC_Default_Handler_L,
    /*  1 */    GIC_Default_Handler_L,
    /*  2 */    GIC_Default_Handler_L,
    /*  3 */    GIC_Default_Handler_L,
    /*  4 */    GIC_Default_Handler_L,
    /*  5 */    GIC_Default_Handler_L,
    /*  6 */    GIC_Default_Handler_L,
    /*  7 */    GIC_Default_Handler_L,
    /*  8 */    GIC_Default_Handler_L,
    /*  9 */    GIC_Default_Handler_L,
    /* 10 */    GIC_Default_Handler_L,
    /* 11 */    GIC_Default_Handler_L,
    /* 12 */    GIC_Default_Handler_L,
    /* 13 */    GIC_Default_Handler_L,
    /* 14 */    GIC_Default_Handler_L,
    /* 15 */    GIC_Default_Handler_L,
    /* Interrupts */
    /*  0 */    GIC_Default_Handler_L,
    /*  1 */    GIC_Default_Handler_L,
    /*  2 */    GIC_Default_Handler_L,
    /*  3 */    GIC_Default_Handler_L,
    /*  4 */    GIC_Default_Handler_L,
    /*  5 */    GIC_Default_Handler_L,
    /*  6 */    GIC_Default_Handler_L,
    /*  7 */    GIC_Default_Handler_L,
    /*  8 */    GIC_Default_Handler_L,
    /*  9 */    GIC_Default_Handler_L,
    /* 10 */    GIC_Default_Handler_L,
    /* 11 */    GIC_Default_Handler_L,
    /* 12 */    GIC_Default_Handler_L,
    /* 13 */    GIC_Default_Handler_L,
    /* 14 */    GIC_Default_Handler_L,
    /* 15 */    GIC_Default_Handler_L,
    /* 16 */    GIC_Default_Handler_L,
    /* 17 */    GIC_Default_Handler_L,
    /* 18 */    GIC_Default_Handler_L,
    /* 19 */    GIC_Default_Handler_L,
    /* 20 */    GIC_Default_Handler_L,
    /* 21 */    GIC_Default_Handler_L,
    /* 22 */    GIC_Default_Handler_L,
    /* 23 */    GIC_Default_Handler_L,
    /* 24 */    GIC_Default_Handler_L,
    /* 25 */    GIC_Default_Handler_L,
    /* 26 */    GIC_Default_Handler_L,
    /* 27 */    GIC_Default_Handler_L,
    /* 28 */    GIC_Default_Handler_L,
    /* 29 */    GIC_Default_Handler_L,
    /* 30 */    GIC_Default_Handler_L,
    /* 31 */    GIC_Default_Handler_L,
    /* 32 */    GIC_Default_Handler_L,
    /* 33 */    GIC_Default_Handler_L,
    /* 34 */    GIC_Default_Handler_L,
    /* 35 */    GIC_Default_Handler_L,
    /* 36 */    GIC_Default_Handler_L,
    /* 37 */    GIC_Default_Handler_L,
    /* 38 */    GIC_Default_Handler_L,
    /* 39 */    GIC_Default_Handler_L,
    /* 40 */    GIC_Default_Handler_L,
    /* 41 */    GIC_Default_Handler_L,
    /* 42 */    GIC_Default_Handler_L,
    /* 43 */    GIC_Default_Handler_L,
    /* 44 */    GIC_Default_Handler_L,
    /* 45 */    GIC_Default_Handler_L,
    /* 46 */    GIC_Default_Handler_L,
    /* 47 */    GIC_Default_Handler_L,
    /* 48 */    GIC_Default_Handler_L,
    /* 49 */    GIC_Default_Handler_L,
    /* 50 */    GIC_Default_Handler_L,
    /* 51 */    GIC_Default_Handler_L,
    /* 52 */    GIC_Default_Handler_L,
    /* 53 */    GIC_Default_Handler_L,
    /* 54 */    GIC_Default_Handler_L,
    /* 55 */    GIC_Default_Handler_L,
    /* 56 */    GIC_Default_Handler_L,
    /* 57 */    GIC_Default_Handler_L,
    /* 58 */    GIC_Default_Handler_L,
    /* 59 */    GIC_Default_Handler_L,
    /* 60 */    GIC_Default_Handler_L,
    /* 61 */    GIC_Default_Handler_L,
    /* 62 */    GIC_Default_Handler_L,
    /* 63 */    GIC_Default_Handler_L,
};
#endif  /* STATIC_DISPATCH_TABLE */


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                           INTERFACE FUNCTIONS                                           */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        __disable_irq                                                                          */
/*                                                                                                         */
/* Parameters:      none      Use in place of *_irq() intrinsics if it is necessary to                     */
/*                            keep track of the previous interrupt state                                   */
/* Returns:         The previous value of the I bit (previous interrupt state)                             */
/* Description:                                                                                            */
/*                  This routine disable interrupts                                                        */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 __enable_irq_ret_psr_i(void)
{
    UINT32 a_old_i_bit, a_psr;
    a_old_i_bit = 0;
    a_psr = 0;

    __asm __volatile__ (
        "MRS %[a_psr], DAIF                   \n\t" // Read current DAIF flags
        "AND %[a_old_i_bit], %[a_psr], #0x80  \n\t" // Extract IRQ mask bit (I bit)
        "BIC %[a_psr], %[a_psr], #0x80        \n\t" // Clear I bit to enable IRQ
        "MSR DAIF, %[a_psr]                   \n\t" // Write updated DAIF flags
        "MOV %[a_old_i_bit], %[a_old_i_bit], lsr #7 \n\t" // Shift I bit to get its previous state (0 or 1)
        : [a_old_i_bit]"+r"(a_old_i_bit), [a_psr]"+r"(a_psr)
    );

    return a_old_i_bit;
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:        __disable_irq                                                                          */
/*                                                                                                         */
/* Parameters:      none     Use in place of *_irq() intrinsics if it is necessary to                      */
/*                           keep track of the previous interrupt state                                    */
/* Returns:         The previous value of the I bit (previous interrupt state)                             */
/* Description:                                                                                            */
/*                  This routine disable interrupts                                                        */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 __disable_irq_ret_psr_i(void)
{
    UINT32 old_i_bit, psr;
    old_i_bit = 0;
    psr = 0;

    __asm __volatile__ (
        "MRS %[psr], DAIF                   \n\t" // Read the current DAIF register value into x0
        "AND %[old_i_bit], %[psr], #0x80              \n\t" // Extract the IRQ mask bit (I bit) from x0 and store it in x1
        "ORR %[psr], %[psr], #0x80              \n\t" // Set the IRQ mask bit (I bit) in x0 to disable IRQs
        "MSR DAIF, %[psr]                   \n\t" // Write the modified value back to the DAIF register
        "MOV %[old_i_bit], %[old_i_bit], LSR #7  \n\t" // Shift the extracted IRQ bit right by 7 to get its state (0 or 1)
        : [old_i_bit]"+r"(old_i_bit), [psr]"+r"(psr) );
	
    return old_i_bit;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_InitTab                                                                            */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine initializes the dispatch-table (interrupt-vector).                        */
/*---------------------------------------------------------------------------------------------------------*/
void GIC_InitTab (void)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* PSR register is read into this variable                                                             */
    /*-----------------------------------------------------------------------------------------------------*/
    UINT    saved_psr = 0;
#ifndef STATIC_DISPATCH_TABLE
    UINT    int_no;
#endif

    INTERRUPTS_SAVE_DISABLE(saved_psr);

#ifndef STATIC_DISPATCH_TABLE
    /*-----------------------------------------------------------------------------------------------------*/
    /* Initial Interrupts & Traps                                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    for (int_no = 0; int_no < GIC_TABLE_SIZE; int_no++)
    {
        GIC_SW_DispatchTable[int_no] = GIC_Default_Handler_L;
    }

#endif  /* STATIC_DISPATCH_TABLE */


    INTERRUPTS_RESTORE(saved_psr);
}
#endif /* EXTERNAL_DISPATCH_TABLE */



/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_Init                                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  inttable - TRUE: Install null_handlers for all interrupts                              */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:    This function only sets the Global Interrupt Enable (I bit in PSR register).           */
/*                  The Local Interrupt Enable (E bit in PSR register) remains cleared.                    */
/*                  The upper-layer should call ENABLE_INTERRUPTS to set the E bit when desired.           */
/* Description:                                                                                            */
/*                  This routine:                                                                          */
/*                  1. Masks all interrupts.                                                               */
/*                  2. Clears all pending interrupts.                                                      */
/*                  3. if inttable is TRUE: Installs the default interrupt dispatch-table                  */
/*                     (null_handlers for all interrupts).                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void GIC_Init (BOOLEAN inttable)
{
    UINT group_no;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Disable Interrupts                                                                                  */
    /*-----------------------------------------------------------------------------------------------------*/
    // Disable the distributor
    SET_REG_FIELD (GIC_D_CTLR, GIC_D_CTLR_EnableGrp0, 0);
    DISABLE_INTERRUPTS();

    for(group_no = 0; group_no < GIC_NUM_OF_INTERRUPT_GROUPS; group_no++)
    {
        /*-------------------------------------------------------------------------------------------------*/
        /* Mask All Interrupts                                                                             */
        /*-------------------------------------------------------------------------------------------------*/
        REG_WRITE(GIC_D_ICENABLER(group_no), 0xFFFFFFFF);

        /*-------------------------------------------------------------------------------------------------*/
        /* Clear all pending interrupts                                                                    */
        /*-------------------------------------------------------------------------------------------------*/
        REG_WRITE(GIC_D_ICPENDR(group_no), 0xFFFFFFFF);
    }

#ifndef EXTERNAL_DISPATCH_TABLE
    if (inttable)
    {
        GIC_InitTab();
    }
#endif

    // Set priority mask to allow all interrupts
    REG_WRITE (GIC_C_PMR(0), 0xFF);

    // Enable the distributor
    SET_REG_FIELD (GIC_D_CTLR, GIC_D_CTLR_EnableGrp0, 1);

    // Enable the CPU interface
    SET_REG_FIELD (GIC_C_CTLR, GIC_C_CTLR_EnableGrp0, 1);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Note that at this point interrupts are still disabled, since the E bit in PSR register is cleared.  */
    /* The upper-layer should call ENABLE_INTERRUPTS to set the E bit when desired                         */
    /*-----------------------------------------------------------------------------------------------------*/
}



#ifndef STATIC_DISPATCH_TABLE
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_InstallTrap                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  trap_no - number of trap for which the handler should be installed.                    */
/*                  proc   - trap handler procedure to be installed.                                       */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine installs the provided trap handler in the dispatch table.                 */
/*                  The function is used to install or change the trap handler when the                    */
/*                  dispatch-table is placed in RAM.                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void GIC_InstallTrap (
    GIC_TRAP_SRC_T  trap_no,
    HANDLER_T       proc
)
{
    ASSERT(trap_no < GIC_TRAP_NUM);

    ATOMIC_OP((GIC_SW_DispatchTable[trap_no] = proc));
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_InstallHandler                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  int_no - number of interrupt for which the handler should be installed.                */
/*                  proc   - interrupt handler procedure to be installed.                                  */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine installs the provided interrupt handler in the dispatch table.            */
/*                  The function is used to install or change the interrupt handler when the               */
/*                  dispatch-table is placed in RAM.                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void GIC_InstallHandler (
    GIC_INT_SRC_T   int_no,
    HANDLER_T       proc
)
{
    ASSERT(int_no < GIC_INTERRUPT_NUM);

    ATOMIC_OP((GIC_SW_DispatchTable[GIC_INT_BASE + int_no] = proc));
}
#endif  /* STATIC_DISPATCH_TABLE */



/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_EnableInt                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  int_no  - interrupt number                                                             */
/*                  enable  - TRUE to enable interrupt, FALSE to disable                                   */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine enables/disables interrupt number int_no.                                 */
/*---------------------------------------------------------------------------------------------------------*/
void GIC_EnableInt (GIC_INT_SRC_T int_no, BOOLEAN enable)
{
    UINT8  group_no;
    UINT8  input_no;
    UINT32 iser = 0;
    const UINT32 CPU_TARGET = 0x1 ;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Check for Illegal Cases                                                                             */
    /*-----------------------------------------------------------------------------------------------------*/
    ASSERT(int_no < GIC_INTERRUPT_NUM);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Determine the interrupt register and its offset inside the register                                 */
    /*-----------------------------------------------------------------------------------------------------*/
    group_no = GIC_GET_INTERRUPT_GROUP_NUM(int_no);
    input_no = GIC_GET_INTERRUPT_INPUT_NUM(int_no);

    if (enable)
    {
        /*-------------------------------------------------------------------------------------------------*/
        /* Enable interrupt                                                                                */
        /*-------------------------------------------------------------------------------------------------*/
        SET_VAR_BIT(iser, input_no);
        REG_WRITE(GIC_D_ISENABLER(group_no), iser);

        // Route int to CPU 0 
        REG_WRITE(GIC_D_ITARGETSR(int_no / 4), (CPU_TARGET << ((int_no % 4) * 8)));

    }
    else
    {
        /*-------------------------------------------------------------------------------------------------*/
        /* Disable interrupt                                                                               */
        /*-------------------------------------------------------------------------------------------------*/
        SET_VAR_BIT(iser, input_no);
        REG_WRITE(GIC_D_ICENABLER(group_no), iser);
    }
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_IntEnabled                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  int_no    - Number of interrupt to check if enabled                                    */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine checks if given interrupt is enabled                                      */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN GIC_IntEnabled (GIC_INT_SRC_T int_no)
{
    UINT8 group_no;
    UINT8 input_no;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Check for Illegal Cases                                                                             */
    /*-----------------------------------------------------------------------------------------------------*/
    ASSERT(int_no < GIC_INTERRUPT_NUM);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Determine the interrupt register and its offset inside the register                                 */
    /*-----------------------------------------------------------------------------------------------------*/
    group_no = GIC_GET_INTERRUPT_GROUP_NUM(int_no);
    input_no = GIC_GET_INTERRUPT_INPUT_NUM(int_no);

    return (BOOLEAN)READ_REG_BIT(GIC_D_ISENABLER(group_no), input_no);
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_ClearInt                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  int_no - interrupt number                                                              */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine clears interrupt number int_no.                                           */
/*---------------------------------------------------------------------------------------------------------*/
void GIC_ClearInt (GIC_INT_SRC_T int_no)
{
    UINT8  group_no;
    UINT8  input_no;
    UINT32 icpr = 0;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Check for Illegal Cases                                                                             */
    /*-----------------------------------------------------------------------------------------------------*/
    ASSERT(int_no < GIC_INTERRUPT_NUM);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Determine the interrupt register and its offset inside the register                                 */
    /*-----------------------------------------------------------------------------------------------------*/
    group_no = GIC_GET_INTERRUPT_GROUP_NUM(int_no);
    input_no = GIC_GET_INTERRUPT_INPUT_NUM(int_no);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Clear the pending interrupt                                                                         */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_VAR_BIT(icpr, input_no);
    REG_WRITE(GIC_D_ICPENDR(group_no), icpr);
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_PendingInt                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  int_no - interrupt number                                                              */
/*                                                                                                         */
/* Returns:         interrupts status                                                                      */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine returns TRUE if given int_no interrupt is pending                         */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN GIC_PendingInt (GIC_INT_SRC_T int_no)
{
    UINT8 group_no;
    UINT8 input_no;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Check for Illegal Cases                                                                             */
    /*-----------------------------------------------------------------------------------------------------*/
    ASSERT(int_no < GIC_INTERRUPT_NUM);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Determine the interrupt register and its offset inside the register                                 */
    /*-----------------------------------------------------------------------------------------------------*/
    group_no = GIC_GET_INTERRUPT_GROUP_NUM(int_no);
    input_no = GIC_GET_INTERRUPT_INPUT_NUM(int_no);

    return (BOOLEAN)READ_REG_BIT(GIC_D_ISPENDR(group_no), input_no);
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_ConfigPriority                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  int_no   - interrupt number                                                            */
/*                  priority - interrupt priority level                                                    */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine sets int_no interrupt's priority.                                         */
/*---------------------------------------------------------------------------------------------------------*/
void GIC_ConfigPriority (GIC_INT_SRC_T int_no, UINT8 priority)
{
    UINT8 group_no;
    UINT8 input_no;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Check for Illegal Cases                                                                             */
    /*-----------------------------------------------------------------------------------------------------*/
    ASSERT(int_no < GIC_INTERRUPT_NUM);
    ASSERT(priority < GIC_MAX_PRIORITY);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Determine the priority register and its offset inside the register                                  */
    /*-----------------------------------------------------------------------------------------------------*/
    group_no = GIC_GET_PRIORITY_GROUP_NUM(int_no);
    input_no = GIC_GET_PRIORITY_INPUT_NUM(int_no);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Set the priority                                                                                    */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD(GIC_C_PMR(group_no), GIC_C_PMR_PriorityMask, priority);
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_ConfigTrapPriority                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  trap_no  - trap number                                                                 */
/*                  priority - trap priority level                                                         */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine sets trap_no trap's priority.                                             */
/*---------------------------------------------------------------------------------------------------------*/
void GIC_ConfigTrapPriority (GIC_TRAP_SRC_T trap_no, UINT8 priority)
{
#if 0
    UINT8 group_no;
    UINT8 input_no;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Check for Illegal Cases                                                                             */
    /*-----------------------------------------------------------------------------------------------------*/
    ASSERT(trap_no < GIC_TRAP_NUM);
    ASSERT(trap_no > GIC_TRAP_HARD_FAULT);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Determine the priority register and its offset inside the register                                  */
    /*-----------------------------------------------------------------------------------------------------*/
    group_no = GIC_GET_PRIORITY_GROUP_NUM(trap_no) - 1;
    input_no = GIC_GET_PRIORITY_INPUT_NUM(trap_no);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Set the priority                                                                                    */
    /*-----------------------------------------------------------------------------------------------------*/
    // TODO:     SCS_SetTrapPriority(group_no, input_no, priority);
#endif
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_Isr_L                                                                              */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine implements Interrupt Service Routine                                      */
/*---------------------------------------------------------------------------------------------------------*/
_ISR_ void GIC_Isr_L (void)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* Retrieve ISR vector number                                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    UINT16 vector = 0; // TODO: SCS_GetActiveVectorNumber();

    /*-----------------------------------------------------------------------------------------------------*/
    /* Clear the pending interrupt                                                                         */
    /*-----------------------------------------------------------------------------------------------------*/
    GIC_ClearInt((GIC_INT_SRC_T)(vector - GIC_TRAP_NUM));

    /*-----------------------------------------------------------------------------------------------------*/
    /* Invoke appropriate vector                                                                           */
    /*-----------------------------------------------------------------------------------------------------*/
    (GIC_SW_DispatchTable[vector])(vector - GIC_TRAP_NUM);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_PrintRegs                                                                          */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module registers                                               */
/*---------------------------------------------------------------------------------------------------------*/
void GIC_PrintRegs (void)
{
    UINT n;

    HAL_PRINT("/*--------------*/\n");
    HAL_PRINT("/*     GIC      */\n");
    HAL_PRINT("/*--------------*/\n\n");

    for(n = 0; n < GIC_NUM_OF_INTERRUPT_GROUPS; n++)
    {
        HAL_PRINT("GIC_D_ISENABLER(%d)   = 0x%08X\n", n, REG_READ(GIC_D_ISENABLER(n)));
    }

    for(n = 0; n < GIC_NUM_OF_INTERRUPT_GROUPS; n++)
    {
        HAL_PRINT("GIC_D_ICENABLER(%d)   = 0x%08X\n", n, REG_READ(GIC_D_ICENABLER(n)));
    }
    //for(n = 0; n < GIC_NUM_OF_INTERRUPT_GROUPS; n++)
    //{
    //    HAL_PRINT("GIC_D_ISACTIVER(%d)   = 0x%08X\n", n, REG_READ(GIC_D_ISACTIVER(n)));
    //}

    for(n = 0; n < GIC_NUM_OF_INTERRUPT_GROUPS; n++)
    {
        HAL_PRINT("GIC_D_IPRIORITYR(%d)   = 0x%08X\n", n, REG_READ(GIC_D_IPRIORITYR(n)));
    }

    for(n = 0; n < GIC_NUM_OF_INTERRUPT_GROUPS; n++)
    {
        HAL_PRINT("GIC_D_ITARGETSR(%d)   = 0x%08X\n", n, REG_READ(GIC_D_ITARGETSR(n)));
    }

    for(n = 0; n < GIC_NUM_OF_INTERRUPT_GROUPS; n++)
    {
        HAL_PRINT("GIC_D_ICFGR(%d)   = 0x%08X\n", n, REG_READ(GIC_D_ICFGR(n)));
    }

    HAL_PRINT("GIC_C_CTLR      = 0x%08X\n", REG_READ( GIC_C_CTLR ));
    HAL_PRINT("GIC_D_TYPER     = 0x%08X\n", REG_READ( GIC_D_TYPER));
    HAL_PRINT("GIC_D_IIDR      = 0x%08X\n", REG_READ( GIC_D_IIDR ));
    HAL_PRINT("GIC_D_SGIR      = 0x%08X\n", REG_READ( GIC_D_SGIR ));
    HAL_PRINT("GIC_C_BPR       = 0x%08X\n", REG_READ( GIC_C_BPR  ));
    HAL_PRINT("GIC_C_IAR       = 0x%08X\n", REG_READ( GIC_C_IAR  ));
    HAL_PRINT("GIC_C_EOIR      = 0x%08X\n", REG_READ( GIC_C_EOIR ));
    HAL_PRINT("GIC_C_HPPIR     = 0x%08X\n", REG_READ( GIC_C_HPPIR));
    HAL_PRINT("GIC_C_APR       = 0x%08X\n", REG_READ( GIC_C_APR  ));
    HAL_PRINT("GIC_C_NSAPR     = 0x%08X\n", REG_READ( GIC_C_NSAPR));
    HAL_PRINT("GIC_C_IIDR      = 0x%08X\n", REG_READ( GIC_C_IIDR ));
    HAL_PRINT("GIC_C_CTLR      = 0x%08X\n", REG_READ( GIC_C_CTLR ));

    HAL_PRINT("\n");
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_PrintVersion                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module version                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void GIC_PrintVersion (void)
{
    HAL_PRINT("GIC         = %X\n", MODULE_VERSION(GIC_MODULE_TYPE));
}

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                             LOCAL FUNCTIONS                                             */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* TRAP Routing functions                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
TRAP_ROUTER_FUNCTION(GIC_TrapRouting_RES0_L, 0)
TRAP_ROUTER_FUNCTION(GIC_TrapRouting_RST_L , 1)
TRAP_ROUTER_FUNCTION(GIC_TrapRouting_UNDEF_L, 2)
TRAP_ROUTER_FUNCTION(GIC_TrapRouting_SWI_L, 3)
TRAP_ROUTER_FUNCTION(GIC_TrapRouting_PABT_L, 4)
TRAP_ROUTER_FUNCTION(GIC_TrapRouting_DABT_L, 5)
TRAP_ROUTER_FUNCTION(GIC_TrapRouting_HVC_L, 6)
TRAP_ROUTER_FUNCTION(GIC_TrapRouting_IRQ_L, 7)
TRAP_ROUTER_FUNCTION(GIC_TrapRouting_FIQ_L, 8)
TRAP_ROUTER_FUNCTION(GIC_TrapRouting_SYSERR_L, 9)
TRAP_ROUTER_FUNCTION(GIC_TrapRouting_SPURIOUS_L, 10)
TRAP_ROUTER_FUNCTION(GIC_TrapRouting_SMC_L, 11)
TRAP_ROUTER_FUNCTION(GIC_TrapRouting_EXT_ABT_L, 12)
TRAP_ROUTER_FUNCTION(GIC_TrapRouting_VIRQ_L, 13)
TRAP_ROUTER_FUNCTION(GIC_TrapRouting_NMI_L, 14)
TRAP_ROUTER_FUNCTION(GIC_TrapRouting_DEBUG_L, 15)

/*---------------------------------------------------------------------------------------------------------*/
/* DEFAULT SW HANDLER                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
void GIC_Default_Handler_L (UINT32 int_num)
{
    ASSERT(FALSE);
}

