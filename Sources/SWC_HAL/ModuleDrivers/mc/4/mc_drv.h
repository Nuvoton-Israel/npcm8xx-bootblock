/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   mc_drv.h                                                                 */
/*            This file contains MC driver interface                          */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/

#ifndef MC_DRV_H
#define MC_DRV_H

#include __MODULE_IF_HEADER_FROM_DRV(mc)



DEFS_STATUS      ddr_phy_cfg1 (DDR_Setup *ddr_setup);
DEFS_STATUS      ddr_phy_cfg2 (DDR_Setup *ddr_setup);
void             arbel_mc_init (DDR_Setup *ddr_setup);



#define ADRCTL_MA_SWEEP       MASK_BIT(0)
#define ADRCTL_SWEEP          MASK_BIT(1)
#define TRIM_2_LANE0_SWEEP    MASK_BIT(2)
#define TRIM_2_LANE1_SWEEP    MASK_BIT(3)
#define VREF_SWEEP            MASK_BIT(4)
#define DRAM_SWEEP            MASK_BIT(5)
#define OP_DQS_SWEEP          MASK_BIT(6)
#define IP_DQS_SWEEP          MASK_BIT(7)

#define ENHANCED_SWEEPING_AND_LEVELING
DEFS_STATUS           Sweep_adrctrl_ma_l         (void);
DEFS_STATUS           Sweep_adrctrl_l         (void);
DEFS_STATUS           Sweep_trim2_lane_l         (UINT16 TRIM2_ilane);
DEFS_STATUS           Sweep_VREF_l         (void);
DEFS_STATUS           Sweep_DRAM_l         (DDR_Setup *ddr_setup);
DEFS_STATUS           Sweep_OP_DQS_l         (void);
DEFS_STATUS           Sweep_IP_DQS_l         (void);



DEFS_STATUS           Sweep_DQS_Trim_l          (DDR_Setup *ddr_setup, int SweepType, BOOLEAN DoCenter, UINT16 SweepBitMask);
DEFS_STATUS           Sweep_DQn_Trim_l          (DDR_Setup *ddr_setup, int SweepType, BOOLEAN DoCenter, UINT16 SweepBitMask);
UINT16                MC_MemStressTestLong      (BOOLEAN bECC, BOOLEAN bQuick, UINT16 SweepBitMask, BOOLEAN abort_on_error);



#define SWEEP_OUT_DQ    0 // ber bit param;  negative/positive -63 to 63
#define SWEEP_IN_DQ     1 // ber bit param;  negative/positive -63 to 63
#define SWEEP_OUT_DM    2 // ber lane param; negative/positive -63 to 63
#define SWEEP_OUT_DQS   3 // ber lane param; positive
#define SWEEP_IN_DQS    4 // ber lane param; positive
#define SWEEP_OUT_LANE  5 // ber lane param (TRIM2); positive


#endif //MC_DRV_H

