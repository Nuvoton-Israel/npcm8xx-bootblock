/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   clk_drv.c                                                                */
/*            This file contains CLK module implementation                    */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                                 INCLUDES                                                */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#include __CHIP_H_FROM_DRV()

#include "hal.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Include driver registers                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
#include "../4/clk_regs.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Include driver header                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#include "../4/clk_drv.h"


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                         TYPES & DEFINITIONS                                             */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* PLLCON0 and PLLCON2 possible values:                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define CLK_125MHZ_PLLCON0_2_REG_CFG    0x00284401
#define CLK_333MHZ_PLLCON0_2_REG_CFG    0x00A02403
#define CLK_500MHZ_PLLCON0_2_REG_CFG    0x00282201
#define CLK_600MHZ_PLLCON0_2_REG_CFG    0x00302201
#define CLK_666MHZ_PLLCON0_2_REG_CFG    0x00A02203
#define CLK_700MHZ_PLLCON0_2_REG_CFG    0x001C2101
#define CLK_720MHZ_PLLCON0_2_REG_CFG    0x00902105
#define CLK_750MHZ_PLLCON0_2_REG_CFG    0x003C2201 /* change from spec to avoid VCO too high*/
#define CLK_800MHZ_PLLCON0_2_REG_CFG    0x00202101
#define CLK_825MHZ_PLLCON0_2_REG_CFG    0x00212101
#define CLK_850MHZ_PLLCON0_2_REG_CFG    0x00222101
#define CLK_888MHZ_PLLCON0_2_REG_CFG    0x03782119
#define CLK_900MHZ_PLLCON0_2_REG_CFG    0x00242101
#define CLK_950MHZ_PLLCON0_2_REG_CFG    0x00262101
#define CLK_960MHZ_PLLCON0_2_REG_CFG    0x00C02105
#define CLK_1000MHZ_PLLCON0_2_REG_CFG   0x00282101

/*---------------------------------------------------------------------------------------------------------*/
/* PLLCON 1 possible values (notice that PLL1 in Z2 has a divider /2, so OTDV1 is smaller in half          */
/*---------------------------------------------------------------------------------------------------------*/
#define CLK_300MHZ_PLLCON1_REG_CFG      0x00182201
#define CLK_500MHZ_PLLCON1_REG_CFG      0x00282201
#define CLK_666MHZ_PLLCON1_REG_CFG      0x00A02203
#define CLK_700MHZ_PLLCON1_REG_CFG      0x00382201
#define CLK_720MHZ_PLLCON1_REG_CFG      0x01202205
#define CLK_750MHZ_PLLCON1_REG_CFG      0x003C2201
#define CLK_775MHZ_PLLCON1_REG_CFG      0x003E2201
#define CLK_787_5MHZ_PLLCON1_REG_CFG    0x003F2201
#define CLK_800MHZ_PLLCON1_REG_CFG      0x00402201
#define CLK_850MHZ_PLLCON1_REG_CFG      0x00442201
#define CLK_900MHZ_PLLCON1_REG_CFG      0x00482201
#define CLK_950MHZ_PLLCON1_REG_CFG      0x004C2201
#define CLK_975MHZ_PLLCON1_REG_CFG      0x004E2201
#define CLK_1000MHZ_PLLCON1_REG_CFG     0x00502201
#define CLK_1037MHZ_PLLCON1_REG_CFG     0x00532201
#define CLK_1050MHZ_PLLCON1_REG_CFG     0x00542201
#define CLK_1062_5_MHZ_PLLCON1_REG_CFG  0x00542201
#define CLK_1066MHZ_PLLCON1_REG_CFG     0x01002203
#define CLK_1100MHZ_PLLCON1_REG_CFG     0x00582201
#define CLK_1150MHZ_PLLCON1_REG_CFG     0x005C2201
#define CLK_1175MHZ_PLLCON1_REG_CFG     0x005E2201
#define CLK_1200MHZ_PLLCON1_REG_CFG     0x00602201
#define CLK_1225MHZ_PLLCON1_REG_CFG     0x00622201

/*---------------------------------------------------------------------------------------------------------*/
/* PLLCON 2 possible values                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
#define CLK_960MHZ_PLLCON2_REG_CFG    0x00C02105

#define LOK_TIMEOUT  100000  /* 4ms if 25 MHz */

/*---------------------------------------------------------------------------------------------------------*/
/* Local definitions                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
/* SD\MMC Clock Target frequency : Souhld be 40MHz-50MHz*/
#ifndef SD_CLK_TARGET_FREQ
#define SD_CLK_TARGET_FREQ          (50 * _1MHz_)
#endif

/* USB clocks  */
#define SU48_DESIRED_FREQUENCY      48  // MHz (dont use _1MHz_)
#define SU_DESIRED_FREQUENCY        30  // MHz (dont use _1MHz_)

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                            LOCAL FUNCTIONS                                              */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_CalculatePLLFrequency                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  pllVal    -                                                                            */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine returns the PLL frequency in Hz                                           */
/*---------------------------------------------------------------------------------------------------------*/
static UINT32 CLK_CalculatePLLFrequency (UINT32 pllVal)
{
    UINT32  CLK_FIN         = EXT_CLOCK_FREQUENCY_KHZ; // 25MHz in KHz units
    UINT32  CLK_FOUT        = 0;
    UINT32  CLK_NR          = 0;
    UINT32  CLK_NF          = 0;
    UINT32  CLK_NO          = 0;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Extract PLL fields:                                                                                 */
    /*-----------------------------------------------------------------------------------------------------*/
    CLK_NR = READ_VAR_FIELD(pllVal, PLLCONn_INDV);   /* PLL Input Clock Divider */
    CLK_NF = READ_VAR_FIELD(pllVal, PLLCONn_FBDV);   /* PLL VCO Output Clock Feedback Divider). */
    CLK_NO = (READ_VAR_FIELD(pllVal, PLLCONn_OTDV1)) * (READ_VAR_FIELD(pllVal, PLLCONn_OTDV2));   /* PLL Output Clock Divider 1 */

    /*-----------------------------------------------------------------------------------------------------*/
    /* Calculate PLL frequency in Hz:                                                                     */
    /*-----------------------------------------------------------------------------------------------------*/
    CLK_FOUT = ((10*CLK_FIN*CLK_NF)/(CLK_NO*CLK_NR));

    /*-----------------------------------------------------------------------------------------------------*/
    /* Notice: for better accurecy we multiply the "MONE" in 10, and later in 100 to get to Hz units.      */
    /*-----------------------------------------------------------------------------------------------------*/

    /*-----------------------------------------------------------------------------------------------------*/
    /* Returning value in Hertz:                                                                           */
    /*-----------------------------------------------------------------------------------------------------*/
    return  CLK_FOUT*100 ;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ConfigureFastSkipInitClocks                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  none                                                                                   */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures fast skip init mode that touches only dividers                 */
/*---------------------------------------------------------------------------------------------------------*/
static void CLK_ConfigureFastSkipInitClocks (void)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* clk diveiders local values                                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    UINT32  clkdiv1_L;
    UINT32  clkdiv2_L;
    UINT32  clkdiv3_L;
    UINT32  clkdiv4_L;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Read CLKDIV default configuration                                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
    clkdiv1_L          = REG_READ(CLKDIV1);
    clkdiv2_L          = REG_READ(CLKDIV2);
    clkdiv3_L          = REG_READ(CLKDIV3);
    clkdiv4_L          = REG_READ(CLKDIV4);

    // Fast skip init: put all dividers an minimum:
    SET_VAR_FIELD(clkdiv1_L, CLKDIV1_CLK4DIV,    CLKDIV1_CLK4DIV_DIV(1));  // 12.5MHz

    SET_VAR_FIELD(clkdiv2_L, CLKDIV2_APB4CKDIV,  CLKDIV2_APBxCKDIV_DIV(2));
    SET_VAR_FIELD(clkdiv2_L, CLKDIV2_APB3CKDIV,  CLKDIV2_APBxCKDIV_DIV(2));
    SET_VAR_FIELD(clkdiv2_L, CLKDIV2_APB2CKDIV,  CLKDIV2_APBxCKDIV_DIV(2));
    SET_VAR_FIELD(clkdiv2_L, CLKDIV2_APB1CKDIV,  CLKDIV2_APBxCKDIV_DIV(2));
    SET_VAR_FIELD(clkdiv2_L, CLKDIV2_APB5CKDIV,  CLKDIV2_APBxCKDIV_DIV(2));
    SET_VAR_FIELD(clkdiv2_L, CLKDIV2_SD1CKDIV,   CLKDIV2_SD1CKDIV_DIV(32));

    SET_VAR_FIELD(clkdiv3_L, CLKDIV3_SPI0CKDIV,  CLKDIV3_SPI0CKDIV_DIV(3));
    SET_VAR_FIELD(clkdiv3_L, CLKDIV3_SPI1CKDIV,  CLKDIV3_SPI1CKDIV_DIV(3));
    SET_VAR_FIELD(clkdiv1_L, CLKDIV1_AHB3CKDIV,  CLKDIV1_AHB3CKDIV_DIV(3)); // SPI3
    SET_VAR_FIELD(clkdiv3_L, CLKDIV3_SPIXCKDIV,  CLKDIV3_SPIXCKDIV_DIV(3));

    SET_VAR_FIELD(clkdiv2_L, CLKDIV2_GFXCKDIV,   CLKDIV2_GFXCKDIV_DIV(1));
    SET_VAR_FIELD(clkdiv1_L, CLKDIV1_PCICKDIV,   CLKDIV1_PCICKDIV_DIV(1));
    SET_VAR_FIELD(clkdiv1_L, CLKDIV1_MMCCKDIV,   CLKDIV1_MMCCKDIV_DIV(1));
    SET_VAR_FIELD(clkdiv4_L, CLKDIV4_RGREFDIV,   CLKDIV4_RGREFDIV_DIV(1));

    SET_VAR_FIELD(clkdiv4_L  , CLKDIV4_RCPREFDIV,  CLKDIV4_RCPREFDIV_DIV(1));
    SET_VAR_FIELD(clkdiv1_L  , CLKDIV1_ADCCKDIV,   CLKDIV1_ADCCKDIV_DIV(1));
    SET_VAR_FIELD(clkdiv1_L  , CLKDIV1_PRE_ADCCKDIV,  CLKDIV1_PRE_ADCCKDIV_DIV(1) );
    SET_VAR_FIELD(clkdiv2_L  , CLKDIV2_CLKOUTDIV,  CLKDIV2_CLKOUTDIV_DIV(16));

    SET_VAR_FIELD(clkdiv2_L  , CLKDIV2_SUCKDIV,    CLKDIV2_SUCKDIV_DIV(1));
    SET_VAR_FIELD(clkdiv2_L  , CLKDIV2_SU48CKDIV,  CLKDIV2_SU48CKDIV_DIV(1));

    SET_VAR_FIELD(clkdiv1_L  , CLKDIV1_UARTDIV1,   CLKDIV1_UARTDIV1_DIV(7));
    SET_VAR_FIELD(clkdiv3_L  , CLKDIV3_UARTDIV2,   CLKDIV3_UARTDIV2_DIV(7));

    /*-----------------------------------------------------------------------------------------------------*/
    /* Change CLKDIV configuration:                                                                        */
    /*-----------------------------------------------------------------------------------------------------*/
    REG_WRITE(CLKDIV1, clkdiv1_L);
    REG_WRITE(CLKDIV2, clkdiv2_L);
    REG_WRITE(CLKDIV3, clkdiv3_L);
    REG_WRITE(CLKDIV4, clkdiv4_L);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Wait for 10 clock cycles between clkDiv change and clkSel change:                                   */
    /*-----------------------------------------------------------------------------------------------------*/
    CLK_Delay_MicroSec(10);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        WaitForPllLock                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  none                                                                                   */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine ...                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
static DEFS_STATUS WaitForPllLock (PLL_MODULE_T pll)
{
    volatile UINT32 loki_bit = 0;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Read LOKI till PLL is locked or timeout expires                                                     */
    /*-----------------------------------------------------------------------------------------------------*/
    while (1)
    {
        switch (pll)
        {
            case PLL0:
                loki_bit = READ_REG_FIELD(PLLCON0, PLLCONn_LOKI);
                break;
            case PLL1:
                loki_bit = READ_REG_FIELD(PLLCON1, PLLCONn_LOKI);
                break;
            case PLL2:
                loki_bit = READ_REG_FIELD(PLLCON2, PLLCONn_LOKI);
                break;
            case PLL_GFX:
                loki_bit = READ_REG_FIELD(PLLCONG, PLLCONG_LOKI);
                break;
            default:
                return DEFS_STATUS_PARAMETER_OUT_OF_RANGE;
        }

        /*-------------------------------------------------------------------------------------------------*/
        /* PLL is locked, clear and break                                                                  */
        /*-------------------------------------------------------------------------------------------------*/
        if (loki_bit == 1)
        {
            switch (pll)
            {
                case PLL0:
                    SET_REG_FIELD(PLLCON0, PLLCONn_LOKS, 1);
                    break;
                case PLL1:
                    SET_REG_FIELD(PLLCON1, PLLCONn_LOKS, 1);
                    break;
                case PLL2:
                    SET_REG_FIELD(PLLCON2, PLLCONn_LOKS, 1);
                    break;
                case PLL_GFX:
                    SET_REG_FIELD(PLLCONG, PLLCONn_LOKS, 1);
                    break;
                default:
                    return DEFS_STATUS_PARAMETER_OUT_OF_RANGE;
            }

            break;
        }
    }

    return DEFS_STATUS_OK;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetPllGFreq                                                                        */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Returns the frequency of PLLG in Hz                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static UINT32 CLK_GetPllGFreq (void)
{
    UINT32 pllVal = REG_READ(PLLCONG);

    return (CLK_CalculatePLLFrequency(pllVal) / 2); // note: pixclk and clkout have /2 after PLLG
}

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                           INTERFACE FUNCTIONS                                           */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ConfigureClocks                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  straps -  strap values from the PWRON register (flipped according to FUSTRAP register) */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures the chip PLLs and selects clock sources for modules            */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_ConfigureClocks (UINT32 strp)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* pll configuration local values                                                                      */
    /*-----------------------------------------------------------------------------------------------------*/
    UINT32  pllcon0_L  = 0;
    UINT32  pllcon1_L  = 0;
    UINT32  pllcon2_L  = 0;

    /*-----------------------------------------------------------------------------------------------------*/
    /* clk diveiders local values                                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    UINT32  clkdiv1_L  = 0;
    UINT32  clkdiv2_L  = 0;
    UINT32  clkdiv3_L  = 0;
    UINT32  clkdiv4_L  = 0;

    /*-----------------------------------------------------------------------------------------------------*/
    /* clk mux local values                                                                                */
    /*-----------------------------------------------------------------------------------------------------*/
    UINT32  clksel_L   = 0;

    /*-----------------------------------------------------------------------------------------------------*/
    /*  CPU Throttling Control Register (THRTL_CNT)                                                    */
    /*-----------------------------------------------------------------------------------------------------*/
    UINT32 thrtl_clk_L = 0;

    UINT32 clksel_clkref_L = 0;

    /* SKIP_INIT */
    if (strp == 1)
    {
        return;
    }

    /* Fast SKIP_INIT */
    if (strp == 5)
    {
        CLK_ConfigureFastSkipInitClocks();
        return;
    }

    if (strp > 5)
    {
        strp &= 0x3;  // STRAP14 is don't care in all cases apart from SKIP_INIT
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* Common configurations for the clk mux. will be changes later according to straps 1-2                */
    /*-----------------------------------------------------------------------------------------------------*/
    clksel_L           = REG_READ(CLKSEL);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Read CLKDIV default configuration                                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
    clkdiv1_L          = REG_READ(CLKDIV1);
    clkdiv2_L          = REG_READ(CLKDIV2);
    clkdiv3_L          = REG_READ(CLKDIV3);
    clkdiv4_L          = REG_READ(CLKDIV4);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Set all clocks to default state - do not rely on the default value                                  */
    /*-----------------------------------------------------------------------------------------------------*/
    SET_VAR_FIELD(clksel_L, CLKSEL_RCPCKSEL    , CLKSEL_RCPCKSEL_CLKREF   );
    SET_VAR_FIELD(clksel_L, CLKSEL_RGSEL       , CLKSEL_RGSEL_CLKREF       );
    SET_VAR_FIELD(clksel_L, CLKSEL_AHB6SSEL    , CLKSEL_AHB6SSEL_CLKREF    );
    SET_VAR_FIELD(clksel_L, CLKSEL_GFXMSEL     , CLKSEL_GFXMSEL_CLKREF     );
    SET_VAR_FIELD(clksel_L, CLKSEL_CLKOUTSEL   , CLKSEL_CLKOUTSEL_CLKREF   );
    SET_VAR_FIELD(clksel_L, CLKSEL_PCIGFXCKSEL , CLKSEL_PCIGFXCKSEL_CLKREF );
    SET_VAR_FIELD(clksel_L, CLKSEL_ADCCKSEL    , CLKSEL_ADCCKSEL_CLKREF    );
    SET_VAR_FIELD(clksel_L, CLKSEL_MCCKSEL     , CLKSEL_MCCKSEL_CLKREF     );
    SET_VAR_FIELD(clksel_L, CLKSEL_SUCKSEL     , CLKSEL_SUCKSEL_CLKREF     );
    SET_VAR_FIELD(clksel_L, CLKSEL_UARTCKSEL   , CLKSEL_UARTCKSEL_CLKREF   );
    SET_VAR_FIELD(clksel_L, CLKSEL_SDCKSEL     , CLKSEL_SDCKSEL_CLKREF     );
    SET_VAR_FIELD(clksel_L, CLKSEL_PIXCKSEL    , CLKSEL_PIXCKSEL_CLKREF    );
    SET_VAR_FIELD(clksel_L, CLKSEL_CPUCKSEL    , CLKSEL_CPUCKSEL_CLKREF    );

    clksel_clkref_L = clksel_L;

    // default and\or recommended values:
    SET_VAR_FIELD(clkdiv1_L  , CLKDIV1_ADCCKDIV,   CLKDIV1_ADCCKDIV_DIV(1));
    SET_VAR_FIELD(clkdiv1_L  , CLKDIV1_CLK4DIV,    CLKDIV1_CLK4DIV_DIV(2));
    SET_VAR_FIELD(clkdiv1_L  , CLKDIV1_PRE_ADCCKDIV,  CLKDIV1_PRE_ADCCKDIV_DIV(1) );
    SET_VAR_FIELD(clkdiv1_L  , CLKDIV1_UARTDIV1,   CLKDIV1_UARTDIV1_DIV(7));
    SET_VAR_FIELD(clkdiv1_L  , CLKDIV1_MMCCKDIV,   CLKDIV1_MMCCKDIV_DIV(32));
    SET_VAR_FIELD(clkdiv1_L  , CLKDIV1_AHB3CKDIV,  CLKDIV1_AHB3CKDIV_DIV(2));
    SET_VAR_FIELD(clkdiv1_L  , CLKDIV1_PCICKDIV,   CLKDIV1_PCICKDIV_DIV(6));
    SET_VAR_FIELD(clkdiv2_L  , CLKDIV2_APB4CKDIV,  CLKDIV2_APBxCKDIV_DIV(4));
    SET_VAR_FIELD(clkdiv2_L  , CLKDIV2_APB3CKDIV,  CLKDIV2_APBxCKDIV_DIV(4));
    SET_VAR_FIELD(clkdiv2_L  , CLKDIV2_APB2CKDIV,  CLKDIV2_APBxCKDIV_DIV(4));
    SET_VAR_FIELD(clkdiv2_L  , CLKDIV2_APB1CKDIV,  CLKDIV2_APBxCKDIV_DIV(4));
    SET_VAR_FIELD(clkdiv2_L  , CLKDIV2_APB5CKDIV,  CLKDIV2_APBxCKDIV_DIV(2));
    SET_VAR_FIELD(clkdiv2_L  , CLKDIV2_CLKOUTDIV,  CLKDIV2_CLKOUTDIV_DIV(16));
    SET_VAR_FIELD(clkdiv2_L  , CLKDIV2_GFXCKDIV,   CLKDIV2_GFXCKDIV_DIV(5));
    SET_VAR_FIELD(clkdiv2_L  , CLKDIV2_SUCKDIV,    CLKDIV2_SUCKDIV_DIV(1));
    SET_VAR_FIELD(clkdiv2_L  , CLKDIV2_SU48CKDIV,  CLKDIV2_SU48CKDIV_DIV(1));
    SET_VAR_FIELD(clkdiv2_L  , CLKDIV2_SD1CKDIV,   CLKDIV2_SD1CKDIV_DIV(32));
    SET_VAR_FIELD(clkdiv3_L  , CLKDIV3_SPI1CKDIV,  CLKDIV3_SPI1CKDIV_DIV(1));
    SET_VAR_FIELD(clkdiv3_L  , CLKDIV3_UARTDIV2,   CLKDIV3_UARTDIV2_DIV(7));
    SET_VAR_FIELD(clkdiv3_L  , CLKDIV3_SPI0CKDIV,  CLKDIV3_SPI0CKDIV_DIV(1));
    SET_VAR_FIELD(clkdiv3_L  , CLKDIV3_SPIXCKDIV,  CLKDIV3_SPIXCKDIV_DIV(1));
    SET_VAR_FIELD(clkdiv4_L  , CLKDIV4_RGREFDIV,   CLKDIV4_RGREFDIV_DIV(1));
    SET_VAR_FIELD(clkdiv4_L  , CLKDIV4_JTM2CKDV,   CLKDIV4_JTM2CKDV_DIV(1));
    SET_VAR_FIELD(clkdiv4_L  , CLKDIV4_RCPREFDIV,  CLKDIV4_RCPREFDIV_DIV(10));
    SET_VAR_FIELD(clkdiv4_L  , CLKDIV4_JTM1CKDV,   CLKDIV4_JTM1CKDV_DIV(1));

    SET_VAR_FIELD(thrtl_clk_L, THRTL_CNT_TH_DIV,   THRTL_CNT_TH_DIV_DIV(1)     );

    switch (strp)
    {
        case 0:
        /* DDR = MCBPCK  CPU = FT    AXI = FT/2  AHB = FT/4 AHB3 = FT/4 CLK_SPI0 = FT/4  CLK_APB5 = FT/4   CLK_APB2,3 = FT/4    CLK_APB1,4 = FT/4   PCI = PLL0/5  GFX = PLL0/4 */

            /* clk selection is for this bypass: */
            SET_VAR_FIELD(clksel_L, CLKSEL_CPUCKSEL, CLKSEL_CPUCKSEL_SYSBPCK);
            SET_VAR_FIELD(clksel_L, CLKSEL_MCCKSEL,  CLKSEL_MCCKSEL_MCBPCK);
            SET_VAR_FIELD(clksel_L, CLKSEL_PIXCKSEL, CLKSEL_PIXCKSEL_CLKOUT_GPIO160);

            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_GFXCKDIV,  CLKDIV2_GFXCKDIV_DIV(1));
            SET_VAR_FIELD(clkdiv1_L, CLKDIV1_PCICKDIV,  CLKDIV1_PCICKDIV_DIV(1));
            SET_VAR_FIELD(clkdiv1_L, CLKDIV1_MMCCKDIV,  CLKDIV1_MMCCKDIV_DIV(1));

            SET_VAR_FIELD(clkdiv3_L  , CLKDIV3_SPI0CKDIV,  CLKDIV3_SPI0CKDIV_DIV(5));
            SET_VAR_FIELD(clkdiv4_L  , CLKDIV4_RCPREFDIV,  CLKDIV4_RCPREFDIV_DIV(1));

            break;

        case 1:
        /* DDR = 25      CPU = 25    AXI = 12.5  AHB = 6.25 AHB3 = 3.125    CLK_SPI0 = 6.25  CLK_APB5 = 3.125  CLK_APB2,3 = 1.5625  CLK_APB1,4 = 1.5625 PCI = 4.167   GFX = 5 */
            ASSERT(0);

            break;

        case 2:
        /* DDR = 667     CPU = 500   AXI = 250   AHB = 125  AHB3 = 17.85    CLK_SPI0 = 17.85 CLK_APB5 = 62.5   CLK_APB2,3 = 62.5    CLK_APB1,4 = 31.25  PCI = 137.14  GFX = 160 */
            pllcon0_L  = CLK_500MHZ_PLLCON0_2_REG_CFG;
            pllcon1_L  = CLK_666MHZ_PLLCON1_REG_CFG;
            pllcon2_L  = CLK_960MHZ_PLLCON2_REG_CFG;

            SET_VAR_FIELD(clksel_L, CLKSEL_CPUCKSEL,     CLKSEL_CPUCKSEL_PLL0);
            SET_VAR_FIELD(clksel_L, CLKSEL_MCCKSEL,      CLKSEL_MCCKSEL_PLL1);
            SET_VAR_FIELD(clksel_L, CLKSEL_RCPCKSEL    , CLKSEL_RCPCKSEL_PLL0);
            SET_VAR_FIELD(clksel_L, CLKSEL_RGSEL       , CLKSEL_RGSEL_PLL0);
            SET_VAR_FIELD(clksel_L, CLKSEL_AHB6SSEL    , CLKSEL_AHB6SSEL_PLL2);
            SET_VAR_FIELD(clksel_L, CLKSEL_GFXMSEL     , CLKSEL_GFXMSEL_PLL2);
            SET_VAR_FIELD(clksel_L, CLKSEL_SDCKSEL     , CLKSEL_SDCKSEL_PLL0);
            SET_VAR_FIELD(clksel_L, CLKSEL_PCIGFXCKSEL,  CLKSEL_PCIGFXCKSEL_PLL0);
            SET_VAR_FIELD(clksel_L, CLKSEL_SUCKSEL     , CLKSEL_SUCKSEL_PLL2);

            SET_VAR_FIELD(clkdiv1_L, CLKDIV1_CLK4DIV,    CLKDIV1_CLK4DIV_DIV(2));
            SET_VAR_FIELD(clkdiv3_L, CLKDIV3_SPI0CKDIV,  CLKDIV3_SPI0CKDIV_DIV(7));
            SET_VAR_FIELD(clkdiv3_L, CLKDIV3_SPI1CKDIV,  CLKDIV3_SPI1CKDIV_DIV(7));
            SET_VAR_FIELD(clkdiv1_L, CLKDIV1_AHB3CKDIV,  CLKDIV1_AHB3CKDIV_DIV(7)); // SPI3
            SET_VAR_FIELD(clkdiv3_L, CLKDIV3_SPIXCKDIV,  CLKDIV3_SPIXCKDIV_DIV(7));

            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_APB4CKDIV,  CLKDIV2_APBxCKDIV_DIV(4));
            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_APB3CKDIV,  CLKDIV2_APBxCKDIV_DIV(2));
            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_APB2CKDIV,  CLKDIV2_APBxCKDIV_DIV(2));
            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_APB1CKDIV,  CLKDIV2_APBxCKDIV_DIV(3));
            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_APB5CKDIV,  CLKDIV2_APBxCKDIV_DIV(2));
            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_SUCKDIV,    CLKDIV2_SUCKDIV_DIV(16));
            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_SU48CKDIV,  CLKDIV2_SU48CKDIV_DIV(10));

            SET_VAR_FIELD(clkdiv4_L, CLKDIV4_RGREFDIV,   CLKDIV4_RGREFDIV_DIV(4)); // GMAC should be 125MHz
            SET_VAR_FIELD(clkdiv4_L, CLKDIV4_RCPREFDIV,  CLKDIV4_RCPREFDIV_DIV(5));

            SET_VAR_FIELD(clkdiv1_L, CLKDIV1_MMCCKDIV,   CLKDIV1_MMCCKDIV_DIV(20));
            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_GFXCKDIV ,  CLKDIV2_GFXCKDIV_DIV(3));
            SET_VAR_FIELD(clkdiv1_L, CLKDIV1_PCICKDIV,   CLKDIV1_PCICKDIV_DIV(4));

            break;

        case 3:
        /* DDR = 1062.5  CPU = 1000  AXI = 500   AHB = 250  AHB3 = 19.2 CLK_SPI0 = 19.2  CLK_APB5 = 62.5   CLK_APB2,3 = 62.5    CLK_APB1,4 = 31.25  PCI = 1   GFX = 37.14 */

            pllcon0_L  = CLK_1000MHZ_PLLCON0_2_REG_CFG;
            pllcon1_L  = CLK_1062_5_MHZ_PLLCON1_REG_CFG;
            pllcon2_L  = CLK_960MHZ_PLLCON2_REG_CFG;

            /* clk selection is for this bypass: */
            SET_VAR_FIELD(clksel_L, CLKSEL_CPUCKSEL, CLKSEL_CPUCKSEL_PLL0);
            SET_VAR_FIELD(clksel_L, CLKSEL_MCCKSEL,  CLKSEL_MCCKSEL_PLL1);

            SET_VAR_FIELD(clksel_L, CLKSEL_RGSEL       , CLKSEL_RGSEL_PLL0       );
            SET_VAR_FIELD(clksel_L, CLKSEL_RCPCKSEL    , CLKSEL_RCPCKSEL_PLL0       );
            SET_VAR_FIELD(clksel_L, CLKSEL_AHB6SSEL    , CLKSEL_AHB6SSEL_PLL2    );
            SET_VAR_FIELD(clksel_L, CLKSEL_GFXMSEL     , CLKSEL_GFXMSEL_PLL2     );
            SET_VAR_FIELD(clksel_L, CLKSEL_SDCKSEL     , CLKSEL_SDCKSEL_PLL0);
            SET_VAR_FIELD(clksel_L, CLKSEL_PCIGFXCKSEL,  CLKSEL_PCIGFXCKSEL_PLL0);
            SET_VAR_FIELD(clksel_L, CLKSEL_SUCKSEL     , CLKSEL_SUCKSEL_PLL2);

            /* clk dividers */
            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_CLKOUTDIV,  CLKDIV2_CLKOUTDIV_DIV(16));
            SET_VAR_FIELD(clkdiv4_L, CLKDIV4_RGREFDIV,   CLKDIV4_RGREFDIV_DIV(8)); // GMAC should be 125MHz
            SET_VAR_FIELD(clkdiv4_L, CLKDIV4_JTM2CKDV,   CLKDIV4_JTM2CKDV_DIV(1));
            SET_VAR_FIELD(clkdiv4_L, CLKDIV4_RCPREFDIV,  CLKDIV4_RCPREFDIV_DIV(10));
            SET_VAR_FIELD(clkdiv4_L, CLKDIV4_JTM1CKDV,   CLKDIV4_JTM1CKDV_DIV(1));

            SET_VAR_FIELD(clkdiv1_L, CLKDIV1_CLK4DIV,    CLKDIV1_CLK4DIV_DIV(2));
            SET_VAR_FIELD(clkdiv3_L, CLKDIV3_SPI0CKDIV,  CLKDIV3_SPI0CKDIV_DIV(13));
            SET_VAR_FIELD(clkdiv3_L, CLKDIV3_SPI1CKDIV,  CLKDIV3_SPI1CKDIV_DIV(13));
            SET_VAR_FIELD(clkdiv1_L, CLKDIV1_AHB3CKDIV,  CLKDIV1_AHB3CKDIV_DIV(13)); // SPI3
            SET_VAR_FIELD(clkdiv3_L, CLKDIV3_SPIXCKDIV,  CLKDIV3_SPIXCKDIV_DIV(13));

            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_APB4CKDIV,  CLKDIV2_APBxCKDIV_DIV(8));
            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_APB3CKDIV,  CLKDIV2_APBxCKDIV_DIV(4));
            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_APB2CKDIV,  CLKDIV2_APBxCKDIV_DIV(4));
            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_APB1CKDIV,  CLKDIV2_APBxCKDIV_DIV(7));
            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_APB5CKDIV,  CLKDIV2_APBxCKDIV_DIV(4));
            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_SUCKDIV,    CLKDIV2_SUCKDIV_DIV(16));
            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_SU48CKDIV,  CLKDIV2_SU48CKDIV_DIV(10));

            SET_VAR_FIELD(clkdiv1_L, CLKDIV1_MMCCKDIV,   CLKDIV1_MMCCKDIV_DIV(20));
            SET_VAR_FIELD(clkdiv2_L, CLKDIV2_GFXCKDIV ,  CLKDIV2_GFXCKDIV_DIV(6));
            SET_VAR_FIELD(clkdiv1_L, CLKDIV1_PCICKDIV,   CLKDIV1_PCICKDIV_DIV(8));

            break;

        default:
            ASSERT(0);
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* Switch clock sources to external clock for all muxes (while initing the PLLs)                       */
    /*-----------------------------------------------------------------------------------------------------*/
    // Switch the clock source to another PLL or external clock (25 MHz)
    REG_WRITE(CLKSEL, clksel_clkref_L);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Change PLL configuration (leave PLL in reset mode):                                                 */
    /*-----------------------------------------------------------------------------------------------------*/
    if ((strp != 0) && (strp != 5)) // in test mode and fast skip init PLLs are not used
    {
        // Set PWDEN bit
        SET_VAR_FIELD(pllcon0_L, PLLCONn_PWDEN, PLLCONn_PWDEN_POWER_DOWN);
        SET_VAR_FIELD(pllcon1_L, PLLCONn_PWDEN, PLLCONn_PWDEN_POWER_DOWN);
        SET_VAR_FIELD(pllcon2_L, PLLCONn_PWDEN, PLLCONn_PWDEN_POWER_DOWN);

        /* set to power down == 1 */
        REG_WRITE(PLLCON0, pllcon0_L);
        REG_WRITE(PLLCON1, pllcon1_L);
        REG_WRITE(PLLCON2, pllcon2_L);

        /*--------------------------------------------------------------------------------------------*/
        /* Wait 2ms for PLL:                                                                          */
        /*--------------------------------------------------------------------------------------------*/
        CLK_Delay_MicroSec(10);

        /*--------------------------------------------------------------------------------------------*/
        /* Enable PLLs after configuring. Delays to prevent power issues.                             */
        /*--------------------------------------------------------------------------------------------*/
        SET_REG_FIELD(PLLCON0, PLLCONn_PWDEN, PLLCONn_PWDEN_NORMAL);
        CLK_Delay_MicroSec(10);
        SET_REG_FIELD(PLLCON1, PLLCONn_PWDEN, PLLCONn_PWDEN_NORMAL);
        CLK_Delay_MicroSec(10);
        SET_REG_FIELD(PLLCON2, PLLCONn_PWDEN, PLLCONn_PWDEN_NORMAL);
        CLK_Delay_MicroSec(10);

        /*--------------------------------------------------------------------------------------------*/
        /* Wait 750usec for PLL to stabilize:                                                         */
        /*--------------------------------------------------------------------------------------------*/
        WaitForPllLock(PLL0);
        WaitForPllLock(PLL1);
        WaitForPllLock(PLL2);
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* Change CLKDIV configuration:                                                                        */
    /*-----------------------------------------------------------------------------------------------------*/
    REG_WRITE(CLKDIV1, clkdiv1_L);
    REG_WRITE(CLKDIV2, clkdiv2_L);
    REG_WRITE(CLKDIV3, clkdiv3_L);
    REG_WRITE(CLKDIV4, clkdiv4_L);
    REG_WRITE(THRTL_CNT, thrtl_clk_L);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Wait for 200 clock cycles between clkDiv change and clkSel change:                                  */
    /*-----------------------------------------------------------------------------------------------------*/
    CLK_Delay_MicroSec(400);

    /*----------------------------------------------------------------------------------------------------*/
    /* Change clock source according to configuration:                                                     */
    /*-----------------------------------------------------------------------------------------------------*/
    /*-----------------------------------------------------------------------------------------------------*/
    /* in order to avoid power surge clksel is set gradualy, according to this schema:                     */
    /* [ brackets] mean do it in the same step                                                             */
    /*                                                                                                     */
    /* MC, [DVC,   CPU], GFXMSEL, GPRFSEL,  GFXCKSEL, [PIXCKSEL, CLKOUTSEL, SUCKSEL, TIMCKSEL, UARTCKSEL,  */
    /* SDCKSEL]                                                                                            */
    /*-----------------------------------------------------------------------------------------------------*/

    SET_REG_FIELD(CLKSEL, CLKSEL_MCCKSEL     , READ_VAR_FIELD(clksel_L, CLKSEL_MCCKSEL )     );
    CLK_Delay_MicroSec(20);
    SET_REG_FIELD(CLKSEL, CLKSEL_CPUCKSEL    , READ_VAR_FIELD(clksel_L, CLKSEL_CPUCKSEL)     );
    CLK_Delay_MicroSec(20);
    SET_REG_FIELD(CLKSEL, CLKSEL_AHB6SSEL    , READ_VAR_FIELD(clksel_L, CLKSEL_AHB6SSEL)     );
    CLK_Delay_MicroSec(20);
    SET_REG_FIELD(CLKSEL, CLKSEL_GFXMSEL     , READ_VAR_FIELD(clksel_L, CLKSEL_GFXMSEL )     );
    CLK_Delay_MicroSec(20);
    SET_REG_FIELD(CLKSEL, CLKSEL_PCIGFXCKSEL , READ_VAR_FIELD(clksel_L, CLKSEL_PCIGFXCKSEL ) );
    CLK_Delay_MicroSec(20);
    SET_REG_FIELD(CLKSEL, CLKSEL_PIXCKSEL    , READ_VAR_FIELD(clksel_L, CLKSEL_PIXCKSEL )    );
    CLK_Delay_MicroSec(20);
    SET_REG_FIELD(CLKSEL, CLKSEL_RCPCKSEL    , READ_VAR_FIELD(clksel_L, CLKSEL_RCPCKSEL  )   );
    CLK_Delay_MicroSec(20);
    SET_REG_FIELD(CLKSEL, CLKSEL_RGSEL       , READ_VAR_FIELD(clksel_L, CLKSEL_RGSEL  )      );
    CLK_Delay_MicroSec(20);
    SET_REG_FIELD(CLKSEL, CLKSEL_CLKOUTSEL   , READ_VAR_FIELD(clksel_L, CLKSEL_CLKOUTSEL )   );
    CLK_Delay_MicroSec(20);
    SET_REG_FIELD(CLKSEL, CLKSEL_ADCCKSEL    , READ_VAR_FIELD(clksel_L, CLKSEL_ADCCKSEL)     );
    CLK_Delay_MicroSec(20);
    SET_REG_FIELD(CLKSEL, CLKSEL_SUCKSEL     , READ_VAR_FIELD(clksel_L, CLKSEL_SUCKSEL  )    );
    CLK_Delay_MicroSec(20);
    //SET_REG_FIELD(CLKSEL, CLKSEL_UARTCKSEL   , READ_VAR_FIELD(clksel_L, CLKSEL_UARTCKSEL)    );
    //CLK_Delay_MicroSec(20);
    SET_REG_FIELD(CLKSEL, CLKSEL_SDCKSEL     , READ_VAR_FIELD(clksel_L, CLKSEL_SDCKSEL  )    );
    CLK_Delay_MicroSec(20);

    CLK_ConfigureADCClock(ADC_CLK);

#if defined (FUSE_MODULE_TYPE)
    FUSE_Init(); /* Set APBRT to CLK_APB4 frequency. */
#endif

    return;
}

#if defined (UART_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ConfigureUartClock                                                                 */
/*                                                                                                         */
/* Parameters:      freq - freq of the source clock                                                        */
/*                  uartDiv - divisor for uart                                                             */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures the Uart clock source to CLKREF (25MHZ), devider will be 7 so  */
/*                  the uart clock will be 25Mhz/7 = 3.57Mhz, which is lower the default AHB clock.        */
/*                  Configuring baudrate of 115200, will give actual baudrate of 111600 (3.32% off)        */
/*                                                                                                         */
/*                  For PD optimization, the uart divisor and the uart clk (which is later been used to    */
/*                  calculate the baudrate divisor) will always be set to minimum (1 and 0 accordingly)    */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS CLK_ConfigureUartClock (UINT32 freq, UINT32 uartDiv)
{
    /*-------------------------------------------------------------------------------------------------*/
    /* Set divider                                                                                     */
    /*-------------------------------------------------------------------------------------------------*/
    SET_REG_FIELD(CLKDIV1, CLKDIV1_UARTDIV1, CLKDIV1_UARTDIV1_DIV(uartDiv));
    SET_REG_FIELD(CLKDIV3, CLKDIV3_UARTDIV2, CLKDIV3_UARTDIV2_DIV(uartDiv));

    /*-------------------------------------------------------------------------------------------------*/
    /* Choose CLKREF as a source:                                                                      */
    /*-------------------------------------------------------------------------------------------------*/
    if (freq == EXT_CLOCK_FREQUENCY_HZ)
    {
        SET_REG_FIELD(CLKSEL, CLKSEL_UARTCKSEL, CLKSEL_UARTCKSEL_CLKREF);
    }
    else
    {
        SET_REG_FIELD(CLKSEL, CLKSEL_UARTCKSEL, CLKSEL_UARTCKSEL_PLL2);
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* Wait for 200 clock cycles between clkDiv change and clkSel change, for clockref it 8us              */
    /*-----------------------------------------------------------------------------------------------------*/
    CLK_Delay_MicroSec(20);

    //for( uart = UART0_DEV; uart <= UART7_DEV; uart++)
    //  UART_ResetFIFOs(uart, TRUE, TRUE);

    return DEFS_STATUS_OK;
}
#endif // defined (UART_MODULE_TYPE)

#if defined (USB_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ConfigureUSBClock                                                                  */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Configures the USB clock to 60MHz by checking which PLL is                             */
/*                  dividable by 60 and setting correct SEL and DIV values.                                */
/*                  Assumes that one of the PLLs is dividable by 60                                        */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS CLK_ConfigureUSBClock (void)
{
    UINT32  choosenPllFreq;
    UINT32  divider;

    choosenPllFreq = EXT_CLOCK_FREQUENCY_HZ;
    // Find a PLL that can create an exact 48MHz value:
    if (CLK_GetPll2Freq() % (SU48_DESIRED_FREQUENCY *_1MHz_) == 0)
    {
        choosenPllFreq = CLK_GetPll2Freq();
        SET_REG_FIELD(CLKSEL, CLKSEL_SUCKSEL, CLKSEL_SUCKSEL_PLL2);
    }
    else if (CLK_GetPll0Freq() % (SU48_DESIRED_FREQUENCY *_1MHz_) == 0)
    {
        choosenPllFreq = CLK_GetPll0Freq();
        SET_REG_FIELD(CLKSEL, CLKSEL_SUCKSEL, CLKSEL_SUCKSEL_PLL0);
    }
    else if (CLK_GetPll1Freq() % (SU48_DESIRED_FREQUENCY *_1MHz_) == 0)
    {
        choosenPllFreq = CLK_GetPll1Freq();
        SET_REG_FIELD(CLKSEL, CLKSEL_SUCKSEL, CLKSEL_SUCKSEL_PLL1);
    }
    else
    {
        return DEFS_STATUS_INVALID_PARAMETER;
    }

    CLK_Delay_MicroSec(20);

    // Set OHCI divider
    divider = choosenPllFreq / (SU48_DESIRED_FREQUENCY *_1MHz_);
    SET_REG_FIELD(CLKDIV2, CLKDIV2_SU48CKDIV, CLKDIV2_SU48CKDIV_DIV(divider));
    CLK_Delay_MicroSec(20);

    // Set UTMI divider
    divider = choosenPllFreq / (SU_DESIRED_FREQUENCY *_1MHz_);
    SET_REG_FIELD(CLKDIV2, CLKDIV2_SUCKDIV, CLKDIV2_SUCKDIV_DIV(divider));
    CLK_Delay_MicroSec(20);

    return DEFS_STATUS_OK;
}
#endif //  defined (USB_MODULE_TYPE)

#if 0
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ConfigureAPBClock                                                                  */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         APB freq in Hz                                                                         */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Configures APB divider to 2, return the frequency of APB in Hz                         */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_ConfigureAPBClock (APB_CLK apb)
{
}
#endif

#if defined (TMC_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ConfigureTimerClock                                                                */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs Timer clock configuration                                        */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_ConfigureTimerClock (void)
{
    SET_REG_FIELD(CLKEN1, CLKEN1_TIMER0_4, 1);
    SET_REG_FIELD(CLKEN1, CLKEN1_TIMER5_9, 1);
    SET_REG_FIELD(CLKEN3, CLKEN3_TIMER10_14, 1);

    // Note: In Arbel there is no divider and selector for timer clock. It comes only from CLKREF.

    /*-----------------------------------------------------------------------------------------------------*/
    /* Returning the clock frequency                                                                       */
    /*-----------------------------------------------------------------------------------------------------*/
    return EXT_CLOCK_FREQUENCY_MHZ * _1MHz_;
}
#endif // defined (TMC_MODULE_TYPE)

#if defined (EMC_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ConfigureEMCClock                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ethNum -  ethernet module number                                                       */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures EMC clocks                                                     */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_ConfigureEMCClock (UINT32 ethNum)
{
}
#endif // #if defined (EMC_MODULE_TYPE)

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ConfigureGMACClock                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ethNum -  ethernet module number                                                       */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures GMAC clocks                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_ConfigureGMACClock (UINT32 ethNum)
{
    UINT32 source = 0;
	UINT32 div = 0;
    UINT32 sel =  READ_REG_FIELD(CLKSEL, CLKSEL_RGSEL);

    switch (sel)
    {
    case CLKSEL_RGSEL_PLL0:
        source = CLK_GetPll0Freq();
        break;
    case CLKSEL_RGSEL_PLL1:
        source = CLK_GetPll1Freq();
        break;
    case CLKSEL_RGSEL_CLKREF:
        source = EXT_CLOCK_FREQUENCY_HZ;
        break;
    default:
        ASSERT(0);
    }
    div = source / (125 * _1MHz_); // GMAC should be 125MHz always.

    SET_REG_FIELD(CLKDIV4, CLKDIV4_RGREFDIV, CLKDIV4_RGREFDIV_DIV(div)); 
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ConfigureRootComplexClock                                                          */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine configures RC (root complex) clocks                                       */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_ConfigureRootComplexClock (void)
{
    UINT32 source = 0;
	UINT32 div = 0;
    UINT32 sel =  READ_REG_FIELD(CLKSEL, CLKSEL_RCPCKSEL);

    switch (sel)
    {
    case CLKSEL_RGSEL_PLL0:
        source = CLK_GetPll0Freq();
        break;
    case CLKSEL_RGSEL_PLL1:
        source = CLK_GetPll1Freq();
        break;
    case CLKSEL_RGSEL_CLKREF:
        source = EXT_CLOCK_FREQUENCY_HZ;
        break;
    default:
        ASSERT(0);
    }
    div = source / (100 * _1MHz_); // RC should be 100MHz always.

    SET_REG_FIELD(CLKDIV4, CLKDIV4_RCPREFDIV, CLKDIV4_RCPREFDIV_DIV(div)); 
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ConfigureSDClock                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  sdNum -  SD module number                                                              */
/*                                                                                                         */
/* Returns:         SD clock frequency                                                                     */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Configures the SD clock to frequency closest to the target clock from beneath          */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_ConfigureSDClock (UINT32 sdNum)
{
    UINT32  divider;
    UINT32  choosenPllFreq;   //In Hz

    /*-----------------------------------------------------------------------------------------------------*/
    /* Parameters check                                                                                    */
    /*-----------------------------------------------------------------------------------------------------*/
    if (sdNum >= SD_NUM_OF_MODULES)
    {
        return DEFS_STATUS_PARAMETER_OUT_OF_RANGE;
    }
    // Find a PLL that can create an exact 48MHz value:
    if (CLK_GetPll0Freq() % (SD_CLK_TARGET_FREQ) == 0)
    {
        choosenPllFreq = CLK_GetPll0Freq();
        SET_REG_FIELD(CLKSEL, CLKSEL_SDCKSEL, CLKSEL_SDCKSEL_PLL0);
    }
    else if (CLK_GetPll2Freq() % (SD_CLK_TARGET_FREQ) == 0)
    {
        choosenPllFreq = CLK_GetPll2Freq();
        SET_REG_FIELD(CLKSEL, CLKSEL_SDCKSEL, CLKSEL_SDCKSEL_PLL2);
    }
    else if (CLK_GetPll1Freq() % (SD_CLK_TARGET_FREQ) == 0)
    {
        choosenPllFreq = CLK_GetPll1Freq();
        SET_REG_FIELD(CLKSEL, CLKSEL_SDCKSEL, CLKSEL_SDCKSEL_PLL1);
    }
    else
    {
        choosenPllFreq = EXT_CLOCK_FREQUENCY_HZ;
        SET_REG_FIELD(CLKSEL, CLKSEL_SDCKSEL, CLKSEL_SDCKSEL_CLKREF);
    }

    CLK_Delay_MicroSec(200);

    divider = choosenPllFreq / SD_CLK_TARGET_FREQ;

    // can't get to 50MHz from clkref:
    if (divider == 0)
    {
        divider = 1;
    }

    SET_REG_FIELD(CLKDIV1, CLKDIV1_MMCCKDIV, CLKDIV1_MMCCKDIV_DIV(divider));
    SET_REG_FIELD(CLKDIV2, CLKDIV2_SD1CKDIV, CLKDIV2_SD1CKDIV_DIV(divider+1));

    /*-----------------------------------------------------------------------------------------------------*/
    /* Wait to the divider to stabilize (according to spec)                                                */
    /*-----------------------------------------------------------------------------------------------------*/
    CLK_Delay_MicroSec(200);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Configure Clock Enable Register : enabled by default                                                */
    /*-----------------------------------------------------------------------------------------------------*/
    // SET_REG_FIELD(CLKEN2, CLKEN2_MMC, 1);

    return SD_CLK_TARGET_FREQ;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ConfigureADCClock                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  adc_freq -  adc frequency in Hz.                                                       */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine sets ADC clock in Hzs                                                     */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_ConfigureADCClock (_UNUSED_ UINT32 adc_freq)
{
    // UINT32  adc_div = DIV_ROUND(CLK_GetADCFreq() , adc_freq);

    // Note: two dividers to get to ADC clock from selector.
    // Normaly requires 25MHz. Use CLKREF and no dividers:

    SET_REG_FIELD(CLKSEL, CLKSEL_ADCCKSEL, CLKSEL_ADCCKSEL_CLKREF);
    CLK_Delay_MicroSec(200);

    SET_REG_FIELD(CLKDIV1, CLKDIV1_PRE_ADCCKDIV, CLKDIV1_PRE_ADCCKDIV_DIV(1));
    CLK_Delay_MicroSec(200);

    SET_REG_FIELD(CLKDIV1, CLKDIV1_ADCCKDIV, CLKDIV1_ADCCKDIV_DIV(1));
    CLK_Delay_MicroSec(200);

    return;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetADCClock                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                                                                                                         */
/* Returns:          adc frequency in Hz.                                                                  */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine gets ADC clock in Hz                                                      */
/*---------------------------------------------------------------------------------------------------------*/
int CLK_GetADCClock (void)
{
    UINT32  FOUT        = 0;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Get ADC source clock:                                                                               */
    /*-----------------------------------------------------------------------------------------------------*/
    if (READ_REG_FIELD(CLKSEL, CLKSEL_ADCCKSEL) == CLKSEL_ADCCKSEL_PLL0)
    {
        FOUT = CLK_GetPll0Freq();
    }
    else if (READ_REG_FIELD(CLKSEL, CLKSEL_ADCCKSEL) == CLKSEL_ADCCKSEL_PLL1)
    {
        FOUT = CLK_GetPll1Freq();
    }
    else if (READ_REG_FIELD(CLKSEL, CLKSEL_ADCCKSEL) == CLKSEL_ADCCKSEL_PLL2)
    {
        FOUT = CLK_GetPll2Freq();
    }
    else
    {
        /*-------------------------------------------------------------------------------------------------*/
        /* External clock, assume low freq ref clock (25MHz):                                              */
        /*-------------------------------------------------------------------------------------------------*/
        FOUT = EXT_CLOCK_FREQUENCY_HZ; //FOUT is specified in MHz
    }

    FOUT =  FOUT /
        (1 + READ_REG_FIELD(CLKDIV1, CLKDIV1_PRE_ADCCKDIV))  /
        (1 << READ_REG_FIELD(CLKDIV1, CLKDIV1_ADCCKDIV));

    /*-----------------------------------------------------------------------------------------------------*/
    /* Returing value in Hertz                                                                             */
    /*-----------------------------------------------------------------------------------------------------*/
    return FOUT;
}

#if defined (EMC_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ResetEMC                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  deviceNum -                                                                            */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs SW reset of EMC module                                           */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_ResetEMC (UINT32 deviceNum)
{
    if (deviceNum == 0)
    {
        SET_REG_FIELD(IPSRST1, IPSRST1_EMC1, 1);
        SET_REG_FIELD(IPSRST1, IPSRST1_EMC1, 0);
    }
    else
    {
        SET_REG_FIELD(IPSRST1, IPSRST1_EMC2, 1);
        SET_REG_FIELD(IPSRST1, IPSRST1_EMC2, 0);
    }
}
#endif // #if defined (EMC_MODULE_TYPE)

#if defined (GMAC_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ResetGMAC                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  deviceNum -                                                                            */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs SW reset of GMAC                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_ResetGMAC (UINT32 deviceNum)
{
    if (deviceNum == 2)
    {
        SET_REG_FIELD(IPSRST2, IPSRST2_GMAC1, 1);
        SET_REG_FIELD(IPSRST2, IPSRST2_GMAC1, 0);
    }
    else if (deviceNum == 3)
    {
        SET_REG_FIELD(IPSRST2, IPSRST2_GMAC2, 1);
        SET_REG_FIELD(IPSRST2, IPSRST2_GMAC2, 0);
    }
}
#endif // #if defined (GMAC_MODULE_TYPE)

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetTimeStamp                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         Current time stamp                                                                     */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_GetTimeStamp (UINT32 time_quad[2])
{
    UINT32 Seconds;
    UINT32 RefClocks;

    do
    {
        Seconds = REG_READ(SECCNT);
        RefClocks = REG_READ(CNTR25M);
    } while (REG_READ(SECCNT) != Seconds);

    time_quad[0] = RefClocks;
    time_quad[1] = Seconds;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetUpTimeMiliseconds                                                               */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         Uptime in ms                                                                           */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
UINT64 CLK_GetUpTimeMiliseconds (void)
{
    UINT32 time_quad[2] __attribute__ ((aligned (8)));

    CLK_GetTimeStamp(time_quad);

    return (UINT64)((UINT32)time_quad[1]*1000 + (UINT32)DIV_ROUND(time_quad[0], 25000) );


}


#if defined (FIU_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ResetFIU                                                                           */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs SW reset of FIU                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_ResetFIU (FIU_MODULE_T fiu)
{
    if (fiu == FIU_MODULE_0)
    {
        SET_REG_FIELD(IPSRST1, IPSRST1_SPI0, 1);
        SET_REG_FIELD(IPSRST1, IPSRST1_SPI0, 0);
    }

    if (fiu == FIU_MODULE_1)
    {
        SET_REG_FIELD(IPSRST2, IPSRST2_SPI1, 1);
        SET_REG_FIELD(IPSRST2, IPSRST2_SPI1, 0);
    }

    if ((fiu == FIU_MODULE_3) || (fiu == FIU_MODULE_2))
    {
        SET_REG_FIELD(IPSRST1, IPSRST1_SPI3, 1);
        SET_REG_FIELD(IPSRST1, IPSRST1_SPI3, 0);
    }

    if (fiu == FIU_MODULE_X)
    {
        SET_REG_FIELD(IPSRST3, IPSRST3_SPIX, 1);

        CLK_Delay_MicroSec(5);
        SET_REG_FIELD(IPSRST3, IPSRST3_SPIX, 0);

        CLK_Delay_MicroSec(5);
    }
}
#endif //#if defined (FIU_MODULE_TYPE)

#if defined (UART_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ResetUART                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs SW reset of all UARTs. 0 and 1 have shared reset. 2 and 3 too.   */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_ResetUART (void)
{
     SET_REG_FIELD(IPSRST1, IPSRST1_UART01, 1);
     SET_REG_FIELD(IPSRST1, IPSRST1_UART01, 0);

     SET_REG_FIELD(IPSRST1, IPSRST1_UART23, 1);
     SET_REG_FIELD(IPSRST1, IPSRST1_UART23, 0);
}
#endif // #if defined (UART_MODULE_TYPE)

#if defined (AES_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ResetAES                                                                           */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs SW reset of AES                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_ResetAES (void)
{
    SET_REG_FIELD(IPSRST1, IPSRST1_AES, 1);
    SET_REG_FIELD(IPSRST1, IPSRST1_AES, 0);
}
#endif //if defined (AES_MODULE_TYPE)

#if defined (MC_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ResetMC                                                                            */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs SW reset of MC                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_ResetMC (void)
{
    HAL_PRINT("\n>MC reset \n");

    SET_REG_FIELD(IPSRST1, IPSRST1_MC, 1);
    CLK_Delay_MicroSec(10000);
    SET_REG_FIELD(IPSRST1, IPSRST1_MC, 0);
    CLK_Delay_MicroSec(10000);

    /* Force re-training of DDR (because DDR module is reinitialized*/
    SET_REG_FIELD(INTCR2, INTCR2_MC_INIT, 0);
}
#endif // #if defined (MC_MODULE_TYPE)

#if defined (TMC_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ResetTIMER                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  deviceNum -                                                                            */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs SW reset of Timer                                                */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_ResetTIMER (UINT32 deviceNum)
{
    if (deviceNum <= 4)
    {
        SET_REG_FIELD(IPSRST1, IPSRST1_TIM0_4, 1);
        SET_REG_FIELD(IPSRST1, IPSRST1_TIM0_4, 0);
    }
    else if (deviceNum <= 9)
    {
        SET_REG_FIELD(IPSRST1, IPSRST1_TIM5_9, 1);
        SET_REG_FIELD(IPSRST1, IPSRST1_TIM5_9, 0);
    }
    else
    {
        SET_REG_FIELD(IPSRST3, IPSRST3_TIMER10_14, 1);
        SET_REG_FIELD(IPSRST3, IPSRST3_TIMER10_14, 0);
    }
}
#endif // #if defined (TMC_MODULE_TYPE)

#if defined (SD_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ResetSD                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  sdNum -  SD module number                                                              */
/*                                                                                                         */
/* Returns:         DEFS_STATUS Error code                                                                 */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs SW reset of SD                                                   */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS CLK_ResetSD (UINT32 sdNum)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* Parameters check                                                                                    */
    /*-----------------------------------------------------------------------------------------------------*/
    if (sdNum >= SD_NUM_OF_MODULES)
    {
        return DEFS_STATUS_PARAMETER_OUT_OF_RANGE;
    }

    if (sdNum == SD1_DEV)
    {
        SET_REG_FIELD(IPSRST2, IPSRST2_SDHC, 1);
        SET_REG_FIELD(IPSRST2, IPSRST2_SDHC, 0);
    }
    else if (sdNum == SD2_DEV)
    {
        SET_REG_FIELD(IPSRST2, IPSRST2_SDHC, 1);
        SET_REG_FIELD(IPSRST2, IPSRST2_SDHC, 0);
    }

    return DEFS_STATUS_OK;
}
#endif // #if defined (SD_MODULE_TYPE)

#if defined (PSPI_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ResetPSPI                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  deviceNum -  PSPI module number                                                        */
/*                                                                                                         */
/* Returns:         DEFS_STATUS Error code                                                                 */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs SW reset of PSPI                                                 */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS CLK_ResetPSPI (UINT32 deviceNum)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* Parameters check                                                                                    */
    /*-----------------------------------------------------------------------------------------------------*/
    if (deviceNum >= PSPI_NUM_OF_MODULES)
    {
        return HAL_ERROR_BAD_DEVNUM;
    }

    if (deviceNum == PSPI1_DEV)
    {
        SET_REG_FIELD(IPSRST2, IPSRST2_PSPI1, 1);
        SET_REG_FIELD(IPSRST2, IPSRST2_PSPI1, 0);
    }
    else if (deviceNum == PSPI2_DEV)
    {
        SET_REG_FIELD(IPSRST2, IPSRST2_PSPI2, 1);
        SET_REG_FIELD(IPSRST2, IPSRST2_PSPI2, 0);
    }

    return DEFS_STATUS_OK;
}
#endif // #if defined (PSPI_MODULE_TYPE)

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetPLL0toAPBdivisor                                                                */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine returns the value achieved by dividing PLL0 frequency to APB frequency    */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetPLL0toAPBdivisor (APB_CLK apb)
{
    UINT32 apb_divisor = 1;

    apb_divisor = apb_divisor * 2;       // Pre Clock divider (div by 1\2)
    apb_divisor = apb_divisor * (READ_REG_FIELD(CLKDIV1, CLKDIV1_CLK4DIV) + 1);       // AHBn divider (div by 1\2\3\4)

    switch (apb)
    {
        case CLK_APB1:
            apb_divisor = apb_divisor * (1 << READ_REG_FIELD(CLKDIV2, CLKDIV2_APB1CKDIV));     // APB divider
            break;
        case CLK_APB2:
            apb_divisor = apb_divisor * (1 << READ_REG_FIELD(CLKDIV2, CLKDIV2_APB2CKDIV));     // APB divider
            break;
        case CLK_APB3:
            apb_divisor = apb_divisor * (1 << READ_REG_FIELD(CLKDIV2, CLKDIV2_APB3CKDIV));     // APB divider
            break;
        case CLK_APB4:
            apb_divisor = apb_divisor * (1 << READ_REG_FIELD(CLKDIV2, CLKDIV2_APB4CKDIV));     // APB divider
            break;
        case CLK_APB5:
            apb_divisor = apb_divisor * (1 << READ_REG_FIELD(CLKDIV2, CLKDIV2_APB5CKDIV));     // APB divider
            break;
        case CLK_SPI0:
            apb_divisor = apb_divisor * (1 + READ_REG_FIELD(CLKDIV3, CLKDIV3_SPI0CKDIV));       // CLK_SPI0 divider
            break;
        case CLK_SPI1:
            apb_divisor = apb_divisor * (1 + READ_REG_FIELD(CLKDIV3, CLKDIV3_SPI1CKDIV));       // CLK_SPI1 divider
            break;
        case CLK_SPIX:
            apb_divisor = apb_divisor * (1 + READ_REG_FIELD(CLKDIV3, CLKDIV3_SPIXCKDIV));       // CLK_SPIX divider
            break;
        case CLK_SPI3_AHB3:
            apb_divisor = apb_divisor * (1 + READ_REG_FIELD(CLKDIV1, CLKDIV1_AHB3CKDIV));       // CLK_SPI3 divider
            break;
#ifdef NPCM850_TIP
        case CLK_TIP_APB:
            apb_divisor = apb_divisor * (1 + READ_REG_FIELD(TIPCFGR, TIPCFGR_APB_CLK_DIV));     // CLK_TIP_APB divider
            break;
#endif
        default:
            apb_divisor = 0xFFFFFFFF;
            break;
    }

    return apb_divisor;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_Delay_MicroSec                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  microSec -  number of microseconds to delay                                            */
/*                                                                                                         */
/* Returns:         Number of iterations executed                                                          */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs a busy delay (empty loop)                                        */
/*                  the number of iterations is based on current CPU clock calculation and cache           */
/*                  enabled/disabled                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_Delay_MicroSec (UINT32 microSec)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* This register is reset by only VSB power-up reset. The value of this register represents a counter  */
    /* with a 25 MHz clock, used to update the SECCNT register. This field is updated every 640ns.         */
    /* The 4 LSB of this field are always 0. When this field reaches a value of 25,000,000 it goes to 0    */
    /* and SEC_CNT field is updated.                                                                       */
    /*-----------------------------------------------------------------------------------------------------*/

    /* not testing that microSec < 33 sec (2^25bit) us */

    UINT32 iUsCnt1[2], iUsCnt2[2];
    UINT32 seconds;
    UINT32 micro_seconds;

    CLK_GetTimeStamp(iUsCnt1);
    seconds = microSec / _1MHz_;
    iUsCnt1[1] = iUsCnt1[1] + seconds;

    micro_seconds = (microSec % _1MHz_) * EXT_CLOCK_FREQUENCY_MHZ;
    iUsCnt1[0] = iUsCnt1[0] + micro_seconds;
    if (iUsCnt1[0] >= EXT_CLOCK_FREQUENCY_HZ)
    {
        iUsCnt1[1] ++;
        iUsCnt1[0] -= EXT_CLOCK_FREQUENCY_HZ;
    }

    while (TRUE)
    {
        CLK_GetTimeStamp(iUsCnt2);
        if (iUsCnt2[1] > iUsCnt1[1])
        {
            break;
        }
        else if (iUsCnt2[1] == iUsCnt1[1])
        {
            if (iUsCnt2[0] >= iUsCnt1[0])
            {
                break;
            }
        }
    }

    return microSec + (_1MHz_ * (iUsCnt2[1] - iUsCnt1[1])) + (iUsCnt2[0] - iUsCnt1[0]) / EXT_CLOCK_FREQUENCY_MHZ;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_Delay_Sec                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  microSec -  number of microseconds to delay                                            */
/*                                                                                                         */
/* Returns:         void                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs a busy delay (empty loop)                                        */
/*                  Accuracy is 1 sec, always rounded up                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_Delay_Sec (UINT32 second_delay)
{
    UINT32 seconds_start = REG_READ(SECCNT);
    UINT32 seconds_now = 0;

    while (TRUE)
    {
        seconds_now = REG_READ(SECCNT);
        if (seconds_now > (seconds_start + second_delay))
        {
            break;
        }
    }

    return;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_Delay_Since                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  microSecDelay -  number of microseconds to delay since t0_time. if zero: no delay.     */
/*                  t0_time       - start time , to measure time from.                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  get a time stamp, delay microSecDelay from it. If microSecDelay has already passed     */
/*                  since the time stamp , then no delay is executed. returns the time that elapsed since  */
/*                  t0_time .                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_Delay_Since (UINT32 microSecDelay, UINT32 t0_time[2])
{
    UINT32 iUsCnt2[2];
    UINT32 timeElapsedSince;  // Acctual delay generated by FW
    UINT32 minimum_delay;

    // check overflow (172sec in NPCM7XX and NPCM8XX)
    if (microSecDelay > ((0xFFFFFFFF - CNTR25M_ACCURECY) / EXT_CLOCK_FREQUENCY_MHZ))
    {
        minimum_delay = 0xFFFFFFFF - CNTR25M_ACCURECY*3;
    }
    else
    {
        minimum_delay = (microSecDelay * EXT_CLOCK_FREQUENCY_MHZ) + CNTR25M_ACCURECY; /* this is equivalent to microSec/0.64 + minimal tic length.*/
    }

    do
    {
        CLK_GetTimeStamp(iUsCnt2);
        timeElapsedSince =  ((EXT_CLOCK_FREQUENCY_MHZ * _1MHz_) * (iUsCnt2[1] - t0_time[1])) + (iUsCnt2[0] - t0_time[0]);
    } while(timeElapsedSince < minimum_delay);

    /*-----------------------------------------------------------------------------------------------------*/
    /* return elapsed time                                                                                 */
    /*-----------------------------------------------------------------------------------------------------*/
    return (UINT32)(timeElapsedSince / EXT_CLOCK_FREQUENCY_MHZ);
}

#if 0
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_Time_MicroSec                                                                      */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         Current time stamp                                                                     */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_GetTimeStamp (UINT32 time_quad[2])
{
}
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_Delay_Cycles                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  cycles -  num of cycles to delay                                                       */
/*                                                                                                         */
/* Returns:         Number of iterations executed                                                          */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs delay in number of cycles (delay in C code).                     */
/*                  For a more accurate delay, use : CLK_Delay_MicroSec                                    */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_Delay_Cycles (UINT32 cycles)
{
    volatile UINT   i          = 0;
    volatile UINT32 iterations = 0;

    /*-----------------------------------------------------------------------------------------------------*/
    /* The measurements were done on PD over 50 cycles, fetches from ROM:                                  */
    /*-----------------------------------------------------------------------------------------------------*/
    const UINT CYCLES_IN_ONE_ITERATION_CACHE_DISABLED  =   145;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Calculate number of iterations                                                                      */
    /*-----------------------------------------------------------------------------------------------------*/
    iterations = cycles/CYCLES_IN_ONE_ITERATION_CACHE_DISABLED + 1;

    /*-----------------------------------------------------------------------------------------------------*/
    /* The actual wait loop:                                                                               */
    /*-----------------------------------------------------------------------------------------------------*/
    //ICACHE_SAVE_DISABLE(cacheState);
    for (i = 0; i < iterations; i++);
    //ICACHE_RESTORE(cacheState);

    return iterations;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetCPUFreq                                                                         */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine calculates CPU frequency in Hz                                            */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetCPUFreq (void)
{
    UINT32  FOUT        = 0;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Calculate CPU clock:                                                                                */
    /*-----------------------------------------------------------------------------------------------------*/
    if (READ_REG_FIELD(CLKSEL, CLKSEL_CPUCKSEL) == CLKSEL_CPUCKSEL_PLL0)
    {
        FOUT = CLK_GetPll0Freq();
    }
    else if (READ_REG_FIELD(CLKSEL, CLKSEL_CPUCKSEL) == CLKSEL_CPUCKSEL_PLL1)
    {
        FOUT = CLK_GetPll1Freq();
    }
    else if (READ_REG_FIELD(CLKSEL, CLKSEL_CPUCKSEL) == CLKSEL_CPUCKSEL_PLL2)
    {
        FOUT = CLK_GetPll2Freq()*2; // CPU get PLL1 before divider.
    }
    else if (READ_REG_FIELD(CLKSEL, CLKSEL_CPUCKSEL) == CLKSEL_CPUCKSEL_CLKREF)
    {
        /*-------------------------------------------------------------------------------------------------*/
        /* Reference clock 25MHz:                                                                          */
        /*-------------------------------------------------------------------------------------------------*/
        FOUT = EXT_CLOCK_FREQUENCY_HZ; //FOUT is specified in MHz
    }
    else // CLKSEL_CPUCKSEL_SYSBPCK (120MHz)
    {
        /*-------------------------------------------------------------------------------------------------*/
        /* External clock, assume low freq ref clock (120MHz):                                              */
        /*-------------------------------------------------------------------------------------------------*/
        FOUT = 120000000; //FOUT is specified in Hz
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* Returing value in Hertz                                                                             */
    /*-----------------------------------------------------------------------------------------------------*/
    return FOUT;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetMCFreq                                                                          */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine calculates MC frequency                                                   */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetMCFreq (void)
{
    UINT32 FOUT = 0;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Calculate MC clock:                                                                                 */
    /*-----------------------------------------------------------------------------------------------------*/
    if (READ_REG_FIELD(CLKSEL, CLKSEL_MCCKSEL) == CLKSEL_MCCKSEL_PLL1)
    {
        FOUT = CLK_GetPll1Freq();  // don't add divider. The external one is equal to OTDV1*OTDV2
    }
    else if (READ_REG_FIELD(CLKSEL, CLKSEL_MCCKSEL) == CLKSEL_MCCKSEL_MCBPCK)
    {
        FOUT = 100000000; // external output on MCBPCK pin.
    }
    else if (READ_REG_FIELD(CLKSEL, CLKSEL_MCCKSEL) == CLKSEL_MCCKSEL_CLKREF)
    {
        /*-------------------------------------------------------------------------------------------------*/
        /* Reference clock 25MHz:                                                                          */
        /*-------------------------------------------------------------------------------------------------*/
        FOUT = EXT_CLOCK_FREQUENCY_HZ; //FOUT is specified in MHz
    }
    else
    {
        /*-------------------------------------------------------------------------------------------------*/
        /* External clock, assume low freq ref clock (25MHz):                                              */
        /*-------------------------------------------------------------------------------------------------*/
        FOUT = EXT_CLOCK_FREQUENCY_HZ; //FOUT is specified in MHz
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* Returing value in Hertz                                                                             */
    /*-----------------------------------------------------------------------------------------------------*/
    return FOUT;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetAPBFreq                                                                         */
/*                                                                                                         */
/* Parameters:      apb number,1 to 5                                                                      */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine returns APB frequency in Hz                                               */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetAPBFreq (APB_CLK apb)
{
    /*-----------------------------------------------------------------------------------------------------*/
    /* Avalilable APBs between 1 to 5, SPI0-3, X                                                           */
    /*-----------------------------------------------------------------------------------------------------*/
    DEFS_STATUS_COND_CHECK((apb < CLK_NUM) && (apb >= CLK_APB1) , DEFS_STATUS_PARAMETER_OUT_OF_RANGE);

    return  CLK_GetCPUFreq()  / CLK_GetPLL0toAPBdivisor(apb);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetCPFreq                                                                          */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine returns CP frequency in Hz (CLK4)                                         */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetCPFreq (void)
{
    /*-------------------------------------------------------------------------------------------------*/
    /* In Arbel APB freq is CPU frequency divided by Pre Clock divider                                 */
    /*-------------------------------------------------------------------------------------------------*/
    return  (CLK_GetCPUFreq()  / (2 * (READ_REG_FIELD(CLKDIV1, CLKDIV1_CLK4DIV) + 1)));
}

#if 0
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetTimerFreq                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine calculates timer frequency in Hz                                          */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetTimerFreq (void)
{
}
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_SetCPFreq                                                                          */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine sets CP frequency in Hz                                                   */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS CLK_SetCPFreq (UINT32 cpFreq)
{
    UINT32 clkDiv = CLK_GetCPUFreq() / cpFreq / 2;  // CPU clock, followed by Pre-clk divider (/2 fixed).

    DEFS_STATUS_COND_CHECK(clkDiv <= 4, DEFS_STATUS_INVALID_PARAMETER);

    SET_REG_FIELD(CLKDIV1, CLKDIV1_CLK4DIV, CLKDIV1_CLK4DIV_DIV(clkDiv));

    CLK_Delay_MicroSec(200);

    return  DEFS_STATUS_OK;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetSDClock                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  sdNum -  SD module number                                                              */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Returns the SD base clock frequency in Hz                                              */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetSDClock (UINT32 sdNum)
{
    UINT32  divider;
    UINT32  FOUT        = 0;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Get ADC source clock:                                           */
    /*-----------------------------------------------------------------------------------------------------*/
    if  (READ_REG_FIELD(CLKSEL, CLKSEL_SDCKSEL) == CLKSEL_SDCKSEL_PLL0)
    {
        FOUT = CLK_GetPll0Freq();
    }
    else if (READ_REG_FIELD(CLKSEL, CLKSEL_SDCKSEL) == CLKSEL_SDCKSEL_PLL1)
    {
        FOUT = CLK_GetPll1Freq();
    }
    else if (READ_REG_FIELD(CLKSEL, CLKSEL_SDCKSEL) == CLKSEL_SDCKSEL_PLL2)
    {
        FOUT = CLK_GetPll2Freq();
    }
    else
    {
        FOUT = EXT_CLOCK_FREQUENCY_HZ;
    }

    if ((SD_DEV_NUM_T)sdNum == SD1_DEV)
    {
        divider = 1 + READ_REG_FIELD(CLKDIV1, CLKDIV1_MMCCKDIV);
    }
    else if (sdNum == SD2_DEV)
    {
        divider = 2*(1 + READ_REG_FIELD(CLKDIV2, CLKDIV2_SD1CKDIV));
    }
    else
    {
        return DEFS_STATUS_PARAMETER_OUT_OF_RANGE;
    }

    return (FOUT/divider);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ConfigurePCIClock                                                                  */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs configuration of PCI clock depending on                          */
/*                  presence of VGA BIOS as specified by STRAP13                                           */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_ConfigurePCIClock (void)
{
    UINT32 divider = 0;

    if (READ_REG_FIELD(CLKSEL, CLKSEL_CPUCKSEL) == CLKSEL_CPUCKSEL_CLKREF)
    {
        SET_REG_FIELD(CLKSEL, CLKSEL_PCIGFXCKSEL,  CLKSEL_PCIGFXCKSEL_PLL0);
        CLK_Delay_MicroSec(200);

        SET_REG_FIELD(CLKDIV1, CLKDIV1_PCICKDIV,  CLKDIV1_PCICKDIV_DIV(5));
        CLK_Delay_MicroSec(200);

        SET_REG_FIELD(CLKDIV2, CLKDIV2_GFXCKDIV,  CLKDIV2_GFXCKDIV_DIV(4));
        CLK_Delay_MicroSec(200);
    }
    else
    {
        divider =  DIV_ROUND(CLK_GetPll1Freq(), 125000000);
        SET_REG_FIELD(CLKSEL, CLKSEL_PCIGFXCKSEL,  CLKSEL_PCIGFXCKSEL_PLL1);
        CLK_Delay_MicroSec(200);

        SET_REG_FIELD(CLKDIV1, CLKDIV1_PCICKDIV,  CLKDIV1_PCICKDIV_DIV(divider));
        CLK_Delay_MicroSec(200);

        SET_REG_FIELD(CLKDIV2, CLKDIV2_GFXCKDIV,  CLKDIV2_GFXCKDIV_DIV(divider-1));
        CLK_Delay_MicroSec(200);
    }

    return;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetPCIClock                                                                        */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine returns configuration of PCI clock                                        */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetPCIClock (void)
{
    UINT32 source = 0;
    UINT32 sel =  READ_REG_FIELD(CLKSEL, CLKSEL_PCIGFXCKSEL);

    if (sel == CLKSEL_PCIGFXCKSEL_PLL0)
        source = CLK_GetPll0Freq();
    if (sel == CLKSEL_PCIGFXCKSEL_PLL1)
        source = CLK_GetPll1Freq();
    if (sel == CLKSEL_PCIGFXCKSEL_CLKREF)
        source = EXT_CLOCK_FREQUENCY_HZ;
    if (sel == CLKSEL_PCIGFXCKSEL_PLL2)
        source = CLK_GetPll2Freq();

    return (source / (1 + (UINT32)READ_REG_FIELD(CLKDIV1, CLKDIV1_PCICKDIV)));
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetGFXClock                                                                        */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                 This routine returns configuration of GFX clock                                         */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetGFXClock (void)
{
    UINT32 source = 0;
    UINT32 sel =  READ_REG_FIELD(CLKSEL, CLKSEL_PCIGFXCKSEL);

    if (sel == CLKSEL_PCIGFXCKSEL_PLL0)
        source = CLK_GetPll0Freq();
    if (sel == CLKSEL_PCIGFXCKSEL_PLL1)
        source = CLK_GetPll1Freq();
    if (sel == CLKSEL_PCIGFXCKSEL_CLKREF)
        source = EXT_CLOCK_FREQUENCY_HZ;
    if (sel == CLKSEL_PCIGFXCKSEL_PLL2)
        source = CLK_GetPll2Freq();

    return (source / (1 + (UINT32)READ_REG_FIELD(CLKDIV2, CLKDIV2_GFXCKDIV)));
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetPll0Freq                                                                        */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Returns the frequency of PLL0 in Hz                                                    */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetPll0Freq (void)
{
    UINT32 pllVal = REG_READ(PLLCON0);

    return CLK_CalculatePLLFrequency(pllVal);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetPll1Freq                                                                        */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Returns the frequency of PLL1 in Hz                                                    */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetPll1Freq (void)
{
    UINT32 pllVal = REG_READ(PLLCON1);

    return CLK_CalculatePLLFrequency(pllVal);  // FOUTVCO/(OTDV2*OTDV1)  or  FOUTPOSTDIV

    // Note: Most selectors on the clock tree get PLL1 FOUTPOSTDIV (which is *after* OTDV1 and 2).
    // This feature is only used if the PLL dividers will have unexpected issue.
    // MC get FOUTVCO, which is *before* the OTDV2 divider, and then there is  /2 divider outside the PLL.
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetPll2Freq                                                                        */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Returns the frequency of PLL2 in Hz                                                    */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetPll2Freq (void)
{
    UINT32  pllVal      = 0;

    pllVal = REG_READ(PLLCON2);

    return (CLK_CalculatePLLFrequency(pllVal)/2); // note: some clock selectors have /2 after PLL2
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_Configure_CPU_MC_Clock                                                             */
/*                                                                                                         */
/* Parameters:      mcFreq - frequency in Hz of MC                                                         */
/* Parameters:      cpuFreq - frequency in Hz of CPU                                                       */
/* Parameters:      pll0Freq - frequency in Hz of PLL0, if not used by CPU we can set it to something else */
/* Returns:         status                                                                                 */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs configuration of MC and CPU Clocks                               */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS CLK_Configure_CPU_MC_Clock (UINT32 mcFreq, UINT32 cpuFreq, UINT32 pll0_freq)
{
    UINT32 pllcon1_L = 0;
    UINT32 pllcon0_L = 0;
    UINT32 clksel_l = 0;
    UINT32 clksel_now_l = 0;

    UINT32 clksel_clkref_l = 0;
    UINT32 pll0_freq_tmp = 0;

    UINT32 PLLCON0_current_reg_value = 0;
    UINT32 PLLCON1_current_reg_value = 0;


    PLLCON0_current_reg_value = REG_READ(PLLCON0);
    PLLCON1_current_reg_value = REG_READ(PLLCON1);

    clksel_l           = REG_READ(CLKSEL);
    clksel_clkref_l    = clksel_l;

    pllcon0_L = PLLCON0_current_reg_value;
    pllcon1_L = PLLCON1_current_reg_value;

    if(pll0_freq != 0)
        pll0_freq_tmp =  pll0_freq;
    else
        pll0_freq_tmp = cpuFreq;

    /*---------------------------------------------------------------------------------------------------------*/
    /* PLLCON 1 possible values (notice that PLL1 has a divider /2, so OTDV1 is smaller in half                */
    /*---------------------------------------------------------------------------------------------------------*/
    if ( mcFreq <= 500000000)      pllcon1_L = CLK_500MHZ_PLLCON1_REG_CFG       ;
    else if ( mcFreq <= 666000000)      pllcon1_L = CLK_666MHZ_PLLCON1_REG_CFG       ;
    else if ( mcFreq <= 700000000)      pllcon1_L = CLK_700MHZ_PLLCON1_REG_CFG       ;
    else if ( mcFreq <= 720000000)      pllcon1_L = CLK_720MHZ_PLLCON1_REG_CFG       ;
    else if ( mcFreq <= 750000000)      pllcon1_L = CLK_750MHZ_PLLCON1_REG_CFG       ;
    else if ( mcFreq <= 775000000)      pllcon1_L = CLK_775MHZ_PLLCON1_REG_CFG       ;
    else if ( mcFreq <= 787500000)      pllcon1_L = CLK_787_5MHZ_PLLCON1_REG_CFG     ;
    else if ( mcFreq <= 800000000)      pllcon1_L = CLK_800MHZ_PLLCON1_REG_CFG       ;
    else if ( mcFreq <= 850000000)      pllcon1_L = CLK_850MHZ_PLLCON1_REG_CFG       ;
    else if ( mcFreq <= 900000000)      pllcon1_L = CLK_900MHZ_PLLCON1_REG_CFG       ;
    else if ( mcFreq <= 950000000)      pllcon1_L = CLK_950MHZ_PLLCON1_REG_CFG       ;
    else if ( mcFreq <= 975000000)      pllcon1_L = CLK_975MHZ_PLLCON1_REG_CFG       ;
    else if ( mcFreq <= 1000000000)     pllcon1_L = CLK_1000MHZ_PLLCON1_REG_CFG      ;
    else if ( mcFreq <= 1037000000)     pllcon1_L = CLK_1037MHZ_PLLCON1_REG_CFG      ;
    else if ( mcFreq <= 1050000000)     pllcon1_L = CLK_1050MHZ_PLLCON1_REG_CFG      ;
    else if ( mcFreq <= 1062500000)     pllcon1_L = CLK_1062_5_MHZ_PLLCON1_REG_CFG   ;
    else if ( mcFreq <= 1067000000)     pllcon1_L = CLK_1066MHZ_PLLCON1_REG_CFG      ;
    else if ( mcFreq <= 1100000000)     pllcon1_L = CLK_1100MHZ_PLLCON1_REG_CFG      ;
    else if ( mcFreq <= 1150000000)     pllcon1_L = CLK_1150MHZ_PLLCON1_REG_CFG      ;
    else if ( mcFreq <= 1200000000)     pllcon1_L = CLK_1200MHZ_PLLCON1_REG_CFG      ;
    else   return DEFS_STATUS_FAIL;

    /*---------------------------------------------------------------------------------------------------------*/
    /* PLLCON 0 possible values (notice that PLL1 in Z2 has a divider /2, so OTDV1 is smaller in half      */
    /*---------------------------------------------------------------------------------------------------------*/
    if ( pll0_freq_tmp <= 125000000 )  pllcon0_L = CLK_125MHZ_PLLCON0_2_REG_CFG ;
    else if ( pll0_freq_tmp <= 333000000 )  pllcon0_L = CLK_333MHZ_PLLCON0_2_REG_CFG ;
    else if ( pll0_freq_tmp <= 500000000 )  pllcon0_L = CLK_500MHZ_PLLCON0_2_REG_CFG ;
    else if ( pll0_freq_tmp <= 600000000 )  pllcon0_L = CLK_600MHZ_PLLCON0_2_REG_CFG ;
    else if ( pll0_freq_tmp <= 666000000 )  pllcon0_L = CLK_666MHZ_PLLCON0_2_REG_CFG ;
    else if ( pll0_freq_tmp <= 700000000 )  pllcon0_L = CLK_700MHZ_PLLCON0_2_REG_CFG ;
    else if ( pll0_freq_tmp <= 720000000 )  pllcon0_L = CLK_720MHZ_PLLCON0_2_REG_CFG ;
    else if ( pll0_freq_tmp <= 750000000 )  pllcon0_L = CLK_750MHZ_PLLCON0_2_REG_CFG ;
    else if ( pll0_freq_tmp <= 800000000 )  pllcon0_L = CLK_800MHZ_PLLCON0_2_REG_CFG ;
    else if ( pll0_freq_tmp <= 825000000 )  pllcon0_L = CLK_825MHZ_PLLCON0_2_REG_CFG ;
    else if ( pll0_freq_tmp <= 850000000 )  pllcon0_L = CLK_850MHZ_PLLCON0_2_REG_CFG ;
    else if ( pll0_freq_tmp <= 888000000 )  pllcon0_L = CLK_888MHZ_PLLCON0_2_REG_CFG ;
    else if ( pll0_freq_tmp <= 900000000 )  pllcon0_L = CLK_900MHZ_PLLCON0_2_REG_CFG ;
    else if ( pll0_freq_tmp <= 950000000 )  pllcon0_L = CLK_950MHZ_PLLCON0_2_REG_CFG ;
    else if ( pll0_freq_tmp <= 960000000 )  pllcon0_L = CLK_960MHZ_PLLCON0_2_REG_CFG ;
    else if ( pll0_freq_tmp <= 1000000000)  pllcon0_L = CLK_1000MHZ_PLLCON0_2_REG_CFG;
    else   return DEFS_STATUS_CLK_ERROR;


    if (( pllcon0_L == (REG_READ(PLLCON0) & 0x7FFFFFFF)) &&
    	( pllcon1_L == (REG_READ(PLLCON1) & 0x7FFFFFFF)) &&
        (pll0_freq == 0))
    {
        HAL_PRINT(KGRN "CLK: already set to requested values, no need to reset\n" KNRM);
        return DEFS_STATUS_SYSTEM_NOT_INITIALIZED;
    }


    /*-----------------------------------------------------------------------------------------------------*/
    /* CLKSEL handling: Check if MC freq != CPU freq. If so, need to set CPU to different PLL.             */
    /*-----------------------------------------------------------------------------------------------------*/
    if (mcFreq != cpuFreq)
    {
        /* ERROR: can't select 3 different settings */
        //if (pll0_freq != 0)
        /// return DEFS_STATUS_FAIL;

        /*-----------------------------------------------------------------------------------------------------*/
        /* MC and CPU use different PLLs . CPU on PLL0, MC on PLL1                                             */
        /*-----------------------------------------------------------------------------------------------------*/
        SET_VAR_FIELD(clksel_l, CLKSEL_CPUCKSEL,  CLKSEL_CPUCKSEL_PLL0);
        SET_VAR_FIELD(clksel_l, CLKSEL_MCCKSEL,   CLKSEL_MCCKSEL_PLL1);
    }
    else // if (mcFreq == cpuFreq)
    {
        /*-----------------------------------------------------------------------------------------------------*/
        /* MC and CPU use same PLL1                                                                            */
        /*-----------------------------------------------------------------------------------------------------*/
        SET_VAR_FIELD(clksel_l, CLKSEL_CPUCKSEL,  CLKSEL_CPUCKSEL_PLL1);
        SET_VAR_FIELD(clksel_l, CLKSEL_MCCKSEL,   CLKSEL_MCCKSEL_PLL1);
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* PLL0 handling : Check if PLL is set to the value CPU is set: Notice:even if MC freq == CPU freq,    */
    /* both will be connected to PLL1, and PLL0 will be turned off.                                        */
    /*-----------------------------------------------------------------------------------------------------*/
    if (pllcon0_L != (REG_READ(PLLCON0) & 0x7FFFFFFF) && (mcFreq != cpuFreq))
    {
        clksel_now_l = REG_READ(CLKSEL);

        /*-----------------------------------------------------------------------------------------------------*/
        /* Switch clock sources to external clock for all muxes (only for PLL0 users)                      */
        /*-----------------------------------------------------------------------------------------------------*/
        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_RCPCKSEL) ==  CLKSEL_RCPCKSEL_PLL0)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_RCPCKSEL      , CLKSEL_RCPCKSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_RGSEL) ==  CLKSEL_RGSEL_PLL0)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_RGSEL     , CLKSEL_RGSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_CLKOUTSEL) ==  CLKSEL_CLKOUTSEL_PLL0)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_CLKOUTSEL     , CLKSEL_CLKOUTSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_PCIGFXCKSEL) ==  CLKSEL_PCIGFXCKSEL_PLL0)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_PCIGFXCKSEL   , CLKSEL_PCIGFXCKSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_ADCCKSEL) ==  CLKSEL_ADCCKSEL_PLL0)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_ADCCKSEL      , CLKSEL_ADCCKSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_SUCKSEL) ==  CLKSEL_SUCKSEL_PLL0)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_SUCKSEL       , CLKSEL_SUCKSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_UARTCKSEL) ==  CLKSEL_UARTCKSEL_PLL0)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_UARTCKSEL     , CLKSEL_UARTCKSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_SDCKSEL) ==  CLKSEL_SDCKSEL_PLL0)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_SDCKSEL       , CLKSEL_SDCKSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_CPUCKSEL) ==  CLKSEL_CPUCKSEL_PLL0)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_CPUCKSEL      , CLKSEL_CPUCKSEL_CLKREF);

        REG_WRITE(CLKSEL, clksel_clkref_l);
        CLK_Delay_MicroSec(20);

        /*-----------------------------------------------------------------------------------------------------*/
        /* Change PLL configuration (leave PLL in reset mode):                                                 */
        /*-----------------------------------------------------------------------------------------------------*/
        // Set PWDEN bit
        SET_VAR_FIELD(pllcon0_L, PLLCONn_PWDEN, PLLCONn_PWDEN_POWER_DOWN);

        /* set to power down == 1 */
        REG_WRITE(PLLCON0, pllcon0_L);

        /*-----------------------------------------------------------------------------------------------------*/
        /* Wait 10usec for PLL:                                                                                */
        /*-----------------------------------------------------------------------------------------------------*/
        //3. Wait 20us or more;
        CLK_Delay_MicroSec(20);

        /*-----------------------------------------------------------------------------------------------------*/
        /* Enable PLLs after configuring. Delays to prevent power issues.                                      */
        /*-----------------------------------------------------------------------------------------------------*/
        SET_VAR_FIELD(pllcon0_L, PLLCONn_PWDEN, PLLCONn_PWDEN_NORMAL);
        REG_WRITE(PLLCON0, pllcon0_L);
        CLK_Delay_MicroSec(20);

        /*-----------------------------------------------------------------------------------------------------*/
        /* Wait 750usec for PLL to stabilize:                                                                  */
        /*-----------------------------------------------------------------------------------------------------*/
        WaitForPllLock(PLL0);
        /* Wait 10usec for PLL:   */
        CLK_Delay_MicroSec(60);

        // return All that was on PLL0 back:
        REG_WRITE(CLKSEL, clksel_l);
        CLK_Delay_MicroSec(20);
        SET_REG_FIELD(INTCR2, INTCR2_CFGDONE, 1);
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* PLL1 handling                                                                                       */
    /*-----------------------------------------------------------------------------------------------------*/
    if (pllcon1_L != (REG_READ(PLLCON1) & 0x7FFFFFFF))
    {
        clksel_l           = REG_READ(CLKSEL);
        clksel_clkref_l    = clksel_l;
        clksel_now_l       = REG_READ(CLKSEL);

        /*-----------------------------------------------------------------------------------------------------*/
        /* if both freq are same, move everything to PLL1 and shutdown PLL0: Notice clksel_l is used for final value for CLKSEL reg !*/
        /*-----------------------------------------------------------------------------------------------------*/
        if ((mcFreq == cpuFreq)|| (pll0_freq != 0))
        {

            /*-----------------------------------------------------------------------------------------------------*/
            /* MC and CPU use same PLL1                                                                            */
            /*-----------------------------------------------------------------------------------------------------*/
            SET_VAR_FIELD(clksel_l, CLKSEL_RCPCKSEL    , CLKSEL_RCPCKSEL_PLL0    );
            SET_VAR_FIELD(clksel_l, CLKSEL_RGSEL       , CLKSEL_RGSEL_PLL0       );
            SET_VAR_FIELD(clksel_l, CLKSEL_AHB6SSEL    , CLKSEL_AHB6SSEL_PLL2    );
            SET_VAR_FIELD(clksel_l, CLKSEL_GFXMSEL     , CLKSEL_GFXMSEL_PLL2     );
            SET_VAR_FIELD(clksel_l, CLKSEL_CLKOUTSEL   , CLKSEL_CLKOUTSEL_CLKREF );
            SET_VAR_FIELD(clksel_l, CLKSEL_PCIGFXCKSEL , CLKSEL_PCIGFXCKSEL_PLL1 );
            SET_VAR_FIELD(clksel_l, CLKSEL_ADCCKSEL    , CLKSEL_ADCCKSEL_CLKREF  );
            SET_VAR_FIELD(clksel_l, CLKSEL_MCCKSEL     , CLKSEL_MCCKSEL_PLL1     );
            SET_VAR_FIELD(clksel_l, CLKSEL_SUCKSEL     , CLKSEL_SUCKSEL_PLL2     );
            SET_VAR_FIELD(clksel_l, CLKSEL_UARTCKSEL   , CLKSEL_UARTCKSEL_PLL2   );
            SET_VAR_FIELD(clksel_l, CLKSEL_SDCKSEL     , CLKSEL_SDCKSEL_PLL1     );
            SET_VAR_FIELD(clksel_l, CLKSEL_PIXCKSEL    , CLKSEL_PIXCKSEL_PLLG    );
            SET_VAR_FIELD(clksel_l, CLKSEL_CPUCKSEL    , CLKSEL_CPUCKSEL_PLL1    );

            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_CPUCKSEL,  CLKSEL_CPUCKSEL_CLKREF);
        }
        else
        {
            SET_VAR_FIELD(clksel_l, CLKSEL_CPUCKSEL,  CLKSEL_CPUCKSEL_PLL0);
            SET_VAR_FIELD(clksel_l, CLKSEL_MCCKSEL,   CLKSEL_MCCKSEL_PLL1);
        }

        /*-----------------------------------------------------------------------------------------------------*/
        /* check if other modules are using PLL1. If so, move them someplace else while the PLL1 is configuring*/
        /* Notice ! here we decide on clksel_clkref_l which is for the time of te switching only.              */
        /*-----------------------------------------------------------------------------------------------------*/
        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_RCPCKSEL) ==  CLKSEL_RCPCKSEL_PLL1)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_RCPCKSEL      , CLKSEL_RCPCKSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_RGSEL) ==  CLKSEL_RGSEL_PLL1)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_RGSEL     , CLKSEL_RGSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_CLKOUTSEL) ==  CLKSEL_CLKOUTSEL_PLL1)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_CLKOUTSEL     , CLKSEL_CLKOUTSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_PCIGFXCKSEL) ==  CLKSEL_PCIGFXCKSEL_PLL1)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_PCIGFXCKSEL   , CLKSEL_PCIGFXCKSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_ADCCKSEL) ==  CLKSEL_ADCCKSEL_PLL1)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_ADCCKSEL      , CLKSEL_ADCCKSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_MCCKSEL) ==  CLKSEL_MCCKSEL_PLL1)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_MCCKSEL       , CLKSEL_MCCKSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_SUCKSEL) ==  CLKSEL_SUCKSEL_PLL1)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_SUCKSEL       , CLKSEL_SUCKSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_UARTCKSEL) ==  CLKSEL_UARTCKSEL_PLL1)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_UARTCKSEL     , CLKSEL_UARTCKSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_SDCKSEL) ==  CLKSEL_SDCKSEL_PLL1)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_SDCKSEL       , CLKSEL_SDCKSEL_CLKREF);

        if (READ_VAR_FIELD(clksel_now_l, CLKSEL_CPUCKSEL) ==  CLKSEL_CPUCKSEL_PLL1)
            SET_VAR_FIELD(clksel_clkref_l, CLKSEL_CPUCKSEL      , CLKSEL_CPUCKSEL_CLKREF);

        /*-----------------------------------------------------------------------------------------------------*/
        /*  "hold" the clocks while PLL1 is reconfiured.                                                       */
        /*-----------------------------------------------------------------------------------------------------*/
        REG_WRITE(CLKSEL, clksel_clkref_l);
        CLK_Delay_MicroSec(60);

        /*-----------------------------------------------------------------------------------------------------*/
        /* Now reconfigure PLL1:                                                                               */
        /*-----------------------------------------------------------------------------------------------------*/
        SET_VAR_FIELD(pllcon1_L, PLLCONn_PWDEN, PLLCONn_PWDEN_POWER_DOWN);
        REG_WRITE(PLLCON1, pllcon1_L);
        CLK_Delay_MicroSec(60);

        SET_VAR_FIELD(pllcon1_L, PLLCONn_PWDEN, PLLCONn_PWDEN_NORMAL);
        REG_WRITE(PLLCON1, pllcon1_L);
        CLK_Delay_MicroSec(60);

        WaitForPllLock(PLL1);
        CLK_Delay_MicroSec(60);

        /*-----------------------------------------------------------------------------------------------------*/
        /* return everything back to PLL1, includeing the CPU                                                  */
        /*-----------------------------------------------------------------------------------------------------*/
        REG_WRITE(CLKSEL, clksel_l);
        CLK_Delay_MicroSec(60);

        /* Force re-training of DDR (because clock is reinitialized*/
        SET_REG_FIELD(INTCR2, INTCR2_MC_INIT, 0);
        SET_REG_FIELD(INTCR2, INTCR2_CFGDONE, 1);
    }


    return DEFS_STATUS_OK;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_Verify_and_update_dividers                                                         */
/*                                                                                                         */
/* Parameters:      None                                                                                   */
/* Returns:         DEFS_STATUS                                                                            */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine fix all the dividers after PLL change                                     */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS CLK_Verify_and_update_dividers (void)
{
    UINT32 clkDiv;

    UINT32 clk4Freq = CLK_GetCPUFreq() / 2 / (READ_REG_FIELD(CLKDIV1, CLKDIV1_CLK4DIV) + 1);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Fix CLK_APB1 to be above 25MHz                                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    clkDiv = (clk4Freq / (32*_1MHz_));

    if (clkDiv >= 8)
        clkDiv = CLKDIV2_APBxCKDIV_DIV(8);
    else if (clkDiv >= 4)
        clkDiv = CLKDIV2_APBxCKDIV_DIV(4);
    else if (clkDiv >= 2 )
        clkDiv = CLKDIV2_APBxCKDIV_DIV(2);
    else
        clkDiv = CLKDIV2_APBxCKDIV_DIV(1);

    SET_REG_FIELD(CLKDIV2, CLKDIV2_APB1CKDIV, clkDiv);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Fix CLK_APB2-3 to be above 50MHz                                                                    */
    /*-----------------------------------------------------------------------------------------------------*/
    clkDiv = (clk4Freq / (50 *_1MHz_));
    if (clkDiv >= 8)
        clkDiv = CLKDIV2_APBxCKDIV_DIV(8);
    else if (clkDiv >= 4)
        clkDiv = CLKDIV2_APBxCKDIV_DIV(4);
    else if (clkDiv >=2 )
        clkDiv = CLKDIV2_APBxCKDIV_DIV(2);
    else
        clkDiv = CLKDIV2_APBxCKDIV_DIV(1);

    SET_REG_FIELD(CLKDIV2, CLKDIV2_APB2CKDIV, clkDiv);
    if ((CLK_GetAPBFreq(CLK_APB2) > (63 * _1MHz_)) && (clkDiv < CLKDIV2_APBxCKDIV_DIV(8)))
        clkDiv++;

    SET_REG_FIELD(CLKDIV2, CLKDIV2_APB2CKDIV, clkDiv);
    SET_REG_FIELD(CLKDIV2, CLKDIV2_APB3CKDIV, clkDiv);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Fix CLK_APB4 to be above 25MHz                                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    clkDiv = (clk4Freq / (25 * _1MHz_));
    if (clkDiv >= 8)
        clkDiv = CLKDIV2_APBxCKDIV_DIV(8);
    else if (clkDiv >= 4)
        clkDiv = CLKDIV2_APBxCKDIV_DIV(4);
    else if (clkDiv >=2 )
        clkDiv = CLKDIV2_APBxCKDIV_DIV(2);
    else
        clkDiv = CLKDIV2_APBxCKDIV_DIV(1);

    SET_REG_FIELD(CLKDIV2, CLKDIV2_APB4CKDIV, clkDiv);
    if ((CLK_GetAPBFreq(CLK_APB4) > (63 * _1MHz_)) && (clkDiv < CLKDIV2_APBxCKDIV_DIV(8)))
        clkDiv++;

    SET_REG_FIELD(CLKDIV2, CLKDIV2_APB4CKDIV, clkDiv);
    if ((CLK_GetAPBFreq(CLK_APB4) < (25 * _1MHz_)) && (clkDiv > CLKDIV2_APBxCKDIV_DIV(1)))
        clkDiv--;

    SET_REG_FIELD(CLKDIV2, CLKDIV2_APB4CKDIV, clkDiv);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Fix CLK_APB5 to be above 62.5MHz and below 125Mhz                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
    clkDiv = (clk4Freq / (125 *_1MHz_));
    if (clkDiv >= 8)
        clkDiv = CLKDIV2_APBxCKDIV_DIV(8);
    else if (clkDiv >= 4)
        clkDiv = CLKDIV2_APBxCKDIV_DIV(4);
    else if (clkDiv >=2 )
        clkDiv = CLKDIV2_APBxCKDIV_DIV(2);
    else
        clkDiv = CLKDIV2_APBxCKDIV_DIV(1);

    SET_REG_FIELD(CLKDIV2, CLKDIV2_APB5CKDIV, clkDiv);
    if (CLK_GetAPBFreq(CLK_APB5) < (62.5 * _1MHz_))
        clkDiv--;

    SET_REG_FIELD(CLKDIV2, CLKDIV2_APB5CKDIV, clkDiv);
    if (CLK_GetAPBFreq(CLK_APB5) > (125 * _1MHz_))
        clkDiv++;

    SET_REG_FIELD(CLKDIV2, CLKDIV2_APB5CKDIV, clkDiv);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Fix CLK_SPIX to be below 33.3Mhz                                                                    */
    /*-----------------------------------------------------------------------------------------------------*/
    clkDiv = (clk4Freq / (33 *_1MHz_));

    SET_REG_FIELD(CLKDIV3, CLKDIV3_SPIXCKDIV, CLKDIV3_SPIXCKDIV_DIV(clkDiv));
    if (CLK_GetAPBFreq(CLK_SPIX) > (34 * _1MHz_))
        clkDiv++;

    SET_REG_FIELD(CLKDIV3, CLKDIV3_SPIXCKDIV, CLKDIV3_SPIXCKDIV_DIV(clkDiv));


    CLK_ConfigurePCIClock();
    CLK_ConfigureADCClock(25 * _1MHz_);
    CLK_ConfigureSDClock(SD1_DEV);
    CLK_ConfigureSDClock(SD2_DEV);


	// change CLKDIV4_RGREFDIV_DIV  125MHz

    return DEFS_STATUS_OK;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_ConfigureFIUClock                                                                  */
/*                                                                                                         */
/* Parameters:      fiu - module (0, 3, X).                                                                */
/*                  clkDiv - actual number to write to reg. The value is clkdDiv + 1)                      */
/* Returns:         DEFS_STATUS                                                                            */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine config the FIU clock (according to the header )                           */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS CLK_ConfigureFIUClock (UINT fiu, UINT8 clkDiv)
{
     /*----------------------------------------------------------------------------------------------------*/
     /* Defines the clock divide ratio from AHB to FIU0 clock.                                             */
     /*----------------------------------------------------------------------------------------------------*/
     UINT32  ratio = 0;

     /*----------------------------------------------------------------------------------------------------*/
     /* Ignored if FIU_Clk_Divider is either 0 or 0FFh.                                                    */
     /*----------------------------------------------------------------------------------------------------*/
     if (fiu != FIU_MODULE_1)
     {
          /*----------------------------------------------------------------------------------------------------*/
          /* Ignored if FIU_Clk_Divider is either 0 or 0FFh.                                                    */
          /*----------------------------------------------------------------------------------------------------*/
          if ( (clkDiv == 0) || (clkDiv > 0x20))
          {
               return DEFS_STATUS_PARAMETER_OUT_OF_RANGE;
          }
     }
     else  /* FIU1 */
     {
          if (clkDiv == 0) 
          {
               return DEFS_STATUS_PARAMETER_OUT_OF_RANGE;
          }
     }

     /* set SPIn clk div */
     switch (fiu)
     {
        case FIU_MODULE_0:
            SET_REG_FIELD(CLKDIV3, CLKDIV3_SPI0CKDIV,  (CLKDIV3_SPI0CKDIV_DIV(clkDiv) & 0x1F));
            break;
        case FIU_MODULE_1:
            SET_REG_FIELD(CLKDIV3, CLKDIV3_SPI1CKDIV,  (CLKDIV3_SPI1CKDIV_DIV(clkDiv) & 0xFF));
            break;
        case FIU_MODULE_2:
            // fallthrough
        case FIU_MODULE_3:
            SET_REG_FIELD(CLKDIV1, CLKDIV1_AHB3CKDIV, (CLKDIV1_AHB3CKDIV_DIV(clkDiv)  & 0x1F));
            break;
        case FIU_MODULE_X:
            SET_REG_FIELD(CLKDIV3, CLKDIV3_SPIXCKDIV,  (CLKDIV3_SPIXCKDIV_DIV(clkDiv) & 0x1F));
            break;
        default:
            return DEFS_STATUS_INVALID_PARAMETER;
     }

     /*----------------------------------------------------------------------------------------------------*/
     /* After changing this field, ensure a delay of 250 CLK_SPI0 clock cycles before changing CPUCKSEL    */
     /* field in CLKSEL register or accessing the AHB18 bus.                                               */
     /*----------------------------------------------------------------------------------------------------*/
     ratio = 2 * READ_REG_FIELD(CLKDIV1, CLKDIV1_CLK4DIV) * clkDiv;

     /* delay is according to ratio. */
     CLK_Delay_Cycles(250 * ratio);

     return DEFS_STATUS_OK;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetFIUClockDiv                                                                     */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine config the FIU clock (according to the header )                           */
/*---------------------------------------------------------------------------------------------------------*/
UINT8 CLK_GetFIUClockDiv (UINT fiu)
{
     /*----------------------------------------------------------------------------------------------------*/
     /* Defines the clock divide ratio from AHB to FIU0 clock.1                                            */
     /*----------------------------------------------------------------------------------------------------*/
     switch (fiu)
     {
        case FIU_MODULE_0:
            return READ_REG_FIELD(CLKDIV3, CLKDIV3_SPI0CKDIV) + 1;
        case FIU_MODULE_1:
            return READ_REG_FIELD(CLKDIV3, CLKDIV3_SPI1CKDIV) + 1;
        case FIU_MODULE_3:
            return READ_REG_FIELD(CLKDIV1, CLKDIV1_AHB3CKDIV) + 1;
        case FIU_MODULE_X:
            return READ_REG_FIELD(CLKDIV3, CLKDIV3_SPIXCKDIV) + 1;
        default:
            return 0xFF;
     }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetClkoutFreq                                                                      */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                 This routine returns configuration of CLKOUT                                            */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetClkoutFreq (void)
{
    UINT32 source = 0;
    UINT32 sel =  READ_REG_FIELD(CLKSEL, CLKSEL_CLKOUTSEL);

    switch (sel)
    {
    case CLKSEL_CLKOUTSEL_PLL0:
        source = CLK_GetPll0Freq();
        break;
    case CLKSEL_CLKOUTSEL_PLL1:
        source = CLK_GetPll1Freq();
        break;
    case CLKSEL_CLKOUTSEL_CLKREF:
        source = EXT_CLOCK_FREQUENCY_HZ;
        break;
    case CLKSEL_CLKOUTSEL_PLLG:
        source = CLK_GetPllGFreq();
        break;
    case CLKSEL_CLKOUTSEL_PLL2:
        source = CLK_GetPll2Freq();
        break;
    default:
        ASSERT(0);
    }

    return source / (READ_REG_FIELD(CLKDIV2, CLKDIV2_CLKOUTDIV) + 1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_SetPixelClock                                                                      */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                 This routine sets configuration of GFX pixel clock                                      */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_SetPixelClock (void)
{
    SET_REG_FIELD(CLKSEL, CLKSEL_PIXCKSEL, CLKSEL_PIXCKSEL_PLLG);

    return;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetPixelClock                                                                      */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                 This routine returns configuration of GFX clock                                         */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetPixelClock (void)
{
    UINT32 source = 0;
    UINT32 sel =  READ_REG_FIELD(CLKSEL, CLKSEL_PIXCKSEL);

    if (sel == CLKSEL_PIXCKSEL_PLLG)
        source = CLK_GetPllGFreq();
    else if (sel == CLKSEL_PIXCKSEL_CLKOUT_GPIO160)
        source = 50 * _1MHz_; // GFXBYPCK;
    else  // (pci_sel == CLKSEL_PIXCKSEL_CLKREF)
        source = EXT_CLOCK_FREQUENCY_HZ;

    return source;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetUSB_OHCI_Clock                                                                  */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                 This routine returns configuration of USB OHCI clock (48MHz)                            */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetUSB_OHCI_Clock (void)
{
    UINT32 source = 0;
    UINT32 sel =  READ_REG_FIELD(CLKSEL, CLKSEL_SUCKSEL);

    switch (sel)
    {
    case CLKSEL_SUCKSEL_PLL0:
        source = CLK_GetPll0Freq();
        break;
    case CLKSEL_SUCKSEL_PLL1:
        source = CLK_GetPll1Freq();
        break;
    case CLKSEL_SUCKSEL_PLL2:
        source = CLK_GetPll2Freq();
        break;
    case CLKSEL_SUCKSEL_CLKREF:
    default:
        source = EXT_CLOCK_FREQUENCY_HZ;
        break;
    }

    return source / ( READ_REG_FIELD(CLKDIV2, CLKDIV2_SU48CKDIV) + 1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetUSB_UTMI_Clock                                                                  */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                 This routine returns configuration of USB UTMI clock (30MHz)                            */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetUSB_UTMI_Clock (void)
{
    UINT32 source = 0;
    UINT32 sel =  READ_REG_FIELD(CLKSEL, CLKSEL_SUCKSEL);

    switch (sel)
    {
    case CLKSEL_SUCKSEL_PLL0:
        source = CLK_GetPll0Freq();
        break;
    case CLKSEL_SUCKSEL_PLL1:
        source = CLK_GetPll1Freq();
        break;
    case CLKSEL_SUCKSEL_PLL2:
        source = CLK_GetPll2Freq();
        break;
    case CLKSEL_SUCKSEL_CLKREF:
    default:
        source = EXT_CLOCK_FREQUENCY_HZ;
        break;
    }

    return source / ( READ_REG_FIELD(CLKDIV2, CLKDIV2_SUCKDIV) + 1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_Get_RC_Phy_and_I3C_Clock                                                           */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                 This routine returns configuration of RC PCIe phy clock (100MHz normaly)                */
/*                 This is also I3C clock                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_Get_RC_Phy_and_I3C_Clock (void)
{
    UINT32 source = 0;
    UINT32 sel =  READ_REG_FIELD(CLKSEL, CLKSEL_RCPCKSEL);

    switch (sel)
    {
    case CLKSEL_RCPCKSEL_PLL0:
        source = CLK_GetPll0Freq();
        break;
    case CLKSEL_RCPCKSEL_PLL1:
        source = CLK_GetPll1Freq();
        break;
    case CLKSEL_RCPCKSEL_CLKREF:
        source = EXT_CLOCK_FREQUENCY_HZ;
        break;
    default:
        ASSERT(0);
    }

    return source / ( READ_REG_FIELD(CLKDIV4, CLKDIV4_RCPREFDIV) + 1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetUartClock                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                 This routine returns configuration of UART clock                                        */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetUartClock (void)
{
    UINT32 source = 0;
    UINT32 sel =  READ_REG_FIELD(CLKSEL, CLKSEL_UARTCKSEL);

    switch (sel)
    {
    case CLKSEL_UARTCKSEL_PLL0:
        source = CLK_GetPll0Freq();
        break;
    case CLKSEL_UARTCKSEL_PLL1:
        source = CLK_GetPll1Freq();
        break;

    case CLKSEL_UARTCKSEL_PLL2:
        source = CLK_GetPll2Freq();
        break;

    case CLKSEL_UARTCKSEL_CLKREF:
    default:
        source = EXT_CLOCK_FREQUENCY_HZ;
        break;
    }

    return source / ( READ_REG_FIELD(CLKDIV1, CLKDIV1_UARTDIV1) + 1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_GetGMACClock                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                 This routine returns configuration of GMAC clock                                        */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 CLK_GetGMACClock (void)
{
    UINT32 source = 0;
    UINT32 sel =  READ_REG_FIELD(CLKSEL, CLKSEL_RGSEL);

    switch (sel)
    {
    case CLKSEL_RGSEL_PLL0:
        source = CLK_GetPll0Freq();
        break;
    case CLKSEL_RGSEL_PLL1:
        source = CLK_GetPll1Freq();
        break;
    case CLKSEL_RGSEL_CLKREF:
        source = EXT_CLOCK_FREQUENCY_HZ;
        break;
    default:
        ASSERT(0);
    }

    return source / ( READ_REG_FIELD(CLKDIV4, CLKDIV4_RGREFDIV) + 1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_PrintRegs                                                                          */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module registers                                               */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_PrintRegs (void)
{
    HAL_PRINT("CLK:\n");
    HAL_PRINT("------\n");
    HAL_PRINT("CLKEN1    = 0x%08lx     \n",  REG_READ(CLKEN1    ));
    HAL_PRINT("CLKEN2    = 0x%08lx     \n",  REG_READ(CLKEN2    ));
    HAL_PRINT("CLKEN3    = 0x%08lx     \n",  REG_READ(CLKEN3    ));
    HAL_PRINT("CLKEN4    = 0x%08lx     \n",  REG_READ(CLKEN4    ));
    HAL_PRINT("CLKSEL    = 0x%08lx     \n",  REG_READ(CLKSEL    ));
    HAL_PRINT("CLKDIV1   = 0x%08lx     \n",  REG_READ(CLKDIV1   ));
    HAL_PRINT("CLKDIV2   = 0x%08lx     \n",  REG_READ(CLKDIV2   ));
    HAL_PRINT("CLKDIV3   = 0x%08lx     \n",  REG_READ(CLKDIV3   ));
    HAL_PRINT("CLKDIV4   = 0x%08lx     \n",  REG_READ(CLKDIV4   ));
    HAL_PRINT("PLLCON0   = 0x%08lx     \n",  REG_READ(PLLCON0   ));
    HAL_PRINT("PLLCON1   = 0x%08lx     \n",  REG_READ(PLLCON1   ));
    HAL_PRINT("PLLCON2   = 0x%08lx     \n",  REG_READ(PLLCON2   ));
    HAL_PRINT("PLLCONG   = 0x%08lx     \n",  REG_READ(PLLCONG   ));
    HAL_PRINT("SWRSTR    = 0x%08lx     \n",  REG_READ(SWRSTR    ));
    HAL_PRINT("IPSRST1   = 0x%08lx     \n",  REG_READ(IPSRST1   ));
    HAL_PRINT("IPSRST2   = 0x%08lx     \n",  REG_READ(IPSRST2   ));
    HAL_PRINT("IPSRST3   = 0x%08lx     \n",  REG_READ(IPSRST3   ));
    HAL_PRINT("IPSRST4   = 0x%08lx     \n",  REG_READ(IPSRST4   ));
    HAL_PRINT("WD0RCR    = 0x%08lx     \n",  REG_READ(WD0RCR    ));
    HAL_PRINT("WD1RCR    = 0x%08lx     \n",  REG_READ(WD1RCR    ));
    HAL_PRINT("WD2RCR    = 0x%08lx     \n",  REG_READ(WD2RCR    ));
    HAL_PRINT("SWRSTC1   = 0x%08lx     \n",  REG_READ(SWRSTC1   ));
    HAL_PRINT("SWRSTC2   = 0x%08lx     \n",  REG_READ(SWRSTC2   ));
    HAL_PRINT("SWRSTC3   = 0x%08lx     \n",  REG_READ(SWRSTC3   ));
    HAL_PRINT("TIPRSTC   = 0x%08lx     \n",  REG_READ(TIPRSTC   ));
    HAL_PRINT("CORSTC    = 0x%08lx     \n",  REG_READ(CORSTC    ));
    HAL_PRINT("WD0RCRB   = 0x%08lx     \n",  REG_READ(WD0RCRB   ));
    HAL_PRINT("WD1RCRB   = 0x%08lx     \n",  REG_READ(WD1RCRB   ));
    HAL_PRINT("WD2RCRB   = 0x%08lx     \n",  REG_READ(WD2RCRB   ));
    HAL_PRINT("SWRSTC1B  = 0x%08lx     \n",  REG_READ(SWRSTC1B  ));
    HAL_PRINT("SWRSTC2B  = 0x%08lx     \n",  REG_READ(SWRSTC2B  ));
    HAL_PRINT("SWRSTC3B  = 0x%08lx     \n",  REG_READ(SWRSTC3B  ));
    HAL_PRINT("TIPRSTCB  = 0x%08lx     \n",  REG_READ(TIPRSTCB  ));
    HAL_PRINT("CORSTCB   = 0x%08lx     \n",  REG_READ(CORSTCB   ));
    HAL_PRINT("AHBCKFI   = 0x%08lx     \n",  REG_READ(AHBCKFI   ));
    HAL_PRINT("SECCNT    = 0x%08lx     \n",  REG_READ(SECCNT    ));
    HAL_PRINT("CNTR25M   = 0x%08lx     \n",  REG_READ(CNTR25M   ));
    HAL_PRINT("BUSTO     = 0x%08lx     \n",  REG_READ(BUSTO     ));
    HAL_PRINT("IPSRSTDIS1    = 0x%08lx     \n",  REG_READ(IPSRSTDIS1 ));
    HAL_PRINT("IPSRSTDIS2    = 0x%08lx     \n",  REG_READ(IPSRSTDIS2 ));
    HAL_PRINT("IPSRSTDIS3    = 0x%08lx     \n",  REG_READ(IPSRSTDIS3 ));
    HAL_PRINT("IPSRSTDIS4    = 0x%08lx     \n",  REG_READ(IPSRSTDIS4 ));
    HAL_PRINT("CLKENDIS1     = 0x%08lx     \n",  REG_READ(CLKENDIS1 ));
    HAL_PRINT("CLKENDIS2     = 0x%08lx     \n",  REG_READ(CLKENDIS2 ));
    HAL_PRINT("CLKENDIS3     = 0x%08lx     \n",  REG_READ(CLKENDIS3 ));
    HAL_PRINT("CLKENDIS4     = 0x%08lx     \n",  REG_READ(CLKENDIS4 ));
    HAL_PRINT("THRTL_CNT     = 0x%08lx     \n",  REG_READ(THRTL_CNT ));

    HAL_PRINT("\n");
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CLK_PrintVersion                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module version                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void CLK_PrintVersion (void)
{
    HAL_PRINT("CLK        = %d\n", CLK_MODULE_TYPE);
}

