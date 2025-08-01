/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   arbel_mc_security.c                                                      */
/*            This file contains MC module security settings                  */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/

#define MC_RANGE_IN_USE 4
#define MC_PORTS 15

const UINT32 mc_range_partitioning[MC_RANGE_IN_USE][2] = 
				{{0, 16 * _1MB_},     // GFX
				{_16MB_, _32MB_},     // TIP only
				{_32MB_, 96 * _1MB_}, // BL31\OpTEE
				{96 * _1MB_, 0x0}};   // Till end of device
				
const UINT8 mc_range_prot[MC_PORTS][MC_RANGE_IN_USE] = {    
	// Port 0 GFX1
	{RANGE_FULL_ACCESS_RW, RANGE_FULL_ACCESS_RW_DISABLED,  RANGE_FULL_ACCESS_RW_DISABLED, RANGE_FULL_ACCESS_RW},
	// Port 1 A35
	{RANGE_FULL_ACCESS_RW, RANGE_FULL_ACCESS_RW_DISABLED,  RANGE_SECURE_ACCESS_RW,        RANGE_FULL_ACCESS_RW},
	// Port 2 GFX0
	{RANGE_FULL_ACCESS_RW, RANGE_FULL_ACCESS_RW_DISABLED,  RANGE_FULL_ACCESS_RW_DISABLED, RANGE_FULL_ACCESS_RW},
	// Port 3 PCIRC                                                                       
	{RANGE_FULL_ACCESS_RW, RANGE_FULL_ACCESS_RW_DISABLED,  RANGE_FULL_ACCESS_RW_DISABLED, RANGE_FULL_ACCESS_RW},
	// Port 4 USBD_EMMC                                                                   
	{RANGE_FULL_ACCESS_RW, RANGE_FULL_ACCESS_RW_DISABLED,  RANGE_FULL_ACCESS_RW_DISABLED, RANGE_FULL_ACCESS_RW},
	// Port 5 USB Host                                                                    
	{RANGE_FULL_ACCESS_RW, RANGE_FULL_ACCESS_RW_DISABLED,  RANGE_FULL_ACCESS_RW_DISABLED, RANGE_FULL_ACCESS_RW},
	// Port 6 CoP                                                                         
	{RANGE_FULL_ACCESS_RW, RANGE_FULL_ACCESS_RW_DISABLED,  RANGE_FULL_ACCESS_RW_DISABLED, RANGE_FULL_ACCESS_RW},
	// Port 7 GMAC1                                                                       
	{RANGE_FULL_ACCESS_RW, RANGE_FULL_ACCESS_RW_DISABLED,  RANGE_FULL_ACCESS_RW_DISABLED, RANGE_FULL_ACCESS_RW},
	// Port 8 GMAC2                                                                       
	{RANGE_FULL_ACCESS_RW, RANGE_FULL_ACCESS_RW_DISABLED,  RANGE_FULL_ACCESS_RW_DISABLED, RANGE_FULL_ACCESS_RW},
	// Port 9 GMAC34_GDMA_VDMA                                                            
	{RANGE_FULL_ACCESS_RW, RANGE_FULL_ACCESS_RW_DISABLED,  RANGE_FULL_ACCESS_RW_DISABLED, RANGE_FULL_ACCESS_RW},
	// Port 10 ECE                                                                        
	{RANGE_FULL_ACCESS_RW, RANGE_FULL_ACCESS_RW_DISABLED,  RANGE_FULL_ACCESS_RW_DISABLED, RANGE_FULL_ACCESS_RW},
	// Port 11 VCD1                                                                       
	{RANGE_FULL_ACCESS_RW, RANGE_FULL_ACCESS_RW_DISABLED,  RANGE_FULL_ACCESS_RW_DISABLED, RANGE_FULL_ACCESS_RW},
	// Port 12 VCD2                                                                       
	{RANGE_FULL_ACCESS_RW, RANGE_FULL_ACCESS_RW_DISABLED,  RANGE_FULL_ACCESS_RW_DISABLED, RANGE_FULL_ACCESS_RW},
	// Port 13 TIP                                                                        
	{RANGE_FULL_ACCESS_RW, RANGE_FULL_ACCESS_RW,           RANGE_FULL_ACCESS_RW,          RANGE_FULL_ACCESS_RW},
	// Port 14 - FLM                                                                      
	{RANGE_FULL_ACCESS_RW, RANGE_FULL_ACCESS_RW_DISABLED,  RANGE_FULL_ACCESS_RW_DISABLED, RANGE_FULL_ACCESS_RW},
};

static void print_mode(UINT8 mode)
{
	switch (mode)
	{
		case RANGE_FULL_ACCESS_RW:
			HAL_PRINT_DBG("FULL_ACCESS_RW     ");
			break;
		case RANGE_FULL_ACCESS_RO:
			HAL_PRINT_DBG("FULL_ACCESS_RO     ");
			break;
		case RANGE_FULL_ACCESS_WO:
			HAL_PRINT_DBG("FULL_ACCESS_WO     ");
			break;
		case RANGE_FULL_ACCESS_RW_DISABLED:
			HAL_PRINT_DBG("FULL_ACCESS_RW_DIS ");
			break;
		case RANGE_SECURE_ACCESS_RW:
			HAL_PRINT_DBG("SECURE_ACCESS_RW   ");
			break;
		case RANGE_SECURE_ACCESS_RO:
			HAL_PRINT_DBG("SECURE_ACCESS_RO   ");
			break;
		case RANGE_SECURE_ACCESS_WO:
			HAL_PRINT_DBG("SECURE_ACCESS_WO   ");
			break;
		case RANGE_UNDEFINED:
			HAL_PRINT_DBG("UNDEF              ");
			break;
		default:
			HAL_PRINT_DBG("Unknown Mode");
			break;
	}
}

static void mc_configure_security (void)
{
	UINT32 ranges_addr = 0;
	UINT32 ranges_prot_addr = 0;
	
	HAL_PRINT_DBG("Secure regions cfg\n");
	
	// Set the Start/End addr of all ranges on all ports
	// Loop on all ranges in use
	for (int prt = 0; prt < MC_PORTS; prt++)
	{
		HAL_PRINT_DBG("Port %d\n", prt);
		for (int rng = 0; rng < 16; rng++)
		{
			HAL_PRINT_DBG("\tRange %d ", rng);
			// Range start address:
			ranges_addr = RANGE_START_ADDRESS(prt, rng);
			HAL_PRINT_DBG("\taddr %#010lx ", ranges_addr);
			if (rng < MC_RANGE_IN_USE)
			{
				*(UINT32 *)ranges_addr = (mc_range_partitioning[rng][0]) >> 14;
				HAL_PRINT_DBG(" %#010lx ", *(UINT32 *)ranges_addr); 
			}
			else
			{
				*(UINT32 *)ranges_addr = 0xFFFFF;
				HAL_PRINT_DBG(" 0x000FFFFF "); 
			}

			// Range end address
			ranges_addr = RANGE_END_ADDRESS(prt, rng);
			HAL_PRINT_DBG(": addr %#010lx ", ranges_addr);
			if (rng < MC_RANGE_IN_USE - 1)
			{
				*(UINT32 *)ranges_addr = (mc_range_partitioning[rng][1] - 1) >> 14;
				HAL_PRINT_DBG(" %#010lx\n", *(UINT32 *)ranges_addr);
			}
			else
			{
				*(UINT32 *)ranges_addr = 0xFFFFF;
				HAL_PRINT_DBG(" 0x000FFFFF \n");
			}
		}
	}

	HAL_PRINT_DBG("Done\n\n");
	// Port/Range security configuration 
	for (int prt = 0; prt < MC_PORTS; prt++)
	{
		HAL_PRINT_DBG("\nPort %d\n", prt);
		UINT32 temp_addr = 0;
		for (int rng = 0; rng < MC_RANGE_IN_USE; rng++)
		{
			HAL_PRINT_DBG("\tRange %d ", rng);
			print_mode(mc_range_prot[prt][rng]);
			// Set Access
			ranges_addr = RANGE_PROT_ADDRESS(prt, rng);
			*(UINT32 *)ranges_addr |= RANGE_PROT_MASK(prt, rng, GET_ACCESS_BY_MODE(mc_range_prot[prt][rng]));
			HAL_PRINT_DBG("\tPROT addr %#010lx %#010lx\t", ranges_addr, *(UINT32 *)ranges_addr); 
			ranges_addr = RANGE_RID_ADDRESS(prt,rng);
			*(UINT32 *)ranges_addr |= RANGE_RID_MASK(rng, GET_RID_BY_MODE(mc_range_prot[prt][rng]));
			HAL_PRINT_DBG(" RID addr %#010lx %#010lx\t", ranges_addr, *(UINT32 *)ranges_addr); 
			ranges_addr = RANGE_WID_ADDRESS(prt,rng);
			*(UINT32 *)ranges_addr |= RANGE_WID_MASK(rng, GET_WID_BY_MODE(mc_range_prot[prt][rng]));
			HAL_PRINT_DBG(" WID addr %#010lx %#010lx\n", ranges_addr, *(UINT32 *)ranges_addr); 
		}

		for (int rng = MC_RANGE_IN_USE; rng < 16; rng++)
		{
			HAL_PRINT_DBG("\tRange %d ", rng);
			print_mode(RANGE_UNDEFINED);
			
			// Undefined range keep are meaningless but must be initialized as well 
			ranges_addr = RANGE_PROT_ADDRESS(prt, rng);
			*(UINT32 *)ranges_addr |= RANGE_PROT_MASK(prt, rng, GET_ACCESS_BY_MODE(RANGE_UNDEFINED));
			HAL_PRINT_DBG("\tPROT addr %#010lx %#010lx\t", ranges_addr, *(UINT32 *)ranges_addr); 
			ranges_addr = RANGE_RID_ADDRESS(prt,rng);
			*(UINT32 *)ranges_addr |= RANGE_RID_MASK(rng, PORTz_RID_WID_ALLOW_NONE);
			HAL_PRINT_DBG(" RID addr %#010lx %#010lx\t", ranges_addr, *(UINT32 *)ranges_addr); 
			ranges_addr = RANGE_WID_ADDRESS(prt,rng);
			*(UINT32 *)ranges_addr |= RANGE_WID_MASK(rng, PORTz_RID_WID_ALLOW_NONE);
			HAL_PRINT_DBG(" WID addr %#010lx %#010lx\n", ranges_addr, *(UINT32 *)ranges_addr); 
		}
	}
	// Configuration is done. Enable security
	SET_REG_FIELD(DENALI_CTL_198, DENALI_CTL_198_PORT_ADDR_PROTECTION_EN, 0x1);
	
}