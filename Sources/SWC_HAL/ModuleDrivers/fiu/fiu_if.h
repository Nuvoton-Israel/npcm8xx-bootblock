/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   fiu_if.h                                                                 */
/*            This file contains Flash Interface Unit (FIU) interface         */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/
#ifndef _FIU_IF_H
#define _FIU_IF_H

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                                 INCLUDES                                                */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#include __CHIP_H_FROM_IF()

#if defined FIU_MODULE_TYPE
#include __MODULE_HEADER(fiu, FIU_MODULE_TYPE)
#elif defined FIU_MODULE_TYPE_EXT
/*---------------------------------------------------------------------------------------------------------*/
/* This case is for projects that run code from FLASH memory                                               */
/* the flash_dev module needs to include FIU functions but FIU code must be located in RAM                 */
/* in such case the FIU is compiled outside the HAL in order to locate its object on RAM memory            */
/*---------------------------------------------------------------------------------------------------------*/
#include __MODULE_HEADER(fiu, FIU_MODULE_TYPE_EXT)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                           TYPES & DEFINITIONS                                           */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

#ifdef FIU_CAPABILITY_CRC
/* Interface Selction bit.                                                       */
/*  - In order to set to Intel do "and" with FW_HEADER_INTEL_INTERFACE.          */
/*  - In order to set to ARM do "or" with FW_HEADER_ARM_INTERFACE.               */
/*  - In order to get selection do "and" with FW_HEADER_INTERFACE_SELECTION_MASK */
#define FW_HEADER_INTEL_INTERFACE            0xFE
#define FW_HEADER_ARM_INTERFACE              0x01
#define FW_HEADER_INTERFACE_SELECTION_MASK   0x01
#endif

#ifdef FIU_CAPABILITY_UNLIMITED_BURST
typedef enum
{
    FIU_READ_BURST_NORMAL_READ, /* do not change - code relies on enum value */
    FIU_READ_BURST_UNLIMITED    /* do not change - code relies on enum value */
}FIU_READ_BURST_T;
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* FIU Modules                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef FIU_MODULE_T
typedef enum FIU_MODULE_T
{
    FIU_MODULE_0 = 0,
    FIU_MODULE_1 = 1,
    FIU_MODULE_2 = 2,
    FIU_MODULE_3 = 3,
    FIU_MODULE_X = 4
} FIU_MODULE_T;
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* FIU Chip Select                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum  FIU_CS_T
{
    FIU_CS_0 = 0,
    FIU_CS_1 = 1,
    FIU_CS_2 = 2,
    FIU_CS_3 = 3
} FIU_CS_T;

/*---------------------------------------------------------------------------------------------------------*/
/* FIU Read Burst Size                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum FIU_R_BURST_T
{
    FIU_R_BURST_ONE_BYTE        = 0,    /* do not change - code relies on enum value */
    FIU_R_BURST_FOUR_BYTE       = 2,    /* do not change - code relies on enum value */
    FIU_R_BURST_SIXTEEN_BYTE    = 3     /* do not change - code relies on enum value */
} FIU_R_BURST_T;

/*---------------------------------------------------------------------------------------------------------*/
/* FIU Write Burst Size                                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum FIU_W_BURST_T
{
    FIU_W_BURST_ONE_BYTE        = 0,    /* do not change - code relies on enum value */
#ifndef FIU_CAPABILITY_FIXED_WRITE_BURST
    FIU_W_BURST_FOUR_BYTE       = 2,    /* do not change - code relies on enum value */
    FIU_W_BURST_SIXTEEN_BYTE    = 3     /* do not change - code relies on enum value */
#endif
} FIU_W_BURST_T;

/*---------------------------------------------------------------------------------------------------------*/
/* FIU Dummy bytes count                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum FIU_DUMMY_COUNT_T
{
    FIU_DUMMY_COUNT_0        = 0,
    FIU_DUMMY_COUNT_0p5      = 1,
    FIU_DUMMY_COUNT_1        = 2,
    FIU_DUMMY_COUNT_2        = 3,
    FIU_DUMMY_COUNT_3        = 4,
    FIU_DUMMY_COUNT_4        = 5,
    FIU_DUMMY_SKIP_CONFIG    = 6
} FIU_DUMMY_COUNT_T;

#ifdef FIU_CAPABILITY_BF_ENABLE
/*---------------------------------------------------------------------------------------------------------*/
/* FIU Bus Fault (BF) trap generation                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum FIU_BF_TRAP_T
{
  FIU_BF_TRAP_DISABLE           = 0,
  FIU_BF_TRAP_ENABLE            = 1
} FIU_BF_TRAP_T;
#else
/*---------------------------------------------------------------------------------------------------------*/
/* FIU Illegal Address (IAD) trap generation                                                               */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum FIU_IAD_TRAP_T
{
  FIU_IAD_TRAP_DISABLE          = 0,
  FIU_IAD_TRAP_ENABLE           = 1
} FIU_IAD_TRAP_T;
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* FIU Device Size                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum FIU_DEV_SIZE_T
{
    //lint -e{113} Suppress Inconsistent enum declaration error
    FIU_DEV_SIZE_128KB          = 1,
    FIU_DEV_SIZE_256KB          = 2,
    FIU_DEV_SIZE_512KB          = 4,
    FIU_DEV_SIZE_1MB            = 8,
    FIU_DEV_SIZE_2MB            = 16,
    FIU_DEV_SIZE_4MB            = 32,
#if defined FIU_CAPABILITY_SUPPORT_128MB_FLASH || defined FIU_CAPABILITY_SUPPORT_64MB_FLASH || defined FIU_CAPABILITY_SUPPORT_16MB_FLASH || defined FIU_CAPABILITY_SUPPORT_8MB_FLASH
    FIU_DEV_SIZE_8MB            = 64,
#if defined FIU_CAPABILITY_SUPPORT_128MB_FLASH || defined FIU_CAPABILITY_SUPPORT_64MB_FLASH || defined FIU_CAPABILITY_SUPPORT_16MB_FLASH
    FIU_DEV_SIZE_16MB           = 128,
#if defined FIU_CAPABILITY_SUPPORT_128MB_FLASH || defined FIU_CAPABILITY_SUPPORT_64MB_FLASH
    FIU_DEV_SIZE_32MB           = 256,
    FIU_DEV_SIZE_64MB           = 512,
#if defined FIU_CAPABILITY_SUPPORT_128MB_FLASH
    FIU_DEV_SIZE_128MB          = 1024
#endif
#endif
#endif
#endif
} FIU_DEV_SIZE_T;

/*---------------------------------------------------------------------------------------------------------*/
/* FIU Read Mode                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum FIU_READ_MODE_T
{
    //lint -e{113} Suppress Inconsistent enum declaration error
    FIU_NORMAL_READ             = 0,
    FIU_FAST_READ               = 1,
    FIU_FAST_READ_DUAL_OUTPUT   = 2,
    FIU_FAST_READ_DUAL_IO       = 3,
#ifdef FIU_CAPABILITY_QUAD_READ
    FIU_FAST_READ_QUAD_IO       = 4,
    FIU_FAST_READ_QUAD_OUTPUT   = 5,
#endif
#ifdef FIU_CAPABILITY_SPI_X
    FIU_FAST_READ_SPI_X         = 6,
#endif
    FIU_READ_MODE_NUM

} FIU_READ_MODE_T;

/*---------------------------------------------------------------------------------------------------------*/
/* FIU Write Mode                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum FIU_WRITE_MODE_T
{
    FIU_NORMAL_WRITE            = 0,
    FIU_QUAD_WRITE              = 2,
} FIU_WRITE_MODE_T;

/*---------------------------------------------------------------------------------------------------------*/
/* FIU UMA data size                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum FIU_UMA_DATA_SIZE_T
{
    FIU_UMA_DATA_SIZE_0         =  0,
    FIU_UMA_DATA_SIZE_1         =  1,
    FIU_UMA_DATA_SIZE_2         =  2,
    FIU_UMA_DATA_SIZE_3         =  3,
    FIU_UMA_DATA_SIZE_4         =  4,
    FIU_UMA_DATA_SIZE_5         =  5,
    FIU_UMA_DATA_SIZE_6         =  6,
    FIU_UMA_DATA_SIZE_7         =  7,
    FIU_UMA_DATA_SIZE_8         =  8,
    FIU_UMA_DATA_SIZE_9         =  9,
    FIU_UMA_DATA_SIZE_10        = 10,
    FIU_UMA_DATA_SIZE_11        = 11,
    FIU_UMA_DATA_SIZE_12        = 12,
    FIU_UMA_DATA_SIZE_13        = 13,
    FIU_UMA_DATA_SIZE_14        = 14,
    FIU_UMA_DATA_SIZE_15        = 15,
    FIU_UMA_DATA_SIZE_16        = 16
} FIU_UMA_DATA_SIZE_T;

#ifdef FIU_CAPABILITY_SEC_CHIP_SELECT
/*---------------------------------------------------------------------------------------------------------*/
/* FIU Low Device Size                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
typedef UINT FIU_LOW_DEV_SIZE_T;
#endif

#ifdef FIU_CAPABILITY_CRC
/*---------------------------------------------------------------------------------------------------------*/
/*                                      FIU CRC32 and Checksum mode.                                       */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum FIU_ERROR_CHECK_MODE_T
{
    FIU_CRC32_MODE          = 0,
    FIU_CHECKSUM_MODE       = 1
} FIU_ERROR_CHECK_MODE_T;

/*---------------------------------------------------------------------------------------------------------*/
/*                                  FIU read on the fly or FW write mode                                   */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum FIU_ERR_CHECK_READ_MODE_T
{
    FIU_FW_WRITE_MODE       = 0,
    FIU_ON_THE_FLY_MODE     = 1
} FIU_ERR_CHECK_READ_MODE_T;

/*---------------------------------------------------------------------------------------------------------*/
/*                               FIU error check calculation enable/disable                                */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum FIU_ERR_CHECK_ENABLE_DISABLE_MODE_T
{
    FIU_CALC_DISABLE_MODE    = 0,
    FIU_CALC_ENABLE_MODE     = 1
} FIU_ERR_CHECK_ENABLE_DISABLE_MODE_T;

/*---------------------------------------------------------------------------------------------------------*/
/*                                      FIU configuration structure.                                       */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{
    FIU_ERR_CHECK_READ_MODE_T   readMode;
    FIU_ERROR_CHECK_MODE_T      errCheckMode;
} FIU_ERROR_CHECK_CONFIGURATION_T;
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* FIU_DIRECTION_MODE                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum FIU_DIRECTION_MODE_T
{
    FIU_DIRECTION_MODE_SINGLES         = 0, /* do not change the  value. SW rely on it */
    FIU_DIRECTION_MODE_INCREASING      = 1
} FIU_DIRECTION_MODE_T;

#ifdef FIU_CAPABILITY_MASTER
/*---------------------------------------------------------------------------------------------------------*/
/*                                          FIU master interrupts masks                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define FIU_MASTER_INT_RD_PEND_UMA            0x01
#define FIU_MASTER_INT_RD_PEND_FIU            0x02
#define FIU_MASTER_INT_GRANT_ASR              0x04

typedef void (*FIU_SLAVE_INT_HANDLER)(UINT8 status);

#endif // FIU_CAPABILITY_MASTER


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                           INTERFACE FUNCTIONS                                           */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_Reset                                                                              */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine resets the module                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_Reset (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_Init                                                                               */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/*                  fiu_module          - fiu module number. Value is ignored if only one FIU module on    */
/*                  chipSelect          - device to init. optional per fiu driver.                         */
/*                  initVal             - default value to write to regs. If zero - ignored.               */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine inits the module  , performed for each FIU and CS to be used              */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_Init (FIU_MODULE_T fiu_module, FIU_CS_T chipSelect, UINT32 initVal);

#ifdef FIU_CAPABILITY_UNLIMITED_BURST
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_EnUsingUnlimitedBurst                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number.                                                        */
/*                  en         - TRUE to enable using Unlimited Burst , FALSE otherwise                    */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Set/Clear the FIU_useUnlimitedBurst_l[] variable                                       */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_EnUsingUnlimitedBurst (FIU_MODULE_T fiu_module, BOOLEAN en);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_CanUseUnlimitedBurst                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number.                                                        */
/*                                                                                                         */
/* Returns:         TRUE/FALSE                                                                             */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Return TRUE/FALSE to indicate if the FIU can (or can't) use Unlimited Burst            */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN FIU_CanUseUnlimitedBurst (FIU_MODULE_T fiu_module);
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_Config                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module          - fiu module number. Value is ignored if only one FIU module on    */
/*                  chip                                                                                   */
/*                  flash_size          - The flash size (If there are few flash devices -                 */
/*                                        The flash size of the largest flash device).                     */
/*                  read_mode           - Read mode select (Normal/Fast-Read).                             */
/*                  read_burst_size     - Flash read burst size (1/16 bytes).                              */
/*                  write_burst_size    - Flash write burst size (1/4 bytes).                              */
/*                  bf_trap/iad_trap    - BF/IAD trap generation enable.                                   */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures the FIU module.                                                */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_Config (   FIU_MODULE_T    fiu_module,
                    FIU_DEV_SIZE_T  flash_size,
                    FIU_READ_MODE_T read_mode,
                    FIU_R_BURST_T   read_burst_size,
                    FIU_W_BURST_T   write_burst_size,
#ifdef FIU_CAPABILITY_BF_ENABLE
                    FIU_BF_TRAP_T   bf_trap);
#else
                    FIU_IAD_TRAP_T  iad_trap);
#endif
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigCommand                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module       - fiu module number. Value is ignored if only one FIU module on chip  */
/*                  readCommandByte  - byte to read on commands cycle                                      */
/*                  writeCommandByte - byte to write on commands cycle                                     */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures the FIU module command byte                                    */
/*                  Notice that 0 command is not on the spec If user wish to avvoid configuration - set it */
/*                  to zero !                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_ConfigCommand (FIU_MODULE_T fiu_module, UINT8 readCommandByte, UINT8 writeCommandByte);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigFlashSize                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number. Value is ignored if only one FIU module on chip        */
/*                  flash_size - Flash size to configure                                                   */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures FIU flash size                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_ConfigFlashSize (FIU_MODULE_T fiu_module, FIU_DEV_SIZE_T flash_size);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_DirectRead_SetCommadCode                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module  - fiu module number. Value is ignored if only one FIU module on chip       */
/*                  cmd         - SPI flash command byte                                                   */
/*                  enable      - Boolean indicating whether to enable or disable this feature.            */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures the FIU to support none standard SPI read commands.            */
/*                  If "enable" == TRUE , any FIU read transaction will use the "cmd" data byte as the     */
/*                  SPI read transaction command byte                                                      */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_DirectRead_SetCommadCode (FIU_MODULE_T fiu_module, UINT8 cmd, BOOLEAN enable);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_DirectWrite_SetCommadCode                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module  - fiu module number.                                                       */
/*                  cmd         - SPI flash command byte                                                   */
/*                  enable      - Boolean indicating whether to enable or disable this feature.            */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures the FIU to support direct SPI write commands.                  */
/*                  If "enable" == TRUE , any FIU direct write transaction will use the "cmd" data byte as */
/*                  the SPI write transaction command byte                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_DirectWrite_SetCommadCode (FIU_MODULE_T fiu_module, UINT8 cmd, BOOLEAN enable);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigReadMode                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number. Value is ignored if only one FIU module on chip        */
/*                  read_mode  - FIU read mode                                                             */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures the FIU read mode.                                             */
/*                  In case the high freq flash clock is about to set (above 33MHz), One of fast-read      */
/*                  modes must be configured in order to allow access to the flash (as high freq flash     */
/*                  clock can work in fast read mode only).                                                */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_ConfigReadMode (FIU_MODULE_T fiu_module, FIU_READ_MODE_T read_mode);

#ifdef FIU_CAPABILITY_WRITE_MODE_SELECT
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigWriteMode                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number. Value is ignored if only one FIU module on chip        */
/*                  write_mode - FIU write mode                                                            */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures the FIU write mode.                                            */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_ConfigWriteMode (FIU_MODULE_T fiu_module, FIU_WRITE_MODE_T write_mode);
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_GetReadMode                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number. Value is ignored if only one FIU module on chip        */
/*                                                                                                         */
/* Returns:         read_mode - FIU read mode                                                              */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine returns the FIU read mode.                                                */
/*---------------------------------------------------------------------------------------------------------*/
FIU_READ_MODE_T FIU_GetReadMode (FIU_MODULE_T fiu_module);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigBurstSize                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module        - fiu module number. Value is ignored if only one FIU module on chip */
/*                  read_burst_size   - FIU read burst size                                                */
/*                  write_burst_size  - FIU write burst size                                               */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures read/write burst size                                          */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_ConfigBurstSize (FIU_MODULE_T fiu_module, FIU_R_BURST_T read_burst_size, FIU_W_BURST_T write_burst_size);

#ifdef FIU_CAPABILITY_UNLIMITED_BURST
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigReadBurstType                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module        - fiu module number. Value is ignored if only one FIU module on chip */
/*                  read_burst_type   - FIU read burst type                                                */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures read burst type                                                */
/*                  In order to work with unlimited burst mode the user should:                            */
/*                  1. Configure read burst size to FIU_R_BURST_SIXTEEN_BYTE using                         */
/*                     FIU_ConfigBurstSize(...) func call                                                  */
/*                  2. Call this routine using read_burst_type = FIU_READ_BURST_UNLIMITED                  */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_ConfigReadBurstType (FIU_MODULE_T fiu_module, FIU_READ_BURST_T read_burst_type);
#endif

#ifdef FIU_CAPABILITY_AUTO_ASSERTION
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_EnableAutoAssertion                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number. on this version of the driver it is not used.          */
/*                  chipSelect - Select the CS on the given flash device.                                  */
/*                  enable     - boolean indicating whether to enable or disable 4-byte address.           */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine enables/disables the automatic assertion control for F_CS11 or F_CS10     */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_EnableAutoAssertion (FIU_MODULE_T fiu_module, FIU_CS_T chipSelect, BOOLEAN enable);
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_EnableDmaRequest                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number.                                                        */
/*                  enable     - boolean indicating whether to enable or disable Dma Request .             */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine enables/disables the Dma Request. When Dma Request is enabled, and also   */
/*                  UNLIMITED Burst is enabled, using FIU_ConfigReadBurstType(), a DMA request signal is   */
/*                  asserted when an FIU buffer is filled with data and ready                              */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_EnableDmaRequest (FIU_MODULE_T fiu_module, BOOLEAN enable);


#ifdef FIU_CAPABILITY_BF_ENABLE
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigBusFaultTrap                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module       - fiu module number. Value is ignored if only one FIU module on chip  */
/*                  enable           - Bus fault trap configuration (enable/disable)                       */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine enabled/disables Bus Fault generation by FIU                              */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_ConfigBusFaultTrap (FIU_MODULE_T fiu_module, FIU_BF_TRAP_T bf_trap);
#else
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigIADTrap                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module       - fiu module number. Value is ignored if only one FIU module on chip  */
/*                  iad_trap         - IAD trap configuration (enable/disable)                             */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine enabled/disables IAD trap generation by FIU                               */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_ConfigIADTrap (FIU_MODULE_T fiu_module, FIU_IAD_TRAP_T iad_trap);
#endif // FIU_CAPABILITY_BF_ENABLE

#ifdef FIU_CAPABILITY_IO2_IO3_DRIVE_HIGH
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigIO2andIO3Drive                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module       - fiu module number.                                                  */
/*                  high             - TRUE:  IO2 & IO3 signals are driven high before CS signal assertion */
/*                                   - FALSE: IO2 & IO3 signals are driven low before CS signal assertion  */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures the behavior of IO2 & IO3 signals before CS signal is          */
/*                  asserted.                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_ConfigIO2andIO3Drive (FIU_MODULE_T fiu_module, BOOLEAN high);
#endif

#ifdef FIU_CAPABILITY_UMA_DISABLE
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_UMA_Disable                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module       - fiu module number.                                                  */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine disables the UMA operation                                                */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_UMA_Disable (FIU_MODULE_T fiu_module);
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ManualWrite                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module       - fiu module number. Value is ignored if only one FIU module on chip  */
/*                  device           - Select the flash device to be accessed                              */
/*                  transaction_code - Specify the SPI UMA transaction code                                */
/*                  address          - Location on the flash, in the flash address space                   */
/*                  data             - a pointer to a data buffer (buffer of bytes)                        */
/*                  data_size        - data buffer size in bytes. Legal sizes are 0-256                    */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine "manual" page programming without using UMA.                              */
/*                  The data can be programmed upto the size of the whole page in a single SPI transaction */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FIU_ManualWrite ( FIU_MODULE_T            fiu_module,
                              FIU_CS_T                device,
                              UINT8                   transaction_code,
                              UINT32                  address,
                              UINT8 *                 data,
                              UINT32                  data_size);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_UMA_read                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module       - fiu module number. Value is ignored if only one FIU module on chip  */
/*                  device           - Select the flash device to be accessed                              */
/*                  transaction_code - Specify the SPI UMA transaction code                                */
/*                  address          - Location on the flash , in the flash address space                  */
/*                  address_size     - if TRUE, 3 bytes address, to be placed in FIU_UMA_AB0-2             */
/*                                     else (FALSE), no address for this SPI UMA transaction               */
/*                  data             - a pointer to a data buffer to hold the read data.                   */
/*                  data_size        - data buffer size                                                    */
/*                  timeout          - command timeout                                                     */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine Read up to 4 bytes from the flash. using the FIU User Mode Access (UMA)   */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FIU_UMA_Read (  FIU_MODULE_T            fiu_module,
                            FIU_CS_T                device,
                            UINT8                   transaction_code,
                            UINT32                  address,
                            BOOLEAN                 address_size,
                            UINT8*                  data,
                            FIU_UMA_DATA_SIZE_T     data_size,
                            UINT32                  timeout);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_UMA_Write                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module       - fiu module number. Value is ignored if only one FIU module on chip  */
/*                  device           - Select the flash device to be accessed                              */
/*                  transaction_code - Specify the SPI UMA transaction code                                */
/*                  address          - Location on the flash, in the flash address space                   */
/*                  address_size     - if TRUE, 3 bytes address, to be placed in FIU_UMA_AB0-2             */
/*                                     else (FALSE), no address for this SPI UMA transaction               */
/*                  data             - a pointer to a data buffer (buffer of bytes)                        */
/*                  data_size        - data buffer size                                                    */
/*                  timeout          - command timeout                                                     */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine write up to 16 bytes to the flash using the FIU User Mode Access (UMA)    */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FIU_UMA_Write ( FIU_MODULE_T            fiu_module,
                            FIU_CS_T                device,
                            UINT8                   transaction_code,
                            UINT32                  address,
                            BOOLEAN                 address_size,
                            const UINT8*            data,
                            FIU_UMA_DATA_SIZE_T     data_size,
                            UINT32                  timeout);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_UMA_DataRead                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module       - fiu module number. Value is ignored if only one FIU module on chip  */
/*                  device           - Select the flash device to be accessed                              */
/*                  data             - a pointer to a data buffer to hold the read data.                   */
/*                  data_size        - data buffer size                                                    */
/*                  timeout          - command timeout                                                     */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine assume that the chip select is already low (using FIU_ForceChipSelect)    */
/*                  and it only generates UAM read transactions without sending any command                */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FIU_UMA_DataRead( FIU_MODULE_T   fiu_module,
                              FIU_CS_T       device,
                              UINT8*         data,
                              UINT32         data_size,
                              UINT32         timeout);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_UMA_DataWrite                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module       - fiu module number.                                                  */
/*                  device           - (Not used) Not Aplicable                                            */
/*                  data             - a pointer to a data buffer (buffer of bytes).                       */
/*                  data_size        - data buffer size                                                    */
/*                  timeout          - command timeout                                                     */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine assume that the chip select is already low (using FIU_ForceChipSelect)    */
/*                  This routine just write data to the flash using UMA.                                   */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FIU_UMA_DataWrite ( FIU_MODULE_T    fiu_module,
                                FIU_CS_T        device,
                                const UINT8*    data,
                                UINT16          data_size,
                                UINT32          timeout);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_UMA_ioctl                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  address -                                                                              */
/*                  address_size - in bytes                                                                */
/*                  addr_bits -                                                                            */
/*                  cmd_bits -                                                                             */
/*                  data_rd -                                                                              */
/*                  data_rd_bits -                                                                         */
/*                  data_rd_size -                                                                         */
/*                  data_wr -                                                                              */
/*                  data_wr_bits -                                                                         */
/*                  data_wr_size -                                                                         */
/*                  device -                                                                               */
/*                  fiu_module -                                                                           */
/*                  timeout -                                                                              */
/*                  transaction_code -                                                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs generic UMA op. This function doesn't use DRD\DWR regs for inputs*/
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FIU_UMA_ioctl (  FIU_MODULE_T            fiu_module,
                            FIU_CS_T                device,
                            UINT8                   transaction_code,
                            UINT32                  address,
                            UINT                    address_size,
                            UINT8*                  data_rd,
                            UINT                    data_rd_size,
                            const UINT8*            data_wr,
                            UINT                    data_wr_size,
                            UINT                    cmd_bits,
                            UINT                    addr_bits,
                            UINT                    data_wr_bits,
                            UINT                    data_rd_bits,                           
                            UINT32                  timeout,
                            UINT                    dummy_bytes);
                            
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_PageWrite                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module       - fiu module number.                                                  */
/*                  device           - (Not used) Not Aplicable                                            */
/*                  transaction_code - Specify the SPI UMA transaction code                                */
/*                  address          - Location on the flash, in the flash address space                   */
/*                  data             - a pointer to a data buffer (buffer of bytes)                        */
/*                  data_size        - data buffer size in bytes                                           */
/*                  timeout          - command timeout                                                     */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine executes page programming without using UMA.                              */
/*                  The data can be programmed upto the size of the whole page in a single SPI transaction */
/*                  The data address/size must fit page boundaries for page programming to succeed         */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FIU_PageWrite ( FIU_MODULE_T    fiu_module,
                            FIU_CS_T        device,
                            UINT8           transaction_code,
                            UINT32          address,
                            const UINT8*    data,
                            UINT16          data_size,
                            UINT32          timeout);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_WriteRead                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module       - fiu module number.                                                  */
/*                  device           - (Not used) Not Aplicable                                            */
/*                  transaction_code - Specify the SPI UMA transaction code                                */
/*                  address          - Location on the flash , in the flash address space                  */
/*                  asize            - number of bytes in the address. Only 0,1,2, or 4 can be used        */
/*                  wdata            - a pointer to a buffer that holds data to be written to the falsh    */
/*                  wsize            - number of bytes in the "wdata" buffer                               */
/*                  rdata            - a pointer to a data buffer to hold the read data                    */
/*                  rsize            - number of bytes to read                                             */
/*                  timeout          - command timeout                                                     */
/*                                                                                                         */
/* Returns:         DEFS_STATUS_OK or other DEFS_STATUS error                                              */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine writes and reads data from the flash in the following way                 */
/*                  - Send the transaction_code                                                            */
/*                  - If (asize > 0) send the address                                                      */
/*                  - If (wsize > 0) send wsize bytes from wdata                                           */
/*                  - If (rsize > 0) read rsize bytes to rdata                                             */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FIU_WriteRead ( FIU_MODULE_T    fiu_module,
                            FIU_CS_T        device,
                            UINT8           transaction_code,
                            UINT32          address,
                            UINT8           asize,
                            const UINT8*    wdata,
                            UINT            wsize,
                            UINT8*          rdata,
                            UINT            rsize,
                            UINT32          timeout);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigAddressSize                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number.                                                        */
/*                  size       - the selected address size                                                 */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine select the number of address bytes generated for any SPI transaction      */
/*                  possible values are 0 to 4 , where 0 is no address byte and 4 is 4 address bytes       */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_ConfigAddressSize (FIU_MODULE_T fiu_module, UINT8 size);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_GetAddressSize                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number.                                                        */
/*                                                                                                         */
/* Returns:         address size                                                                           */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine returns the number of address bytes generated for any SPI transaction     */
/*                  possible values are 0 to 4 , where 0 is no address byte and 4 is 4 address bytes       */
/*---------------------------------------------------------------------------------------------------------*/
UINT8 FIU_GetAddressSize (FIU_MODULE_T fiu_module);

#ifdef FIU_CAPABILITY_4B_ADDRESS
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_Config4ByteAddress                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number. Value is ignored if only one FIU module on chip        */
/*                  device     - Select the flash device to be accessed.                                   */
/*                  chipSelect - Select the CS on the given flash device.                                  */
/*                  enable     - boolean indicating whether to enable or disable 4-byte address.           */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine enables 4-byte addres SPI transactions or disables it (and use 3-byte     */
/*                  address mode)                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_Config4ByteAddress (FIU_MODULE_T fiu_module, FIU_CS_T device, FIU_CS_T chipSelect, BOOLEAN enable);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_Get4ByteAddress                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number. Value is ignored if only one FIU module on chip        */
/*                  device     - Select the flash device to be accessed.                                   */
/*                  chipSelect - Select the CS on the given flash device.                                  */
/*                                                                                                         */
/* Returns:         TRUE if the device is in 4-byte addres mode; FALSE otherwise                           */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine retrieves whether the device is in 4-byte addres mode                     */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN FIU_Get4ByteAddress (FIU_MODULE_T fiu_module, FIU_CS_T device, FIU_CS_T chipSelect);
#endif

#ifdef FIU_CAPABILITY_CONFIG_DUMMY_CYCLES
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigDummyCyles                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module  - fiu module number. Value is ignored if only one FIU module on chip       */
/*                  dummyCycles - number of dummy cycles.                                                  */
/*                  enable      - boolean indicating whether to enable or disable dummy cycles.            */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures the number of dummy cycles                                     */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_ConfigDummyCyles (FIU_MODULE_T fiu_module, UINT8 dummyCycles, BOOLEAN enable);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_GetDummyCyles                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module  - fiu module number. Value is ignored if only one FIU module on chip       */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine retrieves the number of dummy cycles                                      */
/*---------------------------------------------------------------------------------------------------------*/
UINT8 FIU_GetDummyCyles (FIU_MODULE_T fiu_module);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_GetDummyCylesEnable                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module  - fiu module number. Value is ignored if only one FIU module on chip       */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine retrieves the dummy cycles enablement                                     */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN FIU_GetDummyCylesEnable (FIU_MODULE_T fiu_module);
#endif

#ifdef FIU_CAPABILITY_SEC_CHIP_SELECT
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_SelectChip                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number. Value is ignored if only one FIU module on chip        */
/*                  device     - Select the flash device to be accessed.                                   */
/*                  address    - Location on the flash , in the flash address space                        */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects the set of SPI Chip Select Signals on the SPI bus for a           */
/*                  given device according to the given address                                            */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_SelectChip (FIU_MODULE_T fiu_module, FIU_CS_T device, UINT32 address);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigChipSelect                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number. Value is ignored if only one FIU module on chip        */
/*                  cs         - configures the Chip Select.                                               */
/*                               0: The secondary set of SPI Chip Select Signals F_CS11/F_CS01 are used    */
/*                               1: The primary set of SPI Chip Select Signals F_CS10/F_CS00 are used      */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures the set of SPI Chip Select Signals on the SPI bus              */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_ConfigChipSelect (FIU_MODULE_T fiu_module, FIU_CS_T cs);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ForceChipSelect                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module   - fiu module number. Value is ignored if only one FIU module on chip      */
/*                  device       - Select the flash device to be accessed.                                 */
/*                  high_low     - 0: Force the Chip Select to Low                                         */
/*                                 1: Force the Chip Select to High                                        */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine force the selected SPI Chip Select (according to the "device") to         */
/*                  be high or low .                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_ForceChipSelect (FIU_MODULE_T fiu_module, FIU_CS_T device, UINT high_low);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_GetChipSelect                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number. Value is ignored if only one FIU module on chip        */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine retrieves the set of SPI Chip Select Signals selected on the SPI bus      */
/*---------------------------------------------------------------------------------------------------------*/
FIU_CS_T FIU_GetChipSelect (FIU_MODULE_T fiu_module);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigLowDevSize                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number. Value is ignored if only one FIU module on chip        */
/*                  device     - Select the flash device to be accessed.                                   */
/*                  size       - integer represeting the SPI Low Device flash size in MBytes.              */
/*                               Posibale values are: 1, 2, 4, 8, 16, 32, 64, 128                          */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures the Low Device size                                            */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_ConfigLowDevSize (FIU_MODULE_T fiu_module, FIU_CS_T device, FIU_LOW_DEV_SIZE_T size);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_GetLowDevSize                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number. Value is ignored if only one FIU module on chip        */
/*                  device     - Select the flash device to be accessed.                                   */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine retrieves the Low Device size                                             */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 FIU_GetLowDevSize (FIU_MODULE_T fiu_module, FIU_CS_T device);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_EnableTwoDevices                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number. Value is ignored if only one FIU module on chip        */
/*                  device     - Select the flash device to be accessed.                                   */
/*                  option     - Select the pin option                                                     */
/*                  enable     - boolean indicating whether to enable two devices on the SPI bus.          */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine enables/disables two devices on the SPI bus                               */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_EnableTwoDevices (FIU_MODULE_T fiu_module, FIU_CS_T device, UINT option, BOOLEAN enable);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_TwoDevicesEnabled                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number. Value is ignored if only one FIU module on chip        */
/*                  device     - Select the flash device to be accessed.                                   */
/*                                                                                                         */
/* Returns:         TRUE if two devices are enabled on the SPI bus; FALSE otherwise                        */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine retrieves whether two devices are enabled on the SPI bus                  */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN FIU_TwoDevicesEnabled (FIU_MODULE_T fiu_module, FIU_CS_T device);
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_SetLowDevice                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number.                                                        */
/*                  device     - The chip select (FIU_CS_0 or FIU_CS_1 ) which is connected to the low     */
/*                               low device.                                                               */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Set the device (CS0 or CS1) connected to the low addresses of the SPI interface        */
/*                  address range                                                                          */
/*                  FIU_CS_0: The device connected to F_CS0 is located at the SPI base address             */
/*                  FIU_CS_1: The device connected to F_CS1 is located at the SPI base address             */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_SetLowDevice (FIU_MODULE_T fiu_module, FIU_CS_T device);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_GetLowDevice                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number.                                                        */
/*                                                                                                         */
/* Returns:         FIU_CS_0 or FIU_CS_1                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Returns the device (CS0 or CS1) that connected to the low addresses of the SPI         */
/*                  interface address range                                                                */
/*                  FIU_CS_0: The device connected to F_CS0 is located at the SPI base address             */
/*                  FIU_CS_1: The device connected to F_CS1 is located at the SPI base address             */
/*---------------------------------------------------------------------------------------------------------*/
FIU_CS_T FIU_GetLowDevice (FIU_MODULE_T fiu_module);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigDummyCyclesCount                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module       - fiu module number. Value is ignored if only one FIU module on chip  */
/*                  writeDummyCycles - Number of Dummy cycles for write                                    */
/*                  readDummyCycles  - Number of Dummy cycles for read                                     */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine config the number of dummy cycles                                         */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FIU_ConfigDummyCyclesCount (FIU_MODULE_T fiu_module, FIU_DUMMY_COUNT_T readDummyCycles);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_SetDirection                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module       - fiu module number. Value is ignored if only one FIU module on chip  */
/*                                                                                                         */
/* Returns:         DEFS_STATUS                                                                            */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine config the read direction of FIU                                          */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FIU_SetDirection (FIU_MODULE_T fiu_module , FIU_DIRECTION_MODE_T mode);

#ifdef FIU_CAPABILITY_CRC
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_CrcCksmConfig                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module      - fiu module number. Value is ignored if only one FIU module on chip   */
/*                  checksumCrcMode - Checksum or CRC                                                      */
/*                  fwSpiMode       - FW write data or data is read on the fly.                            */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This configure error check utility.                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_CrcCksmConfig ( FIU_MODULE_T              fiu_module,
                         FIU_ERROR_CHECK_MODE_T    errCheckMode,
                         FIU_ERR_CHECK_READ_MODE_T readMode);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_CrcCksmStart                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module      - fiu module number. Value is ignored if only one FIU module on chip   */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine start the error check calculation                                         */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_CrcCksmStart (FIU_MODULE_T fiu_module);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_CrcCksmWrite                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module      - fiu module number. Value is ignored if only one FIU module on chip   */
/*                  buffer          - Pointer to the buffer that should be writen.                         */
/*                  bufferLen       - Buffer length.                                                       */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine writes bytes in order to calculate the error check.                       */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_CrcCksmWrite (FIU_MODULE_T fiu_module, const UINT8* buffer, UINT32 bufferLen);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_CrcCksmStopAndResult                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module      - fiu module number. Value is ignored if only one FIU module on chip   */
/*                                                                                                         */
/* Returns:         32 bit result.                                                                         */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine stops the error check calculation, and return result.                     */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 FIU_CrcCksmStopAndResult (FIU_MODULE_T fiu_module);
#endif

#ifdef FIU_CAPABILITY_MASTER
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_SlaveInit                                                                          */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine initialize FIU master                                                     */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_SlaveInit (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_MasterConfig                                                                       */
/*                                                                                                         */
/* Parameters:      arbThresh    -  Master FIU priority duration during an arbitration window of 1024      */
/*                                  clock cycles                                                           */
/*                  cntThresh    -  Master FIU inactivity duration threshold. The duration is measured     */
/*                                  in units of FIU clock cycles.                                          */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configure FIU master thresholds                                           */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_MasterConfig (UINT8 arbThresh, UINT8 cntThresh);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_SlaveGetStatus                                                                     */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/*                                                                                                         */
/* Returns:         FIU master status                                                                      */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine returns FIU master status                                                 */
/*---------------------------------------------------------------------------------------------------------*/
UINT8 FIU_SlaveGetStatus (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_SlaveIntEnable                                                                     */
/*                                                                                                         */
/* Parameters:      intEnableMask    -  Interrupt enable mask                                              */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine enable/disable FIU master events according to input mask                  */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_SlaveIntEnable (UINT8 intEnableMask);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_SlaveRegisterCallback                                                              */
/*                                                                                                         */
/* Parameters:      fiuMasterCallback    -  Callback to FIU master events                                  */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine registers user callback for events notifications                          */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_SlaveRegisterCallback (FIU_SLAVE_INT_HANDLER fiuMasterCallback);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_SlaveUMABlock                                                                      */
/*                                                                                                         */
/* Parameters:      block  -  TRUE to block master flash read operation by the slave FIU. False otherwise  */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine blocks/release UMA for master FIU read operations                         */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_SlaveUMABlock (BOOLEAN block);
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_PrintRegs                                                                          */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module registers                                               */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_PrintRegs (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_PrintModuleRegs                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - The module to be printed.                                                 */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module instance registers                                      */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_PrintModuleRegs (FIU_MODULE_T fiu_module);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_PrintVersion                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module version                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_PrintVersion (void);

#endif  /* _FIU_IF_H */

