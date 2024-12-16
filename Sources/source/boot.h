/*----------------------------------------------------------------------------*/
/*   SPDX-License-Identifier: GPL-2.0                                         */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   boot.h                                                                   */
/*            This file contains API of routines to boot an image             */
/*  Project:                                                                  */
/*            Arbel BootBlock A35                                             */
/*----------------------------------------------------------------------------*/

#ifndef _BOOT_H
#define _BOOT_H

#include "hal.h"
#if  defined(DEBUG_LOG) || defined(DEV_LOG)
#include "apps/serial_printf/serial_printf.h"
#else
#define         serial_printf(x,...)                   (void)0
#define         serial_printf_debug(x,...)             (void)0
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* TIP Image structure                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/

#pragma pack(1)
typedef struct TIP_HEADER_STRUCT_Tag
{                                                   // Offset    Size(bytes)
    UINT32  anchor;                                 // 0         4
    UINT32  ext_anchor;                             // 4         4
    UINT32  reserved;                               // 8         4
    UINT32  img_crc;                                // 12        4
    UINT8   signature[96];                          // 16        96
    UINT8   spi0_flash_clk;                         // 112       1
    UINT8   spi1_flash_clk;                         // 113       1
    UINT8   spi3_flash_clk;                         // 114       1
    UINT8   reserved_0[3];                          // 115       3
    UINT16  spi_flash_rd_mode;                      // 118       2
    UINT32  load_start_addr;                        // 120       4
    UINT32  bmc_bb_pointer;                         // 124       4
    UINT32  bmc_bb_length;                          // 128       4
    UINT32  img_length;                             // 132       4
    UINT8   reserved_1;                             // 136       1
    UINT8   reserved_2[3];                          // 137       3
    UINT32  key_index;                              // 140       4
    UINT32  key_invalid;                            // 144       4
    UINT32  reserved_3;                             // 148       4
    UINT16  otp_version;                            // 152       2
    UINT16  minor_version;                          // 154       2
    UINT32  active_img_table;                       // 156       4
    UINT32  copy1_img_table;                        // 160       4
    UINT32  copy2_img_table;                        // 164       4
    UINT32  fw_table_pointer;                       // 168       4
    UINT32  challenge_pointer;                      // 172       4

    // used by TIP FW only:
    UINT32  uboot_CodePointerDRAM;                  // 176       4
    UINT32  uboot_CodeSize;                         // 180       4

    UINT8   reserved_4[72];                         // 184       72

}  TIP_HEADER_STRUCT_T;
#pragma pack()



#pragma pack(1)
typedef union IMG_HEADER_tag
{
    TIP_HEADER_STRUCT_T header ;

    UINT8    bytes[256];

    UINT16   words[128];

    UINT32   dwords[64];
}  IMG_HEADER_T;
#pragma pack()


/*---------------------------------------------------------------------------------------------------------*/
/* Boot block image structure                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#pragma pack(1)
typedef union BOOTBLOCK_HEADER_tag
{
	struct
	{
		                                       // Offset       Size(bytes) Description
		UINT8   startTag[8];                   // 0            8           AA55_0850h, 424F_4F54h (‘BOOT’)
		UINT8   signature[96];                 // 8            256         Boot Block ECC signature decrypted with Customer’s Private Key

		UINT8   reserved[152];                 // 0x68         152         Reserved for future use, signed.
		//  Start signed area:
		UINT32  version;                       // 0x100       4           Version (Major.Minor)
		UINT32  vendor;                        // 0x104       4
		UINT32  board_type;                    // 0x108       4

		UINT16  mc_freq;                       // 0x10C       2
		UINT16  cpu_freq;                      // 0x10E       2

		// ddr parameters
		UINT32  soc_drive;                     // 0x110       4
		UINT32  soc_odt;                       // 0x114       4
		UINT32  dram_drive;                    // 0x118       4
		UINT32  dram_odt;                      // 0x11C       4

		UINT32  NoECC_Region_0_Start;          // 0x120       4
		UINT32  NoECC_Region_0_End;            // 0x124       4
		UINT32  NoECC_Region_1_Start;          // 0x128       4
		UINT32  NoECC_Region_1_End;            // 0x12C       4
		UINT32  dram_max_size;                 // 0x130       4
		UINT8   mc_config;                     // 0x134       1
		UINT8   host_if;                       // 0x135       1

		UINT8   mc_dqs_in_lane0;               // 0x136       1
		UINT8   mc_dqs_in_lane1;               // 0x137       1
		UINT8   mc_dqs_out_lane0;              // 0x138       1
		UINT8   mc_dqs_out_lane1;              // 0x139       1

		UINT8   mc_dlls_trim_adrctl;           // 0x13A       1
		UINT8   mc_dlls_trim_adrctrl_ma;       // 0x13B       1
		UINT8   mc_dlls_trim_clk;              // 0x13C       1

		UINT8   mc_dlls_trim_clk_sqew;         // 0x13D       1

		UINT8   mc_phase1[2];                     // 0x13E       2
		UINT8   mc_phase2[2];                     // 0x140       2
		UINT8   mc_dlls_trim_1[2];                // 0x142       2
		UINT8   mc_dlls_trim_2[2];                // 0x144       2
		UINT8   mc_dlls_trim_3[2];                // 0x146       2

		UINT8   mc_trim2_init_offset[2];          // 0x148       2
		UINT8   mc_vref_soc[2];                   // 0x14A       2

		UINT8   mc_vref_dram;                   // 0x14C       1
		UINT8   mc_sweep_debug;                 // 0x14D       1
		UINT8   mc_sweep_main_flow;             // 0x14E       1

		UINT8   fiu0_divider;                   // 0x14F       1
		UINT8   fiu1_divider;                   // 0x150       1
		UINT8   fiu3_divider;                   // 0x151       1
		UINT8   gmmap;                          // 0x152       1
		UINT8   i3c_rc_divider;                 // 0x153       1
		UINT32  baud;                           // 0x154       4

		UINT32  fiu_cfg_drd_set[3];             // 0x158       4 * 3

		UINT16  mc_gpio_test_complete;          // 0x164       2
		UINT16  mc_gpio_test_pass;              // 0x166       2

		UINT32  NoECC_Region_2_Start;          // 0x168       4
		UINT32  NoECC_Region_2_End;            // 0x16C       4
		UINT32  NoECC_Region_3_Start;          // 0x170       4
		UINT32  NoECC_Region_3_End;            // 0x174       4
		UINT32  NoECC_Region_4_Start;          // 0x178       4
		UINT32  NoECC_Region_4_End;            // 0x17C       4
		
		UINT32  NoECC_Region_5_Start;          // 0x180       4
		UINT32  NoECC_Region_5_End;            // 0x184       4
		UINT32  NoECC_Region_6_Start;          // 0x188       4
		UINT32  NoECC_Region_6_End;            // 0x18C       4
		UINT32  NoECC_Region_7_Start;          // 0x190       4
		UINT32  NoECC_Region_7_End;            // 0x194       4
		UINT32  pll0_override;                 // 0x198       4
		
		UINT8   reservedSigned3[0x5C];         // 0x202       0x5C        Reserved for future use, signed.

 		UINT32  destAddr;                      // 0x1F8
 		UINT32  codeSize;                      // 0x1FC
 	} header ;

	UINT8    bytes[512];

	UINT32   words[128];

}  BOOTBLOCK_HEADER_T;
#pragma pack()


/*---------------------------------------------------------------------------------------------------------*/
/* Board type                                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum   BOARD_T
{
	BOARD_DARBEL    = 0,
	BOARD_SVB       = 1,
	BOARD_EB        = 2,
	BOARD_MS        = 3,
	BOARD_RUN_BMC   = 10,
	BOARD_UNKNOWN   = -1,

} BOARD_T;


/*---------------------------------------------------------------------------------------------------------*/
/* Vendor (of the board)                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum   BOARD_VENDOR_T
{
	VENDOR_CUSTOMER_0  = 0,  // use external customization file : bootblock_$vendor$_cutomoize.c
	VENDOR_CUSTOMER_1  = 1,  // use external customization file : bootblock_$vendor$_cutomoize.c
	VENDOR_CUSTOMER_2  = 2,  // use external customization file : bootblock_$vendor$_cutomoize.c
	VENDOR_CUSTOMER_3  = 3,  // use external customization file : bootblock_$vendor$_cutomoize.c
	VENDOR_CUSTOMER_4  = 4,  // use external customization file : bootblock_$vendor$_cutomoize.c
	VENDOR_CUSTOMER_5  = 5,  // use external customization file : bootblock_$vendor$_cutomoize.c
	VENDOR_CUSTOMER_6  = 6,  // use external customization file : bootblock_$vendor$_cutomoize.c

	VENDOR_NUVOTON     = 100,  // The headen champion. Select options specific for EB and SVB. useing bootblock_nuvoton_cutomoize.c.
	VENDOR_UNKNOWN     = 101
} BOARD_VENDOR_T;


/*---------------------------------------------------------------------------------------------------------*/
/* HOST IF                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum   HOST_IF_T
{
	HOST_IF_UNKNOWN_0         = 0xFF,         // Bootblock skip host configuration
	HOST_IF_LPC               = 0,            // Bootblock selects HOST IF LPC.
	HOST_IF_ESPI              = 1,            // Bootblock selects HOST IF ESPI
	HOST_IF_GPIO              = 2,            // Bootblock host IF is not configured. Pins are set as GPIOs and inputs
	HOST_IF_RELEASE_HOST_WAIT = 3,            // Release host wait only
} HOST_IF_T;


typedef struct {
	UINT8 VersionDescription[0xB0];
	UINT32 BootBlockTag;
	UINT32 BootblockVersion;
} BOOTBLOCK_Version_T;


/*---------------------------------------------------------------------------------------------------------*/
/* Boot module exported functions                                                                          */
/*---------------------------------------------------------------------------------------------------------*/

BOARD_T         BOOTBLOCK_GetBoardType (void);
BOARD_VENDOR_T  BOOTBLOCK_GetVendorType (void);
UINT32          BOOTBLOCK_Get_MC_freq (void);
UINT32          BOOTBLOCK_Get_CPU_freq (void);
void            BOOTBLOCK_Get_DDR_Setup (DDR_Setup *ddr_setup);
HOST_IF_T       BOOTBLOCK_Get_host_if (void);
UINT8           BOOTBLOCK_Get_SPI_clk_divider (UINT spi);
UINT8           BOOTBLOCK_Get_i3c_RC_clk_divider (void);
UINT32          BOOTBLOCK_Get_pll0_override (void);
UART_BAUDRATE_T BOOTBLOCK_GetUartBaud (void);
UINT32          BOOTBLOCK_Get_FIU_DRD_CFG (UINT32 fiu);

#endif /* _BOOT_H */
