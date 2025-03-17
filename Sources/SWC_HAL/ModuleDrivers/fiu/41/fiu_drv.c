/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   fiu_drv.c                                                                */
/*            This file contains FIU driver implementation                    */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/
#ifndef _FIU_DRV_C
#define _FIU_DRV_C


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                                 INCLUDES                                                */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#include __CHIP_H_FROM_DRV()

#include <stdio.h>
#include <string.h>

/*---------------------------------------------------------------------------------------------------------*/
/* NVIC Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined NVIC_MODULE_TYPE
#include __MODULE_IF_HEADER_FROM_DRV(nvic)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Include FIU driver definitions                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#include "fiu_drv.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Include FIU registers definition                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#include "fiu_regs.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Local Defines                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Verbose prints                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define FIU_MSG_DEBUG(fmt,args...)
#define FIU_PROT_MSG_DEBUG(fmt,args...)

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                     LOCAL FUNCTIONS IMPLEMENTATION                                      */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_BusyWait_l                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module  - fiu module number.                                                       */
/*                  timeout     - FIU operation timeout                                                    */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine is polling on FIU transaction completeness bit.                           */
/*                  If timeout equals zero - The routine can poll forever                                  */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FIU_BusyWait_l (FIU_MODULE_T  fiu_module,
                            UINT32        timeout)
{
    if (timeout == 0)
    {
        /*-------------------------------------------------------------------------------------------------*/
        /* wait for indication that transaction has put on the bus                                         */
        /*-------------------------------------------------------------------------------------------------*/
        while (READ_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_EXEC_DONE) == FIU_TRANS_STATUS_IN_PROG){}
    }
    else
    {
        /*-------------------------------------------------------------------------------------------------*/
        /* wait for indication that transaction has put on the bus                                         */
        /*-------------------------------------------------------------------------------------------------*/
        BUSY_WAIT_TIMEOUT(READ_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_EXEC_DONE) == FIU_TRANS_STATUS_IN_PROG,
                          FIU_USEC_TO_LOOP_COUNT(timeout));
    }

    return DEFS_STATUS_OK;
}


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
void FIU_Reset (void)
{
    // TODO: how to reset FIU?
    HAL_PRINT("FIU_Reset\n");

    return;
}

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
void FIU_Init (FIU_MODULE_T fiu_module, _UNUSED_ FIU_CS_T chipSelect, UINT32 initVal)
{
    const UINT32 FIU_DRD_RST_VAL = 0x0300100B;

    if(initVal != 0)
    {
        REG_WRITE(FIU_DRD_CFG(fiu_module), initVal);
    }
    else
    {
        REG_WRITE(FIU_DRD_CFG(fiu_module), FIU_DRD_RST_VAL);
    }
}

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
                    FIU_BF_TRAP_T   bf_trap)
#else
                    _UNUSED_ FIU_IAD_TRAP_T iad_trap)
#endif
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* Configure Flash Size                                                                                */
    /*-----------------------------------------------------------------------------------------------------*/
    FIU_ConfigFlashSize( fiu_module, flash_size);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Configure read mode                                                                                 */
    /*-----------------------------------------------------------------------------------------------------*/
    FIU_ConfigReadMode( fiu_module, read_mode);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Configure burst sizes                                                                               */
    /*-----------------------------------------------------------------------------------------------------*/
    FIU_ConfigBurstSize( fiu_module, read_burst_size, write_burst_size);
}

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
void FIU_ConfigCommand (FIU_MODULE_T fiu_module, UINT8 readCommandByte, UINT8 writeCommandByte)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* read command byte. Notice that 0 command is not on the spec If user wish to avvoid configuration - set it to zero ! */
    /*-----------------------------------------------------------------------------------------------------*/
    if( readCommandByte != 0)
    {
        SET_REG_FIELD( FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_RDCMD, readCommandByte);
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* write command byte                                                                                  */
    /*-----------------------------------------------------------------------------------------------------*/
    if ( writeCommandByte != 0)
    {
        SET_REG_FIELD( FIU_DWR_CFG(fiu_module),  FIU_DWR_CFG_WRCMD, writeCommandByte);
    }
}

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
void FIU_ConfigFlashSize (FIU_MODULE_T fiu_module, FIU_DEV_SIZE_T flash_size)
{
#ifdef FIU_CAPABILITY_4B_ADDRESS
    /*-----------------------------------------------------------------------------------------------------*/
    /* Configure flash address                                                                             */
    /*-----------------------------------------------------------------------------------------------------*/
    if ( flash_size <= FIU_DEV_SIZE_16MB )
    {
        FIU_Config4ByteAddress(fiu_module, FIU_CS_0, FIU_CS_0, FALSE); // both FIU_CS_0 are ignored
    }
    else
    {
        FIU_Config4ByteAddress(fiu_module, FIU_CS_0, FIU_CS_0, TRUE); // both FIU_CS_0 are ignored
    }
#endif
}

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
void FIU_ConfigReadMode (FIU_MODULE_T fiu_module, FIU_READ_MODE_T read_mode)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* Change the access type. Warning: Once you go QUAD, you can't go back                                */
    /*-----------------------------------------------------------------------------------------------------*/
    if (( read_mode == FIU_FAST_READ) || ( read_mode == FIU_NORMAL_READ))
    {
        SET_REG_FIELD( FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_ACCTYPE, 0);
    }
    else if ( (read_mode == FIU_FAST_READ_DUAL_IO)  ||  (read_mode == FIU_FAST_READ_DUAL_OUTPUT) )
    {
        SET_REG_FIELD( FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_ACCTYPE, 1);
    }
#ifdef FIU_CAPABILITY_QUAD_READ
    else if (read_mode == FIU_FAST_READ_QUAD_IO)
    {
        SET_REG_FIELD( FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_ACCTYPE, 2);
    }
#endif
#ifdef FIU_CAPABILITY_SPI_X
    else if (read_mode == FIU_FAST_READ_SPI_X)
    {
        SET_REG_FIELD( FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_ACCTYPE, 3);
    }
#endif
    else
    {
        ASSERT(FALSE);
    }
}

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
FIU_READ_MODE_T FIU_GetReadMode (FIU_MODULE_T fiu_module)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* read FIU_DRD_CFG_ACCTYPE + FIU_DRD_CFG_DBW to get the mode                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    switch (READ_REG_FIELD( FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_ACCTYPE))
    {
        case 0:
            if ( (READ_REG_FIELD( FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_DBW)) == 1)
                return FIU_FAST_READ;
            else
                return FIU_NORMAL_READ;

        case 1:
            if ( (READ_REG_FIELD( FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_DBW)) == 1)
                return FIU_FAST_READ_DUAL_IO;
            else
                return FIU_FAST_READ_DUAL_OUTPUT;

        case 2:
            return FIU_FAST_READ_QUAD_IO;

        case 3:
            return FIU_FAST_READ_SPI_X;

        default:
            ASSERT(FALSE);
            break;
     }

     return FIU_NORMAL_READ;
}

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
void FIU_ConfigBurstSize (FIU_MODULE_T fiu_module, FIU_R_BURST_T read_burst_size, FIU_W_BURST_T write_burst_size)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* Configure write burst size                                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD( FIU_DWR_CFG(fiu_module),  FIU_DWR_CFG_W_BURST, write_burst_size);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Configure read burst size                                                                           */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD( FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_R_BURST, read_burst_size);
}

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
void FIU_ConfigBurstSize (FIU_MODULE_T  fiu_module,
                          FIU_R_BURST_T read_burst_size,
                          // Note : called from FIU_Config. IF does not have this parameter
                          FIU_W_BURST_T write_burst_size)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* Configure write burst size                                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD( FIU_DWR_CFG(fiu_module),  FIU_DWR_CFG_W_BURST, write_burst_size);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Configure read burst size                                                                           */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD( FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_R_BURST, read_burst_size);
}
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
void FIU_EnableAutoAssertion (FIU_MODULE_T fiu_module, FIU_CS_T chipSelect, BOOLEAN enable)
{
    return;
}
#endif

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
void FIU_ConfigBusFaultTrap (FIU_MODULE_T fiu_module, FIU_BF_TRAP_T bf_trap)
{
    return;
}
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
void FIU_ConfigIADTrap (_UNUSED_ FIU_MODULE_T fiu_module, _UNUSED_ FIU_IAD_TRAP_T iad_trap)
{
}
#endif // FIU_CAPABILITY_BF_ENABLE

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
void FIU_UMA_Disable (FIU_MODULE_T fiu_module)
{
    SET_REG_FIELD(FIU_DWR_CFG(fiu_module), FIU_DWR_CFG_LCK, 0x01);
}
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
                              UINT32                  data_size)
{
    UINT32  uma_cfg  = 0x0;
    UINT32  num_data_chunks;
    UINT32  remain_data;
    UINT32  idx = 0;

    FIU_MSG_DEBUG("> FIU_ManualWrite fiu=%d cs=%d ta=0x%x addr=%#08lx, %d bytes \n", fiu_module, device, transaction_code, address, data_size);

    SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_SW_CS, 0);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Constructing var for FIU_UMA_CFG register status                                                    */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_WDATSIZ, 16);        /* Setting Write Data size */

    /*-----------------------------------------------------------------------------------------------------*/
    /* Calculating relevant data                                                                           */
    /*-----------------------------------------------------------------------------------------------------*/
    num_data_chunks  = data_size / 16;
    remain_data  = data_size % 16;

    /*-----------------------------------------------------------------------------------------------------*/
    /* First we activate Chip Select (CS) for the given flash device                                       */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_DEV_NUM, (UINT32)device);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Writing the transaction code and the address to the bus                                             */
    /*-----------------------------------------------------------------------------------------------------*/
    FIU_UMA_Write(fiu_module, device, transaction_code, address, TRUE, NULL, FIU_UMA_DATA_SIZE_0, 0);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Starting the data writing loop in multiples of 16                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
    for(idx = 0; idx < num_data_chunks; ++idx)
    {
        /*-------------------------------------------------------------------------------------------------*/
        /* first byte command and follow 3 bytes address used as DATA                                      */
        /*-------------------------------------------------------------------------------------------------*/
        FIU_UMA_Write(fiu_module, device, data[0], (UINT32)NULL, FALSE, &data[1], FIU_UMA_DATA_SIZE_15, 0);

        data += 16;
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* Handling chunk remains                                                                              */
    /*-----------------------------------------------------------------------------------------------------*/
    if (remain_data > 0)
    {
        FIU_UMA_Write(fiu_module, device, data[0], (UINT32)NULL, FALSE, &data[1], (FIU_UMA_DATA_SIZE_T)(remain_data - 1), 0);
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* Finally we de-activating the Chip select and returning to "automatic" CS control                    */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_SW_CS, 1);

    return DEFS_STATUS_OK;
}

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
                            UINT32                  timeout)
{
    /* TODO:   address size should be a number, not a boolean. Here it's either 0, 3, 4  */
    /* TODO:   add mode parameter (single\dual\quad mode). */

    UINT32 uma_cfg = 0;

    DEFS_STATUS ret;

    UINT32 data_reg[4] = {0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA};

    UINT32 UMA_addr_size;

    FIU_READ_MODE_T readMode = FIU_GetReadMode(fiu_module); // assumption: read and write mode are symetric.
    UINT32 bitsPerCycle = 0; // One command bit per clock (default)

    if ((readMode == FIU_FAST_READ_DUAL_IO) || (readMode == FIU_FAST_READ_DUAL_OUTPUT))
    {
        bitsPerCycle = 1; // Two command bits transferred per clock
    }

    if ((readMode == FIU_FAST_READ_QUAD_IO) || (readMode == FIU_FAST_READ_SPI_X))
    {
        bitsPerCycle = 2; // Four command bits transferred per clock
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* First we activate Chip Select (CS) for the given flash device                                       */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_DEV_NUM, (UINT32)device);

    /*-----------------------------------------------------------------------------------------------------*/
    /* set transaction code in FIU_UMA_CODE                                                                */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD(FIU_UMA_CMD(fiu_module), FIU_UMA_CMD_CMD, transaction_code);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_CMDSIZ, 1);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Set address size bit                                                                                */
    /*-----------------------------------------------------------------------------------------------------*/
    UMA_addr_size = address_size;

    if (address_size == 1)
    {
        if (FIU_Get4ByteAddress(fiu_module, device, device))
        {
            UMA_addr_size = 4;
        }
        else
        {
            UMA_addr_size = 3;
        }
    }

    REG_WRITE(FIU_UMA_ADDR(fiu_module), address);

    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_ADDSIZ, UMA_addr_size);
    /*-----------------------------------------------------------------------------------------------------*/
    /* Set data size and direction                                                                         */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_RDATSIZ, data_size);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_WDATSIZ, 0);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_CMBPCK,  bitsPerCycle);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_ADBPCK,  bitsPerCycle);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_WDBPCK,  bitsPerCycle);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_RDBPCK,  bitsPerCycle);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_DBPCK,   bitsPerCycle);

    //FIU_MSG_DEBUG("\t\t> UMA_read fiu%d cs%d tr=0x%02x addr=0x%x (%x),    %d bytes \n",
    //                   fiu_module, device, transaction_code, address, address_size, (int)data_size);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Set UMA CFG                                                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
    REG_WRITE(FIU_UMA_CFG(fiu_module), uma_cfg);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Initiate the read                                                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_EXEC_DONE, 1);

    /*-----------------------------------------------------------------------------------------------------*/
    /* wait for indication that transaction has terminated                                                 */
    /*-----------------------------------------------------------------------------------------------------*/
    ret = FIU_BusyWait_l(fiu_module, timeout);

    /*-----------------------------------------------------------------------------------------------------*/
    /* copy read data from FIU_UMA_DB0-3 regs to data buffer                                               */
    /*-----------------------------------------------------------------------------------------------------*/
    /*-----------------------------------------------------------------------------------------------------*/
    /* Set the UMA data registers - FIU_UMA_DB0-3                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    if (data_size >= FIU_UMA_DATA_SIZE_1)
    {
        data_reg[0] = REG_READ(FIU_UMA_DR0(fiu_module));
    }
    if (data_size >= FIU_UMA_DATA_SIZE_5)
    {
        data_reg[1] = REG_READ(FIU_UMA_DR1(fiu_module));
    }
    if (data_size >= FIU_UMA_DATA_SIZE_9)
    {
        data_reg[2] = REG_READ(FIU_UMA_DR2(fiu_module));
    }
    if (data_size >= FIU_UMA_DATA_SIZE_13)
    {
        data_reg[3] = REG_READ(FIU_UMA_DR3(fiu_module));
    }

    memcpy(data, data_reg, data_size);

    FIU_MSG_DEBUG("\t\t> UMA_read fiu%d cs%d tr=0x%02x addr=0x%x (%x),  %d bytes ",
                         fiu_module, device, transaction_code, address, address_size, (int)data_size);
    FIU_MSG_DEBUG("                 %#08lx %#08lx %#08lx %#08lx \n",
                  data_reg[0], data_reg[1], data_reg[2], data_reg[3]);

    return ret;
}

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
                            UINT32                  timeout)
{
    /* TODO:   address size should be a number, not a boolean. Here it's either 0, 3, 4  */
    /* TODO:   add mode parameter (single\dual\quad mode). */

    UINT32 uma_cts;
    UINT32 uma_cfg;
    // UINT32 fiu_dwr_cfg;
    UINT32 chunk_data[FIU_MAX_UMA_DATA_SIZE/sizeof(UINT32)];
    DEFS_STATUS ret;
    FIU_READ_MODE_T readMode = FIU_GetReadMode(fiu_module); // assumption: read and write mode are symetric.
    UINT32 bitsPerCycle = 0; // One command bit per clock (default)

    if ((readMode == FIU_FAST_READ_DUAL_IO) || (readMode == FIU_FAST_READ_DUAL_OUTPUT))
    {
        bitsPerCycle = 1; // Two command bits transferred per clock
    }

    if ((readMode == FIU_FAST_READ_QUAD_IO) || (readMode == FIU_FAST_READ_SPI_X))
    {
        bitsPerCycle = 2; // Four command bits transferred per clock
    }

    FIU_MSG_DEBUG("\t\t> UMA write fiu%d cs%d ta 0x%02x address_size = %d, data_size = %d \n", fiu_module, device, transaction_code, address_size, data_size);

    /*-----------------------------------------------------------------------------------------------------*/
    /* set device number - DEV_NUM in FIU_UMA_CTS                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    DEFS_STATUS_COND_CHECK(fiu_module < FLASH_NUM_OF_MODULES, DEFS_STATUS_PARAMETER_OUT_OF_RANGE);
    DEFS_STATUS_COND_CHECK(device < FLASH_NUM_OF_DEVICES, DEFS_STATUS_PARAMETER_OUT_OF_RANGE);

    SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_RDYST, 1); // set 1 to clear

    // fiu_dwr_cfg = REG_READ(FIU_DWR_CFG(fiu_module));
    uma_cfg = 0x0;
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_CMBPCK,  bitsPerCycle);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_ADBPCK,  bitsPerCycle);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_WDBPCK,  bitsPerCycle);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_RDBPCK,  bitsPerCycle);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_DBPCK,   bitsPerCycle);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_CMDSIZ,  1);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_DBSIZ,   0);

    // in SPI-X no command byte is transferred:
    if (readMode == FIU_FAST_READ_SPI_X)
    {
        SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_CMBPCK,  0);
        SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_CMDSIZ,  0);
        SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_DBSIZ,   0);
    }

    if (address_size == 1) // backward compatible to BOOLEAN:
    {
        address_size = (READ_REG_FIELD(FIU_DWR_CFG(fiu_module), FIU_DWR_CFG_ADDSIZ) == 0) ? 3 : 4;
    }

    if(address_size)
    {
        FIU_MSG_DEBUG(" addr %#08lx, size %d Bytes, ", address, address_size);
    }

    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_ADDSIZ, address_size);

    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_WDATSIZ, data_size);     // Set data size and write direction
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_RDATSIZ, 0);
    /*-----------------------------------------------------------------------------------------------------*/
    /* Set UMA configuration                                                                               */
    /*-----------------------------------------------------------------------------------------------------*/
    REG_WRITE(FIU_UMA_CFG(fiu_module), uma_cfg);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Set transaction code (command byte source)                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    REG_WRITE(FIU_UMA_CMD(fiu_module), transaction_code);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Set the UMA address registers                                                                       */
    /*-----------------------------------------------------------------------------------------------------*/
    REG_WRITE(FIU_UMA_ADDR(fiu_module), address); // if address_size is zero, field is ignored.

    /*-----------------------------------------------------------------------------------------------------*/
    /* copy write data to FIU_UMA_DW0-3 regs                                                               */
    /*-----------------------------------------------------------------------------------------------------*/
    /*-----------------------------------------------------------------------------------------------------*/
    /* Set the UMA data registers - FIU_UMA_DW0-3                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    memset(chunk_data, 0, sizeof(chunk_data));
    if (data_size <= sizeof(chunk_data))
        memcpy(chunk_data, data, data_size);
    else
        return DEFS_STATUS_FAIL;
    REG_WRITE(FIU_UMA_DW(fiu_module, 0), chunk_data[0]);
    REG_WRITE(FIU_UMA_DW(fiu_module, 1), chunk_data[1]);
    REG_WRITE(FIU_UMA_DW(fiu_module, 2), chunk_data[2]);
    REG_WRITE(FIU_UMA_DW(fiu_module, 3), chunk_data[3]);

#if 0
    for (i=0; i<data_size; i+=4)
    {
        UINT j;
        UINT8  write_data[4];

        *(UINT32*)(&write_data) = 0;

        for (j=0; j<4; j++)
        {
            if ((j+i)<data_size)
            {
                write_data[j] = data[j+i];
            }
        }
        REG_WRITE(FIU_UMA_DW(fiu_module, i/4), *(UINT32*)(&write_data));
    }
#endif

    uma_cts = REG_READ(FIU_UMA_CTS(fiu_module));
    SET_VAR_FIELD(uma_cts, FIU_UMA_CTS_RDYIE, 0); // no interrupts here
    SET_VAR_FIELD(uma_cts, FIU_UMA_CTS_RDYST, 1); // set 1 to clear
    SET_VAR_FIELD(uma_cts, FIU_UMA_CTS_EXEC_DONE, 1);
    SET_VAR_FIELD(uma_cts, FIU_UMA_CTS_DEV_NUM, device);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Verify APB transaction completed                                                                    */
    /*-----------------------------------------------------------------------------------------------------*/
    DELAY_USEC(10, 0);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Initiate the write                                                                                  */
    /*-----------------------------------------------------------------------------------------------------*/
    REG_WRITE(FIU_UMA_CTS(fiu_module), uma_cts);


    /*-----------------------------------------------------------------------------------------------------*/
    /* wait for indication that transaction has terminated                                                 */
    /*-----------------------------------------------------------------------------------------------------*/
    ret = FIU_BusyWait_l(fiu_module, timeout);

    return ret;
}


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
DEFS_STATUS FIU_UMA_ioctl ( FIU_MODULE_T            fiu_module,
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
                            UINT                    dummy_bytes)
{
    UINT32 data_reg[4];

    UINT32 uma_cfg = REG_READ(FIU_UMA_CFG(fiu_module));

    UINT32 offset = 0;
    UINT32 size = data_rd_size;
    if (data_wr_size)
        size = data_wr_size;

    DEFS_STATUS ret = DEFS_STATUS_OK;
    const UINT32 bits_log[6] = {0, 0, 1, 1, 2, 2};

    DEFS_STATUS_COND_CHECK(cmd_bits > 0, DEFS_STATUS_PARAMETER_OUT_OF_RANGE);
    DEFS_STATUS_COND_CHECK(addr_bits > 0, DEFS_STATUS_PARAMETER_OUT_OF_RANGE);
    DEFS_STATUS_COND_CHECK(data_wr_bits > 0, DEFS_STATUS_PARAMETER_OUT_OF_RANGE);
    DEFS_STATUS_COND_CHECK(data_rd_bits > 0, DEFS_STATUS_PARAMETER_OUT_OF_RANGE);

    DEFS_STATUS_COND_CHECK(cmd_bits < 5, DEFS_STATUS_PARAMETER_OUT_OF_RANGE);
    DEFS_STATUS_COND_CHECK(addr_bits < 5, DEFS_STATUS_PARAMETER_OUT_OF_RANGE);
    DEFS_STATUS_COND_CHECK(data_wr_bits < 5, DEFS_STATUS_PARAMETER_OUT_OF_RANGE);
    DEFS_STATUS_COND_CHECK(data_rd_bits < 5, DEFS_STATUS_PARAMETER_OUT_OF_RANGE);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Set the UMA data registers - FIU_UMA_DW0-3                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    memset(data_reg, 0, sizeof(data_reg));

    /*-----------------------------------------------------------------------------------------------------*/
    /* First we activate Chip Select (CS) for the given flash device                                       */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_DEV_NUM, (UINT32)device);

    SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_SW_CS, 0);

    /*-----------------------------------------------------------------------------------------------------*/
    /* set transaction code in FIU_UMA_CODE                                                                */
    /*-----------------------------------------------------------------------------------------------------*/
    REG_WRITE(FIU_UMA_CMD(fiu_module), 0);
    SET_REG_FIELD(FIU_UMA_CMD(fiu_module), FIU_UMA_CMD_CMD, transaction_code);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_CMDSIZ, 1);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Set address                                                                                         */
    /*-----------------------------------------------------------------------------------------------------*/
    REG_WRITE(FIU_UMA_ADDR(fiu_module), address);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_ADDSIZ, address_size);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Set data size and direction                                                                         */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_RDATSIZ, data_rd_size);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_WDATSIZ, data_wr_size);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_DBSIZ,   dummy_bytes);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_CMBPCK,  bits_log[cmd_bits]);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_ADBPCK,  bits_log[addr_bits]);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_WDBPCK,  bits_log[data_wr_bits]);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_RDBPCK,  bits_log[data_rd_bits]);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_DBPCK,   bits_log[addr_bits]);

    // SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_DBSIZ,   1);

    //FIU_MSG_DEBUG("\t\t> UMA_read fiu%d cs%d tr=0x%02x addr=0x%x (%x),    %d bytes \n",
    //                   fiu_module, device, transaction_code, address, address_size, (int)data_size);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Set UMA CFG                                                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
    REG_WRITE(FIU_UMA_CFG(fiu_module), uma_cfg);

    do {
        UINT32 length = MIN(16, (size - offset)); // can only transmit 16 bytes max

        if (data_wr_size)
        {
            if (length <= sizeof(data_reg))
                memcpy(data_reg, data_wr + offset , length);
            else
                return DEFS_STATUS_FAIL;
            REG_WRITE(FIU_UMA_DW(fiu_module, 0), data_reg[0]);
            REG_WRITE(FIU_UMA_DW(fiu_module, 1), data_reg[1]);
            REG_WRITE(FIU_UMA_DW(fiu_module, 2), data_reg[2]);
            REG_WRITE(FIU_UMA_DW(fiu_module, 3), data_reg[3]);
            SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_WDATSIZ, length);
        }

        REG_WRITE(FIU_UMA_ADDR(fiu_module), address + offset);

        if(data_rd_size)
            SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_RDATSIZ, length);

        REG_WRITE(FIU_UMA_CFG(fiu_module), uma_cfg);

        /*-----------------------------------------------------------------------------------------------------*/
        /* Initiate the ioctl                                                                                  */
        /*-----------------------------------------------------------------------------------------------------*/
        SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_EXEC_DONE, 1);

        /*-----------------------------------------------------------------------------------------------------*/
        /* wait for indication that transaction has terminated                                                 */
        /*-----------------------------------------------------------------------------------------------------*/
        ret = FIU_BusyWait_l(fiu_module, timeout);

        /*-----------------------------------------------------------------------------------------------------*/
        /* copy read data from FIU_UMA_DB0-3 regs to data buffer                                               */
        /*-----------------------------------------------------------------------------------------------------*/
        /*-----------------------------------------------------------------------------------------------------*/
        /* Set the UMA data registers - FIU_UMA_DB0-3                                                          */
        /*-----------------------------------------------------------------------------------------------------*/
        if (data_rd_size)
        {
            if (length >= FIU_UMA_DATA_SIZE_1)
            {
                data_reg[0] = REG_READ(FIU_UMA_DR0(fiu_module));
            }
            if (length >= FIU_UMA_DATA_SIZE_5)
            {
                data_reg[1] = REG_READ(FIU_UMA_DR1(fiu_module));
            }
            if (length >= FIU_UMA_DATA_SIZE_9)
            {
                data_reg[2] = REG_READ(FIU_UMA_DR2(fiu_module));
            }
            if (length >= FIU_UMA_DATA_SIZE_13)
            {
                data_reg[3] = REG_READ(FIU_UMA_DR3(fiu_module));
            }


            memcpy(data_rd + offset, data_reg, length);

            /* clear dummy bytes settings */
            SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_DBSIZ, 0);
            REG_WRITE(FIU_UMA_CFG(fiu_module), uma_cfg);
        }

        FIU_MSG_DEBUG("\t\t> UMA_ioctl fiu%d cs%d tr=0x%02x addr=0x%x (%x), rd %u bytes, wr %u bytes, uma_size %u, uma_cfg = %#010lx ",
                             fiu_module, device, transaction_code, address + offset, address_size, data_rd_size, data_wr_size, length, uma_cfg);

        FIU_MSG_DEBUG("                 %#08lx %#08lx %#08lx %#08lx \n",
                      data_reg[0], data_reg[1], data_reg[2], data_reg[3]);

        offset += 16;

        /* After writing the first 16 bytes no need to send address and command for the next sequence */
        SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_ADDSIZ, 0);
        SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_CMDSIZ, 0);

    } while (offset < size);

    SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_SW_CS, 1);

    return ret;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_PageWrite                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*              fiu_module       - fiu module number. Value is ignored if only one FIU module on chip      */
/*              device           - Select the flash device to be accessed                                  */
/*              transaction_code - Specify the SPI UMA transaction code                                    */
/*              address          - Location on the flash, in the flash address space                       */
/*              data             - a pointer to a data buffer (buffer of bytes)                            */
/*              data_size        - data buffer size in bytes                                               */
/*              timeout          - command timeout                                                         */
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
                            UINT32          timeout)
{
    /* TODO:   address size should be a number, not a boolean. Here it's either 0, 3, 4  */
    /* TODO:   add mode parameter (single\dual\quad mode). */

    //UINT   i;
    UINT32 uma_cts;
    UINT32 uma_cfg;
    UINT32 fiu_dwr_cfg;
    UINT16 chunk_size;
    DEFS_STATUS ret;

    /*-----------------------------------------------------------------------------------------------------*/
    /* set device number - DEV_NUM in FIU_UMA_CTS                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    DEFS_STATUS_COND_CHECK (device < FLASH_NUM_OF_DEVICES, DEFS_STATUS_PARAMETER_OUT_OF_RANGE);

    fiu_dwr_cfg = REG_READ(FIU_DWR_CFG(fiu_module));
    uma_cfg = 0x0;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Send the Address                                                                                    */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_CMBPCK,  0);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_ADBPCK,  READ_VAR_FIELD(fiu_dwr_cfg, FIU_DWR_CFG_ABPCK));
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_WDBPCK,  READ_VAR_FIELD(fiu_dwr_cfg, FIU_DWR_CFG_DBPCK));
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_DBPCK,   0);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_RDBPCK,  0);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_CMDSIZ,  1);
    // Set address size bytes to 3 if FIU_DWR.CFG_ADDSIZ == 0 and to 4 otherwize
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_ADDSIZ,  (READ_VAR_FIELD(fiu_dwr_cfg, FIU_DWR_CFG_ADDSIZ)==0)?3:4);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_DBSIZ,   0);
    SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_RDATSIZ, 0);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Set transaction code (command byte source)                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    REG_WRITE(FIU_UMA_CMD(fiu_module), transaction_code);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Set the UMA address registers                                                                       */
    /*-----------------------------------------------------------------------------------------------------*/
    REG_WRITE(FIU_UMA_ADDR(fiu_module), address);

    uma_cts = REG_READ(FIU_UMA_CTS(fiu_module));
    SET_VAR_FIELD(uma_cts, FIU_UMA_CTS_RDYST, 1); // set 1 to clear
    SET_VAR_FIELD(uma_cts, FIU_UMA_CTS_EXEC_DONE, 1);
    SET_VAR_FIELD(uma_cts, FIU_UMA_CTS_DEV_NUM, device % 4);
    SET_VAR_FIELD(uma_cts, FIU_UMA_CTS_SW_CS, 0);

#if 0
    /*-----------------------------------------------------------------------------------------------------*/
    /* Initiate the read                                                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
    REG_WRITE(FIU_UMA_CTS(fiu_module), uma_cts);

    /*-----------------------------------------------------------------------------------------------------*/
    /* wait for indication that transaction has terminated                                                 */
    /*-----------------------------------------------------------------------------------------------------*/
    ret = FIU_BusyWait_l(fiu_module, timeout);
    if (ret != DEFS_STATUS_OK)
    {
        SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_SW_CS, 1); // cs returns to default
        return ret;
    }
#endif

    /*-----------------------------------------------------------------------------------------------------*/
    /* Send the Data                                                                                      */
    /*-----------------------------------------------------------------------------------------------------*/
    while (data_size)
    {
        UINT32 chunk_data[FIU_MAX_UMA_DATA_SIZE/sizeof(UINT32)];

        /*-----------------------------------------------------------------------------------------------------*/
        /* In order to save the busy wait time we first do all calculation than we wait for previous           */
        /* transaction to end and only then update the registers                                               */
        /*-----------------------------------------------------------------------------------------------------*/
        chunk_size = MIN(FIU_MAX_UMA_DATA_SIZE, data_size);

        SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_WDATSIZ, chunk_size);     // Set data size and write direction
        if (chunk_size <= sizeof (chunk_data))
            memcpy (chunk_data, data, chunk_size);
        else
             return DEFS_STATUS_FAIL;
        /*-----------------------------------------------------------------------------------------------------*/
        /* wait for indication that transaction has terminated                                                 */
        /*-----------------------------------------------------------------------------------------------------*/
        ret = FIU_BusyWait_l(fiu_module, timeout);
        if (ret != DEFS_STATUS_OK)
        {
            SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_SW_CS, 1); // cs returns to default
            return ret;
        }

        /*-----------------------------------------------------------------------------------------------------*/
        /* copy write data to FIU_UMA_DW0-3 regs                                                               */
        /*-----------------------------------------------------------------------------------------------------*/
        /*-----------------------------------------------------------------------------------------------------*/
        /* Set the UMA data registers - FIU_UMA_DW0-3                                                          */
        /*-----------------------------------------------------------------------------------------------------*/
        REG_WRITE(FIU_UMA_DW(fiu_module, 0), chunk_data[0]);
        REG_WRITE(FIU_UMA_DW(fiu_module, 1), chunk_data[1]);
        REG_WRITE(FIU_UMA_DW(fiu_module, 2), chunk_data[2]);
        REG_WRITE(FIU_UMA_DW(fiu_module, 3), chunk_data[3]);

#if 0
        for (i=0; i<chunk_size; i+=4)
        {
            UINT j;
            UINT8  write_data[4];

            *(UINT32*)(&write_data) = 0;

            for (j=0; j<4; j++)
            {
                if ((j+i)<chunk_size)
                {
                    write_data[j] = data[j+i];
                }
            }
            REG_WRITE(FIU_UMA_DW(fiu_module, i/4), *(UINT32*)(&write_data));
        }
#endif
        /*-----------------------------------------------------------------------------------------------------*/
        /* Set UMA configuration                                                                               */
        /*-----------------------------------------------------------------------------------------------------*/
        REG_WRITE(FIU_UMA_CFG(fiu_module), uma_cfg);

        /*-----------------------------------------------------------------------------------------------------*/
        /* Initiate the write                                                                                  */
        /*-----------------------------------------------------------------------------------------------------*/
        REG_WRITE(FIU_UMA_CTS(fiu_module), uma_cts);

        // next transactions do not include command and address
        if (READ_VAR_FIELD(uma_cfg, FIU_UMA_CFG_CMDSIZ))
        {
            SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_CMDSIZ,  0);  // no command needed
            SET_VAR_FIELD(uma_cfg, FIU_UMA_CFG_ADDSIZ,  0);  // no address needed
        }

        data_size -= chunk_size;
        data += chunk_size;
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* wait for indication that last transaction has terminated                                            */
    /*-----------------------------------------------------------------------------------------------------*/
    ret = FIU_BusyWait_l(fiu_module, timeout);

    SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_SW_CS, 1); // cs returns to default
    return ret;
}

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
void FIU_Config4ByteAddress (FIU_MODULE_T fiu_module, _UNUSED_ FIU_CS_T device, _UNUSED_ FIU_CS_T chipSelect, BOOLEAN enable)
{

    FIU_MSG_DEBUG("\t%s fiu%d %sable 4B\n", __FUNCTION__, fiu_module, (enable == TRUE)? "en" : "dis");

    if(enable)
    {
        SET_REG_FIELD( FIU_DRD_CFG(fiu_module) ,  FIU_DRD_CFG_ADDSIZ,  FIU_DRD_CFG_ADDSIZE_32BIT) ;
        SET_REG_FIELD( FIU_DWR_CFG(fiu_module) ,  FIU_DWR_CFG_ADDSIZ,  FIU_DRD_CFG_ADDSIZE_32BIT) ;
    }
    else
    {
        SET_REG_FIELD( FIU_DRD_CFG(fiu_module) ,  FIU_DRD_CFG_ADDSIZ,  FIU_DRD_CFG_ADDSIZE_24BIT) ;
        SET_REG_FIELD( FIU_DWR_CFG(fiu_module) ,  FIU_DWR_CFG_ADDSIZ,  FIU_DRD_CFG_ADDSIZE_24BIT) ;
    }
}

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
BOOLEAN FIU_Get4ByteAddress (FIU_MODULE_T fiu_module, _UNUSED_ FIU_CS_T device, _UNUSED_ FIU_CS_T chipSelect)
{
    // Assuming write and read are both in the same mode.
    return !!READ_REG_FIELD( FIU_DRD_CFG(fiu_module) ,  FIU_DRD_CFG_ADDSIZ) ;
}
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
void FIU_ConfigDummyCyles (FIU_MODULE_T fiu_module, UINT8 dummyCycles, _UNUSED_ BOOLEAN enable)
{
    // HW can handle only multiplies of 8 cycles
    ASSERT((dummyCycles % 8) == 0);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Config read dummy cycles                                                                            */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD( FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_DBW, dummyCycles/8);

    return;
}

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
UINT8 FIU_GetDummyCyles (FIU_MODULE_T fiu_module)
{
    return READ_REG_FIELD(FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_DBW);
}

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
BOOLEAN FIU_GetDummyCylesEnable (FIU_MODULE_T fiu_module)
{
    return !!FIU_GetDummyCyles(fiu_module);
}
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
void FIU_SelectChip (FIU_MODULE_T fiu_module, FIU_CS_T device, UINT32 address)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* First we activate Chip Select (CS) for the given flash device                                       */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_DEV_NUM, (UINT32)device);

    if (address < FLASH_MEMORY_SIZE(fiu_module))
    {
        SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_SW_CS, 0);
    }
    else
    {
        // address out of range, release CS~
        SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_SW_CS, 1);
    }
}

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
void FIU_ConfigChipSelect (FIU_MODULE_T fiu_module, FIU_CS_T cs)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* First we activate Chip Select (CS) for the given flash device                                       */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_DEV_NUM, (UINT32)cs);

    return;
}

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
FIU_CS_T FIU_GetChipSelect (FIU_MODULE_T fiu_module)
{
    return (FIU_CS_T)READ_REG_FIELD(FIU_UMA_CTS(fiu_module), FIU_UMA_CTS_DEV_NUM);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigLowDevSize                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu_module - fiu module number. Value is ignored if only one FIU module on chip        */
/*                  device     - Select the flash device to be accessed.                                   */
/*                  size       - SPI Low Device size.                                                      */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures the Low Device size                                            */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_ConfigLowDevSize (_UNUSED_ FIU_MODULE_T fiu_module, _UNUSED_ FIU_CS_T device, _UNUSED_ FIU_LOW_DEV_SIZE_T size)
{
    return; // not needed in 41 version
}

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
UINT32 FIU_GetLowDevSize (_UNUSED_ FIU_MODULE_T fiu_module, _UNUSED_ FIU_CS_T device)
{
    return 0; // not needed in 41 version
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigLowDevSize                                                                   */
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
void FIU_EnableTwoDevices (_UNUSED_ FIU_MODULE_T fiu_module,
                           _UNUSED_ FIU_CS_T device,
                           _UNUSED_ UINT option,
                           _UNUSED_ BOOLEAN enable)
{
    return;  // not needed in 41 version
}

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
BOOLEAN FIU_TwoDevicesEnabled (_UNUSED_ FIU_MODULE_T fiu_module, _UNUSED_ FIU_CS_T device)
{
    return FALSE; //  // not needed in 41 version
}
#endif

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
DEFS_STATUS FIU_ConfigDummyCyclesCount (FIU_MODULE_T fiu_module, FIU_DUMMY_COUNT_T readDummyCycles)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* Config read dummy cycles                                                                            */
    /*-----------------------------------------------------------------------------------------------------*/
    if( readDummyCycles != FIU_DUMMY_SKIP_CONFIG)
    {
        if ( (readDummyCycles == FIU_DUMMY_COUNT_0) || (readDummyCycles == FIU_DUMMY_COUNT_0p5))
        {
            SET_REG_FIELD( FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_DBW, 0);
        }
        else if (readDummyCycles == FIU_DUMMY_COUNT_1)
        {
            SET_REG_FIELD( FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_DBW, 1);
        }
        else if (readDummyCycles == FIU_DUMMY_COUNT_2)
        {
            SET_REG_FIELD( FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_DBW, 2);
        }
        else if (readDummyCycles == FIU_DUMMY_COUNT_3)
        {
            SET_REG_FIELD( FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_DBW, 3);
        }
        else if (readDummyCycles == FIU_DUMMY_COUNT_4)
        {
            SET_REG_FIELD( FIU_DRD_CFG(fiu_module),  FIU_DRD_CFG_DBW, 4);
        }
        else
        {
            return DEFS_STATUS_PARAMETER_OUT_OF_RANGE;
        }
    }

    return DEFS_STATUS_OK;
}

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
DEFS_STATUS FIU_SetDirection (FIU_MODULE_T fiu_module , FIU_DIRECTION_MODE_T mode)
{
    SET_REG_FIELD(FIU_CFG(fiu_module), FIU_CFG_INCRSING, mode);

    if(mode == FIU_DIRECTION_MODE_INCREASING)
    {
        SET_REG_FIELD(FIU_CFG(fiu_module), FIU_CFG_FIU_FIX, 1);
    }

    return DEFS_STATUS_OK;
}

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
                         FIU_ERR_CHECK_READ_MODE_T readMode)
{
}

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
void FIU_CrcCksmStart (FIU_MODULE_T fiu_module)
{
}

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
void FIU_CrcCksmWrite (FIU_MODULE_T fiu_module, const UINT8* buffer, UINT32 bufferLen)
{
}

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
UINT32 FIU_CrcCksmStopAndResult (FIU_MODULE_T fiu_module)
{
}
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
void FIU_SlaveInit (void)
{
}

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
void FIU_MasterConfig (UINT8 arbThresh, UINT8 cntThresh)
{
}

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
UINT8 FIU_SlaveGetStatus (void)
{
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_SlaveIntEnable                                                                     */
/*                                                                                                         */
/* Parameters:      intEnableMask    -  Interrupt enable mask                                              */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine enable/disable FIU master events according to input mask                  */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_SlaveIntEnable (UINT8 intEnableMask)
{
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_SlaveRegisterCallback                                                              */
/*                                                                                                         */
/* Parameters:      fiuMasterCallback    -  Callback to FIU master events                                  */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine registers user callback for events notifications                          */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_SlaveRegisterCallback (FIU_SLAVE_INT_HANDLER fiuMasterCallback)
{

}

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
void FIU_SlaveUMABlock (BOOLEAN block)
{
}
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_SetProtectionRegion                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu -  FIU module                                                                      */
/*                  index -  region index, up to 6                                                         */
/*                  startAddr - start addresses of range.                                                  */
/*                  endAddr - end addresses off range.                                                     */
/*                  csUse - 0 invalid, 1 CS0, 2 CS1, 3 - both CS.                                          */
/*                                                                                                         */
/* Returns:         DEFS_STATUS                                                                            */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine sets a region in the flash protection.                                    */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FIU_SetProtectionRegion (FIU_MODULE_T fiu, UINT index, UINT32 startAddr, UINT32 endAddr, UINT csUse)
{
    UINT32 val = 0;

    if (index >= FIU_PRT_RANG_MAX)
    {
        FIU_PROT_MSG_DEBUG("FIU%d region ind %d too big\n", fiu, index);
        return DEFS_STATUS_PARAMETER_OUT_OF_RANGE;
    }

    SET_VAR_FIELD(val, FIU_PRT_RANG_STRTRANGm, startAddr >> 14);
    SET_VAR_FIELD(val, FIU_PRT_RANG_LASTRANGm, (endAddr - 0x3FFF) >> 14);
    SET_VAR_FIELD(val, FIU_PRT_RANG_CSUSE, csUse);

    REG_WRITE(FIU_PRT_RANG (fiu, index), val);
    FIU_PROT_MSG_DEBUG("FIU%d FIU_PRT_RANG%d %#010lx\n", fiu, index, REG_READ(FIU_PRT_RANG (fiu, index)));
    return DEFS_STATUS_OK;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_SetProtectionCommand                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  addrBits - 1/2/4 bits                                                                  */
/*                  addrBytes - 4/3 bytes. For commands relevant for entire chip field is don't care.      */
/*                  cmdBits - 1/2/4                                                                        */
/*                  command - command byte                                                                 */
/*                  com_index - up to 40 commands are supported.                                           */
/*                  fiu -  FIU module                                                                      */
/*                  range_ind - index of range where cmd is blocked. 6 - lock of entire flash. 7 - always OK */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine sets a command to enable or block.                                        */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FIU_SetProtectionCommand (FIU_MODULE_T fiu, UINT com_index, UINT range_ind, UINT8 command,
                                        UINT addrBytes, UINT addrBits, UINT cmdBits)
{
    UINT32 val = 0;
    UINT reg_ind = com_index >> 1;
    UINT reg_A_B = com_index % 2;

    if (reg_ind >= FIU_PRT_CMD_MAX)
    {
        FIU_PROT_MSG_DEBUG("FIU%d cmd ind %d too big\n", fiu, reg_ind);
        return DEFS_STATUS_PARAMETER_OUT_OF_RANGE;
    }

    SET_VAR_FIELD(val, FIU_PRT_CMD_ADBPCKA, addrBits - 1);
    SET_VAR_FIELD(val, FIU_PRT_CMD_CMBPCKA, cmdBits - 1);
    SET_VAR_FIELD(val, FIU_PRT_CMD_ADDSZA, (addrBytes == 4) ? 1 : 0);

    /* 3 bits for address range selection [15, 9, 8] */
    SET_VAR_FIELD(val, FIU_PRT_CMD_ADRNGSELA, (range_ind >> 2) & 0x01);
    SET_VAR_FIELD(val, FIU_PRT_CMD_FRBDCA, range_ind & 0x3);
    SET_VAR_FIELD(val, FIU_PRT_CMD_CMDA, command);

    if (reg_A_B == 0)
    {
        SET_REG_FIELD(FIU_PRT_CMD (fiu, reg_ind), FIU_PRT_CMD_A, val);
    }
    else
    {
        SET_REG_FIELD(FIU_PRT_CMD (fiu, reg_ind), FIU_PRT_CMD_B, val);
    }

    FIU_PROT_MSG_DEBUG("FIU%d FIU_PRT_CMD%d %#010lx val %#010lx addr %#010lx \n",
        fiu, reg_ind, REG_READ(FIU_PRT_CMD(fiu, reg_ind)), val, REG_ADDR(FIU_PRT_CMD(fiu, reg_ind)));

    return DEFS_STATUS_OK;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_ConfigProtection                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  bEnable - enable\disable. (disable only if not previously locked)                      */
/*                  devSize - size of flash                                                                */
/*                  fiu -  fiu module                                                                      */
/*                  lock - lock the flash protection.                                                      */
/*                  settings - handler for various parameters required for flash protecton.                */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configure flas protection.                                                */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FIU_ConfigProtection (FIU_MODULE_T fiu, BOOLEAN lock, BOOLEAN bEnable, UINT32 devSize, FIU_PROTECTION_SETTING_T *settings)
{
    UINT32 val = 0;
    UINT32 devSizeInMega;

    DEFS_STATUS_COND_CHECK(settings != NULL, DEFS_STATUS_INVALID_PARAMETER);
    DEFS_STATUS_COND_CHECK(devSize > _128MB_, DEFS_STATUS_INVALID_PARAMETER);

    devSizeInMega = devSize >> 20;

    SET_VAR_FIELD(val, FIU_PRT_CFG_DEVSIZ, devSizeInMega ? LOG_4(devSizeInMega) : 0);
    SET_VAR_FIELD(val, FIU_PRT_CFG_FCS0, settings->cs_enable[0]);
    SET_VAR_FIELD(val, FIU_PRT_CFG_FCS0_LCK, settings->cs_value[0]);
    SET_VAR_FIELD(val, FIU_PRT_CFG_FCS1, settings->cs_enable[1]);
    SET_VAR_FIELD(val, FIU_PRT_CFG_FCS1_LCK,  settings->cs_value[1]);
    SET_VAR_FIELD(val, FIU_PRT_CFG_FIO2_ENLCK, settings->io2_enable);
    SET_VAR_FIELD(val, FIU_PRT_CFG_FIO2_LVL, settings->io2_value);

    SET_VAR_FIELD(val, FIU_PRT_CFG_PVE, 1); // clear status of "Protection Violation Encountered" status.

    if (settings->handler != NULL)
    {
        SET_VAR_FIELD(val, FIU_PRT_CFG_PVIE, 1);
        /*-------------------------------------------------------------------------------------*/
        /* Registering event handler to GIC\NVIC                                               */
        /*-------------------------------------------------------------------------------------*/
        INTERRUPT_REGISTER_AND_ENABLE(FIU_INTERRUPT_PROVIDER, FIU_INTERRUPT(fiu), settings->handler, \
                                        FIU_INTERRUPT_POLARITY, FIU_INTERRUPT_PRIORITY);
    }

    SET_VAR_FIELD(val, FIU_PRT_CFG_OCALWD, settings->otherCommandsAllowed);
    SET_VAR_FIELD(val, FIU_PRT_CFG_PEN, bEnable);

    REG_WRITE(FIU_PRT_CFG (fiu), val);

    if (lock == TRUE)
    {
        SET_REG_FIELD(FIU_PRT_CFG(fiu), FIU_PRT_CFG_LCK, 1);
    }

    FIU_PROT_MSG_DEBUG("FIU%d FIU_PRT_CFG %#010lx val %#010lx addr %#010lx \n", fiu,
                    REG_READ(FIU_PRT_CFG(fiu)), val, REG_ADDR(FIU_PRT_CFG(fiu)));

    return DEFS_STATUS_OK;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function         FIU_GetProtectionViolationStatus                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fiu -  fiu module index                                                                */
/*                  clearStatus -                                                                          */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine returns the status of the protection. TRUE: violation accored.            */
/*                  FALSE: otherwise.                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN FIU_GetProtectionViolationStatus (FIU_MODULE_T fiu, BOOLEAN clearStatus)
{
    BOOLEAN ret_val = (BOOLEAN)READ_REG_FIELD(FIU_PRT_CFG(fiu), FIU_PRT_CFG_PVE);

    if (clearStatus == TRUE)
    {
        SET_REG_FIELD(FIU_PRT_CFG(fiu), FIU_PRT_CFG_PVE, 1); // W1C
    }

    return ret_val;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_PrintRegsModules_l                                                                 */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module registers per module                                    */
/*---------------------------------------------------------------------------------------------------------*/
static void FIU_PrintRegsModules_l (_UNUSED_ FIU_MODULE_T n)
{
    HAL_PRINT("/*--------------*/\n");
    HAL_PRINT("/*     FIU%d     */\n", n);
    HAL_PRINT("/*--------------*/\n\n");
    HAL_PRINT("FIU_DRD_CFG(n)       = %#010lx \n", REG_READ(FIU_DRD_CFG(n)));
    HAL_PRINT("FIU_DWR_CFG(n)       = %#010lx \n", REG_READ(FIU_DWR_CFG(n)));
    HAL_PRINT("FIU_UMA_CFG(n)       = %#010lx \n", REG_READ(FIU_UMA_CFG(n)));
    HAL_PRINT("FIU_UMA_CTS(n)       = %#010lx \n", REG_READ(FIU_UMA_CTS(n)));
    HAL_PRINT("FIU_UMA_CMD(n)       = %#010lx \n", REG_READ(FIU_UMA_CMD(n)));
    HAL_PRINT("FIU_UMA_ADDR(n)      = %#010lx \n", REG_READ(FIU_UMA_ADDR(n)));
    HAL_PRINT("FIU_UMA_DW0(n)       = %#010lx \n", REG_READ(FIU_UMA_DW0(n)));
    HAL_PRINT("FIU_UMA_DW1(n)       = %#010lx \n", REG_READ(FIU_UMA_DW1(n)));
    HAL_PRINT("FIU_UMA_DW2(n)       = %#010lx \n", REG_READ(FIU_UMA_DW2(n)));
    HAL_PRINT("FIU_UMA_DW3(n)       = %#010lx \n", REG_READ(FIU_UMA_DW3(n)));
    HAL_PRINT("FIU_UMA_DR0(n)       = %#010lx \n", REG_READ(FIU_UMA_DR0(n)));
    HAL_PRINT("FIU_UMA_DR1(n)       = %#010lx \n", REG_READ(FIU_UMA_DR1(n)));
    HAL_PRINT("FIU_UMA_DR2(n)       = %#010lx \n", REG_READ(FIU_UMA_DR2(n)));
    HAL_PRINT("FIU_UMA_DR3(n)       = %#010lx \n", REG_READ(FIU_UMA_DR3(n)));
    HAL_PRINT("FIU_PRT_CFG(n)       = %#010lx \n", REG_READ(FIU_PRT_CFG(n)));

    for (int m = 0 ; m < FIU_PRT_CMD_MAX; m++)
        HAL_PRINT("FIU_PRT_CMD(%d, %d)      = %#010lx \n", n, m, REG_READ(FIU_PRT_CMD(n, m)));

    for (int m = 0 ; m < FIU_PRT_RANG_MAX; m++)
        HAL_PRINT("FIU_PRT_RANG(%d, %d)      = %#010lx \n", n, m, REG_READ(FIU_PRT_RANG(n, m)));

    HAL_PRINT("FIU_STPL_CFG(n)      = %#010lx \n", REG_READ(FIU_STPL_CFG(n)));
    HAL_PRINT("FIU_CFG(n)           = %#010lx \n", REG_READ(FIU_CFG(n)));
    HAL_PRINT("FIU_VER(n)           = %#010lx \n", REG_READ(FIU_VER(n)));
    HAL_PRINT("FIU_DLY_CFG(n)       = %#010lx \n", REG_READ(FIU_DLY_CFG(n)));
    HAL_PRINT("FIU_PRT_RANG(n, 0)   = %#010lx \n", REG_READ(FIU_PRT_RANG(n, 0)));

    HAL_PRINT("\n");
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_PrintRegs                                                                          */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module registers                                               */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_PrintRegs (void)
{
    FIU_PrintRegsModules_l(FIU_MODULE_0);
    FIU_PrintRegsModules_l(FIU_MODULE_1);
    FIU_PrintRegsModules_l(FIU_MODULE_3);
    FIU_PrintRegsModules_l(FIU_MODULE_X);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FIU_PrintVersion                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module version                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void FIU_PrintVersion (void)
{
    HAL_PRINT("FIU         = %X\n", MODULE_VERSION(FIU_MODULE_TYPE));
}
#endif // _FIU_DRV_C

