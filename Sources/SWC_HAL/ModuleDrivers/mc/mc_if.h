/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   mc_if.h                                                                  */
/*            This file contains MC module interface                          */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/
#ifndef _MC_IF_H
#define _MC_IF_H

#include __CHIP_H_FROM_IF()

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                                 INCLUDES                                                */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#define UNIQUIFY_PHY_HALF_PERIOD      1


typedef struct MC_INIT_VALUES_tag
{
	UINT32 DQS_input;
	UINT32 DQS_output;
	UINT8  DQS_start_enh_sweep;       // 0x16C
	UINT8  DQS_end_enh_sweep;         // 0x16D

} MC_INIT_VALUES;


typedef enum ENUM_MR6_TRAINING_tag
{
	MR6_ENABLE_TRAINING_OFF,
	MR6_ENABLE_TRAINING_1,
	MR6_ENABLE_TRAINING_ON,

} ENUM_MR6_TRAINING;

typedef enum ENUM_BIT_LEVEL_SAMPLE_TYPE_tag
{
	BIT_LEVEL_SAMPLE_NEGATIVE_ONLY = 0,
	BIT_LEVEL_SAMPLE_POSITIVE_ONLY = 1,
	BIT_LEVEL_SAMPLE_BOTH = 3,

} ENUM_BIT_LEVEL_SAMPLE_TYPE;

//Keep the order of the enums according to frequency
typedef enum ENUM_DRAM_CLK_TYPE_tag
{
	DRAM_CLK_TYPE_1600 = 0,
	DRAM_CLK_TYPE_2133 = 1,
} ENUM_DRAM_CLK_TYPE;

/*---------------------------------------------------------------------------------------------------------*/
/* DDR setup                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct DDR_Setup_tag
{
	UINT32   cpu_clk;
	UINT32   mc_clk;
	ENUM_DRAM_CLK_TYPE  dram_type_clk;
	int      soc_drive;
	int      dram_odt;
	int      dram_drive;
	int      soc_odt ;
	UINT8    mc_config;
	UINT32   MR2_DATA;
	UINT32   MR6_DATA;
	UINT32   MR1_DATA;
	UINT32	 MR0_DATA;
	UINT64   ddr_size;
	UINT64   max_ddr_size;
	BOOLEAN  ddr_ddp;
	UINT8    ECC_enable;
	UINT32   NonECC_Region_Start[8];  // in 1M alignment
	UINT32   NonECC_Region_End[8];    // in 1M alignment
	
	int      SaveDRAMVref;
	ENUM_MR6_TRAINING mr6_training_state;
	int      dll_recalib_trim_adrctrl_ma;
	BOOLEAN  dll_recalib_trim_increment_ma;
	int      dlls_trim_adrctrl;
	BOOLEAN  dlls_trim_adrctrl_incr;
	int      dlls_trim_clk;
	BOOLEAN  dlls_trim_clk_incr;
	UINT8    phy_c2d;
	UINT8    phy_additive_latency;
	UINT8    cas_latency;
	UINT8    cas_write_latency;
	UINT8    read_latency_adjust;
	UINT8    write_latency_adjust;
	UINT8    phy_tRTP;
	UINT8    phy_tCCD_L;
	UINT8    phy_cal_npu_offset;
	UINT8    phy_incr_cal_npu;
	UINT8    phy_cal_ppu_offset;
	UINT8    phy_incr_cal_ppu;
	UINT8    phy_cal_npd_offset;
	UINT8    phy_incr_cal_npd;
	UINT8    phy_cal_ppu_op8_offset;
	UINT8    phy_incr_cal_ppu_op8;
	UINT8    phy_cal_npd_offset_op8;
	UINT8    phy_incr_cal_npd_op8;

	UINT8    dqs_in_lane0;
	UINT8    dqs_in_lane1;
	UINT8    dqs_out_lane0;
	UINT8    dqs_out_lane1;

	UINT8    hdr_dlls_trim_adrctl;
	UINT8    hdr_dlls_trim_adrctrl_ma;
	UINT8    hdr_dlls_trim_clk;
	UINT8    hdr_dlls_trim_clk_sqew;

	UINT8    phase1[2];
	UINT8    phase2[2];
	UINT8    dlls_trim_1[2];
	UINT8    dlls_trim_2[2];
	UINT8    dlls_trim_3[2];
	UINT8    trim2_init_offset[2];

	UINT8    vref_soc[2];
	UINT8    vref_dram;
	UINT8    gmmap;

	UINT8    sweep_debug;   // enable running charectarization sweeps. bitwise.
	UINT8    sweep_main_flow; // same, but for none debug
	UINT8    print_enable;

	UINT16   mc_gpio_test_pass;
	UINT16   mc_gpio_test_complete;
	
	BOOLEAN     mc_gpio_test_pass_active_low;
	BOOLEAN     mc_gpio_test_complete_active_low;

	BOOLEAN     b_gpio_test_pass;
	BOOLEAN     b_gpio_test_complete;

} DDR_Setup;

/*---------------------------------------------------------------------------------------------------------*/
/* Priority type                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum
{
    MC_PRIO_HIGH,
    MC_PRIO_MIDDLE,
    MC_PRIO_LOW,
} MC_PRIO_T;

/*-------------------------------------------------------------------------------------*/
/* Bit Fields to be used by mc_config  (Values returned from BOOTBLOCK_Get_MC_config)  */
/*-------------------------------------------------------------------------------------*/
#define MC_CAPABILITY_ECC_ENABLE                        MASK_BIT(0)
// #define MC_CAPABILITY_ENAHANCED_SWEEP                MASK_BIT(1)
#define MC_CAPABILITY_DRAM_CLOCK_TYPE                   MASK_BIT(2)
#define MC_CAPABILITY_3_SEC_DELAY                       MASK_BIT(3)
#define MC_CAPABILITY_SWEEP_ENABLE                      MASK_BIT(4)
#define MC_CAPABILITY_PRINT_ENABLE                      MASK_BIT(5)
#define MC_CAPABILITY_DDP_DRAM                          MASK_BIT(6)


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_ConfigureDDR                                                                        */
/*                                                                                                         */
/* Parameters:      UINT8 mc_config : value from  BOOTBLOCK_Get_MC_config (BB header)                      */
/* Returns:         status                                                                                 */
/* Side effects:    none                                                                                   */
/* Description:                                                                                            */
/*                  Set default configuration for the DDR Memory Controller                                */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS MC_ConfigureDDR (DDR_Setup *ddr_setup);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_Init_DDR_Setup                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ddr_setup -                                                                            */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS MC_Init_DDR_Setup (DDR_Setup *ddr_setup);


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_Init_DDR_Setup                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ddr_setup -  MC training context                                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
 /*                  This routine performs recalc the MC training context (ddr_setup)                      */
 /*--------------------------------------------------------------------------------------------------------*/
DEFS_STATUS MC_Init_DDR_Setup_re_calc (DDR_Setup *ddr_setup);


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_SetPortPriority                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  portNum     - Number of the Memory port to set the priority to                         */
/*                  fixedPrio   - Indicates if to set fixed priority or auto priority                      */
/*                  priority    - In fixed mode, setting fixed priority                                    */
/*                                In auto mode, setting initial priority                                   */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine priority configuration for Memory controller ports                        */
/*---------------------------------------------------------------------------------------------------------*/
void MC_SetPortPriority (UINT portNum, BOOLEAN fixedPrio, MC_PRIO_T priority);


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_ReconfigureODT                                                                      */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Transfers the PCTL to CFG state, writes to ODTCFG register and                         */
/*                  returns to ACTIVE state.                                                               */
/*                  Assumes: PCTL has already been configured.                                             */
/*                  This function serves the Booter to bypass Z1 ROM configuration issue.                  */
/*---------------------------------------------------------------------------------------------------------*/
void MC_ReconfigureODT (void);


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_EnableCoherency                                                                     */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Enables the Coherency Reorder feature in Memory Controller                             */
/*---------------------------------------------------------------------------------------------------------*/
void MC_EnableCoherency (void);


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_UpdateDramSize                                                                      */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Update refresh rate according to size                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void MC_UpdateDramSize (DDR_Setup *ddr_setup, UINT64 iDramSize);


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_CheckDramSize                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  measure the DRAM size                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
UINT64 MC_CheckDramSize (DDR_Setup *ddr_setup);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_MemStressTest                                                                        */
/*                                                                                                         */
/* Parameters:      bECC : if true, mem test is for ECC lane. Can't sweep it directly, must check ber bit  */
/* Returns:         bitwise error in UINT16 (one UINT8 for each lane)                                      */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine does memory test with a list of patterns. Used for sweep.                 */
/*---------------------------------------------------------------------------------------------------------*/
UINT16 MC_MemStressTest (BOOLEAN bECC, BOOLEAN bQuick );


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_Init_Denali                                                                         */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*          This routine init the memory controller                                                        */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS MC_Init_Denali (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_ClearInterrupts                                                                     */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine clear all MC interrupts. Called from error handler too.                   */
/*---------------------------------------------------------------------------------------------------------*/
void MC_ClearInterrupts (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_PrintRegs                                                                           */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module registers                                               */
/*---------------------------------------------------------------------------------------------------------*/
void MC_PrintRegs (void);


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_PrintVersion                                                                        */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module version                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void MC_PrintVersion (void);


#endif //_MC_IF_H

