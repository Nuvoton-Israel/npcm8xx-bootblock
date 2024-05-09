/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   tmc_regs.h                                                               */
/*            This file contains TMC module registers                         */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/
#ifndef _TMC_REGS_H
#define _TMC_REGS_H

#include __CHIP_H_FROM_DRV()


/**************************************************************************************************************************/
/*   Full System Watchdog Control Register (FSWCR)                                                                        */
/**************************************************************************************************************************/
#define  FSWCR                          (FWS_BASE_ADDR + 0x001C) , TMC_ACCESS, 32   /* Offset: TMC_BA + 001Ch */
#define  FSWCR_WDT_CNT                   16 , 8              /* WDT_CNT (Watchdog Count). Determines the number of WD intervals until the watchdog interrupt     */
#define  FSWCR_WTLK                      15 , 1              /* WTLK (Watchdog Timer Lock)       */
#define  FSWCR_WTCLK                     10 , 2              /* 11-10 WTCLK (Watchdog Timer Clock Divide). Determines whether the watchdog timer clock input is                       */
#define  FSWCR_FREEZE_EN                 9 , 1               /* 9 FREEZE_EN (ICE Debug Mode Acknowledge Enable - nDBGACK_EN).                                                         */
#define  FSWCR_WTE                       7 , 1               /* 7 WTE (Watchdog Timer Enable).                                                                                        */
#define  FSWCR_WTIS                      4 , 2               /* 5-4 WTIS (Watchdog Timer Interval Select). Selects the watchdog timer interval. No matter which interval              */
#define  FSWCR_WTRF                      2 , 1               /* 2 WTRF (Watchdog Timer Reset Flag). When the watchdog timer initiates a reset, the hardware sets                      */
#define  FSWCR_WTRE                      1 , 1               /* 1 WTRE (Watchdog Timer Reset Enable). Setting this bit enables the Watchdog Timer reset function.                     */
#define  FSWCR_WTR                       0 , 1               /* 0 WTR (Watchdog Timer Reset). Brings the watchdog timer into a known state. It resets the watchdog                    */


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                              TMR Registers                                              */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/***********************************************************************************************************/
/*    Timer Control and Status Register 0_4 (TCR0_4)                                                       */
/***********************************************************************************************************/
#define TCR0_4(m, n)                         (TMC_BASE_ADDR(m) + (n == 0 ? 0x0000 : (n == 1 ? 0x0004 : (n == 2 ? 0x0020 : (n == 3 ? 0x0024 : 0x0040))))),  MEM,    32         /* Offset: 0000h-0040h */
#define TCR0_4_FREEZE_EN                      31,   1               /* 31 FREEZE_EN (ICE Debug Mode Acknowledge (nDBGACK_EN) */
#define TCR0_4_CEN                            30,   1               /* 30 CEN (Counter Enable) */
#define TCR0_4_IE                             29,   1               /* 29 IE (Interrupt Enable) */
#define TCR0_4_MODE                           27,   2               /* 27-28 MODE (Timer Operating Mode) */
#define TCR0_4_CRST                           26,   1               /* 26 CRST (Counter Reset) */
#define TCR0_4_CACT                           25,   1               /* 25 CACT (Timer is in Active) */
#define TCR0_4_PRESCALE                       0,    8               /* 0-7 PRESCALE (Clock Prescale Divide Count) */

/***********************************************************************************************************/
/*    Timer Initial Count Register 0_4 (TICR0_4)                                                           */
/***********************************************************************************************************/
#define TICR0_4(m, n)                        (TMC_BASE_ADDR(m) + (n == 0 ? 0x0008 : (n == 1 ? 0x000C : (n == 2 ? 0x0028 : (n == 3 ? 0x002C : 0x0048))))),  MEM,    32         /* Offset: 0008h-0048h */
#define TICR0_4_TIC                           0,    32               /* 0-31 TIC (Timer Initial Count) */

/***********************************************************************************************************/
/*    Timer Data Register 0_4 (TDR0_4)                                                                     */
/***********************************************************************************************************/
#define TDR0_4(m, n)                         (TMC_BASE_ADDR(m) + (n == 0 ? 0x0010 : (n == 1 ? 0x0014 : (n == 2 ? 0x0030 : (n == 3 ? 0x0034 : 0x0050))))),  MEM,    32         /* Offset: 0010h-0050h */
#define TDR0_4_TDR                            0,    32               /* 0-31 TDR (Timer Data Register) */

/***********************************************************************************************************/
/*    Timer Interrupt Status Register (TISR)                                                               */
/***********************************************************************************************************/
#define TISR(m)                              (TMC_BASE_ADDR(m) + 0x0018),  MEM,    32         /* Offset: 0018h */
#define TISR_TIF4                             4,    1               /* 4 TIF4 (Timer Interrupt Flag 4) */
#define TISR_TIF3                             3,    1               /* 3 TIF3 (Timer Interrupt Flag 3) */
#define TISR_TIF2                             2,    1               /* 2 TIF2 (Timer Interrupt Flag 2) */
#define TISR_TIF1                             1,    1               /* 1 TIF1 (Timer Interrupt Flag 1) */
#define TISR_TIF0                             0,    1               /* 0 TIF0 (Timer Interrupt Flag 0) */

/***********************************************************************************************************/
/*    Watchdog Timer Control Register (WTCR)                                                               */
/***********************************************************************************************************/
#define WTCR(m)                              (TMC_BASE_ADDR(m) + 0x001C),  MEM,    32         /* Offset: 001Ch */
#define WTCR_WDT_CNT                          16,   8               /* 16-23 WDT_CNT (Watchdog Count) */
#define WTCR_WTLK                             15,   1               /* 15 WTLK (Watchdog Timer Lock) */
#define WTCR_WTCLK                            10,   2               /* 10-11 WTCLK (Watchdog Timer Clock Divide) */
#define WTCR_FREEZE_EN                        9,    1               /* 9 FREEZE_EN (ICE Debug Mode Acknowledge Enable _ nDBGACK_EN) */
#define WTCR_WTTME                            8,    1               /* 8 WTTME (Watchdog Timer Test Mode Enable) */
#define WTCR_WTE                              7,    1               /* 7 WTE (Watchdog Timer Enable) */
#define WTCR_WTIE                             6,    1               /* 6 WTIE (Watchdog Timer Interrupt Enable) */
#define WTCR_WTIS                             4,    2               /* 4-5 WTIS (Watchdog Timer Interval Select) */
#define WTCR_WTIF                             3,    1               /* 3 WTIF (Watchdog Timer Interrupt Flag) */
#define WTCR_WTRF                             2,    1               /* 2 WTRF (Watchdog Timer Reset Flag) */
#define WTCR_WTRE                             1,    1               /* 1 WTRE (Watchdog Timer Reset Enable) */
#define WTCR_WTR                              0,    1               /* 0 WTR (Watchdog Timer Reset) */

/***********************************************************************************************************/
/*    Timer Version Register (TIMVER) (New in ARBEL)                                                       */
/***********************************************************************************************************/
#define TIMVER(m)                            (TMC_BASE_ADDR(m) + 0x007C),  MEM,    32         /* Offset: 007Ch */
#define TIMVER_VERSION                        0,    8               /* 0-7 VERSION (Version) */


typedef enum TCR_MODE_type
{
    TCR_MODE_ONESHOT   = 0x0,
    TCR_MODE_PERIODIC  = 0x1,
    TCR_MODE_TOGGLE    = 0x2,
} TCR_MODE_T;

/*---------------------------------------------------------------------------------------------------------*/
/* WTIS (Watchdog Timer Interval Select). Selects the watchdog timer interval. No matter which interval    */
/* is chosen, the reset timeout always occurs 1024 prescale clocks later than the interrupt on timeout.    */
/*---------------------------------------------------------------------------------------------------------*/
                        /* Bits     Interrupt TO    Reset Timeout          Real Time Interval    Real Time Interval   Real Time Interval   */
                        /* 5 4                                             CLK = 25 MHz / 256    CLK = 25 MHz /2048   CLK = 25 MHz / 65536 */
#define  WTIS_MODE_0      0x00  /* 2^14 clocks      2^14 + 1024 clocks     0.17 sec              1.42606336 sec       43 sec  */
#define  WTIS_MODE_1      0x01  /* 2^16 clocks      2^16 + 1024 clocks     0.67 sec              5.4525952 sec        172 sec   */
#define  WTIS_MODE_2      0x02  /* 2^18 clocks      2^18 + 1024 clocks     2.7 sec               21.55872256 sec      687 sec   */
#define  WTIS_MODE_3      0x03  /* 2^20 clocks      2^20 + 1024 clocks     10.7 sec              85.983232 sec        2750 sec  */

/*---------------------------------------------------------------------------------------------------------*/
/* WTCLK (Watchdog Timer Clock Divide). Determines whether the watchdog timer clock input is               */
/* divided. The clock source of the watchdog timer is the UART clock (24 MHz).                             */
/* When WTTME = 1, setting this field has no effect on the watchdog timer clock (using original clk input).*/
/*---------------------------------------------------------------------------------------------------------*/
#define    WTCLK_DIV_1             0x00   /*  0 0: Using original clock input     */
#define    WTCLK_DIV_256           0x01   /*  0 1: Clock input divided by 256     */
#define    WTCLK_DIV_2048          0x02   /*  1 0: Clock input divided by 2048    */
#define    WTCLK_DIV_65536         0x03   /*  1 1: Clock input divided by 65536   */


#endif // _TMC_REGS_H
