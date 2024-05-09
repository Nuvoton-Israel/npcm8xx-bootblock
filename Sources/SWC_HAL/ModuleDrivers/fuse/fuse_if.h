/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   fuse_if.h                                                                */
/*            This file contains API of FUSE module routines.                 */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/
#ifndef FUSE_IF_H
#define FUSE_IF_H

#include "hal.h"


#if defined (AES_MODULE_TYPE)
#include __MODULE_IF_HEADER_FROM_IF(aes)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Fuse module definitions                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define FUSE_ARR_BYTE_SIZE          128
#define KEYS_ARR_BYTE_SIZE          128


/*---------------------------------------------------------------------------------------------------------*/
/* Fuse ECC type                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum FUSE_ECC_TYPE_tag
{
    FUSE_ECC_MAJORITY = 0,
    FUSE_ECC_NIBBLE_PARITY = 1,
    FUSE_ECC_64_72 = 2,
    FUSE_ECC_NONE = 3
}  FUSE_ECC_TYPE_T;

/*---------------------------------------------------------------------------------------------------------*/
/* Fuse key Type                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum FUSE_KEY_TYPE_tag
{
    FUSE_KEY_AES = 0,
    FUSE_KEY_RSA = 1,
    FUSE_KEY_ECC = 2
}  FUSE_KEY_TYPE_T;



/*---------------------------------------------------------------------------------------------------------*/
/* Fuse module enumerations                                                                                */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Storage Array Type:                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum
{
    KEY_SA    = 0,
    FUSE_SA   = 1
} FUSE_STORAGE_ARRAY_T;


/*---------------------------------------------------------------------------------------------------------*/
/* FUSTRAP fields definition                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum FUSE_FUSTRAP_FIELDS_T_tag
{
    FUSE_FUSTRAP_oWDEN = 27,                            // (Watchdog Enable).
    FUSE_FUSTRAP_oHLTOF = 26,                           // (Halt on Failure). I
    FUSE_FUSTRAP_oAESKEYACCLK = 25,                     // (AES Key Access Lock).
    FUSE_FUSTRAP_oJDIS = 24,                            // (JTAG Disable).
    FUSE_FUSTRAP_oSECBOOT = 23,                         // (Secure Boot).
    FUSE_FUSTRAP_USEFUSTRAP = 22,                       //
    FUSE_FUSTRAP_oAltImgLoc = 18,                       //  Alternate image location definition.

    /*-----------------------------------------------------------------------------------------------------*/
    /* Added on Z2                                                                                         */
    /*-----------------------------------------------------------------------------------------------------*/
    FUSE_FUSTRAP_oHINDDIS         =      14,            // oHINDDIS: disable eSPI independent mode
    FUSE_FUSTRAP_oCPU1STOP2 =            16,            // {oCPU1STOP2} - when set, stops CPU core 1 clock.
    FUSE_FUSTRAP_oCPU1STOP1 =            17             // {oCPU1STOP1} - when set, CPU core 1 stops and cannot be woken.


} FUSE_FUSTRAP_FIELDS_T;


/*---------------------------------------------------------------------------------------------------------*/
/* Fuse module exported functions                                                                          */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* HW level functions:                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#ifdef FUSE_CAPABILITY_SECURE_VALUE
void          FUSE_Set_Prog_Magic_Word (UINT32 value, BOOLEAN init);
UINT32        FUSE_Get_Prog_Magic_Word (void);
#endif

DEFS_STATUS   FUSE_Init                      (void);
void          FUSE_Read                      (FUSE_STORAGE_ARRAY_T arr, UINT16 addr, UINT8 *data);


#if defined (AES_MODULE_TYPE)
void          FUSE_UploadKey                 (AES_KEY_SIZE_T keySize, UINT8 keyIndex);
#endif

#if defined (AES_MODULE_TYPE) || defined ( RSA_MODULE_TYPE)
DEFS_STATUS   FUSE_ReadKey                   (FUSE_KEY_TYPE_T  keyType, UINT8  keyIndex, UINT8 *output);
DEFS_STATUS   FUSE_SelectKey                 (UINT8 keyIndex);
#endif

void          FUSE_DisableKeyAccess          (void);
void          FUSE_SetBlockAccess            (FUSE_STORAGE_ARRAY_T array, UINT32 block, BOOLEAN lockForRead, BOOLEAN lockForWrite, BOOLEAN lockRegister);
DEFS_STATUS   FUSE_LockAccess                (FUSE_STORAGE_ARRAY_T array, UINT32 lockForRead, UINT32 lockForWrite, BOOLEAN lockRegister);

/*---------------------------------------------------------------------------------------------------------*/
/* ECC handling                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS   FUSE_NibParEccDecode           (const UINT8 *datain,    UINT8 *dataout,    UINT32  encoded_size);
DEFS_STATUS   FUSE_MajRulEccDecode           (const UINT8 *datain,    UINT8 *dataout,    UINT32  encoded_size);


#ifdef FUSE_CAP_GET_FUSTRAP_FIELDS
/*---------------------------------------------------------------------------------------------------------*/
/* FUSTRAP register getter                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
UINT          FUSE_Fustrap_Get               (FUSE_FUSTRAP_FIELDS_T oFuse);
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Print functions:                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void          FUSE_PrintRegs                 (void);
void          FUSE_PrintModuleRegs           (FUSE_STORAGE_ARRAY_T array);
void          FUSE_PrintVersion              (void);


#endif /* FUSE_IF_H */
