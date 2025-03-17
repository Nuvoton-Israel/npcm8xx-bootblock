/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*    hal_secured.h                                                           */
/*            This file contains HAL security utilities                       */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/

#ifndef __HAL_SECURED_H__
#define __HAL_SECURED_H__

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                           INTERFACE FUNCTIONS                                           */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

typedef void (*SecureErrorCallback) (UINT32 error, UINT32 line);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        HAL_SEC_memcmp_action                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ptr1                  - Pointer to a block of memory                                   */
/*                  ptr2                  - Pointer to a block of memory                                   */
/*                  n                     - Number of bytes to be compared.                                */
/*                  secure_error_callback - Callbak function in case of a security error detection.        */
/*                                                                                                         */
/* Returns:         SECURED_BOOLEAN_T                                                                      */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This function compares the first n bytes of memory pointed by ptr1 to the first n      */
/*                  bytes pointed by ptr2, returns SECURED_TRUE if they all match or SECURED_FALSE if not  */
/*                  If the function detects an attempt to glitch the code it will call the                 */
/*                  secure_error_callback function (if not NULL)                                           */
/*---------------------------------------------------------------------------------------------------------*/
SECURED_BOOLEAN_T HAL_SEC_memcmp_action (const void* ptr1, const void* ptr2, UINT n,
                                         SecureErrorCallback secure_error_callback);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        HAL_SEC_memcmp                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  ptr1    - Pointer to a block of memory                                                 */
/*                  ptr2    - Pointer to a block of memory                                                 */
/*                  n       - Number of bytes to be compared.                                              */
/*                                                                                                         */
/* Returns:         SECURED_BOOLEAN_T                                                                      */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This function compares the first n bytes of memory pointed by ptr1 to the first n      */
/*                  bytes pointed by ptr2, returns SECURED_TRUE if they all match or SECURED_FALSE if not  */
/*---------------------------------------------------------------------------------------------------------*/
SECURED_BOOLEAN_T HAL_SEC_memcmp (const void* ptr1, const void* ptr2, UINT n);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        HAL_SEC_memcpy8                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  dest    - Pointer to the destination array where the content is to be copied           */
/*                  src     - Pointer to the source of data to be copied                                   */
/*                  n       - Number of bytes to be copied.                                                */
/*                  rnd     - Random value                                                                 */
/*                                                                                                         */
/* Returns:         a pointer to destination, which is dest.                                               */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This function copies from source pointer to destination n bytes (byte by byte).        */
/*                  It uses rnd value to start the copy from a random index.                               */
/*---------------------------------------------------------------------------------------------------------*/
void* HAL_SEC_memcpy8 (UINT8* dest, const UINT8* src, UINT n, UINT rnd);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        HAL_SEC_memcpy32                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  dest    - Pointer to the destination array where the content is to be copied           */
/*                  src     - Pointer to the source of data to be copied                                   */
/*                  n       - Number of bytes to be copied.                                                */
/*                  rnd     - Random value                                                                 */
/*                                                                                                         */
/* Returns:         a pointer to destination, which is dest.                                               */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This function copies 32-bits words from source pointer to destination (n bytes total). */
/*                  It uses rnd value to start the copy from a random index.                               */
/*                                                                                                         */
/*                  NOTE: Use this function only when dest and src addresses and n are 32-bit alligned!    */
/*---------------------------------------------------------------------------------------------------------*/
void* HAL_SEC_memcpy32 (UINT32* dest, const UINT32* src, UINT n, UINT rnd);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        HAL_SEC_memcpy                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  dest    - Pointer to the destination array where the content is to be copied           */
/*                  src     - Pointer to the source of data to be copied                                   */
/*                  n       - Number of bytes to be copied.                                                */
/*                  rnd     - Random value                                                                 */
/*                                                                                                         */
/* Returns:         a pointer to destination, which is dest.                                               */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This function copies from source pointer to destination. The function checks the       */
/*                  alignment of the given addresses and size and call the suitable function to perform    */
/*                  the copying.                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void* HAL_SEC_memcpy (void* dest, const void* src, UINT n, UINT rnd);

#endif /* __HAL_SECURED_H__ */

