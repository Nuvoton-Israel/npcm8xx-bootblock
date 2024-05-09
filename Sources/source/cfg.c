/*----------------------------------------------------------------------------*/
/*   SPDX-License-Identifier: GPL-2.0                                         */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   cfg.c                                                                    */
/* This file contains configuration routines for ROM code and Booter          */
/*  Project:  Arbel                                                           */
/*----------------------------------------------------------------------------*/
#define CFG_C

#include <string.h>
#include "hal.h"
#include "../SWC_HAL/hal_regs.h"
#include "boot.h"
#include "cfg.h"
#include "apps/serial_printf/serial_printf.h"


/*---------------------------------------------------------------------------------------------------------*/
/* Function Implementation                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/





/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CFG_SHM_ReleaseHostWait                                                                */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine releases the LPC bus stall, allowing host to communicate with the device. */
/*---------------------------------------------------------------------------------------------------------*/
void CFG_SHM_ReleaseHostWait (void)
{
	SET_REG_FIELD(SMC_CTL, SMC_CTL_HOSTWAIT, TRUE);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CFG_PrintResetType                                                                     */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the type of last reset and return reset num                        */
/*---------------------------------------------------------------------------------------------------------*/
int CFG_PrintResetType (void)
{
	int resetNum = 1;
	UINT32 val = REG_READ(RESSR);
	/*-----------------------------------------------------------------------------------------------------*/
	/* on first reset RESSR is cleared by the ROM code. Use INTCR2 inverted value instead                  */
	/*-----------------------------------------------------------------------------------------------------*/
	if ( val == 0)
	{
		val = REG_READ(INTCR2);
	}

	if ( READ_VAR_FIELD(val, RESSR_CORST) == 1 )
		serial_printf(">Last reset was CORST\n");

	if ( READ_VAR_FIELD(val, RESSR_WD0RST) == 1 )
		serial_printf(">Last reset was WD0\n");

	if ( READ_VAR_FIELD(val, RESSR_WD1RST) == 1 )
		serial_printf(">Last reset was WD1\n");

	if ( READ_VAR_FIELD(val, RESSR_WD2RST) == 1 )
		serial_printf(">Last reset was WD2\n");

	if ( READ_VAR_FIELD(val, RESSR_SWRST1) == 1 )
		serial_printf(">Last reset was SW1\n");

	if ( READ_VAR_FIELD(val, RESSR_SWRST2) == 1 )
		serial_printf(">Last reset was SW2\n");

	if ( READ_VAR_FIELD(val, RESSR_SWRST3) == 1 )
		serial_printf(">Last reset was SW3\n");

	if ( READ_VAR_FIELD(val, RESSR_TIP_RESET) == 1 )
		serial_printf(">Last reset was TIP reset\n");

	if ( READ_VAR_FIELD(val, RESSR_PORST) == 1 )
	{
		serial_printf(">Last reset was PORST\n");
		resetNum = 0;
	}

	return resetNum;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CFG_set_flag_update_approve                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void    CFG_SW_reset(void)
{
	serial_printf(KGRN "> SW1 reset...\n\n\n" KNRM);
	SET_REG_FIELD(SWRSTR, SWRSTR_SWRST1, 1);   // get immidiate reset
	while(1);  // will never get here.
}




#undef CFG_C
