/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   gic_if.h                                                                 */
/* This file contains Nested Vectored Interrupt Controller (GIC) interface    */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/
#ifndef _GIC_IF_H
#define _GIC_IF_H

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                                 INCLUDES                                                */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#include __CHIP_H_FROM_IF()

#include "hal.h"

#if defined GIC_MODULE_TYPE
#include __MODULE_HEADER(gic, GIC_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                           TYPES & DEFINITIONS                                           */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

//#ifndef GIC_ISR
#define GIC_ISR GIC_Isr_L
typedef void (*GIC_ISR_t)(void);

//#else
//extern _ISR_ void GIC_ISR (void);
//#endif


/*---------------------------------------------------------------------------------------------------------*/
/* Trap Mapping                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum
{
    GIC_TRAP_0   = 0,    /*  */
    GIC_TRAP_1   = 1,    /*  */
    GIC_TRAP_2   = 2,    /*  */
    GIC_TRAP_3   = 3,    /*  */
    GIC_TRAP_4   = 4,    /*  */
    GIC_TRAP_5   = 5,    /*  */
    GIC_TRAP_6   = 6,    /*  */
    GIC_TRAP_7   = 7,    /*  */
    GIC_TRAP_8   = 8,    /*  */
    GIC_TRAP_9   = 9,    /*  */
    GIC_TRAP_10  = 10,   /*  */
    GIC_TRAP_11  = 11,   /*  */
    GIC_TRAP_12  = 12,   /*  */
    GIC_TRAP_13  = 13,   /*  */
    GIC_TRAP_14  = 14,   /*  */
    GIC_TRAP_15  = 15,   /*  */
    GIC_TRAP_16  = 16,   /*  */
    GIC_TRAP_17  = 17,   /*  */
    GIC_TRAP_18  = 18,   /*  */
    GIC_TRAP_19  = 19,   /*  */
    GIC_TRAP_20  = 20,   /*  */
    GIC_TRAP_21  = 21,   /*  */
    GIC_TRAP_22  = 22,   /*  */
    GIC_TRAP_23  = 23,   /*  */
    GIC_TRAP_24  = 24,   /*  */
    GIC_TRAP_25  = 25,   /*  */
    GIC_TRAP_26  = 26,   /*  */
    GIC_TRAP_27  = 27,   /*  */
    GIC_TRAP_28  = 28,   /*  */
    GIC_TRAP_29  = 29,   /*  */
    GIC_TRAP_30  = 30,   /*  */
    GIC_TRAP_31  = 31    /*  */
} GIC_TRAP_SRC_T;


typedef void (*HANDLER_T)(UINT int_num);


/*---------------------------------------------------------------------------------------------------------*/
/* Interrupt Dispatch Table Size (includes all traps and interrupts)                                       */
/*---------------------------------------------------------------------------------------------------------*/
#define GIC_TABLE_SIZE          (GIC_TRAP_NUM + GIC_INTERRUPT_NUM)

/*---------------------------------------------------------------------------------------------------------*/
/* First table enteries are dedicated for traps, followed by interrupts                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define GIC_INT_BASE            GIC_TRAP_NUM

#ifdef EXTERNAL_DISPATCH_TABLE
#define STATIC_DISPATCH_TABLE       /* External dispatch table must be static */
#endif

#ifdef STATIC_DISPATCH_TABLE
/*---------------------------------------------------------------------------------------------------------*/
/* Allow the same code for static dispatch table (handler installation is ignored)                         */
/*---------------------------------------------------------------------------------------------------------*/
#define     GIC_InstallHandler (INT_NO, PROC)    {}
#define     GIC_InstallTrap (TRAP_NO, PROC)      {}
#endif


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                           INTERFACE FUNCTIONS                                           */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

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
void GIC_Init (BOOLEAN inttable);

#ifndef EXTERNAL_DISPATCH_TABLE
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_InitTab                                                                            */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine initializes the dispatch-table (interrupt-vector).                        */
/*---------------------------------------------------------------------------------------------------------*/
void GIC_InitTab (void);

#endif /* EXTERNAL_DISPATCH_TABLE */

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
void GIC_InstallTrap (GIC_TRAP_SRC_T trap_no, HANDLER_T proc);

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
void GIC_InstallHandler (GIC_INT_SRC_T int_no, HANDLER_T proc);

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
void GIC_EnableInt (GIC_INT_SRC_T int_no, BOOLEAN enable);

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
BOOLEAN GIC_IntEnabled (GIC_INT_SRC_T int_no);

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
void GIC_ClearInt (GIC_INT_SRC_T int_no);

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
BOOLEAN GIC_PendingInt (GIC_INT_SRC_T int_no);

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
void GIC_ConfigPriority (GIC_INT_SRC_T int_no, UINT8 priority);

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
void GIC_ConfigTrapPriority (GIC_TRAP_SRC_T trap_no, UINT8 priority);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        null_handler                                                                           */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine implements a NULL handler (used as dummy or placeholder in dispatch table)*/
/*---------------------------------------------------------------------------------------------------------*/
void null_handler (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_Isr_L                                                                              */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine implements Interrupt Service Routine                                      */
/*---------------------------------------------------------------------------------------------------------*/
_ISR_ void GIC_Isr_L (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        __disable_irq                                                                          */
/*                                                                                                         */
/* Parameters:      none      Use in place of *_irq() intrinsics if it is necessary to                     */
/*                            keep track of the previous interrupt state                                   */
/* Returns:         The previous value of the I bit (previous interrupt state)                             */
/* Description:                                                                                            */
/*                  This routine disable interrupts                                                        */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 __enable_irq_ret_psr_i (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        __disable_irq                                                                          */
/*                                                                                                         */
/* Parameters:      none     Use in place of *_irq() intrinsics if it is necessary to                      */
/*                           keep track of the previous interrupt state                                    */
/* Returns:         The previous value of the I bit (previous interrupt state)                             */
/* Description:                                                                                            */
/*                  This routine disable interrupts                                                        */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 __disable_irq_ret_psr_i (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_PrintRegs                                                                          */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module registers                                               */
/*---------------------------------------------------------------------------------------------------------*/
void GIC_PrintRegs (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GIC_PrintVersion                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module version                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void GIC_PrintVersion (void);


#endif /* _GIC_IF_H */

