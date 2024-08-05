/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   mc_drv.c                                                                 */
/*        This file contains configuration routines for Memory Controller     */
/* Project:                                                                   */
/*        SWC HAL                                                             */
/*----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------*/
/*                         INCLUDES                        */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#include __CHIP_H_FROM_DRV()

#include "hal.h"
#include "mc_drv.h"
#include "mc_regs.h"
#ifndef NO_LIBC
//#include <string.h>
#endif


//#define (mem_stop - mem_start) mem_start0
//#define mem_stop ((UINT32)1<<22) // 4M
#define MEM_TEST_MASK ((UINT32)(mem_stop-1))
extern BOOLEAN   bPrint;

//------------------------------------------------------------------------------------------
// -------   Golden Numbers   ----------
// 23/12/2015: changed Golden_Numbers buffer size to aline 0xFF for simplicity

const UINT32 Golden_Numbers[256] =
{
	// Keep 0         Keep 1
	0x007F00FFU,		~0x007F00FFU,  // lane-1 fixed to 0, lane-0 MSB bit 7 toggle while others are const 1
	0x00BF00FFU,		~0x00BF00FFU,
	0x00DF00FFU,		~0x00DF00FFU,
	0x00EF00FFU,		~0x00EF00FFU,
	0x00F700FFU,		~0x00F700FFU,
	0x00FB00FFU,		~0x00FB00FFU,
	0x00FD00FFU,		~0x00FD00FFU,
	0x00FE00FFU,		~0x00FE00FFU,  // lane-1 fixed to 0, lane-0 MSB bit 0 toggle while others are const 1
	0x00FF007FU,		~0x00FF007FU,  // lane-1 fixed to 0, lane-0 LSB bit 7 toggle while others are const 1
	0x00FF00BFU,		~0x00FF00BFU,
	0x00FF00DFU,		~0x00FF00DFU,
	0x00FF00EFU,		~0x00FF00EFU,
	0x00FF00F7U,		~0x00FF00F7U,
	0x00FF00FBU,		~0x00FF00FBU,
	0x00FF00FDU,		~0x00FF00FDU,
	0x00FF00FEU,		~0x00FF00FEU,  // lane-1 fixed to 0, lane-0 LSB bit 0 toggle while others are const 1
	0x00FF00FFU,		~0x00FF00FFU,  // lane-1 fixed to 0, lane-0 fixed to 1

	0x7F0000FFU,		~0x7F0000FFU,  // lane-1 bit 7 const 0 while others toggle, lane-0 toggle
	0xBF0000FFU,		~0xBF0000FFU,
	0xDF0000FFU,		~0xDF0000FFU,
	0xEF0000FFU,		~0xEF0000FFU,
	0xF70000FFU,		~0xF70000FFU,
	0xFB0000FFU,		~0xFB0000FFU,
	0xFD0000FFU,		~0xFD0000FFU,
	0xFE0000FFU,		~0xFE0000FFU,
	0xFF00007FU,		~0xFF00007FU,  // lane-0 bit 7 const 0 while others toggle, lane-1 toggle
	0xFF0000BFU,		~0xFF0000BFU,
	0xFF0000DFU,		~0xFF0000DFU,
	0xFF0000EFU,		~0xFF0000EFU,
	0xFF0000F7U,		~0xFF0000F7U,
	0xFF0000FBU,		~0xFF0000FBU,
	0xFF0000FDU,		~0xFF0000FDU,
	0xFF0000FEU,		~0xFF0000FEU,  // lane-0 bit 0 const 0 while others toggle, lane-1 toggle
	0xFF0000FFU,		~0xFF0000FFU,  // lane-0 toggle, lane-1 toggle (invert)

	0x007FFF00U,		~0x007FFF00U,
	0x00BFFF00U,		~0x00BFFF00U,
	0x00DFFF00U,		~0x00DFFF00U,
	0x00EFFF00U,		~0x00EFFF00U,
	0x00F7FF00U,		~0x00F7FF00U,
	0x00FBFF00U,		~0x00FBFF00U,
	0x00FDFF00U,		~0x00FDFF00U,
	0x00FEFF00U,		~0x00FEFF00U,
	0x00FF7F00U,		~0x00FF7F00U,
	0x00FFBF00U,		~0x00FFBF00U,
	0x00FFDF00U,		~0x00FFDF00U,
	0x00FFEF00U,		~0x00FFEF00U,
	0x00FFF700U,		~0x00FFF700U,
	0x00FFFB00U,		~0x00FFFB00U,
	0x00FFFD00U,		~0x00FFFD00U,
	0x00FFFE00U,		~0x00FFFE00U,
	0x00FFFF00U,		~0x00FFFF00U, // lane-0 toggle, lane-1 toggle (invert)

	0x7F00FF00U,		~0x7F00FF00U, 	// New numbers 28-10-2010
	0xBF00FF00U,		~0xBF00FF00U,
	0xDF00FF00U,		~0xDF00FF00U,
	0xEF00FF00U,		~0xEF00FF00U,
	0xF700FF00U,		~0xF700FF00U,
	0xFB00FF00U,		~0xFB00FF00U,
	0xFD00FF00U,		~0xFD00FF00U,
	0xFE00FF00U,		~0xFE00FF00U,
	0xFF007F00U,		~0xFF007F00U,
	0xFF00BF00U,		~0xFF00BF00U,
	0xFF00DF00U,		~0xFF00DF00U,
	0xFF00EF00U,		~0xFF00EF00U,
	0xFF00F700U,		~0xFF00F700U,
	0xFF00FB00U,		~0xFF00FB00U,
	0xFF00FD00U,		~0xFF00FD00U,
	0xFF00FE00U,		~0xFF00FE00U,
	0xFF00FF00U,		~0xFF00FF00U,  // lane-1 fixed to 1, lane-0 fixed to 0

	0x7FFF0000U,		~0x7FFF0000U,  // bit 15 const (0 or 1), all others bits are toggle  // New numbers 30-04-2015
	0xBFFF0000U,		~0xBFFF0000U,
	0xDFFF0000U,		~0xDFFF0000U,
	0xEFFF0000U,		~0xEFFF0000U,
	0xF7FF0000U,		~0xF7FF0000U,
	0xFBFF0000U,		~0xFBFF0000U,
	0xFDFF0000U,		~0xFDFF0000U,
	0xFEFF0000U,		~0xFEFF0000U,
	0xFF7F0000U,		~0xFF7F0000U,
	0xFFBF0000U,		~0xFFBF0000U,
	0xFFDF0000U,		~0xFFDF0000U,
	0xFFEF0000U,		~0xFFEF0000U,
	0xFFF70000U,		~0xFFF70000U,
	0xFFFB0000U,		~0xFFFB0000U,
	0xFFFD0000U,		~0xFFFD0000U,
	0xFFFE0000U,		~0xFFFE0000U,  // bit 0 const (0 or 1), all others bits are toggle
	0xFFFF0000U,		~0xFFFF0000U,  // all bits are toggle

	0xAA55AA55U,		~0xAA55AA55U,
	0xFFFFFFFFU,		~0xFFFFFFFFU,   // New numbers 28-10-2010

	//---------------------------
	// padded to aline with 256
	//---------------------------

	0x007F00FFU,		~0x007F00FFU,  // lane-1 fixed to 0, lane-0 MSB bit 7 toggle while others are const 1
	0x00BF00FFU,		~0x00BF00FFU,
	0x00DF00FFU,		~0x00DF00FFU,
	0x00EF00FFU,		~0x00EF00FFU,
	0x00F700FFU,		~0x00F700FFU,
	0x00FB00FFU,		~0x00FB00FFU,
	0x00FD00FFU,		~0x00FD00FFU,
	0x00FE00FFU,		~0x00FE00FFU,  // lane-1 fixed to 0, lane-0 MSB bit 0 toggle while others are const 1
	0x00FF007FU,		~0x00FF007FU,  // lane-1 fixed to 0, lane-0 LSB bit 7 toggle while others are const 1
	0x00FF00BFU,		~0x00FF00BFU,
	0x00FF00DFU,		~0x00FF00DFU,
	0x00FF00EFU,		~0x00FF00EFU,
	0x00FF00F7U,		~0x00FF00F7U,
	0x00FF00FBU,		~0x00FF00FBU,
	0x00FF00FDU,		~0x00FF00FDU,
	0x00FF00FEU,		~0x00FF00FEU,  // lane-1 fixed to 0, lane-0 LSB bit 0 toggle while others are const 1
	0x00FF00FFU,		~0x00FF00FFU,  // lane-1 fixed to 0, lane-0 fixed to 1

	0x7F0000FFU,		~0x7F0000FFU,  // lane-1 bit 7 const 0 while others toggle, lane-0 toggle
	0xBF0000FFU,		~0xBF0000FFU,
	0xDF0000FFU,		~0xDF0000FFU,
	0xEF0000FFU,		~0xEF0000FFU,
	0xF70000FFU,		~0xF70000FFU,
	0xFB0000FFU,		~0xFB0000FFU,
	0xFD0000FFU,		~0xFD0000FFU,
	0xFE0000FFU,		~0xFE0000FFU,
	0xFF00007FU,		~0xFF00007FU,  // lane-0 bit 7 const 0 while others toggle, lane-1 toggle
	0xFF0000BFU,		~0xFF0000BFU,
	0xFF0000DFU,		~0xFF0000DFU,
	0xFF0000EFU,		~0xFF0000EFU,
	0xFF0000F7U,		~0xFF0000F7U,
	0xFF0000FBU,		~0xFF0000FBU,
	0xFF0000FDU,		~0xFF0000FDU,
	0xFF0000FEU,		~0xFF0000FEU,  // lane-0 bit 0 const 0 while others toggle, lane-1 toggle
	0xFF0000FFU,		~0xFF0000FFU,  // lane-0 toggle, lane-1 toggle (invert)

	0x007FFF00U,		~0x007FFF00U,
	0x00BFFF00U,		~0x00BFFF00U,
	0x00DFFF00U,		~0x00DFFF00U,
	0x00EFFF00U,		~0x00EFFF00U,
	0x00F7FF00U,		~0x00F7FF00U,
	0x00FBFF00U,		~0x00FBFF00U,
	0x00FDFF00U,		~0x00FDFF00U,

};


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_mem_test_long                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  itr -                                                                                  */
/*                  mem_start -                                                                            */
/*                  mem_stop -                                                                             */
/*                  print -                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
static int MC_mem_test_long (UINT32 itr, UINT32 mem_start, UINT32 mem_stop, BOOLEAN print)
{
	UINT32 counter = 0;
	UINT32 golden_cnt=0;
	UINT32 index = 0;
	UINT32 TmpAddr, TmpDataRd, TmpDataWr;

	UINT32 g_CPU0_MemTest_Fail = 0; //complete with failure
	UINT32 g_CPU0_MemTest_Loop = 0;
	UINT32 g_CPU0_MemTest_Addr = 0;
	UINT32 g_CPU0_MemTest_Data_Expected = 0;
	UINT32 g_CPU0_MemTest_Data_Read = 0;
	UINT32 g_CPU0_MemTest_Completed = 0;
	UINT32 error = 0;

	HAL_PRINT("\n");
	
	while (counter++ < itr)
	{
		TmpAddr = 0;
		if (print)
			HAL_PRINT_DBG("Start Mem test %d", counter);


		//--------------------------------------
		// write pattern
		//--------------------------------------
		golden_cnt = 0;
		for (index = mem_start ; index < mem_stop; index += 4)
		{
			*(volatile UINT32*)(UINT64)index = Golden_Numbers[golden_cnt++ & 0xFF];
			/*if (index == 0x100)
			{
				*((volatile UINT32*)(TmpAddr+ mem_stop)) = 0xAABBCCDD;
			}*/
			//TmpAddr += index + (4 * 8);
		}

		//---------------------------------------------------
		// read and verify, write anti-pattern (for noise only)
		//----------------------------------------------------
		TmpAddr = 0;

		golden_cnt = 0;


		for (index = mem_start ; index < mem_stop; index += 4)
		{
			TmpDataWr = Golden_Numbers[golden_cnt++ & 0xFF];
			TmpDataRd = *(volatile UINT32*)(UINT64)index;
			if (TmpDataWr != TmpDataRd)
			{
				g_CPU0_MemTest_Fail = TRUE; //complete with failure
				g_CPU0_MemTest_Loop = index;
				g_CPU0_MemTest_Addr = TmpAddr + mem_stop;
				g_CPU0_MemTest_Data_Expected = TmpDataWr;
				g_CPU0_MemTest_Data_Read = TmpDataRd;

				if (print)
					HAL_PRINT("Memtest failed index %d address %#010lx Expected %#010lx read %#010lx ", g_CPU0_MemTest_Loop, index, TmpDataWr, TmpDataRd);
				error++;
				if (error > 100)	return error;

				//while (TRUE);
			}
			//*((volatile UINT32*)(TmpAddr+ mem_stop)) = ~TmpDataWr;
			//TmpAddr += mem_start + (4 * 8) + 4;
			//TmpAddr &= MEM_TEST_MASK;
		}

		// UINT8 a line test
#if 1
		//--------------------------------------
		// write pattern
		//--------------------------------------
		TmpAddr = mem_start;
		golden_cnt = 0;
		for (index = mem_start ; index < mem_stop; index++)
		{
			TmpDataWr = Golden_Numbers[golden_cnt++ & 0xFF];
			*(volatile UINT8*)(UINT64)TmpAddr = (UINT8)TmpDataWr;

			TmpAddr += (4 * 8) + sizeof(UINT8);
			TmpAddr &= MEM_TEST_MASK;
		}

		//---------------------------------------------------
		// read and verify
		//----------------------------------------------------
		TmpAddr = mem_start;
		golden_cnt = 0;
		for (index = mem_start ; index < mem_stop; index += 1)
		{
			TmpDataWr = Golden_Numbers[golden_cnt++ & 0xFF];
			TmpDataRd = *(volatile UINT8*)(UINT64)TmpAddr;
			if ((UINT8)TmpDataWr != (UINT8)TmpDataRd)
			{
				g_CPU0_MemTest_Fail = TRUE; //complete with failure
				g_CPU0_MemTest_Loop = index;
				g_CPU0_MemTest_Addr = TmpAddr + mem_stop;
				g_CPU0_MemTest_Data_Expected = TmpDataWr;
				g_CPU0_MemTest_Data_Read = TmpDataRd;

				if (print)
					HAL_PRINT("Memtest failed index %d address %#010lx Expected %#010lx read %#010lx ", g_CPU0_MemTest_Loop, index, TmpDataRd, TmpDataWr);
				error++;

				if (error > 100)	return error;
			}
			TmpAddr += (4 * 8) + sizeof(UINT8);
			TmpAddr &= MEM_TEST_MASK;
		}
#endif
	}
	//----------------------------------------
//#ifdef CACHE_ENABLE
//	dcache_disable();
//#endif
	g_CPU0_MemTest_Completed = TRUE;

	if (g_CPU0_MemTest_Completed == TRUE)
	{
		g_CPU0_MemTest_Data_Read = (mem_stop - mem_start);
		HAL_PRINT_DBG("Memtest completed 0x%X bytes\n", g_CPU0_MemTest_Data_Read);
		if (print)
			HAL_PRINT("Test pass\n");
	}
	else
	{
		if (print)
			HAL_PRINT("Test fail\n");

	}

	return error;


}