/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   fuse_regs.h                                                              */
/*            This file contains register definitions for fuse module         */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/

#ifndef FUSE_REGS_H
#define FUSE_REGS_H

#include __CHIP_H_FROM_DRV()

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                             FUSE Registers                                              */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/***********************************************************************************************************/
/*    Fuse Array Status Register (FST)                                                                     */
/***********************************************************************************************************/
#define FST                                  (FUSE_BASE_ADDR + 0x00),  MEM,    32         /* Offset: 00h */
#define FST_RIEN                              2,    1               /* 2 RIEN (Ready Interrupt Enable) */
#define FST_RDST                              1,    1               /* 1 RDST (Ready Status) */
#define FST_RDY                               0,    1               /* 0 RDY (Ready) */

/***********************************************************************************************************/
/*    Fuse Array Address Register (FADDR)                                                                  */
/***********************************************************************************************************/
#define FADDR                                (FUSE_BASE_ADDR + 0x04),  MEM,    32         /* Offset: 04h */
#define FADDR_SPEN                            17,   1               /* 17 SPEN (Auto Smart Programming Enable) */
#define FADDR_IN_PROG                         16,   1               /* 16 IN_PROG (In Programming) */
#define FADDR_BYTEADDR                        3,    13               /* 3-15 BYTEADDR (Fuse Read Address) */
#define FADDR_BITPOS                          0,    3               /* 0-2 BITPOS (Bit Position) */

/***********************************************************************************************************/
/*    Fuse Array Data Register (FDATA)                                                                     */
/***********************************************************************************************************/
#define FDATA                                (FUSE_BASE_ADDR + 0x08),  MEM,    32         /* Offset: 08h */
#define FDATA_FDATA                           0,    8               /* 0-7 FDATA (None) */

/***********************************************************************************************************/
/*    Fuse Key Index Register (FKEYIND)                                                                    */
/***********************************************************************************************************/
#define FKEYIND                              (FUSE_BASE_ADDR + 0x10),  MEM,    32         /* Offset: 10h */
#define FKEYIND_KIND                          18,   2               /* 18-19 KIND (Key Index) */
#define FKEYIND_FUSERR                        8,    1               /* 8 FUSERR (Fuse Error) */
#define FKEYIND_KSIZE                         4,    3               /* 4-6 KSIZE (Key Size) */
#define FKEYIND_KVAL                          0,    1               /* 0 KVAL (Key Valid) */

/***********************************************************************************************************/
/*    Fuse Array Control Register (FCTL)                                                                   */
/***********************************************************************************************************/
#define FCTL                                 (FUSE_BASE_ADDR + 0x14),  MEM,    32         /* Offset: 14h */
#define FCTL_FCTL                             0,    32               /* 0-31 FCTL (Fuse Array Control) */

/***********************************************************************************************************/
/*    Fuse Array Access Register (FAAR)                                                                    */
/***********************************************************************************************************/
#define FAAR                                 (FUSE_BASE_ADDR + 0x18),  MEM,    32         /* Offset: 18h */
#define FAAR_LOCK                             3,    1               /* 3 LOCK (FAAR lock) */
#define FAAR_FOFFA                            1,    2               /* 1-2 FOFFA (Force Off Fuse Array) */
#define FAAR_FAAR_RDY                         0,    1               /* 0 FAAR_RDY (FAAR Ready) */

/***********************************************************************************************************/
/*    Sideband Master Select Register (SMSR)                                                               */
/***********************************************************************************************************/
#define SMSR                                 (FUSE_BASE_ADDR + 0x50),  MEM,    32         /* Offset: 50h */
#define SMSR_LOCK                             2,    1               /* 2 LOCK (SMSR lock) */
#define SMSR_FOFSB                            1,    1               /* 1 FOFSB (Force Off Sideband) */
#define SMSR_SMSR_RDY                         0,    1               /* 0 SMSR_RDY (SMSR Ready) */

/***********************************************************************************************************/
/*    Last Key Register (LASTKEY)                                                                          */
/***********************************************************************************************************/
#define LASTKEY                              (FUSE_BASE_ADDR + 0x54),  MEM,    32         /* Offset: 54h */
#define LASTKEY_LOCK                          7,    1               /* 7 LOCK (LASTKEY lock) */
#define LASTKEY_FOFLK                         6,    1               /* 6 FOFLK (Force Off Last Key) */
#define LASTKEY_LAST_KEY                      1,    5               /* 1-5 LAST_KEY (Last Key Used by ROM Code) */
#define LASTKEY_LK_RDY                        0,    1               /* 0 LK_RDY (LASTKEY Ready) */

/***********************************************************************************************************/
/*    Version Register (VERSION)                                                                           */
/***********************************************************************************************************/
#define VERSION                              (FUSE_BASE_ADDR + 0x1C),  MEM,    32         /* Offset: 1Ch */
#define VERSION_VERSION                       0,    8               /* 0-7 VERSION (None) */

/***********************************************************************************************************/
/*    Fuse Array Configuration Register 0 to 7 (FCFG0-7)                                                   */
/***********************************************************************************************************/
#define FCFG0_7(n)                           (FUSE_BASE_ADDR + (0x30 + (4 * (n)))),  MEM,    32         /* Offset: 30h-4Ch */
#define FCFG0_7_TIP(n)                       (TIP_FUSE_BASE_ADDR + (0x30 + (4 * (n)))),  MEM,    32         /* Offset: 30h-4Ch */
#define FCFG0_7_A35(n)                       (FUSE_PHYS_BASE_ADDR + (0x30 + (4 * (n)))),  MEM,    32         /* Offset: 30h-4Ch */
#define FCFG0_7_FDIS                          31,   1               /* 31 FDIS (Fuse Array Disable) */
#define FCFG0_7_APBRT                         24,   7               /* 24-30 APBRT (APB Clock Rate) */
#define FCFG0_7_FCFGLK                        16,   8               /* 16-23 FCFGLK (FCFG Lock) */
#define FCFG0_7_FPRGDIS                       8,    8               /* 8-15 FPRGDIS (Fuse Program Disable) */
#define FCFG0_7_FRDLK                         0,    8               /* 0-7 FRDLK (Fuse Read Lock) */

/***********************************************************************************************************/
/*    Fuse Strap Register 1 (FUSTRAP1)                                                                     */
/***********************************************************************************************************/
#define FUSTRAP1                             (FUSE_BASE_ADDR + 0x20),  MEM,    32         /* Offset: 20h */
#define FUSTRAP1_JTAG_LOCK_CTL                30,   2               /* TIP_FUSE 20-21  */
#define FUSTRAP1_TIPnSPILOAD_POR_EN           29,   1               /* TIP_FUSE 23  */
#define FUSTRAP1_FIXED_KMT_OFF                28,   1               /* TIP_FUSE 22 */
#define FUSTRAP1_OWDEN                        27,   1               /* 27 OWDEN (Watchdog Enable) */
#define FUSTRAP1_OERR_HALT_EN                 26,   1               /* 26 OERR_HALT_EN (Z1 Security Enabled) */
#define FUSTRAP1_OAESKEYACCLK                 25,   1               /* 25 OAESKEYACCLK (AES Key Access Lock) */
#define FUSTRAP1_OJDIS                        24,   1               /* 24 OJDIS (JTAG Disable) */
#define FUSTRAP1_OSECBOOT                     23,   1               /* 23 OSECBOOT (Secure Boot) */
#define FUSTRAP1_USEFUSTRAP                   22,   1               /* 22 USEFUSTRAP (Use FUSTRAP) */
#define FUSTRAP1_TIP_PCIMBOX_DIS              21,   1               /* TIP_FUSE 19 */
#define FUSTRAP1_SEC_EV_ERR_REN               20,   1               /* TIP_FUSE 18 */
#define FUSTRAP1_EN_FL_INIT                   19,   1               /* TIP_FUSE 17 */
#define FUSTRAP1_BSPI10_DIS                   18,   1               /* TIP_FUSE 16 */
#define FUSTRAP1_OCPU1STOP1                   17,   1               /* 17 OCPU1STOP1 (CPU1 Stop 1) */
#define FUSTRAP1_OCPU1STOP2                   16,   1               /* 16 OCPU1STOP2 (CPU1 Stop 2) */
#define FUSTRAP1_oSPIX_EN                     15,   1               /* 15 oSPIX_EN (SPI-X logging enable) */
#define FUSTRAP1_OHINDDIS                     14,   1               /* 14 OHINDDIS (Host Independence Disable) */
#define FUSTRAP1_OCPU23STOP1                  12,   1               /* 12 OCPU23STOP1 (CPU2 and CPU 3 Stop 1) */
#define FUSTRAP1_OCPU23STOP2                  11,   1               /* 11 OCPU23STOP2 (CPU2 and CPU3 Stop 2) */
#define FUSTRAP1_oLMS_AUTH_EN                 10,   1               /* 10 oLMS_AUTH_EN whether LMS is enabled */
#define FUSTRAP1_oFAST_UART                   9 ,   1               /* 9  oFAST_UART whether which uart to choose, fast (750Kb\sec) or normal (115.2Kb/sec) */
#define FUSTRAP1_oCOP_NO_RESET                8 ,   1               /* 8  oCOP_NO_RESET when set COP will not reset with TIP while not compromizing secrets */
#define FUSTRAP1_oCOP_NO_RESET_DIS            7 ,   1               /* 7  permanently disable the oCOP_NO_RESET option selection */
#define FUSTRAP1_CKFRQ                        0,    2               /* 0-1 CKFRQ (FUSTRAP2-1: Clock Frequency) */

/***********************************************************************************************************/
/*    Fuse Strap Register 2 (FUSTRAP2)                                                                     */
/***********************************************************************************************************/
#define FUSTRAP2                             (FUSE_BASE_ADDR + 0x24),  MEM,    32         /* Offset: 24h */
#define FUSTRAP2_TIP_STRP15_1(n)              (17 + (n) - 1),   1    /* 17-31 TIP_STRP15-1 (Trusted Integrated Processor Straps 15-1) */
#define FUSTRAP2_OLOCKLK                      16,   1                /* 16 OLOCKLK (ROM Code Locks Last Key Register) */
#define FUSTRAP2_OPKI15_0                     0,    16               /* 0-15 OPKI15-0 (Public Key Invalidate 15-0) */
#define FUSTRAP2_ROM_RD_BLK                   17,    1               /* TIP_FUSE 1    */
#define FUSTRAP2_BSPI01_DIS                   18,    1               /* TIP_FUSE 2    */
#define FUSTRAP2_NON_RECOVERABLE_PQA          19,    1               /* TIP_FUSE 3    */
#define FUSTRAP2_TIP_PQA_STS                  20,    1               /* TIP_FUSE 4    */
#define FUSTRAP2_RND_DLY_EN                   21,    1               /* TIP_FUSE 5    */
#define FUSTRAP2_TIP_UART_DIS                 22,    1               /* TIP_FUSE 6    */
#define FUSTRAP2_TIPnSPILOAD_EN               23,    1               /* TIP_FUSE 7    */
#define FUSTRAP2_BMC_JDIS                     24,    1               /* TIP_FUSE 8    */
#define FUSTRAP2_TIP_JDIS                     25,    1               /* TIP_FUSE 9    */
#define FUSTRAP2_TIPWD_EN                     26,    1               /* TIP_FUSE 10   */
#define FUSTRAP2_FIPS                         27,    1               /* TIP_FUSE 11   */
#define FUSTRAP2_RAM_PAR_EN                   28,    1               /* TIP_FUSE 12   */
#define FUSTRAP2_ROM_PAR_EN                   29,    1               /* TIP_FUSE 13   */
#define FUSTRAP2_ROM_LOOP_FOREVER             31,    1               /* TIP_FUSE 15   */

/*---------------------------------------------------------------------------------------------------------*/
/* FKEYIND field values                                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define  FKEYIND_KSIZE_VALUE_128          0x4
#define  FKEYIND_KSIZE_VALUE_192          0x5
#define  FKEYIND_KSIZE_VALUE_256          0x6

#define FCFGn_BLOCK_MASK(block)                     (BUILD_FIELD_VAL(FCFG0_7_FRDLK, MASK_BIT((block) % 8))      |   \
                                                     BUILD_FIELD_VAL(FCFG0_7_FPRGDIS, MASK_BIT((block) % 8))    |   \
                                                     BUILD_FIELD_VAL(FCFG0_7_FCFGLK, MASK_BIT((block) % 8)))
#define FCFGn_W_BLOCK_MASK(block)                   (BUILD_FIELD_VAL(FCFG0_7_FPRGDIS, MASK_BIT((block) % 8)))
#define FCFGn_R_BLOCK_MASK(block)                   (BUILD_FIELD_VAL(FCFG0_7_FRDLK, MASK_BIT((block) % 8)))

#define TIP_FCFG0_7(n)                              FCFG0_7(n)
#define TIP_FCFG_BLOCK_IS_LOCKED(block)             ((REG_READ(TIP_FCFG0_7((block) / 8)) & FCFGn_BLOCK_MASK(block)) == FCFGn_BLOCK_MASK(block))
#define TIP_FCFG_BLOCK_IS_W_LOCKED(block)          ((REG_READ(TIP_FCFG0_7((block) / 8)) & FCFGn_W_BLOCK_MASK(block)) == FCFGn_W_BLOCK_MASK(block))
#define TIP_FCFG_BLOCK_IS_R_LOCKED(block)          ((REG_READ(TIP_FCFG0_7((block) / 8)) & FCFGn_R_BLOCK_MASK(block)) == FCFGn_R_BLOCK_MASK(block))


#endif /* FUSE_REGS_H */
