/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   gic_regs.h                                                               */
/*            This file contains GIC v2 module registers  (for A35)           */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/

#ifndef _GIC_REGS_H
#define _GIC_REGS_H

#include __CHIP_H_FROM_DRV()


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                GIC Distributor Registers (GICD)                                         */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************/
/*    Distributor Control Register (GIC_D_CTLR)                                                             */
/***********************************************************************************************************/
#define GIC_D_CTLR                             (GICD_BASE_ADDR + 0x000) , GIC_ACCESS , 32
#define GIC_D_CTLR_EnableGrp0                   0,   1               /* Enable Group 0 interrupts */
#define GIC_D_CTLR_EnableGrp1                   1,   1               /* Enable Group 1 interrupts */
#define GIC_D_CTLR_ARE                          4,   1               /* Enable affinity routing */

/***********************************************************************************************************/
/*    Interrupt Controller Type Register (GIC_D_TYPER)                                                      */
/***********************************************************************************************************/
#define GIC_D_TYPER                            (GICD_BASE_ADDR + 0x004) , GIC_ACCESS , 32
#define GIC_D_TYPER_ITLINESNUM                  0,   5               /* Number of interrupt lines */

/***********************************************************************************************************/
/*    Distributor Implementer Identification Register (GIC_D_IIDR)                                          */
/***********************************************************************************************************/
#define GIC_D_IIDR                             (GICD_BASE_ADDR + 0x008) , GIC_ACCESS , 32
#define GIC_D_IIDR_IMPLEMENTER                  0,   12              /* Implementer code */
#define GIC_D_IIDR_REVISION                     12,  4               /* Revision number */
#define GIC_D_IIDR_VARIANT                      16,  4               /* Variant number */
#define GIC_D_IIDR_PRODUCTID                    20,  12              /* Product ID */

/***********************************************************************************************************/
/*    Interrupt Group Registers (GIC_D_IGROUPRn)                                                            */
/***********************************************************************************************************/
#define GIC_D_IGROUPR(n)                       (GICD_BASE_ADDR + 0x080 + (n) * 4) , GIC_ACCESS , 32
#define GIC_D_IGROUPR_GROUP                     0,   32              /* Group assignment for interrupts 32*(n+1) */

/***********************************************************************************************************/
/*    Interrupt Set-Enable Registers (GIC_D_ISENABLERn)                                                     */
/***********************************************************************************************************/
#define GIC_D_ISENABLER(n)                     (GICD_BASE_ADDR + 0x100 + (n) * 4) , GIC_ACCESS , 32
#define GIC_D_ISENABLER_SET_ENABLE              0,   32              /* Set enable for interrupts 32*(n+1) */

/***********************************************************************************************************/
/*    Interrupt Clear-Enable Registers (GIC_D_ICENABLERn)                                                   */
/***********************************************************************************************************/
#define GIC_D_ICENABLER(n)                     (GICD_BASE_ADDR + 0x180 + (n) * 4) , GIC_ACCESS , 32
#define GIC_D_ICENABLER_CLEAR_ENABLE            0,   32              /* Clear enable for interrupts 32*(n+1) */

/***********************************************************************************************************/
/*    Interrupt Set-Pending Registers (GIC_D_ISPENDRn)                                                      */
/***********************************************************************************************************/
#define GIC_D_ISPENDR(n)                       (GICD_BASE_ADDR + 0x200 + (n) * 4) , GIC_ACCESS , 32
#define GIC_D_ISPENDR_SET_PENDING               0,   32              /* Set pending for interrupts 32*(n+1) */

/***********************************************************************************************************/
/*    Interrupt Clear-Pending Registers (GIC_D_ICPENDRn)                                                    */
/***********************************************************************************************************/
#define GIC_D_ICPENDR(n)                       (GICD_BASE_ADDR + 0x280 + (n) * 4) , GIC_ACCESS , 32
#define GIC_D_ICPENDR_CLEAR_PENDING             0,   32              /* Clear pending for interrupts 32*(n+1) */

/***********************************************************************************************************/
/*    Interrupt Priority Registers (GIC_D_IPRIORITYRn)                                                      */
/***********************************************************************************************************/
#define GIC_D_IPRIORITYR(n)                    (GICD_BASE_ADDR + 0x400 + (n) * 4) , GIC_ACCESS , 32
#define GIC_D_IPRIORITYR_PRIORITY               0,   8               /* Priority field for each interrupt */

/***********************************************************************************************************/
/*    Interrupt Processor Targets Registers (GIC_D_ITARGETSRn)                                              */
/***********************************************************************************************************/
#define GIC_D_ITARGETSR(n)                     (GICD_BASE_ADDR + 0x800 + (n) * 4) , GIC_ACCESS , 32
#define GIC_D_ITARGETSR_CPU_TARGET              0,   8               /* CPU target for interrupt */

/***********************************************************************************************************/
/*    Interrupt Configuration Registers (GIC_D_ICFGRn)                                                      */
/***********************************************************************************************************/
#define GIC_D_ICFGR(n)                         (GICD_BASE_ADDR + 0xC00 + (n) * 4) , GIC_ACCESS , 32
#define GIC_D_ICFGR_CONFIG                      0,   2               /* Configuration for interrupts 16*(n+1) */

/***********************************************************************************************************/
/*    Software Generated Interrupt Register (GIC_D_SGIR)                                                    */
/***********************************************************************************************************/
#define GIC_D_SGIR                             (GICD_BASE_ADDR + 0xF00) , GIC_ACCESS , 32
#define GIC_D_SGIR_SGIINTID                     0,   4               /* Software generated interrupt ID */
#define GIC_D_SGIR_CPUTARGETLIST                16,  8               /* Target CPUs for SGI */
#define GIC_D_SGIR_TARGETLISTFILTER             24,  2               /* Target list filter */

/***********************************************************************************************************/
/*    Peripheral ID Registers (GIC_D_PIDRn)                                                                 */
/***********************************************************************************************************/
#define GIC_D_PIDR(n)                          (GICD_BASE_ADDR + 0xFE0 + (n) * 4) , GIC_ACCESS , 32
#define GIC_D_PIDR_ID                           0,   32              /* Peripheral ID Register */

/***********************************************************************************************************/
/*    Component ID Registers (GIC_D_CIDRn)                                                                  */
/***********************************************************************************************************/
#define GIC_D_CIDR(n)                          (GICD_BASE_ADDR + 0xFF0 + (n) * 4) , GIC_ACCESS , 32
#define GIC_D_CIDR_ID                           0,   32              /* Component ID Register */



/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                GIC CPU Interface Registers (GICC)                                       */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************/
/*    CPU Interface Control Register (GIC_C_CTLR)                                                           */
/***********************************************************************************************************/
#define GIC_C_CTLR                             (GICC_BASE_ADDR + 0x000) , GIC_ACCESS , 32
#define GIC_C_CTLR_EnableGrp0                   0,   1               /* Enable handling of Group 0 interrupts */
#define GIC_C_CTLR_EnableGrp1                   1,   1               /* Enable handling of Group 1 interrupts */
#define GIC_C_CTLR_CBPR                         2,   1               /* Enable Binary Point Register */
#define GIC_C_CTLR_ARE                          3,   1               /* Enable Architecturally defined bits */
#define GIC_C_CTLR_Reserved                     4,   28              /* Reserved */

/***********************************************************************************************************/
/*    Interrupt Priority Register (GIC_C_PMR)                                                                 */
/***********************************************************************************************************/
#define GIC_C_PMR(n)                           (GICC_BASE_ADDR + 0x004 + (n)*4) , GIC_ACCESS , 32
#define GIC_C_PMR_PriorityMask                 0,   8               /* Priority mask */

/***********************************************************************************************************/
/*    Binary Point Register (GIC_C_BPR)                                                                     */
/***********************************************************************************************************/
#define GIC_C_BPR                              (GICC_BASE_ADDR + 0x008) , GIC_ACCESS , 32
#define GIC_C_BPR_BinaryPoint                  0,   3               /* Number of bits for binary point */

/***********************************************************************************************************/
/*    Interrupt Acknowledge Register (GIC_C_IAR)                                                            */
/***********************************************************************************************************/
#define GIC_C_IAR                              (GICC_BASE_ADDR + 0x00C) , GIC_ACCESS , 32
#define GIC_C_IAR_InterruptID                  0,   10              /* Interrupt ID */

/***********************************************************************************************************/
/*    End of Interrupt Register (GIC_C_EOIR)                                                                 */
/***********************************************************************************************************/
#define GIC_C_EOIR                             (GICC_BASE_ADDR + 0x010) , GIC_ACCESS , 32
#define GIC_C_EOIR_InterruptID                 0,   10              /* Interrupt ID */

/***********************************************************************************************************/
/*    Running Priority Register (GIC_C_RPR)                                                                 */
/***********************************************************************************************************/
#define GIC_C_RPR                              (GICC_BASE_ADDR + 0x014) , GIC_ACCESS , 32
#define GIC_C_RPR_InterruptPriority            0,   8               /* Current interrupt priority */

/***********************************************************************************************************/
/*    Highest Priority Pending Interrupt Register (GIC_C_HPPIR)                                              */
/***********************************************************************************************************/
#define GIC_C_HPPIR                            (GICC_BASE_ADDR + 0x018) , GIC_ACCESS , 32
#define GIC_C_HPPIR_InterruptID                0,   10              /* Highest priority pending interrupt ID */

/***********************************************************************************************************/
/*    Alias Priority Register (GIC_C_APR)                                                                   */
/***********************************************************************************************************/
#define GIC_C_APR                              (GICC_BASE_ADDR + 0x0D0) , GIC_ACCESS , 32
#define GIC_C_APR_AliasPriority                0,   8               /* Priority value */

/***********************************************************************************************************/
/*    Non-secure Alias Priority Register (GIC_C_NSAPR)                                                       */
/***********************************************************************************************************/
#define GIC_C_NSAPR                            (GICC_BASE_ADDR + 0x0E0) , GIC_ACCESS , 32
#define GIC_C_NSAPR_NS_AliasPriority           0,   8               /* Non-secure priority value */

/***********************************************************************************************************/
/*    Identification Register (GIC_C_IIDR)                                                                   */
/***********************************************************************************************************/
#define GIC_C_IIDR                             (GICC_BASE_ADDR + 0x0FC) , GIC_ACCESS , 32
#define GIC_C_IIDR_ImplementerID               0,   12              /* Implementer ID */
#define GIC_C_IIDR_Revision                    12,  4               /* Revision */
#define GIC_C_IIDR_Variant                     16,  4               /* Variant */
#define GIC_C_IIDR_Arch                        20,  4               /* Architecture */

/***********************************************************************************************************/
/*    CPU Interface Control Register (GIC_C_CTLR)                                                           */
/***********************************************************************************************************/
#define GIC_C_CTLR                             (GICC_BASE_ADDR + 0x000) , GIC_ACCESS , 32
#define GIC_C_CTLR_EnableGrp0                   0,   1               /* Enable handling of Group 0 interrupts */
#define GIC_C_CTLR_EnableGrp1                   1,   1               /* Enable handling of Group 1 interrupts */
#define GIC_C_CTLR_CBPR                         2,   1               /* Enable Binary Point Register */
#define GIC_C_CTLR_ARE                          3,   1               /* Enable Architecturally defined bits */
#define GIC_C_CTLR_Reserved                     4,   28              /* Reserved */


#endif /* _GIC_REGS_H */

