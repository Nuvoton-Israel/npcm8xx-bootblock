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



DEFS_STATUS FUSE_WRPR_get            (UINT16 fuse_address, UINT16 fuse_length, FUSE_ECC_TYPE_T fuse_ecc, UINT8* value);

#endif // _FUSE_WRAPPER_
#endif // #ifdef FUSE_MODULE_TYPE

