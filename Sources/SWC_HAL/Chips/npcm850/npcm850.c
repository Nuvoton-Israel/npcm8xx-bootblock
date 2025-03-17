/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   npcm850.c                                                                */
/*            This file contains chip dependent functionality implementation  */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                                 INCLUDES                                                */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#include __CHIP_H_FROM_CHIP()

#include "npcm850_if.h"
#include "hal_regs.h"

#if defined (FUSE_MODULE_TYPE)
#include "npcm850_fuse_wrapper.h"
#endif


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                           TYPES & DEFINITIONS                                           */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/


const UINT FLASH_ADDR_ARRAY_NPCM850[FLASH_NUM_OF_MODULES * FLASH_NUM_OF_DEVICES] =
                                                        {SPI0CS0_BASE_ADDR,
                                                        SPI0CS1_BASE_ADDR,
                                                        0,
                                                        0,

                                                        SPI1CS0_BASE_ADDR,
                                                        SPI1CS1_BASE_ADDR,
                                                        SPI1CS2_BASE_ADDR,
                                                        SPI1CS3_BASE_ADDR,

                                                        SPI3CS0_BASE_ADDR,
                                                        SPI3CS1_BASE_ADDR,
                                                        SPI3CS2_BASE_ADDR,
                                                        SPI3CS3_BASE_ADDR

                                                        // SPI-X:
                                                        /* ,
                                                        SPIXCS0_BASE_ADDR,
                                                        SPIXCS1_BASE_ADDR
                                                        */
                                                        };



/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                           INTERFACE FUNCTIONS                                           */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------*/
/* Register access macroes                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define POR_STATUS()         !(READ_REG_FIELD(INTCR2, INTCR2_PORST))             // Reset source was POR (powerOn) reset
#define COR_STATUS()         !(READ_REG_FIELD(INTCR2, INTCR2_CORST))             // Reset source was COR reset
#define MARK_CFG_DONE()      SET_REG_FIELD(INTCR2, INTCR2_CFGDONE, 1)           // Chip configuration (MC, CLK, Flash, etc') is done
#define CFG_IS_DONE()        (READ_REG_FIELD(INTCR2, INTCR2_CFGDONE))           // Chip configuration (MC, CLK, Flash, etc') is done
#define ENABLE_JTAG()        SET_REG_FIELD(FLOCKR1, FLOCKR1_JTAGDIS, 0)         // Rnables J-Tag
#define DISABLE_JTAG()       SET_REG_FIELD(FLOCKR1, FLOCKR1_JTAGDIS, 1)         // Disables J-Tag


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                           INTERFACE FUNCTIONS                                           */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

#if defined (UART_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_Mux_Uart                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  dev -                                                                                  */
/*                  CoreSP -                                                                               */
/*                  sp1 -                                                                                  */
/*                  sp2 -                                                                                  */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects UART muxing                                                       */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS CHIP_Mux_Uart (UART_DEV_T dev, BOOLEAN CoreSP, BOOLEAN sp1, BOOLEAN sp2)
{
    // UART4-6 and TIP UART ("UART7") are each a general UART with connection to external signals

    if (dev == UART4_DEV)
    {
        SET_REG_FIELD(MFSEL5, MFSEL5_BU4SEL, 1);
        SET_REG_FIELD(MFSEL1, MFSEL1_HSI2CSEL, 0);

        return DEFS_STATUS_OK;
    }
    else if (dev == UART5_DEV)
    {
        SET_REG_FIELD(MFSEL5, MFSEL5_BU5SEL, 1);
        SET_REG_FIELD(MFSEL1, MFSEL1_HSI2CSEL, 0);

        return DEFS_STATUS_OK;
    }
    else if (dev == UART6_DEV)
    {
        SET_REG_FIELD(MFSEL5, MFSEL5_BU6SEL, 1);
        SET_REG_FIELD(MFSEL1, MFSEL1_HSI2BSEL, 0);
        SET_REG_FIELD(MFSEL7, MFSEL7_TPUARTSEL, 0);

        return DEFS_STATUS_OK;
    }
    else if (dev == UART_TIP_DEV)
    {
        SET_REG_FIELD(MFSEL7, MFSEL7_TPUARTSEL, 1);
        SET_REG_FIELD(MFSEL5, MFSEL5_BU6SEL, 0);
        SET_REG_FIELD(MFSEL1, MFSEL1_HSI2BSEL, 0);

        return DEFS_STATUS_OK;
    }
    else if (dev == UART_CP_DEV)
    {
        SET_REG_FIELD(MFSEL6, MFSEL6_CP1URXDSEL, 1);
        SET_REG_FIELD(MFSEL6, MFSEL6_CP1UTXDSEL, 1);

        return DEFS_STATUS_OK;
    }

    // Other UARTs are configured according to the sp mode

    if (sp1)
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_HSI1ASEL, 1);
        SET_REG_FIELD(MFSEL4, MFSEL4_BSPASEL, 0); /* Select TXD1+RXD1 */
    }

    if (sp2)
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_HSI2ASEL, 1);
        SET_REG_FIELD(MFSEL4, MFSEL4_BSPASEL, 0); /* Select TXD2+RXD2 */
    }

    if (CoreSP)
    {
        if (READ_REG_FIELD(PWRON, PWRON_BSPA) == 0)
        {
            SET_REG_FIELD(MFSEL4, MFSEL4_BSPASEL, 1);

            // Note: If this bit is set, MFSEL1 bit 9 and 11 must be set to 0.
            SET_REG_FIELD(MFSEL1, MFSEL1_BSPSEL, 0);
            SET_REG_FIELD(MFSEL1, MFSEL1_HSI2ASEL, 0);
        }
        else
        {
            SET_REG_FIELD(MFSEL4, MFSEL4_BSPASEL, 0);
            SET_REG_FIELD(MFSEL1, MFSEL1_BSPSEL, 1);
        }
    }

    return DEFS_STATUS_OK;
}
#endif // UART_MODULE_TYPE

#if defined (FIU_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        SPI_Quad_Enable                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  devNum  - SPI Flash device number                                                      */
/*                  FIU_cs - CS SPI to configure.                                                          */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine qonfigures the SPI to be QUAD mode enabled                                */
/*---------------------------------------------------------------------------------------------------------*/
void SPI_Quad_Enable(FIU_MODULE_T devNum, FIU_CS_T FIU_cs)
{
    UINT8 status2;
    FIU_UMA_Read(devNum,  FIU_cs, 0x35, 0, FALSE, &status2, FIU_UMA_DATA_SIZE_1, 0);
    if ((status2 & 0x2) == 0)
    {
        status2 |= 0x2;
        // write as non volatile register
        FIU_UMA_Write(devNum, FIU_cs, 0x50,  0, FALSE, NULL, FIU_UMA_DATA_SIZE_0, 0);
        FIU_UMA_Write(devNum, FIU_cs, 0x31, 0, FALSE, &status2, FIU_UMA_DATA_SIZE_1, 0);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_MuxFIU                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  devNum  - SPI Flash device number                                                      */
/*                  csXEnable - enable CS.                                                                 */
/*                  quadMode -  is Quad mode                                                               */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects FIU muxing . Notice it does not disable a CS!                     */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS CHIP_MuxFIU (   FIU_MODULE_T      devNum,
                            BOOLEAN           cs0Enable,
                            BOOLEAN           cs1Enable,
                            BOOLEAN           cs2Enable,
                            BOOLEAN           cs3Enable,
                            BOOLEAN           quadMode)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* CS2 and CS3 are multiplexed with spid2 and spid3. So when in quad mode, can't use CS2 and CS3       */
    /*-----------------------------------------------------------------------------------------------------*/

    //HAL_PRINT("FIU%d_Mux cs0 %d, cs1 %d, cs2 %d, cs3 %d, Quad %d\n",
    //         devNum, cs0Enable, cs1Enable, cs2Enable, cs3Enable, quadMode);

    switch (devNum)
    {
        case FIU_MODULE_0:

            /* config CS */
            if (cs0Enable == TRUE)
            {
                // nothing to do.
            }

            /* config CS1 */
            if (cs1Enable == TRUE)
            {
                SET_REG_FIELD(MFSEL1, MFSEL1_S0CS1SEL, 1);
                REG_WRITE(GPnOSRC(1), 1);
            }

            /* select io bus width (1/2/4  <=> single/dual/quad ) */
            if (quadMode == TRUE)
            {
                if (cs0Enable == TRUE)
                {
                    SPI_Quad_Enable(FIU_MODULE_0,  FIU_CS_0);
                }

                if (cs1Enable == TRUE)
                {
                    SPI_Quad_Enable(FIU_MODULE_0,  FIU_CS_1);
                }
            }

            break;

        case FIU_MODULE_1:

            /* select SPI1 */
            SET_REG_FIELD(MFSEL3, MFSEL3_SPI1SEL, 1);

            SET_REG_FIELD(MFSEL3, MFSEL3_FIN1916SEL, 0);

            SET_REG_FIELD(MFSEL7, MFSEL7_SMB15SELB, 0);

            SET_REG_FIELD(MFSEL6, MFSEL6_FM1SEL, 0);

            /* config CS */
            if (cs0Enable == TRUE)
            {
                 SET_REG_BIT(GPnOSRC(6), 11); // fast slew rate GPIO203
            }

            /* config CS1 */
            if (cs1Enable == TRUE)
            {
                SET_REG_FIELD(MFSEL5, MFSEL5_NSPI1CS1SEL, 1);
                SET_REG_BIT(GPnOSRC(7), 9); // fast slew rate GPIO233

            }

            /* config CS2 */
            if (cs2Enable == TRUE)
            {
                SET_REG_FIELD(MFSEL5, MFSEL5_NSPI1CS2SEL, 1);
                SET_REG_BIT(GPnOSRC(5), 31); // fast slew rate GPIO191
            }
            else
            {
                SET_REG_FIELD(MFSEL5, MFSEL5_NSPI1CS2SEL, 0);
            }

            /* config CS3 */
            if (cs3Enable == TRUE)
            {
                SET_REG_FIELD(MFSEL5, MFSEL5_NSPI1CS3SEL, 1);
                SET_REG_BIT(GPnOSRC(6), 0); // fast slew rate GPIO192
            }
            else
            {
                SET_REG_FIELD(MFSEL5, MFSEL5_NSPI1CS3SEL, 0);
            }

            if (quadMode == TRUE)
            {
                SET_REG_FIELD(MFSEL5, MFSEL5_NSPI1CS2SEL, 0);
                SET_REG_FIELD(MFSEL5, MFSEL5_NSPI1CS3SEL, 0);
                SET_REG_FIELD(MFSEL5, MFSEL5_SPI1D23SEL, 1);
                SET_REG_FIELD(MFSEL6, MFSEL6_FM1SEL, 0);
                SET_REG_FIELD(MFSEL7, MFSEL7_SMB15SELB, 0);
            }

            break;

        // why do they call the 3rd FIU FIU3? WTF?
        // Even my 3yo kid knows how to count better than that.
        // So to make it easier, I hereby call FIU3 FIU2.
        case FIU_MODULE_2:
        case FIU_MODULE_3:

            /* select SPI3 */
            SET_REG_FIELD(MFSEL4, MFSEL4_SP3SEL, 1);

            SET_REG_FIELD(INTCR4, INTCR4_FIU_FIX3, 1);
            SET_REG_FIELD(MFSEL6, MFSEL6_GPIO1836SEL, 0);

            /* config CS */
            if (cs0Enable == TRUE)
            {
                // nothing to do.
            }

            /* config CS1 */
            if (cs1Enable == TRUE)
            {
                SET_REG_FIELD(MFSEL4, MFSEL4_S3CS1SEL, 1);
                SET_REG_FIELD(MFSEL7, MFSEL7_GPIO187SEL, 0);
            }

            /* config CS2 */
            if (cs2Enable == TRUE)
            {
                SET_REG_FIELD(MFSEL4, MFSEL4_S3CS2SEL, 1);
                SET_REG_FIELD(MFSEL4, MFSEL4_SP3QSEL, 0);
                SET_REG_FIELD(MFSEL7, MFSEL7_GPIO1889SEL, 0);
            }

            /* config CS3 */
            if (cs3Enable == TRUE)
            {
                SET_REG_FIELD(MFSEL4, MFSEL4_S3CS3SEL, 1);
                SET_REG_FIELD(MFSEL4, MFSEL4_SP3QSEL, 0);
                SET_REG_FIELD(MFSEL7, MFSEL7_GPIO1889SEL, 0);
            }

            /* select io bus width (1/2/4  <=> single/dual/quad ) */
            if (quadMode == TRUE)
            {
                SET_REG_FIELD (MFSEL4, MFSEL4_S3CS2SEL, 0);  /*  0: GPIO33/SPI0D2 ,1: nSPI0CS2     */
                SET_REG_FIELD (MFSEL4, MFSEL4_S3CS3SEL, 0);  /*  0: GPIO34/SPI0D3 ,1: nSPI0CS3     */
                SET_REG_FIELD (MFSEL4, MFSEL4_SP3QSEL , 1);  /*  enable quad */

                if (cs0Enable == TRUE)
                {
                    SPI_Quad_Enable(FIU_MODULE_3,  FIU_CS_0);
                }

                if (cs1Enable == TRUE)
                {
                    SPI_Quad_Enable(FIU_MODULE_3,  FIU_CS_1);
                }
            }
            else
            {
                SET_REG_FIELD (MFSEL4, MFSEL4_SP3QSEL , 0);  /*  disable quad */
            }

            break;

        case FIU_MODULE_X:

            /* config CS */
            if (cs0Enable == TRUE)
            {
                // nothing to do.
            }

            /* config CS1 */
            if (cs1Enable == TRUE)
            {
                SET_REG_FIELD(MFSEL4, MFSEL4_SXCS1SEL, 1);
            }

            SET_REG_FIELD(MFSEL6, MFSEL6_FM2SEL, 0);

            SET_REG_FIELD(MFSEL4, MFSEL4_SPXSEL, 1);

            break;

        default:
            ASSERT(0);
            break;
    }

    if (devNum != FIU_MODULE_X)
    {
        FIU_SetDirection(devNum, FIU_DIRECTION_MODE_INCREASING);
    }
    else
    {
        FIU_SetDirection(devNum, FIU_DIRECTION_MODE_SINGLES);
    }

    return DEFS_STATUS_OK;
}
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_Mux_RMII                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  devNum -                                                                               */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects RMII mux                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_Mux_RMII(UINT devNum)
{
    if(devNum == 0)
    {
        //SET_REG_FIELD(CLKEN3, CLKEN3_GDMA1, 1);
        SET_REG_FIELD(MFSEL3, MFSEL3_RMII1SEL, 1);
        SET_REG_FIELD(MFSEL1, MFSEL1_R1MDSEL, 1);
        SET_REG_FIELD(MFSEL1, MFSEL1_R1ERRSEL, 1);
        SET_REG_FIELD(INTCR,  INTCR_R1EN, 1);

        SET_REG_FIELD(MFSEL4, MFSEL4_SG1MSEL, 1);
        // TODO: SET_REG_FIELD(MFSEL4, MFSEL4_RG1SEL, 1);
    }
    else
    {
        //SET_REG_FIELD(CLKEN3, CLKEN3_GDMA2, 1);
        SET_REG_FIELD(MFSEL1, MFSEL1_RMII2SEL, 1);
        SET_REG_FIELD(MFSEL1, MFSEL1_R2MDSEL, 1);
        SET_REG_FIELD(MFSEL1, MFSEL1_R2ERRSEL, 1);

        SET_REG_FIELD(MFSEL4, MFSEL4_RG2MSEL, 1);
        SET_REG_FIELD(MFSEL4, MFSEL4_RG2SEL, 1);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_SetDacSense                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Makes the trigger for sense circuits to be GFX core output signal (miscOUT2)           */
/*                  instead of VSYNC falling edge                                                          */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_SetDacSense(void)
{
    SET_REG_FIELD(INTCR, INTCR_DAC_SNS, 1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_Mux_GMII                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  devNum -                                                                               */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects GMII mux                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_Mux_GMII (UINT devNum)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* Setting the GMII Muxing                                                                             */
    /*-----------------------------------------------------------------------------------------------------*/
    switch (devNum)
    {
        case 1:
            SET_REG_FIELD(MFSEL3, MFSEL3_RMII1SEL, 1);
            break;

        case 2:
            SET_REG_FIELD(MFSEL1, MFSEL1_RMII2SEL, 1);
            break;

        case 3:
            SET_REG_FIELD(MFSEL5, MFSEL5_RMII3SEL, 1);
            break;
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* Setting MII TX clock mux                                                                            */
    /*-----------------------------------------------------------------------------------------------------*/
    //SET_REG_FIELD(MFSEL3, MFSEL3_MTCKSEL, 1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_Mux_SIOXH                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  enable -                                                                               */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects SIOXH mux                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_Mux_SIOXH(BOOLEAN enable)
{
    if (enable)
    {
        SET_REG_FIELD(MFSEL3, MFSEL3_IOXHSEL, 1);
    }
    else
    {
        SET_REG_FIELD(MFSEL3, MFSEL3_IOXHSEL, 0);
    }
}

#if defined (SPI_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_MuxSPI                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  devNum - PSPI device number                                                            */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs muxing for Peripheral SPI device                                 */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_MuxSPI(SPI_MODULE_T devNum)
{
    switch (devNum)
    {
        case SPI_MODULE_0:     SET_REG_FIELD(MFSEL3, MFSEL3_SPI1SEL, 2);          break;
        case SPI_MODULE_1:     SET_REG_FIELD(MFSEL3, MFSEL3_PSPI2SEL, 1);          break;
        default :              ASSERT(0);                                          break;
    }
}
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_Mux_GSPI                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs muxing for Graphics Core SPI Signals                             */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_Mux_GSPI(BOOLEAN bOn)
{
    if ( bOn == TRUE)
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_GSPISEL, 1);  // Config to Graphics Core SPI
    }
    else
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_GSPISEL, 0);  // Config to GPIO
    }
}

#ifdef MC_MODULE_TYPE
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_PowerOn_GetMemorySize                                                             */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine returns configured DDR memory size                                        */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CHIP_PowerOn_GetMemorySize(void)
{
    UINT32 pwronRAMsize = _512MB_;
#if 0

    STRP_MemSize_Get();


    switch(pwronRAMsize)
    {
    case STRP_MEMSIZ_512MB_MODE:      // 512 MiB
        return 0x20000000;
    case STRP_MEMSIZ_1GB_MODE:        // 1 GiB.
        return 0x40000000;
    case STRP_MEMSIZ_2GB_MODE:        // 2 GiB.
        return 0x80000000;
    case STRP_MEMSIZ_RESERVED:        // 512 MiB. // use minimum
        return 0x20000000;
    }
#endif

    return 0;
}
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_Mux_GPIO                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  gpio_num -                                                                             */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs mux for given GPIO                                               */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_Mux_GPIO(UINT gpio_num)
{
    if ( gpio_num == 169)
    {
        SET_REG_FIELD(MFSEL3, MFSEL3_SCISEL, 0);
    }
    if ( gpio_num == 162)
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_SIRQSEL, 1);
    }
    if ( gpio_num == 168)
    {
        SET_REG_FIELD(MFSEL3, MFSEL3_CLKRUNSEL, 1); // Selects GPIO168 not LPC nCLKRUN. When this bit is 1, MFSEL4.8 = 0
        SET_REG_FIELD(MFSEL4, MFSEL4_ESPISEL, 0);
    }

    if ( gpio_num == 163)
    {
        SET_REG_FIELD(MFSEL3, MFSEL3_CLKRUNSEL, 1);
        SET_REG_FIELD(MFSEL4, MFSEL4_ESPISEL, 0);
    }

    if ( (gpio_num == 161)  ||
        ((gpio_num >= 164) && (gpio_num <= 167)) ||
         (gpio_num == 166) ||
        (gpio_num == 95))
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_LPCSEL, 1);
        SET_REG_FIELD(MFSEL4, MFSEL4_ESPISEL, 0);
    }

    if (gpio_num == 190)
    {
        SET_REG_FIELD(FLOCKR1, FLOCKR1_PSMISEL, 1);
    }
}

#if defined (FLM_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_MuxFLM                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  flm_module - FLM Module number                                                         */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects the FLM function to the corresponding pin.                        */
/*                                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS            CHIP_MuxFLM(FLM_MODULE_T flm_module, BOOLEAN quad_en)
{
        switch (flm_module)
        {
            case FLM_MODULE_0:
                SET_REG_FIELD(MFSEL6, MFSEL6_FM0SEL, 1);

                SET_REG_FIELD(I2CSEGSEL , I2CSEGSEL_S0SBSEL, 0);
                SET_REG_FIELD(I2CSEGSEL , I2CSEGSEL_S0SCSEL, 0);
                SET_REG_FIELD(I2CSEGSEL , I2CSEGSEL_S0SDSEL, 0);
                SET_REG_FIELD(I2CSEGSEL , I2CSEGSEL_S0DESEL, 0);
            break;
            case FLM_MODULE_1:
                SET_REG_FIELD(MFSEL6, MFSEL6_FM1SEL, 1);

                SET_REG_FIELD(MFSEL5, MFSEL5_NSPI1CS1SEL, 0);
                SET_REG_FIELD(MFSEL5, MFSEL5_SPI1D23SEL, 0);
                SET_REG_FIELD(MFSEL5, MFSEL5_NSPI1CS2SEL, 0);
                SET_REG_FIELD(MFSEL5, MFSEL5_NSPI1CS3SEL, 0);

                SET_REG_FIELD(MFSEL3, MFSEL3_SPI1SEL, 1);
                SET_REG_FIELD(MFSEL3, MFSEL3_FIN1916SEL, 0);

                SET_REG_FIELD(MFSEL7, MFSEL7_SMB15SELB, 0);
            break;
            case FLM_MODULE_2:
                SET_REG_FIELD(MFSEL6, MFSEL6_FM2SEL, 1);
                SET_REG_FIELD(MFSEL4, MFSEL4_SXCS1SEL, 0);
                SET_REG_FIELD(MFSEL4, MFSEL4_SPXSEL, 0);

            break;
            case FLM_MODULE_3:
                SET_REG_FIELD(MFSEL6, MFSEL6_GPIO1836SEL, 0);
                // De sellecte SPI3
                 SET_REG_FIELD(MFSEL4, MFSEL4_SP3SEL, 0);
                 SET_REG_FIELD(MFSEL4, MFSEL4_S3CS1SEL, 0);
                 SET_REG_FIELD(MFSEL4, MFSEL4_S3CS2SEL, 0);
                 SET_REG_FIELD(MFSEL4, MFSEL4_S3CS3SEL, 0);
                 if (quad_en == TRUE)
                 {
                    SET_REG_FIELD(MFSEL4, MFSEL4_SP3QSEL, 0);
                 }
            break;
            default:
            break;
        }
     return DEFS_STATUS_OK;
}
#endif

#if defined (JTM_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_MuxJTM                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  jtm_module - JTM Module number                                                         */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects the JTM function to the corresponding pin.                        */
/*                                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS            CHIP_MuxJTM(JTM_MODULE_T jtm_module)
{
    // Reset Module and enable clocks
    if(jtm_module == 0)
    {
         SET_REG_FIELD(CLKEN4, CLKEN4_JTM1, 1);
         SET_REG_FIELD(MFSEL5, MFSEL5_JM1SEL, 1);
    }
    else  if(jtm_module == 1)
    {
        SET_REG_FIELD(CLKEN4, CLKEN4_JTM2, 1);
        SET_REG_FIELD(MFSEL5, MFSEL5_JM2SEL, 1);
    }
    else
    {
        return DEFS_STATUS_INVALID_PARAMETER;
    }
     return DEFS_STATUS_OK;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_ResetJTM                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  jtm_module - JTM Module number                                                         */
/*                    state     TRUE - reset, FALSE - enable                                               */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine clears of asserts the reset of the JTM Module.                            */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS            CHIP_ResetJTM(JTM_MODULE_T jtm_module, BOOLEAN state)
{
    UINT32 val;

    // Reset Module and enable clocks
    if(jtm_module == 0)
    {
        if (state == TRUE)
        {
            CLEAR_REG_BIT(IPSRST4, IPSRST4_JTM1);
        }
        else
        {
            SET_REG_BIT(IPSRST4, IPSRST4_JTM1);
        }
    }
    else   if(jtm_module == 1)
    {
       if (state == TRUE)
        {
            CLEAR_REG_BIT(IPSRST4, IPSRST4_JTM2);
        }
        else
        {
            SET_REG_BIT(IPSRST4, IPSRST4_JTM2);
        }
    }
    else
    {
        return DEFS_STATUS_INVALID_PARAMETER;
    }

    return DEFS_STATUS_OK;
}
#endif

#if defined (SD_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_Mux_SD                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                 sdNum    - [in], SD device number                                                       */
/*                 devType  - [in], SD1 device type (SD or MMC)                                            */
/*                                                                                                         */
/* Returns:         SWC HAL Error code                                                                     */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects SD mux                                                            */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS CHIP_Mux_SD(SD_DEV_NUM_T sdNum, SD_DEV_TYPE_T sdType)
{
    if (sdNum >= SD_NUM_OF_MODULES)
    {
        return DEFS_STATUS_PARAMETER_OUT_OF_RANGE;
    }

    if (sdNum == SD1_DEV)
    {
        SET_REG_FIELD(MFSEL3, MFSEL3_SD1SEL, 1);
    }
    else if (sdNum == SD2_DEV)
    {
        SET_REG_FIELD(MFSEL3, MFSEL3_MMCSEL, 1);
    }

    return DEFS_STATUS_OK;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_EmmcBootTimeConfig                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  clk - Card clock frequency (in Hz units)                                               */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine calculates and configures the number of SDCLK clock cycles:               */
/*                   1) In a 50 millisecond time.                                                          */
/*                   1) In a 1 second time.                                                                */
/*                  These values are being used to determine whether Boot Time-Out has occured.            */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_EmmcBootTimeConfig (UINT32 clk)
{
    // TODO: UINT32 cycles = 0;

    /* Configure the number of SDCLK clock cycles in 50ms */
    // TODO: cycles = (clk/_1SEC_IN_MSEC_) * 50;

    // TODO: SET_REG_FIELD(EMMCBTCNT, EMMCBTCNT_TIME50MSEC, cycles);

    /* Configure the number of SDCLK clock cycles in 1s -
       Note that card clock frequency is actually the number of SDCLK clock cycles in 1 second */
    // TODO: cycles = clk;
    // TODO: REG_WRITE(EMMCBTT, cycles);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_EmmcBootStart                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ack - TRUE means expect the boot acknowledge data, FALSE otherwise.                    */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine initiates a EMMC boot sequence.                                           */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_EmmcBootStart (BOOLEAN ack)
{
    /* Start boot */
    // TODO: SET_REG_FIELD(EMMCBTCNT, EMMCBTCNT_BTACK, ack);
    // TODO: SET_REG_FIELD(EMMCBTCNT, EMMCBTCNT_BOOTREQ, 1);    //Also enables the clock to the card
    // TODO: REG_WRITE(EMMCBTST, MASK_FIELD(EMMCBTST_BOOTERRORS));  //W1C for bits 1-3, bit 0 is RO.

    /* Wait untill boot is acknowledged */
    // TODO: while (!READ_REG_FIELD(EMMCBTST, EMMCBTST_BOOTRUN));
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_EmmcBootInvalid                                                                   */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         TRUE if boot is invalid, FALSE otherwise                                               */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine checks whether the current boot process is invalid,                       */
/*                  i.e., one of the following has occured:                                                */
/*                   1) The device did not respond in time with the boot acknowledge sequence or data.     */
/*                   2) An error in data structure occurred.                                               */
/*                   3) A CRC error occurred when reading from the eMMC device.                            */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_EmmcBootInvalid (void)
{

    // TODO: return (READ_REG_FIELD(EMMCBTST, EMMCBTST_BOOTTO) ||
    // TODO:         READ_REG_FIELD(EMMCBTST, EMMCBTST_BOOTERR) ||
    // TODO:         READ_REG_FIELD(EMMCBTST, EMMCBTST_BOOTCRC));
    return FALSE;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_EmmcBootEnd                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine ends a EMMC boot sequence, either when the boot sequence is done,         */
/*                  or an error is discovered.                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_EmmcBootEnd (void)
{
    // TODO: SET_REG_FIELD(EMMCBTCNT, EMMCBTCNT_BOOTREQ, 0);
}
#endif //  ( SD_MODULE_TYPE)

#if defined (VCD_MODULE_TYPE) // TODO: need to implement
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_SetVCDInput                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  headNum - Number of the head (1 or 2)                                                  */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects the intput for the VCD                                            */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_SetVCDInput (/* TODO: CHIP_VCD_HEAD_T headNum */ void)
{
#if 0
    if (headNum == GCR_VCD_HEAD1)
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_DVH1SEL, 1);
    }
    else if (headNum == GCR_VCD_HEAD2)
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_DVH1SEL, 0);
    }
#endif
    ASSERT(0);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_SetGFXInput                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  config - GFX configuration                                                             */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects the intput for the VCD                                            */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_SetGFXConfig (/* TODO: VCD_CONFIG_T config */ void)
{
#if 0
    if      (config == GCR_GFX_CONFIG_OUTPUT_FROM_HEAD1)
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_DVH1SEL, MFSEL1_DVOSEL_OUTPUT_HEAD1);
    }
    else if (config == GCR_GFX_CONFIG_OUTPUT_FROM_HEAD2)
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_DVH1SEL, MFSEL1_DVOSEL_OUTPUT_HEAD2);
    }
    else if (config == GCR_GFX_CONFIG_INPUT_TO_KVM)
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_DVH1SEL, MFSEL1_DVOSEL_INPUT_KVM);
    }
#endif
    ASSERT(0);
}
#endif // VCD_MODULE_TYPE

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_ConfigUSBBurstSize                                                                */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Configures the USB burst size in USB test register                                     */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_ConfigUSBBurstSize (void)
{
    // SET_REG_FIELD(USBTEST, USBTEST_BURSTSIZE, 3);   // Burst configured to 8 + 4
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_ResetTypeIsPor                                                                    */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Returns true if reset source was POR (powerOn) reset                                   */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_ResetTypeIsPor (void)
{
    return POR_STATUS();
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_ResetTypeIsCor                                                                    */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Returns TRUE if and only if a Core reset is performed                                  */
/*                  and FALSE on any other reset type.                                                     */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_ResetTypeIsCor (void)
{
    return (COR_STATUS() && !POR_STATUS());
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_MarkCfgDone                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Sets a flag in scratch register specifying that CLK/DDR configuration were performed   */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_MarkCfgDone (void)
{
    MARK_CFG_DONE();
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_CfgWasDone                                                                        */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Returns true if a flag in scratch register indicates                                   */
/*                  that CLK/DDR configuration was already done                                            */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_CfgWasDone (void)
{
    return CFG_IS_DONE();
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_EnableJtag                                                                        */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Enable Jtag access                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_EnableJtag (void)
{
    ENABLE_JTAG() ;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_DisableJtag                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Disable Jtag access                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_DisableJtag (void)
{
    DISABLE_JTAG();
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_DisableModules                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  mdlr -  Value to write to MDLR register                                                */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Writes to the MDLR register which determines which modules to disable                  */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_DisableModules (UINT32 mdlr)
{
    REG_WRITE(MDLR, mdlr);
}

#if defined (PECI_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_MuxPECI                                                                           */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         TRUE if PECI is enabled; FALSE otherwise.                                              */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine enables the PECI signal for the PECI module (only in PECI bonding option, */
/*                  which is indicated by PECIST internal strap being set to 1).                           */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_MuxPECI (void)
{
    // TBD
    return TRUE;
}
#endif // PECI_MODULE_TYPE

#if defined (ADC_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_MuxADC                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  adc_module - The ADC module to be selected.                                            */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects the ADCx function to the corresponding pin.                       */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_MuxADC (ADC_INPUT_T adc_module)
{
    ASSERT(adc_module < ADC_NUM_OF_INPUTS);

    /// SET_REG_BIT(DEVALT6, adc_module);
}
#endif

#if defined (PWM_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_MuxPWM                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  pwm_module - The PWM module to be selected.                                            */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects the PWMx function to the corresponding pin.                       */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_MuxPWM (PWM_MODULE_T pwm_module)
{
    switch(pwm_module)
    {
    case PWM_MODULE_0:      SET_REG_FIELD(MFSEL2, MFSEL2_PWM0SEL, 1);  break;
    case PWM_MODULE_1:      SET_REG_FIELD(MFSEL2, MFSEL2_PWM1SEL, 1);  break;
    case PWM_MODULE_2:      SET_REG_FIELD(MFSEL2, MFSEL2_PWM2SEL, 1);  break;
    case PWM_MODULE_3:      SET_REG_FIELD(MFSEL2, MFSEL2_PWM3SEL, 1);  break;
    case PWM_MODULE_4:      SET_REG_FIELD(MFSEL2, MFSEL2_PWM4SEL, 1);  break;
    case PWM_MODULE_5:      SET_REG_FIELD(MFSEL2, MFSEL2_PWM5SEL, 1);  break;
    case PWM_MODULE_6:      SET_REG_FIELD(MFSEL2, MFSEL2_PWM6SEL, 1);  break;
    case PWM_MODULE_7:      SET_REG_FIELD(MFSEL2, MFSEL2_PWM7SEL, 1);  break;
    default:                ASSERT(0); break;
   }

}
#endif

#if defined (MFT_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_MuxMFT                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  mft_module - The MFT module to be selected.                                            */
/*                  mft_timer -  The MFT timer to be selected.                                             */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects the TXn function to the corresponding pin, where:                 */
/*                  · 'X' is the MFT timer number  (A/B).                                                  */
/*                  · 'n' is the MFT module number (1/2/etc').                                             */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_MuxMFT (MFT_MODULE_T mft_module, MFT_TIMER_T mft_timer)
{
    //SET_REG_BIT(MFSEL, (2*mft_module + mft_timer));
}
#endif

#if defined (SMB_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_SMBPullUp                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  module_num - SMB module number.                                                        */
/*                  enable     - TRUE to configre as Pull-Up; FALSE to configre as Pull-Down.              */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures pull-up/down for SMB interface pins.                           */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_SMBPullUp (SMB_MODULE_T module_num, BOOLEAN enable)
{
    ASSERT(module_num < SMB_NUM_OF_MODULES);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Setting Pull Up/Down configuration                                                                  */
    /*-----------------------------------------------------------------------------------------------------*/
    //if (enable)
    //{
    //    SET_REG_BIT(DEVPU0, module_num);
    //}

    //else
    //{
    //    CLEAR_REG_BIT(DEVPU0, module_num);
    //}
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_MuxSMB                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  smb_module - SMB Module number                                                         */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects the SMBx function to the corresponding pin.                       */
/*                  NOTE: This function does not set the segment! Use CHIP_Mux_Segment in addition         */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_MuxSMB (SMB_MODULE_T smb_module, BOOLEAN bEnable)
{
    switch (smb_module)
    {
    case 0:
    if (READ_REG_FIELD(MFSEL1, MFSEL1_SMB0SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_SMB0SEL, bEnable);
    }
    SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S0DECFG , 0x0);  // Smbus 0 Drive enabled: set it to float.
    SET_REG_FIELD(I2CSEGCTL , I2CSEGCTL_S0D_WE_EN, 0x3) ; // Smbus 0 Drive enabled: set it to float.
    break;

    case 1:
    if (READ_REG_FIELD(MFSEL1, MFSEL1_SMB1SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_SMB1SEL, bEnable);
    }
    break;

    case 2:
    if (READ_REG_FIELD(MFSEL1, MFSEL1_SMB2SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_SMB2SEL, bEnable);
    }
    break;

    case 3:
    if (READ_REG_FIELD(MFSEL1, MFSEL1_SMB3SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_SMB3SEL, bEnable);
    }
    break;

    case 4:
    if (READ_REG_FIELD(MFSEL1, MFSEL1_SMB4SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_SMB4SEL, bEnable);
    }
    SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S4DECFG , 0x0);  // Smbus 4 Drive enabled: set it to float.
    SET_REG_FIELD(I2CSEGCTL , I2CSEGCTL_S4D_WE_EN, 0x3) ; // Smbus 4 Drive enabled: set it to float.
    break;

    case 5:
    if (READ_REG_FIELD(MFSEL1, MFSEL1_SMB5SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL1, MFSEL1_SMB5SEL, bEnable);
    }
    break;

    case 6:
    if (READ_REG_FIELD(MFSEL3, MFSEL3_SMB6SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL3, MFSEL3_SMB6SEL, bEnable);
    }
    break;

    case 7:
    if (READ_REG_FIELD(MFSEL3, MFSEL3_SMB7SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL3, MFSEL3_SMB7SEL, bEnable);
    }
    break;

    case 8:
    case 24:
    if (READ_REG_FIELD(MFSEL4, MFSEL4_SMB8SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL4, MFSEL4_SMB8SEL, bEnable);
    }
    break;

    case 9:
    case 25:
    if (READ_REG_FIELD(MFSEL4, MFSEL4_SMB9SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL4, MFSEL4_SMB9SEL, bEnable);
    }
    break;

    case 10:
    case 26:
    if (READ_REG_FIELD(MFSEL4, MFSEL4_SMB10SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL4, MFSEL4_SMB10SEL, bEnable);
    }
    break;

    case 11:
    if (READ_REG_FIELD(MFSEL4, MFSEL4_SMB11SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL4, MFSEL4_SMB11SEL, bEnable);
    }
    break;

    case 12:
    if (READ_REG_FIELD(MFSEL3, MFSEL3_SMB12SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL3, MFSEL3_SMB12SEL, bEnable);
    }
    break;

    case 13:
    if (READ_REG_FIELD(MFSEL3, MFSEL3_SMB13SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL3, MFSEL3_SMB13SEL, bEnable);
    }
    break;

    case 14:
    if (READ_REG_FIELD(MFSEL3, MFSEL3_SMB14SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL3, MFSEL3_SMB14SEL, bEnable);
    }
    break;

    case 15:
    if (READ_REG_FIELD(MFSEL3, MFSEL3_SMB15SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL3, MFSEL3_SMB15SEL, bEnable);
    }
    break;

    case 16:
    if (READ_REG_FIELD(MFSEL5, MFSEL5_SMB16SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL5, MFSEL5_SMB16SEL, bEnable);
    }
    break;

    case 17:
    if (READ_REG_FIELD(MFSEL5, MFSEL5_SMB17SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL5, MFSEL5_SMB17SEL, bEnable);
    }
    break;

    case 18:
    if (READ_REG_FIELD(MFSEL5, MFSEL5_SMB18SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL5, MFSEL5_SMB18SEL, bEnable);
    }
    break;

    case 19:
    if (READ_REG_FIELD(MFSEL5, MFSEL5_SMB19SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL5, MFSEL5_SMB19SEL, bEnable);
    }
    break;

    case 20:
    if (READ_REG_FIELD(MFSEL5, MFSEL5_SMB20SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL5, MFSEL5_SMB20SEL, bEnable);
    }
    break;

    case 21:
    if (READ_REG_FIELD(MFSEL5, MFSEL5_SMB21SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL5, MFSEL5_SMB21SEL, bEnable);
    }
    break;

    case 22:
    if (READ_REG_FIELD(MFSEL5, MFSEL5_SMB22SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL5, MFSEL5_SMB22SEL, bEnable);
    }
    break;

    case 23:
    if (READ_REG_FIELD(MFSEL5, MFSEL5_SMB23SEL) == !bEnable)
    {
        SET_REG_FIELD(MFSEL5, MFSEL5_SMB23SEL, bEnable);
    }
    break;

    default:
    ASSERT(FALSE);
    break;
    }

    CHIP_SMBPullUp(smb_module, bEnable);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_Mux_Segment                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  smb_module - SMB Module number                                                         */
/*                  segment - 0 => A, 1 => B , 2 => C, 3 => D                                              */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects the SMBx segment to use.                                          */
/*                  It does not mux the SMB! use CHIP_MuxSMB in addition                                   */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_Mux_Segment (SMB_MODULE_T smb_module, UINT32 segment, BOOLEAN bEnable)
{
    if ((smb_module > (SMB_MODULE_T)5) || (segment > 3))
    {
        return;
    }

    switch (smb_module)
    {
    case 0:
        {
            REG_WRITE(I2CSEGCTL, (REG_READ(I2CSEGCTL) & 0xFFFC0000) | BUILD_FIELD_VAL(I2CSEGCTL_SMB0SS, segment) | BUILD_FIELD_VAL(I2CSEGCTL_WEN0SS, 1));
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S0SBSEL, 0);
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S0SCSEL, 0);
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S0SDSEL, 0);
            if (bEnable == TRUE)
            {
                switch(segment)
                {
                    case 1:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S0SBSEL, bEnable);
                        break;

                    case 2:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S0SCSEL, bEnable);
                        break;

                    case 3:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S0SDSEL, bEnable);
                        break;

                    case 0:
                    default:

                        break;
                }
            }
            break;
        }

        case 1:
        {
            REG_WRITE(I2CSEGCTL, (REG_READ(I2CSEGCTL) & 0xFFFC0000) | BUILD_FIELD_VAL(I2CSEGCTL_SMB1SS,segment) | BUILD_FIELD_VAL(I2CSEGCTL_WEN1SS,1));
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S1SBSEL, 0);
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S1SCSEL, 0);
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S1SDSEL, 0);
            if (bEnable == TRUE)
            {
                switch(segment)
                {
                    case 1:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S1SBSEL, bEnable);
                        break;

                    case 2:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S1SCSEL, bEnable);
                        break;

                    case 3:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S1SDSEL, bEnable);
                        break;

                    case 0:
                    default:

                        break;
                }
            }
            break;
        }

         case 2:
         {
            REG_WRITE(I2CSEGCTL, (REG_READ(I2CSEGCTL) & 0xFFFC0000) | BUILD_FIELD_VAL(I2CSEGCTL_SMB2SS, segment) | BUILD_FIELD_VAL(I2CSEGCTL_WEN2SS, 1));
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S2SBSEL, 0);
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S2SCSEL, 0);
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S2SDSEL, 0);
            if (bEnable == TRUE)
            {
                switch(segment)
                {
                    case 1:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S2SBSEL, bEnable);
                        break;

                    case 2:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S2SCSEL, bEnable);
                        break;

                    case 3:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S2SDSEL, bEnable);
                        break;

                    case 0:
                    default:

                        break;
                }
            }
            break;
        }

        case 3:
        {
            REG_WRITE(I2CSEGCTL, (REG_READ(I2CSEGCTL) & 0xFFFC0000) | BUILD_FIELD_VAL(I2CSEGCTL_SMB3SS, segment) | BUILD_FIELD_VAL(I2CSEGCTL_WEN3SS, 1));
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S3SBSEL, 0);
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S3SCSEL, 0);
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S3SDSEL, 0);
            if (bEnable == TRUE)
            {
                switch(segment)
                {
                    case 1:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S3SBSEL, bEnable);
                        break;

                    case 2:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S3SCSEL, bEnable);
                        break;

                    case 3:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S3SDSEL, bEnable);
                        break;

                    case 0:
                    default:

                        break;
                }
            }
            break;
        }

        case 4:
        {
            REG_WRITE(I2CSEGCTL, (REG_READ(I2CSEGCTL) & 0xFFFC0000) | BUILD_FIELD_VAL(I2CSEGCTL_SMB4SS, segment) | BUILD_FIELD_VAL(I2CSEGCTL_WEN4SS, 1));
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S4SBSEL, 0);
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S4SCSEL, 0);
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S4SDSEL, 0);
            if (bEnable == TRUE)
            {
                switch(segment)
                {
                    case 1:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S4SBSEL, bEnable);
                        break;

                    case 2:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S4SCSEL, bEnable);
                        break;

                    case 3:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S4SDSEL, bEnable);
                        break;

                    case 0:
                    default:

                        break;
                }
            }
            break;
       }

       case 5:
       {
            REG_WRITE(I2CSEGCTL, (REG_READ(I2CSEGCTL) & 0xFFFC0000) | BUILD_FIELD_VAL(I2CSEGCTL_SMB5SS, segment) | BUILD_FIELD_VAL(I2CSEGCTL_WEN5SS, 1));
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S5SBSEL, 0);
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S5SCSEL, 0);
            SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S5SDSEL, 0);
            if (bEnable == TRUE)
            {
                switch(segment)
                {
                    case 1:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S5SBSEL, bEnable);
                        break;

                    case 2:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S5SCSEL, bEnable);
                        break;

                    case 3:
                        SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S5SDSEL, bEnable);
                        break;

                    case 0:
                    default:

                        break;
                }
            }
            break;
        }

        case 6:
        {
             REG_WRITE(I2CSEGCTL, (REG_READ(I2CSEGCTL) & 0xFFFC0000) | BUILD_FIELD_VAL(I2CSEGCTL_SMB6SS, segment) | BUILD_FIELD_VAL(I2CSEGCTL_WEN5SS, 1));
             SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S6SBSEL, 0);
             SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S6SCSEL, 0);
             SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S6SDSEL, 0);

             if (bEnable == TRUE)
             {
                 switch(segment)
                 {
                     case 1:
                         SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S6SBSEL, bEnable);
                         break;

                     case 2:
                         SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S6SCSEL, bEnable);
                         break;

                     case 3:
                         SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S6SDSEL, bEnable);
                         break;

                     case 0:
                     default:
                         break;
                 }
             }
             break;
        }

        case 7:
        {
             REG_WRITE(I2CSEGCTL, (REG_READ(I2CSEGCTL) & 0xFFFC0000) | BUILD_FIELD_VAL(I2CSEGCTL_SMB7SS, segment) | BUILD_FIELD_VAL(I2CSEGCTL_WEN5SS, 1));
             SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S7SBSEL, 0);
             SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S7SCSEL, 0);
             SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S7SDSEL, 0);
             if (bEnable == TRUE)
             {
                 switch(segment)
                 {
                     case 1:
                         SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S7SBSEL, bEnable);
                         break;

                     case 2:
                         SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S7SCSEL, bEnable);
                         break;

                     case 3:
                         SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S7SDSEL, bEnable);
                         break;

                     case 0:
                     default:
                         break;
                 }
             }
             break;

        }

        default:
            ASSERT(FALSE);
            break;
    }

    return;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_SMB_GetGpioNumbers                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  smb_module - SMB Module number                                                         */
/*                  segment - SMB segment number (0,1,2,3 means A,B,C,D pins)                              */
/*                  ret_scl_gpio - SCL to gpio pin number                                                  */
/*                  ret_sda_gpio - SDA to gpio pin number                                                  */
/*                                                                                                         */
/* Returns:         status                                                                                 */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine return the gpio number of SCL signal                                      */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS CHIP_SMB_GetGpioNumbers (SMB_MODULE_T smb_module, UINT segment, INT* ret_scl_gpio, INT* ret_sda_gpio)
{
    const INT SMB_SCL_GPIO[SMB_NUM_OF_MODULES][SMB_NUM_OF_SEGMENT] = {
    /*  SMB0   */ {114, 194, 196, 199},
    /*  SMB1   */ {116, 127, 125, 5},
    /*  SMB2   */ {118, 123, 121, 7},
    /*  SMB3   */ {31, 40, 38, 60},
    /*  SMB4   */ {29, 23, 21, 23},
    /*  SMB5   */ {27, 12, 14, 93},
    /*  SMB6   */ {171, 3, 1, 10},
    /*  SMB7   */ {173, 141  , 24, 142},
    /*  SMB8   */ {128,     -1, -1, -1},
    /*  SMB9   */ {130,     -1, -1, -1},
    /*  SMB10  */ {132,     -1, -1, -1},
    /*  SMB11  */ {134,     -1, -1, -1},
    /*  SMB12  */ {220,     -1, -1, -1},
    /*  SMB13  */ {222,     -1, -1, -1},
    /*  SMB14  */ { 23,     32, -1, -1},
    /*  SMB15  */ { 21,    192, -1, -1},
    /*  SMB16  */ { 10,    218, -1, -1},
    /*  SMB17  */ {  3,     -1, -1, -1},
    /*  SMB18  */ {  1,     -1, -1, -1},
    /*  SMB19  */ { 60,     -1, -1, -1},
    /*  SMB20  */ {234,     -1, -1, -1},
    /*  SMB21  */ {169,     -1, -1, -1},
    /*  SMB22  */ { 40,     -1, -1, -1},
    /*  SMB23  */ { 38,    134, -1, -1},
    /*  SMB24  */ {128,     -1, -1, -1},
    /*  SMB25  */ {130,     -1, -1, -1},
    /*  SMB26  */ {132,     -1, -1, -1}
    };





    const INT SMB_SDA_GPIO[SMB_NUM_OF_MODULES][SMB_NUM_OF_SEGMENT] = {
    /*  SMB0   */ {115, 195 ,202, 198},
    /*  SMB1   */ {117, 126 ,124 , 4},
    /*  SMB2   */ {119, 122 ,120, 6},
    /*  SMB3   */ {30, 396, 37, 59},
    /*  SMB4   */ {28, 22, 20,  22},
    /*  SMB5   */ {26, 13, 15, 94},
    /*  SMB6   */ {172, 2, 0, 11},
    /*  SMB7   */ {174, 16, 25,  143},
    /*  SMB8   */ {129,     -1, -1, -1},
    /*  SMB9   */ {131,     -1, -1, -1},
    /*  SMB10  */ {133,     -1, -1, -1},
    /*  SMB11  */ {135,     -1, -1, -1},
    /*  SMB12  */ {221,     -1, -1, -1},
    /*  SMB13  */ {223,     -1, -1, -1},
    /*  SMB14  */ { 22,    187, -1, -1},
    /*  SMB15  */ { 20,    191, -1, -1},
    /*  SMB16  */ { 11,    219, -1, -1},
    /*  SMB17  */ {  2,     -1, -1, -1},
    /*  SMB18  */ {  0,     -1, -1, -1},
    /*  SMB19  */ { 59,     -1, -1, -1},
    /*  SMB20  */ {235,     -1, -1, -1},
    /*  SMB21  */ {170,     -1, -1, -1},
    /*  SMB22  */ { 39,     -1, -1, -1},
    /*  SMB23  */ { 37,    135, -1, -1},
    /*  SMB24  */ {129,     -1, -1, -1},
    /*  SMB25  */ {131,     -1, -1, -1},
    /*  SMB26  */ {133,     -1, -1, -1}};


    INT ret_val = 0;
    if ((smb_module >= SMB_NUM_OF_MODULES) || (segment >= SMB_NUM_OF_SEGMENT))
        return DEFS_STATUS_INVALID_PARAMETER;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Find SCL GPIO number on the table                                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
    ret_val = SMB_SCL_GPIO[smb_module][segment];

    if (ret_val == -1)
        return DEFS_STATUS_INVALID_PARAMETER;

    *ret_scl_gpio = ret_val;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Find SDA GPIO number on the table                                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
    ret_val = SMB_SDA_GPIO[smb_module][segment];

    if (ret_val == -1)
        return DEFS_STATUS_INVALID_PARAMETER;

    *ret_sda_gpio = ret_val;

    return DEFS_STATUS_OK;
}
#endif

#if 0
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_MuxSMB2Segment                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  smb_module - SMB Module number                                                         */
/*                  segment - 0 => A, 1 => B , 2 => C, 3 => D                                              */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects the SMBx function to the to a segment mapping.                    */
/*                  This feature is applicable to SMB0 to SMB5 only                                        */
/* NOTE! This function should be performed by the MP side to mux channels and segemtns. The code is here   */
/*       for Reference only                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_MuxSMB2Segment (SMB_MODULE_T smb_module, UINT segment)
{
    switch (smb_module)
    {
    case 0:
    SET_REG_FIELD(MFSEL1, MFSEL1_SMB0SEL, TRUE);
    SET_REG_FIELD(MFSEL1, MFSEL1_DVOSEL, 0 );        // DVO is muxed with the SMB
    SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S0DECFG, 0x2);  // Smbus 0 Drive enabled: output signal that is driven by some targets.
    break;

    case 1:
        SET_REG_FIELD(MFSEL1, MFSEL1_SMB1SEL, TRUE);
        switch(segment)
        {
        case 1:
            SET_REG_FIELD(MFSEL1, MFSEL1_DVOSEL, 0 );
            break;

        case 2:
            SET_REG_FIELD(MFSEL1, MFSEL1_DVOSEL, 0 );
            break;

        case 3:
            SET_REG_FIELD(MFSEL3, MFSEL3_IOX2SEL, 0 );
            break;

        case 0:
        default:
            // SET_REG_FIELD(MFSEL1, MFSEL1_DVOSEL, 0 );
            break;
        }

        break;

     case 2:
    SET_REG_FIELD(MFSEL1, MFSEL1_SMB2SEL, TRUE);
    SET_REG_FIELD(MFSEL1, MFSEL1_DVOSEL, 0 );
    switch(segment)
    {
        case 1:
        SET_REG_FIELD(MFSEL1, MFSEL1_DVOSEL, 0 );
        break;

        case 2:
        SET_REG_FIELD(MFSEL1, MFSEL1_DVOSEL, 0 );
        break;

        case 3:
        SET_REG_FIELD(MFSEL3, MFSEL3_IOX2SEL, 0 );
        break;

        case 0:
        default:
        break;
    }

    break;

    case 3:
    SET_REG_FIELD(MFSEL1, MFSEL1_SMB3SEL, TRUE);
    SET_REG_FIELD(MFSEL1, MFSEL1_DVOSEL, 0 );
    switch(segment)
    {
        case 1:
        SET_REG_FIELD(MFSEL1, MFSEL1_DVOSEL, 0 );
        break;

        case 2:
        SET_REG_FIELD(MFSEL1, MFSEL1_DVOSEL, 0 );
        break;

        case 3:
        SET_REG_FIELD(MFSEL2, MFSEL2_HG6SEL, 0 );
        SET_REG_FIELD(MFSEL2, MFSEL2_HG7SEL, 0 );
        break;

        case 0:
        default:
        break;
    }
    break;

    case 4:
    SET_REG_FIELD(MFSEL1, MFSEL1_SMB4SEL, TRUE);
    switch(segment)
    {
        case 1:
        SET_REG_FIELD(MFSEL3, MFSEL3_PSPI2SEL, 0 );
        break;

        case 2:
        SET_REG_FIELD(MFSEL2, MFSEL2_HG0SEL, 0 );
        SET_REG_FIELD(MFSEL2, MFSEL2_HG1SEL, 0 );
        break;

        case 3:
        SET_REG_FIELD(MFSEL2, MFSEL2_HG2SEL, 0 );
        SET_REG_FIELD(MFSEL2, MFSEL2_HG3SEL, 0 );
        break;

        case 0:
        default:
        break;
    }

    SET_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S4DECFG, 0);  // Smbus 0 Drive enabled
    break;

   case 5:
    SET_REG_FIELD(MFSEL1, MFSEL1_SMB5SEL, TRUE);
    switch(segment)
    {
        case 1:
        SET_REG_FIELD(MFSEL1, MFSEL1_GSPISEL, 0 );
        break;


        case 2:
        SET_REG_FIELD(MFSEL1, MFSEL1_GSPISEL, 0 );
        break;

        case 3:

        SET_REG_FIELD(MFSEL1, MFSEL1_KBCICSEL, 0 );
        break;

        case 0:
        default:
        break;
    }
    break;
    case 6:
        SET_REG_FIELD(MFSEL3, MFSEL3_SMB6SEL, 1);
        break;

    case 7:
        SET_REG_FIELD(MFSEL3, MFSEL3_SMB7SEL, 1);
        break;

    case 8:
        SET_REG_FIELD(MFSEL4, MFSEL4_SMB8SEL, 1);
        break;

    case 9:
        SET_REG_FIELD(MFSEL4, MFSEL4_SMB9SEL, 1);
        break;

    case 10:
        SET_REG_FIELD(MFSEL4, MFSEL4_SMB10SEL, 1);
        break;

    case 11:
        SET_REG_FIELD(MFSEL4, MFSEL4_SMB11SEL, 1);


    case 12:
        SET_REG_FIELD(MFSEL3, MFSEL3_SMB12SEL, 1);
        break;

    case 13:
        SET_REG_FIELD(MFSEL3, MFSEL3_SMB13SEL, 1);
        break;

    case 14:
        SET_REG_FIELD(MFSEL3, MFSEL3_SMB14SEL, 1);
        break;

    case 15:
        SET_REG_FIELD(MFSEL3, MFSEL3_SMB15SEL, 1);
        break;

    default:
    ASSERT(FALSE);
    break;
    }

    CHIP_Mux_Segment(smb_module, segment, TRUE);

    CHIP_SMBPullUp(smb_module, TRUE);
}
#endif // if 0

#if defined (SMB_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_SMB_GetSegment                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  smb_module - SMB Module number                                                         */
/*                  segment - 0 => A, 1 => B , 2 => C, 3 => D                                              */
/*                                                                                                         */
/* Returns:         status                                                                                 */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine returns the segment number which is currently muxed to a given module     */
/*                  This feature is applicable to SMB0 to SMB5 only                                        */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS CHIP_SMB_GetSegment (SMB_MODULE_T smb_module, UINT* segment)
{
    *segment = 0;

    /*-----------------------------------------------------------------------------------------------------*/
    /* parameter validity check                                                                            */
    /*-----------------------------------------------------------------------------------------------------*/
    if (smb_module > SMB_NUM_OF_MODULES )
    {
        return DEFS_STATUS_INVALID_PARAMETER;
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* SMB0 to SMB5 have segments, the rest has a single segment only                                      */
    /*-----------------------------------------------------------------------------------------------------*/
    if (smb_module > 7 )
    {
        return DEFS_STATUS_OK;
    }

    switch (smb_module)
    {
    case 0:
    if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S0SBSEL) != 0 )
        *segment = 1;
    else if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S0SCSEL) != 0 )
        *segment = 2;
    else if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S0SDSEL) != 0 )
        *segment = 3;
    else
        *segment = 0;
    break;

    case 1:
    if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S1SBSEL) != 0 )
        *segment = 1;
    else if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S1SCSEL) != 0 )
        *segment = 2;
    else if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S1SDSEL) != 0 )
        *segment = 3;
    else
        *segment = 0;
    break;

    case 2:
    if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S2SBSEL) != 0 )
        *segment = 1;
    else if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S2SCSEL) != 0 )
        *segment = 2;
    else if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S2SDSEL) != 0 )
        *segment = 3;
    else
        *segment = 0;
    break;

    case 3:
    if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S3SBSEL) != 0 )
        *segment = 1;
    else if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S3SCSEL) != 0 )
        *segment = 2;
    else if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S3SDSEL) != 0 )
        *segment = 3;
    else
        *segment = 0;
    break;

    case 4:
    if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S4SBSEL) != 0 )
        *segment = 1;
    else if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S4SCSEL) != 0 )
        *segment = 2;
    else if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S4SDSEL) != 0 )
        *segment = 3;
    else
        *segment = 0;
    break;

    case 5:
    if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S5SBSEL) != 0 )
        *segment = 1;
    else if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S5SCSEL) != 0 )
        *segment = 2;
    else if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S5SDSEL) != 0 )
        *segment = 3;
    else
        *segment = 0;
    break;

    case 6:
    if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S6SBSEL) != 0 )
        *segment = 1;
    else if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S6SCSEL) != 0 )
        *segment = 2;
    else if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S6SDSEL) != 0 )
        *segment = 3;
    else
        *segment = 0;
    break;


    case 7:
    if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S7SBSEL) != 0 )
        *segment = 1;
    else if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S7SCSEL) != 0 )
        *segment = 2;
    else if ( READ_REG_FIELD(I2CSEGSEL, I2CSEGSEL_S7SDSEL) != 0 )
        *segment = 3;
    else
        *segment = 0;
    break;



    default:
    ASSERT(0);
    }

    return DEFS_STATUS_OK;
}
#endif // SMB_MODULE_TYPE

#if defined (DAC_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_MuxDAC                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  dac_input - DAC Module number                                                          */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine selects the DACx outputs interface on multiplexed pin.                    */
/*                  The function should be called prior to any other DAC operation.                        */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_MuxDAC (DAC_INPUT_T dac_input)
{
    ASSERT(dac_input < DAC_NUM_OF_INPUTS);

    /*-----------------------------------------------------------------------------------------------------*/
    /* SET_REG_BIT(DEVALT4, dac_input);                                                                    */
    /*-----------------------------------------------------------------------------------------------------*/
}
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_Get_Version                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine returns the chip version (POLEG_VERSION_Z1/ POLEG_VERSION_Z2              */
/*                                                         POLEG_VERSION_A1)                               */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CHIP_Get_Version (void)
{
    return READ_REG_FIELD(PDID, PDID_VERSION);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_Disable_CPU1                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine disable second core for singel core solution (used in Z2 and above)       */
/*---------------------------------------------------------------------------------------------------------*/
void   CHIP_Disable_CPU1 (void)
{
    UINT32        flockr1 = 0;

    flockr1 = REG_READ(FLOCKR1);

    SET_VAR_FIELD(flockr1, FLOCKR1_CPU1CKDIS, 1);

    SET_VAR_FIELD(flockr1, FLOCKR1_CPU1CKDLK, 1);

    REG_WRITE(FLOCKR1, flockr1);

    return;
}

#if defined (ESPI_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_MuxESPI                                                                           */
/*                                                                                                         */
/* Parameters:      driveStrength_mA ESPI drive strength in mA                                             */
/* Returns:         TRUE if ESPI is enabled; FALSE otherwise.                                              */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine enables the ESPI                                                          */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_MuxESPI (UINT driveStrength_mA)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* Enable ESPI signals                                                                                 */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD(MFSEL4, MFSEL4_ESPISEL, 1);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Select ESPI drive strength                                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    if (driveStrength_mA <= 8)
    {
        // TODO: SET_REG_FIELD(DSCNT, DSCNT_ESPI, 2); // 0 1 0: 8 mA
    }
    else if(driveStrength_mA <= 12)
    {
        // TODO: SET_REG_FIELD(DSCNT, DSCNT_ESPI, 3); // 0 1 1: 12 mA (LPC default)
    }
    else if(driveStrength_mA <= 16)
    {
        // TODO: SET_REG_FIELD(DSCNT, DSCNT_ESPI, 6); // 1 1 0: 16 mA (may be used for eSPI)
    }
    else if(driveStrength_mA <= 24)
    {
        // TODO: SET_REG_FIELD(DSCNT, DSCNT_ESPI, 7); // 1 1 1: 24 mA (Should be used for eSPI – requires an external 13.7 ohm series resistor)
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}
#endif


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_MuxLPC                                                                            */
/*                                                                                                         */
/* Parameters:      driveStrength_mA LPC drive strength in mA                                              */
/* Returns:         TRUE if LPC is enabled; FALSE otherwise.                                               */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine enables the LPC                                                           */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_MuxLPC (_UNUSED_ UINT driveStrength_mA)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* Enable ESPI signals                                                                                 */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD(MFSEL4, MFSEL4_ESPISEL, 0);
    SET_REG_FIELD(MFSEL3, MFSEL3_CLKRUNSEL, 1);
    SET_REG_FIELD(INTCR2, INTCR2_DIS_ESPI_AUTO_INIT, 1);

#if 0
    /*-----------------------------------------------------------------------------------------------------*/
    /* Select ESPI drive strength                                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    if (driveStrength_mA <= 8)
    {
        SET_REG_FIELD(DSCNT, DSCNT_ESPI, 2); // 0 1 0: 8 mA
    }
    else if(driveStrength_mA <= 12)
    {
        SET_REG_FIELD(DSCNT, DSCNT_ESPI, 3); // 0 1 1: 12 mA (LPC default)
    }
    else if(driveStrength_mA <= 16)
    {
        SET_REG_FIELD(DSCNT, DSCNT_ESPI, 6); // 1 1 0: 16 mA (may be used for eSPI)
    }
    else if(driveStrength_mA <= 24)
    {
        SET_REG_FIELD(DSCNT, DSCNT_ESPI, 7); // 1 1 1: 24 mA (Should be used for eSPI – requires an external 13.7 ohm series resistor)
    }
    else
    {
        return FALSE;
    }
#endif

    return TRUE;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_StrapJ3EN                                                                         */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine gets strap 11 (Jtag3 Enable) value.                                       */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_StrapJ3EN (void)
{
    return READ_REG_FIELD(PWRON, PWRON_J3EN);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_StrapFUP                                                                          */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine gets strap 9 (Flash UART Programming) value.                              */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_StrapFUP (void)
{
    return READ_REG_FIELD(PWRON, PWRON_FUP);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_StrapSECEN                                                                        */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine gets strap 8 (Security Enable) value.                                     */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_StrapSECEN (void)
{
    return READ_REG_FIELD(PWRON, PWRON_SECEN);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_StrapBSPA                                                                         */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine gets strap 5 (BMC debug Serial Port) value.                               */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_StrapBSPA (void)
{
    return READ_REG_FIELD(PWRON, PWRON_BSPA);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_StrapJ2EN                                                                         */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine gets strap 4 (J2EN) value.                                                */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_StrapJ2EN (void)
{
    return READ_REG_FIELD(PWRON, PWRON_J2EN);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_StrapSPI0F18                                                                      */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine gets strap 3 (SPI0F18) value.                                             */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_StrapSPI0F18 (void)
{
    return READ_REG_FIELD(PWRON, PWRON_SPI0F18);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_PrintRegs_GCR                                                                     */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the chip registers                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_PrintRegs_GCR (void)
{
    HAL_PRINT("/*--------------*/\n");
    HAL_PRINT("/*     CHIP     */\n");
    HAL_PRINT("/*--------------*/\n\n");

    HAL_PRINT("PDID        = %#010lx \n", REG_READ(PDID        ));
    HAL_PRINT("PWRON       = %#010lx \n", REG_READ(PWRON       ));
    HAL_PRINT("SWSTRPS     = %#010lx \n", REG_READ(SWSTRPS     ));
    HAL_PRINT("MFSEL1      = %#010lx \n", REG_READ(MFSEL1      ));
    HAL_PRINT("MFSEL2      = %#010lx \n", REG_READ(MFSEL2      ));
    HAL_PRINT("MFSEL3      = %#010lx \n", REG_READ(MFSEL3      ));
    HAL_PRINT("MFSEL4      = %#010lx \n", REG_READ(MFSEL4      ));
    HAL_PRINT("MFSEL5      = %#010lx \n", REG_READ(MFSEL5      ));
    HAL_PRINT("MFSEL6      = %#010lx \n", REG_READ(MFSEL6      ));
    HAL_PRINT("MFSEL7      = %#010lx \n", REG_READ(MFSEL7      ));
    HAL_PRINT("MISCPE      = %#010lx \n", REG_READ(MISCPE      ));
    HAL_PRINT("SPLDCNT     = %#010lx \n", REG_READ(SPLDCNT     ));
    HAL_PRINT("A35_MODE    = %#010lx \n", REG_READ(A35_MODE    ));
    HAL_PRINT("SPSWC       = %#010lx \n", REG_READ(SPSWC       ));
    HAL_PRINT("INTCR       = %#010lx \n", REG_READ(INTCR       ));
    HAL_PRINT("INTSR2      = %#010lx \n", REG_READ(INTSR2      ));
    HAL_PRINT("OBSCR1      = %#010lx \n", REG_READ(OBSCR1      ));
    HAL_PRINT("OBSCR2      = %#010lx \n", REG_READ(OBSCR2      ));
    HAL_PRINT("HIFCR       = %#010lx \n", REG_READ(HIFCR       ));
    HAL_PRINT("SD2SUR1     = %#010lx \n", REG_READ(SD2SUR1     ));
    HAL_PRINT("SD2SUR2     = %#010lx \n", REG_READ(SD2SUR2     ));
    HAL_PRINT("SD2IRV3     = %#010lx \n", REG_READ(SD2IRV3     ));
    HAL_PRINT("SD2IRV4     = %#010lx \n", REG_READ(SD2IRV4     ));
    HAL_PRINT("SD2IRV5     = %#010lx \n", REG_READ(SD2IRV5     ));
    HAL_PRINT("SD2IRV6     = %#010lx \n", REG_READ(SD2IRV6     ));
    HAL_PRINT("SD2IRV7     = %#010lx \n", REG_READ(SD2IRV7     ));
    HAL_PRINT("SD2IRV8     = %#010lx \n", REG_READ(SD2IRV8     ));
    HAL_PRINT("SD2IRV9     = %#010lx \n", REG_READ(SD2IRV9     ));
    HAL_PRINT("SD2IRV10    = %#010lx \n", REG_READ(SD2IRV10    ));
    HAL_PRINT("SD2IRV11    = %#010lx \n", REG_READ(SD2IRV11    ));
    HAL_PRINT("INTCR2      = %#010lx \n", REG_READ(INTCR2      ));
    HAL_PRINT("SRCNT       = %#010lx \n", REG_READ(SRCNT       ));
    HAL_PRINT("RESSR       = %#010lx \n", REG_READ(RESSR       ));
    HAL_PRINT("RLOCKR1     = %#010lx \n", REG_READ(RLOCKR1     ));
    HAL_PRINT("FLOCKR1     = %#010lx \n", REG_READ(FLOCKR1     ));
    HAL_PRINT("FLOCKR2     = %#010lx \n", REG_READ(FLOCKR2     ));
    HAL_PRINT("FLOCKR3     = %#010lx \n", REG_READ(FLOCKR3     ));
    HAL_PRINT("DSCNT       = %#010lx \n", REG_READ(DSCNT       ));
    HAL_PRINT("MDLR        = %#010lx \n", REG_READ(MDLR        ));
    HAL_PRINT("INTCR3      = %#010lx \n", REG_READ(INTCR3      ));
    HAL_PRINT("INTCR4      = %#010lx \n", REG_READ(INTCR4      ));
    HAL_PRINT("PCIRCTL     = %#010lx \n", REG_READ(PCIRCTL     ));
    HAL_PRINT("I2CSEGSEL   = %#010lx \n", REG_READ(I2CSEGSEL   ));
    HAL_PRINT("I2CSEGCTL   = %#010lx \n", REG_READ(I2CSEGCTL   ));
    HAL_PRINT("VSRCR       = %#010lx \n", REG_READ(VSRCR       ));
    HAL_PRINT("MLOCKR      = %#010lx \n", REG_READ(MLOCKR      ));
    HAL_PRINT("SCRPAD_02   = %#010lx \n", REG_READ(SCRPAD_02   ));
    HAL_PRINT("SCRPAD_03   = %#010lx \n", REG_READ(SCRPAD_03   ));
    HAL_PRINT("ETSR        = %#010lx \n", REG_READ(ETSR        ));
    HAL_PRINT("EDFFSR      = %#010lx \n", REG_READ(EDFFSR      ));
#if 0
    HAL_PRINT("INTCRPCE2   = %#010lx \n", REG_READ(INTCRPCE2   ));
    HAL_PRINT("INTCRPCE0   = %#010lx \n", REG_READ(INTCRPCE0   ));
    HAL_PRINT("INTCRPCE1   = %#010lx \n", REG_READ(INTCRPCE1   ));
    HAL_PRINT("INTCRPCE3   = %#010lx \n", REG_READ(INTCRPCE3   ));
    HAL_PRINT("INTCRPCE2B  = %#010lx \n", REG_READ(INTCRPCE2B  ));
    HAL_PRINT("INTCRPCE0B  = %#010lx \n", REG_READ(INTCRPCE0B  ));
    HAL_PRINT("INTCRPCE1B  = %#010lx \n", REG_READ(INTCRPCE1B  ));
    HAL_PRINT("INTCRPCE3B  = %#010lx \n", REG_READ(INTCRPCE3B  ));
#endif
    HAL_PRINT("SCRPAD      = %#010lx \n", REG_READ(SCRPAD      ));
    HAL_PRINT("USB1PHYCTL  = %#010lx \n", REG_READ(USB1PHYCTL  ));
    HAL_PRINT("USB2PHYCTL  = %#010lx \n", REG_READ(USB2PHYCTL  ));
    HAL_PRINT("USB3PHYCTL  = %#010lx \n", REG_READ(USB3PHYCTL  ));
    HAL_PRINT("CPCTL1      = %#010lx \n", REG_READ(CPCTL1      ));
    HAL_PRINT("CP2BST1     = %#010lx \n", REG_READ(CP2BST1     ));
    HAL_PRINT("B2CPNT1     = %#010lx \n", REG_READ(B2CPNT1     ));
    HAL_PRINT("CPPCTL1     = %#010lx \n", REG_READ(CPPCTL1     ));
    HAL_PRINT("CPBPNTR1    = %#010lx \n", REG_READ(CPBPNTR1    ));
    HAL_PRINT("CPCTL2      = %#010lx \n", REG_READ(CPCTL2      ));
    HAL_PRINT("CP2BST2     = %#010lx \n", REG_READ(CP2BST2     ));
    HAL_PRINT("B2CPNT2     = %#010lx \n", REG_READ(B2CPNT2     ));
    HAL_PRINT("CPSP2       = %#010lx \n", REG_READ(CPSP2       ));

    HAL_PRINT("\n");
}


#if defined (FUSE_MODULE_TYPE)
#include "npcm850_fuse_wrapper.c"
#endif

