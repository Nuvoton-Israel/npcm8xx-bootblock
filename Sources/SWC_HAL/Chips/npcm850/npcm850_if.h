/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   npcm850_if.h                                                             */
/*            This file contains chip dependent interface                     */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/

#ifndef __NPCM850_IF_H__
#define __NPCM850_IF_H__

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                                 INCLUDES                                                */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Module Dependencies                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/

#if defined (ADC_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(adc)
#endif

#if defined (AES_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(aes)
#endif

#if defined (CLK_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(clk)
#endif

#if defined (DAC_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(dac)
#endif

#if defined (FIU_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(fiu)
#endif

#if defined (FUSE_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(fuse)
#include "npcm850_fuse_wrapper.h"
#endif

#if defined (GDMA_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(gdma)
#endif

#if defined (GPIO_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(gpio)
#endif

#if defined (MFT_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(mft)
#endif

#if defined (PECI_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(peci)
#endif

#if defined (PS2_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(ps2)
#endif

#if defined (PWM_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(pwm)
#endif

#if defined (SD_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(sd)
#endif

#if defined (SHM_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(shm)
#endif

#if defined (SMB_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(smb)
#endif

#if defined (SPI_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(spi)
#endif

#if defined (UART_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_CHIP_DRV(uart)
#endif


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                           TYPES & DEFINITIONS                                           */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

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
DEFS_STATUS CHIP_Mux_Uart (UART_DEV_T dev, BOOLEAN CoreSP, BOOLEAN sp1, BOOLEAN sp2);
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
void SPI_Quad_Enable(FIU_MODULE_T devNum, FIU_CS_T FIU_cs);

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
                            BOOLEAN           quadMode);
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
void CHIP_Mux_RMII(UINT devNum);

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
void CHIP_SetDacSense(void);

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
void CHIP_Mux_GMII(UINT devNum);

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
void CHIP_Mux_SIOXH(BOOLEAN enable);

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
void CHIP_MuxSPI (SPI_MODULE_T devNum);
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
void CHIP_Mux_GSPI (BOOLEAN bOn);

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
UINT32 CHIP_PowerOn_GetMemorySize (void);
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
void CHIP_Mux_GPIO(UINT gpio_num);

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
DEFS_STATUS CHIP_Mux_SD(SD_DEV_NUM_T sdNum, SD_DEV_TYPE_T sdType);

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
void CHIP_EmmcBootTimeConfig (UINT32 clk);

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
void CHIP_EmmcBootStart (BOOLEAN ack);

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
BOOLEAN CHIP_EmmcBootInvalid (void);

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
void CHIP_EmmcBootEnd (void);
#endif //  ( SD_MODULE_TYPE)

#if defined (VCD_MODULE_TYPE)
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
void CHIP_SetVCDInput (/* TODO: CHIP_VCD_HEAD_T headNum */ void);

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
void CHIP_SetGFXConfig (/* TODO: VCD_CONFIG_T config */ void);
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
void CHIP_ConfigUSBBurstSize (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_ResetTypeIsPor                                                                    */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Returns true if reset source was POR (powerOn) reset                                   */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_ResetTypeIsPor (void);

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
BOOLEAN CHIP_ResetTypeIsCor (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_MarkCfgDone                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Sets a flag in scratch register specifying that CLK/DDR configuration were performed   */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_MarkCfgDone (void);

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
BOOLEAN CHIP_CfgWasDone (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_EnableJtag                                                                        */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Enable Jtag access                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_EnableJtag (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_DisableJtag                                                                       */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Disable Jtag access                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_DisableJtag (void);

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
void CHIP_DisableModules (UINT32 mdlr);


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
BOOLEAN CHIP_MuxPECI  (void);
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
void CHIP_MuxADC (ADC_INPUT_T adc_module);
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
void CHIP_MuxPWM (PWM_MODULE_T pwm_module);
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
void CHIP_MuxMFT (MFT_MODULE_T mft_module, MFT_TIMER_T mft_timer);
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
void CHIP_SMBPullUp (SMB_MODULE_T module_num, BOOLEAN enable);

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
void CHIP_MuxSMB (SMB_MODULE_T smb_module, BOOLEAN bEnable);

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
void CHIP_Mux_Segment (SMB_MODULE_T smb_module, UINT32 segment, BOOLEAN bEnable);

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
DEFS_STATUS CHIP_SMB_GetGpioNumbers (SMB_MODULE_T smb_module, UINT segment, INT* ret_scl_gpio, INT* ret_sda_gpio);

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
void CHIP_MuxSMB2Segment (SMB_MODULE_T smb_module, UINT segment);
#endif // if 0

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
DEFS_STATUS CHIP_SMB_GetSegment (SMB_MODULE_T smb_module, UINT* segment);
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
void CHIP_MuxDAC (DAC_INPUT_T dac_input);
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
UINT32   CHIP_Get_Version (void);

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
void   CHIP_Disable_CPU1 (void);

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
BOOLEAN CHIP_MuxESPI (UINT driveStrength_mA);
#endif // ESPI_MODULE_TYPE

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_MuxLPC                                                                            */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         TRUE if LPC is enabled; FALSE otherwise.                                               */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine enables the LPC                                                           */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_MuxLPC (UINT driveStrength_mA);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_StrapJ3EN                                                                         */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine gets strap 11 (Jtag3 Enable) value.                                       */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_StrapJ3EN (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_StrapFUP                                                                          */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine gets strap 9 (Flash UART Programming) value.                              */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_StrapFUP (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_StrapSECEN                                                                        */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine gets strap 8 (Security Enable) value.                                     */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_StrapSECEN (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_StrapBSPA                                                                         */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine gets strap 5 (BMC debug Serial Port) value.                               */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_StrapBSPA (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_StrapJ2EN                                                                         */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine gets strap 4 (J2EN) value.                                                */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_StrapJ2EN (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_StrapSPI0F18                                                                      */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine gets strap 3 (SPI0F18) value.                                             */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN CHIP_StrapSPI0F18 (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_PrintRegs                                                                         */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the chip registers                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_PrintRegs (void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        CHIP_PrintRegs_GCR                                                                     */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the chip registers                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void CHIP_PrintRegs_GCR (void);

#endif //__NPCM850_IF_H__

