/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   mc_drv_sweeps.c                                                          */
/* This file contains manual sweeps and training for DDR. This file is imported from Poleg, not in use */
/* Project:                                                                   */
/*        SWC HAL                                                             */
/*----------------------------------------------------------------------------*/




// #ifdef ENHANCED_SWEEPING_AND_LEVELING

// max sweep from -62 to +62  NTIL, please check:
#define MC_DQ_DQS_BITWISE_TRIM_SAMPLING_POINTS_TEST  ((UINT32)128)
#define NUM_OF_LANES_TEST_SWEEP                      ((UINT32)2)

extern BOOLEAN   bPrint;

// move to global to use them outside of this funtion.
UINT16            g_Table_Y_BitStatus[MC_DQ_DQS_BITWISE_TRIM_SAMPLING_POINTS_TEST] __attribute__((aligned(16))); // 16bit for 16 bit of data. each bit is 0 for pass or 1 for fail.
UINT16            g_Table_Y_LaneStatus[MC_DQ_DQS_BITWISE_TRIM_SAMPLING_POINTS_TEST] __attribute__((aligned(16)));
UINT32            g_PresetTrim[NUM_OF_LANES_TEST_SWEEP][8] __attribute__((aligned(16)));


static volatile int               g_Table_Y_BestTrim_EyeCenter[16]    __attribute__((aligned(16)));
static volatile int               g_Table_Y_BestTrim_EyeSize[16]      __attribute__((aligned(16)));
static volatile UINT16            g_Table_Z_BitStatus  [40][74] __attribute__((aligned(16)));
static volatile UINT16            g_Table_Z2_BitStatus [40][74] __attribute__((aligned(16)));
static volatile int               g_Table_MinEyeSize[74][2]          __attribute__((aligned(16)));

static void mc_init_sweep_arrays (void)
{
	unsigned int i, j;
	
	for (i = 0; i < MC_DQ_DQS_BITWISE_TRIM_SAMPLING_POINTS_TEST; i++)
	{
		g_Table_Y_BitStatus[i] = 0xFFFF;
		g_Table_Y_LaneStatus[i] = 0xFFFF;
	}

	for (i = 0; i < 16; i++)
	{
		g_Table_Y_BestTrim_EyeCenter[i] = 0;
		g_Table_Y_BestTrim_EyeSize[i] = 0;
	}

	for (i = 0; i < 40; i++)
	{
		for (j = 0; j < 74; j++)
		{
			g_Table_Z_BitStatus[i][j] = 0;
			g_Table_Z2_BitStatus[i][j] = 0;
		}
	}

	for (i = 0; i < 74; i++)
	{
		for (j = 0; j < 2; j++)
		{
			g_Table_MinEyeSize[i][j] = 0;
		}
	}

	for (i = 0; i < NUM_OF_LANES_TEST_SWEEP; i++)
	{
		for (j = 0; j < 8; j++)
		{
			g_PresetTrim[i][j] = 0;
		}
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        Sweep_DQn_Trim_l                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  DoCenter -                                                                             */
/*                  SweepBitMask -                                                                         */
/*                  SweepType -                                                                            */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS Sweep_DQn_Trim_l (DDR_Setup *ddr_setup, int SweepType, BOOLEAN DoCenter, UINT16 SweepBitMask)
{
	int		 samplePoint = 0;
	UINT32		 ilane ,ibit, ind;
	UINT32		 NewTrimReg;

	int startScan =((int)-62);
	int stopScan  =((int)62);

	int		 sum = 0;
	int		 Separation;

	DEFS_STATUS		 status = DEFS_STATUS_OK;

	switch (SweepType)
	{
		case SWEEP_OUT_DQ:
			HAL_PRINT_DBG("\n>Sweep Output DQn:");
			break;

		case SWEEP_IN_DQ:
			HAL_PRINT_DBG("\n>Sweep Input DQn:");
			break;

		case SWEEP_OUT_DM:
			HAL_PRINT_DBG("\n>Sweep Output DM:");
			break;

		default:
			HAL_PRINT_DBG(KRED "\n>Sweep_DQn_Trim_l ERROR: invalid sweep type. 0x%X\n" KNRM, SweepType);
			return DEFS_STATUS_FAIL;
	}

	if(ddr_setup != NULL)
	{
		if (!(ddr_setup->sweep_main_flow & (1 << SweepType))) {
			HAL_PRINT_DBG("\tSkip sweep\n");
			return DEFS_STATUS_OK;
		}
	}
	
	HAL_PRINT_DBG(" (SweepBitMask=0x%04X) \n", SweepBitMask);

	mc_init_sweep_arrays();

	/*---------------------------------------------------------------------------------------------------------*/
	/*      Get default values (of all lanes)                                                                  */
	/*---------------------------------------------------------------------------------------------------------*/
	for (ilane = 0 ; ilane < NUM_OF_LANES_TEST_SWEEP	 ; ilane++)
	{
		//HAL_PRINT_DBG("lane%u:", ilane);
		if ( SweepType == SWEEP_OUT_DM)
		{
			REG_WRITE(PHY_LANE_SEL, (ilane * DQS_DLY_WIDTH) + 0x800);
			UINT32 DM = READ_REG_FIELD(OP_DQ_DM_DQS_BITWISE_TRIM, OP_DQ_DM_DQS_BITWISE_TRIM_op_dq_dm_dqs_bitwise_trim_reg);
			for (ibit = 0; ibit < 8; ibit++)
				g_PresetTrim[ilane][ibit] = DM;
		}
		else
		{
			for (ibit = 0; ibit < 8; ibit++)
			{
				REG_WRITE(PHY_LANE_SEL,(ilane * DQS_DLY_WIDTH) + (ibit << 8));
				if ( SweepType == SWEEP_IN_DQ)
					g_PresetTrim[ilane][ibit] = READ_REG_FIELD(IP_DQ_DQS_BITWISE_TRIM, IP_DQ_DQS_BITWISE_TRIM_ip_dq_dqs_bitwise_trim_reg);
				else if (SweepType == SWEEP_OUT_DQ)
					g_PresetTrim[ilane][ibit] = READ_REG_FIELD(OP_DQ_DM_DQS_BITWISE_TRIM, OP_DQ_DM_DQS_BITWISE_TRIM_op_dq_dm_dqs_bitwise_trim_reg);
			}
		}
		//HAL_PRINT_DBG("\n");
	}

	/*---------------------------------------------------------------------------------------------------------*/
	/*      Sweep                                                                                              */
	/*---------------------------------------------------------------------------------------------------------*/
	for (samplePoint = startScan ; samplePoint <= stopScan; samplePoint++)
	{
		NewTrimReg = TWOS_COMP_7BIT_VALUE_TO_REG(samplePoint);

		// Update trim value into DQn and DM as needed
		for (ilane = 0; ilane < NUM_OF_LANES_TEST_SWEEP ; ilane++)
		{
			if (((SweepBitMask >> (ilane * 8))& 0xFF) == 0xFF)
				continue;

			if ( SweepType == SWEEP_OUT_DM)
			{
				REG_WRITE(PHY_LANE_SEL, ilane * DQS_DLY_WIDTH + 0x800);
				REG_WRITE(OP_DQ_DM_DQS_BITWISE_TRIM, 0x180 |  (0x7F & NewTrimReg));
			}
			else
			{
				for (ibit = 0; ibit < 8; ibit++)
				{
					if (((SweepBitMask >> (ilane * 8 + ibit))& 0x1) == 0x1)
						continue;

					REG_WRITE(PHY_LANE_SEL,(ilane * DQS_DLY_WIDTH) + (ibit << 8));
					if ( SweepType == SWEEP_IN_DQ)
						REG_WRITE(IP_DQ_DQS_BITWISE_TRIM, 0x80 |  (0x7F & NewTrimReg));
					else if ( SweepType == SWEEP_OUT_DQ)
						REG_WRITE(OP_DQ_DM_DQS_BITWISE_TRIM, 0x180 |  (0x7F & NewTrimReg));
				}
			}
		}

		// dummy accsess to DDR
		IOW32(0x1000, IOR32(0x1000));
		REG_WRITE(PHY_LANE_SEL, 0);

		g_fail_rate_0 = 0; // NTIL: Meir want to check failure rate
		g_fail_rate_1 = 0; // NTIL: Meir want to check failure rate
		UINT16 ber = MC_MemStressTestLong(FALSE, FALSE, SweepBitMask,FALSE);
		g_Table_Y_BitStatus[(samplePoint - startScan) % MC_DQ_DQS_BITWISE_TRIM_SAMPLING_POINTS_TEST] = ber;
		//if ((g_fail_rate_0 < 100) || (g_fail_rate_1 < 100) )
		//	HAL_PRINT_DBG(" val= %3d; ber=%04X; faliure rate: lane1= %u, lane0= %u \n", samplePoint,  ber, g_fail_rate_1, g_fail_rate_0);
	}

	//-------------------------------------
	HAL_PRINT_DBG("                                   lane1     lane0    \n");
	HAL_PRINT_DBG("                                   7... ...0 7... ...0\n");
	Separation = TRUE;
	for (samplePoint = startScan; samplePoint < stopScan; samplePoint++)
	{
		if ( g_Table_Y_BitStatus[(samplePoint - startScan)%MC_DQ_DQS_BITWISE_TRIM_SAMPLING_POINTS_TEST] != 0xFFFF)
		{
			HAL_PRINT_DBG(" reg= 0x%02X; val= %3d; bitwise ber= %u%u%u%u %u%u%u%u %u%u%u%u %u%u%u%u\n", (UINT8) TWOS_COMP_7BIT_VALUE_TO_REG(samplePoint),  samplePoint,
			BINARY_PRINT16(g_Table_Y_BitStatus[(samplePoint - startScan)%MC_DQ_DQS_BITWISE_TRIM_SAMPLING_POINTS_TEST]));
			Separation = FALSE;
		}
		else if (Separation == FALSE)
		{
			HAL_PRINT_DBG(" reg= 0x%02X; val= %3d; -----\n", (UINT8) TWOS_COMP_7BIT_VALUE_TO_REG(samplePoint),  samplePoint);
			Separation = TRUE;
		}
	}
	//-------------------------------------

	FindMidBiggestEyeBitwise_l(g_Table_Y_BitStatus, ARRAY_SIZE(g_Table_Y_BitStatus), g_Table_Y_BestTrim_EyeCenter, g_Table_Y_BestTrim_EyeSize, 16);

	/*-----------------------------------------------------------------------------------------------------*/
	/* Add startScan as on offset to best trim (if the step is different than 1 need to change this too)   */
	/*-----------------------------------------------------------------------------------------------------*/
	for(ibit = 0; ibit < 16; ibit++)
	{
		g_Table_Y_BestTrim_EyeCenter[ibit] = g_Table_Y_BestTrim_EyeCenter[ibit] + startScan /*-  2*/ ;  // needed for SVB // NTIL: removed the -2 for SVB !!!
	}

	/*---------------------------------------------------------------------------------------------------------*/
	/* Check results and print info	                                                                           */
	/*---------------------------------------------------------------------------------------------------------*/
	HAL_PRINT_DBG("\n");
	ilane = 0;

	for(ibit = 0; ibit < 16; ibit++)
	{
		if( ibit == 8)
		{
			ilane++;
			HAL_PRINT_DBG("\n");
		}

		if (((SweepBitMask >>ibit)& 0x1) == 0x1)
			continue;

		HAL_PRINT_DBG(" lane= %u; bit= %u; eye size= %2u; ", ilane, (ibit % 8), g_Table_Y_BestTrim_EyeSize[ibit]);

		if (DoCenter == TRUE)
		{
			/*---------------------------------------------------------------------------------------------*/
			/* Check if there is a valid eye (more then 4).	 If not: use the original value:               */
			/*---------------------------------------------------------------------------------------------*/
			if  (g_Table_Y_BestTrim_EyeSize[ibit] < 4) // NTIL: changed 5 to 4 for ATE
			{
				HAL_PRINT(KRED " eye center= %3d (Error: no eye center or eye size too small, use the origin value %3d)" KNRM, ilane, (ibit % 8), g_Table_Y_BestTrim_EyeSize[ibit], ( TWOS_COMP_7BIT_REG_TO_VALUE( g_PresetTrim[ilane][ibit % 8])));
				g_Table_Y_BestTrim_EyeCenter[ibit] = (TWOS_COMP_7BIT_REG_TO_VALUE( g_PresetTrim[ilane][ibit % 8]) );
				status = DEFS_STATUS_HARDWARE_ERROR;
			}
			else
			{
				HAL_PRINT_DBG(" eye center= %3d (origin val= %3d; diff= %3d)", g_Table_Y_BestTrim_EyeCenter[ibit], (TWOS_COMP_7BIT_REG_TO_VALUE( g_PresetTrim[ilane][ibit % 8])),	(g_Table_Y_BestTrim_EyeCenter[ibit] -  TWOS_COMP_7BIT_REG_TO_VALUE( g_PresetTrim[ilane][ibit % 8])));
			}
		}
		HAL_PRINT_DBG("\n");
	}
	HAL_PRINT_DBG("\n");

	/*---------------------------------------------------------------------------------------------------------*/
	/*    Set new values for trim or restore origin values                                                     */
	/*---------------------------------------------------------------------------------------------------------*/
	if ( SweepType == SWEEP_IN_DQ) // input sweep
	{
		for (ilane = 0; ilane < NUM_OF_LANES_TEST_SWEEP ; ilane++)
		{
			for (ibit = 0; ibit < 8; ibit++)
			{
				ind = ilane * 8 + ibit;
				if (g_Table_Y_BestTrim_EyeSize[ind] < 10) {
					HAL_PRINT_DBG(KRED "ERROR:   in DQn lane %d bit %d eye size %3d \n" KNRM, ilane, ibit, g_Table_Y_BestTrim_EyeSize[ind]);
				} else if (g_Table_Y_BestTrim_EyeSize[ind] >= 12) {
					HAL_PRINT_DBG(KGRN "PASS:    in DQn lane %d bit %d eye size %3d \n" KNRM, ilane, ibit, g_Table_Y_BestTrim_EyeSize[ind]);
				} else {
					HAL_PRINT_DBG(KYEL "WARNING: in DQn lane %d bit %d eye size %3d \n" KNRM, ilane, ibit, g_Table_Y_BestTrim_EyeSize[ind]);
				}
				
				if ( ((SweepBitMask >> ind ) & 0x1) == 0x1)
						continue;

				REG_WRITE(PHY_LANE_SEL,(ilane * DQS_DLY_WIDTH) + (ibit << 8));
				if (DoCenter == TRUE)
					REG_WRITE(IP_DQ_DQS_BITWISE_TRIM, (0x80 |  ( TWOS_COMP_7BIT_VALUE_TO_REG(g_Table_Y_BestTrim_EyeCenter[ilane * 8 + ibit]) )));
				else // restore origin value
					REG_WRITE(IP_DQ_DQS_BITWISE_TRIM, 0x80 | g_PresetTrim[ilane][ibit]);
			}
		}
	}
	else if ( SweepType == SWEEP_OUT_DQ) // output sweep
	{
		// on output: set the average since don't have per bit control:
		for (ilane = 0; ilane < NUM_OF_LANES_TEST_SWEEP; ilane++)
		{
			for (ibit = 0 ; ibit < 8 ; ibit++)
			{
				if (((SweepBitMask >> (ilane * 8 + ibit))& 0x1) == 0x1)
					continue;

				ind = (ilane * 8 + ibit) % 16;
				if (g_Table_Y_BestTrim_EyeSize[ind] < 10) {
					HAL_PRINT_DBG(KRED "ERROR:   out DQn lane %d bit %d eye size %3d \n" KNRM, ilane, ibit, g_Table_Y_BestTrim_EyeSize[ind]);
				} else if (g_Table_Y_BestTrim_EyeSize[ind] >= 12) {
					HAL_PRINT_DBG(KGRN "PASS:    out DQn lane %d bit %d eye size %3d \n" KNRM, ilane, ibit, g_Table_Y_BestTrim_EyeSize[ind]);
				} else {
					HAL_PRINT_DBG(KYEL "WARNING: out DQn lane %d bit %d eye size %3d \n" KNRM, ilane, ibit, g_Table_Y_BestTrim_EyeSize[ind]);
				}

				// Set the best values found by sweep back to the registers:
				REG_WRITE(PHY_LANE_SEL,(ilane * DQS_DLY_WIDTH) + (ibit << 8));
				if (DoCenter == TRUE)
					REG_WRITE(OP_DQ_DM_DQS_BITWISE_TRIM, 0x180 |  ( TWOS_COMP_7BIT_VALUE_TO_REG(g_Table_Y_BestTrim_EyeCenter[ind])));
				else // restore origin value
					REG_WRITE(OP_DQ_DM_DQS_BITWISE_TRIM,  0x180 | g_PresetTrim[ilane][ibit]);
			}
		}
	}
	else if ( SweepType == SWEEP_OUT_DM) // output DM
	{
		// on DM set the average DQn center eye to DM
		for (ilane = 0; ilane < NUM_OF_LANES_TEST_SWEEP; ilane++)
		{
			if (((SweepBitMask >> (ilane * 8))& 0xFF) == 0xFF)
				continue;

			sum = 0;
			for (ibit = 0 ; ibit < 8 ; ibit++)
				sum += g_Table_Y_BestTrim_EyeCenter[ilane * 8 + ibit];

			// NTIL: not good since sum is not the REG value !!!!! sum = DIV_ROUND(sum, 8); // sum is int, not UINT.. fixed around func; add +4  ( find the average of the bits)
			if (sum>0)
				sum = (sum + 4) / 8;
			else
				sum = (sum - 4) / 8;

			REG_WRITE(PHY_LANE_SEL, (ilane * DQS_DLY_WIDTH) + 0x800);
			if (DoCenter == TRUE)
			{
				REG_WRITE(OP_DQ_DM_DQS_BITWISE_TRIM, 0x180 | TWOS_COMP_7BIT_VALUE_TO_REG(sum) ); // override DM bit too.
				HAL_PRINT_DBG(" DM lane= %u; origin value= %3d; new value= %3d; diff= %3d \n", ilane, TWOS_COMP_7BIT_REG_TO_VALUE(g_PresetTrim[ilane][0]), sum, sum-TWOS_COMP_7BIT_REG_TO_VALUE(g_PresetTrim[ilane][0]));
			}
			else // restore origin value
				REG_WRITE(OP_DQ_DM_DQS_BITWISE_TRIM, 0x180 |  g_PresetTrim[ilane][0]);
		}
	}

	IOW32(0x1000, IOR32(0x1000));
	REG_WRITE(PHY_LANE_SEL, 0);

	return status;
}

DEFS_STATUS Sweep_DQS_Trim_l (DDR_Setup *ddr_setup, int SweepType, BOOLEAN DoCenter, UINT16 SweepBitMask)
{
	int		 samplePoint = 0;
	UINT32		 ilane ,ibit;
	UINT32		 NewTrimReg;

	int startScan =((int)0);
	int stopScan  =((int)63);

	int		 Separation;

	DEFS_STATUS		 status = DEFS_STATUS_OK;


	switch (SweepType)
	{
		case SWEEP_OUT_DQS:
			HAL_PRINT_DBG("\n>Sweep Output DQS:");
			break;

		case SWEEP_IN_DQS:
			HAL_PRINT_DBG("\n>Sweep Input DQS:");
			break;

		case SWEEP_OUT_LANE:
			HAL_PRINT_DBG("\n>Sweep Output TRIM_2:");
			break;

		default:
			HAL_PRINT_DBG(KRED "\n>ERROR: invalid sweep type.\n" KNRM);
			return DEFS_STATUS_FAIL;
	}

	if(ddr_setup != NULL)
	{
		if (!(ddr_setup->sweep_main_flow & (1 << SweepType))) {
			HAL_PRINT_DBG("\tSkip sweep\n");
			return DEFS_STATUS_OK;
		}
	}
	
	HAL_PRINT_DBG(" (SweepBitMask = 0x%04X) \n", SweepBitMask);

	mc_init_sweep_arrays ();

	/*---------------------------------------------------------------------------------------------------------*/
	/*      Get default values (of all lanes)                                                                  */
	/*---------------------------------------------------------------------------------------------------------*/
	for (ilane = 0 ; ilane < NUM_OF_LANES_TEST_SWEEP ; ilane++)
	{
		//HAL_PRINT_DBG_DBG("lane%u:", ilane);
		if ( SweepType == SWEEP_OUT_DQS)
		{
			REG_WRITE(PHY_LANE_SEL, (ilane * DQS_DLY_WIDTH) + 0x900);
			UINT32 DQS = READ_REG_FIELD(OP_DQ_DM_DQS_BITWISE_TRIM, OP_DQ_DM_DQS_BITWISE_TRIM_op_dq_dm_dqs_bitwise_trim_reg);
			g_PresetTrim[ilane][0] = DQS;
			startScan = 16;
			stopScan = 63-16;
		}
		else if (SweepType == SWEEP_IN_DQS)
		{
			REG_WRITE(PHY_LANE_SEL, (ilane * DQS_DLY_WIDTH) + 0x800);
			UINT32 DQS = READ_REG_FIELD(IP_DQ_DQS_BITWISE_TRIM, IP_DQ_DQS_BITWISE_TRIM_ip_dq_dqs_bitwise_trim_reg);
			g_PresetTrim[ilane][0] = DQS;
			startScan = 16;
			stopScan = 63-16;
		}
		else if (SweepType == SWEEP_OUT_LANE)
		{
			REG_WRITE(PHY_LANE_SEL, (ilane * SLV_DLY_WIDTH));
			UINT32 trim2 = READ_REG_FIELD(PHY_DLL_TRIM_2, PHY_DLL_TRIM_2_dlls_trim_2);
			g_PresetTrim[ilane][0] = trim2;
			startScan = 0;
			stopScan = 63;
		}

		//HAL_PRINT_DBG_DBG("\n");
	}

	/*---------------------------------------------------------------------------------------------------------*/
	/*      Sweep                                                                                              */
	/*---------------------------------------------------------------------------------------------------------*/
	for (samplePoint = startScan ; samplePoint <= stopScan; samplePoint++)
	{
		NewTrimReg = (UINT32)samplePoint;

		// Update trim value into DQn and DM as needed
		for (ilane = 0; ilane < NUM_OF_LANES_TEST_SWEEP ; ilane++)
		{
			if (((SweepBitMask >> (ilane * 8)) & 0xFF) == 0xFF)
				continue;

			if ( SweepType == SWEEP_OUT_DQS)
			{
				REG_WRITE(PHY_LANE_SEL, ilane * DQS_DLY_WIDTH + 0x900);
				REG_WRITE(OP_DQ_DM_DQS_BITWISE_TRIM, 0x180 |  (0x3F & NewTrimReg));
			}
			else if ( SweepType == SWEEP_IN_DQS)
			{
				REG_WRITE(PHY_LANE_SEL, ilane * DQS_DLY_WIDTH + 0x800);
				REG_WRITE(IP_DQ_DQS_BITWISE_TRIM, 0x80 |  (0x3F & NewTrimReg));
			}
			else if ( SweepType == SWEEP_OUT_LANE)
			{
				REG_WRITE(PHY_LANE_SEL, ilane * SLV_DLY_WIDTH);
				SET_REG_FIELD(PHY_DLL_TRIM_2, PHY_DLL_TRIM_2_dlls_trim_2, 0x3F & NewTrimReg);
			}
		}


		// dummy accsess to DDR
		IOW32(0x1000, IOR32(0x1000));
		REG_WRITE(PHY_LANE_SEL, 0);

		g_fail_rate_0 = 0; // NTIL: Meir want to check failure rate
		g_fail_rate_1 = 0; // NTIL: Meir want to check failure rate
		UINT16 ber = MC_MemStressTestLong(FALSE, FALSE, SweepBitMask, FALSE);
		g_Table_Y_BitStatus[(samplePoint - startScan) % MC_DQ_DQS_BITWISE_TRIM_SAMPLING_POINTS_TEST] = ber;

		// Output DQS, Input DQS and TRIM2 delays are set per lane and not per DQ therefore combine (merge) ber restouls of DQ0 to DQ7 and DQ8 to DQ15
		if ((ber & 0x00FF) != 0)
			ber |= 0x00FF;  // one for all. if one DQ bit in lane0 fail, mark all lane0 DQ as fail.
		if ((ber & 0xFF00) != 0)
			ber |= 0xFF00;  // one for all. if one DQ bit in lane1 fail, mark all lane1 DQ as fail.
		g_Table_Y_LaneStatus[(samplePoint - startScan) % MC_DQ_DQS_BITWISE_TRIM_SAMPLING_POINTS_TEST] = ber;

		//if ((g_fail_rate_0 < 100) || (g_fail_rate_1 < 100) )
		//	HAL_PRINT_DBG(" val= %3d; ber=%04X; faliure rate: lane1= %u, lane0= %u \n", samplePoint,  ber, g_fail_rate_1, g_fail_rate_0);
	}

	//-------------------------------------
	HAL_PRINT_DBG("                                   lane1     lane0    \n");
	HAL_PRINT_DBG("                                   7... ...0 7... ...0\n");
	Separation = TRUE;
	for (samplePoint = startScan; samplePoint < stopScan; samplePoint++)
	{
		if ( g_Table_Y_BitStatus[(samplePoint - startScan)%MC_DQ_DQS_BITWISE_TRIM_SAMPLING_POINTS_TEST] != 0xFFFF)
		{
			HAL_PRINT_DBG(" reg= 0x%02X; val= %3d; bitwise ber= %u%u%u%u %u%u%u%u %u%u%u%u %u%u%u%u\n", (UINT8)samplePoint,  samplePoint,
			BINARY_PRINT16(g_Table_Y_BitStatus[(samplePoint - startScan)%MC_DQ_DQS_BITWISE_TRIM_SAMPLING_POINTS_TEST]));
			Separation = FALSE;
		}
		else if (Separation == FALSE)
		{
			HAL_PRINT_DBG(" reg= 0x%02X; val= %3d; -----\n", (UINT8)samplePoint,  samplePoint);
			Separation = TRUE;
		}
	}
	//-------------------------------------

	FindMidBiggestEyeBitwise_l(g_Table_Y_LaneStatus, ARRAY_SIZE(g_Table_Y_LaneStatus), g_Table_Y_BestTrim_EyeCenter, g_Table_Y_BestTrim_EyeSize, 16);

	/*-----------------------------------------------------------------------------------------------------*/
	/* Add startScan as on offset to best trim (if the step is different than 1 need to change this too)   */
	/*-----------------------------------------------------------------------------------------------------*/
	for(ilane = 0; ilane < 2; ilane++)
	{
		g_Table_Y_BestTrim_EyeCenter[8*ilane] = g_Table_Y_BestTrim_EyeCenter [8*ilane] + startScan /*-  2*/ ;  // needed for SVB // NTIL removed the -2 for SVB
	}

	/*---------------------------------------------------------------------------------------------------------*/
	/* Check results and print info	                                                                           */
	/*---------------------------------------------------------------------------------------------------------*/
	HAL_PRINT_DBG("\n");

	for(ilane = 0; ilane < 2; ilane++)
	{

		if (((SweepBitMask >> (8*ilane))& 0xFF) == 0xFF)
			continue;

		HAL_PRINT_DBG(" lane= %u; eye size= %u;", ilane, g_Table_Y_BestTrim_EyeSize[8*ilane]);

		

		if (DoCenter == TRUE)
		{
			/*---------------------------------------------------------------------------------------------*/
			/* Check if there is a valid eye (more then 15).	 If not: use the original value:           */
			/*---------------------------------------------------------------------------------------------*/
			if  (g_Table_Y_BestTrim_EyeSize[8*ilane] < 15)
			{
				HAL_PRINT(KRED " eye center= %3d (Error: no eye center or eye size too small. Use the origin value %3d).\n" KNRM, g_Table_Y_BestTrim_EyeCenter[8*ilane], g_PresetTrim[ilane][0]);
				g_Table_Y_BestTrim_EyeCenter[8*ilane] = (( g_PresetTrim[ilane][0]) );
				status = DEFS_STATUS_HARDWARE_ERROR;
			}
			else
			{
				HAL_PRINT_DBG(" eye center= %3d (origin val= %3d; diff= %3d)", g_Table_Y_BestTrim_EyeCenter[8*ilane], (( g_PresetTrim[ilane][0])), (g_Table_Y_BestTrim_EyeCenter[8*ilane] -  ( g_PresetTrim[ilane][0])));
			}
		}
		HAL_PRINT_DBG("\n");
	}

	/*---------------------------------------------------------------------------------------------------------*/
	/*    Set new values for trim or restore origin values                                                     */
	/*---------------------------------------------------------------------------------------------------------*/
	if ( SweepType == SWEEP_OUT_DQS) // output DQS
	{
		// on DQS set the average DQn center eye to DQS
		for (ilane = 0; ilane < NUM_OF_LANES_TEST_SWEEP; ilane++)
		{

			if (((SweepBitMask >> (ilane * 8))& 0xFF) == 0xFF)
				continue;


			int New_Value = g_Table_Y_BestTrim_EyeCenter[ilane * 8]; // take the value of D0 and D8

			REG_WRITE(PHY_LANE_SEL, (ilane * DQS_DLY_WIDTH) + 0x900);
			if (DoCenter == TRUE)
				REG_WRITE(OP_DQ_DM_DQS_BITWISE_TRIM, 0x180 | New_Value& 0x3F ); // override DM bit too.
			else // restore origin value
				REG_WRITE(OP_DQ_DM_DQS_BITWISE_TRIM, 0x180 |  g_PresetTrim[ilane][0]);
		}
	}
	else if ( SweepType == SWEEP_IN_DQS) // output DQS
	{
		// on DQS set the average DQn center eye to DQS
		for (ilane = 0; ilane < NUM_OF_LANES_TEST_SWEEP; ilane++)
		{
			if (((SweepBitMask >> (ilane * 8))& 0xFF) == 0xFF)
				continue;

			int New_Value = g_Table_Y_BestTrim_EyeCenter[ilane * 8]; // take the value of D0 and D8

			if (g_Table_Y_BestTrim_EyeSize[8*ilane] < 20)  {
				HAL_PRINT_DBG(KRED "ERROR:   DQS lane%d eye size %3d \n" KNRM, ilane,  g_Table_Y_BestTrim_EyeSize[8*ilane]);
			} else if (g_Table_Y_BestTrim_EyeSize[8*ilane] >= 28){
				HAL_PRINT_DBG(KGRN "PASS:    DQS lane%d eye size %3d \n" KNRM, ilane, g_Table_Y_BestTrim_EyeSize[8*ilane]);
			} else {
				HAL_PRINT_DBG(KYEL "WARNING: DQS lane%d eye size %3d \n" KNRM, ilane, g_Table_Y_BestTrim_EyeSize[8*ilane]);
			}
			
			REG_WRITE(PHY_LANE_SEL, (ilane * DQS_DLY_WIDTH) + 0x800);
			if (DoCenter == TRUE)
				REG_WRITE(IP_DQ_DQS_BITWISE_TRIM, 0x80 | New_Value& 0x3F ); // override DM bit too.
			else // restore origin value
				REG_WRITE(OP_DQ_DM_DQS_BITWISE_TRIM, 0x80 |  g_PresetTrim[ilane][0]);
		}
	}
	else if ( SweepType == SWEEP_OUT_LANE) // output TRIM_2
	{
		// on TRIM_2 set the average DQn center eye to TRIM_2
		for (ilane = 0; ilane < NUM_OF_LANES_TEST_SWEEP; ilane++)
		{
			if (((SweepBitMask >> (ilane * 8))& 0xFF) == 0xFF)
				continue;

			if (g_Table_Y_BestTrim_EyeSize[8*ilane] < 20) {
				HAL_PRINT_DBG(KRED "ERROR:   TRIM2 lane%d eye size %3d \n" KNRM, ilane, g_Table_Y_BestTrim_EyeSize[8*ilane]);
			} else if (g_Table_Y_BestTrim_EyeSize[8*ilane] >= 30) {
				HAL_PRINT_DBG(KGRN "PASS:    TRIM2 lane%d eye size %3d \n" KNRM, ilane, g_Table_Y_BestTrim_EyeSize[8*ilane]);
			} else {
				HAL_PRINT_DBG(KYEL "WARNING: TRIM2 lane%d eye size %3d \n" KNRM, ilane, g_Table_Y_BestTrim_EyeSize[8*ilane]);
			}

			int New_Value = g_Table_Y_BestTrim_EyeCenter[ilane * 8]; // take the value of D0 and D8

			REG_WRITE(PHY_LANE_SEL, (ilane * SLV_DLY_WIDTH));
			if (DoCenter == TRUE)
				SET_REG_FIELD(PHY_DLL_TRIM_2, PHY_DLL_TRIM_2_dlls_trim_2, New_Value& 0x3F ); // override DM bit too.
			else // restore origin value
				SET_REG_FIELD(PHY_DLL_TRIM_2, PHY_DLL_TRIM_2_dlls_trim_2,  g_PresetTrim[ilane][0]);
		}
	}

	IOW32(0x1000, IOR32(0x1000));
	REG_WRITE(PHY_LANE_SEL, 0);

	return status;

}
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_MemStressTestLong                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  abort_on_error -                                                                       */
/*                  bECC -                                                                                 */
/*                  bQuick -                                                                               */
/*                  SweepBitMask -                                                                         */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
UINT16 MC_MemStressTestLong (BOOLEAN bECC, BOOLEAN bQuick, UINT16 SweepBitMask, BOOLEAN abort_on_error)
{
	UINT16  ber = 0;

	ber = SweepBitMask; // bits that are not of our interest, mark as fail so test will be complete faster


	const UINT32   aPtrn[] = {0x00FFFF00, 0xFF0000FF, 0xFFFF0000, 0x0000FFFF};
	const UINT32   iPtrnSize = 0x100;
	const UINT	   iWalkers = 0x10; // NTIL - there is no point to shift 0x0001 more then 16 steps (0 to 15);

	UINT32	 iCnt1 = 0;
	UINT32	 iCnt2 = 0;

	extern UINT32 BOOTBLOCK_IMAGE_START;
	extern UINT32 BOOTBLOCK_IMAGE_END;

	volatile UINT8   *dstAddrStart_U8  = (UINT8 *)0x1000;
	volatile UINT16  *dstAddrStart_U16 = (UINT16 *)0x1000;
	volatile UINT32  *dstAddrStart_U32 = (UINT32 *)0x1000;

	// dummy access to DRAM:
	IOW32(0x1000, IOR32(0x1000));
	REG_WRITE(PHY_LANE_SEL, 0);

	// clear all interrupt status bits:
	REG_WRITE(DENALI_CTL_145, 0xFFFFFFFF);
	REG_WRITE(DENALI_CTL_145, 0);

	/*-----------------------------------------------------------------------------------------------------*/
	/*                    Write Byte accsess test                                                                */
	/*-----------------------------------------------------------------------------------------------------*/
#if 1

	/*-----------------------------------------------------------------------------------------------------*/
	/*  Write pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart_U8 = (UINT8 *)0x1000;
	for( iCnt2 = 0; iCnt2 < iPtrnSize ; iCnt2 ++)
	{
		*dstAddrStart_U8 = 0xFF;
		dstAddrStart_U8++;
		*dstAddrStart_U8 = 0x00;
		dstAddrStart_U8++;
		*dstAddrStart_U8 = 0x00;
		dstAddrStart_U8++;
		*dstAddrStart_U8 = 0xFF;
		dstAddrStart_U8++;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/* Verify pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart_U32 = (UINT32 *)0x1000;
	for( iCnt2 = 0; iCnt2 < iPtrnSize ; iCnt2 ++)
	{
		UINT32 tmp_ber = *dstAddrStart_U32 ^ 0xFF0000FF;
		ber |= (UINT16) (tmp_ber>>0);
		ber |= (UINT16) (tmp_ber>>16);
		dstAddrStart_U32++;

		if (tmp_ber & 0x00FF)
			g_fail_rate_0++;  // counter is 256 if it fail all the time

		if (tmp_ber & 0xFF00)
			g_fail_rate_1++;  // counter is 256 if it fail all the time
	}

	if( ber == 0xFFFF)
	{
		return ber;
	}

#endif

	/*-----------------------------------------------------------------------------------------------------*/
	/*                    Fixed pattern test                                                               */
	/*-----------------------------------------------------------------------------------------------------*/
#if 1
	for (iCnt1 = 0; iCnt1 < ARRAY_SIZE(aPtrn); iCnt1++)
	{
		/*----------------------------------------------------------------------------------------------------*/
		/*  Write pattern                                                                                      */
		/*-----------------------------------------------------------------------------------------------------*/
		// memset((void *)dstAddrStart, (UINT8)aPtrn[iCnt1], iPtrnSize);
		dstAddrStart_U32 = (UINT32 *)0x1000;
		for( iCnt2 = 0; iCnt2 < iPtrnSize ; iCnt2 += 4)
		{
			*dstAddrStart_U32 = aPtrn[iCnt1];
			dstAddrStart_U32++;
		}

		/*-----------------------------------------------------------------------------------------------------*/
		/* Verify pattern                                                                                      */
		/*-----------------------------------------------------------------------------------------------------*/
		// NTIL: removed GetECCSyndrom. There was a bug in the origin code !!! the bug seen when changing aPtrn data from 0xFFFFFFFF to 0xFFFF0000
		dstAddrStart_U32 = (UINT32 *)0x1000;
		for( iCnt2 = 0; iCnt2 <  iPtrnSize ; iCnt2 += 4)
		{
			UINT32 tmp_ber = *dstAddrStart_U32 ^ aPtrn[iCnt1];
			ber |= (UINT16) (tmp_ber>>0);
			ber |= (UINT16) (tmp_ber>>16);

			dstAddrStart_U32++;
		}

		if((iCnt1 == 1)  && (bQuick == TRUE))
		{
			return ber;
		}

		if( ber == 0xFFFF)
		{
			return ber;
		}
	}
#endif

	/*-----------------------------------------------------------------------------------------------------*/
	/* Walking-1 pattern test                                                                              */
	/*-----------------------------------------------------------------------------------------------------*/
#if 1
	/*-----------------------------------------------------------------------------------------------------*/
	/*  Write pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart_U16 = (UINT16 *)0x1000;
	for( iCnt2 = 0; iCnt2 <  iWalkers ; iCnt2++)
	{
		*dstAddrStart_U16 = (0x0001 << (iCnt2));
		dstAddrStart_U16++;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/* Verify pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart_U16 = (UINT16 *)0x1000;
	for( iCnt2 = 0; iCnt2 <  iWalkers ; iCnt2++)
	{
		if (bECC == TRUE)
		{
			ber |= GetECCSyndrom(*dstAddrStart_U16);
		}
		else
		{
			ber |= *dstAddrStart_U16 ^ (0x0001 << (iCnt2));
		}
		dstAddrStart_U16++;
	}
	if( ber == 0xFFFF)
	{
		return ber;
	}
#endif

	/*-----------------------------------------------------------------------------------------------------*/
	/* Walking-0 pattern test                                                                              */
	/*-----------------------------------------------------------------------------------------------------*/
#if 1
	/*-----------------------------------------------------------------------------------------------------*/
	/*  Write pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart_U16 = (UINT16 *)0x1000;
	for( iCnt2 = 0; iCnt2 <  iWalkers ; iCnt2++)
	{
		*dstAddrStart_U16 = 0xFFFF - (0x0001 << (iCnt2));
		dstAddrStart_U16++;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/* Verify pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart_U16 = (UINT16 *)0x1000;
	for( iCnt2 = 0; iCnt2 <  iWalkers ; iCnt2++)
	{
		if (bECC == TRUE)
		{
			ber |= GetECCSyndrom(*dstAddrStart_U16);
		}
		else
		{
			ber |= *dstAddrStart_U16 ^ (0xFFFF - (0x0001 << (iCnt2)));
		}
		dstAddrStart_U16++;
	}
	if( ber == 0xFFFF)
	{
		return ber;
	}
#endif

	/*-----------------------------------------------------------------------------------------------------*/
	/*                   Walking dead pattern test                                                         */
	/*-----------------------------------------------------------------------------------------------------*/
#if 1
	/*-----------------------------------------------------------------------------------------------------*/
	/*  Write pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart_U16 = (UINT16 *)0x1000;
	for( iCnt2 = 0; iCnt2 <  (iWalkers*2) ; iCnt2++)
	{
		if ((iCnt2 % 2) == 0)
		{
			*dstAddrStart_U16 = 0xFFFF - (0x0001 << (iCnt2/2));
		}
		else
		{
			*dstAddrStart_U16 = (0x0001 << (iCnt2/2));
		}
		dstAddrStart_U16++;
	}


	/*-----------------------------------------------------------------------------------------------------*/
	/* Verify pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart_U16 = (UINT16 *)0x1000;
	for( iCnt2 = 0; iCnt2 <  (iWalkers*2) ; iCnt2++)
	{
		if (bECC == TRUE)
		{
			ber |= GetECCSyndrom(*dstAddrStart_U16);
		}
		else
		{
			if ((iCnt2 % 2) == 0)
			{
				ber |= *dstAddrStart_U16 ^ (0xFFFF - (0x0001 << (iCnt2/2)));
			}
			else
			{
				ber |= *dstAddrStart_U16 ^ (0x0001 << (iCnt2/2));
			}
		}
		dstAddrStart_U16++;
	}
	if( ber == 0xFFFF)
	{
		return ber;
	}
#endif

	/*-----------------------------------------------------------------------------------------------------*/
	/*                    Walking dead1 pattern test                                                       */
	/*-----------------------------------------------------------------------------------------------------*/
#if 1
	/*-----------------------------------------------------------------------------------------------------*/
	/*  Write pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart_U16 = (UINT16 *)0x1000;
	for( iCnt2 = 0; iCnt2 <  (iWalkers*2) ; iCnt2++)
	{
		if ((iCnt2 % 2) == 0)
		{
			*dstAddrStart_U16 = 0xFFFF;
		}
		else
		{
			*dstAddrStart_U16 = (0x0001 << (iCnt2/2));
		}
		dstAddrStart_U16++;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/* Verify pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart_U16 = (UINT16 *)0x1000;
	for( iCnt2 = 0; iCnt2 <  (iWalkers*2) ; iCnt2++)
	{
		if (bECC == TRUE)
		{
			ber |= GetECCSyndrom(*dstAddrStart_U16);
		}
		else
		{
			if ((iCnt2 % 2) == 0)
			{
				ber |= *dstAddrStart_U16 ^ 0xFFFF;
			}
			else
			{
				ber |= *dstAddrStart_U16 ^ (0x0001 << (iCnt2/2));
			}
		}

		dstAddrStart_U16++;
	}
	if( ber == 0xFFFF)
	{
		return ber;
	}
#endif

	/*-----------------------------------------------------------------------------------------------------*/
	/*                     Walking dead2 pattern test                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
#if 1
	/*-----------------------------------------------------------------------------------------------------*/
	/*  Write pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart_U16 = (UINT16 *)0x1000;
	for( iCnt2 = 0; iCnt2 <  (iWalkers*2) ; iCnt2++)
	{
		if ((iCnt2 % 2) == 0)
		{
			*dstAddrStart_U16 = 0xFFFF - (0x0001 << (iCnt2/2));
		}
		else
		{
			*dstAddrStart_U16 = 0;
		}
		dstAddrStart_U16++;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/* Verify pattern                                                                                      */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart_U16 = (UINT16 *)0x1000;
	for( iCnt2 = 0; iCnt2 <  (iWalkers*2) ; iCnt2++)
	{
		if (bECC == TRUE)
		{
			ber |= GetECCSyndrom(*dstAddrStart_U16);
		}
		else
		{
			if ((iCnt2 % 2) == 0)
			{
				ber |= *dstAddrStart_U16 ^ (0xFFFF - (0x0001 << (iCnt2/2)));
			}
			else
			{
				ber |= *dstAddrStart_U16 ^ 0;
			}
		}
		dstAddrStart_U16++;
	}
	if( ber == 0xFFFF)
	{
		return ber;
	}
#endif

	/*-----------------------------------------------------------------------------------------------------*/
	/*                     Copy bootblock code from SRAM to DRAM                                           */
	/*-----------------------------------------------------------------------------------------------------*/
#if 1

	// NTIL: move bootblock test to last test since this test take too much time (first it copy then it check)
	/*-----------------------------------------------------------------------------------------------------*/
	/*  Use RAM2 as a pseudo random pattern to test the DDR                                                */
	/*  Copy BB code segment to the DDR                                                                    */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart_U16 = (UINT16 *)0x1000;

	for( iCnt2 = 0; iCnt2 < 0x10000 /* (&(UINT32)BOOTBLOCK_IMAGE_END - RAM2_MEMORY_SIZE)*/ ; iCnt2 += sizeof(UINT16))
	{
		*dstAddrStart_U16 = *(UINT16*)(RAM2_BASE_ADDR + iCnt2);
		dstAddrStart_U16++;
	}

	/*-----------------------------------------------------------------------------------------------------*/
	/* Compare BB code segment to the DDR                                                                  */
	/*-----------------------------------------------------------------------------------------------------*/
	dstAddrStart_U16 = (UINT16 *)0x1000;

	for( iCnt2 = 0; iCnt2 < 0x10000 /* (&(UINT32)BOOTBLOCK_IMAGE_END - RAM2_MEMORY_SIZE)  */ ; iCnt2 += sizeof(UINT16))
	{
		if (bECC == TRUE)
		{
			ber |= GetECCSyndrom(*(UINT16*)(RAM2_BASE_ADDR + iCnt2));
		}
		else
		{
			ber |= *dstAddrStart_U16 ^ *(UINT16*)(RAM2_BASE_ADDR + iCnt2);
		}

		// NTIL: save time and return as soos as posiable when there is total bits error (don't wait to verify to complete)
		if( ber == 0xFFFF)
			return ber;

		dstAddrStart_U16++;
	}
#endif

	return ber;


}




/*---------------------------------------------------------------------------------------------------------*/
/* Function:        Run_Memory_SI_Test                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ddr_setup -                                                                            */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void Run_Memory_SI_Test (DDR_Setup *ddr_setup)
{

	UINT16 ber;
	DEFS_STATUS       status =	DEFS_STATUS_OK;
	int               ilane, ibit ;

	if (ddr_setup->sweep_debug == 0)
		return;

	HAL_PRINT ("\n");
	HAL_PRINT(KCYN "=========================  \n" KNRM);
	HAL_PRINT(KCYN "   Memory SI Tests         \n" KNRM);
	HAL_PRINT(KCYN "========================== \n" KNRM);

	if (ddr_setup->sweep_debug & ADRCTL_MA_SWEEP)
		Sweep_adrctrl_ma_l();

	if (ddr_setup->sweep_debug & ADRCTL_SWEEP) //  adrctrl sweep
		Sweep_adrctrl_l         ();

	if (ddr_setup->sweep_debug & TRIM_2_LANE0_SWEEP) // trim_2 lane 0 sweep
		Sweep_trim2_lane_l(0);

	if (ddr_setup->sweep_debug & TRIM_2_LANE1_SWEEP) // trim_2 lane 1 sweep
		Sweep_trim2_lane_l(1);

	if (ddr_setup->sweep_debug & VREF_SWEEP) // BMC VREF sweep
		Sweep_VREF_l();

	if (ddr_setup->sweep_debug & DRAM_SWEEP) // DRAM VREF sweep
		Sweep_DRAM_l(ddr_setup);

	if (ddr_setup->sweep_debug & IP_DQS_SWEEP)
		Sweep_IP_DQS_l();

	if (ddr_setup->sweep_debug & OP_DQS_SWEEP)
		Sweep_OP_DQS_l();

	HAL_PRINT_DBG(KCYN "\n\n **** THE END **** \n\n");
}



#ifdef ADRCTL_MA_SWEEP
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        Sweep_adrctrl_ma_                                                                      */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS          Sweep_adrctrl_ma_l (void)
{


		volatile UINT16 ber ;
		HAL_PRINT ("\n");
		HAL_PRINT(KCYN "=========================  \n" KNRM);
		HAL_PRINT(KCYN "   adrctrl_ma sweep        \n" KNRM);
		HAL_PRINT(KCYN "========================== \n" KNRM);

		// check DRAM state with origin value before start the sweep, hoping DRAM is still working well
		g_fail_rate_0 = 0;
		g_fail_rate_1 = 0;
#if 0
		HAL_PRINT_DBG(KRED "Start \n" KNRM );
		MC_PrintPhy();
#endif
		ber = MC_MemStressTestLong(FALSE, FALSE, 0,TRUE);
		if (ber != 0)
		{
			HAL_PRINT_DBG(KRED" DRAM state before sweep: PHY_DLL_RECALIB = 0x%08lx; ber=%04X; fail_rate: lane1= %u, lane0= %u \n" KNRM, REG_READ (PHY_DLL_RECALIB), ber, g_fail_rate_1, g_fail_rate_0);
			HAL_PRINT_DBG(KRED "DRAM state is not functional at this point. Restart boot-block. \n" KNRM);
			while (1);
		}

		// Get origin value
		REG_WRITE(PHY_LANE_SEL, 0);
		volatile UINT32 stored_adrctrl_ma      = READ_REG_FIELD (PHY_DLL_RECALIB, PHY_DLL_RECALIB_dlls_trim_adrctrl_ma);
		volatile UINT32 stored_adrctrl_ma_lncr = READ_REG_FIELD (PHY_DLL_RECALIB, PHY_DLL_RECALIB_incr_dly_adrctrl_ma);

		// conver to reg value to int value
		volatile int stored_adrctrl_ma_int = stored_adrctrl_ma;
		if (stored_adrctrl_ma_lncr == 0)
			stored_adrctrl_ma_int *= -1;

		// sweep up from the current value
		HAL_PRINT_DBG(" Sweep_Up: start_value = %3d. \n", stored_adrctrl_ma_int);
		for (int current_val_int = stored_adrctrl_ma_int; current_val_int<=63; current_val_int++)
		{
			// set new value
			SET_REG_FIELD (PHY_DLL_RECALIB, PHY_DLL_RECALIB_dlls_trim_adrctrl_ma, (UINT32)(abs(current_val_int)));
			if (current_val_int>0) // 1-ince; 0-deg
				SET_REG_FIELD (PHY_DLL_RECALIB, PHY_DLL_RECALIB_incr_dly_adrctrl_ma, (UINT32)1); // 1-ince; 0-deg
			else
				SET_REG_FIELD (PHY_DLL_RECALIB, PHY_DLL_RECALIB_incr_dly_adrctrl_ma, (UINT32)0); // 1-ince; 0-deg

			// run memory test and log
			g_fail_rate_0 = 0;
			g_fail_rate_1 = 0;
			UINT16 ber = MC_MemStressTestLong(FALSE, FALSE, 0,FALSE);
			HAL_PRINT_DBG(" val= %3d; ber=%04X; fail_rate: lane1= %u, lane0= %u \n", current_val_int, ber, g_fail_rate_1, g_fail_rate_0);

			// exit of the first error to avoid DRAM stuckness
			if (ber != 0)
				break;
		}

		// restore the origin value
		REG_WRITE(PHY_LANE_SEL, 0);
		SET_REG_FIELD (PHY_DLL_RECALIB, PHY_DLL_RECALIB_dlls_trim_adrctrl_ma, stored_adrctrl_ma);
		SET_REG_FIELD (PHY_DLL_RECALIB, PHY_DLL_RECALIB_incr_dly_adrctrl_ma, stored_adrctrl_ma_lncr);
#if 0
		HAL_PRINT_DBG(KRED "Stage 1 \n" KNRM );
		MC_PrintPhy();
#endif
		// check DRAM state after restoring origin value, hoping DRAM is still working well
		// if DRAM stuck, no point to continue any test !
		g_fail_rate_0 = 0;
		g_fail_rate_1 = 0;
		ber = MC_MemStressTestLong(FALSE, FALSE, 0,TRUE);
		if (ber!=0)
		{
			#if 0
			HAL_PRINT_DBG(KRED "after fail 1 \n" KNRM );
			MC_PrintPhy();
			#endif
			HAL_PRINT_DBG(KRED "DRAM state after sweep: PHY_DLL_RECALIB = 0x%08lx; ber=%04X; fail_rate: lane1= %u, lane0= %u \n" KNRM, REG_READ (PHY_DLL_RECALIB), ber, g_fail_rate_1, g_fail_rate_0);
			HAL_PRINT_DBG(KRED "DRAM state is not functional at this point. Restart boot-block. \n" KNRM);
			while (1);
		}
		#if 0
		HAL_PRINT_DBG(KRED "After test stage 1  \n" KNRM );
			MC_PrintPhy();
		#endif
		//------------------------
		// sweep down from the origin value
		HAL_PRINT_DBG(" Sweep_Down: start_value = %3d \n", stored_adrctrl_ma_int);
		for (int current_val_int = stored_adrctrl_ma_int; current_val_int>-63; current_val_int--)
		{
			//if (current_val_int == -7)
			//	break;
			// set new value
			SET_REG_FIELD (PHY_DLL_RECALIB, PHY_DLL_RECALIB_dlls_trim_adrctrl_ma, (UINT32)(abs(current_val_int)));
			if (current_val_int>0) // 1-ince; 0-deg
				SET_REG_FIELD (PHY_DLL_RECALIB, PHY_DLL_RECALIB_incr_dly_adrctrl_ma, (UINT32)1); // 1-ince; 0-deg
			else
				SET_REG_FIELD (PHY_DLL_RECALIB, PHY_DLL_RECALIB_incr_dly_adrctrl_ma, (UINT32)0); // 1-ince; 0-deg

			// run mempory tets and log
			g_fail_rate_0 = 0;
			g_fail_rate_1 = 0;
			ber = MC_MemStressTestLong(FALSE, FALSE, 0,FALSE);
			HAL_PRINT_DBG(" val= %3d; ber=%04X; fail_rate: lane1= %u, lane0= %u \n", current_val_int, ber, g_fail_rate_1, g_fail_rate_0);

			// exit of the first error to avoid DRAM stuckness
			if (ber != 0)
			{
				ber = 0;
				break;
			}
		}
		#if 0
		HAL_PRINT_DBG(KRED "after stage 2 before restore\n" KNRM );
		MC_PrintPhy();
		#endif
		// restore the origin value
		REG_WRITE(PHY_LANE_SEL, 0);
		SET_REG_FIELD (PHY_DLL_RECALIB, PHY_DLL_RECALIB_dlls_trim_adrctrl_ma, stored_adrctrl_ma);
		SET_REG_FIELD (PHY_DLL_RECALIB, PHY_DLL_RECALIB_incr_dly_adrctrl_ma, stored_adrctrl_ma_lncr);
		#if 0
		HAL_PRINT_DBG(KRED "after stage 2 after restore\n" KNRM );
		MC_PrintPhy();
		#endif
		// check DRAM state after restoring origin value, hoping DRAM is still working well
		// if DRAM stuck, no point to continue any test !
		g_fail_rate_0 = 0;
		g_fail_rate_1 = 0;
		ber = MC_MemStressTestLong(FALSE, FALSE, 0,TRUE);

		if (ber!=0)
		{
			HAL_PRINT_DBG(KRED "after fail 2 \n" KNRM );
			HAL_PRINT_DBG(KRED "DRAM state after sweep: PHY_DLL_RECALIB = 0x%08lx; ber=%04X; fail_rate: lane1= %u, lane0= %u \n" KNRM, REG_READ (PHY_DLL_RECALIB), ber, g_fail_rate_1, g_fail_rate_0);
			HAL_PRINT_DBG(KRED "DRAM state is not functional at this point. Restart boot-block. \n" KNRM);
			while (1);
		}
		//-------------------------------------------------------------------------------------------------------------

		return DEFS_STATUS_OK;
}

#endif //ADRCTL_MA_SWEEP

#ifdef ADRCTL_SWEEP
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        Sweep_adrctrl_l                                                                        */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS           Sweep_adrctrl_l (void)
{
		UINT16 ber;

		HAL_PRINT ("\n");
		HAL_PRINT(KCYN "=========================  \n" KNRM);
		HAL_PRINT(KCYN "   adrctrl sweep           \n" KNRM);
		HAL_PRINT(KCYN "========================== \n" KNRM);

		// check DRAM state with origin value before start the sweep, hoping DRAM is still working well
		g_fail_rate_0 = 0;
		g_fail_rate_1 = 0;
		ber = MC_MemStressTestLong(FALSE, FALSE, 0,FALSE);
		if (ber != 0)
		{
			HAL_PRINT_DBG(KRED " DRAM state before sweep: PHY_DLL_ADRCTRL = 0x%08lx; ber=%04X; fail_rate: lane1= %u, lane0= %u \n" KNRM, REG_READ (PHY_DLL_ADRCTRL), ber, g_fail_rate_1, g_fail_rate_0);
			HAL_PRINT_DBG(KRED " DRAM state is not functional at this point. Restart boot-block. \n" KNRM);
			while (1);
		}

		// Get the origin value
		REG_WRITE(PHY_LANE_SEL, 0);
		UINT32 stored_adrctrl      = READ_REG_FIELD (PHY_DLL_ADRCTRL, PHY_DLL_ADRCTRL_dlls_trim_adrctrl);
		UINT32 stored_adrctrl_lncr = READ_REG_FIELD (PHY_DLL_ADRCTRL, PHY_DLL_ADRCTRL_incr_dly_adrctrl);

		// conver to reg value to int value
		int stored_adrctrl_int = stored_adrctrl;
		if (stored_adrctrl_lncr == 0)
			stored_adrctrl_int *= -1;

		// sweep up from the origin value
		HAL_PRINT_DBG(" Sweep_Up: start_value = %3d \n", stored_adrctrl_int);
		for (int current_val_int = stored_adrctrl_int; current_val_int<=63; current_val_int++)
		{
			// set new value
			SET_REG_FIELD (PHY_DLL_ADRCTRL, PHY_DLL_ADRCTRL_dlls_trim_adrctrl, (UINT32)(abs(current_val_int)));
			if (current_val_int>0) // 1-ince; 0-deg
				SET_REG_FIELD (PHY_DLL_ADRCTRL, PHY_DLL_ADRCTRL_incr_dly_adrctrl, (UINT32)1); // 1-ince; 0-deg
			else
				SET_REG_FIELD (PHY_DLL_ADRCTRL, PHY_DLL_ADRCTRL_incr_dly_adrctrl, (UINT32)0); // 1-ince; 0-deg

			// run memory test and log
			g_fail_rate_0 = 0;
			g_fail_rate_1 = 0;
			ber = MC_MemStressTestLong(FALSE, FALSE, 0,FALSE);
			HAL_PRINT_DBG(" val= %3d; ber=%04X; fail_rate: lane1= %u, lane0= %u \n", current_val_int, ber, g_fail_rate_1, g_fail_rate_0);

			// exit at first error to avoid DRAM stuckness
			if (ber != 0)
				break;
		}
		if (ber==0)
			HAL_PRINT_DBG(" --- reached delay-line limit --- \n");


		// restore the origin value
		HAL_PRINT_DBG("\n restore the origin values %3d. \n", stored_adrctrl_int);
		REG_WRITE(PHY_LANE_SEL, 0);
		SET_REG_FIELD (PHY_DLL_ADRCTRL, PHY_DLL_ADRCTRL_dlls_trim_adrctrl, stored_adrctrl);
		SET_REG_FIELD (PHY_DLL_ADRCTRL, PHY_DLL_ADRCTRL_incr_dly_adrctrl, stored_adrctrl_lncr);

		// check DRAM state after restoring origin value, hoping DRAM is still working well
		// if DRAM stuck, no point to continue any test !
		g_fail_rate_0 = 0;
		g_fail_rate_1 = 0;
		ber = MC_MemStressTestLong(FALSE, FALSE, 0,FALSE);
		if (ber != 0)
		{
			HAL_PRINT_DBG(KRED " DRAM state after sweep: PHY_DLL_ADRCTRL = 0x%08lx; ber=%04X; fail_rate: lane1= %u, lane0= %u \n" KNRM, REG_READ (PHY_DLL_ADRCTRL), ber, g_fail_rate_1, g_fail_rate_0);
			HAL_PRINT_DBG(KRED " DRAM state is not functional at this point. Restart boot-block. \n" KNRM);
			while (1);
		}

		// sweep down from origin value
		HAL_PRINT_DBG(" Sweep_Down: start_value = %3d \n", stored_adrctrl_int);

		// get 1/4 clock cycle (dll_slv_dly_wire) value since TRIM2 is limied by -1/4 clock (the max negative value).
		int dll_slv_dly = -1 * (int) READ_REG_FIELD (PHY_DLL_ADRCTRL, PHY_DLL_ADRCTRL_dll_slv_dly);
		for (int current_val_int = stored_adrctrl_int; current_val_int> MAX(dll_slv_dly,-63); current_val_int--)
		{
			// set new value
			SET_REG_FIELD (PHY_DLL_ADRCTRL, PHY_DLL_ADRCTRL_dlls_trim_adrctrl, (UINT32)(abs(current_val_int)));
			if (current_val_int>0)
				SET_REG_FIELD (PHY_DLL_ADRCTRL, PHY_DLL_ADRCTRL_incr_dly_adrctrl, (UINT32)1);  // 1-ince; 0-deg
			else
				SET_REG_FIELD (PHY_DLL_ADRCTRL, PHY_DLL_ADRCTRL_incr_dly_adrctrl, (UINT32)0);  // 1-ince; 0-deg

			// run memory test and log
			g_fail_rate_0 = 0;
			g_fail_rate_1 = 0;
			ber = MC_MemStressTestLong(FALSE, FALSE, 0,FALSE);
			HAL_PRINT_DBG(" val= %3d; ber=%04X; fail_rate: lane1= %u, lane0= %u \n", current_val_int, ber, g_fail_rate_1, g_fail_rate_0);

			// exit at first error to avoid DRAM stuckness (if not already happend)
			if (ber != 0)
				break;
		}
		if (ber==0)
			HAL_PRINT_DBG(" --- reached delay-line limit --- \n");

		// restore the origin value
		HAL_PRINT_DBG("\n restore the origin values %3d. \n", stored_adrctrl_int);
		REG_WRITE(PHY_LANE_SEL, 0);
		SET_REG_FIELD (PHY_DLL_ADRCTRL, PHY_DLL_ADRCTRL_dlls_trim_adrctrl, stored_adrctrl);
		SET_REG_FIELD (PHY_DLL_ADRCTRL, PHY_DLL_ADRCTRL_incr_dly_adrctrl, stored_adrctrl_lncr);

		// check DRAM state after restoring origin value, hoping DRAM is still working well
		// if DRAM stuck, no point to continue any test !
		g_fail_rate_0 = 0;
		g_fail_rate_1 = 0;
		ber = MC_MemStressTestLong(FALSE, FALSE, 0,FALSE);
		if (ber !=0)
		{
			HAL_PRINT_DBG(KRED " DRAM state after sweep: PHY_DLL_ADRCTRL = 0x%08lx; ber=%04X; fail_rate: lane1= %u, lane0= %u \n" KNRM, REG_READ (PHY_DLL_ADRCTRL), ber, g_fail_rate_1, g_fail_rate_0);
			HAL_PRINT_DBG(KRED " DRAM state is not functional at this point. Restart boot-block. \n" KNRM);
			while (1);
		}
		//-------------------------------------------------------------------------------------------------------------
}


#endif //ADRCTL_SWEEP

#define TRIM_2_SWEEP
#ifdef TRIM_2_SWEEP
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        Sweep_trim2_lane_l                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  TRIM2_ilane -                                                                          */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS           Sweep_trim2_lane_l         (UINT16 TRIM2_ilane)
{
		//#define TRIM2_ilane 0
		UINT16 ber;
		UINT32		 ilane ,ibit;

		HAL_PRINT ("\n");
		HAL_PRINT(KCYN "=========================  \n" KNRM);
		HAL_PRINT(KCYN "   lane %u trim_2 sweep     \n" KNRM, TRIM2_ilane);
		HAL_PRINT(KCYN "========================== \n" KNRM);
		HAL_PRINT(" Sweep only lane %u trim_2 while keep the other lane at default. \n", TRIM2_ilane);

		// check DRAM state with origin value before start the sweep, hoping DRAM is still working well
		g_fail_rate_0 = 0;
		g_fail_rate_1 = 0;
		ber = MC_MemStressTestLong(FALSE, FALSE, 0,FALSE);
		if (ber != 0)
		{
			REG_WRITE(PHY_LANE_SEL, (TRIM2_ilane * SLV_DLY_WIDTH));
			HAL_PRINT_DBG(KRED " DRAM state before sweep:  lane= %u; PHY_DLL_TRIM_2= 0x%08lx; ber=%04X; fail_rate: lane1= %u, lane0= %u \n" KNRM, TRIM2_ilane, REG_READ (PHY_DLL_TRIM_2), ber, g_fail_rate_1, g_fail_rate_0);
			HAL_PRINT_DBG(KRED " DRAM state is not functional at this point. Restart boot-block. \n" KNRM);
		}

		// Get the origin value
		REG_WRITE(PHY_LANE_SEL, (TRIM2_ilane * SLV_DLY_WIDTH));
		UINT32 volatile stored_trim_2  = READ_REG_FIELD (PHY_DLL_TRIM_2, PHY_DLL_TRIM_2_dlls_trim_2);

		// start sweep trim2 lane0 while lane1 is fixed to default
		for (UINT32 current_trim_2=0; current_trim_2<=63; current_trim_2++)
		{
			// set new value
			HAL_PRINT_DBG(KCYN " \n*** Set  lane= %u; PHY_DLL_TRIM_2= %u; **** \n" KNRM, TRIM2_ilane, current_trim_2);
			REG_WRITE(PHY_LANE_SEL, (TRIM2_ilane * SLV_DLY_WIDTH));
			SET_REG_FIELD (PHY_DLL_TRIM_2, PHY_DLL_TRIM_2_dlls_trim_2, current_trim_2);

			// run DQ sweep
			Sweep_DQn_Trim_l(NULL, SWEEP_OUT_DQ, FALSE, 0xFF00 /*0*/); // Run Parametric Sweep on WRITE side (Output DQn), only on lane0

			// check min eye size for the all lane0
			int DQ_min_eyesize_lane0 = 255;
			for (ibit = 0; ibit < 8; ibit++)
				if (g_Table_Y_BestTrim_EyeSize[ibit]<DQ_min_eyesize_lane0)
					DQ_min_eyesize_lane0 = g_Table_Y_BestTrim_EyeSize[ibit];

			/* we sweep only lane0 so no need to monitor lane1 eye
			int DQ_min_eyesize_lane1 = 255;
			for (ibit = 8; ibit < 16; ibit++)
				if (g_Table_Y_BestTrim_EyeSize[ibit]<DQ_min_eyesize_lane1)
					DQ_min_eyesize_lane1 = g_Table_Y_BestTrim_EyeSize[ibit];
			*/

			HAL_PRINT_DBG(KCYN "\n > TRIM_2.lane%u: %u; DQ lane0 min eye :%u;\n\n" KNRM, TRIM2_ilane, current_trim_2, DQ_min_eyesize_lane0);

			// store Sweep_DQs_Trim resoult - only sample point for -20 to +20 range that is 42 to 81.
			for (int samplePoint = 0; samplePoint < 40; samplePoint++)
				g_Table_Z_BitStatus	[samplePoint][current_trim_2] = g_Table_Y_BitStatus [samplePoint + 42];

			// store min eye size
			g_Table_MinEyeSize [current_trim_2][0] = DQ_min_eyesize_lane0;

		}

		//-----------------------------------
		// restore origin SCL trim_2 value
		HAL_PRINT_DBG(KCYN " > Restore SCL trim_2: %u  \n" KNRM, stored_trim_2);
		REG_WRITE(PHY_LANE_SEL, (TRIM2_ilane * SLV_DLY_WIDTH));
		SET_REG_FIELD (PHY_DLL_TRIM_2, PHY_DLL_TRIM_2_dlls_trim_2, stored_trim_2);

		//--------------------------------------------------------------
		// display table of min eye size
		HAL_PRINT_DBG(KCYN "\n\n *** Summary table of min eye size of lane0 vs TRIM2.lane0  **** \n" KNRM);
		HAL_PRINT_DBG(KCYN " *** Note: TRIM2.lane0 tuning value is mark with with '*' \n\n" KNRM);
		for (UINT32 current_trim_2=0; current_trim_2<=63; current_trim_2++)
		{
			HAL_PRINT_DBG(KCYN "TRIM2.lane0: %02u;", current_trim_2);
			int min_eyesize = g_Table_MinEyeSize[current_trim_2][0];
			if ( current_trim_2 == stored_trim_2){
				HAL_PRINT_DBG(KCYN " * ");
			} else if (min_eyesize<5) {
				HAL_PRINT_DBG(KRED "   ");
			} else if (min_eyesize>8) {
				HAL_PRINT_DBG(KGRN "   ");
			} else {
				HAL_PRINT_DBG(KNRM "   ");
			}

			HAL_PRINT_DBG("Min Eye Lane0: %u;    ",  min_eyesize);

			HAL_PRINT_DBG(KNRM "\n");
		}


		// display eye (DQ vs TRIM_2) for each bit.
		for (int ibit = 0; ibit < 8/*16*/; ibit++) // display on lane0 eye, the critical issue we have. no issue was found in lane1
		{
			HAL_PRINT_DBG(KCYN "\n\n *** DQ%u write side eye mask (X: OUT_DQn delay; Y: TRIM2 lane%u delay) **** \n\n" KNRM, ibit, TRIM2_ilane);
			HAL_PRINT_DBG("   -20-19-18-17-16-15-14-13-12-11-10-09-08-07-06-05-04-03-02-01 | +01+02+03+04+05+06+07+08+09+10+11+12+13+14+15+16+17+18+19");

			// get trim2 delay
			REG_WRITE(PHY_LANE_SEL, (UINT32)ibit>>3/*ilane */ * SLV_DLY_WIDTH);
			int center_point_y = (int) READ_REG_FIELD (PHY_DLL_TRIM_2, PHY_DLL_TRIM_2_dlls_trim_2);

			// get OUT_DQn delay
			REG_WRITE(PHY_LANE_SEL,((UINT32)ibit>>3/*ilane */ * DQS_DLY_WIDTH) + (((UINT32)ibit& 0x7) << 8));
			int center_point_x = TWOS_COMP_7BIT_REG_TO_VALUE (READ_REG_FIELD(OP_DQ_DM_DQS_BITWISE_TRIM, OP_DQ_DM_DQS_BITWISE_TRIM_op_dq_dm_dqs_bitwise_trim_reg)) + 62 -42 ;

			for (int current_trim_2=63; current_trim_2>=0; current_trim_2--)
			{
				HAL_PRINT_DBG(KCYN "\n%02u ", current_trim_2);
				for (int samplePoint = 0; samplePoint < 40; samplePoint++)
				{
					UINT16 BitStatus = g_Table_Z_BitStatus [samplePoint][current_trim_2];
					if ((BitStatus >> ibit) & 0x1)
					{
						if ((current_trim_2 == center_point_y) && (samplePoint == center_point_x)) {
							HAL_PRINT_DBG(KCYN "@-@");
						} else {
							HAL_PRINT_DBG(KRED " - ");
						}
					}
					else
					{
						if ((current_trim_2 == center_point_y) && (samplePoint == center_point_x)) {
							HAL_PRINT_DBG(KCYN "@V@");
						} else {
							HAL_PRINT_DBG(KGRN " V ");
						}
					}
			  }
			}
		}
		HAL_PRINT_DBG(KNRM "\n");
		//----------------------------------------------------------------------------------------

		// check DRAM state after restoring origin value, hoping DRAM is still working well
		// if DRAM stuck, no point to continue any test !
		g_fail_rate_0 = 0;
		g_fail_rate_1 = 0;
		ber = MC_MemStressTestLong(FALSE, FALSE, 0,FALSE);
		if (ber != 0)
		{
			REG_WRITE(PHY_LANE_SEL, (TRIM2_ilane * SLV_DLY_WIDTH));
			HAL_PRINT_DBG(KRED " DRAM state after sweep: lane= %u; PHY_DLL_TRIM_2= 0x%08lx; ber=%04X; fail_rate: lane1= %u, lane0= %u \n" KNRM, TRIM2_ilane, REG_READ (PHY_DLL_TRIM_2), ber, g_fail_rate_1, g_fail_rate_0);
			HAL_PRINT_DBG(KRED " DRAM state is not functional at this point. Restart boot-block. \n" KNRM);
		}
		//---------------------------------------------------------------------------------------------


}

#endif //TRIM_2_SWEEP

#ifdef VREF_SWEEP

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        Sweep_VREF_l                                                                           */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS           Sweep_VREF_l (void)
{

	UINT32		 ilane ,ibit;
	//-------------------------------------------------------------------------------------------------------------
		// test BMC VREF (read)
		//--------------------------------------------------------------------------------------------------------------

		HAL_PRINT ("\n");
		HAL_PRINT(KCYN "=========================  \n" KNRM);
		HAL_PRINT(KCYN "   BMC VREF sweep         \n" KNRM);
		HAL_PRINT(KCYN "========================== \n" KNRM);
		HAL_PRINT(" BMC support ranges: 10%% to 90%% in 124 steps (0.645%%, 7.74mV / steps). \n");
		HAL_PRINT(" For simplicity, start sweep from 43%% up to 90%%.  \n");
		HAL_PRINT(" Sweep range: 43%% (51) to 90%% (124) in 73 steps (0.645%%, 7.74mV / steps). \n");

		HAL_PRINT("\n\n");

		// get original VREF values
		UINT32 Origin_BMC_VREF[2];
		for (ilane = 0; ilane < MEM_STRB_WIDTH; ilane++)
		{
			REG_WRITE(PHY_LANE_SEL, VREF_WIDTH * ilane);
			Origin_BMC_VREF[ilane] = READ_REG_FIELD (VREF_TRAINING, VREF_TRAINING_vref_value);
		}

		// Sweep ...
		for (UINT32 current_vref=51; current_vref<=124; current_vref++)
		{

			// Change UVREF trim code for each lane by programming VREF_TRAINING register
			for (ilane = 0; ilane < MEM_STRB_WIDTH; ilane++)
			{
				REG_WRITE(PHY_LANE_SEL, VREF_WIDTH * ilane);
				SET_REG_FIELD (VREF_TRAINING, VREF_TRAINING_vref_value, current_vref);
			}

			float current_vref_percent = ((80/(float)124)*(float)(current_vref)) + (float)8.7;
			HAL_PRINT_DBG(KCYN "\n *** Set BMC VREF %u%% (step %u) **** \n" KNRM, (UINT8)(current_vref_percent+0.5), current_vref);

			Sweep_DQn_Trim_l(NULL, SWEEP_IN_DQ, FALSE, 0); // Run Parametric Sweep on READ side (Input DQn).

			// check min eye size for the all lane0
			int DQ_min_eyesize_lane0 = 255;
			for (ibit = 0; ibit < 8; ibit++)
				if (g_Table_Y_BestTrim_EyeSize[ibit]<DQ_min_eyesize_lane0)
					DQ_min_eyesize_lane0 = g_Table_Y_BestTrim_EyeSize[ibit];

			// check min eye size for the all lane1
			int DQ_min_eyesize_lane1 = 255;
			for (ibit = 8; ibit < 16; ibit++)
				if (g_Table_Y_BestTrim_EyeSize[ibit]<DQ_min_eyesize_lane1)
					DQ_min_eyesize_lane1 = g_Table_Y_BestTrim_EyeSize[ibit];

			HAL_PRINT_DBG(KCYN "\n > BMC VREF: %u%% (step %u); DQ lane0 min eye :%u; DQ lane1 min eye:%u; \n\n" KNRM, (UINT8)(current_vref_percent+0.5), current_vref, DQ_min_eyesize_lane0, DQ_min_eyesize_lane1);

			// store Sweep_DQs_Trim resoult - only sample point for -20 to +20 range that is 42 to 81.
			for (int samplePoint = 0; samplePoint < 40; samplePoint++)
				g_Table_Z_BitStatus	[samplePoint][current_vref-51] = g_Table_Y_BitStatus [samplePoint + 42];

			// store min eye size
			g_Table_MinEyeSize[current_vref-51][0] = DQ_min_eyesize_lane0;
			g_Table_MinEyeSize[current_vref-51][1] = DQ_min_eyesize_lane1;

		}

		//-----------------------------------
		// restore origin BMC VREF from SCL
		for (ilane = 0; ilane < MEM_STRB_WIDTH; ilane++)
		{
			REG_WRITE(PHY_LANE_SEL, VREF_WIDTH * ilane);
			SET_REG_FIELD (VREF_TRAINING, VREF_TRAINING_vref_value, Origin_BMC_VREF[ilane]);
			float current_vref_percent = ((80/(float)124)*(float)(Origin_BMC_VREF[ilane])) + (float)8.7;
			HAL_PRINT_DBG(KCYN " > Restore SCL: lane%u; VREF %u%% (step %u)  \n" KNRM, ilane, (UINT8)(current_vref_percent+0.5), Origin_BMC_VREF[ilane]);
		}

		//--------------------------------------------------------------
		// display table of min eye size
		HAL_PRINT_DBG(KCYN "\n\n *** Summary table of min eye size of lane(n) vs BMC.VREF(n) **** \n" KNRM);
		HAL_PRINT_DBG(KCYN " *** Note: BMC.VREF(n) tuning value is mark with with '*' \n\n" KNRM);

		for (UINT32 current_vref = 124; current_vref>=51; current_vref--)
		{
			float current_vref_percent = ((80/(float)124)*(float)(current_vref)) + (float)8.7;
			HAL_PRINT_DBG(KCYN "VREF: %03u (%02u%%);", current_vref, (UINT8)(current_vref_percent+0.5));
			for (ilane=0; ilane<2; ilane++)
			{
				int min_eyesize = g_Table_MinEyeSize[current_vref-51][ilane];
				if ( current_vref == Origin_BMC_VREF[ilane]) {
					HAL_PRINT_DBG(KCYN " * ");
				} else if (min_eyesize<9) {
					HAL_PRINT_DBG(KRED "   ");
				} else if (min_eyesize>11) {
					HAL_PRINT_DBG(KGRN "   ");
				} else {
					HAL_PRINT_DBG(KNRM "   ");
				}
				HAL_PRINT_DBG("Min Eye Lane%u: %u;    ", ilane, min_eyesize);
			}
			HAL_PRINT_DBG(KNRM "\n");
		}

		//--------------------------------------------------------------
		// display eye (DQ vs VREF) for each bit.
		for (int ibit = 0; ibit < 16; ibit++)
		{
			HAL_PRINT_DBG(KCYN "\n\n *** DQ%u read side eye mask (X: IP_DQn delay; Y: BMC VREF) **** \n\n" KNRM, ibit);
			HAL_PRINT_DBG("           -20-19-18-17-16-15-14-13-12-11-10-09-08-07-06-05-04-03-02-01 | +01+02+03+04+05+06+07+08+09+10+11+12+13+14+15+16+17+18+19");

			ilane = ibit/8;

			// get vref
			int center_point_y = (int)Origin_BMC_VREF [ilane];

			// get IN_DQn delay
			REG_WRITE(PHY_LANE_SEL,(ilane * DQS_DLY_WIDTH) + (((UINT32)ibit& 0x7) << 8));
			int center_point_x = TWOS_COMP_7BIT_REG_TO_VALUE (READ_REG_FIELD(IP_DQ_DQS_BITWISE_TRIM, IP_DQ_DQS_BITWISE_TRIM_ip_dq_dqs_bitwise_trim_reg)) + 62 -42 ;

			for (int current_vref=124; current_vref>=51; current_vref--)
			{
				float current_vref_percent = ((80/(float)124)*(float)(current_vref)) + (float)8.7;
				HAL_PRINT_DBG(KCYN "\n%03u (%02u%%); ", current_vref, (UINT8)(current_vref_percent+0.5));
				for (int samplePoint = 0; samplePoint < 40; samplePoint++)
				{
					UINT16 BitStatus = g_Table_Z_BitStatus [samplePoint][current_vref-51];
					if ((BitStatus >> ibit) & 0x1)
					{
						if ((current_vref == center_point_y) && (samplePoint == center_point_x)) {
							HAL_PRINT_DBG(KCYN "@-@");
						} else {
							HAL_PRINT_DBG(KRED " - ");
						}
					}
					else
					{
						if ((current_vref == center_point_y) && (samplePoint == center_point_x)) {
							HAL_PRINT_DBG(KCYN "@V@");
						} else {
							HAL_PRINT_DBG(KGRN " V ");
						}
					}
			  }
			}
		}
		HAL_PRINT_DBG(KNRM "\n");
		//--------------------------------------------------------------
}


#endif //VREF_SWEEP

#ifdef DRAM_SWEEP
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        Sweep_DRAM_l                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ddr_setup -                                                                            */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS           Sweep_DRAM_l (DDR_Setup *ddr_setup)
{

		UINT16 ber;
		UINT32		 ilane ,ibit;
		//-------------------------------------------------------------------------------------------------------------
		// test DRAM VREF (write) on range 1 and range 0
		//--------------------------------------------------------------------------------------------------------------

		HAL_PRINT ("\n");
		HAL_PRINT(KCYN "=========================  \n" KNRM);
		HAL_PRINT(KCYN "   DRAM VREF sweep         \n" KNRM);
		HAL_PRINT(KCYN "========================== \n" KNRM);
		HAL_PRINT(" DRAM support two VREF ranges:  \n");
		HAL_PRINT(" * Range 2: 45%% to 77.5%% in 50 steps (0.65%%, 7.8mV / steps).  \n");
		HAL_PRINT(" * Range 1: 60%% to 92.5%% in 50 steps (0.65%%, 7.8mV / steps).  \n");
		HAL_PRINT(" For simplicity, start sweep from range 2 up to 60%% and then continue with range 1 up to the end.  \n");
		HAL_PRINT(" Sweep range: 45%% to 92.5%% in 73 steps (0.65%%, 7.8mV / steps). \n");
		HAL_PRINT("\n\n");

		ber = MC_MemStressTestLong(FALSE, FALSE, 0,FALSE);
		if (ber != 0)
		{
			HAL_PRINT_DBG(KRED " DRAM state before sweep:  ber=%04X \n", ber);
			HAL_PRINT_DBG(KRED " DRAM state is not functional at this point. Restart boot-block. \n");
			while (1);
		}

		for (UINT32 current_vref=0; current_vref<=73; current_vref++)
		{
			UINT32 new_value = ddr_setup->MR6_DATA & 0xFF00;
			new_value |= 1<<7; // VREF Calibration Enable
			if (current_vref > 22)
			{
				new_value |= 0<<6; // VREF range 1 (60%..92.5% @ 0.65%)
				new_value |= ((current_vref-23)& 0x3F); // vref 0~63
			}
			else
			{
				new_value |= 1<<6; // VREF range 2 (45%..77% @ 0.65%)
				new_value |= (current_vref& 0x3F); // vref 0~63
			}
			MC_write_mr_regs_single (6, new_value); // at first time, when enable DRAM VREF, need to set enable bit before setting the value so repeate the write twice for all settingds just in case.
			MC_write_mr_regs_single (6, new_value);
			// need 150nsec delay until VREF  settble

			float current_vref_percent = ((float)0.65*(float)current_vref) + (float)45;
			HAL_PRINT_DBG(KCYN "\n *** Set DRAM VREF %u%% (step %u) **** \n" KNRM, (UINT8)(current_vref_percent+0.5), current_vref);

			Sweep_DQn_Trim_l(NULL, SWEEP_OUT_DM, FALSE, 0); // Run Parametric Sweep on WRITE side Output DM and find the center for each DQn and average for each DM lane.

			// check min eye size for the all lane0
			int DM_min_eyesize_lane0 = 255;
			for (ibit = 0; ibit < 8; ibit++)
				if (g_Table_Y_BestTrim_EyeSize[ibit]<DM_min_eyesize_lane0)
					DM_min_eyesize_lane0 = g_Table_Y_BestTrim_EyeSize[ibit];

			// check min eye size for the all lane1
			int DM_min_eyesize_lane1 = 255;
			for (ibit = 8; ibit < 16; ibit++)
				if (g_Table_Y_BestTrim_EyeSize[ibit]<DM_min_eyesize_lane1)
					DM_min_eyesize_lane1 = g_Table_Y_BestTrim_EyeSize[ibit];

			// store Sweep_DQs_Trim resoult - only sample point for -20 to +20 range that is 42 to 81.
			for (int samplePoint = 0; samplePoint < 40; samplePoint++)
				g_Table_Z2_BitStatus [samplePoint][current_vref] = g_Table_Y_BitStatus [samplePoint + 42];

			Sweep_DQn_Trim_l(NULL, SWEEP_OUT_DQ, FALSE, 0); // Run Parametric Sweep on WRITE side (Output DQn) and find the center for each DQn

			// check min eye size for the all lane0
			int DQ_min_eyesize_lane0 = 255;
			for (ibit = 0; ibit < 8; ibit++)
				if (g_Table_Y_BestTrim_EyeSize[ibit]<DQ_min_eyesize_lane0)
					DQ_min_eyesize_lane0 = g_Table_Y_BestTrim_EyeSize[ibit];

			// check min eye size for the all lane0
			int DQ_min_eyesize_lane1 = 255;
			for (ibit = 8; ibit < 16; ibit++)
				if (g_Table_Y_BestTrim_EyeSize[ibit]<DQ_min_eyesize_lane1)
					DQ_min_eyesize_lane1 = g_Table_Y_BestTrim_EyeSize[ibit];

			HAL_PRINT_DBG(KCYN "\n > DRAM VREF: %u%% (step %u); DQ lane0 min eye :%u; DQ lane1 min eye:%u; DM lane0 min eye :%u; DM lane1 min eye:%u;\n\n" KNRM, (UINT8)(current_vref_percent+0.5), current_vref, DQ_min_eyesize_lane0, DQ_min_eyesize_lane1, DM_min_eyesize_lane0, DM_min_eyesize_lane1);

			// store Sweep_DQs_Trim resoult - only sample point for -20 to +20 range that is 42 to 81.
			for (int samplePoint = 0; samplePoint < 40; samplePoint++)
				g_Table_Z_BitStatus	[samplePoint][current_vref] = g_Table_Y_BitStatus [samplePoint + 42];

			// store min eye
			g_Table_MinEyeSize[current_vref][0] = DQ_min_eyesize_lane0;
			g_Table_MinEyeSize[current_vref][1] = DQ_min_eyesize_lane1;
		}

		//-----------------------------------
		// restore origin DRAM VREF from SCL
		float current_vref_percent = ((float)0.65*(float)ddr_setup->SaveDRAMVref) + (float)45;
		HAL_PRINT_DBG(KCYN " > Restore SCL VREFF %u%% (step %u)  \n" KNRM, (UINT8)(current_vref_percent+0.5), ddr_setup->SaveDRAMVref);

		UINT32 new_value = ddr_setup->MR6_DATA & 0xFF00;
		if (ddr_setup->SaveDRAMVref > 22)
		{
			new_value |= 0<<6; // VREF range 1 (60%..92.5% @ 0.65%)
			new_value |= ((ddr_setup->SaveDRAMVref-23)& 0x3F); // vref 0~63
		}
		else
		{
			new_value |= 1<<6; // VREF range 2 (45%..77% @ 0.65%)
			new_value |= (ddr_setup->SaveDRAMVref& 0x3F); // vref 0~63
		}
		MC_write_mr_regs_single (6, new_value | 1<<7); // set new value with VREF enable
		MC_write_mr_regs_single (6, new_value); // disable VREF
		//-----------------------------------

		//--------------------------------------------------------------
		// display table of min eye size
		HAL_PRINT_DBG(KCYN "\n\n *** Summary table of min eye size of both lane0 and lane1 vs DRAM.VREF **** \n" KNRM);
		HAL_PRINT_DBG(KCYN " *** Note: DRAM.VREF tuning value is mark with with '*' \n\n" KNRM);
		for (int current_vref=73; current_vref>=0; current_vref--)
		{
			float current_vref_percent = ((float)0.65*(float)current_vref) + (float)45;
			HAL_PRINT_DBG(KCYN "VREF: %02u (%02u%%); ", current_vref, (UINT8)(current_vref_percent+0.5));
			for (ilane=0; ilane<2; ilane++)
			{
				int min_eyesize = g_Table_MinEyeSize[current_vref][ilane];
				if (ddr_setup->SaveDRAMVref == current_vref) {
					HAL_PRINT_DBG(KCYN " * ");
				} else if (min_eyesize<9) {
					HAL_PRINT_DBG(KRED "   ");
				} else if (min_eyesize>11) {
					HAL_PRINT_DBG(KGRN "   ");
				} else {
					HAL_PRINT_DBG(KNRM "   ");
				}

				HAL_PRINT_DBG("Min Eye Lane%u: %u;    ", ilane, min_eyesize);
			}
			HAL_PRINT_DBG(KNRM "\n");
		}

		//--------------------------------------------------------------
		// display eye (DQ vs VREF) for each bit.
		for (int ibit = 0; ibit < 16; ibit++)
		{
			HAL_PRINT_DBG(KCYN "\n\n *** DQ%u write side eye mask (X: OUT_DQn delay; Y: DRAM VREF) **** \n\n" KNRM, ibit);
			HAL_PRINT_DBG("          -20-19-18-17-16-15-14-13-12-11-10-09-08-07-06-05-04-03-02-01 | +01+02+03+04+05+06+07+08+09+10+11+12+13+14+15+16+17+18+19");

			// get vref
			int center_point_y = (int)ddr_setup->SaveDRAMVref;

			// get OUT_DQn delay
			ilane = ibit/8;
			REG_WRITE( PHY_LANE_SEL, (ilane * DQS_DLY_WIDTH) + (((UINT32)ibit& 0x7) << 8));
			int center_point_x = TWOS_COMP_7BIT_REG_TO_VALUE (READ_REG_FIELD(OP_DQ_DM_DQS_BITWISE_TRIM, OP_DQ_DM_DQS_BITWISE_TRIM_op_dq_dm_dqs_bitwise_trim_reg)) + 62 -42 ;

			for (int current_vref=73; current_vref>=0; current_vref--)
			{
				float current_vref_percent = ((float)0.65*(float)current_vref) + (float)45;
				HAL_PRINT_DBG(KCYN "\n%02u (%02u%%); ", current_vref, (UINT8)(current_vref_percent+0.5));
				for (int samplePoint = 0; samplePoint < 40; samplePoint++)
				{
					UINT16 BitStatus = g_Table_Z_BitStatus [samplePoint][current_vref];
					if ((BitStatus >> ibit) & 0x1)
					{
						if ((current_vref == center_point_y) && (samplePoint == center_point_x)){
							HAL_PRINT_DBG(KCYN "@-@");
						} else {
							HAL_PRINT_DBG(KRED " - ");
						}
					}
					else
					{
						if ((current_vref == center_point_y) && (samplePoint == center_point_x)){
							HAL_PRINT_DBG(KCYN "@V@");
						} else {
							HAL_PRINT_DBG(KGRN " V ");
						}
					}
			  }
			}
		}
		HAL_PRINT_DBG(KNRM "\n");
		//--------------------------------------------------------------
		// display eye (DM vs VREF) for each lane.
		for (int ilane = 0; ilane < 2; ilane++)
		{
			HAL_PRINT_DBG(KCYN "\n\n *** DM%u write side eye mask (X: DM delay; Y: DRAM VREF) **** \n\n" KNRM, ilane);
			HAL_PRINT_DBG("          -20-19-18-17-16-15-14-13-12-11-10-09-08-07-06-05-04-03-02-01 | +01+02+03+04+05+06+07+08+09+10+11+12+13+14+15+16+17+18+19");

			// get vref
			int center_point_y = (int)ddr_setup->SaveDRAMVref;

			// get OUT_DM delay
			REG_WRITE( PHY_LANE_SEL, (ilane * DQS_DLY_WIDTH) + 0x800);
			UINT32 DM = READ_REG_FIELD(OP_DQ_DM_DQS_BITWISE_TRIM, OP_DQ_DM_DQS_BITWISE_TRIM_op_dq_dm_dqs_bitwise_trim_reg);
			int center_point_x = TWOS_COMP_7BIT_REG_TO_VALUE (DM) + 62 -42 ;

			for (int current_vref=73; current_vref>=0; current_vref--)
			{
				float current_vref_percent = ((float)0.65*(float)current_vref) + (float)45;
				HAL_PRINT_DBG(KCYN "\n%02u (%02u%%); ", current_vref, (UINT8)(current_vref_percent+0.5));
				for (int samplePoint = 0; samplePoint < 40; samplePoint++)
				{
					UINT16 BitStatus = g_Table_Z2_BitStatus [samplePoint][current_vref];
					if ((BitStatus >> (ilane * 8)) & 0xFF)
					{
						if ((current_vref == center_point_y) && (samplePoint == center_point_x)){
							HAL_PRINT_DBG(KCYN "@-@");
						} else {
							HAL_PRINT_DBG(KRED " - ");
						}
					}
					else
					{
						if ((current_vref == center_point_y) && (samplePoint == center_point_x)){
							HAL_PRINT_DBG(KCYN "@V@");
						} else {
							HAL_PRINT_DBG(KGRN " V ");
						}
					}
				}
			}
		}
		HAL_PRINT_DBG(KNRM "\n");
		//--------------------------------------------------------------

		// check DRAM state after restoring origin value, hoping DRAM is still working well
		// if DRAM stuck, no point to continue any test !
		ber = MC_MemStressTestLong(FALSE, FALSE, 0,FALSE);
		if (ber != 0)
		{
			HAL_PRINT_DBG(KRED " DRAM state after sweep:  ber=%04X \n" KNRM, ber);
			HAL_PRINT_DBG(KRED " DRAM state is not functional at this point. Restart boot-block. \n" KNRM);
			while (1);
		}
		//-------------------------------------------------------------------



}

#endif //DRAM_SWEEP

#ifdef IP_DQS_SWEEP

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        Sweep_IP_DQS_l                                                                         */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS           Sweep_IP_DQS_l (void)
{
		UINT16 ber;
		UINT32		 ilane ,ibit;
		#define IP_DQS_ilane 0
		#define IP_DQS_start 8
		#define IP_DQS_end   (63-8)

		HAL_PRINT ("\n");
		HAL_PRINT(KCYN "=========================  \n" KNRM);
		HAL_PRINT(KCYN "   lane %u IP_DQS sweep     \n" KNRM, IP_DQS_ilane);
		HAL_PRINT(KCYN "========================== \n" KNRM);
		HAL_PRINT(" Sweep only lane %u IP_DQS while keep the other lane at default. \n", IP_DQS_ilane);

		// Get the origin value
		UINT32 stored_IP_DQS [2];
		REG_WRITE(PHY_LANE_SEL, (0 * DQS_DLY_WIDTH) + 0x800);
		stored_IP_DQS[0]  = READ_REG_FIELD(IP_DQ_DQS_BITWISE_TRIM, IP_DQ_DQS_BITWISE_TRIM_ip_dq_dqs_bitwise_trim_reg);
		REG_WRITE(PHY_LANE_SEL, (1 * DQS_DLY_WIDTH) + 0x800);
		stored_IP_DQS[1]  = READ_REG_FIELD(IP_DQ_DQS_BITWISE_TRIM, IP_DQ_DQS_BITWISE_TRIM_ip_dq_dqs_bitwise_trim_reg);

		// check DRAM state with origin value before start the sweep, hoping DRAM is still working well
		g_fail_rate_0 = 0;
		g_fail_rate_1 = 0;
		ber = MC_MemStressTestLong(FALSE, FALSE, 0,FALSE);
		if (ber != 0)
		{
			HAL_PRINT_DBG(KRED " DRAM state before sweep:  lane= %u; IP_DQS= %u; ber= %04X; fail_rate: lane1= %u, lane0= %u \n" KNRM, IP_DQS_ilane, stored_IP_DQS[IP_DQS_ilane], ber, g_fail_rate_1, g_fail_rate_0);
			HAL_PRINT_DBG(KRED " DRAM state is not functional at this point. Restart boot-block. \n" KNRM);
			while (1);
		}


		// start sweep IP_DQS lane0 while lane1 is fixed to default
		for (UINT32 current_trim=IP_DQS_start; current_trim<=IP_DQS_end; current_trim++)
		{
			// set new value
			HAL_PRINT_DBG(KCYN " \n*** Set  lane= %u; IP_DQS= %u; **** \n" KNRM, IP_DQS_ilane, current_trim);
			REG_WRITE(PHY_LANE_SEL, (IP_DQS_ilane * DQS_DLY_WIDTH) + 0x800);
			REG_WRITE(IP_DQ_DQS_BITWISE_TRIM, 0x80 |  (0x3F & current_trim));

			int DQ_min_eyesize_lane[2] = {255,255};

			// run DQ sweep
			Sweep_DQn_Trim_l(NULL, SWEEP_IN_DQ, FALSE, 0); // Run Parametric Sweep on READ side (Input DQn)
			//Sweep_DQn_Trim_l(SWEEP_OUT_DQ, FALSE, 0); // Run Parametric Sweep on WRITE side (Output DQn)


			// check min eye size for the all lane0
			for (ibit = 0; ibit < 16; ibit++)
				if (g_Table_Y_BestTrim_EyeSize[ibit]<DQ_min_eyesize_lane[(UINT8)ibit/8])
					DQ_min_eyesize_lane[(UINT8)ibit/8] = g_Table_Y_BestTrim_EyeSize[ibit];

			HAL_PRINT_DBG(KCYN "\n > IP_DQS.lane%u: %u; DQ lane%u min eye :%u;\n\n" KNRM, IP_DQS_ilane, current_trim, IP_DQS_ilane, DQ_min_eyesize_lane[IP_DQS_ilane]);

			// store Sweep_DQs_Trim resoult - only sample point for -20 to +20 range that is 42 to 81.
			for (int samplePoint = 0; samplePoint < 40; samplePoint++)
				g_Table_Z_BitStatus	[samplePoint][current_trim] = g_Table_Y_BitStatus [samplePoint + 42];

			// store min eye size (only for one lane)
			g_Table_MinEyeSize [current_trim][0] = DQ_min_eyesize_lane[0];
			g_Table_MinEyeSize [current_trim][1] = DQ_min_eyesize_lane[1];

		}

		//-----------------------------------
		// restore origin SCL trim_2 value
		HAL_PRINT_DBG(KCYN " > Restore SCL IP_DQS: %u  \n" KNRM, stored_IP_DQS[IP_DQS_ilane]);
		REG_WRITE(PHY_LANE_SEL, (IP_DQS_ilane * DQS_DLY_WIDTH) + 0x800);
		REG_WRITE(IP_DQ_DQS_BITWISE_TRIM, 0x80 |  (0x3F & stored_IP_DQS[IP_DQS_ilane]));

		//--------------------------------------------------------------
		// display table of min eye size
		HAL_PRINT_DBG(KCYN "\n\n *** Summary table of min eye size of DQn vs IP_DQS.lane%u  **** \n" KNRM, IP_DQS_ilane);
		HAL_PRINT_DBG(KCYN " *** Note: IP_DQS.lane%u tuning value is mark with with '*' \n\n" KNRM, IP_DQS_ilane);
		for (UINT32 current_trim=IP_DQS_start; current_trim<=IP_DQS_end; current_trim++)
		{
			HAL_PRINT_DBG(KCYN "IP_DQS.lane%u: %02u;", IP_DQS_ilane, current_trim);

			for (int ilane=0; ilane<2; ilane++)
			{
				int min_eyesize = g_Table_MinEyeSize[current_trim][ilane];
				if (current_trim == stored_IP_DQS[ilane]) {
					HAL_PRINT_DBG(KCYN " * ");
				} else if (min_eyesize<9) {
					HAL_PRINT_DBG(KRED "   ");
				} else if (min_eyesize>11) {
					HAL_PRINT_DBG(KGRN "   ");
				} else {
					HAL_PRINT_DBG(KNRM "   ");
				}

				HAL_PRINT_DBG("Min Eye DQ.lane%u: %u;    ", ilane, min_eyesize);
			}

			HAL_PRINT_DBG(KNRM "\n");
		}

		//--------------------------------------------------------------
		// display eye (DQ vs IP_DQS) for each bit.
		for (int ibit = 0; ibit < 16; ibit++) // display on lane0 eye, the critical issue we have. no issue was found in lane1
		{
			HAL_PRINT_DBG(KCYN "\n\n *** DQ%u read side eye mask (X: IP_DQn delay; Y: IP_DQS lane%u delay) **** \n\n" KNRM, ibit, IP_DQS_ilane);
			HAL_PRINT_DBG("   -20-19-18-17-16-15-14-13-12-11-10-09-08-07-06-05-04-03-02-01 | +01+02+03+04+05+06+07+08+09+10+11+12+13+14+15+16+17+18+19");

			// get IP_DQS delay
			int center_point_y = (int) stored_IP_DQS [(UINT32)ibit>>3/*ilane */];;

			// get IP_DQn delay
			REG_WRITE(PHY_LANE_SEL,((UINT32)ibit>>3/*ilane */ * DQS_DLY_WIDTH) + (((UINT32)ibit& 0x7) << 8));
			int center_point_x = TWOS_COMP_7BIT_REG_TO_VALUE (READ_REG_FIELD(IP_DQ_DQS_BITWISE_TRIM, IP_DQ_DQS_BITWISE_TRIM_ip_dq_dqs_bitwise_trim_reg)) + 62 -42 ;

			for (int current_trim=IP_DQS_end; current_trim>=IP_DQS_start; current_trim--)
			{
				HAL_PRINT_DBG(KCYN "\n%02u ", current_trim);
				for (int samplePoint = 0; samplePoint < 40; samplePoint++)
				{
					UINT16 BitStatus = g_Table_Z_BitStatus [samplePoint][current_trim];
					if ((BitStatus >> ibit) & 0x1)
					{
						if ((current_trim == center_point_y) && (samplePoint == center_point_x)){
							HAL_PRINT_DBG(KCYN "@-@");
						} else {
							HAL_PRINT_DBG(KRED " - ");
						}
					}
					else
					{
						if ((current_trim == center_point_y) && (samplePoint == center_point_x)){
							HAL_PRINT_DBG(KCYN "@V@");
						} else {
							HAL_PRINT_DBG(KGRN " V ");
						}
					}
			  }
			}
		}
		HAL_PRINT_DBG(KNRM "\n");
		//----------------------------------------------------------------------------------------

		// check DRAM state after restoring origin value, hoping DRAM is still working well
		// if DRAM stuck, no point to continue any test !
		g_fail_rate_0 = 0;
		g_fail_rate_1 = 0;
		ber = MC_MemStressTestLong(FALSE, FALSE, 0,FALSE);
		if (ber != 0)
		{
			REG_WRITE(PHY_LANE_SEL, (IP_DQS_ilane * DQS_DLY_WIDTH) + 0x800);
			HAL_PRINT_DBG(KRED " DRAM state after sweep:  lane= %u; IP_DQS= 0x%08lx; ber=%04X; fail_rate: lane1= %u, lane0= %u \n" KNRM, IP_DQS_ilane, REG_READ (IP_DQ_DQS_BITWISE_TRIM), ber, g_fail_rate_1, g_fail_rate_0);
			HAL_PRINT_DBG(KRED " DRAM state is not functional at this point. Restart boot-block. \n" KNRM);
			while (1);
		}
		//---------------------------------------------------------------------------------------------

}
#endif


#ifdef OP_DQS_SWEEP

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        Sweep_OP_DQS_l                                                                         */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS           Sweep_OP_DQS_l (void)
{
		UINT16 ber;
		UINT32		 ilane ,ibit;
		#define OP_DQS_ilane 1
		#define OP_DQS_start 8
		#define OP_DQS_end   (63-8)

		HAL_PRINT ("\n");
		HAL_PRINT(KCYN "=========================  \n" KNRM);
		HAL_PRINT(KCYN "   lane %u OP_DQS sweep     \n" KNRM, OP_DQS_ilane);
		HAL_PRINT(KCYN "========================== \n" KNRM);
		HAL_PRINT(" Sweep only lane %u OP_DQS while keep the other lane at default. \n", OP_DQS_ilane);

		// Get the origin value
		UINT32 stored_OP_DQS [2];
		REG_WRITE(PHY_LANE_SEL, (0 * DQS_DLY_WIDTH) + 0x900);
		stored_OP_DQS[0]  = READ_REG_FIELD(OP_DQ_DM_DQS_BITWISE_TRIM, OP_DQ_DM_DQS_BITWISE_TRIM_op_dq_dm_dqs_bitwise_trim_reg);
		REG_WRITE(PHY_LANE_SEL, (1 * DQS_DLY_WIDTH) + 0x900);
		stored_OP_DQS[1]  = READ_REG_FIELD(OP_DQ_DM_DQS_BITWISE_TRIM, OP_DQ_DM_DQS_BITWISE_TRIM_op_dq_dm_dqs_bitwise_trim_reg);

		// check DRAM state with origin value before start the sweep, hoping DRAM is still working well
		g_fail_rate_0 = 0;
		g_fail_rate_1 = 0;
		ber = MC_MemStressTestLong(FALSE, FALSE, 0,FALSE);
		if (ber != 0)
		{
			HAL_PRINT_DBG(KRED " DRAM state before sweep:  lane= %u; OP_DQS= %u; ber= %04X; fail_rate: lane1= %u, lane0= %u \n" KNRM, OP_DQS_ilane, stored_OP_DQS[OP_DQS_ilane], ber, g_fail_rate_1, g_fail_rate_0);
			HAL_PRINT_DBG(KRED " DRAM state is not functional at this point. Restart boot-block. \n" KNRM);
			while (1);
		}


		// start sweep OP_DQS lane0 while lane1 is fixed to default
		for (UINT32 current_trim=OP_DQS_start; current_trim<=OP_DQS_end; current_trim++)
		{
			// set new value
			HAL_PRINT_DBG(KCYN " \n*** Set  lane= %u; OP_DQS= %u; **** \n" KNRM, OP_DQS_ilane, current_trim);
			REG_WRITE(PHY_LANE_SEL, (OP_DQS_ilane * DQS_DLY_WIDTH) + 0x900);
			REG_WRITE(OP_DQ_DM_DQS_BITWISE_TRIM, 0x80 |  (0x3F & current_trim));

			int DQ_min_eyesize_lane[2] = {255,255};

			// run DQ sweep
			//Sweep_DQn_Trim_l(SWEEP_IN_DQ, FALSE, 0); // Run Parametric Sweep on READ side (Input DQn)
			Sweep_DQn_Trim_l(NULL, SWEEP_OUT_DQ, FALSE, 0); // Run Parametric Sweep on WRITE side (Output DQn)

			// check min eye size for the all lane0 and lane1
			for (ibit = 0; ibit < 16; ibit++)
				if (g_Table_Y_BestTrim_EyeSize[ibit]<DQ_min_eyesize_lane[(UINT8)ibit/8])
					DQ_min_eyesize_lane[(UINT8)ibit/8] = g_Table_Y_BestTrim_EyeSize[ibit];

			HAL_PRINT_DBG(KCYN "\n > OP_DQS.lane%u: %u; DQ lane%u min eye :%u;\n\n" KNRM, OP_DQS_ilane, current_trim, OP_DQS_ilane, DQ_min_eyesize_lane[OP_DQS_ilane]);

			// store Sweep_DQs_Trim resoult - only sample point for -20 to +20 range that is 42 to 81.
			for (int samplePoint = 0; samplePoint < 40; samplePoint++)
				g_Table_Z_BitStatus	[samplePoint][current_trim] = g_Table_Y_BitStatus [samplePoint + 42];

			// store min eye size (only for one lane)
			g_Table_MinEyeSize [current_trim][0] = DQ_min_eyesize_lane[0];
			g_Table_MinEyeSize [current_trim][1] = DQ_min_eyesize_lane[1];

		}

		//-----------------------------------
		// restore origin SCL OP_DQS value
		HAL_PRINT_DBG(KCYN " > Restore SCL OP_DQS: %u  \n" KNRM, stored_OP_DQS[OP_DQS_ilane]);
		REG_WRITE(PHY_LANE_SEL, (OP_DQS_ilane * DQS_DLY_WIDTH) + 0x900);
		REG_WRITE(OP_DQ_DM_DQS_BITWISE_TRIM, 0x80 |  (0x3F & stored_OP_DQS[OP_DQS_ilane]));

		//--------------------------------------------------------------
		// display table of min eye size
		HAL_PRINT_DBG(KCYN "\n\n *** Summary table of min eye size of DQn vs OP_DQS.lane%u  **** \n" KNRM, OP_DQS_ilane);
		HAL_PRINT_DBG(KCYN " *** Note: OP_DQS tuning value is mark with with '*' \n\n" KNRM);
		for (UINT32 current_trim=OP_DQS_start; current_trim<=OP_DQS_end; current_trim++)
		{
			HAL_PRINT_DBG(KCYN "OP_DQS.lane%u: %02u;", OP_DQS_ilane, current_trim);
			for (int ilane=0; ilane<2; ilane++)
			{
				int min_eyesize = g_Table_MinEyeSize[current_trim][ilane];
				if (current_trim == stored_OP_DQS[ilane]){
					HAL_PRINT_DBG(KCYN " * ");
				}else if (min_eyesize<9){
					HAL_PRINT_DBG(KRED "   ");
				}else if (min_eyesize>11){
					HAL_PRINT_DBG(KGRN "   ");
				} else {
					HAL_PRINT_DBG(KNRM "   ");
				}
				HAL_PRINT_DBG("Min Eye DQ.lane%u: %u;    ", ilane, min_eyesize);
			}
			HAL_PRINT_DBG(KNRM "\n");
		}

		//--------------------------------------------------------------
		// display eye (DQ vs OP_DQS) for each bit.
		for (int ibit = 0; ibit < 16; ibit++) // display on lane0 eye, the critical issue we have. no issue was found in lane1
		{
			HAL_PRINT_DBG(KCYN "\n\n *** DQ%u write side eye mask (X: OP_DQn delay; Y: OP_DQS lane%u delay) **** \n\n" KNRM, ibit, OP_DQS_ilane);
			HAL_PRINT_DBG("   -20-19-18-17-16-15-14-13-12-11-10-09-08-07-06-05-04-03-02-01 | +01+02+03+04+05+06+07+08+09+10+11+12+13+14+15+16+17+18+19");

			// get OP_DQS delay
			int center_point_y = (int) stored_OP_DQS [(UINT32)ibit >> 3 /*ilane */ ];;

			// get OP_DQn delay
			REG_WRITE(PHY_LANE_SEL,((UINT32)ibit >> 3 /*ilane */ * DQS_DLY_WIDTH) + (((UINT32)ibit& 0x7) << 8));
			int center_point_x = TWOS_COMP_7BIT_REG_TO_VALUE (READ_REG_FIELD(OP_DQ_DM_DQS_BITWISE_TRIM, OP_DQ_DM_DQS_BITWISE_TRIM_op_dq_dm_dqs_bitwise_trim_reg)) + 62 -42 ;

			for (int current_trim =  OP_DQS_end; current_trim >= OP_DQS_start; current_trim--)
			{
				HAL_PRINT_DBG(KCYN "\n%02u ", current_trim);
				for (int samplePoint = 0; samplePoint < 40; samplePoint++)
				{
					UINT16 BitStatus = g_Table_Z_BitStatus[samplePoint][current_trim];
					if ((BitStatus >> ibit) & 0x1)
					{
						if ((current_trim == center_point_y) && (samplePoint == center_point_x)){
							HAL_PRINT_DBG(KCYN "@-@");
						} else{
							HAL_PRINT_DBG(KRED " - ");
						}
					}
					else
					{
						if ((current_trim == center_point_y) && (samplePoint == center_point_x)){
							HAL_PRINT_DBG(KCYN "@V@");
						} else{
							HAL_PRINT_DBG(KGRN " V ");
						}
					}
			  }
			}
		}
		HAL_PRINT_DBG(KNRM "\n");
		//----------------------------------------------------------------------------------------

		// check DRAM state after restoring origin value, hoping DRAM is still working well
		// if DRAM stuck, no point to continue any test !
		g_fail_rate_0 = 0;
		g_fail_rate_1 = 0;
		ber = MC_MemStressTestLong(FALSE, FALSE, 0,FALSE);
		if (ber != 0)
		{
			REG_WRITE(PHY_LANE_SEL, (OP_DQS_ilane * DQS_DLY_WIDTH) + 0x900);
			HAL_PRINT_DBG(KRED " DRAM state after sweep:  lane= %u; OP_DQS= 0x%08lx; ber=%04X; fail_rate: lane1= %u, lane0= %u \n" KNRM, OP_DQS_ilane, REG_READ (OP_DQ_DM_DQS_BITWISE_TRIM), ber, g_fail_rate_1, g_fail_rate_0);
			HAL_PRINT_DBG(KRED " DRAM state is not functional at this point. Restart boot-block. \n" KNRM);
			while (1);
		}
}
#endif


