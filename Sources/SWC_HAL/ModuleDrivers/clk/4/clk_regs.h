/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   clk_regs.h                                                               */
/*            This file contains definitions of CLK registers                 */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/

#ifndef CLK_REGS_H
#define CLK_REGS_H

#include __CHIP_H_FROM_DRV()


/***********************************************************************************************************/
/*    Clock Enable 1 Register (CLKEN1)                                                                     */
/***********************************************************************************************************/
#define CLKEN1                               (CLK_BASE_ADDR + 0x00),  CLK_ACCESS,    32         /* Offset: 00h */
#define CLKEN1_SMB1                           31,   1               /* 31 SMB1 (SMBus Interface 1 Clock Enable Bit) */
#define CLKEN1_SMB0                           30,   1               /* 30 SMB0 (SMBus Interface 0 Clock Enable Bit) */
#define CLKEN1_SMB7                           29,   1               /* 29 SMB7 (SMBus Interface 7 Clock Enable Bit) */
#define CLKEN1_SMB6                           28,   1               /* 28 SMB6 (SMBus Interface 6 Clock Enable Bit) */
#define CLKEN1_ADC                            27,   1               /* 27 ADC (ADC Clock Enable Bit) */
#define CLKEN1_WDT                            26,   1               /* 26 WDT (All Watchdog Timers Clock Enable Bit) */
#define CLKEN1_USBDEV3                        25,   1               /* 25 USBDEV3 (USB Device 3 Clock Enable Bit) */
#define CLKEN1_USBDEV6                        24,   1               /* 24 USBDEV6 (USB Device 6 Clock Enable Bit) */
#define CLKEN1_USBDEV5                        23,   1               /* 23 USBDEV5 (USB Device 5 Clock Enable Bit) */
#define CLKEN1_USBDEV4                        22,   1               /* 22 USBDEV4 (USB Device 4 Clock Enable Bit) */
#define CLKEN1_GMAC4                          21,   1               /* 21 GMAC4 (GMAC4 Clock Enable Bit) */
#define CLKEN1_TIMER5_9                       20,   1               /* 20 TIMER5-9 (Timer 5 to Timer 9 (in Timer Module 1) */
#define CLKEN1_TIMER0_4                       19,   1               /* 19 TIMER0-4 (Timer 0 to Timer 4 (in Timer Module 0) */
#define CLKEN1_PWMM0                          18,   1               /* 18 PWMM0 (PWM Module 0 Clock Enable Bit) */
#define CLKEN1_HUART                          17,   1               /* 17 HUART (HUART Clock Enable Bit) */
#define CLKEN1_SMB5                           16,   1               /* 16 SMB5 (SMBus Interface 5 Clock Enable Bit) */
#define CLKEN1_SMB4                           15,   1               /* 15 SMB4 (SMBus Interface 4 Clock Enable Bit) */
#define CLKEN1_SMB3                           14,   1               /* 14 SMB3 (SMBus Interface 3 Clock Enable Bit) */
#define CLKEN1_SMB2                           13,   1               /* 13 SMB2 (SMBus Interface 2 Clock Enable Bit) */
#define CLKEN1_MC                             12,   1               /* 12 MC (DDR4 CLK_ACCESSory Controller Clock Enable Bit) */
#define CLKEN1_UART01                         11,   1               /* 11 UART01 (UART0 and UART1 Clock Enable Bit) */
#define CLKEN1_AES                            10,   1               /* 10 AES (AES Clock Enable Bit) */
#define CLKEN1_PECI                           9,    1               /* 9 PECI (PECI Clock Enable Bit) */
#define CLKEN1_USBDEV2                        8,    1               /* 8 USBDEV2 (USB Device 2 Clock Enable Bit) */
#define CLKEN1_UART23                         7,    1               /* 7 UART23 (UART2 and UART3 Clock Enable Bit) */
#define CLKEN1_GMAC3                          6,    1               /* 6 GMAC3 (GMAC3 Clock Enable Bit) */
#define CLKEN1_USBDEV1                        5,    1               /* 5 USBDEV1 (USB Device 1 Clock Enable Bit) */
#define CLKEN1_SHM                            4,    1               /* 4 SHM (SHM Clock Enable Bit) */
#define CLKEN1_GDMA0                          3,    1               /* 3 GDMA0 (GDMA0 Clock Enable Bit) */
#define CLKEN1_KCS                            2,    1               /* 2 KCS (KCS Clock Enable Bit) */
#define CLKEN1_SPI3                           1,    1               /* 1 CLK_SPI3 (FIU3 Clock Enable Bit) */
#define CLKEN1_SPI0                           0,    1               /* 0 CLK_SPI0 (FIU0 Clock Enable Bit) */

/***********************************************************************************************************/
/*    Clock Enable 2 Register (CLKEN2)                                                                     */
/***********************************************************************************************************/
#define CLKEN2                               (CLK_BASE_ADDR + 0x28),  CLK_ACCESS,    32         /* Offset: 28h */
#define CLKEN2_CP1                            31,   1               /* 31 CP1 (Coprocessor 1 Clock Enable Bit) */
#define CLKEN2_TOCK                           30,   1               /* 30 TOCK (TOCK Clock Enable Bit) */
#define CLKEN2_GMAC1                          28,   1               /* 28 GMAC1 (Gigabit MAC 1 Module Clock Enable Bit) */
#define CLKEN2_USBIF                          27,   1               /* 27 USBIF (HUB, UTMI and OHCI Clock Enable Bit) */
#define CLKEN2_USBH1                          26,   1               /* 26 USBH1 (USB Host 1 Clock Enable Bit) */
#define CLKEN2_GMAC2                          25,   1               /* 25 GMAC2 (Gigabit MAC 2 Module Clock Enable Bit) */
#define CLKEN2_SPI1                           24,   1               /* 24 SPI1 (FIU 1 Clock Enable Bit) */
#define CLKEN2_PSPI2                          23,   1               /* 23 PSPI2 (Peripheral SPI 2 Clock Enable Bit) */
#define CLKEN2_3DES                           21,   1               /* 21 3DES (3DES Module Clock Enable Bit) */
#define CLKEN2_BT                             20,   1               /* 20 BT (Block Transfer Module Clock Enable Bit) */
#define CLKEN2_SIOX2                          19,   1               /* 19 SIOX2 (Serial GPIO Expander 2 Clock Enable Bit) */
#define CLKEN2_SIOX1                          18,   1               /* 18 SIOX1 (Serial GPIO Expander 1 Clock Enable Bit) */
#define CLKEN2_VIRUART2                       17,   1               /* 17 VIRUART2 (Virtual UART 2 Clock Enable Bit) */
#define CLKEN2_VIRUART1                       16,   1               /* 16 VIRUART1 (Virtual UART 1 Clock Enable Bit) */
#define CLKEN2_VCD                            14,   1               /* 14 VCD (Video Capture and Differentiate Module Clock Enable Bit) */
#define CLKEN2_ECE                            13,   1               /* 13 ECE (Encoding Compression Module Clock Enable Bit) */
#define CLKEN2_VDMA                           12,   1               /* 12 VDMA (VDM DMA Clock Enable Bit) */
#define CLKEN2_AHBPCIBRG                      11,   1               /* 11 AHBPCIBRG (AHB-to-PCI Bridge Clock Enable Bit) */
#define CLKEN2_GFXSYS                         10,   1               /* 10 GFXSYS (Graphics System Clock Enable Bit) */
#define CLKEN2_MMC                            8,    1               /* 8 MMC (Multimedia Card Host Controller Clock Enable Bit) */
#define CLKEN2_MFT7_0                         0,    8               /* 0-7 MFT7-0 (Tachometer Module 7-0 Clock Enable Bits) */

/***********************************************************************************************************/
/*    Clock Enable 3 Register (CLKEN3)                                                                     */
/***********************************************************************************************************/
#define CLKEN3                               (CLK_BASE_ADDR + 0x30),  CLK_ACCESS,    32         /* Offset: 30h */
#define CLKEN3_GPIOM7                         31,   1               /* 31 GPIOM7 (GPIO Module 7 Clock Enable Bit) */
#define CLKEN3_GPIOM6                         30,   1               /* 30 GPIOM6 (GPIO Module 6 Clock Enable Bit) */
#define CLKEN3_GPIOM5                         29,   1               /* 29 GPIOM5 (GPIO Module 5 Clock Enable Bit) */
#define CLKEN3_GPIOM4                         28,   1               /* 28 GPIOM4 (GPIO Module 4 Clock Enable Bit) */
#define CLKEN3_GPIOM3                         27,   1               /* 27 GPIOM3 (GPIO Module 3 Clock Enable Bit) */
#define CLKEN3_GPIOM2                         26,   1               /* 26 GPIOM2 (GPIO Module 2 Clock Enable Bit) */
#define CLKEN3_GPIOM1                         25,   1               /* 25 GPIOM1 (GPIO Module 1 Clock Enable Bit) */
#define CLKEN3_GPIOM0                         24,   1               /* 24 GPIOM0 (GPIO Module 0 Clock Enable Bit) */
#define CLKEN3_ESPI                           23,   1               /* 23 ESPI (ESPI and FIU-EC Module Clock Enable Bit) */
#define CLKEN3_SMB11                          22,   1               /* 22 SMB11 (SMBus Interface 11 Clock Enable Bit) */
#define CLKEN3_SMB10                          21,   1               /* 21 SMB10 (SMBus Interface 10 Clock Enable Bit) */
#define CLKEN3_SMB9                           20,   1               /* 20 SMB9 (SMBus Interface 9 Clock Enable Bit) */
#define CLKEN3_SMB8                           19,   1               /* 19 SMB8 (SMBus Interface 8 Clock Enable Bit) */
#define CLKEN3_SMB15                          18,   1               /* 18 SMB15 (SMBus Interface 15 Clock Enable Bit) */
#define CLKEN3_RNG                            17,   1               /* 17 RNG (Random Number Generator Clock Enable Bit) */
#define CLKEN3_TIMER10_14                     16,   1               /* 16 TIMER10-14 (Timer 10 to Timer 14 (in Timer module 2) */
#define CLKEN3_PCIERC                         15,   1               /* 15 PCIERC (PCI Express Root Complex Clock Enable Bit) */
#define CLKEN3_SECECC                         14,   1               /* 14 SECECC (RSA, ECC and MODP accelerator Clock Enable Bit) */
#define CLKEN3_SHA                            13,   1               /* 13 SHA (SHA-1 and SHA-256 Module Clock Enable Bit) */
#define CLKEN3_SMB14                          12,   1               /* 12 SMB14 (SMBus Interface 14 Clock Enable Bit) */
#define CLKEN3_GDMA2                          11,   1               /* 11 GDMA2 (General Purpose DMA 2 Clock Enable bit) */
#define CLKEN3_GDMA1                          10,   1               /* 10 GDMA1 (General Purpose DMA 1 Clock Enable Bit) */
#define CLKEN3_PCIMBX                         9,    1               /* 9 PCIMBX (PCI Mailbox Clock Enable Bit) */
#define CLKEN3_USBDEV9                        7,    1               /* 7 USBDEV9 (USB Device 9 Clock Enable Bit) */
#define CLKEN3_USBDEV8                        6,    1               /* 6 USBDEV8 (USB Device 8 Clock Enable Bit) */
#define CLKEN3_USBDEV7                        5,    1               /* 5 USBDEV7 (USB Device 7 Clock Enable Bit) */
#define CLKEN3_USBDEV0                        4,    1               /* 4 USBDEV0 (USB Device 0 Clock Enable Bit) */
#define CLKEN3_SMB13                          3,    1               /* 3 SMB13 (SMBus Interface 13 Clock Enable Bit) */
#define CLKEN3_SPIX                           2,    1               /* 2 SPIX (SPIX Clock Enable Bit) */
#define CLKEN3_SMB12                          1,    1               /* 1 SMB12 (SMBus Interface 12 Clock Enable Bit) */
#define CLKEN3_PWMM1                          0,    1               /* 0 PWMM1 (PWM Module 1 Clock Enable Bit) */

/***********************************************************************************************************/
/*    Clock Enable 4 Register (CLKEN4)                                                                     */
/***********************************************************************************************************/
#define CLKEN4                               (CLK_BASE_ADDR + 0x70),  CLK_ACCESS,    32         /* Offset: 70h */
#define CLKEN4_USBH2                          31,   1               /* 31 USBH2 (USB Host 1 Clock Enable Bit) */
#define CLKEN4_JTM2                           30,   1               /* 30 JTM2 (JTAG Master 1 Clock Enable Bit) */
#define CLKEN4_JTM1                           29,   1               /* 29 JTM1 (JTAG Master 2 Clock Enable Bit) */
#define CLKEN4_PWMM2                          28,   1               /* 28 PWMM2 (PWM Module 2 Clock Enable Bit) */
#define CLKEN4_SMB26                          24,   1               /* 24 SMB26 (SMB Interface 26 Clock Enable Bit) */
#define CLKEN4_SMB25                          23,   1               /* 23 SMB25 (SMB Interface 25 Clock Enable Bit) */
#define CLKEN4_SMB24                          22,   1               /* 22 SMB24 (SMB Interface 24 Clock Enable Bit) */
#define CLKEN4_UART6                          18,   1               /* 18 UART6 (UART6 Clock Enable Bit) */
#define CLKEN4_UART5                          17,   1               /* 17 UART5 (UART5 Clock Enable Bit) */
#define CLKEN4_UART4                          16,   1               /* 16 UART4 (UART4 Clock Enable Bit) */
#define CLKEN4_I3C5                           13,   1               /* 13 I3C5 (I3C Controller 5 Clock Enable Bit) */
#define CLKEN4_I3C4                           12,   1               /* 12 I3C4 (I3C Controller 4 Clock Enable Bit) */
#define CLKEN4_I3C3                           11,   1               /* 11 I3C3 (I3C Controller 3 Clock Enable Bit) */
#define CLKEN4_I3C2                           10,   1               /* 10 I3C2 (I3C Controller 2 Clock Enable Bit) */
#define CLKEN4_I3C1                           9,    1               /* 9 I3C1 (I3C Controller 1 Clock Enable Bit) */
#define CLKEN4_I3C0                           8,    1               /* 8 I3C0 (I3C Controller 0 Clock Enable Bit) */
#define CLKEN4_SMB23                          7,    1               /* 7 SMB23 (SMBus Interface 23 Clock Enable Bit) */
#define CLKEN4_SMB22                          6,    1               /* 6 SMB22 (SMBus Interface 22 Clock Enable Bit) */
#define CLKEN4_SMB21                          5,    1               /* 5 SMB21 (SMBus Interface 21 Clock Enable Bit) */
#define CLKEN4_SMB20                          4,    1               /* 4 SMB20 (SMBus Interface 20 Clock Enable Bit) */
#define CLKEN4_SMB19                          3,    1               /* 3 SMB19 (SMBus Interface 19 Clock Enable Bit) */
#define CLKEN4_SMB18                          2,    1               /* 2 SMB18 (SMBus Interface 18 Clock Enable Bit) */
#define CLKEN4_SMB17                          1,    1               /* 1 SMB17 (SMBus Interface 17 Clock Enable Bit) */
#define CLKEN4_SMB16                          0,    1               /* 0 SMB16 (SMBus Interface 16 Clock Enable Bit) */

/***********************************************************************************************************/
/*    Clock Select Register (CLKSEL)                                                                       */
/***********************************************************************************************************/
#define CLKSEL                               (CLK_BASE_ADDR + 0x04),  CLK_ACCESS,    32         /* Offset: 04h */
#define CLKSEL_RCPCKSEL                       27,   2               /* 27-28 RCPCKSEL (Root Complex PHY Clock Source Select Bit) */
#define CLKSEL_RGSEL                          25,   2               /* 25-26 RGSEL (RGMII GMAC Clock Source Select Bit) */
#define CLKSEL_AHB6SSEL                       23,   2               /* 23-24 AHB6SSEL (AHB6 Clock Source Select Bit) */
#define CLKSEL_GFXMSEL                        21,   2               /* 21-22 GFXMSEL (Graphics CLK_ACCESSory Clock Source Select Bit) */
#define CLKSEL_CLKOUTSEL                      18,   3               /* 18-20 CLKOUTSEL (CLKOUT signal Clock Source Select Bit) */
#define CLKSEL_PCIGFXCKSEL                    16,   2               /* 16-17 PCIGFXCKSEL (PCIGraphics System Clock Source Select Bit) */
#define CLKSEL_ADCCKSEL                       14,   2               /* 14-15 ADCCKSEL (ADC Clock Source Select Bit) */
#define CLKSEL_MCCKSEL                        12,   2               /* 12-13 MCCKSEL (CLK_ACCESSory Controller Clock Source Select Bit) */
#define CLKSEL_SUCKSEL                        10,   2               /* 10-11 SUCKSEL (USB Serial Clock Source Select Bit) */
#define CLKSEL_UARTCKSEL                      8,    2               /* 8-9 UARTCKSEL (Core and Host UART Clock Source Select Bit) */
#define CLKSEL_SDCKSEL                        6,    2               /* 6-7 SDCKSEL (SDHC Clock Source Select Bit) */
#define CLKSEL_PIXCKSEL                       4,    2               /* 4-5 PIXCKSEL (Pixel Clock Source Select Bit) */
#define CLKSEL_CPUCKSEL                       0,    3               /* 0-2 CPUCKSEL (CPU, Bus, and AHB System Clock Source Select Bit) */

/***********************************************************************************************************/
/*    Clock Divider Control Register 1 (CLKDIV1)                                                           */
/***********************************************************************************************************/
#define CLKDIV1                              (CLK_BASE_ADDR + 0x08),  CLK_ACCESS,    32
#define CLKDIV1_ADCCKDIV                      28,   3               /* 28-30 ADCCKDIV (ADC Clock Divider Control) */
#define CLKDIV1_CLK4DIV                       26,   2               /* 26-27 CLK4DIV (AMBA AHB Clock Divider Control) */
#define CLKDIV1_PRE_ADCCKDIV                  21,   5               /* 21-25 PRE-ADCCKDIV (PRE-ADC Clock Source Divider Control) */
#define CLKDIV1_UARTDIV1                      16,   5               /* 16-20 UARTDIV1 (UART Clock Source Divider Control 1) */
#define CLKDIV1_MMCCKDIV                      11,   5               /* 11-15 MMCCKDIV (MMC Controller (SDHC2) */
#define CLKDIV1_AHB3CKDIV                     6,    5               /* 6-10 AHB3CKDIV (CLK_SPI3 Clock Divider Control) */
#define CLKDIV1_PCICKDIV                      2,    4               /* 2-5 PCICKDIV (Internal PCI Clock Divider Control) */

/***********************************************************************************************************/
/*    Clock Divider Control Register 2 (CLKDIV2)                                                           */
/***********************************************************************************************************/
#define CLKDIV2                              (CLK_BASE_ADDR + 0x2C),  CLK_ACCESS,    32
#define CLKDIV2_APB4CKDIV                     30,   2               /* 30-31 APB4CKDIV (AMBA CLK_APB4 Clock Divider Control) */
#define CLKDIV2_APB3CKDIV                     28,   2               /* 28-29 APB3CKDIV (AMBA CLK_APB3 Clock Divider Control) */
#define CLKDIV2_APB2CKDIV                     26,   2               /* 26-27 APB2CKDIV (AMBA CLK_APB2 Clock Divider Control) */
#define CLKDIV2_APB1CKDIV                     24,   2               /* 24-25 APB1CKDIV (AMBA CLK_APB1 Clock Divider Control) */
#define CLKDIV2_APB5CKDIV                     22,   2               /* 22-23 APB5CKDIV (AMBA CLK_APB5 Clock Divider Control) */
#define CLKDIV2_CLKOUTDIV                     16,   5               /* 16-20 CLKOUTDIV (CLKOUT Pin Divider Control) */
#define CLKDIV2_GFXCKDIV                      13,   3               /* 13-15 GFXCKDIV*** (Graphics System Clock Divider Control) */
#define CLKDIV2_SUCKDIV                       8,    5               /* 8-12 SUCKDIV (‘Serial’ USB UTMI Bridge Clock Divider Control) */
#define CLKDIV2_SU48CKDIV                     4,    4               /* 4-7 SU48CKDIV (USB Host OHCI Clock Divider Control) */
#define CLKDIV2_SD1CKDIV                      0,    4               /* 0-3 SD1CKDIV (SDHC1 Clock Divider Control) */

/***********************************************************************************************************/
/*    Clock Divider Control Register 3 (CLKDIV3)                                                           */
/***********************************************************************************************************/
#define CLKDIV3                              (CLK_BASE_ADDR + 0x58),  CLK_ACCESS,    32
#define CLKDIV3_SPI1CKDIV                     16,   8               /* 16-23 SPI1CKDIV (SPI1 Clock Divider Control) */
#define CLKDIV3_UARTDIV2                      11,   5               /* 11-15 UARTDIV2 (UART Clock Source Divider Control 2) */
#define CLKDIV3_SPI0CKDIV                     6,    5               /* 6-10 SPI0CKDIV (CLK_SPI0 Clock Divider Control) */
#define CLKDIV3_SPIXCKDIV                     1,    5               /* 1-5 SPIXCKDIV (SPIX Clock Divider Control) */

/***********************************************************************************************************/
/*    Clock Divider Control Register 4 (CLKDIV4)                                                           */
/***********************************************************************************************************/
#define CLKDIV4                              (CLK_BASE_ADDR + 0x7C) ,  CLK_ACCESS , 32
#define CLKDIV4_RGREFDIV                      28,   4               /* 28-31 RGREFDIV (RGMII Reference Clock Divider Control) */
#define CLKDIV4_JTM2CKDV                      16,   12               /* 16-27 JTM2CKDV (JTAG Master 2 Clock Divider Control) */
#define CLKDIV4_RCPREFDIV                     12,   4               /* 12-15 RCPREFDIV (Root Complex PHY Reference Clock Divider Control) */
#define CLKDIV4_JTM1CKDV                      0,    12               /* 0-11 JTM1CKDV (JTAG Master 1 Clock Divider Control) */

/***********************************************************************************************************/
/*    PLL Control Register 0 (PLLCON0)                                                                     */
/***********************************************************************************************************/
#define PLLCON0                              (CLK_BASE_ADDR + 0x0C),  CLK_ACCESS,    32         /* Offset: 0Ch */
#define PLLCON0_HOLD_BIT                      0,    32               /* 0-31 HOLD_BIT (TBD) */

/***********************************************************************************************************/
/*    PLL Control Register 1 (PLLCON1)                                                                     */
/***********************************************************************************************************/
#define PLLCON1                              (CLK_BASE_ADDR + 0x10),  CLK_ACCESS,    32         /* Offset: 10h */
#define PLLCON1_HOLD_BIT                      0,    32               /* 0-31 HOLD_BIT (TBD) */

/***********************************************************************************************************/
/*    PLL Control Register 2 (PLLCON2)                                                                     */
/***********************************************************************************************************/
#define PLLCON2                              (CLK_BASE_ADDR + 0x54),  CLK_ACCESS,    32         /* Offset: 54h */
#define  PLLCONn_LOKI                    31 , 1              /* 31 LOKI (Lock Indication ).                                                                                           */
#define  PLLCONn_LOKS                    30 , 1              /* 30 LOKS (Unlock Indication Sticky bit ).                                                                              */
#define  PLLCONn_FBDV                    16 , 12            /* 27-16 FBDV (PLL VCO Output Clock Feedback Divider). The feedback divider divides the output clock from                */
#define  PLLCONn_OTDV2                   13 , 3             /* 15-13 OTDV2 (PLL Output Clock Divider 2). The output divider divides the VCO clock output. The divide value           */
#define  PLLCONn_PWDEN                   12 , 1              /* 12 PWDEN (Power-Down Mode Enable).                                                                                    */
#define  PLLCONn_OTDV1                   8 , 3              /* 10-8 OTDV1 (PLL Output Clock Divider 1). The output divider divides the VCO clock output. The divide value            */
#define  PLLCONn_INDV                    0 , 6              /* 5-0 INDV (PLL Input Clock Divider). The input divider divides the input reference clock into the PLL. The di-         */

/***********************************************************************************************************/
/*    Software Reset Register (SWRSTR)                                                                     */
/***********************************************************************************************************/
#define SWRSTR                               (CLK_BASE_ADDR + 0x14),  CLK_ACCESS,    32         /* Offset: 14h */
#define SWRSTR_SWRST3                         5,    1               /* 5 SWRST3 (Software Reset Control Bit 3) */
#define SWRSTR_SWRST2                         4,    1               /* 4 SWRST2 (Software Reset Control Bit 2) */
#define SWRSTR_SWRST1                         3,    1               /* 3 SWRST1 (Software Reset Control Bit 1) */

/***********************************************************************************************************/
/*    IP Software Reset Register 1 (IPSRST1)                                                               */
/***********************************************************************************************************/
#define IPSRST1                              (CLK_BASE_ADDR + 0x20),  CLK_ACCESS,    32         /* Offset: 20h */
#define IPSRST1_SMB1                          31,   1               /* 31 SMB1 (SMBus1 Interface Software Reset Control Bit) */
#define IPSRST1_SMB0                          30,   1               /* 30 SMB0 (SMBus0 Interface Software Reset Control Bit) */
#define IPSRST1_SMB7                          29,   1               /* 29 SMB7 (SMBus7 Interface Software Reset Control Bit) */
#define IPSRST1_SMB6                          28,   1               /* 28 SMB6 (SMBuS6 Interface Software Reset Control Bit) */
#define IPSRST1_ADC                           27,   1               /* 27 ADC (ADC Software Reset Control Bit) */
#define IPSRST1_USBDEV3                       25,   1               /* 25 USBDEV3 (USB Device 3 Controller Software Reset Control Bit) */
#define IPSRST1_USBDEV6                       24,   1               /* 24 USBDEV6 (USB Device 6 Controller Software Reset Control Bit) */
#define IPSRST1_USBDEV5                       23,   1               /* 23 USBDEV5 (USB Device 5 Controller Software Reset Control Bit) */
#define IPSRST1_USBDEV4                       22,   1               /* 22 USBDEV4 (USB Device 4 Controller Software Reset Control Bit) */
#define IPSRST1_GMAC4                         21,   1               /* 21 GMAC4 (GMAC4 Software Reset Control Bit) */
#define IPSRST1_TIM5_9                        20,   1               /* 20 TIM5-9 (Timer 5-9 and Watchdog 1 (Timer module 1) */
#define IPSRST1_TIM0_4                        19,   1               /* 19 TIM0-4 (Timer 0-4 and Watchdog 0 (Timer module 0) */
#define IPSRST1_PWMM0                         18,   1               /* 18 PWMM0 (PWM Module 0 Software Reset Control Bit) */
#define IPSRST1_SMB5                          16,   1               /* 16 SMB5 (SMBus5 Interface Software Reset Control Bit) */
#define IPSRST1_SMB4                          15,   1               /* 15 SMB4 (SMBus4 Interface Software Reset Control Bit) */
#define IPSRST1_SMB3                          14,   1               /* 14 SMB3 (SMBus3 Interface Software Reset Control Bit) */
#define IPSRST1_SMB2                          13,   1               /* 13 SMB2 (SMBus2 Interface Software Reset Control Bit) */
#define IPSRST1_MC                            12,   1               /* 12 MC (DDR4 CLK_ACCESSory Controller Software Reset Control Bit) */
#define IPSRST1_UART01                        11,   1               /* 11 UART01 (UARTs 0 and 1 Software Reset Control Bit) */
#define IPSRST1_AES                           10,   1               /* 10 AES (AES Module Software Reset Control Bit) */
#define IPSRST1_PECI                          9,    1               /* 9 PECI (PECI Module Software Reset Control Bit) */
#define IPSRST1_USBDEV2                       8,    1               /* 8 USBDEV2 (USB Device 2 Controller Software Reset Control Bit) */
#define IPSRST1_UART23                        7,    1               /* 7 UART23 (UART 2 and UART 3 Software Reset Control Bit) */
#define IPSRST1_GMAC3                         6,    1               /* 6 GMAC3 (GMAC3 Software Reset Control Bit) */
#define IPSRST1_USBDEV1                       5,    1               /* 5 USBDEV1 (USB Device 1 Controller Software Reset Control Bit) */
#define IPSRST1_GDMA0                         3,    1               /* 3 GDMA0 (GDMA0 Software Reset Control Bit) */
#define IPSRST1_SPI3                          1,    1               /* 1 SPI3 (FIU3 and AHB3 (including SHM part connected to AHB3) Software Reset Control Bit).                             */
#define IPSRST1_SPI0                          0,    1               /* 0 SPI0 (FIU0 Software Reset Control Bit).                                                                             */

/***********************************************************************************************************/
/*    IP Software Reset Register 2 (IPSRST2)                                                               */
/***********************************************************************************************************/
#define IPSRST2                              (CLK_BASE_ADDR + 0x24),  CLK_ACCESS,    32         /* Offset: 24h */
#define IPSRST2_CP1                           31,   1               /* 31 CP1 (Coprocessor 1 Reset Control Bit) */
#define IPSRST2_GMAC1                         28,   1               /* 28 GMAC1 (Gigabit MAC 1 Software Reset Control Bit) */
#define IPSRST2_USBH1                         26,   1               /* 26 USBH1 (USB Host 1 Software Reset Control Bit) */
#define IPSRST2_GMAC2                         25,   1               /* 25 GMAC2 (Gigabit MAC 2 Software Reset Control Bit) */
#define IPSRST2_SPI1                          24,   1               /* 24 SPI1 (FIU 1 Module Software Reset Control Bit) */
#define IPSRST2_PSPI2                         23,   1               /* 23 PSPI2 (Peripheral SPI 2 Software Reset Control Bit) */
#define IPSRST2_3DES                          21,   1               /* 21 3DES (3DES Module Software Reset Control Bit) */
#define IPSRST2_BT                            20,   1               /* 20 BT (Block Transfer Module Software Reset Control Bit) */
#define IPSRST2_SIOX2                         19,   1               /* 19 SIOX2 (Serial GPIO Expander 2 Reset Control Bit) */
#define IPSRST2_SIOX1                         18,   1               /* 18 SIOX1 (Serial GPIO Expander 1 Reset Control Bit) */
#define IPSRST2_VIRUART2                      17,   1               /* 17 VIRUART2 (Virtual UART 2 Reset Control Bit) */
#define IPSRST2_VIRUART1                      16,   1               /* 16 VIRUART1 (Virtual UART 1 Reset Control Bit) */
#define IPSRST2_AHB6                          15,   1               /* 15 AHB6 (AHB6 arbiter Software Reset Control Bit) */
#define IPSRST2_VCD                           14,   1               /* 14 VCD (Video Capture and Differentiate Module Reset Control Bit) */
#define IPSRST2_ECE                           13,   1               /* 13 ECE (Encoding Compression Module Reset Control Bit) */
#define IPSRST2_VDMA                          12,   1               /* 12 VDMA (VDM DMA Module Reset Control Bit) */
#define IPSRST2_AHBPCIBRG                     11,   1               /* 11 AHBPCIBRG (AHB-to-PCI Bridge Reset Control Bit) */
#define IPSRST2_GFXSYS                        10,   1               /* 10 GFXSYS (Graphics System Reset Control Bit) */
#define IPSRST2_MMC                           8,    1               /* 8 MMC (Multimedia Card Host Controller Reset Control Bit) */
#define IPSRST2_MFT7_0                        0,    8               /* 0-7 MFT7-0 (Tachometer Module 7-0 Reset Control Bits) */

/***********************************************************************************************************/
/*    IP Software Reset Register 3 (IPSRST3)                                                               */
/***********************************************************************************************************/
#define IPSRST3                              (CLK_BASE_ADDR + 0x34),  CLK_ACCESS,    32         /* Offset: 34h */
#define IPSRST3_USBPHY2                       25,   1               /* 25 USBPHY2 (USB PHY 2 Software Reset Control Bit) */
#define IPSRST3_USBPHY1                       24,   1               /* 24 USBPHY1 (USB PHY 1 Software Reset Control Bit) */
#define IPSRST3_ESPI                          23,   1               /* 23 ESPI (eSPI Module Software Reset Control Bit) */
#define IPSRST3_SMB11                         22,   1               /* 22 SMB11 (SMBus11 Interface Software Reset Control Bit) */
#define IPSRST3_SMB10                         21,   1               /* 21 SMB10 (SMBus10 Interface Software Reset Control Bit) */
#define IPSRST3_SMB9                          20,   1               /* 20 SMB9 (SMBus9 Interface Software Reset Control Bit) */
#define IPSRST3_SMB8                          19,   1               /* 19 SMB8 (SMBus8 Interface Software Reset Control Bit) */
#define IPSRST3_SMB15                         18,   1               /* 18 SMB15 (SMBus15 Interface Software Reset Control Bit) */
#define IPSRST3_RNG                           17,   1               /* 17 RNG (Random Number Generator Software Reset Control Bit) */
#define IPSRST3_TIMER10_14                    16,   1               /* 16 TIMER10-14 (Timers 10-14 and Watchdog 2 (Timer module 2) */
#define IPSRST3_PCIERC                        15,   1               /* 15 PCIERC (None) */
#define IPSRST3_SECECC                        14,   1               /* 14 SECECC (RSA, ECC and MODP Accelerator Module Reset Control Bit) */
#define IPSRST3_SHA                           13,   1               /* 13 SHA (SHA-1 and SHA-256 Module Reset Control Bit) */
#define IPSRST3_SMB14                         12,   1               /* 12 SMB14 (SMBus14 Interface Software Reset Control Bit) */
#define IPSRST3_GDMA2                         11,   1               /* 11 GDMA2 (General Purpose DMA Module 2 Reset Control Bit) */
#define IPSRST3_GDMA1                         10,   1               /* 10 GDMA1 (General Purpose DMA Module 1 Reset Control Bit) */
#define IPSRST3_SPCIMBX                       9,    1               /* 9 SPCIMBX (PCI Mailbox Reset Control Bit) */
#define IPSRST3_USBHUB                        8,    1               /* 8 USBHUB (USB Hub Reset Control Bit) */
#define IPSRST3_USBDEV9                       7,    1               /* 7 USBDEV9 (USB Device 9 Reset Control Bits) */
#define IPSRST3_USBDEV8                       6,    1               /* 6 USBDEV8 (USB Device 8 Reset Control Bits) */
#define IPSRST3_USBDEV7                       5,    1               /* 5 USBDEV7 (USB Device 7 Reset Control Bits) */
#define IPSRST3_USBDEV0                       4,    1               /* 4 USBDEV0 (USB Device 0 Reset Control Bits) */
#define IPSRST3_SMB13                         3,    1               /* 3 SMB13 (SMBus13 Interface Software Reset Control Bit) */
#define IPSRST3_SPIX                          2,    1               /* 2 SPIX (SPIX Reset Control Bits) */
#define IPSRST3_SMB12                         1,    1               /* 1 SMB12 (SMBus12 Interface Software Reset Control Bit) */
#define IPSRST3_PWMM1                         0,    1               /* 0 PWMM1 (PWM Module 1 Reset Control Bits) */

/***********************************************************************************************************/
/*    IP Software Reset Register 4 (IPSRST4)                                                               */
/***********************************************************************************************************/
#define IPSRST4                              (CLK_BASE_ADDR + 0x74),  CLK_ACCESS,    32         /* Offset: 74h */
#define IPSRST4_USBH2                         31,   1               /* 31 USBH2 (USB Host 2 Software Reset Control Bit) */
#define IPSRST4_JTM2                          30,   1               /* 30 JTM2 (JTAG Master 2 Software Reset Control Bit) */
#define IPSRST4_JTM1                          29,   1               /* 29 JTM1 (JTAG Master 1 Software Reset Control Bit) */
#define IPSRST4_PWMM2                         28,   1               /* 28 PWMM2 (USB Host or Device 3 Software Reset Control Bit) */
#define IPSRST4_PCIERCPHY                     27,   1               /* 27 PCIERCPHY (PCIe Root Complex PHY Software Reset Control Bit) */
#define IPSRST4_USBPHY3                       25,   1               /* 25 USBPHY3 (USB PHY 3 Software Reset Control Bit) */
#define IPSRST4_SMB26                         24,   1               /* 24 SMB26 (SMBus Interface 26 Software Reset Control Bit) */
#define IPSRST4_SMB25                         23,   1               /* 23 SMB25 (SMBus Interface 25 Software Reset Control Bit) */
#define IPSRST4_SMB24                         22,   1               /* 22 SMB24 (SMBus Interface 24 Software Reset Control Bit) */
#define IPSRST4_UART6                         18,   1               /* 18 UART6 (BMC UART 6 Software Reset Control Bit) */
#define IPSRST4_UART5                         17,   1               /* 17 UART5 (BMC UART 5 Software Reset Control Bit) */
#define IPSRST4_UART4                         16,   1               /* 16 UART4 (BMC UART 4 Software Reset Control Bit) */
#define IPSRST4_I3C5                          13,   1               /* 13 I3C5 (I3C Controller 5 Software Reset Control Bit) */
#define IPSRST4_I3C4                          12,   1               /* 12 I3C4 (I3C Controller 4 Software Reset Control Bit) */
#define IPSRST4_I3C3                          11,   1               /* 11 I3C3 (I3C Controller 3 Software Reset Control Bit) */
#define IPSRST4_I3C2                          10,   1               /* 10 I3C2 (I3C Controller 2 Software Reset Control Bit) */
#define IPSRST4_I3C1                          9,    1               /* 9 I3C1 (I3C Controller 1 Software Reset Control Bit) */
#define IPSRST4_I3C0                          8,    1               /* 8 I3C0 (I3C Controller 0 Software Reset Control Bit) */
#define IPSRST4_SMB23                         7,    1               /* 7 SMB23 (SMBus Interface 23 Software Reset Control Bit) */
#define IPSRST4_SMB22                         6,    1               /* 6 SMB22 (SMBus Interface 22 Software Reset Control Bit) */
#define IPSRST4_SMB21                         5,    1               /* 5 SMB21 (SMBus Interface 21 Software Reset Control Bit) */
#define IPSRST4_SMB20                         4,    1               /* 4 SMB20 (SMBus Interface 20 Software Reset Control Bit) */
#define IPSRST4_SMB19                         3,    1               /* 3 SMB19 (SMBus Interface 19 Software Reset Control Bit) */
#define IPSRST4_SMB18                         2,    1               /* 2 SMB18 (SMBus Interface 18 Software Reset Control Bit) */
#define IPSRST4_SMB17                         1,    1               /* 1 SMB17 (SMBus Interface 17 Software Reset Control Bit) */
#define IPSRST4_SMB16                         0,    1               /* 0 SMB16 (SMBus Interface 16 Software Reset Control Bit) */

/***********************************************************************************************************/
/*    Watchdog 0 Reset Control Register (WD0RCR)                                                           */
/***********************************************************************************************************/
#define WD0RCR                               (CLK_BASE_ADDR + 0x38),  CLK_ACCESS,    32         /* Offset: 38h */
#define WD0RCR_ESPI                           31,   1               /* 31 ESPI (ESPI Watchdog Reset Control Bit) */
#define WD0RCR_PCIERC                         30,   1               /* 30 PCIERC (PCI Express Root Complex Watchdog Reset Control Bit) */
#define WD0RCR_SHM                            29,   1               /* 29 SHM (SHM module Watchdog Reset Control Bit) */
#define WD0RCR_PWM                            28,   1               /* 28 PWM (PWM Modules Watchdog Reset Control Bit) */
#define WD0RCR_SPER                           27,   1               /* 27 SPER (Slow Peripherals Watchdog Reset Control Bit) */
#define WD0RCR_SPIBMC                         26,   1               /* 26 SPIBMC (BMC SPI interfaces Watchdog Reset Control Bit) */
#define WD0RCR_SIOX2                          25,   1               /* 25 SIOX2 (Serial IO Expander 2 Watchdog Reset Control Bit) */
#define WD0RCR_SIOX1                          24,   1               /* 24 SIOX1 (Serial IO Expander 1 Watchdog Reset Control Bit) */
#define WD0RCR_GPIOM7                         23,   1               /* 23 GPIOM7 (GPIO Module 7 Watchdog Reset Control Bit) */
#define WD0RCR_GPIOM6                         22,   1               /* 22 GPIOM6 (GPIO Module 6 Watchdog Reset Control Bit) */
#define WD0RCR_GPIOM5                         21,   1               /* 21 GPIOM5 (GPIO Module 5 Watchdog Reset Control Bit) */
#define WD0RCR_GPIOM4                         20,   1               /* 20 GPIOM4 (GPIO Module 4 Watchdog Reset Control Bit) */
#define WD0RCR_GPIOM3                         19,   1               /* 19 GPIOM3 (GPIO Module 3 Watchdog Reset Control Bit) */
#define WD0RCR_GPIOM2                         18,   1               /* 18 GPIOM2 (GPIO Module 2 Watchdog Reset Control Bit) */
#define WD0RCR_GPIOM1                         17,   1               /* 17 GPIOM1 (GPIO Module 1 Watchdog Reset Control Bit) */
#define WD0RCR_GPIOM0                         16,   1               /* 16 GPIOM0 (GPIO Module 0 Watchdog Reset Control Bit) */
#define WD0RCR_TIMER                          15,   1               /* 15 TIMER (Timer Modules Watchdog Reset Control Bit) */
#define WD0RCR_PCIMBX                         14,   1               /* 14 PCIMBX (PCI Mailbox Watchdog Reset Control Bit) */
#define WD0RCR_AHB2PCI                        13,   1               /* 13 AHB2PCI (AHB-to-PCI Bridge Watchdog Reset Control Bit) */
#define WD0RCR_TIP_Reset                      12,   1               /* 12 TIP_Reset - Generates TIP Cold Reset (TIP M4 Core Subsystem Reset) */
#define WD0RCR_MMC                            11,   1               /* 11 MMC (Multimedia Card Controller Watchdog Reset Control Bit) */
#define WD0RCR_DMA                            10,   1               /* 10 DMA (DMA Modules Watchdog Reset Control Bit) */
#define WD0RCR_USBH1                          9,    1               /* 9 USBH1 (USB Host 1 Watchdog Reset Control Bit) */
#define WD0RCR_USBDEV                         8,    1               /* 8 USBDEV (USB Devices Watchdog Reset Control Bit) */
#define WD0RCR_ETH                            7,    1               /* 7 ETH (Ethernet Controllers Watchdog Reset Control Bit) */
#define WD0RCR_CLKS                           6,    1               /* 6 CLKS (Clock Control Watchdog Reset Control Bit) */
#define WD0RCR_MC                             5,    1               /* 5 MC (DDR4 CLK_ACCESSory Controller Watchdog Reset Control Bit) */
#define WD0RCR_RV                             4,    1               /* 4 RV (Remote Video Watchdog Reset Control Bit) */
#define WD0RCR_SEC                            3,    1               /* 3 SEC (Security Peripherals Watchdog Reset Control Bit) */
#define WD0RCR_CP1                            2,    1               /* 2 CP1 (Coprocessor 1 Watchdog Reset Control Bit) */
#define WD0RCR_BMCDBG                         1,    1               /* 1 BMCDBG (BMC CPU Debugger (CoreSight) */
#define WD0RCR_CA35C                          0,    1               /* 0 CA35C (Cortex A35 Cores Watchdog Reset Control Bit) */

/***********************************************************************************************************/
/*    Watchdog 1 Reset Control Register (WD1RCR)***                                                        */
/***********************************************************************************************************/
#define WD1RCR                               (CLK_BASE_ADDR + 0x3C),  CLK_ACCESS,    32         /* Offset: 3Ch */
#define WD1RCR_HOLD_BIT                       0,    32               /* 0-31 HOLD_BIT (TBD) */

/***********************************************************************************************************/
/*    Watchdog 2 Reset Control Register (WD2RCR)                                                           */
/***********************************************************************************************************/
#define WD2RCR                               (CLK_BASE_ADDR + 0x40),  CLK_ACCESS,    32         /* Offset: 40h */
#define WD2RCR_HOLD_BIT                       0,    32               /* 0-31 HOLD_BIT (TBD) */

/***********************************************************************************************************/
/*    Software Reset Control Register 1 (SWRSTC1)                                                          */
/***********************************************************************************************************/
#define SWRSTC1                              (CLK_BASE_ADDR + 0x44),  CLK_ACCESS,    32         /* Offset: 44h */
#define SWRSTC1_HOLD_BIT                      0,    32               /* 0-31 HOLD_BIT (TBD) */

/***********************************************************************************************************/
/*    Software Reset Control Register 2 (SWRSTC2)                                                          */
/***********************************************************************************************************/
#define SWRSTC2                              (CLK_BASE_ADDR + 0x48),  CLK_ACCESS,    32         /* Offset: 48h */
#define SWRSTC2_HOLD_BIT                      0,    32               /* 0-31 HOLD_BIT (TBD) */

/***********************************************************************************************************/
/*    Software Reset Control Register 3 (SWRSTC3)                                                          */
/***********************************************************************************************************/
#define SWRSTC3                              (CLK_BASE_ADDR + 0x4C),  CLK_ACCESS,    32         /* Offset: 4Ch */
#define SWRSTC3_HOLD_BIT                      0,    32               /* 0-31 HOLD_BIT (TBD) */

/***********************************************************************************************************/
/*    TIP Reset Control Register (TIPRSTC)                                                                 */
/***********************************************************************************************************/
#define TIPRSTC                              (CLK_BASE_ADDR + 0x50),  CLK_ACCESS,    32         /* Offset: 50h */
#define TIPRSTC_HOLD_BIT                      0,    32               /* 0-31 HOLD_BIT (TBD) */

/***********************************************************************************************************/
/*    CORST Control Register (CORSTC)                                                                      */
/***********************************************************************************************************/
#define CORSTC                               (CLK_BASE_ADDR + 0x5C),  CLK_ACCESS,    32         /* Offset: 5Ch */
#define CORSTC_HOLD_BIT                       0,    32               /* 0-31 HOLD_BIT (TBD) */

/***********************************************************************************************************/
/*    Watchdog 0 Reset Control Register B (WD0RCRB)                                                        */
/***********************************************************************************************************/
#define WD0RCRB                              (CLK_BASE_ADDR + 0x80),  CLK_ACCESS,    32         /* Offset: 80h */
#define WD0RCRB_JTAGM                         10,   1               /* 10 JTAGM (JTAG Master Modules Watchdog Reset Control Bit) */
#define WD0RCRB_HGPIO                         9,    1               /* 9 HGPIO (Host GPIOs modules Watchdog Reset Control Bit) */
#define WD0RCRB_HOSTPER                       8,    1               /* 8 HOSTPER (Host Peripherals Watchdog Reset Control Bit) */
#define WD0RCRB_PCIGFX                        7,    1               /* 7 PCIGFX (PCI Devices Watchdog Reset Control Bit) */
#define WD0RCRB_FLM                           6,    1               /* 6 FLM (Flash Monitoring Reset Control Bit) */
#define WD0RCRB_BMC_MS                        5,    1               /* 5 BMC_MS (BMC Modules Watchdog Reset Control Bit) */
#define WD0RCRB_I3C                           4,    1               /* 4 I3C (I3C Modules Watchdog Reset Control Bit) */
#define WD0RCRB_SEC_REG_RST                   3,    1               /* 3 SEC_REG_RST (Secure Registers Reset Control Bit) */
#define WD0RCRB_BMCBUS                        2,    1               /* 2 BMCBUS (BMC Bus Watchdog Reset Control Bit) */
#define WD0RCRB_SPI3                          1,    1               /* 1 CLK_SPI3 (FIU3 module and AHB3 Logic Watchdog Reset Control Bit) */
#define WD0RCRB_USBH2                         0,    1               /* 0 USBH2 (USB Host 2 Watchdog Reset Control Bit) */

/***********************************************************************************************************/
/*    Watchdog 1 Reset Control Register B (WD1RCRB)                                                        */
/***********************************************************************************************************/
#define WD1RCRB                              (CLK_BASE_ADDR + 0x84),  CLK_ACCESS,    32         /* Offset: 84h */
#define WD1RCRB_HOLD_BIT                      0,    32               /* 0-31 HOLD_BIT (TBD) */

/***********************************************************************************************************/
/*    Watchdog 2 Reset Control Register B (WD2RCRB)                                                        */
/***********************************************************************************************************/
#define WD2RCRB                              (CLK_BASE_ADDR + 0x88),  CLK_ACCESS,    32         /* Offset: 88h */
#define WD2RCRB_HOLD_BIT                      0,    32               /* 0-31 HOLD_BIT (TBD) */

/***********************************************************************************************************/
/*    Software Reset Control Register 1 B (SWRSTC1B)                                                       */
/***********************************************************************************************************/
#define SWRSTC1B                             (CLK_BASE_ADDR + 0x8C),  CLK_ACCESS,    32         /* Offset: 8Ch */
#define SWRSTC1B_HOLD_BIT                     0,    32               /* 0-31 HOLD_BIT (TBD) */

/***********************************************************************************************************/
/*    Software Reset Control Register 2 B (SWRSTC2B)                                                       */
/***********************************************************************************************************/
#define SWRSTC2B                             (CLK_BASE_ADDR + 0x90),  CLK_ACCESS,    32         /* Offset: 90h */
#define SWRSTC2B_HOLD_BIT                     0,    32               /* 0-31 HOLD_BIT (TBD) */

/***********************************************************************************************************/
/*    Software Reset Control Register 3 B (SWRSTC3B)                                                       */
/***********************************************************************************************************/
#define SWRSTC3B                             (CLK_BASE_ADDR + 0x94),  CLK_ACCESS,    32         /* Offset: 94h */
#define SWRSTC3B_HOLD_BIT                     0,    32               /* 0-31 HOLD_BIT (TBD) */

/***********************************************************************************************************/
/*    TIP Reset Control Register B (TIPRSTCB)                                                              */
/***********************************************************************************************************/
#define TIPRSTCB                             (CLK_BASE_ADDR + 0x98),  CLK_ACCESS,    32         /* Offset: 98h */
#define TIPRSTCB_HOLD_BIT                     0,    32               /* 0-31 HOLD_BIT (TBD) */

/***********************************************************************************************************/
/*    CORST Control Register B (CORSTCB)                                                                   */
/***********************************************************************************************************/
#define CORSTCB                              (CLK_BASE_ADDR + 0x9C),  CLK_ACCESS,    32         /* Offset: 9Ch */
#define CORSTCB_HOLD_BIT                      0,    32               /* 0-31 HOLD_BIT (TBD) */

/***********************************************************************************************************/
/*    PLL Control Register GFX (PLLCONG)                                                                   */
/***********************************************************************************************************/
#define PLLCONG                              (CLK_BASE_ADDR + 0x60),  CLK_ACCESS,    32         /* Offset: 60h */
#define PLLCONG_LOKI                          31,   1               /* 31 LOKI (Lock Indication) */
#define PLLCONG_LOKS                          30,   1               /* 30 LOKS (Unlock Indication Sticky Bit) */
#define PLLCONG_GPLLTST                       29,   1               /* 29 GPLLTST (Graphics PLL Test Mode) */
#define PLLCONG_FBDV                          16,   12               /* 16-27 FBDV (PLL VCO Output Clock Feedback Divider) */
#define PLLCONG_OTDV2                         13,   3               /* 13-15 OTDV2 (PLL Output Clock Divider 2) */
#define PLLCONG_PWDEN                         12,   1               /* 12 PWDEN (Power-Down Mode Enable) */
#define PLLCONG_OTDV1                         8,    3               /* 8-10 OTDV1 (PLL Output Clock Divider 1) */
#define PLLCONG_INDV                          0,    6               /* 0-5 INDV (PLL Input Clock Divider) */

/***********************************************************************************************************/
/*    AHB Clock Frequency Information Register (AHBCKFI)                                                   */
/***********************************************************************************************************/
#define AHBCKFI                              (CLK_BASE_ADDR + 0x64),  CLK_ACCESS,    32         /* Offset: 64h */
#define AHBCKFI_TST1S                         31,   1               /* 31 TST1S (Test Seconds Counter) */
#define AHBCKFI_AHB_CLK_FRQ                   0,    8               /* 0-7 AHB_CLK_FRQ (AHB Clock Frequency) */

/***********************************************************************************************************/
/*    Seconds Counter Register (SECCNT)                                                                    */
/***********************************************************************************************************/
#define SECCNT                               (CLK_BASE_ADDR + 0x68),  CLK_ACCESS,    32         /* Offset: 68h */
#define SECCNT_SEC_CNT                        0,    32               /* 0-31 SEC_CNT (Seconds Count) */

/***********************************************************************************************************/
/*    Counter 25M Register (CNTR25M)                                                                       */
/***********************************************************************************************************/
#define CNTR25M                              (CLK_BASE_ADDR + 0x6C),  CLK_ACCESS,    32         /* Offset: 6Ch */
#define CNTR25M_25MCNT                        0,    25               /* 0-24 25MCNT (25 MHz Count) */

/***********************************************************************************************************/
/*    Bus Time Out Register (BUSTO)                                                                        */
/***********************************************************************************************************/
#define BUSTO                                (CLK_BASE_ADDR + 0x78),  CLK_ACCESS,    32         /* Offset: 78h */
#define BUSTO_TOAHB9                          24,   3               /* 24-26 TOAHB9 (Timeout for AHB9) */
#define BUSTO_TOAXI16                         21,   3               /* 21-23 TOAXI16 (Timeout for AXI16) */
#define BUSTO_TOAXI15                         18,   3               /* 18-20 TOAXI15 (Timeout for AXI15) */
#define BUSTO_TOAHB18                         15,   3               /* 15-17 TOAHB18 (Timeout for AHB18) */
#define BUSTO_TOAHB14                         12,   3               /* 12-14 TOAHB14 (Timeout for AHB14) */
#define BUSTO_TOAHB19                         9,    3               /* 9-11 TOAHB19 (Timeout for AHB19) */
#define BUSTO_TOAHB3                          6,    3               /* 6-8 TOAHB3 (Timeout for AHB3) */
#define BUSTO_TOAHB8                          3,    3               /* 3-5 TOAHB8 (Timeout for AHB8) */
#define BUSTO_TOAHB1                          0,    3               /* 0-2 TOAHB1 (Timeout for AHB1) */

/***********************************************************************************************************/
/*    IPSRST1 Disable Register (IPSRSTDIS1)                                                                */
/***********************************************************************************************************/
#define IPSRSTDIS1                           (CLK_BASE_ADDR + 0xA0),  CLK_ACCESS,    32         /* Offset: A0h */
#define IPSRSTDIS1_IPSRSTDIS                  0,    32               /* 0-31 IPSRSTDIS (IPSRST Register Bit Disable) */

/***********************************************************************************************************/
/*    IPSRST2 Disable Register (IPSRSTDIS2)                                                                */
/***********************************************************************************************************/
#define IPSRSTDIS2                           (CLK_BASE_ADDR + 0xA4),  CLK_ACCESS,    32         /* Offset: A4h */
#define IPSRSTDIS2_IPSRSTDIS                  0,    32               /* 0-31 IPSRSTDIS (IPSRST Register Bit Disable) */

/***********************************************************************************************************/
/*    IPSRST3 Disable Register (IPSRSTDIS3)                                                                */
/***********************************************************************************************************/
#define IPSRSTDIS3                           (CLK_BASE_ADDR + 0xA8),  CLK_ACCESS,    32         /* Offset: A8h */
#define IPSRSTDIS3_IPSRSTDIS                  0,    32               /* 0-31 IPSRSTDIS (IPSRST Register Bit Disable) */

/***********************************************************************************************************/
/*    IPSRST4 Disable Register (IPSRSTDIS4)                                                                */
/***********************************************************************************************************/
#define IPSRSTDIS4                           (CLK_BASE_ADDR + 0xAC),  CLK_ACCESS,    32         /* Offset: ACh */
#define IPSRSTDIS4_IPSRSTDIS                  0,    32               /* 0-31 IPSRSTDIS (IPSRST Register Bit Disable) */

/***********************************************************************************************************/
/*    CLKEN1 Disable Register (CLKENDIS1)                                                                  */
/***********************************************************************************************************/
#define CLKENDIS1                           (CLK_BASE_ADDR + 0xB0),  CLK_ACCESS,    32
#define CLKENDIS1_CLKENDIS                  0,    32               /* 0-31 CLKENDIS (CLKEN Register Bit Disable) */

/***********************************************************************************************************/
/*    CLKEN2 Disable Register (CLKENDIS2)                                                                  */
/***********************************************************************************************************/
#define CLKENDIS2                           (CLK_BASE_ADDR + 0xB4),  CLK_ACCESS,    32
#define CLKENDIS2_CLKENDIS                  0,    32               /* 0-31 CLKENDIS (CLKEN Register Bit Disable) */


/***********************************************************************************************************/
/*    CLKEN3 Disable Register (CLKENDIS3)                                                                  */
/***********************************************************************************************************/
#define CLKENDIS3                           (CLK_BASE_ADDR + 0xB8),  CLK_ACCESS,    32
#define CLKENDIS3_CLKENDIS                  0,    32               /* 0-31 CLKENDIS (CLKEN Register Bit Disable) */


/***********************************************************************************************************/
/*    CLKEN4 Disable Register (CLKENDIS4)                                                                  */
/***********************************************************************************************************/
#define CLKENDIS4                           (CLK_BASE_ADDR + 0xBC),  CLK_ACCESS,    32
#define CLKENDIS4_CLKENDIS                  0,    32               /* 0-31 CLKENDIS (CLKEN Register Bit Disable) */


/***********************************************************************************************************/
/*    CPU Throttling Control Register (THRTL_CNT)                                                          */
/***********************************************************************************************************/
#define THRTL_CNT                            (CLK_BASE_ADDR + 0x0C0),  CLK_ACCESS,    32         /* Offset: 0C0h */
#define THRTL_CNT_TH_DIV                      0,    2               /* 0-1 TH_DIV (Throttling Divider) */


#define  CNTR25M_ACCURECY                 EXT_CLOCK_FREQUENCY_MHZ  /* minimum accurecy 1us which is 5 cycles */


/*---------------------------------------------------------------------------------------------------------*/
/* RCPCKSEL                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define CLKSEL_RCPCKSEL_PLL0             0x00          /*   0 0 : PLL0 clock.                          */
#define CLKSEL_RCPCKSEL_PLL1             0x01          /*   0 1 : PLL1 clock.                          */
#define CLKSEL_RCPCKSEL_CLKREF          0x02          /*   1 0 : PCIe PHY reference clock input (100 MHz, default).    */

/*---------------------------------------------------------------------------------------------------------*/
/* RGSEL                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define CLKSEL_RGSEL_PLL0             0x00          /*   0 0 0 : PLL0 clock.                          */
#define CLKSEL_RGSEL_PLL1             0x01          /*   0 0 1 : PLL1 clock.                          */
#define CLKSEL_RGSEL_CLKREF           0x02          /*   0 1 0 : CLKREF input (25 MHz, default).      */

/*---------------------------------------------------------------------------------------------------------*/
/* AHB6SSEL                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define CLKSEL_AHB6SSEL_CLKREF             0x02          /*  1 0    : CLKREF clock (25 MHz, default).      */
#define CLKSEL_AHB6SSEL_PLL2               0x03          /*  1 1    : PLL2 clock, frequency divided by 3.  */


/*---------------------------------------------------------------------------------------------------------*/
/* GFXMSEL                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define CLKSEL_GFXMSEL_CLKREF             0x02          /*  1 0    : CLKREF clock (25 MHz, default).      */
#define CLKSEL_GFXMSEL_PLL2               0x03          /*  1 1    : PLL2 clock, frequency divided by 3.  */

/*---------------------------------------------------------------------------------------------------------*/
/* CLKOUTSEL                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define CLKSEL_CLKOUTSEL_PLL0             0x00          /*   0 0 0 : PLL0 clock.                          */
#define CLKSEL_CLKOUTSEL_PLL1             0x01          /*   0 0 1 : PLL1 clock.                          */
#define CLKSEL_CLKOUTSEL_CLKREF           0x02          /*   0 1 0 : CLKREF input (25 MHz, default).      */
#define CLKSEL_CLKOUTSEL_PLLG             0x03          /*   0 1 1 : Graphics PLL output clock, divided by 2 */
#define CLKSEL_CLKOUTSEL_PLL2             0x04          /*   1 0 0 : PLL2 clock divided by 2.             */

/*---------------------------------------------------------------------------------------------------------*/
/* PCIGFXCKSEL                                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
#define CLKSEL_PCIGFXCKSEL_PLL0              0x00          /*   0 0   : PLL0 clock.                          */
#define CLKSEL_PCIGFXCKSEL_PLL1              0x01          /*   0 1   : PLL1 clock.                          */
#define CLKSEL_PCIGFXCKSEL_CLKREF            0x02          /*   1 0   : CLKREF clock (25 MHz, default).      */
#define CLKSEL_PCIGFXCKSEL_PLL2              0x03          /*   1 1   : PLL2 clock divided by 2.             */

/*---------------------------------------------------------------------------------------------------------*/
/* ADCCKSEL                                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
#define CLKSEL_ADCCKSEL_PLL0              0x00          /*   0 0   : PLL0 clock.                          */
#define CLKSEL_ADCCKSEL_PLL1              0x01          /*   0 1   : PLL1 clock.                          */
#define CLKSEL_ADCCKSEL_CLKREF            0x02          /*   1 0   : CLKREF clock (25 MHz, default).      */
#define CLKSEL_ADCCKSEL_PLL2              0x03          /*   1 1   : PLL2 clock divided by 2.             */

/*---------------------------------------------------------------------------------------------------------*/
/* MCCKSEL                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define CLKSEL_MCCKSEL_PLL1               0x00          /*  0 0    : PLL1 clock.                          */
#define CLKSEL_MCCKSEL_CLKREF             0x02          /*  1 0    : CLKREF clock (25 MHz, default).      */
#define CLKSEL_MCCKSEL_MCBPCK             0x03          /*  1 1    : MCBPCK clock input.                  */

/*---------------------------------------------------------------------------------------------------------*/
/* SUCKSEL                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define CLKSEL_SUCKSEL_PLL0               0x00          /*  0 0    : PLL0 clock.                          */
#define CLKSEL_SUCKSEL_PLL1               0x01          /*  0 1    : PLL1 clock.                          */
#define CLKSEL_SUCKSEL_CLKREF             0x02          /*  1 0    : CLKREF clock (25 MHz, default).      */
#define CLKSEL_SUCKSEL_PLL2               0x03          /*  1 1    : PLL2 clock divided by 2.             */

/*---------------------------------------------------------------------------------------------------------*/
/* UARTCKSEL                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define CLKSEL_UARTCKSEL_PLL0             0x00          /*  0 0    : PLL0 clock.                          */
#define CLKSEL_UARTCKSEL_PLL1             0x01          /*  0 1    : PLL1 clock.                          */
#define CLKSEL_UARTCKSEL_CLKREF           0x02          /*  1 0    : CLKREF clock (25 MHz, default).      */
#define CLKSEL_UARTCKSEL_PLL2             0x03          /*  1 1    : PLL2 clock divided by 2.             */

/*---------------------------------------------------------------------------------------------------------*/
/* SDCKSEL                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define CLKSEL_SDCKSEL_PLL0               0x00          /*   0 0   : PLL0 clock.                          */
#define CLKSEL_SDCKSEL_PLL1               0x01          /*   0 1   : PLL1 clock.                          */
#define CLKSEL_SDCKSEL_CLKREF             0x02          /*   1 0   : CLKREF clock (25 MHz, default).      */
#define CLKSEL_SDCKSEL_PLL2               0x03          /*   1 1   : PLL2 clock divided by 2.             */

/*---------------------------------------------------------------------------------------------------------*/
/* PIXCKSEL                                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
#define CLKSEL_PIXCKSEL_PLLG              0x00          /*   0 0   : PLL GFX clock after divide to 2.     */
#define CLKSEL_PIXCKSEL_CLKOUT_GPIO160    0x01          /*   0 1   : CLKOUT/GPIO160 pin as input (MFSEL1.21 and GPIO160 controls should be left at default state).*/
#define CLKSEL_PIXCKSEL_CLKREF            0x02          /*   1 0   : CLKREF input. (default)              */

/*---------------------------------------------------------------------------------------------------------*/
/* GPRFSEL                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
//#define CLKSEL_GPRFSEL_GFXBYPCK           0x00          /*   0 0   : GFXBYPCK pin.                        */
//#define CLKSEL_GPRFSEL_USB                0x01          /*   0 1   : USB OHCI Clock (48 MHz).             */
//#define CLKSEL_GPRFSEL_CLKREF             0x02          /*   1 0   : CLKREF input. (default)              */

/*---------------------------------------------------------------------------------------------------------*/
/* CPUCKSEL                                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
#define CLKSEL_CPUCKSEL_PLL0              0x00          /*  0 0 0   : PLL0 clock.                          */
#define CLKSEL_CPUCKSEL_PLL1              0x01          /*  0 0 1   : PLL1 clock.                          */
#define CLKSEL_CPUCKSEL_CLKREF            0x02          /*  0 1 0   : CLKREF input (25 MHz, default).      */
#define CLKSEL_CPUCKSEL_SYSBPCK           0x03          /*  0 1 1   : Bypass clock from pin SYSBPCK.       */
#define CLKSEL_CPUCKSEL_PLL2              0x04          /*  1 0 0   : PLL2 clock (not divided)             */


/* log table for numbers in range of 0-63, numbers are rounded doun to integers */
static const unsigned int clk_log_table[64] = {0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
                                               4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                               5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                                               5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6};

#define LOG_TABLE(n) clk_log_table[n]

/*---------------------------------------------------------------------------------------------------------*/
/* clock division field values:                                                                            */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* CLKDIV1_ADCCKDIV   : ADCCKDIV value is 1,2,4,8,16,32.                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV1_ADCCKDIV_DIV(n)   (LOG_TABLE(n))


/*---------------------------------------------------------------------------------------------------------*/
/* CLKDIV1_CLK4DIV (AMBA AHB Clock Divider Control)                                                        */
/*---------------------------------------------------------------------------------------------------------*/
 #define  CLKDIV1_CLK4DIV_DIV(n)    ((n)-1)
 /* After changing this field, assure a delay of 200 clock cycles before changing CPUCKSEL field in CLKSEL register. */

/*---------------------------------------------------------------------------------------------------------*/
/* CLKDIV1_PRE_ADCCKDIV (Pre ADC Clock Source Divider Control). Default is divide by 1. The division factor is  */
/* (TIMCKDIV +1), where TIMCKDIV value is 0-31.                                                            */
/* Note: in Poleg this field was called TIMCKDIV                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV1_PRE_ADCCKDIV_DIV(n)   ((n) - 1)     /* • TIMCKDIV value is 0-31. */
/* Note: After changing this field, assure a delay of 200 selected clock cycles before the timer is used. */


/*---------------------------------------------------------------------------------------------------------*/
/* CLKDIV1_UARTDIV (UART Clock Source Divider Control). This resulting clock must be 24 MHz for UARTs proper*/
/* operation                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV1_UARTDIV1_DIV(n)   ((n) - 1)     /* Default is divide by 20. */

/*---------------------------------------------------------------------------------------------------------*/
/* CLKDIV1_MMCCKDIV (MMC Controller (SDHC2) Clock Divider Control). Sets the division factor from the clock*/
/* selected by SDCKSEL. The division factor is (MMCCKDIV+1), where MMCCKDIV is 0:31. Default is to div by 32. */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV1_MMCCKDIV_DIV(n)   ((n) - 1)

/*---------------------------------------------------------------------------------------------------------*/
/* CLKDIV1_AHB3CKDIV (CLK_SPI3 Clock Divider Control). Sets the division factor from AHB clock (CLK4) to AHB3  */
/* and CLK_SPI3                                                                                                */
/* clock. The division factor is (AHB3CKDIV+1), where AHB3CKDIV is 0 to 31. Default is to divide by 2.     */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV1_AHB3CKDIV_DIV(n)   ((n) - 1)

/*---------------------------------------------------------------------------------------------------------*/
/* CLKDIV1_PCICKDIV (Internal PCI Clock Divider Control). Sets the division factor from the clock selected */
/* by the CLKSEL GFXCKSEL field. The division factor is (PCICKDIV+1), where PCICKDIV is 0 to 15.           */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV1_PCICKDIV_DIV(n)   ((n) - 1)

/*---------------------------------------------------------------------------------------------------------*/
/* CLKDIV1_CLK2DIV (AMBA AXI Clock Divider Control).                                                       */
/* CLK2DIV field. The division factor is (CLK2DIV+1), where CLK2DIV is 0 to 1.                             */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV1_CLK2DIV_DIV(n)   ((n) - 1)



/*---------------------------------------------------------------------------------------------------------*/
/* CLKDIV2_APBxCKDIV_DIV (AMBA APBx Clock Divider Control).                                                */
/* CLK_APB1 clock must be at least 24 MHz, for UARTs to function. APB clock frequency is up to 67 MHz          */
/* CLK_APB2 clock frequency is up to 67 MHz.                                                                   */
/* CLK_APB3 clock frequency is up to 67 MHz                                                                    */
/* CLK_APB4 clock frequency is up to 67 MHz                                                                    */
/* CLK_APB5 clock must be high to enable PSPI1-2 high SPI clock rate. The preferred setting is divide by 2     */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV2_APBxCKDIV_DIV(n)    (LOG_TABLE(n))
 /* Note: After changing this field, assure a delay of 200 clock cycles before changing CPUCKSEL field in CLKSEL */

/*---------------------------------------------------------------------------------------------------------*/
/* CLKDIV2_CLKOUTDIV (CLKOUT Pin Divider Contro).                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV2_CLKOUTDIV_DIV(n)   ((n) - 1)     /* CLKOUTDIV field. The division factor is (CLKOUTDIV+1), where CLKOUTDIV is 0 to 16. */

/*---------------------------------------------------------------------------------------------------------*/
/* GFXCKDIV (Graphics System Clock Divider Control). Sets the division factor from the clock selected by   */
/* the CLKSEL.GFXCKSEL field. The division factor is (GFXCKDIV+1), where GFXCKDIV is 0 to 7.               */
/* Default is divide by 5.                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV2_GFXCKDIV_DIV(n)   ((n) - 1)     /* is (GFXCKDIV+1), where GFXCKDIV is 0 to 7. Default is to divide by 5. */
/* After changing this field, ensure a delay of 200 selected clock cycles before changing GFXCKSEL field in CLKSEL register. */

/*---------------------------------------------------------------------------------------------------------*/
/* SUCKDIV (‘Serial’ USB UTMI Bridge Clock Divider Control). Sets the division factor from the clock       */
/* selected by                                                                                             */
/* SUCKSEL to the clock used by the UTMI-to-UTMI bridge, that is nominally 30 MHz.                         */
/* The division factor is (SUCKDIV+1), where SUCKDIV is 0 to 31. Default is divide by 16.                  */
/* After changing this field, ensure a delay of 200 clock cycles before changing SUCKSEL field in CLKSEL   */
/* register                                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV2_SUCKDIV_DIV(n)   ((n) - 1)

/*---------------------------------------------------------------------------------------------------------*/
/* SU48CKDIV (USB Host OHCI Clock Divider Control). Sets the division factor from the clock selected       */
/* by SUCKSEL to the clock used by the serial interface, that is nominally 48 MHz.                         */
/* The division factor is (SU48CKDIV+1), where SU48CKDIV is 0 to 15. Default is divide by 10.              */
/* After changing this field, ensure a delay of 200 clock cycles before changing SU48CKSEL field           */
/* in CLKSEL register.                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV2_SU48CKDIV_DIV(n)   ((n) - 1)

/*---------------------------------------------------------------------------------------------------------*/
/* SD1CKDIV (SDHC1 Clock Divider Control). (Place holder in Arbel - May be removed) Sets the division      */
/* factor from the clock selected by SDCKSEL.                                                              */
/* The division factor is 2*(SDCKDIV+1), where SDCKDIV is 0 to 15. Default is divide by 32.                */
/* After changing this field, ensure a delay of 200 clock cycles before changing SDCKSEL field in CLKSEL   */
/* register.                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV2_SD1CKDIV_DIV(n)   (((n)>>1) - 1)

/*---------------------------------------------------------------------------------------------------------*/
/* SPI1CKDIV (SPI1 Clock Divider Control). Sets the division factor from AHB clock to SPI1 clock.          */
/* The division factor is (SPI1CKDIV+1), where SPI1CKDIV is 0 to 255.                                      */
/* Default is divide by 1. Field added in Arbel.                                                           */
/* Note: After changing this field, ensure a delay of 512 AHB clock cycles before changing CPUCKSEL field  */
/* in  CLKSEL register or accessing the AHB18 bus.                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV3_SPI1CKDIV_DIV(n)   ((n) - 1)

/*---------------------------------------------------------------------------------------------------------*/
/* UARTDIV2 (UART Clock Source Divider Control 2). (Added in Arbel) Clock divider for BMC UART 4-6.        */
/* This resulting clock must be multiples of 24 MHz for UARTs proper operation.                            */
/* Default is divide by 20. The division factor is (UARTDIV2 +1), where:                                   */
/* • UARTDIV2 value is 0-31.                                                                               */
/* • Divider clock is the clock source selected by the UARTSEL control register.                           */
/* • UART clock must be equal to or slower than the relevant APB clock for proper operation.               */
/* Note: After changing this field, ensure a delay of 200 selected clock cycles before the UART is used    */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV3_UARTDIV2_DIV(n)   ((n) - 1)

/*---------------------------------------------------------------------------------------------------------*/
/* SPI0CKDIV (CLK_SPI0 Clock Divider Control). Sets the division factor from AHB clock to CLK_SPI0 clock.          */
/* The division factor is (SPI0CKDIV+1), where SPI0CKDIV is 0 to 31. Default is divide by 1.               */
/* Note: After changing this field, ensure a delay of 64 AHB clock cycles before changing CPUCKSEL         */
/* field in CLKSEL register or accessing the AHB18 bus                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV3_SPI0CKDIV_DIV(n)   ((n) - 1)

/*---------------------------------------------------------------------------------------------------------*/
/* SPIXCKDIV (SPIX Clock Divider Control). Sets the division factor from AHB clock to SPIX clock.          */
/* The division factor is (SPIXCKDIV+1), where SPIXCKDIV is 0 to 31. Default is divide by 1.               */
/* Note: After changing this field, ensure a delay of 64 AHB clock cycles before changing                  */
/* CPUCKSEL field in CLKSEL register or accessing the AHB18 bus                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV3_SPIXCKDIV_DIV(n)   ((n) - 1)




/*---------------------------------------------------------------------------------------------------------*/
/* RGREFDIV (RGMII Reference Clock Divider Control).                                                       */
/* Sets the division factor from the clock selected by RGSEL to 125MHz required by the GMAC interface.     */
/* The division factor is (RGREFDIV+1).                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV4_RGREFDIV_DIV(n)   ((n) - 1)

/*---------------------------------------------------------------------------------------------------------*/
/* JTM2CKDV (JTAG Master 2 Clock Divider Control). Not required.                                           */
/* Sets the division factor from AHB clock to JTM2 clock.                                                  */
/* The division factor is (JTM2CKDIV+1), where JTM2CKDIV is 0 to 4095. Default is divide by 1.             */
/* After changing this field, ensure a delay of 8200 AHB clock cycles before changing CPUCKSEL             */
/* field in CLKSEL register or accessing the AHB18 bus.                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV4_JTM2CKDV_DIV(n)   ((n) - 1)

/*---------------------------------------------------------------------------------------------------------*/
/* RCPREFDIV (Root Complex PHY Reference Clock Divider Control).                                           */
/* Sets the division factor from the clock selected by RCPCKSEL to 100MHz required by the PCIe PHY.        */
/* The division factor is (RCPREFDIV+1).                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV4_RCPREFDIV_DIV(n)   ((n) - 1)

/*---------------------------------------------------------------------------------------------------------*/
/* JTM1CKDV (JTAG Master 1 Clock Divider Control). Not required.                                           */
/* Sets the division factor from AHB clock to JTM1 clock.                                                  */
/* The division factor is (JTM1CKDIV+1), where JTM1CKDIV is 0 to 4095. Default is divide by 1.             */
/* After changing this field, ensure a delay of 8200 AHB clock cycles before changing CPUCKSEL             */
/* field in CLKSEL register or accessing the AHB18 bus.                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define  CLKDIV4_JTM1CKDV_DIV(n)   ((n) - 1)

/*---------------------------------------------------------------------------------------------------------*/
/* TH_DIV (Throttling Divider). Controls dividing the CPU clock (Cortex A35 quad core) without changing    */
/* most of the bus clock frequencies. This means that the CPU clock can be slowed, and there is no effect  */
/* on any interface that uses a bus clock. Can be used to temporarily change the CPU clock frequency and   */
/* reduce power when too hot. This bit can be changed at any time, and it will be properly synchronized    */
/* to the CPU clock                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#define THRTL_CNT_TH_DIV_DIV(n)   ((n) - 1)



/*---------------------------------------------------------------------------------------------------------*/
/* PWDEN:  PLL power down values                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define  PLLCONn_PWDEN_NORMAL            0
#define  PLLCONn_PWDEN_POWER_DOWN        1



#endif // CLK_REGS_H

