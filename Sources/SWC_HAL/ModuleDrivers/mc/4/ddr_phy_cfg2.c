
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define MAX_VREF_VAL 126
volatile UINT8 bucket_save_diff_lane[MEM_STRB_WIDTH][MAX_VREF_VAL] __attribute__((aligned(16)));

#define BIT_LVL_SAMPLE_QTY 2
#define RUN_WRITE_LEVELING			    1
#define RUN_BIT_LEVELING			    1
#define BIT_LVL_WR_SIDE			       1
#define BIT_LVL_WR_DYNAMIC            0
#define BIT_LVL_DYNAMIC               0

#define SCL_START_ADDR_0			      (0x8<<16)
#define SCL_START_ADDR_1			      (0x0<<16)
#define SCL_START_ROW_ADDR			    (0x0<<0)   /* Bits 0-15  */
#define SCL_START_COL_ADDR			    (0x0<<16)  /* Bits 16-28 */
#define SCL_START_BANK_ADDR			   (0x0<<29)  /* Bits 29-31 */

#define PHY_HALF_CLK_400			      2.5    /* 800MHz/2 is Half clock rate in ns */
#define PHY_HALF_CLK_533			      1.87   /* 1066MHz/2 is Half clock rate in ns */
#define PHY_AUTO_SCL			          0      /* 0: DSCL Disable 1: DSCL Enable */
#ifdef PHY_SIMULATION_MODE
	#define DSCL_INTERVAL		         21     /* ~10us = 21*256*PHY_HALF_CLK_533 */
#else
	#define DSCL_INTERVAL		         210000 /* ~100ms = 210000*256*PHY_HALF_CLK_533 */
#endif

//#define DRAM_CLK_PERIOD               1/800000000 /* 800MHz: 1.25ns 10066MHz: 0.9380863ns */
//#define DRAM_CLK_PERIOD               1//0.93f /* 800MHz: 1.25ns 10066MHz: 0.9380863ns */
#define SCL_LANES			             0x3

#define  NONE_LP4_PHY 0xFFFFF

#define BUSY_WAIT_TIMEOUT_MC(busy_cond, timeout)	            \
{			                                            \
	 UINT32 __time = timeout;		                    \
	 CLK_Delay_MicroSec(1000);                                  \
			                                            \
	 do		                                            \
	 {		                                            \
			if (__time-- == 0)                          \
			{                                           \
			    status =  DEFS_STATUS_RESPONSE_TIMEOUT; \
			    HAL_PRINT("\ttimeout\n");           \
			    break;                                  \
			}                                           \
	 } while (busy_cond);		                            \
	 CLK_Delay_MicroSec(10000);                                 \
}


#define MC_TIMEOUT 1000


#define DQ_DQS_RATIO 8

#define DQ_DM_RATIO 1

#define MEM_DATA_WIDTH 16
#define MEM_STRB_WIDTH 2

#define MEM_MASK_WIDTH 2

#define MEM_BANK_BITS 3

#define MEM_CHIP_SELECTS 1

#define MEM_BANK_GROUP_BITS 2
#define MEM_CHIP_ID_BITS 4
#define MEM_ADDR_BITS 16

#define VREF_WIDTH 7

#define MEM_ROW_BITS 17
#define MEM_CLOCKS 1
#define MEM_CLOCK_ENABLES 1
#define NUM_UVREF_IO 3
#define UNIQUIFY_PHY_HALF_RATE

#define UNIQUIFY_NO_PROG_HALF_RATE
#define UNIQUIFY_PHY_INTEGRATED_IOS
#define UNIQUIFY_CLK_GATING_ENABLE

#define LPDDR2_DFI_ADDR_BITS 20
#define DDR4_DFI_ADDR_BITS 17

#define UNIQUIFY_ODT_IO

#define UNIQUIFY_NUM_STAGES_C2D 1
#define UNIQUIFY_NUM_STAGES_A2D 1

#define RD_DATA_EN_STAGES 0

#define AHB_REG_DATA_WIDTH 32
#define UNIQUIFY_IO
#define UNIQUIFY_LPDDR4_IO
#define UNIQUIFY_DYNAMIC_IE_SUPPORT
#define UNIQUIFY_DYN_IO_TRIM_UPDATE

#define UNQ_PHY_STAGES 4
#define UNIQUIFY_TEST
#define UNIQUIFY_FLOP_ASSIGN 0.2ns

#define UNIQUIFY_RETENTION_SUPPORT
#define UNIQUIFY_ADDR_CTRL_LOOPBACK

#define UNIQUIFY_ADDR_CTRL_RESULTS  MEM_ADDR_BITS + MEM_BANK_BITS + 4 +    MEM_CHIP_SELECTS +  MEM_CHIP_SELECTS + MEM_CLOCK_ENABLES

#define UNIQUIFY_DLL_TEST

#define NUM_FF_DIVIDER 12
#define DLL_SIGNAL_WIDTH_OP_CTRLS ( MEM_CHIP_SELECTS +  MEM_CHIP_SELECTS +  MEM_CLOCK_ENABLES)

#define DLL_SIGNAL_WIDTH_OP_MA    (MEM_BANK_BITS + 1 + 1 + 1 + MEM_ADDR_BITS)

#define DLL_SIGNAL_WIDTH_APHY     (1 + 2*DLL_SIGNAL_WIDTH_OP_CTRLS + 2*DLL_SIGNAL_WIDTH_OP_MA + 2*MEM_CLOCKS)
#define DLL_SIGNAL_WIDTH_WR_MACRO (DQ_DQS_RATIO + DQ_DQS_RATIO + 3 + 3 + 1 + 1)
#define DLL_SIGNAL_WIDTH_RD_MACRO (DQ_DQS_RATIO)
#define DLL_SIGNAL_WIDTH_DPHY     (4 + 3 + DLL_SIGNAL_WIDTH_WR_MACRO + DLL_SIGNAL_WIDTH_RD_MACRO)
#define DLL_SIGNAL_WIDTH          (DLL_SIGNAL_WIDTH_DPHY + DLL_SIGNAL_WIDTH_APHY)

#define PHY_REVISION_NUMBER  0x010812

#define MIN_MAIN_CLK_DLY  0x5

#define UNIQUIFY_NDLL

#define MAS_DLY_WIDTH 8
#define SLV_DLY_WIDTH 6
#define DQS_DLY_WIDTH 7
#define BITLVL_DLY_WIDTH 6
#define BIT_LVL_SAMPLE_QTY 2



#define ADLL_MAS_DLY_WIDTH MAS_DLY_WIDTH
#define ADLL_SLV_DLY_WIDTH SLV_DLY_WIDTH

#define ADLL_STEP_DLY_MULT 2

#define UNIQUIFY_DLY_CHAIN_STEP 0.010
#define DATA_CAP_DLY_WIDTH MAS_DLY_WIDTH





//NTIL - started with 4!
//1600
//#define DDR4_tCCD_L                   5 //JL TODO
//2133
#define SWEEP_RANGER_PRECENTAGE 0.15f

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        print_full_window_diffs                                                                */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void print_full_window_diffs(void)
{
	int lane = 0;
	int c, i;
	for (lane = 0; lane < MEM_STRB_WIDTH; lane = lane + 1) {
		HAL_PRINT_DBG("### Lane %d:\t", lane);
		c = 0;
		for (i = 0; i < MAX_VREF_VAL; i++)
		{
			if (bucket_save_diff_lane[lane][i] !=0xAA)
			{
				if (c == 0)
					HAL_PRINT_DBG("Start At %d:\n  0x%X\t", i, bucket_save_diff_lane[lane][i]);
				c++;
				if (c % 10 == 0)
				{
					HAL_PRINT_DBG("\n");
				}
			}
			else
			{
				if (c != 0)
				{
					HAL_PRINT_DBG("End At %d",i);
					break;
				}
			}


		}
		HAL_PRINT_DBG("\n");

	}
	HAL_PRINT_DBG("\n");
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        LogLevelingData                                                                        */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints leveling results                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void LogLevelingData(void)
{
	int lane;
	for (lane = 0; lane < MEM_STRB_WIDTH; lane = lane + 1) {
		UINT32 reg_read_val = 0;
		REG_WRITE(PHY_LANE_SEL, SLV_DLY_WIDTH * lane);
		reg_read_val = REG_READ(PHY_DLL_TRIM_2);
		HAL_PRINT_DBG("lane %d Leveling output is 0x%x \n", lane, reg_read_val);

	}
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        setup_vref_training_registers                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ddr_setup -                                                                            */
/*                  turn_on_off_training -                                                                 */
/*                  vref_value -                                                                           */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine sets current VREF                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void setup_vref_training_registers (
	DDR_Setup *ddr_setup,
	UINT32 vref_value,		          /* VREF value to be programmed in DRAM */
	UINT32 turn_on_off_training	/* Turn ON (if 1), Turn OFF (if 2) VREF training mode in DRAM */
) {

	DEFS_STATUS status;
	volatile UINT32 vref_op_code;
	volatile UINT32 pval, pval2;



	// The DRAM has 51 VREF trim values for range 0 and same for range 1 but the ranges partially
	// overlap. So we use Range 1 fully for VREF trim settings from 0 to 50 and then when we switch
	// to range 2 for values above 50, we need to subtract 23 to get the VREF setting in range 0 that
	// is closest to 1 step higher than the last VREF trim setting we used from range 1
	// So removing the portions of the ranges that overlap, there are 74 trim values from 0 to 73.
	if ( vref_value > 22 ) {
		vref_op_code = vref_value - 23;
	} else {
		vref_op_code = vref_value | (1 << 6);
	}




	if (turn_on_off_training == 2)
	{
		ddr_setup->mr6_training_state = MR6_ENABLE_TRAINING_OFF;
	}
	else
	{
		ddr_setup->mr6_training_state = MR6_ENABLE_TRAINING_ON;
	}

	// Issue MRW cmomand to MR6 to setup VREF training value in the DRAM
	setup_registers_MRS(6, vref_op_code, ddr_setup);

} /* of setup_vref_training_registers */


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        setup_vref_training_registers_                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ddr_setup -                                                                            */
/*                  vref_value -                                                                           */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void setup_vref_training_registers_(DDR_Setup *ddr_setup, UINT32 vref_value)
{
	//UINT32 reg_read_val;
	UINT32 turn_on_off_training = 0;
	setup_vref_training_registers(ddr_setup, vref_value, turn_on_off_training);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        setup_registers_MRS                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  data - data to write to MRS register.                                                  */
/*                  index  - MRS register number (0 to 6)                                                  */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs MRS setting using PHY module (workaround for issue)              */
/*---------------------------------------------------------------------------------------------------------*/
static void setup_registers_MRS (
	UINT32 index,
	UINT32 data,
	DDR_Setup *ddr_setup )
{

	DEFS_STATUS status;
	UINT32 control_word = 0;
	UINT32 data_word = data;
	UINT32 DDR4_tCCD_L = 0;
	UINT32  pval = 0;

	switch (index)
	{
	case 6:
		control_word = 0xA;

		// Use the data in MR6_DATA however do not override the vref given
		data_word |= (ddr_setup->MR6_DATA &  0xFFC0);
		if (ddr_setup->mr6_training_state != MR6_ENABLE_TRAINING_OFF)
		{
			data_word |= (1 << 7);   // If in any of the training modes, set bit 7
		}

		break;
	case 5:
		control_word = 0x9;
		break;
	case 4:
		control_word = 0x8;
		break;
	case 0:
	case 1:
	case 2:
	case 3:
		control_word = index;
		break;
	default:
		HAL_PRINT_DBG("Unsupported index: %d\n", index);
		break;
	}

	// HAL_PRINT_DBG("Set MR%d (via PHY): control word 0x%lX, data 0x%lX Data to MRW_CTRL_DDR4 0x%lX\n", index, control_word, data,data_word);

	REG_WRITE(MRW_CTRL_DDR4, data_word);

	// pval = (UINT32)ceilf(150000 / (2 * dram_type_clk_PERIOD));     /* mrw_wait <= tVREF_TIME = 150ns */   //JL TODO: ceilf()
	pval = 0x1FF; // NTIL - hard code to half wait
	pval = (pval << 20) |   /* mrw_wait starts at bit 20 */
		(control_word << 12) |   /* mrw_mr from bit 12 <= mrw_mr[1:0] = bank, mrw_mr[4:3] = bg. So selecting MR5 = 101 (BG0 | BA1 | BA0) */
		1;               /* mrw_cmd */

	REG_WRITE(MRW_CTRL, pval);

	BUSY_WAIT_TIMEOUT_MC(READ_REG_FIELD(SCL_START, SCL_START_set_ddr_scl_go_done) != 0, MC_TIMEOUT);


}



UINT8 set_scl_all_cs(void)
{
	UINT8 orig_cs = 0;
	UINT32 reg_read_val = 0;


	reg_read_val = REG_READ(SCL_CONFIG_2);
	orig_cs = reg_read_val & ((1 << MEM_CHIP_SELECTS) - 1);
	// Setup scl_test_cs such that when DRAM mode register is programmed, it is programmed for all ranks together
	reg_read_val = REG_READ(SCL_CONFIG_2);
	//  NTIL: we only have 1 CS so we need to clear/set bit 0 only yet teh code is
	 // general and include clear/set to be compatible with the verilog source
	reg_read_val &= ~((2 << (MEM_CHIP_SELECTS - 1)) - 1);
	reg_read_val |= ((2 << (MEM_CHIP_SELECTS - 1)) - 1);
	REG_WRITE(SCL_CONFIG_2, reg_read_val);
	return orig_cs;
}


void restore_scl_cs(UINT32 orig_cs)
{
	// Restore scl_test_cs to the value of the rank we are using for training
	UINT32 reg_read_val = REG_READ(SCL_CONFIG_2);
	//  NTIL: we only have 1 CS so we need to clear/set bit 0 only yet teh code is
	// general and include clear/set to be compatible with the verilog source
	reg_read_val &= ~((2 << (MEM_CHIP_SELECTS - 1)) - 1);
	reg_read_val |= orig_cs;
	REG_WRITE(SCL_CONFIG_2, reg_read_val);

}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        vref_dq_training_ddr4                                                                  */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs VREF training. Note that it's dependent on ODT and drive.        */
/*---------------------------------------------------------------------------------------------------------*/

// Formula for calculating VREF output voltage as a function of TRIM_VREF when TRIM_VREF >=2 and <= 64:
// VREF = 0.100*VDDIO + (TRIM_VREF-2)*0.0066*VDDIO
// Formula for calculating VREF output voltage as a function of TRIM_VREF when TRIM_VREF >=65 and <= 126:
// VREF = 0.507*VDDIO + (TRIM_VREF-65)*0.0066*VDDIO

void vref_dq_training_ddr4 (DDR_Setup *ddr_setup)
{

	float  soc_drive = (float)ddr_setup->soc_drive;
	float  dram_odt = (float)ddr_setup->dram_odt ;
	float  dram_drive = (float)ddr_setup->dram_drive ;
	float  soc_odt =  (float)ddr_setup->soc_odt ;
	int lane;
	DEFS_STATUS status;
	UINT32 reg_read_val;
	unsigned int itter = 0;


	// ============== PARAMETER =============== //
	const UINT8 RUN_PHY_VREF = 1;	  // PHY side (i.e. Read) VREF training is always enabled
	const UINT8 RUN_DRAM_VREF = 1;	 // Enable DRAM side (i.e. Write) VREF training only for DRAM types that support it
	const UINT8 PHY_PULL_UP_ODT = 1;  // Only LPDDR4 uses pull down ODT on PHY side, other DRAM types use pull up ODT
	const UINT8 DRAM_PULL_UP_ODT = 1; // Only LPDDR4 uses pull down ODT on DRAM side, other DRAM types use pull up ODT

	const UINT8 MAX_VREF_TRIM_PHY = 126;  // Max VREF trim value supported by PHY selected based on pull down or pull up ODT
			                              // Because UVREF supports 2 trim ranges depending on pull down or pull up ODT
	const UINT8 SWEEP_LIMIT = 73;		   // Total number of unique VREF trim settings supported by DDR4, including both range 0 and range 1
	const UINT8 MAX_BEST_VREF_SAVED = 30; // Maximum number of BEST VREF settings that we need to save
	const UINT32 VREF_TRAINING_ITERATION = 1;

	// ============== SIGNAL LIST =============== //
	UINT32 vref_mid_level_code; // Saves the VREF trim setting corresponding to the theoretical expected mid level VREF
	UINT32 vref_training_value; // Variable used to sweep the value of VREF during training
	UINT32 sweep_range; // Number of VREF trim steps that need to be swept around the center VREF value

	UINT32 vref_step; // used to speed up VREF calculation without going through every VREF value
	BOOLEAN wr_bit_lvl_failure;  // If VREF training fails, we don't read-out '1' and '0' window values on that iteration

	UINT32 window_0;	 // Width of '0' window read-out after VREF training
	UINT32 window_1;	 // Width of '1' window read-out after VREF training
	UINT32 window_diff; // Difference between the width of '0' and '1' windows - minimizing this difference will result
			                     // in closest to 50% duty cycle and this is the main goal of VREF training
	volatile UINT32 best_window_diff_so_far [MEM_STRB_WIDTH] __attribute__((aligned(16))); // Smallest window difference recorded so far at any VREF setting per byte lane
	volatile UINT32 all_best_vref_matches [MEM_STRB_WIDTH][MAX_BEST_VREF_SAVED * VREF_TRAINING_ITERATION] __attribute__((aligned(16))); // ALL VREF settings that result in smallest window difference
	volatile UINT32 all_common_best_vref_matches[MAX_BEST_VREF_SAVED*VREF_TRAINING_ITERATION] __attribute__((aligned(16))); // ALL VREF settings that result in smallest window difference
	volatile UINT32 bucket_common_vref[MAX_VREF_TRIM_PHY] __attribute__((aligned(16)));

	UINT32 highest_best_vref_val; // If multiple VREF settings gave the smallest window difference, the highest of those
	UINT32 lowest_best_vref_val;  // If multiple VREF settings gave the smallest window difference, the lowest of those
	volatile UINT32 num_best_vref_matches[MEM_STRB_WIDTH] __attribute__((aligned(16))); // Total number of VREF settings that give the smallest window difference

	double vref_mid_level; // VREF mid level of high and low DC levels calculated based on ODT & Drive strength

	UINT32   current_vref; // Current VREF setting being trained and/or programmed

	UINT8 orig_cs_config; // Used to save the value of scl_test_cs register and restore it later

	// ============== INITIALIZE VARIABLES =============== //

	vref_step = 1;	  // set to 1 to check every vref_value

	HAL_PRINT("MC Vref training\n");

	for (unsigned int j = 0; j < (MAX_BEST_VREF_SAVED * VREF_TRAINING_ITERATION); j++ )
	{
		all_common_best_vref_matches[j] = 0xFFFFFFFF;
		for (int i = 0; i < MEM_STRB_WIDTH; i++) 
		{
			all_best_vref_matches[i][j] = 0;
		}
	}
	for (int i = 0; i < MEM_STRB_WIDTH; i++) 
	{
		num_best_vref_matches[i]= 0;
		for (int j = 0; j < MAX_VREF_VAL; j++) 
		{
			bucket_save_diff_lane[i][j] = 0;
		}
	}
	for (int i = 0; i < MAX_VREF_TRIM_PHY; i++) 
	{
		bucket_common_vref[i]= 0;
	}

	HAL_PRINT_DBG("SOC:  drive %d odt %d ; DRAM: drive %d odt %d \n",
		ddr_setup->soc_drive, ddr_setup->soc_odt, ddr_setup->dram_drive, ddr_setup->dram_odt );

   //LogMessage("Vref DQ training started at time: %t", $time);
  // NTIL - added afre UNQ review
  	REG_WRITE(BIT_LVL_CONFIG, ((BIT_LEVEL_SAMPLE_POSITIVE_ONLY << 4) | BIT_LVL_SAMPLE_QTY));
	// ****************** READ SIDE VREF TRAINING ******************** //



//**************************************************************************************************************************************************************
//           PHY side VREF:
//**************************************************************************************************************************************************************


	// Calculate the theoretical expected mid level VREF as a fraction of VDDIO and find the
	// corresponding UVREF IO trim code and also taking into account if pull up or pull down ODT
	// is being used. The UVREF IO has 2 trim code ranges based on whether pull up or pull down
	// ODT is being used and the correct formula (given in IO user guide) has to be used based on that
	//vref_mid_level = (1 + (dram_drive / (soc_odt + dram_drive))) / 2.0;
	vref_mid_level = ((soc_odt / 2) + dram_drive) / (soc_odt + dram_drive);
	 /*
	 float temp1 = ceilf(((((float)vref_mid_level - 0.507f) / 0.0066f) + 65) - 0.5f);
	 if (temp1 > 255.0f)
	 {
		   // Value is expected to be less than 255 - if not the case issue an error
		   LogError("Calculation of vref_mid_level_code should have resulted in a BYTE value but got %f\n", temp1);
	 }*/
	 vref_mid_level_code = calc_vref_phy_value(ddr_setup); // (UINT8)ceilf(((((float)vref_mid_level - 0.507f) / 0.0066f) + 65) - 0.5f);
	float temp1 = ceilf(((float)vref_mid_level * SWEEP_RANGER_PRECENTAGE / 0.0066f) - 0.5f); // Sweep range of
	if (temp1 > 255.0f)
	{// Value is expected to be less than 255 - if not the case issue an error
		   HAL_PRINT_DBG("Calculation of sweep_range should have resulted in a BYTE value but got %f\n", temp1);
	}
	sweep_range = (UINT8)ceilf(((float)vref_mid_level * SWEEP_RANGER_PRECENTAGE / 0.0066f) - 0.5f); // Sweep range of

/**/
	for (lane = 0; lane < MEM_STRB_WIDTH; lane++) {
			best_window_diff_so_far[lane] = 255; // Set initial value prior to training
			num_best_vref_matches[lane] = 0;
			for (int j = 0; j < MAX_VREF_TRIM_PHY; j++)
				bucket_save_diff_lane[lane][j] = 0xAA;
	}

	// vref_training_value gives the offset value from the mid-level VREF that we want to train at
	// We sweep it from 0 to sweep_range*2 because from 0 to sweep_range we are decreasing the
	// VREF value from mid-level and from sweep_range + 1 to sweep_range*2 we are increasing the
	// VREF value from mid-level
	for (vref_training_value = 0; vref_training_value < (sweep_range * 2) + 1 ; vref_training_value += vref_step)
	{
		if(vref_training_value == (sweep_range + 1))
			HAL_PRINT_DBG("\n");

		if (vref_mid_level_code - vref_training_value < 2)
		{
			 // If during negative sweep, the trim code becomes smaller than 2 (lowest supported UVREF
			 // trim code), we have to stop and resume with positive sweep above VREF mid-level
			 vref_training_value = sweep_range;
			 continue;
		}
		else if (vref_training_value + vref_mid_level_code > MAX_VREF_TRIM_PHY)
		{
			 // If during positive sweep, the trim code overflows the maximum supported UVREF trim
			 // code, we have to end VREF training
			 break;
		}

		// Change UVREF trim code for each lane by programming VREF_TRAINING register
		for (lane = 0; lane < MEM_STRB_WIDTH; lane++)
		{
			 REG_WRITE(PHY_LANE_SEL, VREF_WIDTH * lane);

			 if (vref_training_value < sweep_range + 1)
			 {
			 	// Negative sweep
			 	current_vref = vref_mid_level_code - vref_training_value;
			 }
			 else
			 {
			 		// Positive sweep
			 	current_vref = vref_mid_level_code + vref_training_value - sweep_range;

			 }

			REG_WRITE(PHY_LANE_SEL, VREF_WIDTH * lane);
			REG_WRITE(VREF_TRAINING,       BUILD_FIELD_VAL(VREF_TRAINING_vref_output_enable, 1) |
							BUILD_FIELD_VAL(VREF_TRAINING_vref_training, 1) |
							BUILD_FIELD_VAL(VREF_TRAINING_vref_value, current_vref));	// Turn on VREF training mode in the PHY

		}
		reg_read_val = REG_READ(DYNAMIC_BIT_LVL);

		// Run VREF training using DRAM MPR read pattern register, so we don't need to initialize any data pattern into the DRAM
		REG_WRITE( SCL_START, 0x30800000 );
		// Wait for training completion
		BUSY_WAIT_TIMEOUT_MC(READ_REG_FIELD(SCL_START, SCL_START_set_ddr_scl_go_done) != 0, 10000);

		CLK_Delay_MicroSec(0x10000);

		// Check VREF training windows for each byte lane
		for (lane = 0; lane < MEM_STRB_WIDTH; lane++)
		{
			HAL_PRINT_DBG("Lane%d ", lane);
			// Check that this lane passed training by reading DYNAMIC_BIT_LVL register, implying '1' and '0' windows were detected as expected
			reg_read_val = READ_REG_FIELD(DYNAMIC_BIT_LVL, DYNAMIC_BIT_LVL_bit_lvl_failure_status);
			if ((reg_read_val  & (1 << lane)) == 0) {
				// HAL_PRINT_DBG("Lane%d PHY_VREF: %d (%d %%)\t", lane, current_vref, DIV_ROUND((100*current_vref), 0x7F));

				// Read out VREF training windows for this byte lane
				REG_WRITE(PHY_LANE_SEL, lane*BITLVL_DLY_WIDTH);
				reg_read_val = REG_READ (VREF_TRAINING_WINDOWS);
				window_0 = READ_REG_FIELD(VREF_TRAINING_WINDOWS, VREF_TRAINING_WINDOWS_ddr_window_0); // (reg_read_val >> 0) & ((1<<BITLVL_DLY_WIDTH) - 1);
				window_1 = READ_REG_FIELD(VREF_TRAINING_WINDOWS, VREF_TRAINING_WINDOWS_ddr_window_1); //(reg_read_val >> 8) & ((1<<BITLVL_DLY_WIDTH) - 1);

				window_diff = (window_0 > window_1) ? window_0 - window_1 : window_1 - window_0;

				HAL_PRINT_DBG("Win_0:%2d, Win_1:%2d, diff:%3d;   \t",
					window_0, window_1, (int)window_0 - (int)window_1);


				if(current_vref > 255)
				{
					HAL_PRINT(KRED "Current VREF was expected to be less than 255 0x%x\n" KNRM, current_vref);
				}
				// Check if window_diff we got is the best so far and if so save the information for later evaluation
				if (window_diff < best_window_diff_so_far[lane])
				{
					best_window_diff_so_far[lane] = window_diff;
					all_best_vref_matches[lane][0] = (UINT8)current_vref;
					num_best_vref_matches[lane] = 1;
					// HAL_PRINT_DBG("\tlane%d CURRENT BEST VREF :0x%lx \t", lane, current_vref);
				}
				else if ((window_diff == best_window_diff_so_far[lane]) &&
					(num_best_vref_matches[lane] < MAX_BEST_VREF_SAVED))
				{
					all_best_vref_matches[lane][num_best_vref_matches[lane]] = (UINT8)current_vref;
					num_best_vref_matches[lane] = num_best_vref_matches[lane] + 1;
				}
				bucket_save_diff_lane[lane % MEM_STRB_WIDTH][current_vref % MAX_VREF_VAL] = window_diff;
			}
			else
			{
				HAL_PRINT("lane%d PHY side VREF training failed , current_vref = %d (0x%x) \n", lane, current_vref, current_vref);
			}
		}
				HAL_PRINT_DBG("\n");
	}
	// print_full_window_diffs();

	// Iterate over ALL VREF settings that gave smallest window_diff in each lane to find the highest and lowest such VREF value for each lane
	for (lane = 0; lane < MEM_STRB_WIDTH; lane++) {
		HAL_PRINT_DBG(KCYN "> BMC VREF (read): lane%d: " KNRM, lane);
		highest_best_vref_val = 0x0;
		lowest_best_vref_val = 0x7F;
		for (UINT32 i = 0; i < num_best_vref_matches[lane]; i++) {
			highest_best_vref_val = all_best_vref_matches[lane][i] > highest_best_vref_val ? all_best_vref_matches[lane][i] : highest_best_vref_val;
			lowest_best_vref_val  = all_best_vref_matches[lane][i] < lowest_best_vref_val  ? all_best_vref_matches[lane][i] : lowest_best_vref_val;
		}

		current_vref = highest_best_vref_val; // Convert from unsigned char to unsigned int to avoid overflow on following line
		current_vref = (current_vref + lowest_best_vref_val) >> 1; // Find midpoint of highest_best_vref_val and lowest_best_vref_val

		REG_WRITE(PHY_LANE_SEL, VREF_WIDTH * lane);
		// Program the final result of VREF training and turn off VREF training
		REG_WRITE(VREF_TRAINING,        BUILD_FIELD_VAL(VREF_TRAINING_vref_output_enable, 1) |
						BUILD_FIELD_VAL(VREF_TRAINING_vref_value, current_vref));	// Turn on VREF training mode in the PHY

		HAL_PRINT_DBG("Read side programmed trained VREF lane%d: %d (0x%lx) ,best = %d (0x%x)\n",
			lane, current_vref, current_vref, highest_best_vref_val, highest_best_vref_val);
	}

	HAL_PRINT_DBG(KMAG "Finished Vref DQ Training PHY Side\n" KNRM);

//**************************************************************************************************************************************************************
//           DRAM side VREF:
//**************************************************************************************************************************************************************

	HAL_PRINT_DBG (KCYN "\n> Run MPR read bit-leveling \n" KNRM);
	// *******************************************
	// Start: Section of code only for NONE_LP4_PHY
	// *******************************************
	// Setup the bit-leveling data in the PHY
	REG_WRITE( SCL_DATA_0, 0xff00ff00 );
	REG_WRITE( SCL_DATA_1, 0xff00ff00 );

	// Run MPR read bit-leveling
	REG_WRITE( SCL_START, 0x30800000) ;
	// Poll till MPR read bit-leveling is finished
	BUSY_WAIT_TIMEOUT_MC(READ_REG_FIELD(SCL_START, SCL_START_set_ddr_scl_go_done) != 0, 10000);

	CLK_Delay_MicroSec(0x10000);
	// Check Failure Status
	reg_read_val = REG_READ( DYNAMIC_BIT_LVL );

	if (READ_REG_FIELD(DYNAMIC_BIT_LVL, DYNAMIC_BIT_LVL_bit_lvl_failure_status) == 0)
	{
		HAL_PRINT_DBG("* Bit leveling passed on all lanes for the read side");
	} else {
		HAL_PRINT(KRED "* ERROR! Bit leveling failed for the read side! Lanes failed(1=fail): 0x%lx \n \n" KNRM, READ_REG_FIELD(DYNAMIC_BIT_LVL, DYNAMIC_BIT_LVL_bit_lvl_failure_status) );
	}


	// Program the IP DQ trim override. The resultant trim values from the previous
	// run of MPR read bit-leveling automatically propagates into the override
	// trim register. Only programming the override is needed.
	HAL_PRINT_DBG (" > Override IP DQ trim values from the previous run of MPR read bit-leveling.  \n");
	SET_REG_FIELD(IP_DQ_DQS_BITWISE_TRIM, IP_DQ_DQS_BITWISE_TRIM_ip_dq_trim_override, 1) ; // ( IP_DQ_DQS_BITWISE_TRIM, 7 );
	// *******************************************
	// End: Section of code for non LP4-PHY
	// *******************************************

	HAL_PRINT_DBG (KCYN "\n> DRAM side (write) VREF training: \n" KNRM);

	// ****************** WRITE SIDE VREF TRAINING ******************** //

	// Calculate the theoretical expected mid level VREF as a fraction of VDDIO and find the
	// corresponding DDR4 or LPDDR4 DRAM VREF trim code respectively depending on whether pull up
	// or pull down ODT is enabled. trim code = 0 corresponds to lowest DRAM VREF value that is
	// supported across both range 0 and range 1 which is 0.45*VDDQ for DDR4 & 0.1*VDDQ for LPDDR4

	// Try different calculation
	vref_mid_level = ((dram_odt / 2) + soc_drive) / (dram_odt + soc_drive);


	vref_mid_level_code = (UINT8)ceilf((((float)vref_mid_level - 0.450f) / 0.0065f) - 0.5f);

	sweep_range = (UINT8)ceilf(((float)vref_mid_level * SWEEP_RANGER_PRECENTAGE / 0.0065f) - 0.5f); // Sweep range of
	// 0.15 (15%) is only a suggested value. User may customize to increase it if required.

	HAL_PRINT_DBG("DRAM  Write side VREF vref_mid_level = %f , vref_mid_level_code = %d , sweep_range = %d \n",
		(float)vref_mid_level, vref_mid_level_code, sweep_range);

	orig_cs_config = set_scl_all_cs();


	// Set the initial VREF value and turn on DRAM mode register bit(s) required for VREF training
	setup_vref_training_registers (ddr_setup, vref_mid_level_code, 1); // Turn on VRCG

	// Restore scl_test_cs to the value of the rank we are using for training
	restore_scl_cs(orig_cs_config);

	// Turn on VREF training mode in the PHY
	REG_WRITE(PHY_LANE_SEL, 0);
	reg_read_val = REG_READ(VREF_TRAINING);
	//  reg_read_val |= ( 1 << 0 &  ~(1 << 0));
	// NTIL - add set bit to enable trtraining - works for all lanes
	reg_read_val |= 1 << 0;
  	REG_WRITE(VREF_TRAINING, reg_read_val);  // Turn on VREF training mode in the PHY

	SET_REG_FIELD(VREF_TRAINING, VREF_TRAINING_vref_training, 1);	// Turn on VREF training mode in the PHY

	for (lane = 0; lane < MEM_STRB_WIDTH; lane = lane + 1) {
		best_window_diff_so_far[lane] = 255; // Set initial value prior to training
		num_best_vref_matches[lane] = 0;
		for (int j = 0; j < MAX_VREF_TRIM_PHY; j++)
			bucket_save_diff_lane[lane][j] = 0xAA;
	}

	// vref_training_value gives the offset value from the mid-level VREF that we want to train at
	// We sweep it from 0 to sweep_range*2 because from 0 to sweep_range we are decreasing the
	// VREF value from mid-level and from sweep_range + 1 to sweep_range*2 we are increasing the
	// VREF value from mid-level
	for (vref_training_value = 0; vref_training_value < (sweep_range * 2) + 1; vref_training_value = vref_training_value + vref_step)
	{

		if(vref_training_value == (sweep_range + 1))
			HAL_PRINT_DBG("\n");
		if (vref_training_value < sweep_range + 1) {
			// If during negative sweep, the trim code becomes smaller than 0 (lowest supported DRAM
			// trim code), we have to stop and resume with positive sweep above VREF mid-level
			if (vref_training_value > vref_mid_level_code) {
				vref_training_value = sweep_range;
				continue;
			}
			else
			{
				current_vref = vref_mid_level_code - vref_training_value;
				setup_vref_training_registers(ddr_setup, current_vref , 0);
			}
		}
		else
		{
			if ((vref_mid_level_code + vref_training_value - sweep_range) > SWEEP_LIMIT) {
				// If during positive sweep, the trim code overflows the maximum supported DRAM trim
				// code, we have to end VREF training
				break;
			}
			else
			{
				current_vref = vref_mid_level_code + vref_training_value - sweep_range;
				setup_vref_training_registers_(ddr_setup, current_vref);
			}
		}
		for (itter = 0; itter < VREF_TRAINING_ITERATION; itter++)
		{
			// HAL_PRINT_DBG("\nIteration: %d\n", itter);
			// Run VREF training for write side
			// Use DRAM Array for training
			REG_WRITE( SCL_START, 0x30500000);
			// Wait for training completion
			BUSY_WAIT_TIMEOUT_MC(READ_REG_FIELD(SCL_START, SCL_START_set_ddr_scl_go_done) != 0, 10000);

			// Check if all lanes passed training by reading DYNAMIC_WRITE_BIT_LVL register, implying '1' and '0' windows were detected as expected
			reg_read_val = REG_READ(DYNAMIC_WRITE_BIT_LVL);
			wr_bit_lvl_failure  = (reg_read_val >> 20);

			// Check VREF training windows for each byte lane
			for (lane = 0; lane < MEM_STRB_WIDTH; lane = lane + 1) {
				if ((wr_bit_lvl_failure & (1 << lane)) == 0) {

					HAL_PRINT_DBG("lane%d ", lane);

					// Read out VREF training windows for this byte lane
					REG_WRITE(PHY_LANE_SEL, lane * BITLVL_DLY_WIDTH);
					reg_read_val = REG_READ (VREF_TRAINING_WINDOWS);
					window_0 = (reg_read_val >> 0) & ((1<<BITLVL_DLY_WIDTH) - 1);
					window_1 = (reg_read_val >> 8) & ((1<<BITLVL_DLY_WIDTH) - 1);

					window_diff = (window_0 > window_1) ? window_0 - window_1 : window_1 - window_0;

					HAL_PRINT_DBG("Win_0:%2d, Win_1:%2d, diff:%3d; \t", window_0, window_1, (int)window_0 - (int)window_1);

					// Check if window_diff we got is the best so far and if so save the information for later evaluation
					if (current_vref > 255)
					{
						HAL_PRINT(KRED "Current VREF was expected to be less than 255 0x%x\n" KNRM, temp1);
					}

#if 1 // TEST_METHOD
					if (window_diff <= 2)
					{
						all_best_vref_matches[lane][num_best_vref_matches[lane]] = (UINT8)current_vref;
						num_best_vref_matches[lane]++;
					}
#else

					if (window_diff < best_window_diff_so_far[lane]) {
						best_window_diff_so_far[lane] = window_diff;
						all_best_vref_matches[lane][0] = (UINT8)current_vref;
						num_best_vref_matches[lane] = 1;
						HAL_PRINT_DBG("CURRENT BEST VREF write side :%d Lane %d\n", current_vref, lane);
					}
					else if ((window_diff == best_window_diff_so_far[lane]) && (num_best_vref_matches[lane] < MAX_BEST_VREF_SAVED)) {
						all_best_vref_matches[lane][num_best_vref_matches[lane]] = (UINT8)current_vref;
						num_best_vref_matches[lane] = num_best_vref_matches[lane] + 1;
					}
#endif
					bucket_save_diff_lane[lane][current_vref % MAX_VREF_VAL] = window_diff;
					// NTIL - take absolut value of Diff <= 1

				}
				else {


					HAL_PRINT("\n**** VREF training failed on one lane or more 0x%X, current_vref = %d (0x%x)\n", wr_bit_lvl_failure, current_vref, current_vref);
				}
			}
			HAL_PRINT_DBG("\n");
		}
	}

	highest_best_vref_val = 0x0;
	lowest_best_vref_val = 0x7F;
	// print_full_window_diffs();
#ifdef TEST_METHOD
	for (int i = 0; i < MAX_VREF_TRIM_PHY; i++) 
	{
		bucket_common_vref[i] = 0;
	}

	int num_common_vref_best = 0;
	for (int i = 0; i < num_best_vref_matches[0]; i++)
	{
		for (int j = 0; j < num_best_vref_matches[1]; j++)
		{
			if (all_best_vref_matches[1][j] == all_best_vref_matches[0][i])
			{
				// Common Value!
				bucket_common_vref[all_best_vref_matches[0][i]] = 1;

				// Start at next position next time

				// We'll break at next  loop
			}

		}

	}
	HAL_PRINT_DBG("Best VREF Values:\t");
	for (int i = 0; i < MAX_VREF_TRIM_PHY; i++)
	{
		if (bucket_common_vref[i] == 1)
		{
			all_common_best_vref_matches[num_common_vref_best++] = i;
			HAL_PRINT_DBG("%d,\t",i );
		}
	}
	num_common_vref_best = (num_common_vref_best % 2) + (num_common_vref_best >> 1) -1;
	HAL_PRINT_DBG("\nCurrent VREF %d\n", all_common_best_vref_matches[num_common_vref_best]);
	current_vref = all_common_best_vref_matches[num_common_vref_best];
#else
	// Iterate over ALL VREF settings that gave smallest window_diff in each lane to find the highest and lowest such VREF value across all lanes
	for (lane = 0; lane < MEM_STRB_WIDTH; lane = lane + 1) {
		for (UINT32 i = 0; i < num_best_vref_matches[lane]; i = i + 1) {
			highest_best_vref_val = all_best_vref_matches[lane][i] > highest_best_vref_val ? all_best_vref_matches[lane][i] : highest_best_vref_val;
			lowest_best_vref_val  = all_best_vref_matches[lane][i] < lowest_best_vref_val  ? all_best_vref_matches[lane][i] : lowest_best_vref_val;
		}
	}

	// Convert from unsigned char to unsigned int to avoid overflow on following line
	current_vref = highest_best_vref_val;
	current_vref = (current_vref + lowest_best_vref_val) >> 1;
#endif



	// Program trained VREF to all ranks
	orig_cs_config = set_scl_all_cs();

	// Debug - add error in case VREF is hiher than 65 - becuase it is most likely didnt pass any VREF value
	if (current_vref >= 65)
	{
		HAL_PRINT(KRED "Note: VREF value set to %d (0x%x) which is too high!\n" KNRM, current_vref, current_vref);
	}
	setup_vref_training_registers_(ddr_setup, current_vref);


	// Turn off VREF training mode in the PHY
	REG_WRITE(PHY_LANE_SEL, 0);
	reg_read_val = REG_READ(VREF_TRAINING);
	reg_read_val &= ~(1 << 0);
	reg_read_val |= (1 << 2);
	REG_WRITE(VREF_TRAINING, reg_read_val);

	// Keep BEST VREF while turning off VrefDQ training mode in mode register
	setup_vref_training_registers(ddr_setup, current_vref, 2);

	// Save DRAM VREF for testing purposes
	// Save the value instead of it's MR value, do the alignment when preparing the MR value
	ddr_setup->SaveDRAMVref = current_vref;



	restore_scl_cs(orig_cs_config);

	HAL_PRINT_DBG("Finished Vref DQ Training DRAM Side. Programmed VREF = %d (0x%x)\n", current_vref, current_vref);

	// Turn off the IP DQ trim override. Required only for NONE_LP4_PHY
	CLEAR_REG_BIT(IP_DQ_DQS_BITWISE_TRIM /*0x1A0*/, 7);


	//HAL_PRINT_DBG("Vref DQ training started at time: %t", $time);
	// NTIL - added afre UNQ review

	 for (int i = 0; i < MEM_STRB_WIDTH; i++)
	{
		REG_WRITE(PHY_LANE_SEL          /*0x12c*/, i);
 //  REG_WRITE(BIT_LVL_CONFIG, ((BIT_LEVEL_SAMPLE_BOTH << 4) | BIT_LVL_SAMPLE_QTY));
		reg_read_val = REG_READ(BIT_LVL_CONFIG);
	}


	// for (int i = 0; i < MEM_STRB_WIDTH; i++)
 //  {
//		REG_WRITE(PHY_LANE_SEL          /*0x12c*/, i);
		REG_WRITE(BIT_LVL_CONFIG, ((BIT_LEVEL_SAMPLE_BOTH << 4) | BIT_LVL_SAMPLE_QTY));
	//   reg_read_val = REG_READ(BIT_LVL_CONFIG);
	// }
	//reg_write_phy(`BIT_LVL_CONFIG, (2'h3 /*bit_lvl_edge_sel*/ << 4) | BIT_LVL_SAMPLE_QTY);

	//NTIL - hard code the VREF value



	for (unsigned int ilane = 0 ; ilane < 2 ; ilane++)
	{
		REG_WRITE( PHY_LANE_SEL , VREF_WIDTH * ilane );

		if(ddr_setup->vref_soc[ilane]      != 0xFF)
		{
			HAL_PRINT(KMAG "Lane%d: set vref_soc to %#010lx\n" KNRM, ilane, ddr_setup->vref_soc[ilane]);
			SET_REG_FIELD(VREF_TRAINING, VREF_TRAINING_vref_value, ddr_setup->vref_soc[ilane]);
		}
	}

	if(ddr_setup->vref_dram      != 0xFF)
	{
		HAL_PRINT(KMAG "set vref_dram to %#010lx\n" KNRM, ddr_setup->vref_dram);
		setup_vref_training_registers_(ddr_setup, (UINT32)ddr_setup->vref_dram);

	}




} /* of vref_dq_training_ddr4 */



/*---------------------------------------------------------------------------------------------------------*/
/* Function:        run_write_leveling                                                                     */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void run_write_leveling (DDR_Setup *ddr_setup)
{
	DEFS_STATUS status;
	UINT32 reg_read_val = 0;
	unsigned int ilane;



	HAL_PRINT_DBG(KCYN "\n====================  \n" KNRM);
	HAL_PRINT_DBG(KCYN "   Write Leveling     \n" KNRM);
	HAL_PRINT_DBG(KCYN "====================  \n" KNRM);

	MC_PrintTrim(TRUE, TRUE);
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Enable write-leveling
	// Set start_wr_lvl for all lanes
	REG_WRITE(WRLVL_AUTOINC_TRIM /*0x14C*/, 0x3);

	// Clear all dlls_trim_2 registers
	REG_WRITE(PHY_DLL_TRIM_2 /*0x134*/, 1 << 16);
	// Set wr level command
	REG_WRITE(SCL_START /*0x100*/, 1 << 30 | 1 << 28);

	// Check for completion
	BUSY_WAIT_TIMEOUT_MC(READ_REG_FIELD(SCL_START, SCL_START_set_ddr_scl_go_done) != 0, MC_TIMEOUT);
	// Read out the result


	reg_read_val = REG_READ(WRLVL_CTRL /*0x149*/);
	if ((reg_read_val & 0x3) == 0x3) {
		HAL_PRINT_DBG("\n##### Write leveling finished on all lanes\n");
		LogLevelingData();
	}
	else {
		HAL_PRINT_DBG("##### Error in ddr_phy_cfg2 @ Write leveling!-  didn't finish\n");
	}
#if 1

	// Overiding TRIM_2 Value to overcome an issue with the SVB.
	// Later, there will be swipping on this value to pin-point it.
	HAL_PRINT_DBG("##### Override DLL Trim_2\n");
	for (ilane = 0 ; ilane < NUM_OF_LANES ; ilane++)
	{
			REG_WRITE( PHY_LANE_SEL , SLV_DLY_WIDTH * ilane );
			reg_read_val = READ_REG_FIELD(PHY_DLL_TRIM_2, PHY_DLL_TRIM_2_dlls_trim_2);

			if (ddr_setup->trim2_init_offset[ilane] != 0xFF)
			{
				if (ddr_setup->trim2_init_offset[ilane] & MASK_BIT(7))
				{
					HAL_PRINT(KMAG "Lane%d: PHY_DLL_TRIM_2_dlls_trim_2  is increased by  %#010lx \n" KNRM, ilane, ddr_setup->trim2_init_offset[ilane] & 0x7F);
					reg_read_val += ddr_setup->trim2_init_offset[ilane];
				}
				else
				{
					HAL_PRINT(KMAG "Lane%d: PHY_DLL_TRIM_2_dlls_trim_2  is decreased by %#010lx \n" KNRM, ilane, ddr_setup->trim2_init_offset[ilane] & 0x7E);
					reg_read_val -= ddr_setup->trim2_init_offset[ilane];
				}
				SET_REG_FIELD(PHY_DLL_TRIM_2, PHY_DLL_TRIM_2_dlls_trim_2, reg_read_val);
			}
	}
	REG_WRITE( PHY_LANE_SEL , 0x00000000 );
#endif
	MC_PrintTrim(TRUE, TRUE);

}


void run_bit_leveling (void)
{
	UINT32 reg_read_val = 0;
	UINT32 save_lowest_val = 0xFFFFFFFF;
	DEFS_STATUS status;
	int lane = 0;


	HAL_PRINT_DBG(KCYN "\n============================  \n" KNRM);
	HAL_PRINT_DBG(KCYN "   Read/Write Bit Leveling     \n" KNRM);
	HAL_PRINT_DBG(KCYN "=============================  \n" KNRM);

	HAL_PRINT_DBG("##### Starting bit leveling routine.\n");
	// Run bit leveling, can be run with write side training, set appropriate
	// software parameters in tb_unq_top
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Setup the bit-leveling data in the PHY
	REG_WRITE(SCL_DATA_0 /*0x104*/, 0xff00ff00);
	REG_WRITE(SCL_DATA_1 /*0x108*/, 0xff00ff00);

	// Run MPR read bit-leveling
	REG_WRITE(SCL_START /*0x100*/, 0x184 << 21);

	// Poll till MPR read bit-leveling is finished
	BUSY_WAIT_TIMEOUT_MC(READ_REG_FIELD(SCL_START, SCL_START_set_ddr_scl_go_done) != 0, MC_TIMEOUT);

	// Check Failure Status
	reg_read_val = REG_READ( DYNAMIC_BIT_LVL /*0x1AC*/ );
	if ( (~reg_read_val & 0xC000) == 0xC000 ) {
		 HAL_PRINT_DBG( "##### Bit leveling passed on all lanes for the read side, DYNAMIC_BIT_LVL = 0x%lx \n" ,
		 	REG_READ( DYNAMIC_BIT_LVL));
		 // REG_WRITE( ERROR_DB_BASE_ADDR + 2*0x4, (1<<16) | (0xCAFE) );
	}
	else
	{
		 HAL_PRINT(KRED "##### Bit leveling failed for the read side! Lanes failed(1=fail) status: 0x%lx \n \n",
		 	READ_REG_FIELD( DYNAMIC_BIT_LVL, DYNAMIC_BIT_LVL_bit_lvl_failure_status));
		 HAL_PRINT_DBG( "\n\n##### Error in ddr_phy_cfg2 @ Read bit leveling \n" KNRM);
		 // REG_WRITE( ERROR_DB_BASE_ADDR + 2*0x4, (1<<16) | (0xDEAD) );
	}




	for (lane = 0; lane < MEM_STRB_WIDTH; lane++) {

		REG_WRITE(PHY_LANE_SEL, SLV_DLY_WIDTH * lane);
		reg_read_val = REG_READ(PHY_DLL_TRIM_2);
		HAL_PRINT_DBG("lane %d Leveling output is PHY_DLL_TRIM_2 = 0x%x  \n", lane, reg_read_val);
		if (save_lowest_val > reg_read_val)
		{
			  save_lowest_val = reg_read_val;
		}
	}




	// Program the IP DQ trim override. The resultant trim values from the previous
	// run of MPR read bit-leveling automatically propagates into the override
	// trim register. Only programming the override is needed.
	SET_REG_BIT( IP_DQ_DQS_BITWISE_TRIM /*0x1A0*/, 7 );

	// Run write-side bit leveling. This will re-run read side bit-leveling
	// which will fail but the correct trim values will always be applied
	// since we programmed the override earlier.
	// Set bit leveling normal from DRAM with write side.
	REG_WRITE( SCL_START /*0x100*/, 0x305<<20 );

	// Poll till write bit-leveling is finished
	BUSY_WAIT_TIMEOUT_MC(READ_REG_FIELD(SCL_START, SCL_START_set_ddr_scl_go_done) != 0, MC_TIMEOUT);

	if ( BIT_LVL_WR_SIDE ) {
		 // Check write side failure status
		 reg_read_val = REG_READ( DYNAMIC_WRITE_BIT_LVL /*0x1C0*/ );
		 if ( (~reg_read_val & 0x300000) == 0x300000 ) {
		    HAL_PRINT_DBG( "##### Bit leveling passed on all lanes for the write side \n" );
		    // REG_WRITE( ERROR_DB_BASE_ADDR + 3*0x4, (3<<16) | (0xCAFE) );
		 } else {
		    HAL_PRINT(KRED "##### Bit leveling failed for the write side! Lanes failed(1=fail): DYNAMIC_WRITE_BIT_LVL_bit_lvl_wr_failure_status =  0x%lx \n \n" KNRM,
		    	READ_REG_FIELD(DYNAMIC_WRITE_BIT_LVL, DYNAMIC_WRITE_BIT_LVL_bit_lvl_wr_failure_status) );
		    //   dut_error( "##### Error in ddr_phy_cfg2 @ Write bit leveling!!" );
		 }
	} // if (BIT_LVL_WR_SIDE)

	// Turn off the IP DQ trim override.
	CLEAR_REG_BIT( IP_DQ_DQS_BITWISE_TRIM /*0x1A0*/, 7 );

	// Write bit-leveling data to DRAM. There should be no worry that the data
	// will be written incorrectly because write bit-leveling has already run.
	// Set the address to start at in the DRAM, Note scl_start_col_addr moves in ddr4
	REG_WRITE( PHY_SCL_START_ADDR /*0x188*/, SCL_START_ADDR_0 );

	// Initialize the data by setting wr_only
	REG_WRITE( SCL_START /*0x100*/, 0x88 << 21 );

	// Poll till write is finished
	BUSY_WAIT_TIMEOUT_MC(READ_REG_FIELD(SCL_START, SCL_START_set_ddr_scl_go_done) != 0, MC_TIMEOUT);

	// Set the address for scl start
	REG_WRITE( PHY_SCL_START_ADDR /*0x188*/, SCL_START_ADDR_1 );

	// Run read bit-leveling using DRAM array
	REG_WRITE( SCL_START /*0x100*/, 0x182<<21 );

	// Poll till normal read bit-leveling is finished
	BUSY_WAIT_TIMEOUT_MC(READ_REG_FIELD(SCL_START, SCL_START_set_ddr_scl_go_done) != 0, MC_TIMEOUT);

#ifdef PRINT_LEVELING
			MC_PrintLeveling(TRUE, TRUE);
#endif
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        ddr_phy_cfg2                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ddr_setup -                                                                            */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS ddr_phy_cfg2 (DDR_Setup *ddr_setup)
{
	UINT32 reg_read_val;
	UINT32 save_lowest_val = 0x3f;
	DEFS_STATUS status = DEFS_STATUS_OK;

	// Wait for the MC initializtion to completed
	BUSY_WAIT_TIMEOUT_MC(READ_REG_FIELD(UNQ_ANALOG_DLL_2, UNQ_ANALOG_DLL_2_analog_dll_lock) != 0x3, MC_TIMEOUT);

	HAL_PRINT("MC cfg2\n");

	if (RUN_WRITE_LEVELING) {
		run_write_leveling(ddr_setup);
	}


#ifdef UNIQUIFY_PHY_HALF_PERIOD
	// Enable gating for bit-leveling
	REG_WRITE(DISABLE_GATING_FOR_SCL /*0x1B8*/, 0x0);
#endif

	// Call VREF training
	vref_dq_training_ddr4(ddr_setup);



	if (RUN_BIT_LEVELING) {
		run_bit_leveling();

	} // if (RUN_BIT_LEVELING)

#ifdef UNIQUIFY_PHY_HALF_PERIOD
	// Enable functional gating
	// Used older gating logic for write side bit level to avoid cutting dqs bursts short
	REG_WRITE( DISABLE_GATING_FOR_SCL /*0x1A0*/, 0x1 );
#endif


	HAL_PRINT_DBG(KCYN "\n=====================================  \n" KNRM);
	HAL_PRINT_DBG(KCYN "   Round-Trip-Delay (SCL) Leveling     \n" KNRM);
	HAL_PRINT_DBG(KCYN "=====================================  \n" KNRM);

	HAL_PRINT_DBG( "##### Starting SCL.\n" );
	// Run SCL
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Clear main clock delta register before doing SCL write-read test
	REG_WRITE( SCL_MAIN_CLK_DELTA /*0x140*/, 0x10 );
	// Assume that bit-leveling has used these
	REG_WRITE( SCL_DATA_0 /*0x104*/, 0x789b3de0 );
	// registers so re-write the scl data
	REG_WRITE( SCL_DATA_1 /*0x108*/, 0xf10e4a56 );
	// Set wr_only bit to initialize memory data
	REG_WRITE( SCL_START /*0x100*/, 1<<28 | 1<<24 );
	BUSY_WAIT_TIMEOUT_MC(READ_REG_FIELD(SCL_START, SCL_START_set_ddr_scl_go_done) != 0, MC_TIMEOUT);

	// Call SCL without write-leveling save restore data(set [31] high for save/restore)
	REG_WRITE( SCL_START /*0x100*/, 1<<29 | 1<<28 | 1<<26 );
	BUSY_WAIT_TIMEOUT_MC(READ_REG_FIELD(SCL_START, SCL_START_set_ddr_scl_go_done) != 0, MC_TIMEOUT);
	reg_read_val = REG_READ( SCL_START /*0x100*/ );
	if ( (reg_read_val & 0x3) == 0x3 ) {
		HAL_PRINT_DBG( "##### SCL passed for all lanes \n" );

	} else {
		HAL_PRINT(KRED  "##### ERROR! SCL failed \n" );
		HAL_PRINT_DBG( "(0=fail): SCL_START_cuml_scl_rslt : 0x%lx\n \n" KNRM,
			READ_REG_FIELD(SCL_START, SCL_START_cuml_scl_rslt) );
		status = DEFS_STATUS_FAIL;
	}

#ifdef UNIQUIFY_PHY_HALF_PERIOD
	// Once SCL has run gating values will be accurate so turn off x propagation fix
	REG_WRITE( DISABLE_GATING_FOR_SCL /*0x1A0*/, 0x3);
#endif
	HAL_PRINT(KCYN "Bit leveling\n" KNRM);

	// Program dynamic bit leveling
	// BIT_LVL_WR_DYNAMIC & BIT_LVL_DYNAMIC are mutually EXCLUSIVE
	if ( ~(BIT_LVL_WR_DYNAMIC & BIT_LVL_DYNAMIC) )
	{
			// REG_WRITE( ERROR_DB_BASE_ADDR + 5*0x4, (5<<16) | (0xCAFE) );
		HAL_PRINT_DBG("done bit lvl dynamic\n");
	}
	else
	{
		// BIT_LVL_WR_DYNAMIC and BIT_LVL_DYNAMIC cannot both be set.
		// REG_WRITE( ERROR_DB_BASE_ADDR + 5*0x4, (5<<16) | (0xDEAD) );
		// exit(1);

		HAL_PRINT_DBG(KMAG "init bit lvl dynamic\n" KNRM);
		status = DEFS_STATUS_FAIL;
	}
	if ( BIT_LVL_WR_DYNAMIC )
	{
		SET_REG_BIT( DYNAMIC_WRITE_BIT_LVL /*0x1C0*/, 0 );
	}
	else
	{
		CLEAR_REG_BIT( DYNAMIC_WRITE_BIT_LVL /*0x1C0*/, 0 );
	}
	if ( BIT_LVL_DYNAMIC ) {
		SET_REG_BIT( DYNAMIC_BIT_LVL /*0x1AC*/, 0 );
	}
	else
	{
		CLEAR_REG_BIT( DYNAMIC_BIT_LVL /*0x1AC*/, 0 );
	}

	// Program DSCL
	// 25:	dscl_save_restore_needed - not supported currently
	// 24:	dscl_en
	// 0-23: dscl_exp_cntq
	if ( (BIT_LVL_WR_DYNAMIC & BIT_LVL_DYNAMIC) ) {
		HAL_PRINT_DBG(KMAG "DSCL enable\n" KNRM);
		REG_WRITE( DSCL_CNT /*0x19C*/, 0 << 25 | PHY_AUTO_SCL << 24 | DSCL_INTERVAL << 0 );
	}
	else {
		HAL_PRINT_DBG("Prepare DSCL parameters (DSCL disable)\n");
		REG_WRITE( DSCL_CNT /*0x19C*/, 1 << 25 | PHY_AUTO_SCL << 24 | DSCL_INTERVAL << 0 );
	}

	// set_odt_in_dram();

	IOW32(0x1000, IOR32(0x1000));
	REG_WRITE(PHY_LANE_SEL,  0);

	SET_REG_FIELD( PHY_DLL_RECALIB, PHY_DLL_RECALIB_disable_recalib, 1);

	return status;

}
