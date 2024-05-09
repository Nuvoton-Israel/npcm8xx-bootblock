/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   npcm850_regs.h                                                           */
/*            This file contains Chip Configuration registers                 */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/

#ifndef __NPCM850_REGS_H__
#define __NPCM850_REGS_H__


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                      Chip Configuration Registers                                       */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/***********************************************************************************************************/
/*    Product Identifier Register (PDID)                                                                   */
/***********************************************************************************************************/
#define PDID                                 (GCR_BASE_ADDR + 0x000),  MEM,    32         /* Offset: 000h */
#define PDID_VERSION                          24,   8               /* 24-31 VERSION (Tapeout Version) */
#define PDID_CHRID                            0,    16               /* 0-15 CHRID (Chip ID) */

/***********************************************************************************************************/
/*    Power-On Setting Register (PWRON)                                                                    */
/***********************************************************************************************************/
#define PWRON                                (GCR_BASE_ADDR + 0x004),  MEM,    32         /* Offset: 004h */
#define PWRON_SKIP_INIT_FAST                  13,   1               /* 13 STRAP14, fast skip init  */
#define PWRON_SPI1F18                         11,   1               /* 11 SPI1F18 (STRAP12: CLK_SPI1 Bus Powered by VSBV4 at 1.8V) */
#define PWRON_J3EN                            10,   1               /* 10 J3EN (STRAP11: JTAG3 Enable) */
#define PWRON_GP1                             9,    1               /* 9 GP1 (STRAP10: General Purpose 1) */
#define PWRON_FUP                             8,    1               /* 8 FUP (STRAP9: Flash UART Command Routine Enable) */
#define PWRON_SECEN                           7,    1               /* 7 SECEN (STRAP8: Security Enable) */
#define PWRON_HIZ                             6,    1               /* 6 HIZ (STRAP7: HI-Z State Control) */
#define PWRON_DMEP_KEY                        5,    1               /* 8 DMEP_KEY (STRAP6: DME Public Key Dump) */
#define PWRON_BSPA                            4,    1               /* 4 BSPA (STRAP5: BSP Alternate Pins) */
#define PWRON_J2EN                            3,    1               /* 3 J2EN (STRAP4: JTAG2 Enable) */
#define PWRON_SPI0F18                         2,    1               /* 2 SPI0F18 (STRAP3: CLK_SPI0 Bus Powered by VSBV3 at 1.8V) */
#define PWRON_CKFRQ                           0,    2               /* 0-1 CKFRQ (STRAP2-1: CPU System and DDR4 Memory Clock Frequency) */

/***********************************************************************************************************/
/*    Software Straps Register (SWSTRPS)                                                                   */
/***********************************************************************************************************/
#define SWSTRPS                              (GCR_BASE_ADDR + 0x008),  MEM,    32         /* Offset: 008h */
#define SWSTRPS_SWSTRP15                      15,   1               /* 15 SWSTRP15 (Software General Purpose Strap 15) */
#define SWSTRPS_SWSTRP14                      14,   1               /* 14 SWSTRP14 (Software General Purpose Strap 14) */
#define SWSTRPS_SWSTRP13                      13,   1               /* 13 SWSTRP13 (Software General Purpose Strap 13) */
#define SWSTRPS_SWSTRP12                      12,   1               /* 12 SWSTRP12 (Software General Purpose Strap 12) */
#define SWSTRPS_SWSTRP11                      11,   1               /* 11 SWSTRP11 (Software General Purpose Strap 11) */
#define SWSTRPS_SWSTRP10                      10,   1               /* 10 SWSTRP10 (Software General Purpose Strap 10) */
#define SWSTRPS_SWSTRP9                       9,    1               /* 9 SWSTRP9 (Software General Purpose Strap 9) */
#define SWSTRPS_SWSTRP8                       8,    1               /* 8 SWSTRP8 (Software General Purpose Strap 8) */
#define SWSTRPS_SWSTRP7                       7,    1               /* 7 SWSTRP7 (Software General Purpose Strap 7) */
#define SWSTRPS_SWSTRP6                       6,    1               /* 6 SWSTRP6 (Software General Purpose Strap 6) */
#define SWSTRPS_SWSTRP5                       5,    1               /* 5 SWSTRP5 (Software General Purpose Strap 5) */

/***********************************************************************************************************/
/*    Multiple Function Pin Select Register 1 (MFSEL1)                                                     */
/***********************************************************************************************************/
#define MFSEL1                               (GCR_BASE_ADDR + 0x260),  MEM,    32         /* Offset: 260h */
#define MFSEL1_SIRQSEL                        31,   1               /* 31 SIRQSEL (Serial IRQ Select) */
#define MFSEL1_IOX1SEL                        30,   1               /* 30 IOX1SEL (Serial GPIO Expander 1 Select) */
#define MFSEL1_HSI2BSEL                       29,   1               /* 29 HSI2BSEL (Host Serial Interface 2 Select B) */
#define MFSEL1_HSI1BSEL                       28,   1               /* 28 HSI1BSEL (Host Serial Interface 1 Select B) */
//#define MFSEL1_DVH1SEL^^^^^ ERROR: FIELD ALREADY APPEARS PREVIOUSLY ON THE SPEC (ON SAME REGISTER) 27,   1               /* 27 DVH1SEL^^^^^ ERROR: FIELD ALREADY APPEARS PREVIOUSLY ON THE SPEC (ON SAME REGISTER) (Digital Video Head 1 Select) */
#define MFSEL1_LPCSEL                         26,   1               /* 26 LPCSEL (LPC Select) */
#define MFSEL1_PECIB                          25,   1               /* 25 PECIB (PECI Bypass) */
#define MFSEL1_GSPISEL                        24,   1               /* 24 GSPISEL (Graphics SPI Select) */
#define MFSEL1_SMISEL                         22,   1               /* 22 SMISEL (SMI Select) */
#define MFSEL1_CLKOSEL                        21,   1               /* 21 CLKOSEL (Clockout Select) */
#define MFSEL1_KBCICSEL                       17,   1               /* 17 KBCICSEL (KBC Interface Controller Select) */
#define MFSEL1_R2MDSEL                        16,   1               /* 16 R2MDSEL (RMII2 MDIO Select) */
#define MFSEL1_R2ERRSEL                       15,   1               /* 15 R2ERRSEL (RMII2 R2RXERR Select) */
#define MFSEL1_RMII2SEL                       14,   1               /* 14 RMII2SEL (RMII2 Select) */
#define MFSEL1_R1MDSEL                        13,   1               /* 13 R1MDSEL (RMII1 MDIO Select) */
#define MFSEL1_R1ERRSEL                       12,   1               /* 12 R1ERRSEL (RMII1 R1RXERR Select) */
#define MFSEL1_HSI2ASEL                       11,   1               /* 11 HSI2ASEL (Host Serial Interface 2 Select A) */
#define MFSEL1_HSI1ASEL                       10,   1               /* 10 HSI1ASEL (Host Serial Interface 1 Select A) */
#define MFSEL1_BSPSEL                         9,    1               /* 9 BSPSEL (BMC Serial Port Select) */
#define MFSEL1_SMB2SEL                        8,    1               /* 8 SMB2SEL (SMB2 Select) */
#define MFSEL1_SMB1SEL                        7,    1               /* 7 SMB1SEL (SMB1 Select) */
#define MFSEL1_SMB0SEL                        6,    1               /* 6 SMB0SEL (SMB0 Select) */
#define MFSEL1_HSI2CSEL                       5,    1               /* 5 HSI2CSEL (Host Serial Interface 2 Select C) */
#define MFSEL1_HSI1CSEL                       4,    1               /* 4 HSI1CSEL (Host Serial Interface 1 Select C) */
#define MFSEL1_S0CS1SEL                       3,    1               /* 3 S0CS1SEL (SPI0CS0 Select) */
#define MFSEL1_SMB5SEL                        2,    1               /* 2 SMB5SEL (SMBus 5 Select) */
#define MFSEL1_SMB4SEL                        1,    1               /* 1 SMB4SEL (SMBus 4 Select) */
#define MFSEL1_SMB3SEL                        0,    1               /* 0 SMB3SEL (SMBus 3 Select) */

/***********************************************************************************************************/
/*    Multiple Function Pin Select Register 2 (MFSEL2)                                                     */
/***********************************************************************************************************/
#define MFSEL2                               (GCR_BASE_ADDR + 0x264),  MEM,    32         /* Offset: 264h */
#define MFSEL2_HG7SEL                         31,   1               /* 31 HG7SEL (Host GPIO7 Select) */
#define MFSEL2_HG6SEL                         30,   1               /* 30 HG6SEL (Host GPIO6 Select) */
#define MFSEL2_HG5SEL                         29,   1               /* 29 HG5SEL (Host GPIO5 Select) */
#define MFSEL2_HG4SEL                         28,   1               /* 28 HG4SEL (Host GPIO4 Select) */
#define MFSEL2_HG3SEL                         27,   1               /* 27 HG3SEL (Host GPIO3 Select) */
#define MFSEL2_HG2SEL                         26,   1               /* 26 HG2SEL (Host GPIO2 Select) */
#define MFSEL2_HG1SEL                         25,   1               /* 25 HG1SEL (Host GPIO1 Select) */
#define MFSEL2_HG0SEL                         24,   1               /* 24 HG0SEL (Host GPIO0 Select) */
#define MFSEL2_PWM7SEL                        23,   1               /* 23 PWM7SEL (PWM7 Select) */
#define MFSEL2_PWM6SEL                        22,   1               /* 22 PWM6SEL (PWM6 Select) */
#define MFSEL2_PWM5SEL                        21,   1               /* 21 PWM5SEL (PWM5 Select) */
#define MFSEL2_PWM4SEL                        20,   1               /* 20 PWM4SEL (PWM4 Select) */
#define MFSEL2_PWM3SEL                        19,   1               /* 19 PWM3SEL (PWM3 Select) */
#define MFSEL2_PWM2SEL                        18,   1               /* 18 PWM2SEL (PWM2 Select) */
#define MFSEL2_PWM1SEL                        17,   1               /* 17 PWM1SEL (PWM1 Select) */
#define MFSEL2_PWM0SEL                        16,   1               /* 16 PWM0SEL (PWM0 Select) */
#define MFSEL2_FI15SEL                        15,   1               /* 15 FI15SEL (FANIN15 Select) */
#define MFSEL2_FI14SEL                        14,   1               /* 14 FI14SEL (FANIN14 Select) */
#define MFSEL2_FI13SEL                        13,   1               /* 13 FI13SEL (FANIN13 Select) */
#define MFSEL2_FI12SEL                        12,   1               /* 12 FI12SEL (FANIN12 Select) */
#define MFSEL2_FI11SEL                        11,   1               /* 11 FI11SEL (FANIN11 Select) */
#define MFSEL2_FI10SEL                        10,   1               /* 10 FI10SEL (FANIN10 Select) */
#define MFSEL2_FI9SEL                         9,    1               /* 9 FI9SEL (FANIN9 Select) */
#define MFSEL2_FI8SEL                         8,    1               /* 8 FI8SEL (FANIN8 Select) */
#define MFSEL2_FI7SEL                         7,    1               /* 7 FI7SEL (FANIN7 Select) */
#define MFSEL2_FI6SEL                         6,    1               /* 6 FI6SEL (FANIN6 Select) */
#define MFSEL2_FI5SEL                         5,    1               /* 5 FI5SEL (FANIN5 Select) */
#define MFSEL2_FI4SEL                         4,    1               /* 4 FI4SEL (FANIN4 Select) */
#define MFSEL2_FI3SEL                         3,    1               /* 3 FI3SEL (FANIN3 Select) */
#define MFSEL2_FI2SEL                         2,    1               /* 2 FI2SEL (FANIN2 Select) */
#define MFSEL2_FI1SEL                         1,    1               /* 1 FI1SEL (FANIN1 Select) */
#define MFSEL2_FI0SEL                         0,    1               /* 0 FI0SEL (FANIN0 Select) */

/***********************************************************************************************************/
/*    Multiple Function Pin Select Register 3 (MFSEL3)                                                     */
/***********************************************************************************************************/
#define MFSEL3                               (GCR_BASE_ADDR + 0x268),  MEM,    32         /* Offset: 268h */
#define MFSEL3_MMCCDSEL                       25,   1               /* 25 MMCCDSEL (nMMCCD Select) */
#define MFSEL3_BU1SEL                         24,   1               /* 24 BU1SEL (BMC UART1 Select) */
#define MFSEL3_I3C5SEL                        22,   1               /* 22 I3C5SEL (I3C 5 Select) */
#define MFSEL3_WDO2SEL                        20,   1               /* 20 WDO2SEL (nWDO2 Select) */
#define MFSEL3_WDO1SEL                        19,   1               /* 19 WDO1SEL (nWDO1 Select) */
#define MFSEL3_IOXHSEL                        18,   1               /* 18 IOXHSEL (Host Serial I/O Expander Select) */
#define MFSEL3_PCIEPUSE                       17,   1               /* 17 PCIEPUSE (PCI Express PHY Usage) */
#define MFSEL3_CLKRUNSEL                      16,   1               /* 16 CLKRUNSEL (CLKRUN Select) */
#define MFSEL3_IOX2SEL                        14,   1               /* 14 IOX2SEL (I/O Expander 2 Select) */
#define MFSEL3_PSPI2SEL                       13,   1               /* 13 PSPI2SEL (PSPI2 Select) */
#define MFSEL3_MMC8SEL                        11,   1               /* 11 MMC8SEL (MMC Select) */
#define MFSEL3_MMCSEL                         10,   1               /* 10 MMCSEL (MMC Select) */
#define MFSEL3_RMII1SEL                       9,    1               /* 9 RMII1SEL (RMII1 Select) */
#define MFSEL3_SMB15SEL                       8,    1               /* 8 SMB15SEL (SMB15 Select) */
#define MFSEL3_SMB14SEL                       7,    1               /* 7 SMB14SEL (SMB14 Select) */
#define MFSEL3_SMB13SEL                       6,    1               /* 6 SMB13SEL (SMB13 Select) */
#define MFSEL3_SMB12SEL                       5,    1               /* 5 SMB12SEL (SMB12 Select) */
#define MFSEL3_SPI1SEL                        4,    1               /* 4 SPI1SEL (PSPI1 Select) */
#define MFSEL3_FIN1916SEL                     3,    1               /* 3 FIN1916SEL (FANIN19 to FANIN16 Select) */
#define MFSEL3_SMB7SEL                        2,    1               /* 2 SMB7SEL (SMB7 Select) */
#define MFSEL3_SMB6SEL                        1,    1               /* 1 SMB6SEL (SMB6 Select) */
#define MFSEL3_SCISEL                         0,    1               /* 0 SCISEL (SCI Select) */

/***********************************************************************************************************/
/*    Multiple Function Pin Select Register 4 (MFSEL4)                                                     */
/***********************************************************************************************************/
#define MFSEL4                               (GCR_BASE_ADDR + 0x26C),  MEM,    32         /* Offset: 26Ch */
#define MFSEL4_JTAG2EN                        31,   1               /* 31 JTAG2EN (Coprocessor Debug Port Enable) */
#define MFSEL4_SXCS1SEL                       28,   1               /* 28 SXCS1SEL (SPIX Chip Select 1 Select) */
#define MFSEL4_SPXSEL                         27,   1               /* 27 SPXSEL (SPIX Select) */
#define MFSEL4_RG2SEL                         24,   1               /* 24 RG2SEL (RGMII 2 Select) */
#define MFSEL4_RG2MSEL                        23,   1               /* 23 RG2MSEL (RGMII 2 MDIO Select) */
#define MFSEL4_BU2SELB                        22,   1               /* 22 BU2SELB (BMC UART 2 Option B Select) */
#define MFSEL4_SG1MSEL                        21,   1               /* 21 SG1MSEL (SGMII 1 MDIO Select) */
#define MFSEL4_SP3QSEL                        20,   1               /* 20 SP3QSEL (CLK_SPI3 Quad Select) */
#define MFSEL4_S3CS3SEL                       19,   1               /* 19 S3CS3SEL (SPI3CS3 Select) */
#define MFSEL4_S3CS2SEL                       18,   1               /* 18 S3CS2SEL (SPI3CS2 Select) */
#define MFSEL4_S3CS1SEL                       17,   1               /* 17 S3CS1SEL (SPI3CS1 Select) */
#define MFSEL4_SP3SEL                         16,   1               /* 16 SP3SEL (CLK_SPI3 Select) */
#define MFSEL4_SMB11SEL                       14,   1               /* 14 SMB11SEL (SMB11 Select) */
#define MFSEL4_SMB10SEL                       13,   1               /* 13 SMB10SEL (SMB10 Select) */
#define MFSEL4_SMB9SEL                        12,   1               /* 12 SMB9SEL (SMB9 Select) */
#define MFSEL4_SMB8SEL                        11,   1               /* 11 SMB8SEL (SMB8 Select) */
#define MFSEL4_CKRQSEL                        9,    1               /* 9 CKRQSEL (nCKRQ Signal Select) */
#define MFSEL4_ESPISEL                        8,    1               /* 8 ESPISEL (eSPI Signal Select) */
#define MFSEL4_MMCRSEL                        6,    1               /* 6 MMCRSEL (MMC Reset Control Select) */
#define MFSEL4_ROSEL                          4,    1               /* 4 ROSEL (Ring Oscillator Output Select) */
#define MFSEL4_ESPIPMESEL                     2,    2               /* 2-3 ESPIPMESEL (ESPI PME Connection Select) */
#define MFSEL4_BSPASEL                        1,    1               /* 1 BSPASEL (BMC Serial Port Alternate Port Select) */
#define MFSEL4_JTAG2SEL                       0,    1               /* 0 JTAG2SEL (Coprocessor Debug Port Select) */

/***********************************************************************************************************/
/*    Multiple Function Pin Select Register 5 (MFSEL5)                                                     */
/***********************************************************************************************************/
#define MFSEL5                               (GCR_BASE_ADDR + 0x270),  MEM,    32         /* Offset: 270h */
#define MFSEL5_SMB23SEL                       31,   1               /* 31 SMB23SEL (SMB23 Select) */
#define MFSEL5_SMB22SEL                       30,   1               /* 30 SMB22SEL (SMB22 Select) */
#define MFSEL5_SMB21SEL                       29,   1               /* 29 SMB21SEL (SMB21 Select) */
#define MFSEL5_SMB20SEL                       28,   1               /* 28 SMB20SEL (SMB20 Select) */
#define MFSEL5_SMB19SEL                       27,   1               /* 27 SMB19SEL (SMB19 Select) */
#define MFSEL5_SMB18SEL                       26,   1               /* 26 SMB18SEL (SMB18 Select) */
#define MFSEL5_SMB17SEL                       25,   1               /* 25 SMB17SEL (SMB17 Select) */
#define MFSEL5_SMB16SEL                       24,   1               /* 24 SMB16SEL (SMB16 Select) */
#define MFSEL5_I3C3SEL                        23,   1               /* 23 I3C3SEL (I3C3 Select) */
#define MFSEL5_TPGPIO5SELB                    22,   1               /* 22 TPGPIO5SELB (TIP GPIO5 Select Option B) */
#define MFSEL5_I3C2SEL                        21,   1               /* 21 I3C2SEL (I3C2 Select) */
#define MFSEL5_TPGPIO4SELB                    20,   1               /* 20 TPGPIO4SELB (TIP GPIO4 Select Option B) */
#define MFSEL5_I3C1SEL                        19,   1               /* 19 I3C1SEL (I3C1 Select) */
#define MFSEL5_GPI36SEL                       18,   1               /* 18 GPI36SEL (GPI36 Select) */
#define MFSEL5_I3C0SEL                        17,   1               /* 17 I3C0SEL (I3C0 Select) */
#define MFSEL5_GPI35SEL                       16,   1               /* 16 GPI35SEL (GPI35 Select) */
#define MFSEL5_JM1SEL                         15,   1               /* 15 JM1SEL (JTAG MASTER 1 Select) */
#define MFSEL5_R3OENSEL                       14,   1               /* 14 R3OENSEL (RMII Output Enable 3 Select) */
#define MFSEL5_BU4SELB                        13,   1               /* 13 BU4SELB (BMC UART 4 Select Option B) */
#define MFSEL5_BU5SELB                        12,   1               /* 12 BU5SELB (BMC UART 5 Select Option B) */
#define MFSEL5_RMII3SEL                       11,   1               /* 11 RMII3SEL (RMII 3 Select) */
#define MFSEL5_R2OENSEL                       10,   1               /* 10 R2OENSEL (RMII Output Enable 2 Select) */
#define MFSEL5_R1OENSEL                       9,    1               /* 9 R1OENSEL (RMII Output Enable 1 Select) */
#define MFSEL5_BU4SEL                         8,    1               /* 8 BU4SEL (BMC UART 4 Select) */
#define MFSEL5_BU5SEL                         7,    1               /* 7 BU5SEL (BMC UART 5 Select) */
#define MFSEL5_BU6SEL                         6,    1               /* 6 BU6SEL (BMC UART 6 Select) */
#define MFSEL5_NSPI1CS3SEL                    5,    1               /* 5 NSPI1CS3SEL (SPI1 Chip-Select 3 Select) */
#define MFSEL5_NSPI1CS2SEL                    4,    1               /* 4 NSPI1CS2SEL (SPI1 Chip-Select 2 Select) */
#define MFSEL5_SPI1D23SEL                     3,    1               /* 3 SPI1D23SEL (SPI1 Data lines 2-3 Select) */
#define MFSEL5_J2J3SEL                        2,    1               /* 2 J2J3SEL (JTAG2 and JTAG3 Slaves Select) */
#define MFSEL5_JM2SEL                         1,    1               /* 1 JM2SEL (JTAG Master 2 Select) */
#define MFSEL5_NSPI1CS1SEL                    0,    1               /* 0 NSPI1CS1SEL (SPI1 Chip Select 1 Select) */

/***********************************************************************************************************/
/*    Multiple Function Pin Select Register 6 (MFSEL6)                                                     */
/***********************************************************************************************************/
#define MFSEL6                               (GCR_BASE_ADDR + 0x274),  MEM,    32         /* Offset: 274h */
#define MFSEL6_CP1URXDSEL                     31,   1               /* 31 CP1URXDSEL (Coprocessor 1 UART RXD Select) */
#define MFSEL6_R3RXERSEL                      30,   1               /* 30 R3RXERSEL (RMII3 RXERR Select) */
#define MFSEL6_CP1GPIO2SELC                   29,   1               /* 29 CP1GPIO2SELC (Coprocessor 1 GPIO2 Option C Select) */
#define MFSEL6_CP1GPIO3SELC                   28,   1               /* 28 CP1GPIO3SELC (Coprocessor 1 GPIO3 Option C Select) */
#define MFSEL6_CP1GPIO4SELB                   27,   1               /* 27 CP1GPIO4SELB (Coprocessor 1 GPIO4 Option B Select) */
#define MFSEL6_CP1GPIO5SELB                   26,   1               /* 26 CP1GPIO5SELB (Coprocessor 1 GPIO5 Option B Select) */
#define MFSEL6_CP1GPIO6SELB                   25,   1               /* 25 CP1GPIO6SELB (Coprocessor 1 GPIO6 Option B Select) */
#define MFSEL6_CP1GPIO7SELB                   24,   1               /* 24 CP1GPIO7SELB (Coprocessor 1 GPIO7 Option B Select) */
#define MFSEL6_CP1GPIO3SELB                   23,   1               /* 23 CP1GPIO3SELB (Coprocessor 1 GPIO3 Option B Select) */
#define MFSEL6_CP1GPIO2SELB                   22,   1               /* 22 CP1GPIO2SELB (Coprocessor 1 GPIO2 Option B Select) */
#define MFSEL6_CP1GPIO1SELB                   21,   1               /* 21 CP1GPIO1SELB (Coprocessor 1 GPIO1 Option B Select) */
#define MFSEL6_CP1GPIO0SELB                   20,   1               /* 20 CP1GPIO0SELB (Coprocessor 1 GPIO0 Option B Select) */
#define MFSEL6_GPIO1836SEL                    19,   1               /* 19 GPIO1836SEL (GPIO183-186 Select) */
#define MFSEL6_FM2SEL                         18,   1               /* 18 FM2SEL (FLM2 Interface Select) */
#define MFSEL6_FM1SEL                         17,   1               /* 17 FM1SEL (FLM1 Interface Select) */
#define MFSEL6_FM0SEL                         16,   1               /* 16 FM0SEL (FLM0 Interface Select) */
#define MFSEL6_NBU1CRTSSEL                    15,   1               /* 15 NBU1CRTSSEL (BMC UART1 Flow Control Select) */
#define MFSEL6_PWM11SEL                       14,   1               /* 14 PWM11SEL (PWM11 Select) */
#define MFSEL6_PWM10SEL                       13,   1               /* 13 PWM10SEL (PWM10 Select) */
#define MFSEL6_PWM9SEL                        12,   1               /* 12 PWM9SEL (PWM9 Select) */
#define MFSEL6_PWM8SEL                        11,   1               /* 11 PWM8SEL (PWM8 Select) */
#define MFSEL6_I3C4SEL                        10,   1               /* 10 I3C4SEL (I3C4 Select) */
#define MFSEL6_CP1GPIO7SEL                    9,    1               /* 9 CP1GPIO7SEL (Coprocessor 1 GPIO7 Select) */
#define MFSEL6_CP1GPIO6SEL                    8,    1               /* 8 CP1GPIO6SEL (Coprocessor 1 GPIO6 Select) */
#define MFSEL6_CP1GPIO5SEL                    7,    1               /* 7 CP1GPIO5SEL (Coprocessor 1 GPIO5 Select) */
#define MFSEL6_CP1GPIO4SEL                    6,    1               /* 6 CP1GPIO4SEL (Coprocessor 1 GPIO4 Select) */
#define MFSEL6_CP1GPIO3SEL                    5,    1               /* 5 CP1GPIO3SEL (Coprocessor 1 GPIO3 Select) */
#define MFSEL6_CP1GPIO2SEL                    4,    1               /* 4 CP1GPIO2SEL (Coprocessor 1 GPIO2 Select) */
#define MFSEL6_CP1GPIO1SEL                    3,    1               /* 3 CP1GPIO1SEL (Coprocessor 1 GPIO1 Select) */
#define MFSEL6_CP1GPIO0SEL                    2,    1               /* 2 CP1GPIO0SEL (Coprocessor 1 GPIO0 Select) */
#define MFSEL6_CP1UTXDSEL                     1,    1               /* 1 CP1UTXDSEL (Coprocessor 1 UART TXD Select) */
#define MFSEL6_SMB23BSEL                      0,    1               /* 0 SMB23BSEL (SMB23 Option B Select) */

/***********************************************************************************************************/
/*    Multiple Function Pin Select Register 7 (MFSEL7)                                                     */
/***********************************************************************************************************/
#define MFSEL7                               (GCR_BASE_ADDR + 0x278),  MEM,    32         /* Offset: 278h */
#define MFSEL7_SMB16SELB                      30,   1               /* 30 SMB16SELB (SMB 16 Option B Select) */
#define MFSEL7_VGADIGSEL                      29,   1               /* 29 VGADIGSEL (VGA Digital Select) */
#define MFSEL7_TPSMB2SEL                      28,   1               /* 28 TPSMB2SEL (TIP SMB 2 Select) */
#define MFSEL7_SMB15SELB                      27,   1               /* 27 SMB15SELB (SMB 15 Option B Select) */
#define MFSEL7_SMB14SELB                      26,   1               /* 26 SMB14SELB (SMB 14 Option B Select) */
#define MFSEL7_GPIO1889SEL                    25,   1               /* 25 GPIO1889SEL (GPIO188-9 Select) */
#define MFSEL7_GPIO187SEL                     24,   1               /* 24 GPIO187SEL (GPIO187 Select) */
#define MFSEL7_FANIN19SELB                    23,   1               /* 23 FANIN19SELB (FANIN19 Option B Select) */
#define MFSEL7_FANIN18SELB                    22,   1               /* 22 FANIN18SELB (FANIN18 Option B Select) */
#define MFSEL7_FANIN17SELB                    21,   1               /* 21 FANIN17SELB (FANIN17 Option B Select) */
#define MFSEL7_FANIN16SELB                    20,   1               /* 20 FANIN16SELB (FANIN16 Option B Select) */
#define MFSEL7_SMB18SELB                      15,   1               /* 15 SMB18SELB (SMB 18 Option B Select) */
#define MFSEL7_SMB17SELB                      14,   1               /* 14 SMB17SELB (SMB 17 Option B Select) */
#define MFSEL7_TPJTAG3SEL                     13,   1               /* 13 TPJTAG3SEL (TIP JTAG 3 Select) */
#define MFSEL7_TPUARTSEL                      12,   1               /* 12 TPUARTSEL (TIP UART Select) */
#define MFSEL7_TPSMB1SEL                      11,   1               /* 11 TPSMB1SEL (TIP SMB 1 Select) */
#define MFSEL7_TPGPIO2BSEL                    10,   1               /* 10 TPGPIO2BSEL (TIP GPIO2 Option B Select) */
#define MFSEL7_TPGPIO1BSEL                    9,    1               /* 9 TPGPIO1BSEL (TIP GPIO1 Option B Select) */
#define MFSEL7_TPGPIO0BSEL                    8,    1               /* 8 TPGPIO0BSEL (TIP GPIO0 Option B Select) */
#define MFSEL7_TPGPIO7SEL                     7,    1               /* 7 TPGPIO7SEL (TIP GPIO7 Select) */
#define MFSEL7_TPGPIO6SEL                     6,    1               /* 6 TPGPIO6SEL (TIP GPIO6 Select) */
#define MFSEL7_TPGPIO5SEL                     5,    1               /* 5 TPGPIO5SEL (TIP GPIO5 Select) */
#define MFSEL7_TPGPIO4SEL                     4,    1               /* 4 TPGPIO4SEL (TIP GPIO4 Select) */
#define MFSEL7_TPGPIO3SEL                     3,    1               /* 3 TPGPIO3SEL (TIP GPIO3 Select) */
#define MFSEL7_TPGPIO2SEL                     2,    1               /* 2 TPGPIO2SEL (TIP GPIO2 Select) */
#define MFSEL7_TPGPIO1SEL                     1,    1               /* 1 TPGPIO1SEL (TIP GPIO1 Select) */
#define MFSEL7_TPGPIO0SEL                     0,    1               /* 0 TPGPIO0SEL (TIP GPIO0 Select) */

/***********************************************************************************************************/
/*    Multiple Function Selection Lock Registers 1-7 (MFSEL_LK1-7)                                         */
/***********************************************************************************************************/
#define MFSEL_LK1_7(n)                       (GCR_BASE_ADDR + (0x280 + (4 * ((n)-1)))),  MEM,    32         /* Offset: 280h-298h */
#define MFSEL_LK1_7_LOCK_INDV_BITS            0,    32               /* 0-31 LOCK_INDV_BITS (Lock Individual Bits) */

/***********************************************************************************************************/
/*    Multiple Function Selection Set Registers 1-7 (MFSEL_SET1-7)                                         */
/***********************************************************************************************************/
#define MFSEL_SET1_7(n)                      (GCR_BASE_ADDR + (0x2A0 + (4 * ((n)-1)))),  MEM,    32         /* Offset: 2A0h-2B8h */
#define MFSEL_SET1_7_SET_INDV_BITS            0,    32               /* 0-31 SET_INDV_BITS (Set Individual Bits) */

/***********************************************************************************************************/
/*    Multiple Function Selection Clear Registers 1-7 (MFSEL_CLR1-7)                                       */
/***********************************************************************************************************/
#define MFSEL_CLR1_7(n)                      (GCR_BASE_ADDR + (0x2C0 + (4 * ((n)-1)))),  MEM,    32         /* Offset: 2C0h-2D8h */
#define MFSEL_CLR1_7_CLR_INDV_BITS            0,    32               /* 0-31 CLR_INDV_BITS (Clear Individual Bits) */

/***********************************************************************************************************/
/*    Miscellaneous Pin Pull-Up/Down Enable Register (MISCPE)                                              */
/***********************************************************************************************************/
#define MISCPE                               (GCR_BASE_ADDR + 0x014),  MEM,    32         /* Offset: 014h */
#define MISCPE_SPI0                           1,    1               /* 1 CLK_SPI0 (CLK_SPI0 Data 3-0) */
#define MISCPE_PWRGD_PS                       0,    1               /* 0 PWRGD_PS (Pull-Down Enable for PWRGD_PS) */

/***********************************************************************************************************/
/*    SPILOAD Control Register (SPLDCNT)                                                                   */
/***********************************************************************************************************/
#define SPLDCNT                              (GCR_BASE_ADDR + 0x018),  MEM,    32         /* Offset: 018h */
#define SPLDCNT_SPLDDLK                       4,    1               /* 4 SPLDDLK (SPLDCNT Lock) */
#define SPLDCNT_SPILDSTDL                     2,    2               /* 2-3 SPILDSTDL (SPILOAD Start Delay) */
#define SPLDCNT_SPILDWDTH                     0,    2               /* 0-1 SPILDWDTH (SPILOAD Pulse Width) */

/***********************************************************************************************************/
/*    A35 Mode Register (A35_MODE)                                                                         */
/***********************************************************************************************************/
#define A35_MODE                             (GCR_BASE_ADDR + 0x034),  MEM,    32         /* Offset: 018h */
#define A35_MODE_AA64BA                       8,    24               /* 8-31 AA64BA (ARM Arch64 Boot Address) */
#define A35_MODE_AA64B_AA32                   5,    1               /* 5 AA64B_AA32 (ARM Arch64 or ARM Arch32 Selection) */
#define A35_MODE_AA32BAS                      4,    1               /* 4 AA32BAS (ARM Arch32 Boot Address Select) */
#define A35_MODE_LCK                          0,    1               /* 0 LCK (Lock Register) */

/***********************************************************************************************************/
/*    Serial Ports Switch Control Register (SPSWC)                                                         */
/***********************************************************************************************************/
#define SPSWC                                (GCR_BASE_ADDR + 0x038),  MEM,    32         /* Offset: 038h */
#define SPSWC_RTSS                            7,    1               /* 7 RTSS (RTS Setting) */
#define SPSWC_DTRS                            6,    1               /* 6 DTRS (DTR Setting) */
#define SPSWC_DCDI                            5,    1               /* 5 DCDI (Serial Interface 2 DCD input) */
#define SPSWC_SPMOD                           0,    3               /* 0-2 SPMOD (Serial Port Mode) */

/***********************************************************************************************************/
/*    Integration Control Register (INTCR)                                                                 */
/***********************************************************************************************************/
#define INTCR                                (GCR_BASE_ADDR + 0x03C),  MEM,    32         /* Offset: 03Ch */
#define INTCR_DUDKSMOD                        30,   2               /* 30-31 DUDKSMOD (Display update during KVM-scan Mode) */
#define INTCR_DDC3I                           29,   1               /* 29 DDC3I (Spare Graphics Control) */
#define INTCR_DDC2I                           28,   1               /* 28 DDC2I (default) */
#define INTCR_GGPCT2_0                        24,   3               /* 24-26 GGPCT2-0 (Graphics General Purpose Control 2-0) */
#define INTCR_SGC2                            23,   1               /* 23 SGC2 (Spare Graphics Control 2) */
#define INTCR_DAC_SNS                         20,   1               /* 20 DAC_SNS (DAC Sense) */
#define INTCR_SGC1                            19,   1               /* 19 SGC1 (Spare Graphics Control 1) */
#define INTCR_DUDKSEN                         16,   1               /* 16 DUDKSEN (Display Update During KVM-Scan Enable) */
#define INTCR_H2RQDIS                         9,    1               /* 9 H2RQDIS (Head 2 Request Disable) */
#define INTCR_H2DISPOFF                       8,    1               /* 8 H2DISPOFF (Head 2 Display Off) */
#define INTCR_R1EN                            5,    1               /* 5 R1EN (Enable RMII Outputs) */
#define INTCR_PSPIFEN                         4,    1               /* 4 PSPIFEN (PSPI Freeze Enable) */
#define INTCR_HIFEN                           3,    1               /* 3 HIFEN (Host Interface Freeze Enable) */
#define INTCR_SMBFEN                          2,    1               /* 2 SMBFEN (SMB Freeze Enable) */
#define INTCR_MFTFEN                          1,    1               /* 1 MFTFEN (MFT Freeze Enable) */
#define INTCR_KCSRST_MODE                     0,    1               /* 0 KCSRST_MODE (Host Interface Modules Reset Mode Select) */

/***********************************************************************************************************/
/*    Integration Status Register 2 (INTSR2)                                                               */
/***********************************************************************************************************/
#define INTSR2                               (GCR_BASE_ADDR + 0x014C),  MEM,    32         /* Offset: 014Ch */
#define INTSR2_USBHOST2ST                     16,   14               /* 16-29 USBHOST2ST (USB HOST2 Status) */

/***********************************************************************************************************/
/*    Observability Control Register 1 (OBSCR1)                                                            */
/***********************************************************************************************************/
#define OBSCR1                               (GCR_BASE_ADDR + 0x044),  MEM,    32         /* Offset: 044h */
#define OBSCR1_OBSPINMSK                      16,   16               /* 16-31 OBSPINMSK (Observability Pin Mask) */
#define OBSCR1_OBSL0                          12,   4               /* 12-15 OBSL0 (Observability Level 0) */
#define OBSCR1_OBSL1                          8,    4               /* 8-11 OBSL1 (Observability Level 1) */
#define OBSCR1_OBSDSEL                        4,    4               /* 4-7 OBSDSEL (Observability Data Select) */
#define OBSCR1_ROTATE                         1,    3               /* 1-3 ROTATE (Rotate) */
#define OBSCR1_OBSEN                          0,    1               /* 0 OBSEN (Observe Enable) */

/***********************************************************************************************************/
/*    Observability Control Register 2 (OBSCR2)                                                            */
/***********************************************************************************************************/
#define OBSCR2                               (GCR_BASE_ADDR + 0x0C4),  MEM,    32         /* Offset: 0C4h */
#define OBSCR2_OBSHEN                         0,    1               /* 0 OBSHEN (Observability on HSTL pins Enable) */

/***********************************************************************************************************/
/*    Observability Data Register 1-2 (OBSDR1-2)                                                           */
/***********************************************************************************************************/
#define OBSDR1_2(n)                          (GCR_BASE_ADDR(m) + (0x048 + (128 * n))),  MEM,    32         /* Offset: 048h-0C8h */
#define OBSDR1_2_OBSDATA                      0,    16               /* 0-15 OBSDATA (Observability Data) */

/***********************************************************************************************************/
/*    Host Interface Control Register (HIFCR)                                                              */
/***********************************************************************************************************/
#define HIFCR                                (GCR_BASE_ADDR + 0x050),  MEM,    32         /* Offset: 050h */
#define HIFCR_LPC_ADDR                        0,    16               /* 0-15 LPC_ADDR (LPC Address) */

/***********************************************************************************************************/
/*    SDHC2 Interface Reset Value Setup Register 1 (SD2SUR1)                                               */
/***********************************************************************************************************/
#define SD2SUR1                              (GCR_BASE_ADDR + 0xB4),  MEM,    32         /* Offset: B4h */
#define SD2SUR1_CORECFG_SUSPRESSUPPORT        31,   1               /* 31 CORECFG_SUSPRESSUPPORT (Core Suspend/Resume Support) */
#define SD2SUR1_CORECFG_SDMASUPPORT           30,   1               /* 30 CORECFG_SDMASUPPORT (SDMA Support) */
#define SD2SUR1_CORECFG_HIGHSPEEDSUPPORT      29,   1               /* 29 CORECFG_HIGHSPEEDSUPPORT (High Speed Support) */
#define SD2SUR1_CORECFG_ADMA2SUPPORT          28,   1               /* 28 CORECFG_ADMA2SUPPORT (ADMA 2 Support) */
#define SD2SUR1_CORECFG_8BITSUPPORT           27,   1               /* 27 CORECFG_8BITSUPPORT (8-Bit Support) */
#define SD2SUR1_CORECFG_MAXBLKLENGTH          25,   2               /* 25-26 CORECFG_MAXBLKLENGTH (Maximum Block Length) */
#define SD2SUR1_CORECFG_TIMEOUTCLKUNIT        24,   1               /* 24 CORECFG_TIMEOUTCLKUNIT (Timeout Clock Unit) */
#define SD2SUR1_CORECFG_TIMEOUTCLKFREQ        19,   5               /* 19-23 CORECFG_TIMEOUTCLKFREQ (Timeout Clock Frequency) */
#define SD2SUR1_CORECFG_TUNINGCOUNT           13,   6               /* 13-18 CORECFG_TUNINGCOUNT (Tuning Count) */
#define SD2SUR1_TEST_MODE                     12,   1               /* 12 TEST_MODE (Test Mode) */
#define SD2SUR1_CORECTRL_OTAPDLYSEL_OTD       8,    4               /* 8-11 CORECTRL_OTAPDLYSEL OTD (Output Tap Delay) */
#define SD2SUR1_CORECTRL_OTAPDLYENA_OTDE      7,    1               /* 7 CORECTRL_OTAPDLYENA OTDE (Output Tap Delay Enable) */
#define SD2SUR1_CORECTRL_ITAPCHGWIN           6,    1               /* 6 CORECTRL_ITAPCHGWIN (Output Tap Delay Gate) */
#define SD2SUR1_CORECTRL_ITAPDLYSEL_ITD       1,    5               /* 1-5 CORECTRL_ITAPDLYSEL ITD (Input Tap Delay) */
#define SD2SUR1_CORECTRL_ITAPDLYENA_ITDE      0,    1               /* 0 CORECTRL_ITAPDLYENA ITDE (Input Tap Delay Enable) */

/***********************************************************************************************************/
/*    SDHC2 Interface Reset Value Setup Register 2 (SD2SUR2)                                               */
/***********************************************************************************************************/
#define SD2SUR2                              (GCR_BASE_ADDR + 0xB8),  MEM,    32         /* Offset: B8h */
#define SD2SUR2_CORECFG_ASYNCWKUPENA          30,   1               /* 30 CORECFG_ASYNCWKUPENA (Asynchronous Wake-Up) */
#define SD2SUR2_CORECFG_SPIBLKMODE            29,   1               /* 29 CORECFG_SPIBLKMODE (SPI Block Mode) */
#define SD2SUR2_CORECFG_SPISUPPORT            28,   1               /* 28 CORECFG_SPISUPPORT (SPI Mode Support) */
#define SD2SUR2_CORECFG_RETUNINGMODES         26,   2               /* 26-27 CORECFG_RETUNINGMODES (Retuning Modes) */
#define SD2SUR2_CORECFG_TUNINGFORSDR50        25,   1               /* 25 CORECFG_TUNINGFORSDR50 (Tuning for SDR50) */
#define SD2SUR2_CORECFG_RETUNINGTIMERCNT      21,   4               /* 21-24 CORECFG_RETUNINGTIMERCNT (Timer Count for Retuning) */
#define SD2SUR2_CORECFG_DDRIVERSUPPORT        20,   1               /* 20 CORECFG_DDRIVERSUPPORT (Driver Type D Support) */
#define SD2SUR2_CORECFG_CDRIVERSUPPORT        19,   1               /* 19 CORECFG_CDRIVERSUPPORT (Driver Type C Support) */
#define SD2SUR2_CORECFG_ADRIVERSUPPORT        18,   1               /* 18 CORECFG_ADRIVERSUPPORT (Driver Type A Support) */
#define SD2SUR2_CORECFG_DDR50SUPPORT          17,   1               /* 17 CORECFG_DDR50SUPPORT (DDR50 Support) */
#define SD2SUR2_CORECFG_SDR104SUPPORT         16,   1               /* 16 CORECFG_SDR104SUPPORT (SDR104 Support) */
#define SD2SUR2_SDR50S CORECFG_SDR50SUPPORT   15,   1               /* 15 SDR50S CORECFG_SDR50SUPPORT (SDR50 Support) */
#define SD2SUR2_CORECFG_SLOTTYPE              13,   2               /* 13-14 CORECFG_SLOTTYPE (Slot Type) */
#define SD2SUR2_CORECFG_ASYNCINTRSUPPORT      12,   1               /* 12 CORECFG_ASYNCINTRSUPPORT (Asynchronous Interrupt Support) */
#define SD2SUR2_CORECFG_64BITSUPPORT          11,   1               /* 11 CORECFG_64BITSUPPORT (64-bit System Bus Support) */
#define SD2SUR2_CORECFG_1P8VOLTSUPPORT        10,   1               /* 10 CORECFG_1P8VOLTSUPPORT (1.8V Support) */
#define SD2SUR2_CORECFG_3P0VOLTSUPPORT        9,    1               /* 9 CORECFG_3P0VOLTSUPPORT (3.0V Support) */
#define SD2SUR2_CORECFG_3P3VOLTSUPPORT        8,    1               /* 8 CORECFG_3P3VOLTSUPPORT (3.3V Support) */
#define SD2SUR2_BASECKFRQ CORECFG_BASECLKFREQ 0,    8               /* 0-7 BASECKFRQ CORECFG_BASECLKFREQ (Base Clock Frequency) */

/***********************************************************************************************************/
/*    SDHC2 Interface Reset Value Register 3 (SD2IRV3) (reg48_dt)                                          */
/***********************************************************************************************************/
#define SD2IRV3                              (GCR_BASE_ADDR + 0xBC),  MEM,    32         /* Offset: BCh */
#define SD2IRV3_DBG_SEL                       24,   3               /* 24-26 DBG_SEL (Debug Select) */
#define SD2IRV3_CORECFG_MAXCURRENT3P3V        16,   8               /* 16-23 CORECFG_MAXCURRENT3P3V (Maximum Current for 3.3V) */
#define SD2IRV3_CORECFG_MAXCURRENT3P0V        8,    8               /* 8-15 CORECFG_MAXCURRENT3P0V (Maximum Current for 3.0V) */
#define SD2IRV3_CORECFG_MAXCURRENT1P8V        0,    8               /* 0-7 CORECFG_MAXCURRENT1P8V (Maximum Current for 1.8V) */

/***********************************************************************************************************/
/*    SDHC2 Interface Reset Value Register 4 (SD2IRV4) (reg50_dt)                                          */
/***********************************************************************************************************/
#define SD2IRV4                              (GCR_BASE_ADDR + 0x220),  MEM,    32         /* Offset: 220h */
#define SD2IRV4_CORECFG_INITPRESETVAL         0,    13               /* 0-12 CORECFG_INITPRESETVAL (Preset Value for Initialization) */

/***********************************************************************************************************/
/*    SDHC2 Interface Reset Value Register 5 (SD2IRV5) (reg54_dt)                                          */
/***********************************************************************************************************/
#define SD2IRV5                              (GCR_BASE_ADDR + 0x224),  MEM,    32         /* Offset: 224h */
#define SD2IRV5_CORECFG_DSPPRESETVAL          0,    13               /* 0-12 CORECFG_DSPPRESETVAL (Preset Value for Default Speed) */

/***********************************************************************************************************/
/*    SDHC2 Interface Reset Value Register 6 (SD2IRV6) (reg58_dt)                                          */
/***********************************************************************************************************/
#define SD2IRV6                              (GCR_BASE_ADDR + 0x228),  MEM,    32         /* Offset: 228h */
#define SD2IRV6_CORECFG_HSPPRESETVAL          0,    13               /* 0-12 CORECFG_HSPPRESETVAL (Preset Value for High Speed) */

/***********************************************************************************************************/
/*    SDHC2 Interface Reset Value Register 7 (SD2IRV7) (reg5C_dt)                                          */
/***********************************************************************************************************/
#define SD2IRV7                              (GCR_BASE_ADDR + 0x22C),  MEM,    32         /* Offset: 22Ch */
#define SD2IRV7_CORECFG_SDR12PRESETVAL        0,    13               /* 0-12 CORECFG_SDR12PRESETVAL (Preset Value for SDR12) */

/***********************************************************************************************************/
/*    SDHC2 Interface Reset Value Register 8 (SD2IRV8) (reg60_dt)                                          */
/***********************************************************************************************************/
#define SD2IRV8                              (GCR_BASE_ADDR + 0x230),  MEM,    32         /* Offset: 230h */
#define SD2IRV8_CORECFG_SDR25PRESETVAL        0,    13               /* 0-12 CORECFG_SDR25PRESETVAL (Preset Value for SDR25) */

/***********************************************************************************************************/
/*    SDHC2 Interface Reset Value Register 9 (SD2IRV9) (reg64_dt)                                          */
/***********************************************************************************************************/
#define SD2IRV9                              (GCR_BASE_ADDR + 0x234),  MEM,    32         /* Offset: 234h */
#define SD2IRV9_CORECFG_SDR50PRESETVAL        0,    13               /* 0-12 CORECFG_SDR50PRESETVAL (Preset Value for SDR50) */

/***********************************************************************************************************/
/*    SDHC2 Interface Reset Value Register 10 (SD2IRV10) (reg68_dt)                                        */
/***********************************************************************************************************/
#define SD2IRV10                             (GCR_BASE_ADDR + 0x238),  MEM,    32         /* Offset: 238h */
#define SD2IRV10_CORECFG_SDR104PRESETVAL      0,    13               /* 0-12 CORECFG_SDR104PRESETVAL (Preset Value for SDR104) */

/***********************************************************************************************************/
/*    SDHC2 Interface Reset Value Register 11 (SD2IRV11) (reg6C_dt)                                        */
/***********************************************************************************************************/
#define SD2IRV11                             (GCR_BASE_ADDR + 0x23C),  MEM,    32         /* Offset: 23Ch */
#define SD2IRV11_CORECFG_DDR50PRESETVAL       0,    13               /* 0-12 CORECFG_DDR50PRESETVAL (Preset Value for DDR50) */

/***********************************************************************************************************/
/*    Integration Control Register 2 (INTCR2)                                                              */
/***********************************************************************************************************/
#define INTCR2                               (GCR_BASE_ADDR + 0x060),  MEM,    32         /* Offset: 060h */
#define INTCR2_PORST                          31,   1               /* 31 PORST (Standby Power-Up Reset Status) */
#define INTCR2_CORST                          30,   1               /* 30 CORST (Core Domain Reset Status) */
#define INTCR2_WD0RST                         29,   1               /* 29 WD0RST (Watchdog 0 Reset Status) */
#define INTCR2_SWR1ST                         28,   1               /* 28 SWR1ST (Software Reset 1 Status) */
#define INTCR2_SWR2ST                         27,   1               /* 27 SWR2ST (Software Reset 2 Status) */
#define INTCR2_SWR3ST                         26,   1               /* 26 SWR3ST (Software Reset 3 Status) */
#define INTCR2_TIP_RESET                      25,   1               /* 25 TIP_RESET (TIP Reset Status)     */
#define INTCR2_WD1RST                         24,   1               /* 24 WD1RST (Watchdog 1 Reset Status) */
#define INTCR2_WD2RST                         23,   1               /* 23 WD2RST (Watchdog 2 Reset Status) */
#define INTCR2_DIS_ESPI_AUTO_INIT             22,   1               /* 22 DIS_ESPI_AUTO_INIT (Disable ESPI Automatic Initialization) */
#define INTCR2_WDC                            21,   1               /* 21 WDC (Watchdog Counter) */
#define INTCR2_RCHOSENIMAGE                   20,   1               /* 20 RCHOSENIMAGE (Current Chosen Image) */
#define INTCR2_MC_INIT                        19,   1               /* 19 MC_INIT (Memory Controller Initialized) */
#define INTCR2_CFGDONE                        18,   1               /* 18 CFGDONE (Clock Configuration indication) */
#define INTCR2_TIP_WDRST_STS                  15,   1               /* 15 TIP_WDRST_STS (TIP WD Reset Status), used by TIP ROM code */
#define INTCR2_TIP_SWRST_STS                  14,   1               /* 14 TIP_SWRST_STS (TIP SW Reset Status), used by TIP ROM code */
#define INTCR2_DBGRST_STS                     13,   1               /* 13 DBGRST_STS (Debugger Reset Status), used by TIP ROM code */
#define INTCR2_SW_SEC_ERR                     12,   1               /* 12 SW_SEC_ERR (Software Security Error), used by TIP ROM code */
#define INTCR2_HOST_INIT                      11,   1               /* 12 HOST_INIT (host init by bootblock). Used by bootblock SW only */
#define INTCR2_SP2                            7,    4               /* 7-11 SP2 (Scratchpad) */
#define INTCR2_H2RBRST                        4,    1               /* 4 H2RBRST (GFX (second head) */
#define INTCR2_H2RST1                         3,    1               /* 3 H2RST1 (GFX (second head) */
#define INTCR2_USB2FS                         1,    1               /* 1 USB2FS (USB2 PHY Force Suspend) */
#define INTCR2_PCIPHYPD                       0,    1               /* 0 PCIPHYPD (PCI Express PHY Power Down) */

/***********************************************************************************************************/
/*    Slew Rate Control Register (SRCNT)                                                                   */
/***********************************************************************************************************/
#define SRCNT                                (GCR_BASE_ADDR + 0x068),  MEM,    32         /* Offset: 068h */
#define SRCNT_TDO                             4,    1               /* 4 TDO (Slew Rate for TDO Output) */
#define SRCNT_SPI0C                           2,    1               /* 2 SPI0C (Slew Rate for SPI0CK Signal) */
#define SRCNT_SPI0D                           1,    1               /* 1 SPI0D (Slew Rate for SPI0D3-0 Signals) */

/***********************************************************************************************************/
/*    Reset Status Register (RESSR)                                                                        */
/***********************************************************************************************************/
#define RESSR                                (GCR_BASE_ADDR + 0x06C),  MEM,    32         /* Offset: 06Ch */
#define RESSR_PORST                           31,   1               /* 31 PORST (Standby Power On Reset Status) */
#define RESSR_CORST                           30,   1               /* 30 CORST (Core Domain Reset Status) */
#define RESSR_WD0RST                          29,   1               /* 29 WD0RST (Watchdog 0 Reset Status) */
#define RESSR_SWRST1                          28,   1               /* 28 SWRST1 (Software Reset 1 Status) */
#define RESSR_SWRST2                          27,   1               /* 27 SWRST2 (Software Reset 2 Status) */
#define RESSR_SWRST3                          26,   1               /* 26 SWRST3 (Software Reset 3 Status) */
#define RESSR_TIP_RESET                       25,   1               /* 25 TIP_RESET (TIP Reset Status)     */
#define RESSR_WD1RST                          24,   1               /* 24 WD1RST (Watchdog 1 Reset Status) */
#define RESSR_WD2RST                          23,   1               /* 23 WD2RST (Watchdog 2 Reset Status) */

/***********************************************************************************************************/
/*    Register Lock Register 1 (RLOCKR1)                                                                   */
/***********************************************************************************************************/
#define RLOCKR1                              (GCR_BASE_ADDR + 0x070),  MEM,    32         /* Offset: 070h */
#define RLOCKR1_RMRGNLK                       31,   1               /* 31 RMRGNLK (RAM Margin Registers Lock) */
#define RLOCKR1_BTOLK                         30,   1               /* 30 BTOLK (BUSTO Register Lock) */
#define RLOCKR1_IPR4LK                        29,   1               /* 29 IPR4LK (IPSRST4 Register Lock) */
#define RLOCKR1_CKE4LK                        28,   1               /* 28 CKE4LK (CLKEN4 Register Lock) */
#ifdef _ARBEL_Z1_
#define RLOCKR1_CORSTLK                       27,   1               /* 27 CORSTLK (CORSTC, CORSTCB Register Lock) */
#define RLOCKR1_SWR34LK                       26,   1               /* 26 SWR34LK (SWRSTC3, SWRSTC3B, TIPRSTCB, TIPRSTC Register Lock) */
#define RLOCKR1_SWR12LK                       25,   1               /* 25 SWR12LK (SWRSTC1, SWRSTC1B, SWRSTC2B, SWRSTC2 Register Lock) */
#define RLOCKR1_WD2RLK                        24,   1               /* 24 WD2RLK (WD2RCR, WD2RCRB Register Lock) */
#define RLOCKR1_WD1RLK                        23,   1               /* 23 WD1RLK (WD1RCR, WD1RCRB Register Lock) */
#define RLOCKR1_WD0RLK                        22,   1               /* 22 WD0RLK (WD0RCR, WD0RCRB Register Lock) */
#endif //_ARBEL_Z1_
#define RLOCKR1_IPR3LK                        21,   1               /* 21 IPR3LK (IPSRST3 Register Lock) */
#define RLOCKR1_CKE3LK                        20,   1               /* 20 CKE3LK (CLKEN3 Register Lock) */
#define RLOCKR1_PLL2LK                        19,   1               /* 19 PLL2LK (PLLCON2 Register Lock) */
#define RLOCKR1_IPR2LK                        17,   1               /* 17 IPR2LK (IPSRST2 Register Lock) */
#define RLOCKR1_IPR1LK                        16,   1               /* 16 IPR1LK (IPSRST1 Register Lock) */
#define RLOCKR1_PLL1LK                        15,   1               /* 15 PLL1LK (PLLCON1 Register Lock) */
#define RLOCKR1_PLL0LK                        14,   1               /* 14 PLL0LK (PLLCON0 Register Lock) */
#define RLOCKR1_CKDVLK                        13,   1               /* 13 CKDVLK (CLKDIV1, CLKDIV2 CLKDIV3 CLKDIV4 Register Lock) */
#define RLOCKR1_CKSLLK                        12,   1               /* 12 CKSLLK (CLKSEL Register Lock) */
#define RLOCKR1_CKE2LK                        11,   1               /* 11 CKE2LK (CLKEN2 Register Lock) */
#define RLOCKR1_CKE1LK                        10,   1               /* 10 CKE1LK (CLKEN1 Register Lock) */
#define RLOCKR1_DS1LK                         9,    1               /* 9 DS1LK (DSCNT Register Lock) */
#define RLOCKR1_HIFLK                         8,    1               /* 8 HIFLK (HIFCR Register Lock) */
#define RLOCKR1_MDLR                          7,    1               /* 7 MDLR (MDLR Register Lock) */
#define RLOCKR1_SR1LK                         6,    1               /* 6 SR1LK (SRCNT Register Lock) */
#define RLOCKR1_CPBPNTR1                      5,    1               /* 5 CPBPNTR1 (CPBPNTR1 Register Lock) */
#define RLOCKR1_VSRCLK                        4,    1               /* 4 VSRCLK (VSRCR Register Lock) */
#define RLOCKR1_I2CSSLK                       3,    1               /* 3 I2CSSLK (I2CSEGSEL Register Lock) */
#define RLOCKR1_MPELK                         2,    1               /* 2 MPELK (MISCPE Register Lock) */
#define RLOCKR1_INT4LK                        1,    1               /* 1 INT4LK (INTCR4 Register Lock) */
#define RLOCKR1_DACLLK                        0,    1               /* 0 DACLLK (DACLVLR Register Lock) */

/***********************************************************************************************************/
/*    Function Lock Register 1 (FLOCKR1)                                                                   */
/***********************************************************************************************************/
#define FLOCKR1                              (GCR_BASE_ADDR + 0x074),  MEM,    32         /* Offset: 074h */
#define FLOCKR1_FLKJTM2                       31,   1               /* 31 FLKJTM2 (new in Arbel) */
#define FLOCKR1_LKJTM2                        30,   1               /* 30 LKJTM2 (new in Arbel) */
#define FLOCKR1_FLKJTM1                       29,   1               /* 29 FLKJTM1 (new in Arbel) */
#define FLOCKR1_LKJTM1                        28,   1               /* 28 LKJTM1 (new in Arbel) */
#define FLOCKR1_CPU1CKDLK                     27,   1               /* 27 CPU1CKDLK (default) */
#define FLOCKR1_CPU1CKDIS                     26,   1               /* 26 CPU1CKDIS (default) */
#define FLOCKR1_MWPSLK                        25,   1               /* 25 MWPSLK (default) */
#define FLOCKR1_MWPSEL                        24,   1               /* 24 MWPSEL (default) */
#define FLOCKR1_ICCFGLK                       23,   1               /* 23 ICCFGLK (default) */
#define FLOCKR1_INTCCFGD                      22,   1               /* 22 INTCCFGD (Connected to CFGSDISABLE input of A35) */
#define FLOCKR1_PSMILK                        21,   1               /* 21 PSMILK (default) */
#define FLOCKR1_PSMISEL                       20,   1               /* 20 PSMISEL (default) */
#define FLOCKR1_RAMVLK                        19,   1               /* 19 RAMVLK (default) */
#define FLOCKR1_RAMV                          18,   1               /* 18 RAMV (256 bytes) */
#define FLOCKR1_JDISLK                        17,   1               /* 17 JDISLK (JTAG Disable Lock) */
#define FLOCKR1_JTAGDIS                       16,   1               /* 16 JTAGDIS (JTAG Disable) */
#define FLOCKR1_LKSRAM2                       15,   1               /* 15 LKSRAM2 (SRAM2 Lock) */
#define FLOCKR1_CNTSRAM2                      14,   1               /* 14 CNTSRAM2 (SRAM2 Control) */
#define FLOCKR1_LKSRAM1                       13,   1               /* 13 LKSRAM1 (SRAM1 Lock) */
#define FLOCKR1_CNTSRAM1                      12,   1               /* 12 CNTSRAM1 (SRAM1 Control) */
#define FLOCKR1_LKGPO2VLK                     11,   1               /* 11 LKGPO2VLK (LKGPO2 Value Lock) */
#define FLOCKR1_LKGPO2VAL                     10,   1               /* 10 LKGPO2VAL (LKGPO2 Value) */
#define FLOCKR1_LKGPO2SLK                     9,    1               /* 9 LKGPO2SLK (LKGPO2 Select Lock) */
#define FLOCKR1_LKGPO2SEL                     8,    1               /* 8 LKGPO2SEL (LKGPO2 Select) */
#define FLOCKR1_LKGPO1VLK                     7,    1               /* 7 LKGPO1VLK (LKGPO1 Value Lock) */
#define FLOCKR1_LKGPO1VAL                     6,    1               /* 6 LKGPO1VAL (LKGPO1 Value) */
#define FLOCKR1_LKGPO1SLK                     5,    1               /* 5 LKGPO1SLK (LKGPO1 Select Lock) */
#define FLOCKR1_LKGPO1SEL                     4,    1               /* 4 LKGPO1SEL (LKGPO1 Select) */
#define FLOCKR1_LKGPO0VLK                     3,    1               /* 3 LKGPO0VLK (LKGPO0 Value Lock) */
#define FLOCKR1_LKGPO0VAL                     2,    1               /* 2 LKGPO0VAL (LKGPO0 Value) */
#define FLOCKR1_LKGPO0SLK                     1,    1               /* 1 LKGPO0SLK (LKGPO0 Select Lock) */
#define FLOCKR1_LKGPO0SEL                     0,    1               /* 0 LKGPO0SEL (LKGPO0 Select) */

/***********************************************************************************************************/
/*    Function Lock Register 2 (FLOCKR2)                                                                   */
/***********************************************************************************************************/
#define FLOCKR2                              (GCR_BASE_ADDR + 0x020),  MEM,    32         /* Offset: 020h */
#define FLOCKR2_F15LK                         31,   1               /* 31 F15LK (Function 15 Lock Bit) */
#define FLOCKR2_F15V                          30,   1               /* 30 F15V (Function 15 Value Bit) */
#define FLOCKR2_F14LK                         29,   1               /* 29 F14LK (Function 14 Lock Bit) */
#define FLOCKR2_F14V                          28,   1               /* 28 F14V (Function 14 Value Bit) */
#define FLOCKR2_F13LK                         27,   1               /* 27 F13LK (Function 13 Lock Bit) */
#define FLOCKR2_F13V                          26,   1               /* 26 F13V (Function 13 Value Bit) */
#ifdef _ARBEL_Z1_
#define FLOCKR2_F12LK                         25,   1               /* 25 F12LK (Function 12 Lock Bit) */
#define FLOCKR2_F12V                          24,   1               /* 24 F12V (Function 12 Value Bit) */
#else
#define FLOCKR2_CRHENLK                       25,   1               /* 25 CRHENLK (CRHEN Lock Bit) */
#define FLOCKR2_CRHEN                         24,   1               /* 24 CRHEN (COP Reset Hold Enable) */
#endif
#define FLOCKR2_TFDLK                         23,   1               /* 23 TFDLK (Lock Test functions disable) */
#define FLOCKR2_TFD                           22,   1               /* 22 TFD (Test functions disable) */
#define FLOCKR2_F10LK                         21,   1               /* 21 F10LK (Function 10 Lock Bit) */
#define FLOCKR2_F10V                          20,   1               /* 20 F10V (Function 10 Value Bit) */
#define FLOCKR2_F9LK                          19,   1               /* 19 F9LK (Function 9 Lock Bit) */
#define FLOCKR2_F9V                           18,   1               /* 18 F9V (Function 9 Value Bit) */
#define FLOCKR2_DIMAENLK                      17,   1               /* 17 DIMAENLK (Lock DIS_IMAEN) */
#define FLOCKR2_DIS_IMAEN                     16,   1               /* 16 DIS_IMAEN (Disable Host Indirect Memory Access) */
#define FLOCKR2_DBMMENLK                      15,   1               /* 15 DBMMENLK (Lock DIS_BMMEN) */
#define FLOCKR2_DIS_BMMEN                     14,   1               /* 14 DIS_BMMEN (Disable eSPI Bus Master) */
#define FLOCKR2_MMCRSTLK                      13,   1               /* 13 MMCRSTLK (Lock nMMCRST) */
#define FLOCKR2_MMCRST                        12,   1               /* 12 MMCRST (Reset to MMC) */
#ifdef _ARBEL_Z1_
#define FLOCKR2_TIPRSTENLK                    11,   1               /* 11 TIPRSTENLK (Reset from TIP Enable lock) */
#define FLOCKR2_TIPRSTEN                      10,   1               /* 10 TIPRSTEN (Reset from TIP Enable) */
#else
#define FLOCKR2_F5LK                          11,   1               /* 11 F5LK (Function 5 Lock Bit) */
#define FLOCKR2_F5V                           10,   1               /* 10 F5V (Function 5 Value Bit) */
#endif
#define FLOCKR2_LRSTENLK                      9,    1               /* 9 LRSTENLK (Reset from LPC Enable lock) */
#define FLOCKR2_LPCRSTEN                      8,    1               /* 8 LPCRSTEN (Reset from LPC Enable) */
#define FLOCKR2_S1CSELK                       7,    1               /* 7 S1CSELK (SPI1 CS Exchange Lock) */
#define FLOCKR2_SPI1_CS_EXC                   6,    1               /* 6 SPI1_CS_EXC (SPI1 CS Exchange) */
#define FLOCKR2_S3CSELK                       5,    1               /* 5 S3CSELK (CLK_SPI3 CS Exchange Lock) */
#define FLOCKR2_SPI3_CS_EXC                   4,    1               /* 4 SPI3_CS_EXC (CLK_SPI3 CS Exchange) */
#define FLOCKR2_S0CSELK                       3,    1               /* 3 S0CSELK (CLK_SPI0 CS Exchange Lock) */
#define FLOCKR2_SPI0_CS_EXC                   2,    1               /* 2 SPI0_CS_EXC (CLK_SPI0 CS Exchange) */
#define FLOCKR2_SRSTMDLK                      1,    1               /* 1 SRSTMDLK (Super IO Reset Mode Lock) */
#define FLOCKR2_SIO_RST_MODE                  0,    1               /* 0 SIO_RST_MODE (Super IO Reset Mode) */

/***********************************************************************************************************/
/*    Function Lock Register 3 (FLOCKR3)                                                                   */
/***********************************************************************************************************/
#define FLOCKR3                              (GCR_BASE_ADDR + 0x024),  MEM,    32         /* Offset: 024h */
#define FLOCKR3_F15LK                         31,   1               /* 31 F15LK (Function 15 Lock Bit) */
#define FLOCKR3_F15V                          30,   1               /* 30 F15V (Function 15 Value Bit) */
#define FLOCKR3_F14LK                         29,   1               /* 29 F14LK (Function 14 Lock Bit) */
#define FLOCKR3_F14V                          28,   1               /* 28 F14V (Function 14 Value Bit) */
#define FLOCKR3_F13LK                         27,   1               /* 27 F13LK (Function 13 Lock Bit) */
#define FLOCKR3_F13V                          26,   1               /* 26 F13V (Function 13 Value Bit) */
#define FLOCKR3_F12LK                         25,   1               /* 25 F12LK (Function 12 Lock Bit) */
#define FLOCKR3_F12V                          24,   1               /* 24 F12V (Function 12 Value Bit) */
#define FLOCKR3_F11LK                         23,   1               /* 23 F11LK (Function 11 Lock Bit) */
#define FLOCKR3_F11V                          22,   1               /* 22 F11V (Function 11 Value Bit) */
#define FLOCKR3_F10LK                         21,   1               /* 21 F10LK (Function 10 Lock Bit) */
#define FLOCKR3_F10V                          20,   1               /* 20 F10V (Function 10 Value Bit) */
#define FLOCKR3_F9LK                          19,   1               /* 19 F9LK (Function 9 Lock Bit) */
#define FLOCKR3_F9V                           18,   1               /* 18 F9V (Function 9 Value Bit) */
#define FLOCKR3_F8LK                          17,   1               /* 17 F8LK (Function 8 Lock Bit) */
#define FLOCKR3_F8V                           16,   1               /* 16 F8V (Function 8 Value Bit) */
#define FLOCKR3_F7LK                          15,   1               /* 15 F7LK (Function 7 Lock Bit) */
#define FLOCKR3_F7V                           14,   1               /* 14 F7V (Function 7 Value Bit) */
#define FLOCKR3_F6LK                          13,   1               /* 13 F6LK (Function 6 Lock Bit) */
#define FLOCKR3_F6V                           12,   1               /* 12 F6V (Function 6 Value Bit) */
#define FLOCKR3_F5LK                          11,   1               /* 11 F5LK (Function 5 Lock Bit) */
#define FLOCKR3_F85V                          10,   1               /* 10 F85V (Function 5 Value Bit) */
#define FLOCKR3_F4LK                          9,    1               /* 9 F4LK (Function 4 Lock Bit) */
#define FLOCKR3_F4V                           8,    1               /* 8 F4V (Function 4 Value Bit) */
#define FLOCKR3_F3LK                          7,    1               /* 7 F3LK (Function 3 Lock Bit) */
#define FLOCKR3_F3V                           6,    1               /* 6 F3V (Function 3 Value Bit) */
#define FLOCKR3_F2LK                          5,    1               /* 5 F2LK (Function 2 Lock Bit) */
#define FLOCKR3_F2V                           4,    1               /* 4 F2V (Function 42 Value Bit) */
#define FLOCKR3_F1LK                          3,    1               /* 3 F1LK (Function 1 Lock Bit) */
#define FLOCKR3_F1V                           2,    1               /* 2 F1V (Function 1 Value Bit) */
#define FLOCKR3_F0LK                          1,    1               /* 1 F0LK (Function 0 Lock Bit) */
#define FLOCKR3_F0V                           0,    1               /* 0 F0V (Function 0 Value Bit) */

/***********************************************************************************************************/
/*    Drive Strength Control Register (DSCNT)                                                              */
/***********************************************************************************************************/
#define DSCNT                                (GCR_BASE_ADDR + 0x078),  MEM,    32         /* Offset: 078h */
#define DSCNT_SPLD                            9,    1               /* 9 SPLD (Drive Strength for nSPILOAD) */
#define DSCNT_IMPLEMENTED_RESERVED_FOR_FUTURE_USE 9,    23               /* 9-31 IMPLEMENTED BUT RESERVED FOR FUTURE USE (None) */
#define DSCNT_RGMII2                          6,    2               /* 6-7 RGMII2 (Drive Strength for RGMII2) */
#define DSCNT_SPI0C                           2,    1               /* 2 SPI0C (Drive Strength for SPI0CK) */
#define DSCNT_SPI0D                           1,    1               /* 1 SPI0D (Added bits 3-2 in Arbel) */

/***********************************************************************************************************/
/*    Module Disable Lock Register (MDLR)                                                                  */
/***********************************************************************************************************/
#define MDLR                                 (GCR_BASE_ADDR + 0x7C),  MEM,    32         /* Offset: 7Ch */
#define MDLR_JTAG_MSTR                        31,   1               /* 31 JTAG_MSTR (JTAG Masters Modules Disable Control) */
#define MDLR_CP1                              30,   1               /* 30 CP1 (Coprocessor 1 Module Disable Control) */
#define MDLR_MMC                              28,   1               /* 28 MMC (MMC Controller Module Disable Control) */
#define MDLR_ARM_CP15                         26,   1               /* 26 ARM_CP15 (ARM CPU CP15 Access Disable Control) */
#define MDLR_FIU13X                           25,   1               /* 25 FIU13X (FIU1, FIU3, FIUX Modules Disable Control) */
#define MDLR_USBH2D8                          24,   1               /* 24 USBH2D8 (USB Host 2 and USB Device 8 Modules Disable Control) */
#define MDLR_ESPI                             23,   1               /* 23 ESPI (ESPI Modules Disable Control) */
#define MDLR_USBD4_7                          22,   1               /* 22 USBD4-7 (USBD4-7 Modules Disable Control) */
#define MDLR_USBH1D9                          21,   1               /* 21 USBH1D9 (USB Host 1 and USB Device 9 Modules Disable Control) */
#define MDLR_AHB2PCI                          20,   1               /* 20 AHB2PCI (AHB2PCI Module Disable Control) */
#define MDLR_CPU_CRYPTO                       19,   1               /* 19 CPU_CRYPTO (CPU Crypto-Accelerators Disable Control) */
#define MDLR_GPIOALL                          18,   1               /* 18 GPIOALL (All GPIO Modules Disable Control) */
#define MDLR_PCIERC                           17,   1               /* 17 PCIERC (PCIE root Complex Module Disable Control) */
#define MDLR_PWMALL                           16,   1               /* 16 PWMALL (All PWM Modules Disable Control) */
#define MDLR_SMBALL                           15,   1               /* 15 SMBALL (SMB0-26 Modules Disable Control) */
#define MDLR_I3CALL                           14,   1               /* 14 I3CALL (I3C0-5 Modules Disable Control) */
#define MDLR_OTPAES                           12,   1               /* 12 OTPAES (OTP and AES Modules Disable Control) */
#define MDLR_GMAC4                            11,   1               /* 11 GMAC4 (GMAC4 Module Disable Control) */
#define MDLR_GMAC3                            10,   1               /* 10 GMAC3 (GMAC3 Module Disable Control) */
#define MDLR_USBD0_3                          9,    1               /* 9 USBD0-3 (USB Device 0 to USB Device 3 Module Disable Control) */
#define MDLR_PSPI2                            8,    1               /* 8 PSPI2 (PSPI2 Module Disable Control) */
#define MDLR_GMAC2                            7,    1               /* 7 GMAC2 (GMAC2 Module Disable Control) */
#define MDLR_GMAC1                            6,    1               /* 6 GMAC1 (GMAC1 Module Disable Control) */
#define MDLR_GDMA                             5,    1               /* 5 GDMA (GDMA0 to GDMA2 Modules Disable Control) */
#define MDLR_RESERVED_1                       4,    1               /* 4 Reserved bit */
#define MDLR_GFXOFF                           3,    1               /* 3 GFXOFF (Graphics Core Disable Control) */
#define MDLR_VGAONLY                          2,    1               /* 2 VGAONLY (VGAONLY) */
#define MDLR_DACPD                            1,    1               /* 1 DACPD (DAC Power Down Control) */
#define MDLR_VCD                              0,    1               /* 0 VCD (VCD Module Disable Control) */

/***********************************************************************************************************/
/*    Integration Control Register 3 (INTCR3)                                                              */
/***********************************************************************************************************/
#define INTCR3                               (GCR_BASE_ADDR + 0x09C),  MEM,    32         /* Offset: 09Ch */
#define INTCR3_USBLPBK2                       31,   1               /* 31 USBLPBK2 (USB Loopback Host 2) */
#define INTCR3_CHIK501                        29,   1               /* 29 CHIK501 (Issue 501 Fix Disable) */
#define INTCR3_USBLPBK                        24,   1               /* 24 USBLPBK (USB Loopback Mode) */
#define INTCR3_RCCORER                        22,   1               /* 22 RCCORER (Root Complex Core Reset) */
#define INTCR3_PECIVSEL                       19,   1               /* 19 PECIVSEL (PECI Voltage Select) */
#define INTCR3_PCIEDEM                        17,   1               /* 17 PCIEDEM (PCI Express De-Emphasis) */
#define INTCR3_DRAMINIT                       16,   1               /* 16 DRAMINIT (DRAM Initialization) */
#define INTCR3_USBPHY3SW                      14,   2               /* 14-15 USBPHY3SW (USB PHY3 Switch) */
#define INTCR3_USBPHY2SW                      12,   2               /* 12-13 USBPHY2SW (USB PHY2 Switch) */
#define INTCR3_ESPI_INT_POL                   11,   1               /* 11 ESPI_INT_POL (eSPI Interrupt Polarity) */
#define INTCR3_IO_I2C_SLOW                    9,    1               /* 9 IO_I2C_SLOW (I/O cell I2C control) */
#define INTCR3_IO_LVI3C_SLOW                  8,    1               /* 8 IO_LVI3C_SLOW (I/O cell LVI3C control) */
#define INTCR3_PCIEENHD                       7,    1               /* 7 PCIEENHD (PCI Express Bridge Enhancement Disable) */
#define INTCR3_UHUB_RWUD                      5,    1               /* 5 UHUB_RWUD (USB Hub Remote Wake-Up Disable) */
#define INTCR3_HSRDIS                         2,    1               /* 2 HSRDIS (Host Secondary Reset Disable) */
#define INTCR3_HHRDIS                         0,    1               /* 0 HHRDIS (Host PCIe Hot Reset Disable) */

/***********************************************************************************************************/
/*    Integration Control Register 4 (INTCR4)                                                              */
/***********************************************************************************************************/
#define INTCR4                               (GCR_BASE_ADDR + 0x0C0),  MEM,    32         /* Offset: 0C0h */
#define INTCR4_GMMAP1                         24,   7               /* 24-30 GMMAP1 (Graphics Memory Map for GFX Port 1) */
#define INTCR4_GMMAP0                         16,   7               /* 16-22 GMMAP0 (Graphics Memory Map for GFX Port 0) */
#define INTCR4_ESPI_SAFEN                     15,   1               /* 15 ESPI_SAFEN (Enable SAF in ESPI) */
#define INTCR4_R3EN                           14,   1               /* 14 R3EN (Enable RMII 3 Outputs) */
#define INTCR4_R2EN                           13,   1               /* 13 R2EN (Enable RMII 2 Outputs) */
#define INTCR4_R1EN                           12,   1               /* 12 R1EN (Enable RMII 1 Outputs) */
#define INTCR4_PCIEDEM2                       11,   1               /* 11 PCIEDEM2 (PCIE PHY 2 Deemphasis) */
#define INTCR4_PCIEPHY2PD                     10,   1               /* 10 PCIEPHY2PD (PCIE PHY 2 Power Down. Enabled Power Down for PCIe PHY 2.0: PCIe PHY 2 functional (default) */
#define INTCR4_LMONIE                         9,    1               /* 9 LMONIE (LED Monitor Interrupt Response Enable) */
#define INTCR4_LMEN                           8,    1               /* 8 LMEN (LED Monitor Enable) */
#define INTCR4_PCIEPHY1TS                     7,    1               /* 7 PCIEPHY1TS (PCIE PHY 1 TX Swing) */
#define INTCR4_RGMIIREF                       6,    1               /* 6 RGMIIREF (GMAC Reference Clock Select) */
#define INTCR4_DMARQSEL0                      4,    2               /* 4-5 DMARQSEL0 (DMA Request Select 0) */
#define INTCR4_FIU_FIX1                       3,    1               /* 3 FIU_FIX1 (Enables FIU FIX for long bursts) */
#define INTCR4_FIU_FIXX                       2,    1               /* 2 FIU_FIXX (Enables FIUX FIX for long bursts) */
#define INTCR4_FIU_FIX3                       1,    1               /* 1 FIU_FIX3 (Enables FIU3 FIX for long bursts) */
#define INTCR4_FIU_FIX0                       0,    1               /* 0 FIU_FIX0 (Enables FIU FIX for long bursts) */

/***********************************************************************************************************/
/*    PCI Reset Control Register (PCIRCTL)                                                                 */
/***********************************************************************************************************/
#define PCIRCTL                              (GCR_BASE_ADDR + 0x0A0),  MEM,    32         /* Offset: 0A0h */
#define PCIRCTL_PCIRSTD                       4,    1               /* 4 PCIRSTD (PCI Reset Drive) */
#define PCIRCTL_PCIRRELC                      1,    1               /* 1 PCIRRELC (PCI Reset Release Clear) */
#define PCIRCTL_PCIRREL                       0,    1               /* 0 PCIRREL (PCI Reset Release) */

/***********************************************************************************************************/
/*    I2C Segment Pin Select Register (I2CSEGSEL)                                                          */
/***********************************************************************************************************/
#define I2CSEGSEL                            (GCR_BASE_ADDR + 0x0E0),  MEM,    32         /* Offset: 0E0h */
#define I2CSEGSEL_S7SDSEL                     29,   1               /* 29 S7SDSEL (Segment 7D Select) */
#define I2CSEGSEL_S7SCSEL                     28,   1               /* 28 S7SCSEL (Segment 7C Select) */
#define I2CSEGSEL_S7SBSEL                     27,   1               /* 27 S7SBSEL (Segment 7B Select) */
#define I2CSEGSEL_S6SDSEL                     26,   1               /* 26 S6SDSEL (Segment 6D Select) */
#define I2CSEGSEL_S6SCSEL                     25,   1               /* 25 S6SCSEL (Segment 6C Select) */
#define I2CSEGSEL_S6SBSEL                     24,   1               /* 24 S6SBSEL (Segment 6B Select) */
#define I2CSEGSEL_S0DESEL                     22,   1               /* 22 S0DESEL (Segment 0 Drive Enable Select) */
#define I2CSEGSEL_S5SDSEL                     21,   1               /* 21 S5SDSEL (Segment 5D Select) */
#define I2CSEGSEL_S5SCSEL                     20,   1               /* 20 S5SCSEL (Segment 5C Select) */
#define I2CSEGSEL_S5SBSEL                     19,   1               /* 19 S5SBSEL (Segment 5B Select) */
#define I2CSEGSEL_S4DECFG                     17,   2               /* 17-18 S4DECFG (Segment 4 Drive Control Configuration) */
#define I2CSEGSEL_S4SDSEL                     16,   1               /* 16 S4SDSEL (Segment 4D Select) */
#define I2CSEGSEL_S4SCSEL                     15,   1               /* 15 S4SCSEL (Segment 4C Select) */
#define I2CSEGSEL_S4SBSEL                     14,   1               /* 14 S4SBSEL (Segment 4B Select) */
#define I2CSEGSEL_S3SDSEL                     13,   1               /* 13 S3SDSEL (Segment 3D Select) */
#define I2CSEGSEL_S3SCSEL                     12,   1               /* 12 S3SCSEL (Segment 3C Select) */
#define I2CSEGSEL_S3SBSEL                     11,   1               /* 11 S3SBSEL (Segment 3B Select) */
#define I2CSEGSEL_S2SDSEL                     10,   1               /* 10 S2SDSEL (Segment 2D Select) */
#define I2CSEGSEL_S2SCSEL                     9,    1               /* 9 S2SCSEL (Segment 2C Select) */
#define I2CSEGSEL_S2SBSEL                     8,    1               /* 8 S2SBSEL (Segment 2B Select) */
#define I2CSEGSEL_S1SDSEL                     7,    1               /* 7 S1SDSEL (Segment 1D Select) */
#define I2CSEGSEL_S1SCSEL                     6,    1               /* 6 S1SCSEL (Segment 1C Select) */
#define I2CSEGSEL_S1SBSEL                     5,    1               /* 5 S1SBSEL (Segment 1B Select) */
#define I2CSEGSEL_S0DECFG                     3,    2               /* 3-4 S0DECFG (Segment 0 Drive Control Configuration) */
#define I2CSEGSEL_S0SDSEL                     2,    1               /* 2 S0SDSEL (Segment 0D Select) */
#define I2CSEGSEL_S0SCSEL                     1,    1               /* 1 S0SCSEL (Segment 0C Select) */
#define I2CSEGSEL_S0SBSEL                     0,    1               /* 0 S0SBSEL (Segment 0B Select) */

/***********************************************************************************************************/
/*    I2C Segment Control Register (I2CSEGCTL)                                                             */
/***********************************************************************************************************/
#define I2CSEGCTL                            (GCR_BASE_ADDR + 0x0E4),  MEM,    32         /* Offset: 0E4h */
#define I2CSEGCTL_WEN7SS                      31,   1               /* 31 WEN7SS (SMB7SS Write Enable) */
#define I2CSEGCTL_SMB7SS                      29,   2               /* 29-30 SMB7SS (SMBus 7 Segment Select) */
#define I2CSEGCTL_WEN6SS                      28,   1               /* 28 WEN6SS (SMB6SS Write Enable) */
#define I2CSEGCTL_SMB6SS                      26,   2               /* 26-27 SMB6SS (SMBus 6 Segment Select) */
#define I2CSEGCTL_S4D_WE_EN                   24,   2               /* SMB4 drive enable ( sum of the two bits below : I2CSEGCTL_S4DWE, I2CSEGCTL_S4DEN)       */
#define I2CSEGCTL_S4DWE                       25,   1               /* 25 S4DWE (S4DEN Write Enable) */
#define I2CSEGCTL_S4DEN                       24,   1               /* 24 S4DEN (Segment 4 SW Drive Enable) */
#define I2CSEGCTL_S0D_WE_EN                   20,   2               /* SMB0 drive enable ( sum of the two bits below : I2CSEGCTL_S0DWE, I2CSEGCTL_S0DEN)       */
#define I2CSEGCTL_S0DWE                       21,   1               /* 21 S0DWE (S0DEN Write Enable) */
#define I2CSEGCTL_S0DEN                       20,   1               /* 20 S0DEN (Segment 0 SW Drive Enable) */
#define I2CSEGCTL_WEN5SS                      17,   1               /* 17 WEN5SS (SMB5SS Write Enable) */
#define I2CSEGCTL_WEN4SS                      16,   1               /* 16 WEN4SS (SMB4SS Write Enable) */
#define I2CSEGCTL_WEN3SS                      15,   1               /* 15 WEN3SS (SMB3SS Write Enable) */
#define I2CSEGCTL_WEN2SS                      14,   1               /* 14 WEN2SS (SMB2SS Write Enable) */
#define I2CSEGCTL_WEN1SS                      13,   1               /* 13 WEN1SS (SMB1SS Write Enable) */
#define I2CSEGCTL_WEN0SS                      12,   1               /* 12 WEN0SS (SMB0SS Write Enable) */
#define I2CSEGCTL_SMB5SS                      10,   2               /* 10-11 SMB5SS (SMBus 5 Segment Select) */
#define I2CSEGCTL_SMB4SS                      8,    2               /* 8-9 SMB4SS (SMBus 4 Segment Select) */
#define I2CSEGCTL_SMB3SS                      6,    2               /* 6-7 SMB3SS (SMBus 3 Segment Select) */
#define I2CSEGCTL_SMB2SS                      4,    2               /* 4-5 SMB2SS (SMBus 2 Segment Select) */
#define I2CSEGCTL_SMB1SS                      2,    2               /* 2-3 SMB1SS (SMBus 1 Segment Select) */
#define I2CSEGCTL_SMB0SS                      0,    2               /* 0-1 SMB0SS (SMBus 0 Segment Select) */

/***********************************************************************************************************/
/*    Voltage Supply Report Register (VSRCR)                                                               */
/***********************************************************************************************************/
#define VSRCR                                (GCR_BASE_ADDR + 0x0E8),  MEM,    32         /* Offset: 0E8h */
#define VSRCR_VR2                             30,   1               /* 30 VR2 (VSBR2 Voltage Level) */
#define VSRCR_V14B                            29,   1               /* 29 V14B (VSBV14 Voltage Level B) */
#define VSRCR_V13B                            28,   1               /* 28 V13B (VSBV13 Voltage Level B) */
#define VSRCR_V11B                            26,   1               /* 26 V11B (VSBV11 Voltage Level B) */
#define VSRCR_V10B                            25,   1               /* 25 V10B (VSBV10 Voltage Level B) */
#define VSRCR_V9B                             24,   1               /* 24 V9B (VSBV9 Voltage Level B) */
#define VSRCR_V8B                             23,   1               /* 23 V8B (VSBV8 Voltage Level B) */
#define VSRCR_V7B                             22,   1               /* 22 V7B (VSBV7 Voltage Level B) */
#define VSRCR_V6B                             21,   1               /* 21 V6B (VSBV6 Voltage Level B) */
#define VSRCR_V5B                             20,   1               /* 20 V5B (VSBV5 Voltage Level B) */
#define VSRCR_V4B                             19,   1               /* 19 V4B (VSBV4 Voltage Level B) */
#define VSRCR_V3B                             18,   1               /* 18 V3B (VSBV3 Voltage Level B) */
#define VSRCR_V2B                             17,   1               /* 17 V2B (VSBV2 Voltage Level B) */
#define VSRCR_V1B                             16,   1               /* 16 V1B (VSBV1 Voltage Level B) */
#define VSRCR_VSIF                            14,   1               /* 14 VSIF (VSBSIF Voltage Level) */
#define VSRCR_V14                             13,   1               /* 13 V14 (VSBV14 Voltage Level) */
#define VSRCR_V13                             12,   1               /* 12 V13 (VSBV13 Voltage Level) */
#define VSRCR_V12                             11,   1               /* 11 V12 (VSBV12 Voltage Level) */
#define VSRCR_V11                             10,   1               /* 10 V11 (VSBV11 Voltage Level) */
#define VSRCR_V10                             9,    1               /* 9 V10 (VSBV10 Voltage Level) */
#define VSRCR_V9                              8,    1               /* 8 V9 (VSBV9 Voltage Level) */
#define VSRCR_V8                              7,    1               /* 7 V8 (VSBV8 Voltage Level) */
#define VSRCR_V7                              6,    1               /* 6 V7 (VSBV7 Voltage Level) */
#define VSRCR_V6                              5,    1               /* 5 V6 (VSBV6 Voltage Level) */
#define VSRCR_V5                              4,    1               /* 4 V5 (VSBV5 Voltage Level) */
#define VSRCR_V4                              3,    1               /* 3 V4 (VSBV4 Voltage Level) */
#define VSRCR_V3                              2,    1               /* 2 V3 (VSBV3 Voltage Level) */
#define VSRCR_V2                              1,    1               /* 1 V2 (VSBV2 Voltage Level) */
#define VSRCR_V1                              0,    1               /* 0 V1 (VSBV1 Voltage Level) */

/***********************************************************************************************************/
/*    Module Lock Register (MLOCKR)                                                                        */
/***********************************************************************************************************/
#define MLOCKR                               (GCR_BASE_ADDR + 0x0EC),  MEM,    32         /* Offset: 0ECh */
#define MLOCKR_AHB2PCI                        0,    1               /* 0 AHB2PCI (AHB-to-PCE Bridge Lock) */

/***********************************************************************************************************/
/*    Voltage Sensor Calibration Register (VSNSCALR)                                                       */
/***********************************************************************************************************/
#define VSNSCALR                             (GCR_BASE_ADDR + 0x28),  MEM,    32         /* Offset: 28Ch */
#define VSNSCALR_LOCK                         31,    1               /* 31 LOCK (Lock This Register) */
#define VSNSCALR_VOTPSNSCAL                   24,    5               /* 28-24 VOTPSNSCAL (VOTP Sensor Calibration) */
#define VSNSCALR_VDDSNSCAL                    16,    5               /* 20-16 VDDSNSCAL (VDD Sensor Calibration) */
#define VSNSCALR_VOTP_EXIST_ST                9,     1               /* 9 VOTP_EXIST_ST (VOTP_EXIST_STATUS) */
#define VSNSCALR_VDD_EXIST_ST                 8,     1               /* 8 VDD_EXIST_ST (VDD_EXIST_STATUS) */
#define VSNSCALR_VSNSEN                       0,     1               /* 0 VSNSEN (Voltage Sensor Enable) */

/***********************************************************************************************************/
/*    Scratchpad Register 2 (SCRPAD_02)                                                                    */
/***********************************************************************************************************/
#define SCRPAD_02                            (GCR_BASE_ADDR + 0x084),  MEM,    32         /* Offset: 084h */
#define SCRPAD_02_SCRATCHPAD                  0,    32               /* 0-31 SCRATCHPAD (Scratchpad) */

/***********************************************************************************************************/
/*    Scratchpad Register 3 (SCRPAD_03)                                                                    */
/***********************************************************************************************************/
#define SCRPAD_03                            (GCR_BASE_ADDR + 0x080),  MEM,    32         /* Offset: 080h */
#define SCRPAD_03_SCRATCHPAD                  0,    32               /* 0-31 SCRATCHPAD (Scratchpad) */

/***********************************************************************************************************/
/*    Eng. Test Straps Register (ETSR)                                                                     */
/***********************************************************************************************************/
#define ETSR                                 (GCR_BASE_ADDR + 0x110),  MEM,    32         /* Offset: 110h */
#define ETSR_ESTRP14_1                        1,    14               /* 1-14 ESTRP14_1 (Eng. Strap 14 to 1) */
#define ETSR_ESTRP12_0                        0,    13               /* 0-12 ESTRP12_0 (Eng. bits 12 to 0) */

/***********************************************************************************************************/
/*    Eng. Debug Flip-Flop Scan Register (EDFFSR)                                                          */
/***********************************************************************************************************/
#define EDFFSR                               (GCR_BASE_ADDR + 0x120),  MEM,    32         /* Offset: 120h */
#define EDFFSR_SCRATCHPAD                     0,    5               /* 0-4 SCRATCHPAD (None) */

#if 0
/***********************************************************************************************************/
/*    Eng. Integration Control Register for PCIe 2 (INTCRPCE2)                                             */
/***********************************************************************************************************/
#define INTCRPCE2                            (GCR_BASE_ADDR + 0x12C),  MEM,    32         /* Offset: 12Ch */
#define INTCRPCE2_PHY_TX0_TERM_OFFSET = INTCRPCE2[4:0]; // DEFAULT IS 00H 0,    32               /* 0-31 PHY_TX0_TERM_OFFSET = INTCRPCE2[4:0]; // DEFAULT IS 00H (None) */

/***********************************************************************************************************/
/*    Eng. Integration Control Register for PCIe 0 (INTCRPCE0)                                             */
/***********************************************************************************************************/
#define INTCRPCE0                            (GCR_BASE_ADDR + 0x130),  MEM,    32         /* Offset: 130h */
#define INTCRPCE0_PCS_TX_DEEMPH_GEN2_3P5DB = INTCRPCE0[5:0]; // DEFAULT IS 18H 0,    32               /* 0-31 PCS_TX_DEEMPH_GEN2_3P5DB = INTCRPCE0[5:0]; // DEFAULT IS 18H (None) */

/***********************************************************************************************************/
/*    Eng. Integration Control Register for PCIe 1 (INTCRPCE1)                                             */
/***********************************************************************************************************/
#define INTCRPCE1                            (GCR_BASE_ADDR + 0x134),  MEM,    32         /* Offset: 134h */
#define INTCRPCE1_MAC0_PCLKREQ_N = INTCRPCE1[1:0]; // DEFAULT IS 00 0,    32               /* 0-31 MAC0_PCLKREQ_N = INTCRPCE1[1:0]; // DEFAULT IS 00 (None) */

/***********************************************************************************************************/
/*    Eng. Integration Control Register for PCIe 3 (INTCRPCE3)                                             */
/***********************************************************************************************************/
#define INTCRPCE3                            (GCR_BASE_ADDR + 0x128),  MEM,    32         /* Offset: 128h */
#define INTCRPCE3_DATAPATH PIE-8 COMPATIBILITY MODE 16,   1               /* 16 DATAPATH PIE-8 COMPATIBILITY MODE (None) */
#define INTCRPCE3_SIGNAL EQUALIZATION PROBLEM 15,   1               /* 15 SIGNAL EQUALIZATION PROBLEM (upstream device only) */
#define INTCRPCE3_DISABLE PHY STATUS TIMEOUT  14,   1               /* 14 DISABLE PHY STATUS TIMEOUT (None) */
#define INTCRPCE3_SET SELECTABLE DEEMPHASIS BIT IN TRANSMITTED TS2 ORDERED SET 8,    1               /* 8 SET SELECTABLE DEEMPHASIS BIT IN TRANSMITTED TS2 ORDERED SET (upstream devices only) */
#define INTCRPCE3_SET COMPLIANCE RECEIVE BIT IN TRANSMITTED TS1ORDERED SET 7,    1               /* 7 SET COMPLIANCE RECEIVE BIT IN TRANSMITTED TS1ORDERED SET (None) */
#define INTCRPCE3_DISABLE SCRAMBLING          6,    1               /* 6 DISABLE SCRAMBLING (None) */
#define INTCRPCE3_ENABLE ERROR INJECTION      5,    1               /* 5 ENABLE ERROR INJECTION (None) */
#define INTCRPCE3_ENABLE INFORMATION ASSERTIONS 4,    1               /* 4 ENABLE INFORMATION ASSERTIONS (None) */
#define INTCRPCE3_ENABLE WARNING ASSERTIONS   3,    1               /* 3 ENABLE WARNING ASSERTIONS (None) */
#define INTCRPCE3_LOOPBACK MASTER: THIS SIGNAL MUST BE SET TO 1 TO DIRECT THE LINK TO LOOPBACK 2,    1               /* 2 LOOPBACK MASTER: THIS SIGNAL MUST BE SET TO 1 TO DIRECT THE LINK TO LOOPBACK (in Master mode) */
#define INTCRPCE3_DISABLE LOW POWER STATE NEGOTIATION: WHEN ASSERTED, THIS SIGNAL DISABLES ALL LOW POWER STATE NEGOTIATION 1,    1               /* 1 DISABLE LOW POWER STATE NEGOTIATION: WHEN ASSERTED, THIS SIGNAL DISABLES ALL LOW POWER STATE NEGOTIATION (None) */

/***********************************************************************************************************/
/*    Eng. Integration Control Register for PCIe 2B (INTCRPCE2B)                                           */
/***********************************************************************************************************/
#define INTCRPCE2B                           (GCR_BASE_ADDR + 0x150),  MEM,    32         /* Offset: 150h */
#define INTCRPCE2B_PHY_TX0_TERM_OFFSET = INTCRPCE2B[4:0]; // DEFAULT IS 00H 0,    32               /* 0-31 PHY_TX0_TERM_OFFSET = INTCRPCE2B[4:0]; // DEFAULT IS 00H (None) */

/***********************************************************************************************************/
/*    Eng. Integration Control Register for PCIe 0B (INTCRPCE0B)                                           */
/***********************************************************************************************************/
#define INTCRPCE0B                           (GCR_BASE_ADDR + 0x154),  MEM,    32         /* Offset: 154h */
#define INTCRPCE0B_PCS_TX_DEEMPH_GEN2_3P5DB = INTCRPCE0B[5:0]; // DEFAULT IS 18H 0,    32               /* 0-31 PCS_TX_DEEMPH_GEN2_3P5DB = INTCRPCE0B[5:0]; // DEFAULT IS 18H (None) */

/***********************************************************************************************************/
/*    Eng. Integration Control Register for PCIe 1B (INTCRPCE1B)                                           */
/***********************************************************************************************************/
#define INTCRPCE1B                           (GCR_BASE_ADDR + 0x158),  MEM,    32         /* Offset: 158h */
#define INTCRPCE1B_MAC0_PCLKREQ_N = INTCRPCE1B[1:0]; // DEFAULT IS 00 0,    32               /* 0-31 MAC0_PCLKREQ_N = INTCRPCE1B[1:0]; // DEFAULT IS 00 (None) */

/***********************************************************************************************************/
/*    Eng. Integration Control Register for PCIe 3B (INTCRPCE3B)                                           */
/***********************************************************************************************************/
#define INTCRPCE3B                           (GCR_BASE_ADDR + 0x15C),  MEM,    32         /* Offset: 15Ch */
#define INTCRPCE3B_DATAPATH PIE-8 COMPATIBILITY MODE 16,   1               /* 16 DATAPATH PIE-8 COMPATIBILITY MODE (None) */
#define INTCRPCE3B_SIGNAL EQUALIZATION PROBLEM 15,   1               /* 15 SIGNAL EQUALIZATION PROBLEM (upstream device only) */
#define INTCRPCE3B_DISABLE PHY STATUS TIMEOUT 14,   1               /* 14 DISABLE PHY STATUS TIMEOUT (None) */
#define INTCRPCE3B_SET SELECTABLE DEEMPHASIS BIT IN TRANSMITTED TS2 ORDERED SET 8,    1               /* 8 SET SELECTABLE DEEMPHASIS BIT IN TRANSMITTED TS2 ORDERED SET (upstream devices only) */
#define INTCRPCE3B_SET COMPLIANCE RECEIVE BIT IN TRANSMITTED TS1ORDERED SET 7,    1               /* 7 SET COMPLIANCE RECEIVE BIT IN TRANSMITTED TS1ORDERED SET (None) */
#define INTCRPCE3B_DISABLE SCRAMBLING         6,    1               /* 6 DISABLE SCRAMBLING (None) */
#define INTCRPCE3B_ENABLE ERROR INJECTION     5,    1               /* 5 ENABLE ERROR INJECTION (None) */
#define INTCRPCE3B_ENABLE INFORMATION ASSERTIONS 4,    1               /* 4 ENABLE INFORMATION ASSERTIONS (None) */
#define INTCRPCE3B_ENABLE WARNING ASSERTIONS  3,    1               /* 3 ENABLE WARNING ASSERTIONS (None) */
#define INTCRPCE3B_LOOPBACK MASTER: THIS SIGNAL MUST BE SET TO 1 TO DIRECT THE LINK TO LOOPBACK 2,    1               /* 2 LOOPBACK MASTER: THIS SIGNAL MUST BE SET TO 1 TO DIRECT THE LINK TO LOOPBACK (in Master mode) */
#define INTCRPCE3B_DISABLE LOW POWER STATE NEGOTIATION: WHEN ASSERTED, THIS SIGNAL DISABLES ALL LOW POWER STATE NEGOTIATION 1,    1               /* 1 DISABLE LOW POWER STATE NEGOTIATION: WHEN ASSERTED, THIS SIGNAL DISABLES ALL LOW POWER STATE NEGOTIATION (None) */
#endif

/***********************************************************************************************************/
/*    Scratchpad Register (SCRPAD) (was ANTEST)                                                            */
/***********************************************************************************************************/
#define SCRPAD                               (GCR_BASE_ADDR + 0x13C),  MEM,    32         /* Offset: 13Ch */
#define SCRPAD_SP                             0,    32               /* 0-31 SP (Scratchpad) */

/***********************************************************************************************************/
/*    USB PHY 1 Control Register (USB1PHYCTL) (USB1TEST)                                                   */
/***********************************************************************************************************/
#define USB1PHYCTL                           (GCR_BASE_ADDR + 0x140),  MEM,    32        /* Offset: 140h */
#define USB1PHYCTL_INS                        31,   1               /* 31 INS (Insert Enable) */
#define USB1PHYCTL_RS                         28,   1               /* 28 RS (Reset Sequence) */
#define USB1PHYCTL_TXPREEMPH                  9,    2               /* 9-10 TXPREEMPH (Transmit Pre-emphasis) */

/***********************************************************************************************************/
/*    USB PHY 2 Control Register (USB2PHYCTL) (USB2TEST)                                                   */
/***********************************************************************************************************/
#define USB2PHYCTL                           (GCR_BASE_ADDR + 0x144),  MEM,    32        /* Offset: 144h */
#define USB2PHYCTL_RS                         28,   1               /* 28 RS (Reset Sequence) */
#define USB2PHYCTL_TXPREEMPH                  9,    2               /* 9-10 TXPREEMPH (Transmit Pre-emphasis) */

/***********************************************************************************************************/
/*    USB PHY 3 Control Register (USB3PHYCTL) (USB3TEST)                                                   */
/***********************************************************************************************************/
#define USB3PHYCTL                           (GCR_BASE_ADDR + 0x148),  MEM,    32        /* Offset: 148h */
#define USB3PHYCTL_RS                         28,   1               /* 28 RS (Reset Sequence) */
#define USB3PHYCTL_TXPREEMPH                  9,    2               /* 9-10 TXPREEMPH (Transmit Pre-emphasis) */

/***********************************************************************************************************/
/*    RAM Read and Write Margin Registers 1-14 (RW_MARGIN1 to RW_MARGIN14)                                 */
/***********************************************************************************************************/
#define RW_MARGIN1_14(n)         (GCR_BASE_ADDR(m) + (0x180 + (4 * n))),  MEM,    32         /* Offset: 180h-1B4h */
#define RW_MARGIN1_14_RAM_RW_MARGIN 0,    32               /* 0-31 RAM_RW_MARGIN (RAM Margin) */

/***********************************************************************************************************/
/*    Watchdog 0 Reset Control Register Lock(WD0RCRLK)                                                     */
/***********************************************************************************************************/
#define WD0RCRLK                             (GCR_BASE_ADDR + 0x400),  CLK_ACCESS,    32

/***********************************************************************************************************/
/*    Watchdog 1 Reset Control Register Lock (WD1RCRLK)                                                    */
/***********************************************************************************************************/
#define WD1RCRLK                             (GCR_BASE_ADDR + 0x404),  CLK_ACCESS,    32

/***********************************************************************************************************/
/*    Watchdog 2 Reset Control Register Lock (WD2RCRLK)                                                    */
/***********************************************************************************************************/
#define WD2RCRLK                             (GCR_BASE_ADDR + 0x408),  CLK_ACCESS,    32

/***********************************************************************************************************/
/*    Software Reset Control Register 1 Lock (SWRSTC1LK)                                                   */
/***********************************************************************************************************/
#define SWRSTC1LK                            (GCR_BASE_ADDR + 0x40C),  CLK_ACCESS,    32

/***********************************************************************************************************/
/*    Software Reset Control Register 2 Lock (SWRSTC2LK)                                                   */
/***********************************************************************************************************/
#define SWRSTC2LK                            (GCR_BASE_ADDR + 0x410),  CLK_ACCESS,    32

/***********************************************************************************************************/
/*    Software Reset Control Register 3 Lock (SWRSTC3LK)                                                   */
/***********************************************************************************************************/
#define SWRSTC3LK                            (GCR_BASE_ADDR + 0x414),  CLK_ACCESS,    32

/***********************************************************************************************************/
/*    TIP Reset Control Register Lock (TIPRSTCLK)                                                          */
/***********************************************************************************************************/
#define TIPRSTCLK                            (GCR_BASE_ADDR + 0x418),  CLK_ACCESS,    32

/***********************************************************************************************************/
/*    CORST Control Register Lock (CORSTCLK)                                                               */
/***********************************************************************************************************/
#define CORSTCLK                             (GCR_BASE_ADDR + 0x41C),  CLK_ACCESS,    32

/***********************************************************************************************************/
/*    Watchdog 0 Reset Control Register B Lock (WD0RCRBLK)                                                 */
/***********************************************************************************************************/
#define WD0RCRBLK                            (GCR_BASE_ADDR + 0x420),  CLK_ACCESS,    32

/***********************************************************************************************************/
/*    Watchdog 1 Reset Control Register B Lock (WD1RCRBLK)                                                 */
/***********************************************************************************************************/
#define WD1RCRBLK                            (GCR_BASE_ADDR + 0x424),  CLK_ACCESS,    32

/***********************************************************************************************************/
/*    Watchdog 2 Reset Control Register B Lock (WD2RCRBLK)                                                 */
/***********************************************************************************************************/
#define WD2RCRBLK                            (GCR_BASE_ADDR + 0x428),  CLK_ACCESS,    32

/***********************************************************************************************************/
/*    Software Reset Control Register 1 B Lock (SWRSTC1BLK)                                                */
/***********************************************************************************************************/
#define SWRSTC1BLK                           (GCR_BASE_ADDR + 0x42C),  CLK_ACCESS,    32

/***********************************************************************************************************/
/*    Software Reset Control Register 2 B Lock (SWRSTC2BLK)                                                */
/***********************************************************************************************************/
#define SWRSTC2BLK                           (GCR_BASE_ADDR + 0x430),  CLK_ACCESS,    32

/***********************************************************************************************************/
/*    Software Reset Control Register 3 B Lock (SWRSTC3BLK)                                                */
/***********************************************************************************************************/
#define SWRSTC3BLK                           (GCR_BASE_ADDR + 0x434),  CLK_ACCESS,    32

/***********************************************************************************************************/
/*    TIP Reset Control Register B Lock (TIPRSTCBLK)                                                       */
/***********************************************************************************************************/
#define TIPRSTCBLK                           (GCR_BASE_ADDR + 0x438),  CLK_ACCESS,    32

/***********************************************************************************************************/
/*    CORST Control Register B Lock (CORSTCBLK)                                                            */
/***********************************************************************************************************/
#define CORSTCBLK                            (GCR_BASE_ADDR + 0x43C),  CLK_ACCESS,    32

/***********************************************************************************************************/
/*    Scratch Pad Register 10-41 (SCRPAD_10-41)                                                            */
/***********************************************************************************************************/
#define SCRPAD_10_41(n)                      (GCR_BASE_ADDR + (0xE00 + (4 * n))),  MEM,    32         /* Offset: E00h-E7Ch */
#define SCRPAD_10_41_SP                       0,    32               /* 0-31 SP (Scratch Pad) */

/***********************************************************************************************************/
/*    Scratch Pad Register 42-73 (SCRPAD_42-73)                                                            */
/***********************************************************************************************************/
#define SCRPAD_42_73(n)                      (GCR_BASE_ADDR + (n == 0 ? 0xE80 : (n == 1 ? 0xE84 : (n == 2 ? 0xE88 : (n == 3 ? 0xE8C : (n == 4 ? 0xE90 : (n == 5 ? 0xE94 : (n == 6 ? 0xE98 : (n == 7 ? 0xE9C : (n == 8 ? 0xEA0 : (n == 9 ? 0xEA4 : (n == 10 ? 0xEA8 : (n == 11 ? 0xEAC : (n == 12 ? 0xEB0 : (n == 13 ? 0xEB4 : (n == 14 ? 0xEB8 : (n == 15 ? 0xEBC : (n == 16 ? 0xEC0 : (n == 17 ? 0xEC4 : (n == 18 ? 0xEC8 : (n == 19 ? 0xECC : (n == 20 ? 0xED0 : (n == 21 ? 0xED54 : (n == 22 ? 0xED8 : (n == 23 ? 0xEDC : (n == 24 ? 0xEE0 : (n == 25 ? 0xEE4 : (n == 26 ? 0xEE8 : (n == 27 ? 0xEE6C : (n == 28 ? 0xEF0 : (n == 29 ? 0xEF4 : (n == 30 ? 0xEF8 : 0xEFC)))))))))))))))))))))))))))))))),  MEM,    32         /* Offset: E80h-EFCh */
#define SCRPAD_42_73_SP                       0,    32               /* 0-31 SP (Scratch Pad) */

/***********************************************************************************************************/
/*    Semaphore Register 00-31 (GP_SEMFR_00-31)                                                            */
/***********************************************************************************************************/
#define GP_SEMFR_00_31(n)                    (GCR_BASE_ADDR + (n == 0 ? 0xF00 : (n == 1 ? 0xF04 : (n == 2 ? 0xF08 : (n == 3 ? 0xE0C : (n == 4 ? 0xF10 : (n == 5 ? 0xF14 : (n == 6 ? 0xF18 : (n == 7 ? 0xF1C : (n == 8 ? 0xF20 : (n == 9 ? 0xF24 : (n == 10 ? 0xF28 : (n == 11 ? 0xF2C : (n == 12 ? 0xF30 : (n == 13 ? 0xF34 : (n == 14 ? 0xF38 : (n == 15 ? 0xF3C : (n == 16 ? 0xF40 : (n == 17 ? 0xF44 : (n == 18 ? 0xF48 : (n == 19 ? 0xF4C : (n == 20 ? 0xF50 : (n == 21 ? 0xF54 : (n == 22 ? 0xF58 : (n == 23 ? 0xF5C : (n == 24 ? 0xF60 : (n == 25 ? 0xF64 : (n == 26 ? 0xF68 : (n == 27 ? 0xF6C : (n == 28 ? 0xF70 : (n == 29 ? 0xF74 : (n == 30 ? 0xF78 : 0xF7C)))))))))))))))))))))))))))))))),  MEM,    32         /* Offset: F00h-F7Ch */
#define GP_SEMFR_00_31_SEM                    0,    4               /* 0-3 SEM (Semaphore) */

/***********************************************************************************************************/
/*    Coprocessor 1 Control Register (CPCTL1)                                                              */
/***********************************************************************************************************/
#define CPCTL1                               (GCR_CP_CTRL_BASE_ADDR + 0x000),  MEM,    32         /* Offset: 000h */
#define CPCTL1_CP_FUSTRAP8_1                  16,   8               /* 16_23 CP_FUSTRAP8_1 (Coprocessor 1 FUSTRAP8-1) */
#define CPCTL1_GPSCRPAD                       0,    6               /* 0-5 GPSCRPAD (None) */

/***********************************************************************************************************/
/*    Coprocessor 1 to BMC Core Status Register (CP2BST1)                                                  */
/***********************************************************************************************************/
#define CP2BST1                              (GCR_CP_CTRL_BASE_ADDR + 0x004),  MEM,    32         /* Offset: 004h */
#define CP2BST1_CP2BST                        0,    32               /* 0-31 CP2BST (Coprocessor to BMC Core Status) */

/***********************************************************************************************************/
/*    BMC Core to Coprocessor 1 Notification Register (B2CPNT1)                                            */
/***********************************************************************************************************/
#define B2CPNT1                              (GCR_CP_CTRL_BASE_ADDR + 0x008),  MEM,    32         /* Offset: 008h */
#define B2CPNT1_B2CPNT                        0,    32               /* 0-31 B2CPNT (BMC to Coprocessor Core Notification) */

/***********************************************************************************************************/
/*    Coprocessor 1 Protection Control Register (CPPCTL1)                                                  */
/***********************************************************************************************************/
#define CPPCTL1                              (GCR_CP_CTRL_BASE_ADDR + 0x00C),  MEM,    32         /* Offset: 00Ch */
#define CPPCTL1_CPD3L                         3,    1               /* 3 CPD3L (Coprocessor and GDMA AHB3 Lock) */
#define CPPCTL1_CPCL                          0,    1               /* 0 CPCL (Coprocessor Control Registers Lock) */

/***********************************************************************************************************/
/*    Coprocessor Boot Pointer Register (CPBPNTR1)                                                         */
/***********************************************************************************************************/
#define CPBPNTR1                             (GCR_CP_CTRL_BASE_ADDR + 0x010),  MEM,    32         /* Offset: 010h */
#define CPBPNTR1_CPBOOTPNT                    0,    32               /* 0-31 CPBOOTPNT (Coprocessor Boot Pointer) */

/***********************************************************************************************************/
/*    Coprocessor 2 (TIP) Control Register (CPCTL2)                                                        */
/***********************************************************************************************************/
#define CPCTL2                               (GCR_TIP_CTRL_BASE_ADDR + 0x000),  MEM,    32         /* Offset: 000h */
#define CPCTL2_CP_FUSTRAP8_1                  16,   8               /* 16-23 CP_FUSTRAP8-1 (Coprocessor 2 FUSTRAP8-1) */
#define CPCTL2_GPSCRPAD                       0,    6               /* 0-5 GPSCRPAD (None) */

/***********************************************************************************************************/
/*    Coprocessor 2 (TIP) to BMC Core Status Register (CP2BST2)                                            */
/***********************************************************************************************************/
#define CP2BST2                              (GCR_TIP_CTRL_BASE_ADDR + 0x004),  MEM,    32         /* Offset: 004h */
#define CP2BST2_CP2BST                        0,    32               /* 0-31 CP2BST (Coprocessor to BMC Core Status) */

/***********************************************************************************************************/
/*    BMC Core to Coprocessor 2 (TIP) Notification Register (B2CPNT2)                                      */
/***********************************************************************************************************/
#define B2CPNT2                              (GCR_TIP_CTRL_BASE_ADDR + 0x008),  MEM,    32         /* Offset: 008h */
#define B2CPNT2_B2CPNT                        0,    32               /* 0-31 B2CPNT (BMC to Coprocessor Core Notification) */

/***********************************************************************************************************/
/*    Coprocessor 2 (TIP) Scratchpad Register (CPSP2)                                                      */
/***********************************************************************************************************/
#define CPSP2                                (GCR_TIP_CTRL_BASE_ADDR + 0x010),  MEM,    32         /* Offset: 010h */
#define CPSP2_SP                              0,    32               /* 0-31 SP (Scratchpad) */


#endif //__NPCM850_REGS_H__
