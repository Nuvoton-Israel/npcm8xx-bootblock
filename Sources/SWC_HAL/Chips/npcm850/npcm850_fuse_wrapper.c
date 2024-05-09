/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*                         fuse_wrapper.c                                     */
/*                                                                            */
/* This file contains fuse wrapper implementation. it wraps all access to the otp */
/*                                                                            */
/*  Project:  Arbel                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#if defined (FUSE_MODULE_TYPE)


#include "npcm850_fuse_wrapper.h"
#include <string.h>


/*---------------------------------------------------------------------------------------------------------*/
/* This global array is used to host the full encloded key                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define        FUSE_ARRAY_MAX_SIZE    256
static UINT8   fuse_encoded[FUSE_ARRAY_MAX_SIZE] = {0};


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_WRPR_get                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fuse_address    -   address in the fuse\key array                                      */
/*                  fuse_length     -   length in bytes inside the fuse array (before encoding)            */
/*                  fuse_ecc        -   nible parity\majority\none                                         */
/*                  value           -   output buffer (the decoded data)                                   */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine read a value from the fuses.                                              */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FUSE_WRPR_get (UINT16 fuse_address, UINT16 fuse_length, FUSE_ECC_TYPE_T fuse_ecc, UINT8* value)
{
    UINT16 iCnt = 0;

    DEFS_STATUS status = DEFS_STATUS_OK;

    DEFS_STATUS_COND_CHECK(fuse_length <= FUSE_ARRAY_MAX_SIZE, DEFS_STATUS_INVALID_PARAMETER);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Read the fuses                                                                                      */
    /*-----------------------------------------------------------------------------------------------------*/
    // if no parity
    if ( (FUSE_ECC_TYPE_T)fuse_ecc == FUSE_ECC_NONE )
    {
        for (iCnt = 0; iCnt < fuse_length ; iCnt++)
        {
            FUSE_Read(0, fuse_address + iCnt , &value[iCnt]);
        }
    }
    else
    {
        for (iCnt = 0; iCnt < fuse_length ; iCnt++)
        {
            FUSE_Read(0, fuse_address + iCnt , &fuse_encoded[iCnt]);
        }

    }


    return status;
}


#endif // FUSE_MODULE_TYPE

