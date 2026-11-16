/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*                         fuse_wrapper.h                                     */
/* This file contains fuse wrapper implementation. it wraps all access to the otp. */
/* For the user: call FUSE_WRPR_set or FUSE_WRPR_get with the property fields. */
/*  Project:  Arbel                                                           */
/*----------------------------------------------------------------------------*/
#if defined (FUSE_MODULE_TYPE)

#ifndef _FUSE_WRAPPER_
#define _FUSE_WRAPPER_

#include "defs.h"

#define DAC_CAL_PROPERTY                32,         8,          FUSE_ECC_NIBBLE_PARITY
#define ADC_CAL_INT_PROPERTY            40,         8,          FUSE_ECC_NIBBLE_PARITY
#define ADC_CAL_EXT_PROPERTY            48,         8,          FUSE_ECC_NIBBLE_PARITY
#define DEVICE_ID_PROPERTY              168,        5,          FUSE_ECC_NONE
#define DIE_INFORMATION_PROPERTY        187,        5,          FUSE_ECC_NONE

DEFS_STATUS FUSE_WRPR_get            (UINT16 fuse_address, UINT16 fuse_length, FUSE_ECC_TYPE_T fuse_ecc, UINT8* value);

#endif // _FUSE_WRAPPER_
#endif // #ifdef FUSE_MODULE_TYPE

