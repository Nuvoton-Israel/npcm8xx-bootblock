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
                                         SecureErrorCallback secure_error_callback)
{
    volatile SECURED_BOOLEAN_T  retVal1 = SECURED_FALSE;
    volatile SECURED_BOOLEAN_T  retVal2 = SECURED_FALSE;
    volatile UINT8              result  = 0;
    volatile UINT               count   = 0;
    UINT                        i;

    DEFS_FLOW_MONITOR_DECLARE(0);

    for (i = 0; i < n; i++)
    {
        result |= (((UINT8*)ptr1)[i] ^ ((UINT8*)ptr2)[i]);
        count++;
    }

    DEFS_FLOW_MONITOR_INCREMENT();

    /*-----------------------------------------------------------------------------------------------------*/
    /* OR operand where 'count' and 'result' are volatile will not be optimized.                           */
    /* If at one point 'count' will be different than 'n' or 'result' will not be zero,                    */
    /* the following if statements will not enter.                                                         */
    /*lint -e564 suppress variable 'count'/'result' depends on order of evaluation"                        */
    /*-----------------------------------------------------------------------------------------------------*/

    if (0 == ((count ^ n) | (count ^ n)))
    {
        DEFS_FLOW_MONITOR_INCREMENT();
    }
    else
    {
        EXECUTE_FUNC(secure_error_callback, (DEFS_STATUS_SECURITY_ERROR, __LINE__));
    }

    if (0 == ((count ^ n) | (count ^ n)))
    {
        DEFS_FLOW_MONITOR_INCREMENT();
    }
    else
    {
        EXECUTE_FUNC(secure_error_callback, (DEFS_STATUS_SECURITY_ERROR, __LINE__));
    }

    if (0 == (result | result))
    {
        retVal1 = SECURED_TRUE;
    }

    if (0 == (result | result))
    {
        retVal2 = SECURED_TRUE;
    }

    if (retVal1 == retVal2)
    {
        DEFS_FLOW_MONITOR_INCREMENT();
    }
    else
    {
        EXECUTE_FUNC(secure_error_callback, (DEFS_STATUS_SECURITY_ERROR, __LINE__));
    }

    //lint -e527 suppress 'Unreachable code at token ;'
    DEFS_FLOW_MONITOR_COMPARE_ACTION(4,
        {
            EXECUTE_FUNC(secure_error_callback, (DEFS_STATUS_SECURITY_ERROR, __LINE__));
            return SECURED_FALSE;
        } );

    return retVal2;
}

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
SECURED_BOOLEAN_T HAL_SEC_memcmp (const void* ptr1, const void* ptr2, UINT n)
{
    return HAL_SEC_memcmp_action(ptr1, ptr2, n, NULL);
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

    if (dest == NULL || src == NULL || n == 0)
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

    if (dest == NULL || src == NULL || dwordNum == 0)
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

