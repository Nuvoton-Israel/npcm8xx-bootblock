/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   fiu_drv.h                                                                */
/*            This file contains FIU driver interface                         */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/

#ifndef _FIU_DRV_H
#define _FIU_DRV_H

#define FIU_CAPABILITY_QUAD_READ
#define FIU_CAPABILITY_SUPPORT_64MB_FLASH
#define FIU_CAPABILITY_SUPPORT_128MB_FLASH
#define FIU_CAPABILITY_SPI_X
#define FIU_CAPABILITY_4B_ADDRESS
#define FIU_CAPABILITY_CONFIG_DUMMY_CYCLES
#define FIU_CAPABILITY_ADDR_MAP
#define FIU_CAPABILITY_PROTECTION

#define FIU_MAX_UMA_DATA_SIZE 16


// not supported:
//#define FIU_CAPABILITY_SEC_CHIP_SELECT


#define    FIU_READ_STATUS_REG_PERIOD        1     /* Usec TODO update  */
#define    FIU_USEC_TO_LOOP_COUNT(timeout)   (timeout / FIU_READ_STATUS_REG_PERIOD)


typedef enum
{
    FIU_TRANS_STATUS_DONE        = 0,
    FIU_TRANS_STATUS_IN_PROG     = 1
} FIU_TRANS_STATUS_T;


#endif  /* _FIU_DRV_H */

