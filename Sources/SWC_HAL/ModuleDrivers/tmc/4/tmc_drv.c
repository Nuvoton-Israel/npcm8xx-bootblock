/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   tmc_drv.c                                                                */
/*            This file contains TMC module implementation                    */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/


#include __CHIP_H_FROM_DRV()

#include "hal.h"

#include "tmc_drv.h"
#include "tmc_regs.h"


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        TMC_watchdogReset                                                                      */
/*                                                                                                         */
/* Parameters:      wd - one per timer module                                                              */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs watchdog reset                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void TMC_WatchdogReset (TMC_MODULE_T wd)
{
    volatile INT i = 1;

    REG_WRITE(WTCR(wd), 0);

}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        TMC_WatchdogTouch                                                                      */
/*                                                                                                         */
/* Parameters:      wd - one per timer module                                                              */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                   This routine performs watchdog touch for a running WD                                 */
/*---------------------------------------------------------------------------------------------------------*/
void TMC_WatchdogTouch (_UNUSED_ TMC_MODULE_T wd)
{
    SET_REG_FIELD(WTCR(wd), WTCR_WTR, 1);
    return;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        TMC_StartWatchDog                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  wd    -   watchdog number (one per module)                                             */
/*                  wd_cnt -  watchdog counter                                                             */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine starts the wd                                                             */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS TMC_StartWatchDog (TMC_MODULE_T wd, UINT32 wd_cnt)
{
    UINT32 VAR_WTCR = 0;

    /*-----------------------------------------------------------------------------------------------------*/
    /* If clock was not configured (warm boot, skip init) - need to set the timer clock.                   */
    /*-----------------------------------------------------------------------------------------------------*/
    if ( CHIP_CfgWasDone() == FALSE)
    {
        CLK_ConfigureTimerClock();
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* SW reset the timer                                                                                  */
    /*-----------------------------------------------------------------------------------------------------*/
    CLK_ResetTIMER(wd);

    switch (wd)
    {
        case TMC_MODULE_0:
            CLK_ResetTIMER(0);  /* cahnnels 0 to 4 belong to TMC_0 */
            break;
        case TMC_MODULE_1:
            CLK_ResetTIMER(5);  /* cahnnels 5 to 9 belong to TMC_1 */
            break;
        case TMC_MODULE_2:
            CLK_ResetTIMER(10); /* cahnnels 10 to 14 belong to TMC_2 */
            break;

        default:
            return DEFS_STATUS_INVALID_PARAMETER;
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* Restart timer to a known state                                                                      */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD(  WTCR(wd), WTCR_WTR, 1);

    /*-----------------------------------------------------------------------------------------------------*/
    /* 0: When the debugger is active (DBGACK is high), the timer clock is stopped.                        */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_VAR_FIELD(VAR_WTCR, WTCR_FREEZE_EN, 0);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Config for ~0 sec. TODO: set to any rate.                                                           */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_VAR_FIELD(VAR_WTCR, WTCR_WTIS , WTIS_MODE_0);
    SET_VAR_FIELD(VAR_WTCR, WTCR_WTCLK, WTCLK_DIV_1);  /* the minimum timeout */

    /*-----------------------------------------------------------------------------------------------------*/
    /* Enables Watchdog Timer reset function.                                                              */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_VAR_FIELD(VAR_WTCR, WTCR_WTRE , 1);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Enable Watchdog                                                                                     */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_VAR_FIELD(VAR_WTCR, WTCR_WTE, 1);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Set time                                                                                            */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_VAR_FIELD(VAR_WTCR, WTCR_WDT_CNT, wd_cnt);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Start the WD                                                                                        */
    /*-----------------------------------------------------------------------------------------------------*/
    REG_WRITE(WTCR(wd), VAR_WTCR);

    return DEFS_STATUS_OK;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        TMC_StopWatchDog                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  wd    -   watchdog number (one per module)                                             */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine stops the wd and clear all pending interrupts                             */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS TMC_StopWatchDog (TMC_MODULE_T wd)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* Disable WD                                                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD(WTCR(wd), WTCR_WTE, 0);

    return DEFS_STATUS_OK;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        TMC_PrintRegs                                                                          */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module registers                                               */
/*---------------------------------------------------------------------------------------------------------*/
void TMC_PrintRegs (void)
{
    UINT i;

    HAL_PRINT("/*--------------*/\n");
    HAL_PRINT("/*     TMC      */\n");
    HAL_PRINT("/*--------------*/\n\n");

    for (i = 0; i < TMC_NUM_OF_MODULES; i++)
    {
        TMC_PrintModuleRegs((TMC_MODULE_T)i);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        TMC_PrintModuleRegs                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  wd - The watchdog number (module) to be printed.                                       */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module instance registers                                      */
/*lint -e{715}      Suppress 'wd' not referenced                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void TMC_PrintModuleRegs (_UNUSED_ TMC_MODULE_T wd)
{
    ASSERT(wd < TMC_NUM_OF_MODULES);

    HAL_PRINT("TMC%d:\n", wd);
    HAL_PRINT("------\n");
    HAL_PRINT("WTCR               = 0x%08X\n", REG_READ(WTCR(wd)));
    HAL_PRINT("TIMVER              = 0x%08X\n", REG_READ(TIMVER(wd)));

    HAL_PRINT("\n");
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        TMC_PrintVersion                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module version                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void TMC_PrintVersion (void)
{
    HAL_PRINT("TMC         = %X\n", MODULE_VERSION(TMC_MODULE_TYPE));
}

