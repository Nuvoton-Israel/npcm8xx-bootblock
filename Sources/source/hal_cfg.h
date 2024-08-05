/*----------------------------------------------------------------------------*/
/*   SPDX-License-Identifier: GPL-2.0                                         */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*       hal_cfg.h                                                            */
/*            Arbel ARBEL A35 BOOTBLOCK HAL main modules                      */
/* Project:   Arbel ARBEL A35 BOOTBLOCK                                       */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifndef _BOOTBLOCK_HAL_CFG_H_
#define _BOOTBLOCK_HAL_CFG_H_

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                             UNUSED MODULES                                              */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#ifndef __GNUC__
#define __GNUC__
#endif


/*---------------------------------------------------------------------------------------------------------*/
/* #define NPCM850  1                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef __LP64__
#define  __LP64__
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Module Drivers exclusion                                                                                */
/*---------------------------------------------------------------------------------------------------------*/

#undef  GIC_MODULE_TYPE
//#undef FUSE_MODULE_TYPE
#undef  STRP_MODULE_TYPE
//#undef  FIU_MODULE_TYPE

//#undef  GPIO_MODULE_TYPE
#undef  PM_CHAN_MODULE_TYPE
#undef  OTP_MODULE_TYPE
#undef  SHA_MODULE_TYPE
#undef  PKA_MODULE_TYPE
#undef  EWOC_MODULE_TYPE
#undef  TWD_MODULE_TYPE
#undef  ICU_MODULE_TYPE
#undef  NVIC_MODULE_TYPE
#undef  SCS_MODULE_TYPE
#undef  SHI_MODULE_TYPE
#undef  MFT_MODULE_TYPE
#undef  DCU_MODULE_TYPE
#undef  ITIM8_MODULE_TYPE
#undef  PWM_MODULE_TYPE
#undef  SWC_MODULE_TYPE
#undef  HFCG_MODULE_TYPE
#undef  STI_MODULE_TYPE
#undef  KBC_HOST_MODULE_TYPE
#undef  SMB_MODULE_TYPE
#undef  ADC_MODULE_TYPE
#undef  PMC_MODULE_TYPE
#undef  SIO_MODULE_TYPE
#undef  GDMA_MODULE_TYPE
#undef  LFCG_MODULE_TYPE
//#undef  SHM_MODULE_TYPE
#undef  SCFG_MODULE_TYPE
#undef  CR_UART_MODULE_TYPE
#undef  MTC_MODULE_TYPE
#undef  DAC_MODULE_TYPE
#undef  KBSCAN_MODULE_TYPE
#undef  SYST_MODULE_TYPE
#undef  MIWU_MODULE_TYPE
#undef  RNG_MODULE_TYPE
#undef  SIB_MODULE_TYPE
#undef  CPS_MODULE_TYPE
#undef  MSWC_MODULE_TYPE
#undef  PS2_MODULE_TYPE
#undef  PECI_MODULE_TYPE
#undef  SCS_MODULE_TYPE
#undef  DES_MODULE_TYPE
#undef  EMC_MODULE_TYPE
#undef  VCD_MODULE_TYPE

#undef  AES_MODULE_TYPE

//#undef  ESPI_MODULE_TYPE
#define ESPI_CAPABILITY_HOST_STATUS
#undef  SPI_MODULE_TYPE


#undef  SD_MODULE_TYPE
// #undef  MC_MODULE_TYPE
#undef  ECE_MODULE_TYPE
#undef  EMC_MODULE_TYPE
#undef  GMAC_MODULE_TYPE
//#undef  UART_MODULE_TYPE
//#undef FUSE_MODULE_TYPE

/*---------------------------------------------------------------------------------------------------------*/
/* Logical Drivers exclusion                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#undef  CIRLED_MODULE_TYPE
#undef  FLASH_DEV_MODULE_TYPE
#undef  TACHO_MODULE_TYPE
#undef  ACPI_MODULE_TYPE
#undef  TIMER_MODULE_TYPE
#undef  CIR_MODULE_TYPE
#undef  FLASH_MODULE_TYPE
#undef  RTC_MODULE_TYPE

/*---------------------------------------------------------------------------------------------------------*/
/*                                             LOGICAL DRIVERS                                             */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Flash SPI Module                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
// #define FLASH_DEV_MODULE_TYPE               hw_fiu

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                           PERIPHERAL DRIVERS                                            */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#define PRINT_FLOAT(x) (x) / 1000000, (((x) % 1000000) / 1000)

#define PRINT_FLOAT2(x) (x) / 1000, (((x) % 1000))


#define ESPI_CAPABILITY_HOST_STATUS
#define ESPI_FATAL_ERROR_ERRATA_ISSUE

#include "./apps/serial_printf/serial_printf.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Print function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define HAL_PRINT_CAPABILITY

/*---------------------------------------------------------------------------------------------------------*/
/* Full debug. Used for Nuvoton.                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#if   defined(DEBUG_LOG) || defined(DEV_LOG)
#define HAL_PRINT(fmt,args...)   serial_printf(fmt ,##args)
#else
#define         HAL_PRINT(x,...)                   (void)0
#endif

#endif // _BOOTBLOCK_HAL_CFG_H_

