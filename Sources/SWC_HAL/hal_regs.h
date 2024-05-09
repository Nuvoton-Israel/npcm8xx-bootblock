/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   hal_regs.h                                                               */
/*            This file contains the HAL registers                            */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/

#ifndef _HAL_REGS_H
#define _HAL_REGS_H

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                             CHIP DEPENDENT                                              */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#if !defined SWC_HAL_FLAT
#include "Chips/chip_regs.h"
#else
#include "chip_regs.h"
#endif


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                               MODULE REGS                                               */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* ADC Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined ADC_MODULE_TYPE
#include __MODULE_REGS_HEADER(adc, ADC_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* AES Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined AES_MODULE_TYPE
#include __MODULE_REGS_HEADER(aes, AES_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* BBRM Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined BBRM_MODULE_TYPE
#include __MODULE_REGS_HEADER(bbrm, BBRM_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* CLK Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined CLK_MODULE_TYPE
#include __MODULE_REGS_HEADER(clk, CLK_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* CM Module Driver                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#if defined CM_MODULE_TYPE
#include __MODULE_REGS_HEADER(cm, CM_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* CPS Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined CPS_MODULE_TYPE
#include __MODULE_REGS_HEADER(cps, CPS_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* CR UART Module Driver                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#if defined CR_UART_MODULE_TYPE
#include __MODULE_REGS_HEADER(cr_uart, CR_UART_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* DAC Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined DAC_MODULE_TYPE
#include __MODULE_REGS_HEADER(dac, DAC_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* DCU Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined DCU_MODULE_TYPE
#include __MODULE_REGS_HEADER(dcu, DCU_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* DES Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined DES_MODULE_TYPE
#include   __MODULE_REGS_HEADER(des, DES_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* DP80 Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined DP80_MODULE_TYPE
#include __MODULE_REGS_HEADER(dp80, DP80_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* DWT Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined DWT_MODULE_TYPE
#include __MODULE_REGS_HEADER(dwt, DWT_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* ECE Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined ECE_MODULE_TYPE
#include __MODULE_REGS_HEADER(ece, ECE_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* EMC Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined EMC_MODULE_TYPE
#include __MODULE_REGS_HEADER(emc, EMC_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* ESPI Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined ESPI_MODULE_TYPE
#include __MODULE_REGS_HEADER(espi, ESPI_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* EWOC Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined EWOC_MODULE_TYPE
#include __MODULE_REGS_HEADER(ewoc, EWOC_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* FIU Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined FIU_MODULE_TYPE || defined FIU_MODULE_TYPE_EXT
#include __MODULE_REGS_HEADER(fiu, FIU_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* FLM Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined FLM_MODULE_TYPE
#include __MODULE_REGS_HEADER(flm, FLM_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* FUSE Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined FUSE_MODULE_TYPE
#include __MODULE_REGS_HEADER(fuse, FUSE_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* GDMA Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined GDMA_MODULE_TYPE
#include __MODULE_REGS_HEADER(gdma, GDMA_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* GIC Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined GIC_MODULE_TYPE
#include __MODULE_REGS_HEADER(gic, GIC_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* GMAC Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined GMAC_MODULE_TYPE
#include __MODULE_REGS_HEADER(gmac, GMAC_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* GPIO Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined GPIO_MODULE_TYPE
#include __MODULE_REGS_HEADER(gpio, GPIO_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* HFCG Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined HFCG_MODULE_TYPE
#include __MODULE_REGS_HEADER(hfcg, HFCG_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* ICU Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined ICU_MODULE_TYPE
#include __MODULE_REGS_HEADER(icu, ICU_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* ITIM8 Module Driver                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#if defined ITIM8_MODULE_TYPE
#include __MODULE_REGS_HEADER(itim8, ITIM8_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* ITIM32 Module Driver                                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#if defined ITIM32_MODULE_TYPE
#include __MODULE_REGS_HEADER(itim32, ITIM32_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* ITIM64 Module Driver                                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#if defined ITIM64_MODULE_TYPE
#include __MODULE_REGS_HEADER(itim64, ITIM64_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* I3C Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined I3C_MODULE_TYPE
#include __MODULE_REGS_HEADER(i3c, I3C_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* KBC Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined KBC_MODULE_TYPE
#include __MODULE_REGS_HEADER(kbc, KBC_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* KBC HOST Module Driver                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#if defined KBC_HOST_MODULE_TYPE
#include __MODULE_REGS_HEADER(kbc_host, KBC_HOST_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Keyboard scan module Driver                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#if defined KBSCAN_MODULE_TYPE
#include __MODULE_REGS_HEADER(kbscan, KBSCAN_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* LCT Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined LCT_MODULE_TYPE
#include __MODULE_REGS_HEADER(lct, LCT_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* LFCG Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined LFCG_MODULE_TYPE
#include __MODULE_REGS_HEADER(lfcg, LFCG_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* MC Module Driver                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#if defined MC_MODULE_TYPE
#include __MODULE_REGS_HEADER(mc, MC_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* MDMA Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined MDMA_MODULE_TYPE
#include __MODULE_REGS_HEADER(mdma, MDMA_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* MFT Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined MFT_MODULE_TYPE
#include __MODULE_REGS_HEADER(mft, MFT_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* MIWU Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined MIWU_MODULE_TYPE
#include __MODULE_REGS_HEADER(miwu, MIWU_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* MPU Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined MPU_MODULE_TYPE
#include __MODULE_REGS_HEADER(mpu, MPU_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* MRC Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined MRC_MODULE_TYPE
#include __MODULE_REGS_HEADER(mrc, MRC_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* MSWC Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined MSWC_MODULE_TYPE
#include __MODULE_REGS_HEADER(mswc, MSWC_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* MTC Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined MTC_MODULE_TYPE
#include __MODULE_REGS_HEADER(mtc, MTC_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* NVIC Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined NVIC_MODULE_TYPE
#include __MODULE_REGS_HEADER(nvic, NVIC_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* OTP Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined OTP_MODULE_TYPE
#include __MODULE_REGS_HEADER(otp, OTP_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* OTPI Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined OTPI_MODULE_TYPE
#include __MODULE_REGS_HEADER(otpi, OTPI_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* PECI Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined PECI_MODULE_TYPE
#include __MODULE_REGS_HEADER(peci, PECI_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* PKA Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined PKA_MODULE_TYPE
#include __MODULE_REGS_HEADER(pka, PKA_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* PM Channel Module Driver                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
#if defined PM_CHAN_MODULE_TYPE
#include __MODULE_REGS_HEADER(pm_chan, PM_CHAN_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* PMC Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined PMC_MODULE_TYPE
#include __MODULE_REGS_HEADER(pmc, PMC_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* PS/2 module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined PS2_MODULE_TYPE
#include __MODULE_REGS_HEADER(ps2, PS2_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* PWM Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined PWM_MODULE_TYPE
#include __MODULE_REGS_HEADER(pwm, PWM_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* RNG Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined RNG_MODULE_TYPE
#include __MODULE_REGS_HEADER(rng, RNG_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* SCS Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined SCS_MODULE_TYPE
#include __MODULE_REGS_HEADER(scs, SCS_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* SD Module Driver                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#if defined SD_MODULE_TYPE
#include __MODULE_REGS_HEADER(sd, SD_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* SHA Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined SHA_MODULE_TYPE
#include __MODULE_REGS_HEADER(sha, SHA_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* SHI Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined SHI_MODULE_TYPE
#include __MODULE_REGS_HEADER(shi, SHI_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* SHM Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined SHM_MODULE_TYPE
#include __MODULE_REGS_HEADER(shm, SHM_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* SIB Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined SIB_MODULE_TYPE
#include __MODULE_REGS_HEADER(sib, SIB_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* SIO Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined SIO_MODULE_TYPE
#include __MODULE_REGS_HEADER(sio, SIO_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* SMB Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined SMB_MODULE_TYPE
#include __MODULE_REGS_HEADER(smb, SMB_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* SPI Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined SPI_MODULE_TYPE
#include __MODULE_REGS_HEADER(spi, SPI_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* SSL Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined SSL_MODULE_TYPE
#include __MODULE_REGS_HEADER(ssl, SSL_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* STI Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined STI_MODULE_TYPE
#include __MODULE_REGS_HEADER(sti, STI_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* SWC Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined SWC_MODULE_TYPE
#include __MODULE_REGS_HEADER(swc, SWC_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* SYST Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined SYST_MODULE_TYPE
#include __MODULE_REGS_HEADER(syst, SYST_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* TMC Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined TMC_MODULE_TYPE
#include __MODULE_REGS_HEADER(tmc, TMC_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* TWD Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined TWD_MODULE_TYPE
#include __MODULE_REGS_HEADER(twd, TWD_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* UART Module Driver                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#if defined UART_MODULE_TYPE
#include __MODULE_REGS_HEADER(uart, UART_MODULE_TYPE)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* VCD Module Driver                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#if defined VCD_MODULE_TYPE
#include __MODULE_REGS_HEADER(vcd, VCD_MODULE_TYPE)
#endif


#endif /* _HAL_REGS_H */


