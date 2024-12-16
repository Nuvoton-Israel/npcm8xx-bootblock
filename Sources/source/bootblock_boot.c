/*----------------------------------------------------------------------------*/
/*  SPDX-License-Identifier: GPL-2.0                                          */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   boot.c                                                                   */
/* This file contains routines responsible to find and boot an image from flash */
/*  Project:  Arbel                                                           */
/*----------------------------------------------------------------------------*/
#define BOOT_C

#ifndef NO_LIBC
#include <string.h>
#endif


#include "hal.h"
#include "hal_regs.h"
#include "boot.h"
#include "cfg.h"
#include "mailbox.h"

#ifdef _NOTIP_
#define BOOTBLOCK_HEADER_ADDR 0x80000000
#else
#define BOOTBLOCK_HEADER_ADDR 0xFFFB0000
#endif

#define WTCR_655_MICRO_COUNTER  (BUILD_FIELD_VAL(WTCR_WTR, 1)|BUILD_FIELD_VAL(WTCR_WTRE, 1)|BUILD_FIELD_VAL(WTCR_WTE, 1)) // 0x83

/*---------------------------------------------------------------------------------------------------------*/
/* Boot module local functions                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
extern void           (*BOOTBLOCK_Init_Before_Image_Check_Vendor) (void);
extern void           (*BOOTBLOCK_Init_Before_UBOOT_Vendor) (void);
extern void           (*BOOTBLOCK_Init_GPIO_Vendor) (void);


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        BOOTBLOCK_GetBoardType                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                                                                                                         */
/* Returns:        get board type: SVB, DRB, EB, .. make sure board vendor name matches board              */
/*---------------------------------------------------------------------------------------------------------*/
BOARD_T       BOOTBLOCK_GetBoardType (void)
{
	UINT32 val_header = 0;

	BOOTBLOCK_HEADER_T *bootBlockHeader = (BOOTBLOCK_HEADER_T*)BOOTBLOCK_HEADER_ADDR;

	val_header = bootBlockHeader->header.board_type;

	return (BOARD_T)val_header;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        BOOTBLOCK_GetUartBaud                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                                                                                                         */
/* Returns:         return the BAUD rate for header                                                        */
/*---------------------------------------------------------------------------------------------------------*/
UART_BAUDRATE_T       BOOTBLOCK_GetUartBaud (void)
{
	UINT32 val_header = 0;

	BOOTBLOCK_HEADER_T *bootBlockHeader = (BOOTBLOCK_HEADER_T*)BOOTBLOCK_HEADER_ADDR;

	val_header = bootBlockHeader->header.baud;

	if (val_header == 0 || val_header == 0xFFFFFFFF)
	{
		val_header = UART_BAUDRATE_115200;
	}

	return (UART_BAUDRATE_T)val_header;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        BOOTBLOCK_Get_DDR_Setup                                                                */
/*                                                                                                         */
/* Parameters:      ddr_setup - struct of DDR parameters from header                                       */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine reads the DDR parameters from the bootblock header                        */
/*---------------------------------------------------------------------------------------------------------*/
void   BOOTBLOCK_Get_DDR_Setup (DDR_Setup *ddr_setup)
{
	UINT32 val_header = 0;
	unsigned int ilane = 0;

	BOOTBLOCK_HEADER_T *bootBlockHeader = (BOOTBLOCK_HEADER_T*)BOOTBLOCK_HEADER_ADDR;

	ddr_setup->soc_drive =     bootBlockHeader->header.soc_drive;
	ddr_setup->dram_drive =    bootBlockHeader->header.dram_drive;
	ddr_setup->soc_odt =       bootBlockHeader->header.soc_odt;
	ddr_setup->dram_odt =      bootBlockHeader->header.dram_odt;

	ddr_setup->mc_clk =        bootBlockHeader->header.mc_freq;
	ddr_setup->cpu_clk =       bootBlockHeader->header.cpu_freq;
	ddr_setup->mc_config =     bootBlockHeader->header.mc_config;
	ddr_setup->ECC_enable =    bootBlockHeader->header.mc_config & MC_CAPABILITY_ECC_ENABLE;
	ddr_setup->dram_type_clk = (bootBlockHeader->header.mc_config & MC_CAPABILITY_DRAM_CLOCK_TYPE) ? DRAM_CLK_TYPE_2133: DRAM_CLK_TYPE_1600;
	ddr_setup->print_enable =  bootBlockHeader->header.mc_config & MC_CAPABILITY_PRINT_ENABLE  ;
	ddr_setup->ddr_ddp =       (bootBlockHeader->header.mc_config & MC_CAPABILITY_DDP_DRAM) ? TRUE : FALSE;

	ddr_setup->NonECC_Region_Start[0] =    bootBlockHeader->header.NoECC_Region_0_Start;
	ddr_setup->NonECC_Region_End[0] =      bootBlockHeader->header.NoECC_Region_0_End;

	ddr_setup->NonECC_Region_Start[1] =    bootBlockHeader->header.NoECC_Region_1_Start;
	ddr_setup->NonECC_Region_End[1] =      bootBlockHeader->header.NoECC_Region_1_End;

	ddr_setup->NonECC_Region_Start[2] =    bootBlockHeader->header.NoECC_Region_2_Start;
	ddr_setup->NonECC_Region_End[2] =      bootBlockHeader->header.NoECC_Region_2_End;
	
	ddr_setup->NonECC_Region_Start[3] =    bootBlockHeader->header.NoECC_Region_3_Start;
	ddr_setup->NonECC_Region_End[3] =      bootBlockHeader->header.NoECC_Region_3_End;
	
	ddr_setup->NonECC_Region_Start[4] =    bootBlockHeader->header.NoECC_Region_4_Start;
	ddr_setup->NonECC_Region_End[4] =      bootBlockHeader->header.NoECC_Region_4_End;

	ddr_setup->NonECC_Region_Start[5] =    bootBlockHeader->header.NoECC_Region_5_Start;
	ddr_setup->NonECC_Region_End[5] =      bootBlockHeader->header.NoECC_Region_5_End;

	ddr_setup->NonECC_Region_Start[6] =    bootBlockHeader->header.NoECC_Region_6_Start;
	ddr_setup->NonECC_Region_End[6] =      bootBlockHeader->header.NoECC_Region_6_End;

	ddr_setup->NonECC_Region_Start[7] =    bootBlockHeader->header.NoECC_Region_7_Start;
	ddr_setup->NonECC_Region_End[7] =      bootBlockHeader->header.NoECC_Region_7_End;
	
	
	ddr_setup->ddr_size =                 bootBlockHeader->header.dram_max_size; // assume maximum number, until proven otherwise.
	
	if ((ddr_setup->ddr_size == 0xffffffff) || (ddr_setup->ddr_size == 0))
	{
		ddr_setup->ddr_size = _4GB_;

		if (CHIP_Get_Version() <= 0x04) 
		{
			
			ddr_setup->ddr_size = _2GB_;
		}
	}

	ddr_setup->max_ddr_size =             ddr_setup->ddr_size;

	
	ddr_setup->dqs_in_lane0 =             bootBlockHeader->header.mc_dqs_in_lane0;
	ddr_setup->dqs_in_lane1 =             bootBlockHeader->header.mc_dqs_in_lane1;
	ddr_setup->dqs_out_lane0 =            bootBlockHeader->header.mc_dqs_out_lane0;
	ddr_setup->dqs_out_lane1 =            bootBlockHeader->header.mc_dqs_out_lane1;


	ddr_setup->hdr_dlls_trim_adrctl =         bootBlockHeader->header.mc_dlls_trim_adrctl;
	ddr_setup->hdr_dlls_trim_adrctrl_ma =     bootBlockHeader->header.mc_dlls_trim_adrctrl_ma;
	ddr_setup->hdr_dlls_trim_clk =            bootBlockHeader->header.mc_dlls_trim_clk;
	ddr_setup->hdr_dlls_trim_clk_sqew =       bootBlockHeader->header.mc_dlls_trim_clk_sqew;

	for (ilane = 0; ilane < 2; ilane++)
	{
		ddr_setup->phase1[ilane]  =           bootBlockHeader->header.mc_phase1[ilane];
		ddr_setup->phase2[ilane]  =           bootBlockHeader->header.mc_phase2[ilane];
		ddr_setup->dlls_trim_1[ilane] =       bootBlockHeader->header.mc_dlls_trim_1[ilane];
		ddr_setup->dlls_trim_2[ilane] =       bootBlockHeader->header.mc_dlls_trim_2[ilane];
		ddr_setup->dlls_trim_3[ilane] =       bootBlockHeader->header.mc_dlls_trim_3[ilane];
		ddr_setup->trim2_init_offset[ilane] = bootBlockHeader->header.mc_trim2_init_offset[ilane];
		ddr_setup->vref_soc[ilane] =          bootBlockHeader->header.mc_vref_soc[ilane];
	}

	ddr_setup->vref_dram =        bootBlockHeader->header.mc_vref_dram;
	ddr_setup->sweep_debug =      bootBlockHeader->header.mc_sweep_debug;
	ddr_setup->sweep_main_flow =  bootBlockHeader->header.mc_sweep_main_flow;
	ddr_setup->gmmap =            bootBlockHeader->header.gmmap;

	ddr_setup->b_gpio_test_complete =        bootBlockHeader->header.mc_gpio_test_complete != 0xFFFF;
	ddr_setup->b_gpio_test_pass =            bootBlockHeader->header.mc_gpio_test_pass != 0xFFFF;
	
	ddr_setup->mc_gpio_test_complete =        bootBlockHeader->header.mc_gpio_test_complete & 0xFF;
	ddr_setup->mc_gpio_test_pass =            bootBlockHeader->header.mc_gpio_test_pass & 0xFF;

	// if MSb is set, invert the signals to be active low.
	ddr_setup->mc_gpio_test_complete_active_low =  ((bootBlockHeader->header.mc_gpio_test_complete & 0x8000) == 0);
	ddr_setup->mc_gpio_test_pass_active_low =      ((bootBlockHeader->header.mc_gpio_test_pass & 0x8000) == 0);

	return;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        BOOTBLOCK_Get_host_if                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                                                                                                         */
/* Returns:        get host if: espi\LPC\none from header                                                  */
/*---------------------------------------------------------------------------------------------------------*/
HOST_IF_T       BOOTBLOCK_Get_host_if (void)
{
	UINT8 val_header = 0;

	BOOTBLOCK_HEADER_T *bootBlockHeader = (BOOTBLOCK_HEADER_T*)BOOTBLOCK_HEADER_ADDR;

	val_header = bootBlockHeader->header.host_if;

	return (HOST_IF_T)val_header;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        BOOTBLOCK_GetVendorType                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                                                                                                         */
/* Returns:        get board vendor type: customer, Nuvoton                                                */
/*---------------------------------------------------------------------------------------------------------*/
BOARD_VENDOR_T       BOOTBLOCK_GetVendorType (void)
{

	UINT32 val_header = 0;

	BOOTBLOCK_HEADER_T *bootBlockHeader = (BOOTBLOCK_HEADER_T*)BOOTBLOCK_HEADER_ADDR;

	val_header = bootBlockHeader->header.vendor;

	return (BOARD_VENDOR_T)val_header;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        BOOTBLOCK_Get_MC_freq                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                                                                                                         */
/* Returns:        get MC freq (in MHZ) from header.                                                       */
/*                 example: if uesr wants 800MHz, write 0x320 on the header at offset 0x10C                */
/*---------------------------------------------------------------------------------------------------------*/
UINT32   BOOTBLOCK_Get_MC_freq (void)
{
	UINT16 val_header = 0;

	BOOTBLOCK_HEADER_T *bootBlockHeader = (BOOTBLOCK_HEADER_T*)BOOTBLOCK_HEADER_ADDR;

	val_header = (UINT32)bootBlockHeader->header.mc_freq;

	return val_header;

}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        BOOTBLOCK_Get_CPU_freq                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                                                                                                         */
/* Returns:        get CPU freq (in MHZ) from header.                                                      */
/*                 example: if uesr wants 800MHz, write 0x320 on the header at offset 0x10E                */
/*---------------------------------------------------------------------------------------------------------*/
UINT32   BOOTBLOCK_Get_CPU_freq (void)
{
	UINT16 val_header = 0;

	BOOTBLOCK_HEADER_T *bootBlockHeader = (BOOTBLOCK_HEADER_T*)BOOTBLOCK_HEADER_ADDR;

	val_header = (UINT32)bootBlockHeader->header.cpu_freq;

	return val_header;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        BOOTBLOCK_Get_FIU_DRD_CFG                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                                                                                                         */
/* Returns:        get FIU_DRD_CFG_Set from header. FIU values can be 0, 1, 2                              */ 
/*---------------------------------------------------------------------------------------------------------*/
UINT32   BOOTBLOCK_Get_FIU_DRD_CFG (UINT fiu)
{
	UINT32 val_header = 0;

	if (fiu > 3)
	{
		ASSERT (0);
		return 0xFFFFFFFF;
	}
	if (fiu == 3) 
	{
		fiu = 2;
	}

	BOOTBLOCK_HEADER_T *bootBlockHeader = (BOOTBLOCK_HEADER_T*)BOOTBLOCK_HEADER_ADDR;

	val_header = (UINT32)bootBlockHeader->header.fiu_cfg_drd_set[fiu];

	return val_header;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        BOOTBLOCK_Get_SPI_clk_divider                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                                                                                                         */
/* Returns:        get SPI clk divider from header.                                                        */
/*---------------------------------------------------------------------------------------------------------*/
UINT8   BOOTBLOCK_Get_SPI_clk_divider (UINT spi)
{
	UINT8 val_header = 0;

	BOOTBLOCK_HEADER_T *bootBlockHeader = (BOOTBLOCK_HEADER_T*)BOOTBLOCK_HEADER_ADDR;

	if (spi == 0)
		val_header = bootBlockHeader->header.fiu0_divider;
	else if (spi == 1)
		val_header = bootBlockHeader->header.fiu1_divider;
	else
		val_header = bootBlockHeader->header.fiu3_divider;
	
	return val_header;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        BOOTBLOCK_Get_i3c_RC_clk_divider                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                                                                                                         */
/* Returns:        get I3C and RC divder (shared) clk divider from header.                                 */
/*---------------------------------------------------------------------------------------------------------*/
UINT8   BOOTBLOCK_Get_i3c_RC_clk_divider (void)
{
	UINT8 val_header = 0;

	BOOTBLOCK_HEADER_T *bootBlockHeader = (BOOTBLOCK_HEADER_T*)BOOTBLOCK_HEADER_ADDR;

	val_header = bootBlockHeader->header.i3c_rc_divider;

	return val_header;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        BOOTBLOCK_Get_pll0_override                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                                                                                                         */
/* Returns:        override PLL0. This is only possible if MC==CPU freq.                                   */
/*---------------------------------------------------------------------------------------------------------*/
UINT32   BOOTBLOCK_Get_pll0_override (void)
{
	UINT32 val_header = 0;

	BOOTBLOCK_HEADER_T *bootBlockHeader = (BOOTBLOCK_HEADER_T*)BOOTBLOCK_HEADER_ADDR;

	val_header = bootBlockHeader->header.pll0_override;

	if (val_header == 0xFFFFFFFF)
	{
		val_header = 0;
	}

	return val_header;
}

#undef BOOT_C

