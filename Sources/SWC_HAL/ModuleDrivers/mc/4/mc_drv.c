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
/*                         INCLUDES                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#include __CHIP_H_FROM_DRV()

#include "hal.h"
#include "mc_drv.h"
#include "mc_regs.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#if (defined(DEBUG_LOG) || defined(DEV_LOG))
#define HAL_PRINT_DBG(...)          \
	{                               \
		if (bPrint)                 \
		{                           \
			HAL_PRINT(__VA_ARGS__); \
		}                           \
	}
#else
#define HAL_PRINT(x, ...) (void)0
#define HAL_PRINT_DBG(x, ...) (void)0
#endif

void *
memset(void *dest, int val, size_t len)
{
	volatile unsigned char *ptr = dest;
	while (len-- > 0)
		*ptr++ = val;
	return dest;
}

void *
memcpy(void *dest, const void *src, size_t len)
{
	volatile char *d = dest;
	const char *s = src;
	while (len--)
		*d++ = *s++;
	return dest;
}


#define BOOTBLOCK_IMAGE_START __bootblock_start
#define BOOTBLOCK_IMAGE_END __bootblock_end

#define NUM_OF_LANES_MAX 2

static unsigned int NUM_OF_LANES = 2;
BOOLEAN bPrint;

// Note:
// PHY is set to VREF_STARTPOINT value. This is a tyical value for DDR_DRV_DQ_48R with PHY_ODT_DQ_120R. VREF training will later-on look for the optimal value.
// DDR device is set to VREF_STARTPOINT value. This is tyical value for PHY_DRV_DQ_48R with DDR_ODT_DQ_120R. VREF training will look for the optimal value.
// For a different DRV and ODT values, VREF_STARTPOINT must be upadte accordantly.
// In case VREF training is skiped, this value will be use for the PHY VREF and user should issue MR6 write to set DDR device VREF value.
// In Z11, PHY VREF bit 0 is tie to VREF select therefore must be fixed to 1
#define VREF_STARTPOINT (0x7 | (1)) // @ range 1 it value is 65% of DRAM Voltage.

static void                 Run_Memory_SI_Test        (DDR_Setup *ddr_setup);
static void                 FindMidBiggestEyeBitwise_l(const UINT16 *SweepDataBuff, const int BuffSize, volatile int *pEyeCenter, volatile int *pEyeSize, int bits);
static DEFS_STATUS          MC_ConfigureDDR_l         (DDR_Setup *ddr_setup, unsigned int try );
static void                 MC_PrintLeveling          (BOOLEAN bIn, BOOLEAN bOut);
static void                 MC_write_mr_regs_all      (void);
static void                 MC_write_mr_regs_single   (UINT8 Index, UINT32 Data);
static UINT16               GetECCSyndrom             (UINT32 readVal);
static void                 Set_DQS_in_val            (int ilane, UINT32 input_val,  BOOLEAN bOverride, BOOLEAN bPrint);
static void                 Set_DQS_out_val           (int ilane, UINT32 output_val, BOOLEAN bOverride, BOOLEAN bPrint);
static BOOLEAN              MC_IsDDRConfigured_l      (void);
static void                 MC_ECC_Enable_l           (BOOLEAN bEn);
static void                 MC_ECC_Init_l             (DDR_Setup *ddr_setup);
static void                 MC_PrintPhy               (void);
static void                 MC_PrintTrim              (BOOLEAN bIn, BOOLEAN bOut);
static void                 MC_BIST_Init_DRAM_mem     (UINT32 start_address, UINT64 size, UINT32 dataPattern);
static void                 setup_registers_MRS       (UINT32 index, UINT32 data, DDR_Setup *ddr_setup );

static UINT32 volatile g_fail_rate_0;
static UINT32 volatile g_fail_rate_1;

#define BINARY_PRINT16(x) READ_VAR_BIT(x, 15), READ_VAR_BIT(x, 14), READ_VAR_BIT(x, 13), READ_VAR_BIT(x, 12), \
						  READ_VAR_BIT(x, 11), READ_VAR_BIT(x, 10), READ_VAR_BIT(x, 9), READ_VAR_BIT(x, 8),   \
						  READ_VAR_BIT(x, 7), READ_VAR_BIT(x, 6), READ_VAR_BIT(x, 5), READ_VAR_BIT(x, 4),     \
						  READ_VAR_BIT(x, 3), READ_VAR_BIT(x, 2), READ_VAR_BIT(x, 1), READ_VAR_BIT(x, 0)
#define BINARY_PRINT4(x) READ_VAR_BIT(x, 3), READ_VAR_BIT(x, 2), READ_VAR_BIT(x, 1), READ_VAR_BIT(x, 0)
#define BINARY_PRINT7(x) READ_VAR_BIT(x, 6), READ_VAR_BIT(x, 5), READ_VAR_BIT(x, 4), \
						 READ_VAR_BIT(x, 3), READ_VAR_BIT(x, 2), READ_VAR_BIT(x, 1), READ_VAR_BIT(x, 0)

/*---------------------------------------------------------------------------------------------------------*/
/* Trim values are 2's complement, on 7 bits instead of 8. These are the conversion macros:                */
/*---------------------------------------------------------------------------------------------------------*/
#define TWOS_COMP_7BIT_REG_TO_VALUE(x) ((int)((((x) & 0x40) == 0) ? (-((x) & 0x3F)) : ((x) & 0x3F)))
#define TWOS_COMP_7BIT_VALUE_TO_REG(x) (((UINT32)(((x) >= 0) ? (0x40 | (x)) : (-(x))) & 0x7F))

#include "arbel_mc_init.c"
#include "mc_drv_sweeps.c"
#include "ddr_phy_cfg1.c"
#include "ddr_phy_cfg2.c"
#include "mc_drv_mem_test.c"

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_ClearInterrupts                                                                     */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine clear all MC interrupts. Called from error handler too.                   */
/*---------------------------------------------------------------------------------------------------------*/
void MC_ClearInterrupts(void)
{
	HAL_PRINT_DBG("\tDENALI_CTL_144 %#010lx ", REG_READ(DENALI_CTL_144));
	HAL_PRINT_DBG("145 %#010lx ", REG_READ(DENALI_CTL_145));
	HAL_PRINT_DBG("146 %#010lx ", REG_READ(DENALI_CTL_146));
	HAL_PRINT_DBG("147 %#010lx ", REG_READ(DENALI_CTL_147));
	HAL_PRINT_DBG("148 %#010lx ", REG_READ(DENALI_CTL_148));
	HAL_PRINT_DBG("149 %#010lx ", REG_READ(DENALI_CTL_149));
	HAL_PRINT_DBG("150  %#010lx ", REG_READ(DENALI_CTL_150));
	HAL_PRINT_DBG("1303 %#010lx ", REG_READ(DENALI_CTL_1303));
	HAL_PRINT_DBG("1304 %#010lx \n", REG_READ(DENALI_CTL_1304));
	HAL_PRINT_DBG("\t95 %#010lx ", REG_READ(DENALI_CTL_95));
	HAL_PRINT_DBG("96 %#010lx ", REG_READ(DENALI_CTL_96));
	HAL_PRINT_DBG("97 %#010lx ", REG_READ(DENALI_CTL_97));
	HAL_PRINT_DBG("98 %#010lx ", REG_READ(DENALI_CTL_98));
	HAL_PRINT_DBG("99 %#010lx ", REG_READ(DENALI_CTL_99));
	HAL_PRINT_DBG("100 %#010lx ", REG_READ(DENALI_CTL_100));
	HAL_PRINT_DBG("101 %#010lx ", REG_READ(DENALI_CTL_101));
	HAL_PRINT_DBG("102 %#010lx ", REG_READ(DENALI_CTL_102));
	HAL_PRINT_DBG("103 %#010lx \n", REG_READ(DENALI_CTL_103));

	HAL_PRINT_DBG(KNRM "* clear MC interrupts\n");

	REG_WRITE(DENALI_CTL_144, 0xFFFFFFFF);	// DENALI_CTL_144_INT_ACK_TIMEOUT
	REG_WRITE(DENALI_CTL_145, 0xFFFFFFFF);	// DENALI_CTL_145_INT_ACK_ECC // DENALI_CTL_145_INT_ACK_LOWPOWER
	REG_WRITE(DENALI_CTL_146, 0xFFFFFFFF);	// reserved.
	REG_WRITE(DENALI_CTL_147, 0xFFFFFFFF);	// DENALI_CTL_147_INT_ACK_TRAINING
	REG_WRITE(DENALI_CTL_148, 0xFFFFFFFF);	// DENALI_CTL_148_INT_ACK_BIST  // DENALI_CTL_148_INT_ACK_DFI
	REG_WRITE(DENALI_CTL_149, 0xFFFFFFFF);	// DENALI_CTL_149_INT_ACK_INIT,  // DENALI_CTL_149_INT_ACK_MISC
	REG_WRITE(DENALI_CTL_150, 0xFFFFFFFF);	// DENALI_CTL_150_INT_ACK_MODE // DENALI_CTL_150_INT_ACK_PARITY
	REG_WRITE(DENALI_CTL_1304, 0xFFFFFFFF); // DENALI_CTL_1304_INT_ACK_USERIF

	HAL_PRINT_DBG("\tDENALI_CTL_158 %#010lx ", REG_READ(DENALI_CTL_158));
	HAL_PRINT_DBG("159 %#010lx ", REG_READ(DENALI_CTL_159));
	HAL_PRINT_DBG("160 %#010lx ", REG_READ(DENALI_CTL_160));
	HAL_PRINT_DBG("167 %#010lx ", REG_READ(DENALI_CTL_167));
	HAL_PRINT_DBG("168 %#010lx ", REG_READ(DENALI_CTL_168));
	HAL_PRINT_DBG("56  %#010lx ", REG_READ(DENALI_CTL_56));
	HAL_PRINT_DBG("1291 %#010lx \n", REG_READ(DENALI_CTL_1291));

	HAL_PRINT_DBG("\tDENALI_CTL_144 %#010lx ", REG_READ(DENALI_CTL_144));
	HAL_PRINT_DBG("145 %#010lx ", REG_READ(DENALI_CTL_145));
	HAL_PRINT_DBG("146 %#010lx ", REG_READ(DENALI_CTL_146));
	HAL_PRINT_DBG("147 %#010lx ", REG_READ(DENALI_CTL_147));
	HAL_PRINT_DBG("148 %#010lx ", REG_READ(DENALI_CTL_148));
	HAL_PRINT_DBG("149 %#010lx ", REG_READ(DENALI_CTL_149));
	HAL_PRINT_DBG("150  %#010lx ", REG_READ(DENALI_CTL_150));
	HAL_PRINT_DBG("1303 %#010lx ", REG_READ(DENALI_CTL_1303));
	HAL_PRINT_DBG("1304 %#010lx \n", REG_READ(DENALI_CTL_1304));
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_ConfigureDDR                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  mc_config - value from  BOOTBLOCK_Get_MC_config (BB header)                            */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Set default configuration for the DDR Memory Controller                                */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS MC_ConfigureDDR(DDR_Setup *ddr_setup)
{
	DEFS_STATUS   status =  DEFS_STATUS_OK;

	unsigned int  try = 0;
	UINT32        iCnt = 0;
	UINT16        mem_test  =0;
	UINT32        error = 0;
	DEFS_STATUS   status_out_sweep =  DEFS_STATUS_OK;
	DEFS_STATUS   status_dqs_sweep =  DEFS_STATUS_OK;
	int           l_BestEyeCenter[8 * NUM_OF_LANES_MAX], l_BestEyeSize[8 * NUM_OF_LANES_MAX];

	ddr_setup->cpu_clk = CLK_GetCPUFreq();

	bPrint = (BOOLEAN)(ddr_setup->print_enable > 0);

	HAL_PRINT(KGRN "MC init\n" KNRM);

	// if memory controller was already initialized do not reinitialize
	if (MC_IsDDRConfigured_l())
	{
		HAL_PRINT("\n>MC already configured.\n");
		return DEFS_STATUS_SYSTEM_NOT_INITIALIZED;
	}

	if (ddr_setup->b_gpio_test_complete)
	{
		HAL_PRINT("init gpio complete %d\n", ddr_setup->mc_gpio_test_complete);
		GPIO_Write(ddr_setup->mc_gpio_test_complete, !ddr_setup->mc_gpio_test_complete_active_low);
		GPIO_Init(ddr_setup->mc_gpio_test_complete, GPIO_DIR_OUTPUT, GPIO_PULL_NONE, GPIO_OTYPE_PUSH_PULL, FALSE);
	}
	if (ddr_setup->b_gpio_test_pass)
	{
		HAL_PRINT("init gpio pass %d\n", ddr_setup->mc_gpio_test_pass);
		GPIO_Write(ddr_setup->mc_gpio_test_pass, !ddr_setup->mc_gpio_test_pass_active_low);
		GPIO_Init(ddr_setup->mc_gpio_test_pass, GPIO_DIR_OUTPUT, GPIO_PULL_NONE, GPIO_OTYPE_PUSH_PULL, FALSE);
	}

	// mark that initialization was not finished
	SET_REG_FIELD(INTCR2, INTCR2_MC_INIT, 0);

	// mask all MC interrupts:
	REG_WRITE(DENALI_CTL_136, 0xFFFFFFFF);
	REG_WRITE(DENALI_CTL_151, 0xFFFFFFFF);
	REG_WRITE(DENALI_CTL_152, 0xFFFFFFFF);
	REG_WRITE(DENALI_CTL_154, 0xFFFFFFFF);
	REG_WRITE(DENALI_CTL_155, 0xFFFFFFFF);
	REG_WRITE(DENALI_CTL_156, 0xFFFFFFFF);
	REG_WRITE(DENALI_CTL_157, 0xFFFFFFFF);
	REG_WRITE(DENALI_CTL_1305, 0xFFFFFFFF);

	// clear all MC interrupts:
	MC_ClearInterrupts();

	HAL_PRINT_DBG("MC init disable int\n");

	HAL_PRINT("\n\n ****** DDR4 Init mc_config = %#010lx \n", ddr_setup->mc_config);

	status = MC_ConfigureDDR_l(ddr_setup, try);

	// TODO: SDRAM_PrintRegs(); // Only when MC was init. When MC was not init, can't read/write MR registers.
	HAL_PRINT_DBG("\n\n");

	ddr_setup->ddr_size = MC_CheckDramSize(ddr_setup);
	MC_UpdateDramSize(ddr_setup, ddr_setup->ddr_size);

	if (status == DEFS_STATUS_OK) // ddr_setup->ddr_size != 0)
	{
		HAL_PRINT("size detected OK\n");


		error = MC_mem_test_long(1, 0, _128KB_, TRUE);	
		HAL_PRINT("mem_test1=0x%x\n", error);

		if (error == 0)
		{
			status = DEFS_STATUS_OK;
			goto done;
		}
	}
	else
	{
		HAL_PRINT(KRED "training fail\n" KNRM);
		MC_PrintRegs();
		MC_PrintPhy();
		return status;
	}

done:

	if (ddr_setup->ECC_enable)
	{
		MC_ECC_Init_l(ddr_setup);
	}

	MC_ClearInterrupts();

	if (ddr_setup->ddr_size > 0)
	{
		HAL_PRINT(KGRN "\n\nDDR training pass, do mem tests: \n\n  *** DRAM SIZE %#010lx, ECC %s, %s\n" KNRM,
				  ddr_setup->ddr_size,
				  ddr_setup->ECC_enable ? "Enable" : "Disable",
				  ddr_setup->ddr_ddp ? "DDP" : "SDP");

		error = MC_mem_test_long(2, 0, _128KB_, TRUE);

		HAL_PRINT("mem_test2=0x%x\n", error);
		if ((error == 0) && ddr_setup->b_gpio_test_pass)
		{
			GPIO_Write(ddr_setup->mc_gpio_test_pass, ddr_setup->mc_gpio_test_pass_active_low);
			HAL_PRINT("set gpio pass %d val %d\n", ddr_setup->mc_gpio_test_pass, ddr_setup->mc_gpio_test_pass_active_low);
		}
		
		if (ddr_setup->b_gpio_test_complete)
		{
			GPIO_Write(ddr_setup->mc_gpio_test_complete, ddr_setup->mc_gpio_test_complete_active_low);
			HAL_PRINT("set gpio complete %d val %d\n", ddr_setup->mc_gpio_test_complete, ddr_setup->mc_gpio_test_complete_active_low);
		}

		if (ddr_setup->ECC_enable == FALSE)
		{
			MC_BIST_Init_DRAM_mem(0x0, ddr_setup->ddr_size, 0x14000000);
		}
	}
	else
	{
		MC_PrintRegs();
		MC_PrintPhy();
		HAL_PRINT(KRED "\n****************************\n" KNRM);
		HAL_PRINT(KRED "*** Your SDRAM is Faulty ***\n" KNRM);
		HAL_PRINT(KRED "****************************\n" KNRM);
	}

	MC_ClearInterrupts();

	// clear all MC interrupts, again:
	MC_ClearInterrupts();

	// un-mask all MC interrupts:
	HAL_PRINT_DBG(KNRM "\nunmask MC interrupts\n");
	REG_WRITE(DENALI_CTL_136, 0);
	REG_WRITE(DENALI_CTL_151, 0);
	REG_WRITE(DENALI_CTL_152, 0);
	REG_WRITE(DENALI_CTL_154, 0);
	REG_WRITE(DENALI_CTL_155, 0);
	REG_WRITE(DENALI_CTL_156, 0);
	REG_WRITE(DENALI_CTL_157, 0);
	REG_WRITE(DENALI_CTL_1305, 0);

	HAL_PRINT_DBG(KNRM "\nmc_init done\n");
	SET_REG_FIELD(INTCR2, INTCR2_MC_INIT, 1);

	return DEFS_STATUS_OK;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_SetBankRowCol                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  bank_diff -                                                                            */
/*                  col_diff -                                                                             */
/*                  row_diff -                                                                             */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine sets row diff bank diff and coloun diff, which sets how many signals are  */
/*                  not used to access the DRAM                                                            */
/*---------------------------------------------------------------------------------------------------------*/
static void MC_SetBankRowCol(int bank_diff, int row_diff, int col_diff)
{
	SET_REG_FIELD(DENALI_CTL_124, DENALI_CTL_124_BANK_DIFF, bank_diff);
	SET_REG_FIELD(DENALI_CTL_124, DENALI_CTL_124_ROW_DIFF, row_diff);
	SET_REG_FIELD(DENALI_CTL_125, DENALI_CTL_125_COL_DIFF, col_diff);

	HAL_PRINT_DBG("* bank%d row%d col%d\n", bank_diff, row_diff, col_diff);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_ClearOutOfRangeInt                                                                  */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine clear out-of-range errors, which may trigger in wrap around test.         */
/*---------------------------------------------------------------------------------------------------------*/
static void MC_ClearOutOfRangeInt(void)
{
	volatile UINT32 tmp;

	// clear out of range interrupt.
	REG_WRITE(DENALI_CTL_1304, 0xFFFFFFFF); // DENALI_CTL_1304_INT_ACK_USERIF

	// read out of range values for next error.
	tmp = REG_READ(DENALI_CTL_158);
	tmp = REG_READ(DENALI_CTL_159);
	tmp = REG_READ(DENALI_CTL_160);
}

#define MC_SIZE_FAIL 0xFFFFFFFFLL
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_CheckWrapAround                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  bank_diff -                                                                            */
/*                  col_diff -                                                                             */
/*                  ddr_setup -                                                                            */
/*                  row_diff -                                                                             */
/*                                                                                                         */
/* Returns:         size of DRAM or MC_SIZE_FAIL - if there is a wraparound at 0x1000                      */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine checks for wrap around to detect size of DRAM                             */
/*---------------------------------------------------------------------------------------------------------*/
UINT64 MC_CheckWrapAround(DDR_Setup *ddr_setup, int bank_diff, int row_diff, int col_diff)
{
	UINT64 shift = 0;		// 1GB value.
	UINT64 shift_limit = 0; // 1K ==> 2GB value.
	const UINT64 MEM_CHECK_DATA = 0xFEDCBA987654321LL;
	const UINT64 MEM_CHECK_ADDR = 0x8000LL;

	volatile UINT64 address = 0;
	UINT64 orig_data = MEMR64(MEM_CHECK_ADDR);
	BOOLEAN bFoundWrap = FALSE;

	MC_ClearInterrupts();

	// Set BANK Diff before testing
	MC_SetBankRowCol(bank_diff, row_diff, col_diff);

	shift_limit = LOG(ddr_setup->max_ddr_size / _128MB_);

	HAL_PRINT_DBG("\n\n>MC: check DRAM size for configuration: BANK diff %d, ROW diff %d, COL diff %d, max_shift %d, max_size %#010lx\n",
				  bank_diff, row_diff, col_diff, shift_limit, ddr_setup->max_ddr_size);

	// Special case for DDP because the pseudo-wrap is at 4K
	address = 0x1010;
	MEMW64(0x10, MEM_CHECK_DATA); // write data
	HAL_PRINT_DBG("Check if data is mirrored at address %#010lx\n", address);

	// check if the data is mirrored at address
	if (MEMR64(address) == MEM_CHECK_DATA)
	{
		HAL_PRINT_DBG("data is mirrored at address %#010lx\n", address);
		MEMW64(0x10, ~MEM_CHECK_DATA); // write the reversed data

		// check if the reversed data is mirrored at address
		if (MEMR64(address) == ~MEM_CHECK_DATA)
		{
			HAL_PRINT_DBG("DDP Wrap around shift %u BANK %d, ROW %d, COL %d address %#010lx\n",
					  shift, bank_diff, row_diff, col_diff, address);
			MC_ClearOutOfRangeInt();
			ddr_setup->ddr_ddp = TRUE;
			return MC_SIZE_FAIL;
		}
	}

	for (shift = 0; shift < shift_limit; shift++)
	{
		if (shift < 4)
		{
			address = (_128MB_ << shift) + MEM_CHECK_ADDR;
		}
		else
		{
			// Mapping of address 2G-4G is done thorugh 4G-6G
			address = 0x100000000ULL + MEM_CHECK_ADDR;
		}

		MEMW64(MEM_CHECK_ADDR, MEM_CHECK_DATA); // write test data

		HAL_PRINT_DBG("Check if data is mirrored at address %#010lx\n", address);

		// check if the data is mirrored at address
		if (MEMR64(address) == MEM_CHECK_DATA)
		{
			MEMW64(MEM_CHECK_ADDR, ~MEM_CHECK_DATA); // write the reversed data

			// check if the reversed data is mirrored at address
			if (MEMR64(address) == ~MEM_CHECK_DATA)
			{
				HAL_PRINT_DBG("Wrap around shift %u BANK %d, ROW %d, COL %d data is mirrored at address %#010lx\n",
						  shift, bank_diff, row_diff, col_diff, address);
				bFoundWrap = TRUE;
				MC_ClearOutOfRangeInt();
				break;
			}
		}
	}

	// restore original data
	MEMW64(MEM_CHECK_ADDR, orig_data);
	MC_ClearInterrupts();
	if (bFoundWrap == TRUE)
	{
		HAL_PRINT_DBG("wrap found size is  %#010lx BANK %d, ROW %d, COL %d:\n", (_128MB_ << shift), bank_diff, row_diff, col_diff);
		return (_128MB_ << shift);
	}
	HAL_PRINT(KRED "Size is greater then %#010lx  BANK %d, ROW %d, COL %d: \n" KNRM, (_128MB_ << shift_limit), bank_diff, row_diff, col_diff);
	return (_128MB_ << shift);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_CheckDramSize                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*          measure the DRAM size                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
UINT64 MC_CheckDramSize(DDR_Setup *ddr_setup)
{
	UINT64 ddr_size;
	UINT64 ddr_size_00;
	UINT64 ddr_size_11;
	UINT64 ddr_size_01;
	UINT64 ddr_size_10;
	UINT32 gmmap = 0;

	MC_ClearInterrupts();

	HAL_PRINT_DBG("\n\n>MC: check DRAM size:\n");

	/* try all bank options*/
	ddr_size_01 = MC_CheckWrapAround(ddr_setup, 0, 1, 2);
	ddr_size_10 = MC_CheckWrapAround(ddr_setup, 1, 0, 2);
	ddr_size_00 = MC_CheckWrapAround(ddr_setup, 0, 0, 2);
	ddr_size_11 = MC_CheckWrapAround(ddr_setup, 1, 1, 2);

	HAL_PRINT_DBG(KMAG "\n\n[bank,row,size] 10 %#010lx; 01 %#010lx; 00 %#010lx; 11 %#010lx\n" KNRM, ddr_size_10, ddr_size_01, ddr_size_00, ddr_size_11);

	if (ddr_size_00 == _4GB_)
	{
		ddr_size = ddr_size_00;
		MC_SetBankRowCol(0, 0, 2);
		ddr_setup->ddr_ddp = TRUE;
	}
	else
	{
		ddr_size = MIN(ddr_size_11, ddr_size_00);

		if ((ddr_size_10 != MC_SIZE_FAIL) && (ddr_size_10 >= ddr_size))
		{
			ddr_size = MAX(ddr_size_10, ddr_size);
			MC_SetBankRowCol(1, 0, 2);
			ddr_setup->ddr_ddp = FALSE;
		}
		if ((ddr_size_01 != MC_SIZE_FAIL) && (ddr_size_01 >= ddr_size))
		{
			ddr_size = MAX(ddr_size_01, ddr_size);
			MC_SetBankRowCol(0, 1, 2);
			ddr_setup->ddr_ddp = FALSE;
		}
		if ((ddr_size_11 != MC_SIZE_FAIL) && (ddr_size_11 >= ddr_size))
		{
			ddr_size = ddr_size_11;
			MC_SetBankRowCol(1, 1, 2);
			ddr_setup->ddr_ddp = FALSE;
		}
	}

	HAL_PRINT(KGRN ">DRAM measured size is: %#010lx. max size from header %#010lx\n" KNRM,
			  ddr_size, ddr_setup->max_ddr_size);
	HAL_PRINT_DBG(KGRN ">bank diff %d. row diff %d    (%#010lx)\n" KNRM,
			  READ_REG_FIELD(DENALI_CTL_124, DENALI_CTL_124_BANK_DIFF),
			  READ_REG_FIELD(DENALI_CTL_124, DENALI_CTL_124_ROW_DIFF),
			  REG_READ(DENALI_CTL_124));

	ddr_setup->ddr_size = MIN(ddr_setup->max_ddr_size, ddr_size);

	// shift holds the correct value of the boundary update INTCR3.GMMAP
	if (ddr_setup->ddr_size == _256MB_)
	{
		gmmap = 0x0D;
	}

	if (ddr_setup->ddr_size == _512MB_)
	{
		gmmap = 0x1B;
	}

	if (ddr_setup->ddr_size == _1GB_)
	{
		gmmap = 0x37;
	}

	if (ddr_setup->ddr_size >= _2GB_)
	{
		gmmap = 0x6F;
	}

	if (ddr_setup->gmmap != 0xFF)
		gmmap = ddr_setup->gmmap;

	SET_REG_FIELD(INTCR4, INTCR4_GMMAP0, gmmap);
	SET_REG_FIELD(INTCR4, INTCR4_GMMAP1, gmmap);

	HAL_PRINT(">INTCR4.GMMAP =   %#010lx\n", gmmap);

	// return the memory size:
	return ddr_setup->ddr_size;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:           Set_DQS_in_val                                                                      */
/*                                                                                                         */
/* Parameters:        ilane - lane to set                                                                  */
/* Parameters:        input_val - to set to DQS input                                                      */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*            This routine sets in         DQS                                                             */
/*---------------------------------------------------------------------------------------------------------*/
static void Set_DQS_in_val(int ilane, UINT32 input_val, BOOLEAN bOverride, BOOLEAN bPrint)
{
	UINT32 overide = (bOverride == TRUE) ? 0x80 : 0x00;

	if (bPrint == TRUE)
		HAL_PRINT(KMAG "lane%u set in DQS to %#010lx\n" KNRM, ilane, input_val);

	REG_WRITE(PHY_LANE_SEL, (ilane * DQS_DLY_WIDTH) + 0x800);
	REG_WRITE(IP_DQ_DQS_BITWISE_TRIM, overide | (0x7F & input_val));

	REG_WRITE(PHY_LANE_SEL, 0);

	// dummy wr\rd ? IOW32(0x1000, IOR32(0x1000));
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        Set_DQS_out_val                                                                        */
/*                                                                                                         */
/* Parameters:        ilane - lane to set                                                                  */
/* Parameters:        output_val - to set to DQS ouput                                                     */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*            This routine sets out DQS                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static void Set_DQS_out_val(int ilane, UINT32 output_val, BOOLEAN bOverride, BOOLEAN bPrint)
{

	UINT32 overide = (bOverride == TRUE) ? 0x80 : 0x00;

	if (bPrint == TRUE)
		HAL_PRINT(KMAG "lane%u set out DQS to %#010lx\n" KNRM, ilane, output_val);

	REG_WRITE(PHY_LANE_SEL, (ilane * DQS_DLY_WIDTH) + 0x900);
	REG_WRITE(OP_DQ_DM_DQS_BITWISE_TRIM, overide | (0x7F & output_val));

	REG_WRITE(PHY_LANE_SEL, 0);

	// dummy wr\rd ? IOW32(0x1000, IOR32(0x1000));
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:    MC_MemStressTest                                                                           */
/*                                                                                                         */
/* Parameters:             bECC : if true, mem test is for ECC lane. Can't sweep it directly, must check ber bit*/
/* Returns:                bitwise error in UINT16 (one UINT8 for each lane)                               */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                        This routine does memory test with a list of patterns. Used for sweep.           */
/*---------------------------------------------------------------------------------------------------------*/
UINT16 MC_MemStressTest(BOOLEAN bECC, BOOLEAN bQuick)
{
	UINT16 ber = 0;

	const UINT32 aPtrn[6] = {0xFFFFFFFF, 0x00000000, 0x55555555, 0xAAAAAAAA, 0x33333333, 0x99999999};
	const UINT32 iPtrnSize = 0x100;
	const UINT iWalkers = 0x10; // NTIL - there is no point to shift 0x0001 more then 16 steps ;

	UINT32 iCnt1 = 0;
	UINT32 iCnt2 = 0;

	extern UINT32 BOOTBLOCK_IMAGE_START;
	extern UINT32 BOOTBLOCK_IMAGE_END;

	volatile UINT16 *dstAddrStart = (UINT16 *)0x1000;

	volatile UINT32 *dstAddrStartLong = (UINT32 *)0x1000;

	// dummy access to DRAM:
	IOW32(0x1000, IOR32(0x1000));
	REG_WRITE(PHY_LANE_SEL, 0);

	/*-----------------------------------------------------------------------------------------------------*/
	/*                    Fixed pattern test                                                               */
	/*-----------------------------------------------------------------------------------------------------*/
	for (iCnt1 = 0; iCnt1 < ARRAY_SIZE(aPtrn); iCnt1++)
	{
		dstAddrStart = (UINT16 *)0x1000;
		dstAddrStartLong = (UINT32 *)0x1000;
		REG_WRITE(DENALI_CTL_145, 0xFFFFFFFF);
		REG_WRITE(DENALI_CTL_145, 0);

		/*-----------------------------------------------------------------------------------------------------*/
		/*  Write pattern                                                                                      */
		/*-----------------------------------------------------------------------------------------------------*/
		// memset((void *)dstAddrStart, (UINT8)aPtrn[iCnt1], iPtrnSize);
		for (iCnt2 = 0; iCnt2 < iPtrnSize; iCnt2 += 4)
		{
			*dstAddrStartLong = (UINT32)aPtrn[iCnt1];

			// clear all interrupt status bits:
			REG_WRITE(DENALI_CTL_145, 0xFFFFFFFF);
			REG_WRITE(DENALI_CTL_145, 0);

			dstAddrStartLong++;
		}

		dstAddrStart = (UINT16 *)0x1000;
		dstAddrStartLong = (UINT32 *)0x1000;
		/*-----------------------------------------------------------------------------------------------------*/
		/* Verify pattern                                                                                      */
		/*-----------------------------------------------------------------------------------------------------*/
		for (iCnt2 = 0; iCnt2 < iPtrnSize; iCnt2 += 2)
		{
			if (bECC == TRUE)
			{
				ber |= GetECCSyndrom(*dstAddrStart);
			}
			else
			{
				ber |= *dstAddrStart ^ aPtrn[iCnt1];
			}
			dstAddrStart++;
		}

		if ((iCnt1 == 1) && (bQuick == TRUE))
		{
			return ber;
		}

		if (ber == 0xFFFF)
		{
			return ber;
		}
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/* Walking-1 pattern test                                                                              */
	/*-----------------------------------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------------------------------------------*/
	/*  Write pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart = (UINT16 *)0x1000;
	for (iCnt2 = 0; iCnt2 < iWalkers; iCnt2++)
	{
		*dstAddrStart = (0x0001 << (iCnt2));
		dstAddrStart++;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/* Verify pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart = (UINT16 *)0x1000;
	for (iCnt2 = 0; iCnt2 < iWalkers; iCnt2++)
	{
		if (bECC == TRUE)
		{
			ber |= GetECCSyndrom(*dstAddrStart);
		}
		else
		{
			ber |= *dstAddrStart ^ (0x0001 << (iCnt2));
		}
		dstAddrStart++;
	}
	if (ber == 0xFFFF)
	{
		return ber;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/* Walking-0 pattern test                                                                              */
	/*-----------------------------------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------------------------------------------*/
	/*  Write pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart = (UINT16 *)0x1000;
	for (iCnt2 = 0; iCnt2 < iWalkers; iCnt2++)
	{
		*dstAddrStart = 0xFFFF - (0x0001 << (iCnt2));
		dstAddrStart++;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/* Verify pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart = (UINT16 *)0x1000;
	for (iCnt2 = 0; iCnt2 < iWalkers; iCnt2++)
	{
		if (bECC == TRUE)
		{
			ber |= GetECCSyndrom(*dstAddrStart);
		}
		else
		{
			ber |= *dstAddrStart ^ (0xFFFF - (0x0001 << (iCnt2)));
		}
		dstAddrStart++;
	}
	if (ber == 0xFFFF)
	{
		return ber;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/*                   Walking dead pattern test                                                         */
	/*-----------------------------------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------------------------------------------*/
	/*  Write pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart = (UINT16 *)0x1000;
	for (iCnt2 = 0; iCnt2 < iWalkers; iCnt2++)
	{
		if ((iCnt2 % 2) == 0)
		{
			*dstAddrStart = 0xFFFF - (0x0001 << (iCnt2 / 2));
		}
		else
		{
			*dstAddrStart = (0x0001 << (iCnt2 / 2));
		}
		dstAddrStart++;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/* Verify pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart = (UINT16 *)0x1000;
	for (iCnt2 = 0; iCnt2 < iWalkers; iCnt2++)
	{
		if (bECC == TRUE)
		{
			ber |= GetECCSyndrom(*dstAddrStart);
		}
		else
		{
			if ((iCnt2 % 2) == 0)
			{
				ber |= *dstAddrStart ^ (0xFFFF - (0x0001 << (iCnt2 / 2)));
			}
			else
			{
				ber |= *dstAddrStart ^ (0x0001 << (iCnt2 / 2));
			}
		}
		dstAddrStart++;
	}
	if (ber == 0xFFFF)
	{
		return ber;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/*                    Walking dead1 pattern test                                                       */
	/*-----------------------------------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------------------------------------------*/
	/*  Write pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart = (UINT16 *)0x1000;
	for (iCnt2 = 0; iCnt2 < iWalkers; iCnt2++)
	{
		if ((iCnt2 % 2) == 0)
		{
			*dstAddrStart = 0xFFFF;
		}
		else
		{
			*dstAddrStart = (0x0001 << (iCnt2 / 2));
		}
		dstAddrStart++;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/* Verify pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart = (UINT16 *)0x1000;
	for (iCnt2 = 0; iCnt2 < iWalkers; iCnt2++)
	{
		if (bECC == TRUE)
		{
			ber |= GetECCSyndrom(*dstAddrStart);
		}
		else
		{
			if ((iCnt2 % 2) == 0)
			{
				ber |= *dstAddrStart ^ 0xFFFF;
			}
			else
			{
				ber |= *dstAddrStart ^ (0x0001 << (iCnt2 / 2));
			}
		}

		dstAddrStart++;
	}
	if (ber == 0xFFFF)
	{
		return ber;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/*                     Walking dead2 pattern test                                                      */
	/*-----------------------------------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------------------------------------------*/
	/*  Write pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart = (UINT16 *)0x1000;
	for (iCnt2 = 0; iCnt2 < iWalkers; iCnt2++)
	{
		if ((iCnt2 % 2) == 0)
		{
			*dstAddrStart = 0xFFFF - (0x0001 << (iCnt2 / 2));
		}
		else
		{
			*dstAddrStart = 0;
		}
		dstAddrStart++;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/* Verify pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart = (UINT16 *)0x1000;
	for (iCnt2 = 0; iCnt2 < iWalkers; iCnt2++)
	{
		if (bECC == TRUE)
		{
			ber |= GetECCSyndrom(*dstAddrStart);
		}
		else
		{
			if ((iCnt2 % 2) == 0)
			{
				ber |= *dstAddrStart ^ (0xFFFF - (0x0001 << (iCnt2 / 2)));
			}
			else
			{
				ber |= *dstAddrStart ^ 0;
			}
		}
		dstAddrStart++;
	}
	if (ber == 0xFFFF)
	{
		return ber;
	}

	// NTIL: move bootblock test to last test since this test take too much time (first it copy then it check)
	/*-----------------------------------------------------------------------------------------------------*/
	/*  Use RAM2 as a pseudo random pattern to test the DDR                                                */
	/*  Copy BB code segment to the DDR                                                                    */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart = (UINT16 *)0x1000;

	for (iCnt2 = 0; iCnt2 < 0x10000 /* (&(UINT32)BOOTBLOCK_IMAGE_END - RAM2_MEMORY_SIZE)*/; iCnt2 += sizeof(UINT16))
	{
		*dstAddrStart = *(UINT16 *)(UINT64)(RAM2_BASE_ADDR + iCnt2);
		dstAddrStart++;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/* Compare BB code segment to the DDR                                                                  */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart = (UINT16 *)0x1000;

	for (iCnt2 = 0; iCnt2 < 0x10000 /* (&(UINT32)BOOTBLOCK_IMAGE_END - RAM2_MEMORY_SIZE)  */; iCnt2 += sizeof(UINT16))
	{
		if (bECC == TRUE)
		{
			ber |= GetECCSyndrom(*(UINT16 *)(UINT64)(RAM2_BASE_ADDR + iCnt2));
		}
		else
		{
			ber |= *dstAddrStart ^ *(UINT16 *)(UINT64)(RAM2_BASE_ADDR + iCnt2);
		}

		// NTIL: save time and return as soos as posiable when there is total bits error (don't wait to verify to complete)
		if (ber == 0xFFFF)
			return ber;

		dstAddrStart++;
	}

	return ber;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FindMidBiggestEyeBitwise_l                                                             */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*          This routine finds the middle of the eye on the array received from MC_MemStressTest           */
/*---------------------------------------------------------------------------------------------------------*/

// TBD: there was a bug in center calculation when there are several eyes !!!.
static void FindMidBiggestEyeBitwise_l(const UINT16 *SweepDataBuff, const int BuffSize, volatile int *pEyeCenter, volatile int *pEyeSize, int bits)
{
	int start = -1;
	int stop = -1;
	int eyeSize = -1;

	int bufInd = 0;
	int bitInd = 0;
	int bitStep = 1; // in bitwise sweep find the biggest eye size on every bit.
	// when sweeping a value that is common to entire lane , like DQS,
	// use bit zero and bit 8 value of the UINT16 of data buf.

	for (bitInd = 0; bitInd < bits; bitInd += bitStep)
	{
		start = -1;
		stop = -1;
		eyeSize = -1;

		for (bufInd = 0; bufInd < BuffSize; bufInd++)
		{
			if ((SweepDataBuff[bufInd] & ((UINT16)0x0001 << bitInd)) == 0)
			{ // pass point
				if (start == (-1))
				{ // found a good start point
					start = bufInd;
					// HAL_PRINT_DBG("[%u",bufInd);
				}
			}
			else
			{ // fail point
				if (start != (-1))
				{ // found fail point (after found a good start point);
					// HAL_PRINT_DBG("...%u]",bufInd);
					if ((bufInd - start) > eyeSize)
					{
						// new eye size is bigger then last found eye
						stop = bufInd - 1; // last good point
						eyeSize = bufInd - start;
						// HAL_PRINT_DBG("*");
					}
					start = -1;
				}
			}
		}

		/*----------------------------------------------------------------------------------------------*/
		/*      Close an unclosed eyes                                                                  */
		/*----------------------------------------------------------------------------------------------*/
		if (start != -1)
		{
			// need to close at the end after found a good start point;
			// HAL_PRINT_DBG("Close the unclosed eyes : bit %u bufInd=%u start=%u\n",bitInd, bufInd, start );
			if (((BuffSize - 1) - start) > eyeSize)
			{
				// new eye size is bigger then last found eye
				stop = (BuffSize - 1) - 1;
				eyeSize = (BuffSize - 1) - start;
				// HAL_PRINT_DBG("*");
			}
			start = -1;
		}

		/*---------------------------------------------------------------------------------------------------------*/
		/*  write results to pEyeCenter and pEyeSize                                                               */
		/*---------------------------------------------------------------------------------------------------------*/

		if (eyeSize == (-1))
		{
			/*------------------------------------------------------------------------------------------------*/
			/* No eye                                                                                         */
			/*------------------------------------------------------------------------------------------------*/
			// HAL_PRINT(KRED ">lane%u, bit%u: No eye !\n" KNRM , ((bitInd>7)?1:0), (bitInd %8));

			// Clear pEyeCenter and pEyeSize
			pEyeCenter[bitInd] = -1;
			pEyeSize[bitInd] = 0;
		}
		else
		{
			// calculate eye center
			pEyeSize[bitInd] = eyeSize;
			pEyeCenter[bitInd] = stop - DIV_CEILING(eyeSize, 2) + 1;
		}
		// HAL_PRINT_DBG("\n>lane%u, bit%u: eye=%u center=%u\n", ((bitInd>7)?1:0), (bitInd %8),  pEyeSize[bitInd] , pEyeCenter[bitInd] );
	}

	return;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_Init_l                                                                              */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*          This routine init the memory controller                                                        */
/*---------------------------------------------------------------------------------------------------------*/
static DEFS_STATUS MC_Init_l(DDR_Setup *ddr_setup)
{
	int Registerindex = 0;
	volatile UINT32 TimeOut = 0;
	UINT32 ddr_freq = CLK_GetPll1Freq();

	HAL_PRINT_DBG("\n>MC_Init_l : Init Mem Controller.\n");
	arbel_mc_init(ddr_setup);

#if TODO // set refresh rate according to MC speed
		 // update PHY according to speed:DENALI_CTL_91_ECC_ENABLE
#ifndef DOUBLE_REFRESH_RATE
	REG_WRITE(DENALI_CTL_51, (78 * (ddr_freq / 10000000))); // 7800 / T[ns]  :
#else
	REG_WRITE(DENALI_CTL_51, (39 * (ddr_freq / 10000000))); // 7800 / T[ns]  :
#endif

	HAL_PRINT_DBG(KNRM ">Refresh rate is %#010lx (%u * (ddr/10000000)\n", IOR32(MC_BASE_ADDR + (51 * 4)), IOR32(MC_BASE_ADDR + (51 * 4)) / (ddr_freq / 10000000));
#endif
	// need delay between reg setting and start:
	CLK_Delay_MicroSec(10000);
	// Start Initialization Seq
	SET_REG_FIELD(DENALI_CTL_0, DENALI_CTL_0_START, 1);

	// wait for MC initialization complete.
	TimeOut = 60000;
	CLK_Delay_MicroSec(10);

	while (!READ_REG_FIELD(DENALI_CTL_135, DENALI_CTL_135_Init))
	{
		if (TimeOut == 0)
		{
			HAL_PRINT(KRED ">LOG_ERROR_MC_INIT_TIMEOUT\n" KNRM);
			return DEFS_STATUS_FAIL;
		}
		TimeOut--;
	}
	CLK_Delay_MicroSec(1000);

	HAL_PRINT_DBG("\n>MC_Init_l : Init Mem Controller done, DENALI_CTL_135 =  %#010lx \n", REG_READ(DENALI_CTL_135));

	return DEFS_STATUS_OK;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_IsDDRConfigured_l                                                                   */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:     TRUE:  if DDR is configured                                                                */
/*          FALSE: if DDR is not configured or need to be reinitialized                                    */
/* Side effects:    none                                                                                   */
/* Description:                                                                                            */
/*          Get the status of DDR Memory Controller initialization                                         */
/*---------------------------------------------------------------------------------------------------------*/
static BOOLEAN MC_IsDDRConfigured_l(void)
{
	// if memory controller was already initialized do not reinitialize
	// we check also DENALI_CTL_0_START since MC might be reset by WD reset while INTCR2_MC_INIT is
	// still set
	if ((1 == READ_REG_FIELD(INTCR2, INTCR2_MC_INIT) && READ_REG_FIELD(DENALI_CTL_0, DENALI_CTL_0_START)))
	{
		return TRUE;
	}

	return FALSE;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_BIST_Init_DRAM_mem                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  DataPattern -   const to write to mem                                                  */
/*                  Size -                                                                                 */
/*                  start_address -                                                                        */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine init memory regions using BIST                                            */
/*---------------------------------------------------------------------------------------------------------*/
static void MC_BIST_Init_DRAM_mem(UINT32 start_address, UINT64 size, UINT32 dataPattern)
{

	UINT32 ReadData, WriteData;

	UINT32 AddressSpace = LOG(size) + 1;

	volatile UINT32 read, err = 0;

	/* BIST only first 2GB */
	if (size > _2GB_)
		AddressSpace = LOG(_2GB_);

	if (size > _2GB_)
	{
		HAL_PRINT(KNRM "\nzero upper 2GB\n");
		for (UINT64 i = 0x100000000; i < 0x100000000 + (size - _2GB_); i += 8)
		{
			if ((i % 0x02000000) == 0)
				HAL_PRINT(".");
			*(UINT64 *)i = 0;
		}
	}

	HAL_PRINT("\nMC: BIST init start=%#010lx size=%#010lx pattern=%#010lx\n",
			  start_address, size, dataPattern);

	// Clear status register to verify no BIST interrup is pending
	SET_REG_FIELD(DENALI_CTL_148, DENALI_CTL_148_INT_ACK_BIST, 0xFF);

	// Init BIST address range - in our case - 85 is constant 0
	REG_WRITE(DENALI_CTL_84, start_address);
	REG_WRITE(DENALI_CTL_85, 0);

	SET_REG_FIELD(DENALI_CTL_83, DENALI_CTL_83_ADDR_SPACE, AddressSpace);
	SET_REG_FIELD(DENALI_CTL_83, DENALI_CTL_83_BIST_DATA_CHECK, 1);

	// set BIST_TEST_MODE to memory initialization mode
	SET_REG_FIELD(DENALI_CTL_87, DENALI_CTL_87_BIST_TEST_MODE, 0x04);

	// set BIST data pattern
	REG_WRITE(DENALI_CTL_88, dataPattern);
	REG_WRITE(DENALI_CTL_89, dataPattern);

	// BIST GO
	SET_REG_FIELD(DENALI_CTL_82, DENALI_CTL_82_BIST_GO, 1);

	// Read status register till interrupt is pending
	while (READ_REG_FIELD(DENALI_CTL_141, DENALI_CTL_141_INT_STATUS_BIST) == 0)
	{
		HAL_PRINT(".");
		CLK_Delay_MicroSec(10000);
	}
	HAL_PRINT("\n");
	HAL_PRINT_DBG("MC: BIST Completed, status = %#010lx\n\n", READ_REG_FIELD(DENALI_CTL_141, DENALI_CTL_141_INT_STATUS_BIST));

	// BIST GO clear
	SET_REG_FIELD(DENALI_CTL_82, DENALI_CTL_82_BIST_GO, 0);

	// Clear BIST interrupt
	SET_REG_FIELD(DENALI_CTL_148, DENALI_CTL_148_INT_ACK_BIST, 0xFF);

	CLK_Delay_MicroSec(30);

	// Read status register to verify interrup is not pending
	if (READ_REG_FIELD(DENALI_CTL_141, DENALI_CTL_141_INT_STATUS_BIST) != 0)
	{
		HAL_PRINT("\nFailed to clear BIST interrupt bit\n");
	}

	MC_ClearInterrupts();
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_ECC_Enable_l                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  bEn - Boolean to en\dis                                                                */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine enables\disables ECC in MC.                                               */
/*---------------------------------------------------------------------------------------------------------*/
static void MC_ECC_Enable_l(BOOLEAN bEn)
{
	HAL_PRINT("\nMC: %s ECC\n", bEn ? "enable" : "disable");

	SET_REG_FIELD(DENALI_CTL_128, DENALI_CTL_128_SWAP_EN, ~bEn);

	SET_REG_FIELD(DENALI_CTL_129, DENALI_CTL_129_DISABLE_RD_INTERLEAVE, 0); /* currently disabled due to issue */

	SET_REG_FIELD(DENALI_CTL_126, DENALI_CTL_126_ADDR_COLLISION_MPM_DIS, bEn);

	SET_REG_FIELD(DENALI_CTL_130, DENALI_CTL_130_IN_ORDER_ACCEPT, bEn);

	SET_REG_FIELD(DENALI_CTL_91, DENALI_CTL_91_ECC_ENABLE, ((bEn) ? 3 : 0));

	CLK_Delay_MicroSec(100);

	return;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_DSCL_Enable                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  bEn - Boolean to en\dis                                                                */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine enables DSCL                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void MC_DSCL_Enable(BOOLEAN bEnable)
{

	HAL_PRINT_DBG(KMAG "DSCL %sable\n" KNRM, bEnable ? "en" : "dis");

	if (bEnable)
	{
		SET_REG_BIT(DYNAMIC_WRITE_BIT_LVL /*0x1C0*/, 0);
	}
	else
	{
		CLEAR_REG_BIT(DYNAMIC_WRITE_BIT_LVL /*0x1C0*/, 0);
	}
	if (bEnable)
	{
		SET_REG_BIT(DYNAMIC_BIT_LVL /*0x1AC*/, 0);
	}
	else
	{
		CLEAR_REG_BIT(DYNAMIC_BIT_LVL /*0x1AC*/, 0);
	}

	// Program DSCL
	// 25:	dscl_save_restore_needed - not supported currently
	// 24:	dscl_en
	// 0-23: dscl_exp_cntq
	REG_WRITE(DSCL_CNT, BUILD_FIELD_VAL(DSCL_CNT_dscl_exp_cnt, 210000) |
							BUILD_FIELD_VAL(DSCL_CNT_dscl_en, bEnable) |
							BUILD_FIELD_VAL(DSCL_CNT_dscl_save_restore_needed, 0));

	// set_odt_in_dram();

	IOW32(0x1000, IOR32(0x1000));
	REG_WRITE(PHY_LANE_SEL, 0);

	CLK_Delay_MicroSec(100);

	return;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:                MC_Enable_ECC_l                                                                */
/*                                                                                                         */
/* Parameters:                none                                                                         */
/* Returns:                                                                                                */
/* Side effects:        none                                                                               */
/* Description:                                                                                            */
/*                       enalbe ECC                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
static void MC_ECC_Init_l(DDR_Setup *ddr_setup)
{
	DEFS_STATUS status;
	UINT32 enable = 0;
	
	MC_ECC_Enable_l(TRUE);

	MC_BIST_Init_DRAM_mem(0x0, ddr_setup->ddr_size, 0x14000000);

	HAL_PRINT_DBG("Status ECC (DENALY_CTL_138) = %#010lx\n", REG_READ(DENALI_CTL_138));

	for (int n = 0; n < 8; n++)
	{
		if (ddr_setup->ECC_enable && (ddr_setup->NonECC_Region_End[n] > 0))
		{
			HAL_PRINT("MC: Set none ECC region %d start = [ %#010lx : %#010lx ]\n",
					  n, ddr_setup->NonECC_Region_Start[n], ddr_setup->NonECC_Region_End[n]);

			SET_REG_FIELD(DENALI_CTL_NONE_ECC(n), DENALI_CTL_NON_ECC_REGION_START_ADDR, ddr_setup->NonECC_Region_Start[n] / _1MB_); // the field is in 1MB resolution
			SET_REG_FIELD(DENALI_CTL_NONE_ECC(n), DENALI_CTL_NON_ECC_REGION_END_ADDR, ddr_setup->NonECC_Region_End[n] / _1MB_); // the field is in 1MB resolution

			enable |= 0x1 << n;
		}
	}
	SET_REG_FIELD(DENALI_CTL_112, DENALI_CTL_112_NON_ECC_REGION_ENABLE, enable);

	MC_ClearInterrupts();

	HAL_PRINT_DBG("MC: ECC init done\n");

	return;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:                MC_manual_issue_DRAM_ZQ                                                        */
/*                                                                                                         */
/* Parameters:                none                                                                         */
/* Returns:                   none                                                                         */
/* Side effects:              none                                                                         */
/* Description:                                                                                            */
/*                        Issue DRAM ZQ calibration DDR Memory Controller                                  */
/*---------------------------------------------------------------------------------------------------------*/
void MC_manual_issue_DRAM_ZQ(void)
{
	DEFS_STATUS status;
	UINT32 temp_var = REG_READ(DENALI_CTL_124);

	HAL_PRINT_DBG("Issue DRAM Short ZQ Command (manual) via MC\n");
	while (READ_REG_FIELD(DENALI_CTL_124, DENALI_CTL_124_ZQ_REQ_PENDING) == 1)
		;												 // wait for idle and not in ZQ cycle
	SET_VAR_FIELD(temp_var, DENALI_CTL_124_ZQ_REQ, 0x1); // Short ZQ
	REG_WRITE(DENALI_CTL_124, temp_var);

	BUSY_WAIT_TIMEOUT_MC(READ_REG_FIELD(DENALI_CTL_124, DENALI_CTL_124_ZQ_REQ_PENDING) != 0, MC_TIMEOUT);

	CLK_Delay_Cycles(0x10000);

	HAL_PRINT_DBG("Issue DRAM Long ZQ Command (manual) via MC\n");
	while (READ_REG_FIELD(DENALI_CTL_124, DENALI_CTL_124_ZQ_REQ_PENDING) == 1)
		;												 // wait for idle and not in ZQ cycle
	SET_VAR_FIELD(temp_var, DENALI_CTL_124_ZQ_REQ, 0x2); // Short ZQ
	REG_WRITE(DENALI_CTL_124, temp_var);

	BUSY_WAIT_TIMEOUT_MC(READ_REG_FIELD(DENALI_CTL_124, DENALI_CTL_124_ZQ_REQ_PENDING) != 0, MC_TIMEOUT);

	HAL_PRINT_DBG("ZQ status log (DENALI_CTL_49.ZQ_STATUS_LOG) = 0x%x \n", READ_REG_FIELD(DENALI_CTL_49, DENALI_CTL_49_ZQ_STATUS_LOG));

	HAL_PRINT_DBG("ZQ timeout (DENALI_CTL_144.INT_ACK_TIMEOUT) = 0x%x \n", REG_READ(DENALI_CTL_144));

	CLK_Delay_Cycles(0x10000);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_ConfigureDDR_l                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ddr_setup - all parameters from header struct.                                         */
/*                  try -  number of retries so far                                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Set default configuration for the DDR Memory Controller                                */
/*---------------------------------------------------------------------------------------------------------*/
static DEFS_STATUS MC_ConfigureDDR_l (DDR_Setup *ddr_setup, unsigned int try)
{
	DEFS_STATUS status = DEFS_STATUS_OK;
	DEFS_STATUS status_out_sweep = DEFS_STATUS_OK;
	unsigned long itrCnt_l = 1;
	int l_BestEyeCenter[NUM_OF_LANES_MAX], l_BestEyeSize[NUM_OF_LANES_MAX];
	int ilane, ibit, TmpReg32;
	UINT32 dramSize = 0;

	NUM_OF_LANES = 2;

	CLK_ResetMC();

	MC_ClearInterrupts();

	CLK_Delay_Cycles(0x100);

	status = ddr_phy_cfg1(ddr_setup);
	if (status != DEFS_STATUS_OK)
		HAL_PRINT(KRED "\nddr_phy_cfg1\n" KNRM);

	status = MC_Init_l(ddr_setup);
	if (status != DEFS_STATUS_OK)
		HAL_PRINT(KRED "\nMC_Init_l\n" KNRM);

#if 1 // WORKAROUND for UNQ issue
	// set_odt_in_dram();
	MC_write_mr_regs_all();
	MC_manual_issue_DRAM_ZQ();
	// set_odt_in_dram(ddr_setup);
#else // set MRS one by one:

	MC_write_mr_regs_single(0, ddr_setup->MR0_DATA);
	MC_write_mr_regs_single(1, ddr_setup->MR1_DATA);
	MC_write_mr_regs_single(2, ddr_setup->MR2_DATA);
	MC_write_mr_regs_single(3, 0);
	MC_write_mr_regs_single(4, 0);
	MC_write_mr_regs_single(5, 0x400);
	MC_write_mr_regs_single(6, ddr_setup->MR6_DATA);

	MC_write_mr_regs_single(0, REG_READ(DENALI_CTL_75) | 1 << 8); // set bit 8 for DLL Reset (must be exec after DLL enable), this bit is auto clear

	MC_manual_issue_DRAM_ZQ();
	// set_odt_in_dram(ddr_setup);
#endif

	// disable refresh before doing Vref training to workaround MC Errata issue:
	SET_REG_FIELD(DENALI_CTL_43, DENALI_CTL_43_TREF_ENABLE, 0);
	CLK_Delay_MicroSec(100);

	status = ddr_phy_cfg2(ddr_setup);

	// disable refresh before doing Vref training to workaround MC Errata issue:
	SET_REG_FIELD(DENALI_CTL_43, DENALI_CTL_43_TREF_ENABLE, 1);
	// refresh Read latency based on actual clock:
	// main_clk_dly register value + C2D + (HALF_PERIOD ? 1:0)
	TmpReg32 = READ_REG_FIELD(SCL_LATENCY, SCL_LATENCY_main_clk_dly) + ddr_setup->phy_c2d;

#ifdef UNIQUIFY_PHY_HALF_PERIOD
	TmpReg32 += 1;
#endif
	SET_REG_FIELD(DENALI_CTL_1291, DENALI_CTL_1291_TDFI_PHY_RDLAT, TmpReg32 * 2);

	CLK_Delay_MicroSec(100);

	if (status != DEFS_STATUS_OK)
		HAL_PRINT(KRED "\nddr_phy_cfg2 fail\n" KNRM); // return DEFS_STATUS_FAIL;

#ifdef ENHANCED_SWEEPING_AND_LEVELING
	if (status != DEFS_STATUS_OK)
	{
		HAL_PRINT(KCYN "MC_ConfigureDDR_l Status failed \n" KNRM);
		return status;
	}

	REG_WRITE(PHY_LANE_SEL, 0);

	if (ddr_setup->dqs_in_lane0 != 0xFF)
	{
		HAL_PRINT("set override bit\n");
		Set_DQS_in_val(0, ddr_setup->dqs_in_lane0, TRUE, TRUE);
	}
	if (ddr_setup->dqs_in_lane1 != 0xFF)
		Set_DQS_in_val(1, ddr_setup->dqs_in_lane1, TRUE, TRUE);
	if (ddr_setup->dqs_out_lane0 != 0xFF)
		Set_DQS_out_val(0, ddr_setup->dqs_out_lane0, TRUE, TRUE);
	if (ddr_setup->dqs_out_lane1 != 0xFF)
		Set_DQS_out_val(1, ddr_setup->dqs_out_lane1, TRUE, TRUE);

	MC_PrintRegs();
	MC_PrintPhy();

	if (status != DEFS_STATUS_OK)
		return status;

	/*--------------------------------------------------------------------*/
	/* Main flow sweeps, accroding to header flag SWEEP_MAIN_FLOW.        */
	/*--------------------------------------------------------------------*/
	HAL_PRINT(KCYN "Main flow Sweeps 0x%x\n" KNRM, ddr_setup->sweep_main_flow);

	status_out_sweep |= Sweep_DQS_Trim_l(ddr_setup, SWEEP_OUT_LANE, TRUE, 0xFF00); // Run Parametric Sweep on WRITE side (TRIM_2.lane0) and find the center for lane0
	status_out_sweep |= Sweep_DQn_Trim_l(ddr_setup, SWEEP_OUT_DM, TRUE, 0);		   // Run Parametric Sweep on WRITE side (Output DM) and find the center for each DQn and average for each DM lane.
	status_out_sweep |= Sweep_DQn_Trim_l(ddr_setup, SWEEP_OUT_DQ, TRUE, 0);		   // Run Parametric Sweep on WRITE side (Output DQn) and find the center for each DQn
	status_out_sweep |= Sweep_DQn_Trim_l(ddr_setup, SWEEP_IN_DQ, TRUE, 0);		   // Run Parametric Sweep on READ side (Input DQn) and find the center for each DQn.
	status_out_sweep |= Sweep_DQS_Trim_l(ddr_setup, SWEEP_IN_DQS, TRUE, 0);		   // Run Parametric Sweep on READ side (Input DQS) and find the center for each DQn.
	status_out_sweep |= Sweep_DQS_Trim_l(ddr_setup, SWEEP_OUT_DQS, TRUE, 0);	   // Run Parametric Sweep on WRITE side (Output DQS) and find the center for each DQn.	

	// MC_DSCL_Enable(FALSE);
	// NTIL TBD: seems code crash when this section moved up. need to check if this is some limit memory size issue or a real DDR ECC issue
	MC_PrintRegs();
	MC_PrintPhy();

	if ((ddr_setup->mc_config & MC_CAPABILITY_SWEEP_ENABLE) || (status_out_sweep != DEFS_STATUS_OK))
	{
		HAL_PRINT(KCYN "====================  \n" KNRM);
		HAL_PRINT(KCYN "   Debug Sweeps       \n" KNRM);
		HAL_PRINT(KCYN "====================  \n" KNRM);

		TMC_StopWatchDog(0);
		TMC_StopWatchDog(1);
		TMC_StopWatchDog(2);

		// NTIL: just for debug, run two round of sweeps
		//---------------------------------
		//  on SVB and ATE there is eye size issue (apparently duo to socket power issue). found there is high correlation between TRIM_2.lane0 skew (TBD: to what ??) and lane 0 eye size.
		// just for debug info
		HAL_PRINT(KCYN "\n *** Sweep TRIM2, both lanes (for debug info) ***\n" KNRM);
		Sweep_DQS_Trim_l(ddr_setup, SWEEP_OUT_LANE, FALSE, 0); // Run Parametric Sweep on WRITE side TRIM_2 and find the center for each DQn and average for eachthe center for each TRIM_2 lane.
		// just for debug info
		HAL_PRINT(KCYN "\n *** Sweep TRIM2, only lane 1 while lane 0 is fixed to SCL value (for debug info) *** \n" KNRM);
		Sweep_DQS_Trim_l(ddr_setup, SWEEP_OUT_LANE, FALSE, 0x00FF); // Run Parametric Sweep on WRITE side TRIM_2 and find the center for each DQn and average for eachthe center for each TRIM_2 lane.
		// sweep TRIM_2.lane0 and look for the best eye size
		HAL_PRINT(KCYN "\n *** Sweep TRIM2, only lane 0 while lane 1 is fixed to SCL value, also center values *** \n" KNRM);
		Sweep_DQS_Trim_l(ddr_setup, SWEEP_OUT_LANE, TRUE, 0xFF00); // Run Parametric Sweep on WRITE side TRIM_2 and find the center for each DQn and average for eachthe center for each TRIM_2 lane.
		//---------------------------------

		HAL_PRINT(KCYN "\n *** just for debug, run another round of sweeps *** \n" KNRM);
		Sweep_DQn_Trim_l(ddr_setup, SWEEP_OUT_DM, TRUE, 0);	 // Run Parametric Sweep on WRITE side Output DM and find the center for each DQn and average for each DM lane.
		Sweep_DQn_Trim_l(ddr_setup, SWEEP_OUT_DQ, TRUE, 0);	 // Run Parametric Sweep on WRITE side (Output DQn) and find the center for each DQn
		Sweep_DQn_Trim_l(ddr_setup, SWEEP_IN_DQ, TRUE, 0);	 // Run Parametric Sweep on READ side and find the center for each DQn.
		Sweep_DQS_Trim_l(ddr_setup, SWEEP_IN_DQS, TRUE, 0);	 // Run Parametric Sweep on READ side and find the center for each DQn.
		Sweep_DQS_Trim_l(ddr_setup, SWEEP_OUT_DQS, TRUE, 0); // Run Parametric Sweep on READ side and find the center for each DQn.
	}

	Run_Memory_SI_Test(ddr_setup);

#endif

	MC_PrintRegs();
	MC_PrintPhy();
	if ((status != DEFS_STATUS_OK) || (status_out_sweep != DEFS_STATUS_OK))
	{
		HAL_PRINT(KRED "\n\nTraining failed, retry\n" KNRM);

		return DEFS_STATUS_FAIL;
	}

	return status;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:                MC_UpdateDramSize                                                              */
/*                                                                                                         */
/* Parameters:              none                                                                           */
/* Returns:                 none                                                                           */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*          Update refresh rate according to size                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void MC_UpdateDramSize(DDR_Setup *ddr_setup, UINT64 iDramSize)
{
	if (iDramSize == 0)
	{
		HAL_PRINT(KRED "\n\n>ERROR: Can't detect SDRAM size.\n>SDRAM device is faulty\n" KNRM);
		return;
	}

	if (ddr_setup->ddr_size > _2GB_)
	{
		SET_REG_FIELD(DENALI_CTL_44, DENALI_CTL_44_TRFC, 0x260); /* Default for 2GB DRAM */
	}

	else if (ddr_setup->ddr_size > _1GB_)
	{
		SET_REG_FIELD(DENALI_CTL_44, DENALI_CTL_44_TRFC, 0x260); /* Default for 2GB DRAM */
	}

	else if (ddr_setup->ddr_size > _512MB_)
	{
		SET_REG_FIELD(DENALI_CTL_44, DENALI_CTL_44_TRFC, 0x175); /* TRFC =  0x118 (280*1/800)==> 350 instead of 260 */
	}

	if (ddr_setup->ECC_enable)
	{
		ddr_setup->ddr_size = 7 * (ddr_setup->ddr_size >> 3);
	}

	REG_WRITE(SCRPAD_02, (UINT32)(MIN(ddr_setup->ddr_size, _2GB_))); /* backward compatible for older uboots */
	REG_WRITE(SCRPAD_03, (UINT32)(ddr_setup->ddr_size / _1MB_));

	// tell uboot what is the DRAM size.
	HAL_PRINT(KGRN "\nMC: DRAM active size %#010lx, ECC %s\nsize in 1MB unit is written to SCRPAD2 (%#010lx = %#010lx 1MB)\n" KNRM,
			  ddr_setup->ddr_size, ddr_setup->ECC_enable ? "enabled" : "disabled", REG_ADDR(SCRPAD_02), REG_READ(SCRPAD_02));
	HAL_PRINT_DBG(KGRN "\n>SCRPAD3 (%#010lx=%#010lx 1MB)\n" KNRM, REG_ADDR(SCRPAD_03), REG_READ(SCRPAD_03));
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_PrintOutleveling                                                                    */
/*                                                                                                         */
/* Parameters:      bIn - print input leveling   bOut - print output leveling                              */
/* Returns:     none                                                                                       */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*          This routine prints the DQ, DM delays                                                          */
/*---------------------------------------------------------------------------------------------------------*/
static void MC_PrintLeveling(BOOLEAN bIn, BOOLEAN bOut)
{
	UINT32 ilane = 0;
	UINT32 ibit = 0;

	//---------------------------------------------------------------
	if (bOut)
	{
		HAL_PRINT_DBG("\n>Data Output Delay:\n");
		for (ilane = 0; ilane < NUM_OF_LANES; ilane++)
		{
			HAL_PRINT_DBG("\t*Lane%d: ", ilane);

			REG_WRITE(PHY_LANE_SEL, ilane * DQS_DLY_WIDTH);

			HAL_PRINT_DBG("OP_DQ_DM_DQS_BITWISE_TRIM = %#010lx \n", REG_READ(OP_DQ_DM_DQS_BITWISE_TRIM));

			for (ibit = 0; ibit < 8; ibit++)
			{
				REG_WRITE(PHY_LANE_SEL, (ilane * DQS_DLY_WIDTH) + (ibit << 8));
				HAL_PRINT_DBG("DQ%d=0x%lx ", ibit, READ_REG_FIELD(OP_DQ_DM_DQS_BITWISE_TRIM, OP_DQ_DM_DQS_BITWISE_TRIM_op_dq_dm_dqs_bitwise_trim_reg));
			}

			ibit = 8;
			REG_WRITE(PHY_LANE_SEL, (ilane * DQS_DLY_WIDTH) + (ibit << 8));
			HAL_PRINT_DBG("DM=0x%lx, ", READ_REG_FIELD(OP_DQ_DM_DQS_BITWISE_TRIM, OP_DQ_DM_DQS_BITWISE_TRIM_op_dq_dm_dqs_bitwise_trim_reg));

			ibit = 9;
			REG_WRITE(PHY_LANE_SEL, (ilane * DQS_DLY_WIDTH) + (ibit << 8));
			HAL_PRINT_DBG("DQS=0x%lx, ", (UINT8)(REG_READ(OP_DQ_DM_DQS_BITWISE_TRIM) & 0x7f));

			REG_WRITE(PHY_LANE_SEL, (ilane * 6));
			if (((REG_READ(PHY_DLL_INCR_TRIM_1) >> ilane) & 0x01) == 0x01)
			{
				HAL_PRINT_DBG(" dlls_trim_1 : +0x%lx , ", (UINT8)(REG_READ(PHY_DLL_TRIM_1) & 0x3f)); // output dqs timing with respect to output dq/dm signals
			}
			else
			{
				HAL_PRINT_DBG(" dlls_trim_1 : -0x%lx , ", (UINT8)(REG_READ(PHY_DLL_TRIM_1) & 0x3f));
			}
			REG_WRITE(PHY_LANE_SEL, (ilane * 5));
			HAL_PRINT_DBG(" phase1: %d /64 cycle, ", READ_REG_FIELD(UNQ_ANALOG_DLL_3, UNQ_ANALOG_DLL_3_phase1)); // DQS delayed on the write side

			REG_WRITE(PHY_LANE_SEL, (ilane * SLV_DLY_WIDTH));
			HAL_PRINT_DBG(" dlls_trim_2: %d ", READ_REG_FIELD(PHY_DLL_TRIM_2, PHY_DLL_TRIM_2_dlls_trim_2)); //  adjust output dq/dqs/dm timing with respect to DRAM clock; This value is set by write-leveling

			REG_WRITE(PHY_LANE_SEL, (ilane * MAS_DLY_WIDTH));
			HAL_PRINT_DBG(" mas_dly: 0x%lx , ", READ_REG_FIELD(UNQ_ANALOG_DLL_2, UNQ_ANALOG_DLL_2_mas_dly));// master delay line setting for this lane

			HAL_PRINT_DBG("\n");
		}
	}
	//---------------------------------------------------------------
	if (bIn)
	{
		HAL_PRINT_DBG("\n>Data Input Delay:\n");
		for (ilane = 0; ilane < NUM_OF_LANES; ilane++)
		{
			HAL_PRINT_DBG("\t*Lane%d: ", ilane);

			REG_WRITE(PHY_LANE_SEL, (ilane * DQS_DLY_WIDTH));
			HAL_PRINT_DBG("IP_DQ_DQS_BITWISE_TRIM = %#010lx \n", REG_READ(IP_DQ_DQS_BITWISE_TRIM));

			for (ibit = 0; ibit < 8; ibit++)
			{
				REG_WRITE(PHY_LANE_SEL, (ilane * DQS_DLY_WIDTH) + (ibit << 8));
				HAL_PRINT_DBG(" DQ%d=0x%lx, ", ibit, READ_REG_FIELD(IP_DQ_DQS_BITWISE_TRIM, IP_DQ_DQS_BITWISE_TRIM_ip_dq_dqs_bitwise_trim_reg));
			}

			ibit = 8;
			REG_WRITE(PHY_LANE_SEL, (ilane * DQS_DLY_WIDTH) + (ibit << 8));
			HAL_PRINT_DBG("		DQ=0x%lx, ", READ_REG_FIELD(IP_DQ_DQS_BITWISE_TRIM, IP_DQ_DQS_BITWISE_TRIM_ip_dq_dqs_bitwise_trim_reg));

			REG_WRITE(PHY_LANE_SEL, (ilane * 6));
			if (((REG_READ(PHY_DLL_INCR_TRIM_3) >> ilane) & 0x01) == 0x01)
			{
				HAL_PRINT_DBG(" dlls_trim_3  +0x%lx, ", READ_REG_FIELD(PHY_DLL_TRIM_3, PHY_DLL_TRIM_3_dlls_trim_3));
			}
			else
			{
				HAL_PRINT_DBG(" dlls_trim_3  -0x%lx, ", READ_REG_FIELD(PHY_DLL_TRIM_3, PHY_DLL_TRIM_3_dlls_trim_3)); // input dqs timing with respect to input dq
			}

			REG_WRITE(PHY_LANE_SEL, (ilane * 5));
			HAL_PRINT_DBG(" phase2 %d /64 cycle, ", READ_REG_FIELD(UNQ_ANALOG_DLL_3, UNQ_ANALOG_DLL_3_phase2));

			REG_WRITE(PHY_LANE_SEL, (ilane * 8));
			HAL_PRINT_DBG(" data_capture_clk 0x%lx, ", READ_REG_FIELD(SCL_DCAPCLK_DLY, SCL_DCAPCLK_DLY_dcap_byte_dly));  // data_capture_clk edge used to transfer data from the input DQS clock domain to the PHY core clock domain // Automatically programmed by SCL

			REG_WRITE(PHY_LANE_SEL, (ilane * 3));
			HAL_PRINT_DBG(" main_clk_delta_dly 0x%lx, ", READ_REG_FIELD(SCL_MAIN_CLK_DELTA, SCL_MAIN_CLK_DELTA_main_clk_delta_dly));

			REG_WRITE(PHY_LANE_SEL, (ilane * 8));
			HAL_PRINT_DBG(" cycle_cnt 0x%lx ", READ_REG_FIELD(SCL_START, SCL_START_cycle_cnt)); // number of delay elements required to delay the clock signal to align with the read DQS falling edge

			HAL_PRINT_DBG("\n");
		}
	}

	IOW32(0x1000, IOR32(0x1000));
	REG_WRITE(PHY_LANE_SEL, 0);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_PrintTrim                                                                           */
/*                                                                                                         */
/* Parameters:      bIn - print input leveling   bOut - print output leveling                              */
/* Returns:     none                                                                                       */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*          This routine prints the DQ, DM delays                                                          */
/*---------------------------------------------------------------------------------------------------------*/
static void MC_PrintTrim(BOOLEAN bIn, BOOLEAN bOut)
{
	UINT32 ilane = 0;
	UINT32 ibit = 0;
	UINT32 tmp;
	int trim1[2], trim2[2], trim_clk, sqew;

	//---------------------------------------------------------------
	if (bOut)
	{
		for (ilane = 0; ilane < NUM_OF_LANES; ilane++)
		{
			HAL_PRINT_DBG("\t*Lane%d: ", ilane);

			REG_WRITE(PHY_LANE_SEL, (ilane * 6));
			trim1[ilane] = REG_READ(PHY_DLL_TRIM_1) & 0x3f;

			if (((REG_READ(PHY_DLL_INCR_TRIM_1) >> ilane) & 0x01) == 0x01)
			{
				HAL_PRINT_DBG(" dlls_trim_1 : +0x%lx , ", trim1[ilane]); // output dqs timing with respect to output dq/dm signals
			}
			else
			{
				HAL_PRINT_DBG(" dlls_trim_1 : -0x%lx , ", trim1[ilane]);
				trim1[ilane] = 0 - trim1[ilane];
			}

			REG_WRITE(PHY_LANE_SEL, (ilane * 5));
			HAL_PRINT_DBG(" phase1: %d /64 cycle, ", READ_REG_FIELD(UNQ_ANALOG_DLL_3, UNQ_ANALOG_DLL_3_phase1));

			REG_WRITE(PHY_LANE_SEL, (ilane * SLV_DLY_WIDTH));
			trim2[ilane] = READ_REG_FIELD(PHY_DLL_TRIM_2, PHY_DLL_TRIM_2_dlls_trim_2);
			HAL_PRINT_DBG(" dlls_trim_2: %d (0x%x)", trim2[ilane], trim2[ilane]);

			REG_WRITE(PHY_LANE_SEL, (ilane * MAS_DLY_WIDTH));
			tmp = READ_REG_FIELD(UNQ_ANALOG_DLL_2, UNQ_ANALOG_DLL_2_mas_dly);
			HAL_PRINT_DBG(" mas_dly: %d (0x%lx) ", tmp, tmp);

			HAL_PRINT_DBG("\n");
		}
	}
	//---------------------------------------------------------------
	if (bIn)
	{
		for (ilane = 0; ilane < NUM_OF_LANES; ilane++)
		{
			HAL_PRINT_DBG("\t*Lane%d: ", ilane);

			REG_WRITE(PHY_LANE_SEL, (ilane * 6));
			if (((REG_READ(PHY_DLL_INCR_TRIM_3) >> ilane) & 0x01) == 0x01)
			{
				HAL_PRINT_DBG(" dlls_trim_3  +0x%lx, ", READ_REG_FIELD(PHY_DLL_TRIM_3, PHY_DLL_TRIM_3_dlls_trim_3)); // (UINT8) (REG_READ(PHY_DLL_TRIM_3) & 0x3f));
			}
			else
			{
				HAL_PRINT_DBG(" dlls_trim_3  -0x%lx, ", READ_REG_FIELD(PHY_DLL_TRIM_3, PHY_DLL_TRIM_3_dlls_trim_3)); // (UINT8) (REG_READ(PHY_DLL_TRIM_3) & 0x3f)); // input dqs timing with respect to input dq
			}
			REG_WRITE(PHY_LANE_SEL, (ilane * 5));
			HAL_PRINT_DBG(" phase2 %d /64 cycle, ", READ_REG_FIELD(UNQ_ANALOG_DLL_3, UNQ_ANALOG_DLL_3_phase2)); // (UINT8) ((REG_READ(UNQ_ANALOG_DLL_3) >>8) & 0x1f));

			REG_WRITE(PHY_LANE_SEL, (ilane * 8));
			HAL_PRINT_DBG(" data_capture_clk 0x%lx, ", READ_REG_FIELD(SCL_DCAPCLK_DLY, SCL_DCAPCLK_DLY_dcap_byte_dly)); //  (UINT8) (REG_READ(SCL_DCAPCLK_DLY) & 0xff)); // data_capture_clk edge used to transfer data from the input DQS clock domain to the PHY core clock domain // Automatically programmed by SCL

			REG_WRITE(PHY_LANE_SEL, (ilane * 3));
			HAL_PRINT_DBG(" main_clk_delta_dly 0x%lx, ", READ_REG_FIELD(SCL_MAIN_CLK_DELTA, SCL_MAIN_CLK_DELTA_main_clk_delta_dly)); // (UINT8) (REG_READ(SCL_MAIN_CLK_DELTA) & 0x7));

			REG_WRITE(PHY_LANE_SEL, (ilane * 8));
			HAL_PRINT_DBG(" cycle_cnt 0x%lx ", READ_REG_FIELD(SCL_START, SCL_START_cycle_cnt)); // (UINT8)((REG_READ(SCL_START)>>20)&0xFF)); // number of delay elements required to delay the clock signal to align with the read DQS falling edge

			HAL_PRINT_DBG("\n");
		}

		REG_WRITE(PHY_LANE_SEL, 0);
		tmp = REG_READ(PHY_DLL_TRIM_CLK);
		HAL_PRINT_DBG("\t*dlls_trim_clk: \t");
		if ((tmp & 0x80) != 0)
		{
			HAL_PRINT_DBG("+"); // set to increment;
		}
		else
		{
			HAL_PRINT_DBG("-"); // decrement. (limited by the value of dlls_trim_2)
		}
		trim_clk = READ_REG_FIELD(PHY_DLL_TRIM_CLK, PHY_DLL_TRIM_CLK_dlls_trim_clk);
		HAL_PRINT_DBG("%d (0x%x)\n", trim_clk, trim_clk);

		// DQSn to CLK skew = dlls_trim_clk – (dlls_trim_2+dlls_trim_1).

		HAL_PRINT_DBG("\t*DQSn to CLK skew = dlls_trim_clk – (dlls_trim_2 + dlls_trim_1):\n");
		for (ilane = 0; ilane < NUM_OF_LANES; ilane++)
		{
			sqew = trim_clk - (trim2[ilane] + trim1[ilane]);
			HAL_PRINT_DBG("\t*lane%d %d (0x%x)\n", ilane, sqew, sqew);
		}
	}

	REG_WRITE(PHY_LANE_SEL, 0);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_PrintOhm                                                                            */
/*                                                                                                         */
/* Parameters:      val - register value (single field)                                                    */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*          This routine prints registers of ODT value in ohm                                              */
/*---------------------------------------------------------------------------------------------------------*/
static void MC_PrintOhm(UINT32 val)
{
	switch (val & 0xF)
	{
	case 0:
		HAL_PRINT_DBG("termination disabled\n");
		return;
	case PHY_DRV_60ohm:
		HAL_PRINT_DBG("60");
		break;
	case PHY_DRV_120ohm:
		HAL_PRINT_DBG("120");
		break;
	case PHY_DRV_240ohm_A:
	case PHY_DRV_240ohm_B:
		HAL_PRINT_DBG("240");
		break;
	case PHY_DRV_34ohm:
		HAL_PRINT_DBG("34");
		break;
	case PHY_DRV_80ohm:
		HAL_PRINT_DBG("80");
		break;
	case PHY_DRV_30ohm:
		HAL_PRINT_DBG("30");
		break;
	case PHY_DRV_40ohm:
		HAL_PRINT_DBG("40");
		break;
	case PHY_DRV_40ohm_B:
		HAL_PRINT_DBG("40_");
		break;
	case PHY_DRV_48ohm:
		HAL_PRINT_DBG("48");
		break;
	default:
		HAL_PRINT_DBG("Reserved ");
	}
	HAL_PRINT_DBG(" ohm\n");
	return;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_PrintPassFail                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  val -                                                                                  */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
static void MC_PrintPassFail(UINT32 val)
{
	if (val == 0)
	{
		HAL_PRINT_DBG("Passed\n");
	}
	else
	{
		HAL_PRINT_DBG(KRED "Failed\n" KNRM);
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_PrintRegs                                                                           */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*          This routine prints the module registers                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void MC_PrintRegs(void)
{
	UINT32 TmpReg32 = 0;
	UINT32 ilane = 0;

	HAL_PRINT_DBG("\n************\n");
	HAL_PRINT_DBG("* MC	 Info *\n");
	HAL_PRINT_DBG("************\n\n");

	HAL_PRINT_DBG("\n>SCL status (SCL_START_cuml_scl_rslt):\n");
	for (ilane = 0; ilane < NUM_OF_LANES; ilane++)
	{
		HAL_PRINT_DBG("\t*lane%lx: ", ilane);
		MC_PrintPassFail((READ_REG_FIELD(SCL_START, SCL_START_cuml_scl_rslt) & (1 << ilane)) == 0);
	}

	HAL_PRINT_DBG("\n>SCL status (WRLVL_CTRL_wrlvl_rslt):\n");
	for (ilane = 0; ilane < NUM_OF_LANES; ilane++)
	{
		HAL_PRINT_DBG("\t*lane%lx: ", ilane);
		MC_PrintPassFail((READ_REG_FIELD(WRLVL_CTRL, WRLVL_CTRL_wrlvl_rslt) & (1 << ilane)) == 0);
	}

	HAL_PRINT_DBG("\n>SCL status (DYNAMIC_WRITE_BIT_LVL_bit_lvl_wr_failure_status):\n");
	for (ilane = 0; ilane < NUM_OF_LANES; ilane++)
	{
		HAL_PRINT_DBG("\t*lane%lx: ", ilane);
		MC_PrintPassFail((READ_REG_FIELD(DYNAMIC_WRITE_BIT_LVL, DYNAMIC_WRITE_BIT_LVL_bit_lvl_wr_failure_status) & (1 << ilane)) == 1);
	}

	HAL_PRINT_DBG("\n>SCL status (IP_DQ_DQS_BITWISE_TRIM_bit_lvl_done_status):\n");
	for (ilane = 0; ilane < NUM_OF_LANES; ilane++)
	{
		HAL_PRINT_DBG("\t*lane%lx: ", ilane);
		MC_PrintPassFail((READ_REG_FIELD(IP_DQ_DQS_BITWISE_TRIM, IP_DQ_DQS_BITWISE_TRIM_bit_lvl_done_status) & (1 << ilane)) == 0);
	}

	HAL_PRINT_DBG("\n>SCL status (DYNAMIC_BIT_LVL_bit_lvl_failure_status):\n");
	for (ilane = 0; ilane < NUM_OF_LANES; ilane++)
	{
		HAL_PRINT_DBG("\t*lane%d: ", ilane);
		MC_PrintPassFail((READ_REG_FIELD(DYNAMIC_BIT_LVL, DYNAMIC_BIT_LVL_bit_lvl_failure_status) & (1 << ilane)) == 1);
	}

	TmpReg32 = REG_READ(PHY_DLL_ADRCTRL);
	HAL_PRINT_DBG("\n>PHY_DLL_ADRCTRL = %#010lx \n", TmpReg32);
	HAL_PRINT_DBG("\t*Number of delay elements corresponds to one clock cycle (dll_mas_dly): %d \n", READ_VAR_FIELD(TmpReg32, PHY_DLL_ADRCTRL_dll_mas_dly));
	HAL_PRINT_DBG("\t*Number of delay elements corresponds to 1/4 clock cycle (dll_slv_dly_wire): %d \n", READ_VAR_FIELD(TmpReg32, PHY_DLL_ADRCTRL_dll_slv_dly));
	/*HAL_PRINT_DBG("\tNote: each delay element is ~30 psec.\n");*/
	if ((TmpReg32 & 0x100) == 0)
	{
		HAL_PRINT_DBG("\t*dlls_trim_adrctl: ");
		if (READ_VAR_FIELD(TmpReg32, PHY_DLL_ADRCTRL_incr_dly_adrctrl))
		{
			HAL_PRINT_DBG("+"); // set to increment; limited by 1/4 clock (i.e., from dlls_trim_clk)
		}
		else
		{
			HAL_PRINT_DBG("-"); // decrement; limited by dlls_trim_2 or 1/4 clock
		}
		HAL_PRINT_DBG("%d   .(pre-set)\n", READ_VAR_FIELD(TmpReg32, PHY_DLL_ADRCTRL_dlls_trim_adrctrl));
	}
	//---------------------------------------------------------------

	HAL_PRINT_DBG("\n>PHY_DLL_RECALIB = %#010lx \n", REG_READ(PHY_DLL_RECALIB));
	HAL_PRINT_DBG("\t*dlls_trim_adrctrl_ma: ");
	if (READ_REG_FIELD(PHY_DLL_RECALIB, PHY_DLL_RECALIB_incr_dly_adrctrl_ma))
	{
		HAL_PRINT_DBG("+"); // set to increment; limited by 1/4 clock (i.e., from dlls_trim_clk)
	}
	else
	{
		HAL_PRINT_DBG("-"); // decrement; limited by dlls_trim_2 or 1/4 clock
	}
	HAL_PRINT_DBG("%d .(pre-set)\n", READ_REG_FIELD(PHY_DLL_RECALIB, PHY_DLL_RECALIB_dlls_trim_adrctrl_ma));

	//---------------------------------------------------------------
	REG_WRITE(PHY_LANE_SEL, 0);
	TmpReg32 = REG_READ(PHY_DLL_TRIM_CLK);
	HAL_PRINT_DBG("\n>PHY_DLL_TRIM_CLK  = %#010lx \n\t*dlls_trim_clk: ", TmpReg32);
	if ((TmpReg32 & 0x80) != 0)
	{
		HAL_PRINT_DBG("+"); // set to increment;
	}
	else
	{
		HAL_PRINT_DBG("-"); // decrement. (limited by the value of dlls_trim_2)
	}
	HAL_PRINT_DBG("%d .(pre-set)\n", READ_REG_FIELD(PHY_DLL_TRIM_CLK, PHY_DLL_TRIM_CLK_dlls_trim_clk));

	//---------------------------------------------------------------
	HAL_PRINT_DBG("\n>PHY_DLL_RISE_FALL = %#010lx \n\t*rise_cycle_cnt = 0x%lx\n",
				  REG_READ(PHY_DLL_RISE_FALL), READ_REG_FIELD(PHY_DLL_RISE_FALL, PHY_DLL_RISE_FALL_rise_cycle_cnt));

	//---------------------------------------------------------------
	TmpReg32 = REG_READ(PHY_PAD_CTRL_1);
	HAL_PRINT_DBG("\n>PHY_PAD_CTRL_1 = %#010lx \n", TmpReg32);
	HAL_PRINT_DBG("\t*DQ and DQS input dynamic term(ODT): ");
	MC_PrintOhm(READ_REG_FIELD(PHY_PAD_CTRL_1, PHY_PAD_CTRL_1_clk_drive_n));

	HAL_PRINT_DBG("\t*DQ, DM and DQS output drive strength: ");
	MC_PrintOhm(READ_REG_FIELD(PHY_PAD_CTRL_1, PHY_PAD_CTRL_1_dq_dqs_drive_n));

	HAL_PRINT_DBG("\t*Addr and ctrl output drive strength: ");
	MC_PrintOhm(READ_REG_FIELD(PHY_PAD_CTRL_1, PHY_PAD_CTRL_1_adrctrl_drive_n));

	HAL_PRINT_DBG("\t*Clock out drive strength: ");
	MC_PrintOhm(READ_REG_FIELD(PHY_PAD_CTRL_1, PHY_PAD_CTRL_1_clk_drive_n));

	//---------------------------------------------------------------
	TmpReg32 = REG_READ(PHY_PAD_CTRL_2);
	HAL_PRINT_DBG("\n>PHY_PAD_CTRL_2 = %#010lx \n", TmpReg32);
	HAL_PRINT_DBG("\t PHY_PAD_CTRL_2_dq_dqs_term_p: ");
	MC_PrintOhm(READ_REG_FIELD(PHY_PAD_CTRL_2, PHY_PAD_CTRL_2_dq_dqs_term_p));

	HAL_PRINT_DBG("\t*PHY_PAD_CTRL_2_dq_dqs_term_n: ");
	MC_PrintOhm(READ_REG_FIELD(PHY_PAD_CTRL_2, PHY_PAD_CTRL_2_dq_dqs_term_n));

	HAL_PRINT_DBG("\t*PHY_PAD_CTRL_2_adrctrl_term_p: ");
	MC_PrintOhm(READ_REG_FIELD(PHY_PAD_CTRL_2, PHY_PAD_CTRL_2_adrctrl_term_p));

	HAL_PRINT_DBG("\t*PHY_PAD_CTRL_2_adrctrl_term_n: ");
	MC_PrintOhm(READ_REG_FIELD(PHY_PAD_CTRL_2, PHY_PAD_CTRL_2_adrctrl_term_n));

	HAL_PRINT_DBG("\t*preamble_dly = ");
	switch (READ_REG_FIELD(PHY_PAD_CTRL, PHY_PAD_CTRL_preamble_dly))
	{
	case 0:
		HAL_PRINT_DBG("2");
		break;
	case 1:
		HAL_PRINT_DBG("1.5");
		break;
	case 2:
		HAL_PRINT_DBG("1");
		break;
	default:
		HAL_PRINT_DBG("Inv\n");
	}

	HAL_PRINT_DBG(" cycles.\n>SCL_LATENCY = %#010lx \n", REG_READ(SCL_LATENCY));
	HAL_PRINT_DBG("\t*capture_clk_dly: = 0x%lx\n", READ_REG_FIELD(SCL_LATENCY, SCL_LATENCY_capture_clk_dly));
	HAL_PRINT_DBG("\t*main_clk_dly: = 0x%lx\n", READ_REG_FIELD(SCL_LATENCY, SCL_LATENCY_main_clk_dly));
	//---------------------------------------------------------------
#ifdef PRINT_LEVELING
	MC_PrintLeveling(TRUE, TRUE);
#endif

	//---------------------------------------------------------------
	HAL_PRINT_DBG("\n>PHY VREF:\n");
	for (ilane = 0; ilane < NUM_OF_LANES; ilane++)
	{
		HAL_PRINT_DBG("\t*Lane%lx:", ilane);
		REG_WRITE(PHY_LANE_SEL, (ilane * 7));
		HAL_PRINT_DBG("%#010lx\n", READ_REG_FIELD(VREF_TRAINING, VREF_TRAINING_vref_value));
	}
	HAL_PRINT_DBG("UNIQUIFY_IO_1 %#010lx  \t", REG_READ(UNIQUIFY_IO_1));
	HAL_PRINT_DBG("UNIQUIFY_IO_2 %#010lx  \t", REG_READ(UNIQUIFY_IO_2));
	HAL_PRINT_DBG("UNIQUIFY_IO_3 %#010lx  \t", REG_READ(UNIQUIFY_IO_3));
	HAL_PRINT_DBG("UNIQUIFY_IO_4 %#010lx  \t", REG_READ(UNIQUIFY_IO_4));
	HAL_PRINT_DBG("UNIQUIFY_IO_5 %#010lx  \n", REG_READ(UNIQUIFY_IO_5));

	HAL_PRINT_DBG(KGRN "MC interrupt status registers:\n%s  = %#010lx\t", "DENALI_CTL_135", REG_READ(DENALI_CTL_135));
	HAL_PRINT_DBG("%s  = %#010lx\t", "DENALI_CTL_137", REG_READ(DENALI_CTL_137));
	HAL_PRINT_DBG("%s  = %#010lx\t", "DENALI_CTL_138", REG_READ(DENALI_CTL_138));
	HAL_PRINT_DBG("%s  = %#010lx\t", "DENALI_CTL_140", REG_READ(DENALI_CTL_140));
	HAL_PRINT_DBG("%s  = %#010lx\t", "DENALI_CTL_141", REG_READ(DENALI_CTL_141));
	HAL_PRINT_DBG("%s  = %#010lx\t", "DENALI_CTL_142", REG_READ(DENALI_CTL_142));
	HAL_PRINT_DBG("%s  = %#010lx\n", "DENALI_CTL_1304" KNRM, REG_READ(DENALI_CTL_1304));
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_PrintVersion                                                                        */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*          This routine prints the module version                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void MC_PrintVersion(void)
{
	HAL_PRINT_DBG("MC		   = %u\n", MC_MODULE_TYPE);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        GetECCSyndrom                                                                          */
/*                                                                                                         */
/* Parameters:      readVal                                                                                */
/* Returns:         status pass or fail                                                                    */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*          This routine                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
static UINT16 GetECCSyndrom(UINT32 readVal)
{
	UINT16 status = 0xFF80; // we can only test lower nibble. Rest are assumed error. status is bitwise.
	UINT32 status_correctable = 0;
	UINT32 status_uncorrectable = 0;
	UINT32 int_status = 0;
	UINT32 volatile data = 0;

	int_status = REG_READ(DENALI_CTL_145);
	// clear all interrupt status bits:
	REG_WRITE(DENALI_CTL_145, 0xFFFFFFFF);
	REG_WRITE(DENALI_CTL_145, 0);

	if ((int_status & (1 << 3)) == (1 << 3))
	{
		// HAL_PRINT_DBG("6-Mult uncorrectable detected.\n");
		return 0xFFFF;
	}

	if ((int_status & (1 << 2)) == (1 << 2))
	{
		// HAL_PRINT_DBG("5-An uncorrectable detected\t");
		return 0xFFFF;
	}

	if ((int_status & (1 << 1)) == (1 << 1))
	{
		// HAL_PRINT_DBG("4-mult correctable detected.\t");
		status = 4;
	}

	if ((int_status & (1 << 0)) == (1 << 0))
	{
		// HAL_PRINT_DBG("3-A correctable detected.\t");
		status = 3;
	}

	// no ECC error.
	if (status == 0xFF80)
		return status;

	data = REG_READ(DENALI_CTL_101);

#if 0 //  TODO
	if ( status != 0)
	{
		status_correctable =   READ_REG_FIELD(DENALI_CTL_100, DENALI_CTL_100_ECC_C_ADDR_1);  IOR32(MC_BASE_ADDR + 4*ECC_C_SYND_ADDR);
		status_uncorrectable = IOR32(MC_BASE_ADDR + 4*ECC_U_SYND_ADDR);

		if(status_correctable != 0)
			// HAL_PRINT_DBG("C synd=%#08lx add=%#08lx data=%#08lx id=%#08lx\t",
			status_correctable,
			IOR32(MC_BASE_ADDR + 4*ECC_C_ADDR_ADDR),
			IOR32(MC_BASE_ADDR + 4*ECC_C_DATA_ADDR),
			IOR32(MC_BASE_ADDR + 4*ECC_C_ID_ADDR));

		if(status_uncorrectable != 0)
			// HAL_PRINT_DBG("Un synd=%#08lx add=%#08lx data=%#08lx id=%#08lx\t",
			status_uncorrectable,
			IOR32(MC_BASE_ADDR + 4*ECC_U_ADDR_ADDR),
			IOR32(MC_BASE_ADDR + 4*ECC_U_DATA_ADDR),
			IOR32(MC_BASE_ADDR + 4*ECC_U_ID_ADDR));

	}



	/*-----------------------------------------------------------------------------------------------------*/
	/* Syndrome Incorrect Bit                                                                              */
	/*-----------------------------------------------------------------------------------------------------*/
	switch (status_correctable)
	{
	case 0x00000000: status = 0xFF80 ; break;   // No Error
	case 0x00000001: status = 0xFF81 ; break;   // Check [0]
	case 0x00000002: status = 0xFF82 ; break;   // Check [1]
	case 0x00000004: status = 0xFF84 ; break;   // Check [2]
	case 0x00000008: status = 0xFF88 ; break;   // Check [3]
	case 0x0000000b: status = 0xFFFF ; break;   // Data [31]
	case 0x0000000e: status = 0xFFFF ; break;   // Data [30]
	case 0x00000010: status = 0xFF90 ; break;   // Check [4]
	case 0x00000013: status = 0xFFFF ; break;   // Data [29]
	case 0x00000015: status = 0xFFFF ; break;   // Data [28]
	case 0x00000016: status = 0xFFFF ; break;   // Data [27]
	case 0x00000019: status = 0xFFFF ; break;   // Data [26]
	case 0x0000001a: status = 0xFFFF ; break;   // Data [25]
	case 0x0000001c: status = 0xFFFF ; break;   // Data [24]
	case 0x00000020: status = 0xFFA0 ; break;   // Check [5]
	case 0x00000023: status = 0xFFFF ; break;   // Data [23]
	case 0x00000025: status = 0xFFFF ; break;   // Data [22]
	case 0x00000026: status = 0xFFFF ; break;   // Data [21]
	case 0x00000029: status = 0xFFFF ; break;   // Data [20]
	case 0x0000002a: status = 0xFFFF ; break;   // Data [19]
	case 0x0000002c: status = 0xFFFF ; break;   // Data [18]
	case 0x00000031: status = 0xFFFF ; break;   // Data [17]
	case 0x00000034: status = 0xFFFF ; break;   // Data [16]
	case 0x00000040: status = 0xFFC0 ; break;   // Check [6]
	case 0x0000004a: status = 0xFFFF ; break;   // Data [15]
	case 0x0000004f: status = 0xFFFF ; break;   // Data [14]
	case 0x00000052: status = 0xFFFF ; break;   // Data [13]
	case 0x00000054: status = 0xFFFF ; break;   // Data [12]
	case 0x00000057: status = 0xFFFF ; break;   // Data [11]
	case 0x00000058: status = 0xFFFF ; break;   // Data [10]
	case 0x0000005b: status = 0xFFFF ; break;   // Data [9]
	case 0x0000005e: status = 0xFFFF ; break;   // Data [8]
	case 0x00000062: status = 0xFFFF ; break;   // Data [7]
	case 0x00000064: status = 0xFFFF ; break;   // Data [6]
	case 0x00000067: status = 0xFFFF ; break;   // Data [5]
	case 0x00000068: status = 0xFFFF ; break;   // Data [4]
	case 0x0000006b: status = 0xFFFF ; break;   // Data [3]
	case 0x0000006d: status = 0xFFFF ; break;   // Data [2]
	case 0x00000070: status = 0xFFFF ; break;   // Data [1]
	case 0x00000075: status = 0xFFFF ; break;   // Data [0]
	default:
		// HAL_PRINT_DBG("\nsynd unknown C=%#010lx, U=%#010lx\n", status_correctable, status_uncorrectable);
	}


	// HAL_PRINT_DBG("stat %#010lx\n", status);
#endif

	return status;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_PrintPhy                                                                            */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Print PHY regs                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
static void MC_PrintPhy(void)
{

	REG_WRITE(PHY_LANE_SEL /*0x12c*/, MAS_DLY_WIDTH * 0x00000000);
	HAL_PRINT_DBG(KNRM "Lane:                     %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("PHY_PAD_CTRL_1                 %#010lx  \n", REG_READ(PHY_PAD_CTRL_1));
	HAL_PRINT_DBG("PHY_PAD_CTRL_2                 %#010lx  \n", REG_READ(PHY_PAD_CTRL_2));
	HAL_PRINT_DBG("PHY_PAD_CTRL_3                 %#010lx  \n", REG_READ(PHY_PAD_CTRL_3));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, MAS_DLY_WIDTH * 0x1);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("PHY_PAD_CTRL_1                 %#010lx  \n", REG_READ(PHY_PAD_CTRL_1));
	HAL_PRINT_DBG("PHY_PAD_CTRL_2                 %#010lx  \n", REG_READ(PHY_PAD_CTRL_2));
	HAL_PRINT_DBG("PHY_PAD_CTRL_3                 %#010lx  \n", REG_READ(PHY_PAD_CTRL_3));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, MAS_DLY_WIDTH * 0x00000000);
	HAL_PRINT_DBG("PHY_PAD_CTRL                   %#010lx  \n", REG_READ(PHY_PAD_CTRL));
	HAL_PRINT_DBG("UNIQUIFY_IO_1                  %#010lx  \n", REG_READ(UNIQUIFY_IO_1));
	HAL_PRINT_DBG("UNIQUIFY_IO_2                  %#010lx  \n", REG_READ(UNIQUIFY_IO_2));
	HAL_PRINT_DBG("UNIQUIFY_IO_3                  %#010lx  \n", REG_READ(UNIQUIFY_IO_3));
	HAL_PRINT_DBG("UNIQUIFY_IO_4                  %#010lx  \n", REG_READ(UNIQUIFY_IO_4));
	HAL_PRINT_DBG("UNIQUIFY_IO_5                  %#010lx  \n", REG_READ(UNIQUIFY_IO_5));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, MAS_DLY_WIDTH * 0x00000000);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("SCL_START                      %#010lx  \n", REG_READ(SCL_START));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, MAS_DLY_WIDTH * 0x1);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("SCL_START                      %#010lx  \n", REG_READ(SCL_START));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, MAS_DLY_WIDTH * 0x0);
	HAL_PRINT_DBG("SCL_DATA_0                     %#010lx  \n", REG_READ(SCL_DATA_0));
	HAL_PRINT_DBG("SCL_DATA_1                     %#010lx  \n", REG_READ(SCL_DATA_1));
	HAL_PRINT_DBG("SCL_LATENCY                    %#010lx  \n", REG_READ(SCL_LATENCY));
	HAL_PRINT_DBG("SCL_RD_ADDR                    %#010lx  \n", REG_READ(SCL_RD_ADDR));
	HAL_PRINT_DBG("SCL_RD_DATA                    %#010lx  \n", REG_READ(SCL_RD_DATA));
	HAL_PRINT_DBG("SCL_CONFIG_1                   %#010lx  \n", REG_READ(SCL_CONFIG_1));
	HAL_PRINT_DBG("SCL_CONFIG_2                   %#010lx  \n", REG_READ(SCL_CONFIG_2));
	HAL_PRINT_DBG("SCL_CONFIG_3                   %#010lx  \n", REG_READ(SCL_CONFIG_3));
	HAL_PRINT_DBG("CBT_CTRL                       %#010lx  \n", REG_READ(CBT_CTRL));
	HAL_PRINT_DBG("PHY_DLL_RECALIB                %#010lx  \n", REG_READ(PHY_DLL_RECALIB));
	HAL_PRINT_DBG("PHY_DLL_ADRCTRL                %#010lx  \n", REG_READ(PHY_DLL_ADRCTRL));
	HAL_PRINT_DBG("PHY_DLL_TRIM_CLK               %#010lx  \n", REG_READ(PHY_DLL_TRIM_CLK));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, SLV_DLY_WIDTH * 0x00000000);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("PHY_DLL_TRIM_1                 %#010lx  \n", REG_READ(PHY_DLL_TRIM_1));
	HAL_PRINT_DBG("PHY_DLL_TRIM_2                 %#010lx  \n", REG_READ(PHY_DLL_TRIM_2));
	HAL_PRINT_DBG("PHY_DLL_TRIM_3                 %#010lx  \n", REG_READ(PHY_DLL_TRIM_3));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, SLV_DLY_WIDTH * 0x1);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("PHY_DLL_TRIM_1                 %#010lx  \n", REG_READ(PHY_DLL_TRIM_1));
	HAL_PRINT_DBG("PHY_DLL_TRIM_2                 %#010lx  \n", REG_READ(PHY_DLL_TRIM_2));
	HAL_PRINT_DBG("PHY_DLL_TRIM_3                 %#010lx  \n", REG_READ(PHY_DLL_TRIM_3));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, SLV_DLY_WIDTH * 0x0);
	HAL_PRINT_DBG("PHY_DLL_INCR_TRIM_1            %#010lx  \n", REG_READ(PHY_DLL_INCR_TRIM_1));
	HAL_PRINT_DBG("PHY_DLL_INCR_TRIM_3            %#010lx  \n", REG_READ(PHY_DLL_INCR_TRIM_3));
	HAL_PRINT_DBG("SCL_DCAPCLK_DLY                %#010lx  \n", REG_READ(SCL_DCAPCLK_DLY));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, 3 * 0x00000000);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("SCL_MAIN_CLK_DELTA             %#010lx  \n", REG_READ(SCL_MAIN_CLK_DELTA));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, 3 * 0x00000001);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("SCL_MAIN_CLK_DELTA             %#010lx  \n", REG_READ(SCL_MAIN_CLK_DELTA));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, 3 * 0x00000000);
	HAL_PRINT_DBG("WRLVL_CTRL                     %#010lx  \n", REG_READ(WRLVL_CTRL));
	HAL_PRINT_DBG("WRLVL_AUTOINC_TRIM             %#010lx  \n", REG_READ(WRLVL_AUTOINC_TRIM));
	HAL_PRINT_DBG("WRLVL_DYN_ODT                  %#010lx  \n", REG_READ(WRLVL_DYN_ODT));
	HAL_PRINT_DBG("WRLVL_ON_OFF                   %#010lx  \n", REG_READ(WRLVL_ON_OFF));
	HAL_PRINT_DBG("WRLVL_STEP_SIZE                %#010lx  \n", REG_READ(WRLVL_STEP_SIZE));
	HAL_PRINT_DBG("UNQ_ANALOG_DLL_1               %#010lx  \n", REG_READ(UNQ_ANALOG_DLL_1));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, MAS_DLY_WIDTH * 0x00000000);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("UNQ_ANALOG_DLL_2               %#010lx  \n", REG_READ(UNQ_ANALOG_DLL_2));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, MAS_DLY_WIDTH * 0x00000001);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("UNQ_ANALOG_DLL_2               %#010lx  \n", REG_READ(UNQ_ANALOG_DLL_2));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, MAS_DLY_WIDTH * 0x00000000);
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, 5 * 0x00000000);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("UNQ_ANALOG_DLL_3               %#010lx  \n", REG_READ(UNQ_ANALOG_DLL_3));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, 5 * 0x00000001);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("UNQ_ANALOG_DLL_3               %#010lx  \n", REG_READ(UNQ_ANALOG_DLL_3));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, 5 * 0x00000000);
	HAL_PRINT_DBG("UNIQUIFY_ADDR_CTRL_LOOPBACK_1  %#010lx  \n", REG_READ(UNIQUIFY_ADDR_CTRL_LOOPBACK_1));
	HAL_PRINT_DBG("UNIQUIFY_ADDR_CTRL_LOOPBACK_2  %#010lx  \n", REG_READ(UNIQUIFY_ADDR_CTRL_LOOPBACK_2));
	HAL_PRINT_DBG("PHY_SCL_START_ADDR             %#010lx  \n", REG_READ(PHY_SCL_START_ADDR));
	HAL_PRINT_DBG("PHY_DLL_RISE_FALL              %#010lx  \n", REG_READ(PHY_DLL_RISE_FALL));
	HAL_PRINT_DBG("DSCL_CNT                       %#010lx  \n", REG_READ(DSCL_CNT));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, (DQS_DLY_WIDTH) * 0x00000000);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("IP_DQ_DQS_BITWISE_TRIM         %#010lx  \n", REG_READ(IP_DQ_DQS_BITWISE_TRIM));
	HAL_PRINT_DBG("OP_DQ_DM_DQS_BITWISE_TRIM      %#010lx  \n", REG_READ(OP_DQ_DM_DQS_BITWISE_TRIM));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, (DQS_DLY_WIDTH) * 0x00000001);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("IP_DQ_DQS_BITWISE_TRIM         %#010lx  \n", REG_READ(IP_DQ_DQS_BITWISE_TRIM));
	HAL_PRINT_DBG("OP_DQ_DM_DQS_BITWISE_TRIM      %#010lx  \n", REG_READ(OP_DQ_DM_DQS_BITWISE_TRIM));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, (BITLVL_DLY_WIDTH + 1) * 0x00000000);
	HAL_PRINT_DBG("DYNAMIC_BIT_LVL                %#010lx  \n", REG_READ(DYNAMIC_BIT_LVL));
	HAL_PRINT_DBG("SCL_WINDOW_TRIM                %#010lx  \n", REG_READ(SCL_WINDOW_TRIM));
	HAL_PRINT_DBG("SCL_GATE_TIMING                %#010lx  \n", REG_READ(SCL_GATE_TIMING));
	HAL_PRINT_DBG("DISABLE_GATING_FOR_SCL         %#010lx  \n", REG_READ(DISABLE_GATING_FOR_SCL));
	HAL_PRINT_DBG("SCL_CONFIG_4                   %#010lx  \n", REG_READ(SCL_CONFIG_4));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, 0x00000000);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("BIT_LVL_CONFIG                 %#010lx  \n", REG_READ(BIT_LVL_CONFIG));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, 0x00000001);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("BIT_LVL_CONFIG                 %#010lx  \n", REG_READ(BIT_LVL_CONFIG));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, 0x00000000);
	HAL_PRINT_DBG("DYNAMIC_WRITE_BIT_LVL          %#010lx  \n", REG_READ(DYNAMIC_WRITE_BIT_LVL));
	HAL_PRINT_DBG("DYNAMIC_WRITE_BIT_LVL_EXT      %#010lx  \n", REG_READ(DYNAMIC_WRITE_BIT_LVL_EXT));
	HAL_PRINT_DBG("DDR4_CONFIG_1                  %#010lx  \n", REG_READ(DDR4_CONFIG_1));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, VREF_WIDTH * 0x00000000);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("VREF_TRAINING                  %#010lx  \n", REG_READ(VREF_TRAINING));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, VREF_WIDTH * 0x00000001);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("VREF_TRAINING                  %#010lx  \n", REG_READ(VREF_TRAINING));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, VREF_WIDTH * 0x00000000);
	HAL_PRINT_DBG("VREF_CA_TRAINING               %#010lx  \n", REG_READ(VREF_CA_TRAINING));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, BITLVL_DLY_WIDTH * 0x00000000);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("VREF_TRAINING_WINDOWS          %#010lx  \n", REG_READ(VREF_TRAINING_WINDOWS));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, BITLVL_DLY_WIDTH * 1);
	HAL_PRINT_DBG("Lane:                          %#010lx  \n", REG_READ(PHY_LANE_SEL));
	HAL_PRINT_DBG("VREF_TRAINING_WINDOWS          %#010lx  \n", REG_READ(VREF_TRAINING_WINDOWS));
	REG_WRITE(PHY_LANE_SEL /*0x12c*/, BITLVL_DLY_WIDTH * 0x00000000);
	HAL_PRINT_DBG("MRW_CTRL                       %#010lx  \n", REG_READ(MRW_CTRL));
	HAL_PRINT_DBG("MRW_CTRL_DDR4                  %#010lx  \n", REG_READ(MRW_CTRL_DDR4));
	HAL_PRINT_DBG("CA_TRAINING                    %#010lx  \n", REG_READ(CA_TRAINING));
	HAL_PRINT_DBG("DLL_TEST                       %#010lx  \n", REG_READ(DLL_TEST));
	HAL_PRINT_DBG("DLL_TEST_START                 %#010lx  \n", REG_READ(DLL_TEST_START));
	HAL_PRINT_DBG("DLL_TEST_RANGE                 %#010lx  \n", REG_READ(DLL_TEST_RANGE));
	HAL_PRINT_DBG("DLL_TEST_RESULT                %#010lx  \n", REG_READ(DLL_TEST_RESULT));
	HAL_PRINT_DBG("DLLM_WINDOW_SIZE               %#010lx  \n", REG_READ(DLLM_WINDOW_SIZE));
	HAL_PRINT_DBG("DC_PARAM_OUT                   %#010lx  \n", REG_READ(DC_PARAM_OUT));
	HAL_PRINT_DBG("DC_PARAM_SEL                   %#010lx  \n", REG_READ(DC_PARAM_SEL));
	HAL_PRINT_DBG("BIT_LVL_MASK                   %#010lx  \n", REG_READ(BIT_LVL_MASK));
	HAL_PRINT_DBG("DYNAMIC_IE_TIMER               %#010lx  \n", REG_READ(DYNAMIC_IE_TIMER));

	return;
}
