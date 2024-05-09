/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*    hal_secured.c                                                           */
/*            This file contains HAL security utilities                       */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                                INCLUDES                                                 */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#include "hal_secured.h"


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                           TYPES & DEFINITIONS                                           */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           HAL_SEC_TRIPLE_OR                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  cond    - condition to check. Variables involved in the condition should be defined    */
/*                            'volatile' in the caller code                                                */
/*                                                                                                         */
/* Returns:         boolean indicating condition satisfaction                                              */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine checks a condition in a secured manner                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define HAL_SEC_TRIPLE_OR(cond)    ((cond) || (cond) || (cond))

/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           HAL_SEC_TRIPLE_AND                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  cond    - condition to check. Variables involved in the condition should be defined    */
/*                            'volatile' in the caller code                                                */
/*                                                                                                         */
/* Returns:         boolean indicating condition satisfaction                                              */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine checks a condition in a secured manner                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define HAL_SEC_TRIPLE_AND(cond)   ((cond) && (cond) && (cond))


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                           INTERFACE FUNCTIONS                                           */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

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
SECURED_BOOLEAN_T HAL_SEC_memcmp (const UINT8* ptr1, const UINT8* ptr2, UINT n)
{
    volatile SECURED_BOOLEAN_T  retVal = SECURED_FALSE;
    volatile SECURED_BOOLEAN_T  result = SECURED_TRUE;
    UINT                        i;
    const volatile UINT8*       ptr1_l = ptr1; // so DEFS_SEC_TRIPLE_OR will not optimize
    const volatile UINT8*       ptr2_l = ptr2; // so DEFS_SEC_TRIPLE_OR will not optimize

    DEFS_FLOW_MONITOR_DECLARE(0);

    for (i = 0; i < n; i++)
    {
        if (HAL_SEC_TRIPLE_OR(ptr1_l[i] != ptr2_l[i]))
        {
            result = SECURED_FALSE;
        }
    }

    DEFS_FLOW_MONITOR_INCREMENT();

    if (i != n)
    {
        result = SECURED_FALSE;
    }
    if (i != n)
    {
        result = SECURED_FALSE;
    }
    if (i != n)
    {
        result = SECURED_FALSE;
    }

    DEFS_FLOW_MONITOR_INCREMENT();

    if (HAL_SEC_TRIPLE_AND(result == SECURED_TRUE))
    {
        DEFS_FLOW_MONITOR_INCREMENT();
        retVal = SECURED_TRUE;
    }

    if (HAL_SEC_TRIPLE_AND(result == SECURED_TRUE))
    {
        DEFS_FLOW_MONITOR_INCREMENT();
        retVal = SECURED_TRUE;
    }

    DEFS_FLOW_MONITOR_COMPARE_ACTION(4, return SECURED_FALSE);

    return retVal;
}

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
void* HAL_SEC_memcpy8 (UINT8* dest, const UINT8* src, UINT n, UINT rnd)
{
    UINT start;
    UINT i;

    if (dest == NULL || n == 0)
    {
        return dest;
    }

    start = rnd % n;

    for (i = start; i < n; i++)
    {
        dest[i] = src[i];
    }

    for (i = 0; i < start; i++)
    {
        dest[i] = src[i];
    }

    return dest;
}

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
void* HAL_SEC_memcpy32 (UINT32* dest, const UINT32* src, UINT n, UINT rnd)
{
    UINT dwordNum = n / sizeof(UINT32);
    UINT start;
    UINT i;

    if (dest == NULL || dwordNum == 0)
    {
        return dest;
    }

    start = rnd % dwordNum;

    for (i = start; i < dwordNum; i++)
    {
        dest[i] = src[i];
    }

    for (i = 0; i < start; i++)
    {
        dest[i] = src[i];
    }

    return dest;
}

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
void* HAL_SEC_memcpy (void* dest, const void* src, UINT n, UINT rnd)
{
    if ((UINT32)dest % sizeof(UINT32) == 0 &&
        (UINT32)src  % sizeof(UINT32) == 0 &&
        (UINT32)n    % sizeof(UINT32) == 0)
    {
        return HAL_SEC_memcpy32((UINT32*)dest, (const UINT32*)src, n, rnd);
    }

    return HAL_SEC_memcpy8((UINT8*)dest, (const UINT8*)src, n, rnd);
}

