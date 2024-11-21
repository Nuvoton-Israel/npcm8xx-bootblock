
#define BIT_LVL_SAMPLE_QTY 2



/*----------------------------------------------------------------------------*/
/* Function:        calc_mr0_val                                              */
/*                                                                            */
/* Parameters:                                                                */
/*                  ddr_setup -     ddr training context                      */
/*                                                                            */
/* Returns:                                                                   */
/* Side effects:                                                              */
/* Description:                                                                                            */
/*                  This routine calc MR0 register on DRAM. CAS latency is reg is broken value [12, 6:4, 2]*/
/*---------------------------------------------------------------------------------------------------------*/
static UINT32 calc_mr0_val (DDR_Setup *ddr_setup)
{

	UINT32 reg_read_val = 0; //A0-A3 are 0
	UINT32 latency = ddr_setup->cas_latency;
	UINT32 tmp = 0;

	if (latency >= 25)
	{
		SET_VAR_BIT(reg_read_val, 12);
		latency -= 25;
		latency += 9;
	}

	if (latency <= 16)
	{
		tmp = latency - 9;
	}
	else
	{
		tmp = 8 + (latency - 18) / 2;
	}
	reg_read_val |= (tmp & 0x00000001) << 2;
	reg_read_val |= (tmp & 0x0000001E) << 3;

	HAL_PRINT_DBG(" reg_read_val = 0x%x \n", reg_read_val);

	// need to clarify the trtp values
	// reg_read_val |= ddr_setup->trtp <<9;

	if (ddr_setup->dram_type_clk > DRAM_CLK_TYPE_1600)
	{
		reg_read_val |=  0x3 << 9;
	}
#ifdef MC_CAPABILITY_CLK_TYPE_1600
	else
	{
		// Check the MR 0 value for 800! Why was the DLL reset set
		reg_read_val |=  0x1 << 9;
	}
#endif
	return reg_read_val;
}




/*---------------------------------------------------------------------------------------------------------*/
/* Function:        calc_mr1_val                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ddr_setup -     ddr training context                                                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
static UINT32 calc_mr1_val (DDR_Setup *ddr_setup)
{

	UINT32 reg_read_val = 1; //A0 is DLL enable

// 2, 1 Output driver impedance (ODI) – Output driver impedance setting
// 00 = RZQ/7 (34 ohm)
// 01 = RZQ/5 (48 ohm)
// 10 = Reserved (Although not JEDEC-defined and not tested, this setting will provide RZQ/6 or 40 ohm)
// 11 = Reserved

	if (ddr_setup->dram_drive == 34)
	{
		reg_read_val |= 0 << 1; // 00b is RZQ/7
	}
	else if (ddr_setup->dram_drive == 48)
	{
		reg_read_val |= 1 << 1; // 01b is RZQ/5
	}
	else
	{
		// No other value supported
		reg_read_val |= 1 << 1;
		HAL_PRINT_DBG("Unsuppored DDR Drive - use 48 %d\n", ddr_setup->dram_drive);
		ddr_setup->dram_drive = 48;
	}

// 10, 9, 8 Nominal ODT (RTT(NOM) – Data bus termination setting
// 000 = RTT(NOM) disabled
// 001 = RZQ/4 (60 ohm)
// 010 = RZQ/2 (120 ohm)
// 011 = RZQ/6 (40 ohm)
// 100 = RZQ/1 (240 ohm)
// 101 = RZQ/5 (48 ohm)
// 110 = RZQ/3 (80 ohm)
// 111 = RZQ/7 (34 ohm)

	switch (ddr_setup->dram_odt)
	{

		case 0:
			reg_read_val |= 0; //RZQ / 7 000b
			break;
		case 34:
			reg_read_val |= 0x7 << 8; //RZQ / 7 111b
			break;
		case 40:
			reg_read_val |= 0x3 << 8; //RZQ/6 011b
			break;
		case 48:
			reg_read_val |= 0x5 << 8; //RZQ/5 101b
			break;
		case 60:
			reg_read_val |= 0x1 << 8; //RZQ/6 001b
			break;
		case 80:
			reg_read_val |= 0x6 << 8; //RZQ / 3 110b
			break;
		case 120:
			reg_read_val |= 0x2 << 8;  //RZQ / 2 010b
			break;
		case 240:
			reg_read_val |= 0x4 << 8;  //RZQ / 1 100b
			break;
		default:
			reg_read_val |= 0x5 << 8; //RZQ/5 101b
			HAL_PRINT("Unsuppored DDR ODT %d, use 48\n", ddr_setup->dram_odt);
			ddr_setup->dram_odt = 48;
			break;

	}
	return reg_read_val;
}




/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_Init_DDR_Setup                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ddr_setup -     ddr training context                                                   */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs init the MC training context (ddr_setup)                         */
/*                  Note: this func only init value. All subsequenct calcs happen later                    */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS MC_Init_DDR_Setup (DDR_Setup *ddr_setup)
{

	// only setting that work
	// ddr_setup->dram_type_clk = DRAM_CLK_TYPE_2133;
		// Aug 25.
	// In future can be done from header
	// Initializes the triming parameters of the CS/DQ/CLK to predifined values
	// It is also possible to avoid the increment parameter and use the sign of the value
	// to state if it's increment or decrement. However then we will need some indication
	// on when to use the default method for ADRCTRL. Waiting for corners for more data.

	// Aug 31
	// Revised the calculation method which is now mas_dly / 2 + the delay parameter
	// Set all the delay to '0' if you want to use the default method, otherwise ALL delays and incremets must be valid
	// In future - consider use increment only!
	ddr_setup->dll_recalib_trim_adrctrl_ma = -13; // reverted to -13 which is the simulation value (value from SCOPE signals  -8);
	ddr_setup->dll_recalib_trim_increment_ma = TRUE;
	ddr_setup->dlls_trim_adrctrl = -13; // reverted to -13 which is the simulation value (value from SCOPE signals  -8);  shmoo: use -24, orig: -8
	ddr_setup->dlls_trim_adrctrl_incr = TRUE;
	ddr_setup->dlls_trim_clk = 0;
	ddr_setup->dlls_trim_clk_incr = TRUE;
	ddr_setup->phy_c2d = 1;
	// By Design
	ddr_setup->phy_additive_latency = 0;
	ddr_setup->phy_tRTP = 6;

	if (ddr_setup->dram_type_clk > DRAM_CLK_TYPE_1600)
	{
		ddr_setup->cas_latency = 0xF;
		ddr_setup->cas_write_latency = 0xE;
		ddr_setup->phy_tCCD_L = 6;
	}
#ifdef MC_CAPABILITY_CLK_TYPE_1600
	else
	{
		// NTIL: check if the correct value should be B or D!
		ddr_setup->cas_latency = 0xB;
		ddr_setup->cas_write_latency = 0xD; // TODO: or 0x0B?
		ddr_setup->phy_tCCD_L = 5;
	}

#else
	else
	{
		HAL_PRINT(KRED "DRAM 1600 - not supported! Reverting to 2133\n" KNRM);
		ddr_setup->dram_type_clk = DRAM_CLK_TYPE_2133;
		ddr_setup->cas_latency = 0xF;
		ddr_setup->cas_write_latency = 0xE;
		ddr_setup->phy_tCCD_L = 6;
	}
#endif

	// Based on TE parameters, currently hard coded, for now w/o analysis and verification
	if (CHIP_Get_Version() == 0x08)
	{
		ddr_setup->phy_cal_npu_offset = 0 ;
		ddr_setup->phy_incr_cal_npu = 0;
		ddr_setup->phy_cal_ppu_offset = 0;
		ddr_setup->phy_incr_cal_ppu = 0;
		ddr_setup->phy_cal_npd_offset = 0;
		ddr_setup->phy_incr_cal_npd = 0;
		ddr_setup->phy_cal_ppu_op8_offset = 0;
		ddr_setup->phy_incr_cal_ppu_op8 = 0;
		ddr_setup->phy_cal_npd_offset_op8 = 0;
		ddr_setup->phy_incr_cal_npd_op8 = 0;
		
	}
	else if (CHIP_Get_Version() == 0x04) // A1
	{
		ddr_setup->phy_cal_npu_offset = 0 ;
		ddr_setup->phy_incr_cal_npu = 0;
		ddr_setup->phy_cal_ppu_offset = 0;
		ddr_setup->phy_incr_cal_ppu = 0;
		ddr_setup->phy_cal_npd_offset = 0;
		ddr_setup->phy_incr_cal_npd = 0;
		ddr_setup->phy_cal_ppu_op8_offset = 0;
		ddr_setup->phy_incr_cal_ppu_op8 = 0;
		ddr_setup->phy_cal_npd_offset_op8 = 0;
		ddr_setup->phy_incr_cal_npd_op8 = 0;

	}
	else // Z1
	{

		ddr_setup->phy_cal_npu_offset = 6 ;
		ddr_setup->phy_incr_cal_npu = 0;
		ddr_setup->phy_cal_ppu_offset = 6;
		ddr_setup->phy_incr_cal_ppu = 0;
		ddr_setup->phy_cal_npd_offset = 8;
		ddr_setup->phy_incr_cal_npd = 1;
		ddr_setup->phy_cal_ppu_op8_offset = 6;
		ddr_setup->phy_incr_cal_ppu_op8 = 0;
		ddr_setup->phy_cal_npd_offset_op8 = 6;
		ddr_setup->phy_incr_cal_npd_op8 = 1;
	}



	return DEFS_STATUS_OK;

}



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
DEFS_STATUS MC_Init_DDR_Setup_re_calc (DDR_Setup *ddr_setup)
{

	// Round_up ((AL + CL)/2) – C2D – (HALF_PERIOD ? 3:1) Assume working in HALF Period
	ddr_setup->read_latency_adjust = ((ddr_setup->phy_additive_latency + ddr_setup->cas_latency) >> 1) +
					 ((ddr_setup->phy_additive_latency + ddr_setup->cas_latency) % 2) -
					   ddr_setup->phy_c2d - 1;
	HAL_PRINT_DBG("read_latency_adjust temp  0x%X\n", ddr_setup->read_latency_adjust);

#ifdef UNIQUIFY_PHY_HALF_PERIOD
	ddr_setup->read_latency_adjust -= 2;
	HAL_PRINT_DBG("read_latency_adjust dfi  0x%X\n", ddr_setup->read_latency_adjust);
#endif
	// Formula: round_up((AL+CWL+1)/2.0) – 2– ((AL+CWL)%2 == 0 && PHASE0 ? 1:0) – C2D

	// round_up((AL+CWL+1)/2.0)
	ddr_setup->write_latency_adjust = ((ddr_setup->phy_additive_latency + ddr_setup->cas_write_latency + 1) >> 1) +
	                                  ((ddr_setup->phy_additive_latency + ddr_setup->cas_write_latency + 1) % 2 ) ;
	ddr_setup->write_latency_adjust -= 2;
	ddr_setup->write_latency_adjust -= ddr_setup->phy_c2d;

#ifndef WRITE_COMMAND_PHASE1
		// ((AL+CWL)%2 == 0 && PHASE0 ? 1:0)
	if(((ddr_setup->phy_additive_latency + ddr_setup->cas_write_latency)%2) == 0)
	{
		// phase is 0 for write
		ddr_setup->write_latency_adjust -= 1;
		HAL_PRINT_DBG("write_latency_adjust dfi  0x%X\n", ddr_setup->write_latency_adjust);
	}
#endif
	HAL_PRINT_DBG("write_latency_adjust dfi  0x%X\n", ddr_setup->write_latency_adjust);


	// Prepare MR2_DATA
	ddr_setup->MR2_DATA = ddr_setup->cas_write_latency << 4;
	ddr_setup->MR1_DATA = calc_mr1_val(ddr_setup);
	ddr_setup->MR0_DATA = calc_mr0_val(ddr_setup);
	ddr_setup->MR6_DATA = (ddr_setup->phy_tCCD_L - 4) << 10 | 0 << 6 | 0x17;

	return DEFS_STATUS_OK;

}




/*---------------------------------------------------------------------------------------------------------*/
/* Function:        calc_vref_phy_value                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ddr_setup -     ddr training context                                                   */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
static UINT32 calc_vref_phy_value (DDR_Setup *ddr_setup)
{
	float vref_mid_level = ((float)1 + ((float)ddr_setup->dram_drive / ((float)ddr_setup->soc_odt + (float)ddr_setup->dram_drive))) / (float)2.0;
	// ODT is being used and the correct formula (given in IO user guide) has to be used based on that
	UINT32 vref_mid_level_code = (UINT32)ceilf((((vref_mid_level - 0.507f) / 0.0066f) + 65) - 0.5f);
	return vref_mid_level_code;

}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        MC_ohm_to_reg                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  val -                                                                                  */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs...                                                               */
/*---------------------------------------------------------------------------------------------------------*/
static UINT32 MC_ohm_to_reg ( UINT32 val)
{
	switch (val & 0xFF)
	{
		case 0:      return 0 ;
		case 60:     return PHY_DRV_60ohm;
		case 80:     return PHY_DRV_80ohm;
		case 120:    return PHY_DRV_120ohm;
		case 240:    return PHY_DRV_240ohm_A;
		case 34:     return PHY_DRV_34ohm;
		case 30:     return PHY_DRV_30ohm;
		case 40:     return PHY_DRV_40ohm;
		case 48:     return PHY_DRV_48ohm;
		default: HAL_PRINT(KRED "MC_ohm_to_reg error \n" KNRM);
	}

	return PHY_DRV_48ohm;
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:        ddr_phy_cfg1                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ddr_setup - parameters from the header                                                 */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs init of PHY before MC init.                                      */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS ddr_phy_cfg1 (DDR_Setup  *ddr_setup)
{
	//DRAM_TYPE: DDR4_1600K
	//dram_type_clk_PERIOD: 1250
	//WL: 11
	//RL: 11
	unsigned int timeout;
	unsigned int ilane;
	UINT32 dll_mas_dly, extra_addrctrl_dly;
	//Aug 24 21 - add specific argumenst for CS, DLL and CLK to allow flexibility
	INT32 dlls_trim_adrctrl, dlls_trim_clk, dll_recalib_trim_adrctrl_ma;
	BOOLEAN dlls_trim_adrctrl_incr, dlls_trim_clk_incr, dll_recalib_trim_increment_ma;
	UINT32 driveStr;
	UINT32 odt;
	UINT32 reg_read_val;
	UINT32 UNQ_IO_1_MASK = (BUILD_FIELD_VAL(UNIQUIFY_IO_1_cal_npu_offset, ddr_setup->phy_cal_npu_offset) |
				BUILD_FIELD_VAL(UNIQUIFY_IO_1_incr_cal_npu, ddr_setup->phy_incr_cal_npu) |
				BUILD_FIELD_VAL(UNIQUIFY_IO_1_cal_ppu_offset, ddr_setup->phy_cal_ppu_offset) |
				BUILD_FIELD_VAL(UNIQUIFY_IO_1_incr_cal_ppu, ddr_setup->phy_incr_cal_ppu) |
				BUILD_FIELD_VAL(UNIQUIFY_IO_1_cal_npd_offset, ddr_setup->phy_cal_npd_offset) |
				BUILD_FIELD_VAL(UNIQUIFY_IO_1_incr_cal_npd, ddr_setup->phy_incr_cal_npd));

	HAL_PRINT("MC cfg1\n");

	HAL_PRINT_DBG("UNQ_IO_1_MASK calc 0x%X\n", UNQ_IO_1_MASK);



	HAL_PRINT_DBG("\n>ddr_phy_cfg1  start\n");

	REG_WRITE( DLLM_WINDOW_SIZE, 0x00000006 );


	REG_WRITE( DDR4_CONFIG_1		 /*0x1c4*/, 0x00000009 ); /* DDR4_CONFIG_1_ddr4, DDR4_CONFIG_1_pod_term */
	REG_WRITE( UNIQUIFY_IO_3, 0x01000000 ); /* UNIQUIFY_IO_3_ucal_2step_cal */

	//REG_WRITE( PHY_PAD_CTRL, 0x10ffc0f3 );
	// Updated data from UNQ, ver 0.1.1 was 0x10ffc0f3
	REG_WRITE( PHY_PAD_CTRL, 0x10ffc000 );


	HAL_PRINT_DBG(">ddr_phy_cfg1 set pad control\n");

	HAL_PRINT_DBG(KMAG ">ddr_phy_cfg1 soc: use odt %d ohm and drive strengh %d ohm from header\n" KNRM ,
		ddr_setup->soc_odt, ddr_setup->soc_drive);

	HAL_PRINT_DBG(KMAG ">ddr_phy_cfg1 dram: use odt %d ohm and drive strengh %d ohm from header\n" KNRM ,
		ddr_setup->dram_odt, ddr_setup->dram_drive);
	driveStr = MC_ohm_to_reg(ddr_setup->soc_drive);
	odt = MC_ohm_to_reg(ddr_setup->soc_odt);


	REG_WRITE( PHY_PAD_CTRL_1, (
		BUILD_FIELD_VAL(PHY_PAD_CTRL_1_clk_drive_p	, driveStr ) |
		BUILD_FIELD_VAL(PHY_PAD_CTRL_1_clk_drive_n	, driveStr ) |
		BUILD_FIELD_VAL(PHY_PAD_CTRL_1_adrctrl_drive_p	, driveStr ) |
		BUILD_FIELD_VAL(PHY_PAD_CTRL_1_adrctrl_drive_n	, driveStr ) |
		BUILD_FIELD_VAL(PHY_PAD_CTRL_1_dq_dqs_drive_p	, driveStr ) |
		BUILD_FIELD_VAL(PHY_PAD_CTRL_1_dq_dqs_drive_n	, driveStr )));

	REG_WRITE( PHY_PAD_CTRL_2 , (
		BUILD_FIELD_VAL(PHY_PAD_CTRL_2_dq_dqs_term_p	 , odt ) |
		BUILD_FIELD_VAL(PHY_PAD_CTRL_2_dq_dqs_term_n	 , odt ) |
		BUILD_FIELD_VAL(PHY_PAD_CTRL_2_adrctrl_term_p	 , odt ) |
		BUILD_FIELD_VAL(PHY_PAD_CTRL_2_adrctrl_term_n	 , odt )));


	REG_WRITE( PHY_PAD_CTRL_3		/*0x09c*/, 0x00000000 );



	// default VREF lane 0:
	REG_WRITE( PHY_LANE_SEL, 0x00000000 );
	reg_read_val = calc_vref_phy_value(ddr_setup);
	reg_read_val = reg_read_val << 4 | (1<< 2);

	HAL_PRINT_DBG(">ddr_phy_cfg1 init VREF , VREF_TRAINING = 0x%x \n", reg_read_val);

	REG_WRITE( VREF_TRAINING, reg_read_val );

	// default VREF lane 1:
	REG_WRITE( PHY_LANE_SEL, 0x00000007 );
	REG_WRITE( VREF_TRAINING, reg_read_val );


	REG_WRITE( PHY_DLL_INCR_TRIM_3, 0x00000000 );
	REG_WRITE( PHY_DLL_INCR_TRIM_1, 0x00000000 );


	HAL_PRINT_DBG(">ddr_phy_cfg1 set trim\n");
	REG_WRITE( PHY_LANE_SEL, 0x00000000 );
	REG_WRITE( PHY_DLL_TRIM_1, 0x00000005 );
	REG_WRITE( PHY_DLL_TRIM_3, 0x0000000a );
	REG_WRITE( PHY_LANE_SEL, 0x00000006 );
	REG_WRITE( PHY_DLL_TRIM_1, 0x00000005 );
	REG_WRITE( PHY_DLL_TRIM_3, 0x0000000a );


	//REG_WRITE( SCL_WINDOW_TRIM, 0x07070000) ; /* NTIL:0x1f01001e ); */
	// Updated data from UNQ
	REG_WRITE( SCL_WINDOW_TRIM, 0x0a050007) ;
	// winA_extra_margin = 0x1E
	// winB_extra_margin = 0x10
	// incr_winA_val = 0x01
	// incr_winB_val = 0x1F


	HAL_PRINT_DBG(">ddr_phy_cfg1 init SCL\n");
	REG_WRITE( UNQ_ANALOG_DLL_1, 0x00000000 );
	REG_WRITE( DYNAMIC_IE_TIMER, 0x0000001a );

#if 0
	// updated 6.3.2021:
	REG_WRITE( SCL_CONFIG_1, 0x01000061 |
				BUILD_FIELD_VAL(SCL_CONFIG_1_dly_dfi_phyupd_ack, 1) |
				BUILD_FIELD_VAL(SCL_CONFIG_1_local_odt_ctrl, 4) );
				//burst8, rd_cas_latency = 6, ddr_odt_ctrl_wr

	// updated 6.3.2021:
	REG_WRITE( SCL_CONFIG_2, 0x81000601 |
						BUILD_FIELD_VAL(SCL_CONFIG_2_double_ref_dly, 1) );  /* wr_cas_latency = 6. dly_dfi_data, no overflows. */
#endif
#ifdef STATIC_SCL_CONFIG

	if (ddr_setup->dram_type_clk > DRAM_CLK_TYPE_1600)
	{
		REG_WRITE(SCL_CONFIG_1, 0x01000081 | 1 << 3 | 4 << 12);
		REG_WRITE(SCL_CONFIG_2, 0x80000701 | 1 << 12);
	}
#ifdef MC_CAPABILITY_CLK_TYPE_1600
	else
	{
		REG_WRITE(SCL_CONFIG_1, 0x01000061 | 1 << 3 | 4 << 12);
		REG_WRITE(SCL_CONFIG_2, 0x81000601 | 1 << 12);
	}
#endif
#else


	REG_WRITE(SCL_CONFIG_1, ((BUILD_FIELD_VAL(SCL_CONFIG_1_burst8,1)) | (BUILD_FIELD_VAL(SCL_CONFIG_1_ddr3, 0)) | \
							(BUILD_FIELD_VAL(SCL_CONFIG_1_dly_dfi_phyupd_ack,1)) | (BUILD_FIELD_VAL(SCL_CONFIG_1_rd_cas_latency,((ddr_setup->cas_latency>>1)+(ddr_setup->cas_latency%2)))) | \
							(BUILD_FIELD_VAL(SCL_CONFIG_1_additive_latency,ddr_setup->phy_additive_latency)) | (BUILD_FIELD_VAL(SCL_CONFIG_1_local_odt_ctrl,4)) | \
							(BUILD_FIELD_VAL(SCL_CONFIG_1_ddr_odt_ctrl_rd, 0)) | (BUILD_FIELD_VAL(SCL_CONFIG_1_ddr_odt_ctrl_wr,1)) ));							//HAL_PRINT_DBG("**************** SCL_CONFIG_1 = 0x%X \n", REG_READ(SCL_CONFIG_1));

	//	 Calucualte the the additional delay
	// Set when PHY is operating
	// in half_rate mode and write latency
	// is either even and write commands are being
	// issued on PHASE0 or odd and write commands are being issued on PHASE1.
	reg_read_val = 0;
#ifndef WRITE_COMMAND_PHASE1
	if ((ddr_setup->cas_write_latency % 2) == 0)
	{
		reg_read_val = 1;
	}
#else
	if ((ddr_setup->cas_write_latency % 2) != 0)
	{
		reg_read_val = 1;
	}
#endif

	REG_WRITE(SCL_CONFIG_2, ((BUILD_FIELD_VAL(SCL_CONFIG_2_scl_test_cs, 1)) |
	                        (BUILD_FIELD_VAL(SCL_CONFIG_2_wr_cas_latency, ((ddr_setup->cas_write_latency >> 1) + (ddr_setup->cas_write_latency % 2)))) |
				(BUILD_FIELD_VAL(SCL_CONFIG_2_double_ref_dly, 1)) |
				(BUILD_FIELD_VAL(SCL_CONFIG_2_dly_dfi_wrdata, reg_read_val)) |
				(BUILD_FIELD_VAL(SCL_CONFIG_2_lpddr2_lpddr3, 0)) |
				(BUILD_FIELD_VAL(SCL_CONFIG_2_swap_phase, 1)) ));

#endif

	REG_WRITE( SCL_CONFIG_3	  /*0x16c*/, 0x00000000 );

	if (ddr_setup->dram_type_clk > DRAM_CLK_TYPE_1600)
	{
		REG_WRITE(DYNAMIC_WRITE_BIT_LVL /*0x1c0*/, 0x0002e261);
		REG_WRITE(SCL_CONFIG_4          /*0x1bc*/, 0x00000001);
	}
#ifdef MC_CAPABILITY_CLK_TYPE_1600
	else
	{

		REG_WRITE(DYNAMIC_WRITE_BIT_LVL /*0x1c0*/, 0x0002b251);
		REG_WRITE(SCL_CONFIG_4          /*0x1bc*/, 0x00000000);
	}
#endif
	REG_WRITE( SCL_GATE_TIMING		/*0x1e0*/, 0x00000190 ); /* From Ben. NTIL: 0x000000f0 ); */

	REG_WRITE( WRLVL_DYN_ODT		 /*0x150*/, (ddr_setup->MR2_DATA) | (ddr_setup->MR2_DATA << 16));

	reg_read_val = ddr_setup->MR1_DATA;
	reg_read_val |= (reg_read_val << 16);
	reg_read_val |= (1 << 23);// output buffer enable
	REG_WRITE( WRLVL_ON_OFF, reg_read_val ); /* changed from 0x03810301 */

	//  Disable recalibration (will be init after ZQ calibration)
	if (ddr_setup->dram_type_clk  > DRAM_CLK_TYPE_1600)
	{
		// NTIL: to update for 1066
		REG_WRITE( PHY_DLL_RECALIB		, 0xac085400 );
		//REG_WRITE( PHY_DLL_RECALIB		, 0xac001000 );
	}
	else
	{
		REG_WRITE( PHY_DLL_RECALIB		, 0xac064000 );
		//REG_WRITE( PHY_DLL_RECALIB		, 0xac001000 );
	}

	CLK_Delay_MicroSec( 100 );


	dlls_trim_adrctrl_incr = dlls_trim_clk_incr = dll_recalib_trim_increment_ma = TRUE;
	dll_mas_dly = REG_READ( PHY_DLL_ADRCTRL) >> 24; // PHY_DLL_ADRCTRL_dll_mas_dly


	// If all three parameters are '0' that means use default method, same delay on all lines and support only increment
	if ((ddr_setup->dlls_trim_adrctrl == 0) && (ddr_setup->dlls_trim_clk == 0) && (ddr_setup->dll_recalib_trim_adrctrl_ma == 0))
	{
		// Original recomendation use 1/4 clock cycle as reference due to the low value in bit-leveling
		extra_addrctrl_dly = (dll_mas_dly >> 2) ;
		HAL_PRINT_DBG(">ddr_phy_cfg1 dll_mas_dly %d extra_addrctrl_dly %d (1/4 clock)\n", dll_mas_dly, extra_addrctrl_dly);
		// By default, all trimming parameters have the same value.
		dlls_trim_adrctrl = dlls_trim_clk = dll_recalib_trim_adrctrl_ma = extra_addrctrl_dly;
		dlls_trim_adrctrl_incr = dlls_trim_clk_incr = dll_recalib_trim_increment_ma = TRUE;
	}
	else
	{
		// Per UNQ recomendation use 1/2 clock cycle as reference due to the low value in bit-leveling
		extra_addrctrl_dly = (dll_mas_dly >> 1) ;
		dlls_trim_adrctrl = dlls_trim_clk = dll_recalib_trim_adrctrl_ma = extra_addrctrl_dly;
		HAL_PRINT_DBG(">ddr_phy_cfg1 dll_mas_dly %d,  extra_addrctrl_dly %d \n", dll_mas_dly, extra_addrctrl_dly);

		dlls_trim_adrctrl += ddr_setup->dlls_trim_adrctrl;
		dlls_trim_adrctrl_incr = ddr_setup->dlls_trim_adrctrl_incr;

		dlls_trim_clk += ddr_setup->dlls_trim_clk;
		dlls_trim_clk_incr = ddr_setup->dlls_trim_clk_incr;

		dll_recalib_trim_adrctrl_ma += ddr_setup->dll_recalib_trim_adrctrl_ma;
		dll_recalib_trim_increment_ma = ddr_setup->dll_recalib_trim_increment_ma;
	}

	// override from header:
	if (ddr_setup->hdr_dlls_trim_adrctl != 0xFF)
	{
		dlls_trim_adrctrl = ddr_setup->hdr_dlls_trim_adrctl  & MASK_FIELD(PHY_DLL_ADRCTRL_dlls_trim_adrctrl);
		dlls_trim_adrctrl_incr    =  READ_VAR_BIT (ddr_setup->hdr_dlls_trim_adrctl, 6);
		HAL_PRINT(KMAG "Override dlls_trim_adrctrl with %c%d  (0x%x)\n",
			dlls_trim_adrctrl_incr? '+': '-',  dlls_trim_adrctrl, dlls_trim_adrctrl);
	}

	// override from header:
	if (ddr_setup->hdr_dlls_trim_clk != 0xFF)
	{
		dlls_trim_clk = ddr_setup->hdr_dlls_trim_clk  & MASK_FIELD(PHY_DLL_TRIM_CLK_dlls_trim_clk);
		dlls_trim_clk_incr = READ_VAR_BIT (ddr_setup->hdr_dlls_trim_clk, 7);
		HAL_PRINT(KMAG "Override dlls_trim_clk with %c%d\n",
			MC_SignToChar(dlls_trim_clk_incr), dlls_trim_clk);
	}

	// override from header:
	if (ddr_setup->hdr_dlls_trim_adrctrl_ma != 0xFF)
	{
		dll_recalib_trim_adrctrl_ma   =          ddr_setup->hdr_dlls_trim_adrctrl_ma  & MASK_FIELD(PHY_DLL_RECALIB_dlls_trim_adrctrl_ma);
		dll_recalib_trim_increment_ma =  READ_VAR_BIT(ddr_setup->hdr_dlls_trim_adrctrl_ma, 6);
		HAL_PRINT(KMAG "Override dlls_trim_adrctrl_ma with %c%d (0x%x)\n",
			MC_SignToChar(dll_recalib_trim_increment_ma),  dll_recalib_trim_adrctrl_ma, dll_recalib_trim_adrctrl_ma);
	}

	// protect from overflow:
	dlls_trim_adrctrl &= MASK_FIELD(PHY_DLL_ADRCTRL_dlls_trim_adrctrl);
	dlls_trim_clk   &=  MASK_FIELD(PHY_DLL_TRIM_CLK_dlls_trim_clk);
	dll_recalib_trim_adrctrl_ma &=  MASK_FIELD(PHY_DLL_RECALIB_dlls_trim_adrctrl_ma);


	REG_WRITE( PHY_DLL_ADRCTRL, ((dlls_trim_adrctrl_incr << 9) | dlls_trim_adrctrl) );
	HAL_PRINT_DBG(KNRM ">ddr_phy_cfg1 set dlls_trim_adrctrl %c%d \n", MC_SignToChar(dlls_trim_adrctrl_incr), dlls_trim_adrctrl);


	REG_WRITE( PHY_LANE_SEL, 0x00000000 );
	// JACOB: REG_WRITE( UNIQUIFY_IO_1, 0x00000001);

	REG_WRITE( PHY_DLL_TRIM_CLK, ((dlls_trim_clk_incr << 7) | dlls_trim_clk) );
	HAL_PRINT_DBG(">ddr_phy_cfg1 dlls_trim_clk	%c%d (reg=0x%x)\n", MC_SignToChar(dlls_trim_clk_incr), dlls_trim_clk, REG_READ (PHY_DLL_TRIM_CLK));
	if (ddr_setup->dram_type_clk > DRAM_CLK_TYPE_1600)
	{
		REG_WRITE( PHY_DLL_RECALIB, ((0xA << 28)|(dll_recalib_trim_increment_ma<<27)|(1<<26)|(0x854<<8) | dll_recalib_trim_adrctrl_ma) ); /* replace 0x10 with 0x640 */
	}
#ifdef MC_CAPABILITY_CLK_TYPE_1600
	else
	{
		REG_WRITE( PHY_DLL_RECALIB, ((0xA << 28)|(dll_recalib_trim_increment_ma<<27)|(1<<26)|(0x640<<8) | dll_recalib_trim_adrctrl_ma) ); /* replace 0x10 with 0x640 */
	}
#endif
	HAL_PRINT_DBG(">ddr_phy_cfg1 dlls_trim_adrctrl_ma %c%d\n",
		MC_SignToChar(READ_REG_FIELD(PHY_DLL_RECALIB, PHY_DLL_RECALIB_incr_dly_adrctrl_ma)),
		READ_REG_FIELD(PHY_DLL_RECALIB, PHY_DLL_RECALIB_dlls_trim_adrctrl_ma));


	REG_WRITE(UNIQUIFY_IO_1, (0x00000001 | UNQ_IO_1_MASK));
	REG_WRITE( SCL_LATENCY, 0x00035076 ); /* SCL_LATENCY_wbl_jitter_threshold = 3 */
	REG_WRITE(BIT_LVL_CONFIG, ((0x1 << 4) | BIT_LVL_SAMPLE_QTY)); /* BIT_LVL_CONFIG_bit_lvl_edge_sel = 1, BIT_LVL_CONFIG_bit_lvl_sample_qty = 2 */

	// Updated flow from uniquify

	REG_WRITE(UNIQUIFY_IO_2, (BUILD_FIELD_VAL(UNIQUIFY_IO_2_cal_ppu_0p8_offset, ddr_setup->phy_cal_ppu_op8_offset) |
				BUILD_FIELD_VAL(UNIQUIFY_IO_2_incr_cal_ppu_0p8, ddr_setup->phy_incr_cal_ppu_op8) |
				BUILD_FIELD_VAL(UNIQUIFY_IO_2_override_cal_ppu_0p8, 0) |
				BUILD_FIELD_VAL(UNIQUIFY_IO_2_cal_npd_0p8_offset, ddr_setup->phy_cal_npd_offset_op8) |
				BUILD_FIELD_VAL(UNIQUIFY_IO_2_incr_cal_npd_0p8, ddr_setup->phy_incr_cal_npd_op8) |
				BUILD_FIELD_VAL(UNIQUIFY_IO_2_override_cal_npd_0p8, 0) |
				BUILD_FIELD_VAL(UNIQUIFY_IO_2_cal_rx_offset, 0) |
				BUILD_FIELD_VAL(UNIQUIFY_IO_2_incr_cal_rx, 0) |
				BUILD_FIELD_VAL(UNIQUIFY_IO_2_override_cal_rx, 0)|
				BUILD_FIELD_VAL(UNIQUIFY_IO_2_reserved1, 0)));

	SET_REG_FIELD(UNIQUIFY_IO_1, UNIQUIFY_IO_1_update, 1);
	SET_REG_FIELD(UNIQUIFY_IO_1, UNIQUIFY_IO_1_update, 0);

	//-------- Done updated flow from uniquify

	REG_WRITE( UNIQUIFY_IO_3, 0x08009200 ); /* new reg */

	HAL_PRINT_DBG(">ZQ calib rx\n");
	REG_WRITE( UNIQUIFY_IO_1, (0x00000002 | UNQ_IO_1_MASK)); /* UNIQUIFY_IO_1_start_rx = 1 */
	CLK_Delay_MicroSec( 100 );

	// TaliP : added wait for calibration complition.Increase TO?
	timeout = 60000;
	while(1)
	{
		if ( READ_REG_FIELD(UNIQUIFY_IO_1, UNIQUIFY_IO_1_done_rx_seen)  != 0 )
			break;

		if (!timeout)
		{
			HAL_PRINT(KRED "\t>MC phy init ZQ calib to. UNIQUIFY_IO_1  %#010lx\n" KNRM, REG_READ(UNIQUIFY_IO_1));
			break;
		}
		timeout--;
	}

	REG_WRITE( UNIQUIFY_IO_1, (0x00000000 | UNQ_IO_1_MASK));
	CLK_Delay_MicroSec( 100 );

	// Updated flow from UNQ
	SET_REG_FIELD(UNIQUIFY_IO_1, UNIQUIFY_IO_1_update, 1);
	SET_REG_FIELD(UNIQUIFY_IO_1, UNIQUIFY_IO_1_update, 0);

	//-------- Done new flow from UNQ

	HAL_PRINT_DBG(">ZQ calib tx\n");
	REG_WRITE( UNIQUIFY_IO_3, 0x08081300 );
	REG_WRITE( UNIQUIFY_IO_1, (0x00000001 |  UNQ_IO_1_MASK)); /* UNIQUIFY_IO_1_start_tx = 1 */
	CLK_Delay_MicroSec( 100 );
	timeout = 60000;
	while(1)
	{
		if ( READ_REG_FIELD(UNIQUIFY_IO_1, UNIQUIFY_IO_1_done_tx_seen)  != 0 )
			break;

		if (!timeout)
		{
			HAL_PRINT(KRED "\t>MC phy init ZQ calib to. UNIQUIFY_IO_1  %#010lx\n" KNRM, REG_READ(UNIQUIFY_IO_1));
			break;

		}
		timeout--;
	}


	if(ddr_setup->dqs_in_lane0    != 0xFF)
		Set_DQS_in_val(  0 , ddr_setup->dqs_in_lane0 , FALSE, TRUE);
	if(ddr_setup->dqs_in_lane1    != 0xFF)
		Set_DQS_in_val(  1 , ddr_setup->dqs_in_lane1 , FALSE, TRUE);
	if(ddr_setup->dqs_out_lane0   != 0xFF)
		Set_DQS_out_val( 0 , ddr_setup->dqs_out_lane0, FALSE, TRUE);
	if(ddr_setup->dqs_out_lane1   != 0xFF)
		Set_DQS_out_val( 1 , ddr_setup->dqs_out_lane1, FALSE, TRUE);

	/* Manual override of trims using IGPS header parameters: */
	for (ilane = 0 ; ilane < 2 ; ilane++)
	{
		REG_WRITE( PHY_LANE_SEL , 0x00000005 * ilane );
		if(ddr_setup->phase1[ilane]       != 0xFF)
		{
			HAL_PRINT(KMAG "Lane%d: set phase1 to %#010lx\n" KNRM, ilane, ddr_setup->phase1[ilane]);
			SET_REG_FIELD(UNQ_ANALOG_DLL_3, UNQ_ANALOG_DLL_3_phase1, ddr_setup->phase1[ilane]);
		}

		if(ddr_setup->phase2[ilane]       != 0xFF)
		{
			HAL_PRINT(KMAG "Lane%d: set phase2 to %#010lx\n" KNRM, ilane, ddr_setup->phase2[ilane]);
			SET_REG_FIELD(UNQ_ANALOG_DLL_3, UNQ_ANALOG_DLL_3_phase2, ddr_setup->phase2[ilane]);
		}


		REG_WRITE( PHY_LANE_SEL , 0x00000006 * ilane );
		if(ddr_setup->dlls_trim_1[ilane] != 0xFF)
		{
			char incr_letter = MC_SignToChar(ddr_setup->dlls_trim_1[ilane] & 0x80);
			
			HAL_PRINT(KMAG "Lane%d: set dlls_trim_1 to %c%d\n" KNRM, ilane, incr_letter, ddr_setup->dlls_trim_1[ilane] & 0x7F);
			SET_REG_FIELD(PHY_DLL_TRIM_1, PHY_DLL_TRIM_1_dlls_trim_1, ddr_setup->dlls_trim_1[ilane]);
			if (incr_letter == '+')
			{
				SET_REG_BIT(PHY_DLL_INCR_TRIM_1, ilane);
			}
			else
			{
				CLEAR_REG_BIT(PHY_DLL_INCR_TRIM_1, ilane);
			}
		}

		if(ddr_setup->dlls_trim_2[ilane] != 0xFF)
		{
			HAL_PRINT(KMAG "Lane%d: set dlls_trim_2 to %d\n" KNRM, ilane, ddr_setup->dlls_trim_2[ilane]);
			SET_REG_FIELD(PHY_DLL_TRIM_2, PHY_DLL_TRIM_2_dlls_trim_2, ddr_setup->dlls_trim_2[ilane]);
		}

		if(ddr_setup->dlls_trim_3[ilane] != 0xFF)
		{
			char incr_letter = MC_SignToChar (ddr_setup->dlls_trim_3[ilane] & 0x80);
			
			HAL_PRINT(KMAG "Lane%d: set dlls_trim_3 to %c%d\n" KNRM, ilane, incr_letter, ddr_setup->dlls_trim_3[ilane] & 0x7F);
			SET_REG_FIELD(PHY_DLL_TRIM_3, PHY_DLL_TRIM_3_dlls_trim_3, ddr_setup->dlls_trim_3[ilane]);
			if (incr_letter == '+')
			{
				SET_REG_BIT(PHY_DLL_INCR_TRIM_3, ilane);
			}
			else
			{
				CLEAR_REG_BIT(PHY_DLL_INCR_TRIM_3, ilane);
			}
		}
	}

	REG_WRITE( UNIQUIFY_IO_1 ,  UNQ_IO_1_MASK);
	CLK_Delay_MicroSec( 100 );

	// Do update Set to force the calibrated settings to  be pushed to all the other I / Oâ€™s.If  not set, PHY periodically performs  updates every recalib_cnt interval,
	//   when dfi_phyupd_req is asserted    with dfi_phyupd_type set to 1.
	SET_REG_BIT( UNIQUIFY_IO_1   , 4 );
	CLEAR_REG_BIT( UNIQUIFY_IO_1 , 4 );
	CLK_Delay_MicroSec( 100 );


	// This bit disable_recalib Prevents the digital DLL from recalibrating after the 1st time.
	// Clear it to enable re-claibration according to interval
	CLEAR_REG_BIT( PHY_DLL_RECALIB , 26); // PHY_DLL_RECALIB_disable_recalib

	CLK_Delay_MicroSec( 100 );

	timeout = 600000;
	while(1)
	{
		if ( READ_REG_FIELD(UNQ_ANALOG_DLL_2, UNQ_ANALOG_DLL_2_analog_dll_lock)  == 0x3 )
			break;

		if (!timeout)
		{
			HAL_PRINT(KRED "\t>UNQ_ANALOG_DLL_2 ZQ calib to. UNQ_ANALOG_DLL_2  %#010lx\n" KNRM, REG_READ(UNQ_ANALOG_DLL_2));
			break;

		}
		timeout--;
	}



	HAL_PRINT_DBG("UNIQUIFY_IO_2: cal_trim_rx_saved 0x%x\n", (REG_READ(UNIQUIFY_IO_2)>>25 & 0x1F));
	HAL_PRINT_DBG("UNIQUIFY_IO_4: cal_trim_npu_saved 0x%x\n", (REG_READ(UNIQUIFY_IO_4) & 0x3F));
	HAL_PRINT_DBG("UNIQUIFY_IO_4: cal_trim_ppu_saved 0x%x\n", (REG_READ(UNIQUIFY_IO_4)>>6 & 0x3F));
	HAL_PRINT_DBG("UNIQUIFY_IO_4: cal_trim_npd_saved 0x%x\n", (REG_READ(UNIQUIFY_IO_4)>>12 & 0x3F));
	HAL_PRINT_DBG("UNIQUIFY_IO_4: cal_trim_ppu_0p8_saved 0x%x\n", (REG_READ(UNIQUIFY_IO_4)>>18 & 0x3F));
	HAL_PRINT_DBG("UNIQUIFY_IO_4: cal_trim_npd_0p8_saved 0x%x\n", (REG_READ(UNIQUIFY_IO_4)>>24 & 0x3F));

	HAL_PRINT_DBG("\n>ddr_phy_cfg1 done\n");
	return DEFS_STATUS_OK;

}
