/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*    defs_secured.h                                                          */
/*            This file contains NTIL security utilities                      */
/* Project:                                                                   */
/*            SWC DEFS                                                        */
/*----------------------------------------------------------------------------*/

#ifndef __DEFS_SECURED_H__
#define __DEFS_SECURED_H__


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                              Secured constants with large hamming distance                              */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#define SECURED_NULL                    ((void*)(0xaa55))

typedef enum
{
    SECURED_TRUE  = 0x9696,
    SECURED_FALSE = 0x6969
} SECURED_BOOLEAN_T;



/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                         Secured utility macros                                          */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/*                                  Secured integer comparison utilities                                   */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COMPARE                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  value1    - first value to compare                                                     */
/*                  value2    - second value to compare                                                    */
/*                                                                                                         */
/* Returns:         boolean indicating condition satisfaction                                              */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine checks a condition twice to protect against side channel attacks          */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COMPARE(value1, value2)                                                                   \
        (value1 == value2) && (value1 == value2)


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COMPARE_CONSISTENCY_FAIL                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  value1      - first value to compare                                                   */
/*                  value2      - second value to compare                                                  */
/*                  failureFunc - a boolean function that should be called in case of lost integrity       */
/*                  ..          - parameters to failure function                                           */
/*                                                                                                         */
/* Returns:         boolean indicating condition satisfaction                                              */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine checks a condition twice to protect against side channel attacks          */
/*                  In case inconsistency between the checks is detected, an attack is assumed and         */
/*                  failure function is called                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COMPARE_CONSISTENCY_FAIL(value1, value2, failureFunc, ...)                                \
        ((value1 == value2) ? ((value1 != value2) ? failureFunc(__VA_ARGS__) : TRUE) : FALSE)


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COMPARE_RET                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  type        - type of condition to check                                               */
/*                  value       - value to check, can be a function call                                   */
/*                  expected    - expected result                                                          */
/*                  err         - error to be returned in case of comparison failure                       */
/*                  failureFunc - a boolean function that should be called in case of lost integrity       */
/*                  ..          - parameters to failure function                                           */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine compares two values twice and returns a required error code if            */
/*                  comparison fails. It calls a failure function in case inconsistency is detected        */
/*                  The values can be functions calls or expression evaluations; in this case the funcion  */
/*                  is called/expression evalued only once and double comparison is done on the result     */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COMPARE_RET(type, value, expected, err, failureFunc, ...)                                 \
{                                                                                                          \
    volatile type tempRes = value;    /* to avoid double execution in case 'cond' is a function */         \
    volatile type expectedRes = expected;                                                                  \
                                                                                                           \
    if (!DEFS_SEC_COMPARE_CONSISTENCY_FAIL(tempRes, expectedRes, failureFunc, __VA_ARGS__))                \
    {                                                                                                      \
        return err;                                                                                        \
    }                                                                                                      \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COMPARE_RET_POSITIVE                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  type        - type of condition to check                                               */
/*                  value       - value to check, can be a function call                                   */
/*                  expected    - expected result                                                          */
/*                  ret         - value to be returned in case of comparison success                       */
/*                  failureFunc - a boolean function that should be called in case of lost integrity       */
/*                  ..          - parameters to failure function                                           */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine compares two values twice and returns a required error code if            */
/*                  comparison fails. It calls a failure function in case inconsistency is detected        */
/*                  The values can be functions calls or expression evaluations; in this case the funcion  */
/*                  is called/expression evalued only once and double comparison is done on the result     */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COMPARE_RET_POSITIVE(type, value, expected, ret, failureFunc, ...)                        \
{                                                                                                          \
    volatile type tempRes = value;    /* to avoid double execution in case 'cond' is a function */         \
    volatile type expectedRes = expected;                                                                  \
                                                                                                           \
    if (DEFS_SEC_COMPARE_CONSISTENCY_FAIL(tempRes, expectedRes, failureFunc, __VA_ARGS__))                 \
    {                                                                                                      \
        return ret;                                                                                        \
    }                                                                                                      \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COMPARE_ACTION                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  type        - type of condition to check                                               */
/*                  value       - value to check, can be a function call                                   */
/*                  expected    - expected result                                                          */
/*                  action      - code to be executed in case of comparison failure                        */
/*                  failureFunc - a boolean function that should be called in case of lost integrity       */
/*                  ..          - parameters to failure function                                           */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine compares two values twice and returns a required error code if            */
/*                  comparison fails. It calls a failure function in case inconsistency is detected        */
/*                  The values can be functions calls or expression evaluations; in this case the funcion  */
/*                  is called/expression evalued only once and double comparison is done on the result     */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COMPARE_ACTION(type, value, expected, action, failureFunc, ...)                           \
{                                                                                                          \
    volatile type tempRes     = value;      /* to avoid double execution in case 'cond' is a function */   \
    volatile type expectedRes = expected;                                                                  \
                                                                                                           \
    if (!DEFS_SEC_COMPARE_CONSISTENCY_FAIL(tempRes, expectedRes, failureFunc, __VA_ARGS__))                \
    {                                                                                                      \
        action;                                                                                            \
    }                                                                                                      \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COMPARE_GOTO                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  type        - type of condition to check                                               */
/*                  value       - value to check, can be a function call                                   */
/*                  expected    - expected result                                                          */
/*                  label       - a label to be jumped to in case of comparison failure                    */
/*                  failureFunc - a boolean function that should be called in case of lost integrity       */
/*                  ..          - parameters to failure function                                           */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine compares two integers twice and returns a required error code if          */
/*                  comparison fails. It calls a failure function in case inconsistency is detected        */
/*                  The values can be functions calls or expression evaluations; in this case the funcion  */
/*                  is called/expression evalued only once and double comparison is done on the result     */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COMPARE_GOTO(type, value, expected, label, failureFunc, ...)                              \
{                                                                                                          \
    volatile type tempRes     = value;      /* to avoid double execution in case 'cond' is a function */   \
    volatile type expectedRes = expected;                                                                  \
                                                                                                           \
    if (!DEFS_SEC_COMPARE_CONSISTENCY_FAIL(tempRes, expectedRes, failureFunc, __VA_ARGS__))                \
    {                                                                                                      \
        goto label;                                                                                        \
    }                                                                                                      \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COMPARE_FAIL                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  type        - type of condition to check                                               */
/*                  value       - value to check, can be a function call                                   */
/*                  expected    - expected result                                                          */
/*                  failureFunc - a boolean function that should be called in case of lost integrity       */
/*                  ..          - parameters to failure function                                           */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine checks a condition securely and calls failure function if the             */
/*                  condition was not met                                                                  */
/*                  The values can be functions calls or expression evaluations; in this case the funcion  */
/*                  is called/expression evalued only once and double comparison is done on the result     */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COMPARE_FAIL(type, value, expected, failureFunc, ...)                                     \
{                                                                                                          \
    volatile type tempRes     = value;    /* to avoid double execution in case 'cond' is a function */     \
    volatile type expectedRes = expected;                                                                  \
                                                                                                           \
    if (!DEFS_SEC_COMPARE_CONSISTENCY_FAIL(tempRes, expectedRes, failureFunc, __VA_ARGS__))                \
    {                                                                                                      \
        (void)failureFunc(__VA_ARGS__);                                                                    \
    }                                                                                                      \
}


/*---------------------------------------------------------------------------------------------------------*/
/*                                  Secured evaluation comparison utilities                                */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_EVAL_RET                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  type        - type of condition to check                                               */
/*                  expression  - an expression to evaluate, typically a function call                     */
/*                  expected    - expected result                                                          */
/*                  failureFunc - a boolean function that should be called in case of lost integrity       */
/*                  ..          - parameters to failure function                                           */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs an expression evaluation (e.g. function call) twice and calls a  */
/*                  failure function if  comparison fails. It calls a failure function in case             */
/*                  inconsistency is detected. Between exrpression evaluation cache is invalidated to make */
/*                  sure the transition to cache is not tampered                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_EVAL_FAIL(type, expression, expected, failureFunc, ...)                                   \
{                                                                                                          \
    volatile type result1 = expression;                                                                    \
    ICACHE_INVALIDATE();                                                                                   \
    volatile type result2 = expression;                                                                    \
                                                                                                           \
    if (result1 != result2)                                                                                \
    {                                                                                                      \
        (void)failureFunc(__VA_ARGS__);                                                                    \
    }                                                                                                      \
                                                                                                           \
    if (!DEFS_SEC_COMPARE_CONSISTENCY_FAIL(result2, expected, failureFunc, __VA_ARGS__))                   \
    {                                                                                                      \
        (void)failureFunc(__VA_ARGS__);                                                                    \
    }                                                                                                      \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_EVAL_RET                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  type        - type of condition to check                                               */
/*                  expression  - an expression to evaluate, typically a function call                     */
/*                  expected    - expected result                                                          */
/*                  err         - error to be returned in case of evaluation failure                       */
/*                  failureFunc - a boolean function that should be called in case of lost integrity       */
/*                  ..          - parameters to failure function                                           */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs an expression evaluation (e.g. function call) twice and returns a*/
/*                  required error code if  comparison fails. It calls a failure function in case          */
/*                  inconsistency is detected. Between exrpression evaluation cache is invalidated to make */
/*                  sure the transition to cache is not tampered                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_EVAL_RET(type, expression, expected, err, failureFunc, ...)                               \
{                                                                                                          \
    volatile type result1 = expression;                                                                    \
    ICACHE_INVALIDATE();                                                                                   \
    volatile type result2 = expression;                                                                    \
                                                                                                           \
    if (result1 != result2)                                                                                \
    {                                                                                                      \
        (void)failureFunc(__VA_ARGS__);                                                                    \
    }                                                                                                      \
                                                                                                           \
    if (!DEFS_SEC_COMPARE_CONSISTENCY_FAIL(result2, expected, failureFunc, __VA_ARGS__))                   \
    {                                                                                                      \
        return err;                                                                                        \
    }                                                                                                      \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_EVAL_ACTION                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  type        - type of condition to check                                               */
/*                  expression  - an expression to evaluate, typically a function call                     */
/*                  expected    - expected result                                                          */
/*                  action      - an action to be performed in case of evaluation failure                  */
/*                  failureFunc - a boolean function that should be called in case of lost integrity       */
/*                  ..          - parameters to failure function                                           */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs an expression evaluation (e.g. function call) twice and performs */
/*                  a required action if  comparison fails. It calls a failure function in case            */
/*                  inconsistency is detected.Between exrpression evaluation cache is invalidated to make  */
/*                  sure the transition to cache is not tampered                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_EVAL_ACTION(type, expression, expected, action, failureFunc, ...)                         \
{                                                                                                          \
    volatile type result1 = expression;                                                                    \
    ICACHE_INVALIDATE();                                                                                   \
    volatile type result2 = expression;                                                                    \
                                                                                                           \
    if (result1 != result2)                                                                                \
    {                                                                                                      \
        (void)failureFunc(__VA_ARGS__);                                                                    \
    }                                                                                                      \
                                                                                                           \
    if (!DEFS_SEC_COMPARE_CONSISTENCY_FAIL(result2, expected, failureFunc, __VA_ARGS__))                   \
    {                                                                                                      \
        action;                                                                                            \
    }                                                                                                      \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_EVAL_GOTO                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  type        - type of condition to check                                               */
/*                  expression  - an expression to evaluate, typically a function call                     */
/*                  expected    - expected result                                                          */
/*                  label       - an label to goto in case of evaluation failure                           */
/*                  failureFunc - a boolean function that should be called in case of lost integrity       */
/*                  ..          - parameters to failure function                                           */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine performs an expression evaluation (e.g. function call) twice and performs */
/*                  a required action if  comparison fails. It calls a failure function in case            */
/*                  inconsistency is detected. Between exrpression evaluation cache is invalidated to make */
/*                  sure the transition to cache is not tampered                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_EVAL_GOTO(type, expression, expected, label, err, failureFunc, ...)                       \
{                                                                                                          \
    volatile type result1 = expression;                                                                    \
    ICACHE_INVALIDATE();                                                                                   \
    volatile type result2 = expression;                                                                    \
                                                                                                           \
    if (result1 != result2)                                                                                \
    {                                                                                                      \
        (void)failureFunc(__VA_ARGS__);                                                                    \
    }                                                                                                      \
                                                                                                           \
    if (!DEFS_SEC_COMPARE_CONSISTENCY_FAIL(result2, expected, failureFunc, __VA_ARGS__))                   \
    {                                                                                                      \
        goto label;                                                                                        \
    }                                                                                                      \
}


/*---------------------------------------------------------------------------------------------------------*/
/*                                  Secured condition checking utilities                                   */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COND_CHECK                                                                    */
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
#define DEFS_SEC_COND_CHECK(cond)         ((cond) && (cond))


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COND_CHECK_RET                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  cond    - condition to check                                                           */
/*                  err     - error to return if condition is not met                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine checks a condition twice and returns the required error if the            */
/*                  condition was not met                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COND_CHECK_RET(cond, err)                                                                 \
{                                                                                                          \
    volatile BOOLEAN tempRes = (cond);    /* to avoid double execution in case 'cond' is a function */     \
    if (!DEFS_SEC_COND_CHECK(tempRes))                                                                     \
    {                                                                                                      \
        return err;                                                                                        \
    }                                                                                                      \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COND_CHECK_RET_POSITIVE                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  cond    - condition to check                                                           */
/*                  ret     - ret to return if condition is met                                            */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine checks a condition twice and returns the required error if the            */
/*                  condition was not met                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COND_CHECK_RET_POSITIVE(cond, ret)                                                        \
{                                                                                                          \
    volatile BOOLEAN tempRes = (cond);    /* to avoid double execution in case 'cond' is a function */     \
    if (DEFS_SEC_COND_CHECK(tempRes))                                                                      \
    {                                                                                                      \
        return ret;                                                                                        \
    }                                                                                                      \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COND_CHECK_FAIL                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  cond        - condition to check                                                       */
/*                  failureFunc - a function that should be called in case of lost integrity               */
/*                  ..          - parameters to failure function                                           */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine checks a condition twice and calls failure function if the                */
/*                  condition was not met                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COND_CHECK_FAIL(cond, failureFunc, ...)                                                   \
{                                                                                                          \
    volatile BOOLEAN tempRes = (cond);    /* to avoid double execution in case 'cond' is a function */     \
    if (!DEFS_SEC_COND_CHECK(tempRes))                                                                     \
    {                                                                                                      \
        failureFunc(__VA_ARGS__);                                                                          \
    }                                                                                                      \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COND_CHECK_ACTION                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  cond    - condition to check                                                           */
/*                  action  - an action to be performed on failure                                         */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This macro checks a condition securely and performs an action in case of error         */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COND_CHECK_ACTION(cond, action)                                                           \
{                                                                                                          \
    volatile BOOLEAN tempRes = (cond);    /* to avoid double execution in case 'cond' is a function */     \
    if (!DEFS_SEC_COND_CHECK(tempRes))                                                                     \
    {                                                                                                      \
        action;                                                                                            \
    }                                                                                                      \
}                                                                                                          \


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COND_CHECK_GOTO                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  cond     - condition to check                                                          */
/*                  endlabel - label to jump to if the condition is not met                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This macro checks a condition securely and jumps to a label in case of error           */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COND_CHECK_GOTO(cond, endlabel)                                                           \
    DEFS_SEC_COND_CHECK_ACTION(cond, goto endlabel)


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COND_CHECK_DELAY_LOOP                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  count       - iterations number                                                        */
/*                  cond        - condition to check                                                       */
/*                  failureFunc - a function that should be called in case of lost integrity               */
/*                  ..          - parameters to failure function                                           */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This macro performs delay loop with condition check and calls a failure function       */
/*                  if the comparison fails                                                                */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COND_CHECK_DELAY_LOOP(count, cond, failureFunc, ...)                                      \
{                                                                                                          \
    volatile UINT16 __i;                                                                                   \
                                                                                                           \
    for (__i = 0; __i < (count); ++__i)                                                                    \
    {                                                                                                      \
        if (!(cond))                                                                                       \
        {                                                                                                  \
            failureFunc(__VA_ARGS__);                                                                      \
        }                                                                                                  \
    }                                                                                                      \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COND_DELAY_LOOP_AND_CHECK                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  count       - iterations number                                                        */
/*                  cond        - condition to check                                                       */
/*                  failureFunc - a function that should be called in case of lost integrity               */
/*                  ..          - parameters to failure function                                           */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This macro performs delay loop and then it does condition check and calls a            */
/*                  failure function if the comparison fails                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COND_DELAY_LOOP_AND_CHECK(count, cond, failureFunc, ...)                                  \
{                                                                                                          \
    DELAY_LOOP(count);                                                                                     \
                                                                                                           \
    if (!(cond))                                                                                           \
    {                                                                                                      \
        failureFunc(__VA_ARGS__);                                                                          \
    }                                                                                                      \
}


/*---------------------------------------------------------------------------------------------------------*/
/*                               Secured Execution of a function from pointer                              */
/*---------------------------------------------------------------------------------------------------------*/
/* Call the macro in the following way:                                                                    */
/* SEC_EXECUTE_FUNC( func, (arg1, arg2))                                                                   */
/* SEC_EXECUTE_FUNC_RET( retvar, func, (arg1, arg2, arg3))                                                 */
/* SEC_EXECUTE_FUNC_RET_VALUE( func, (arg1, arg2, arg3), default_return_value)                             */
/*---------------------------------------------------------------------------------------------------------*/
#define SEC_EXECUTE_FUNC(func, args)                                                                       \
    if (func != SECURED_NULL)                                                                              \
    {                                                                                                      \
        func args;                                                                                         \
    }

#define SEC_EXECUTE_FUNC_RET(ret, func, args)                                                              \
    if (func != SECURED_NULL)                                                                              \
    {                                                                                                      \
        ret = func args;                                                                                   \
    }

#define SEC_EXECUTE_FUNC_RET_VALUE(func, args, default_value)   ((func != SECURED_NULL)? func args : default_value)


/*---------------------------------------------------------------------------------------------------------*/
/*                                     Secured flow control utilities                                      */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_FLOW_MONITOR_DECLARE                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  offset - a random/initial value to use as an offset for the flow counter               */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This macro declares and initializes with an offset value the variables of both the     */
/*                  flowCounter and flowCounterOffset                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_FLOW_MONITOR_DECLARE(offset)       volatile UINT32 flowCounter = (offset);                    \
                                                volatile UINT32 flowCounterOffset = (offset)


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_FLOW_MONITOR_ADD_OFFSET                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  offset - a random number to use as an offset for the flow counter                      */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This macro sets a new base for the flow monitor counter. This base is added to         */
/*                  flowCounter and flowCounterOffset (which later be used by DEFS_FLOW_MONITOR_COMPARE).  */
/*                  This macro is optional when using the flow monitor mechanism, but it recommended to    */
/*                  use it with a random velue in order to make the counter unpredicatable.                */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_FLOW_MONITOR_ADD_OFFSET(offset)                                                               \
{                                                                                                          \
    flowCounter       += (offset);                                                                         \
    flowCounterOffset += (offset);                                                                         \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_FLOW_MONITOR_INCREMENT                                                            */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This macro increments the flow counter. Typically called in critical point of the      */
/*                  flow. This point is defined as one that is important to make sure the flow indeed      */
/*                  passed by                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_FLOW_MONITOR_INCREMENT()    flowCounter++


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_FLOW_MONITOR_COMPARE                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  x           - an expected value of flow counter. This typically would be a constant    */
/*                                defined in the code                                                      */
/*                  failureFunc - a function that should be called in case the flow has been compromised.  */
/*                                The functions receives variable number of parameters                     */
/*                  ...         - parameters to the failure function                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This securely compares the value of actual flow counter with its expected value (added */
/*                  to the offset value), and calls a failure function if the comparison fails             */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_FLOW_MONITOR_COMPARE(x, failureFunc, ...)                                                     \
    if (!DEFS_SEC_COND_CHECK((flowCounterOffset + (x)) == flowCounter))                                    \
    {                                                                                                      \
        failureFunc(__VA_ARGS__);                                                                          \
    }


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_FLOW_MONITOR_COMPARE_ACTION                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  x           - an expected value of flow counter. This typically would be a constant    */
/*                                defined in the code                                                      */
/*                  action      - an action to be performed on failure                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This securely compares the value of actual flow counter with its expected value (added */
/*                  to the offset value) and performs the action if the comparison fails                   */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_FLOW_MONITOR_COMPARE_ACTION(x, action)                                                        \
    if (!DEFS_SEC_COND_CHECK((flowCounterOffset + (x)) == flowCounter))                                    \
    {                                                                                                      \
        action;                                                                                            \
    }


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_FLOW_MONITOR_COMPARE_RET                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  x           - an expected value of flow counter. This typically would be a constant    */
/*                                defined in the code                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This securely compares the value of actual flow counter with its expected value (added */
/*                  to the offset value) and returns a security error if the comparison fails              */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_FLOW_MONITOR_COMPARE_RET(x)                                                                   \
    if (!DEFS_SEC_COND_CHECK((flowCounterOffset + (x)) == flowCounter))                                    \
    {                                                                                                      \
        return DEFS_STATUS_SECURITY_ERROR;                                                                 \
    }


/*---------------------------------------------------------------------------------------------------------*/
/* Delay computing for condition flow 1: takes 1 bit of (x) number from an offset that is based on (n)     */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COND_FLOW_1_DELAY(x,n)   (((x) >> (n)) & 0x01)


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COND_FLOW_1                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  x           - a 3 bits variable used for increment operations                          */
/*                  cond        - a condition to check                                                     */
/*                  action      - action done if condition is eventually met                               */
/*                  failureFunc - a function that should be called in case the flow has been compromised.  */
/*                                The function receives a variable number of parameters                    */
/*                  ...         - parameters to the failure function                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:    This macro defines its own flowCounter                                                 */
/* Description:                                                                                            */
/*                  This macro checks a condition according to the "secured condition 1" flow,             */
/*                  and executes 'Yes' path accordingly.                                                   */
/*                                                                                                         */
/* Example:                                                                                                */
/*                                                                                                         */
/*    void FailureFunc (arg1, arg2, ..., argn);                                                            */
/*                                                                                                         */
/*    {                                                                                                    */
/*        ...                                                                           // Some code       */
/*        DEFS_SEC_COND_FLOW_1( x, cond, action, FailureFunc, arg1, arg2, ..., argn )   // Call macro      */
/*        ...                                                                                              */
/*    }                                                                                                    */
/*                                                                                                         */
/*lint -esym(578,flowCounter)                                                                              */
/*    Suppress declaration of symbol 'flowCounter' hides symbol 'flowCounter' (from earlier code)          */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COND_FLOW_1(x, cond, action, failureFunc, ...)                                            \
{                                                                                                          \
    volatile UINT8 __d;                                                                                    \
                                                                                                           \
    DEFS_FLOW_MONITOR_DECLARE(0);                                                                          \
                                                                                                           \
    __d = 1 + DEFS_SEC_COND_FLOW_1_DELAY(x, 0);                                                            \
    DEFS_SEC_COND_CHECK_DELAY_LOOP(__d, cond, failureFunc, (__VA_ARGS__));                                 \
    DEFS_FLOW_MONITOR_INCREMENT();                                                                         \
                                                                                                           \
    __d =  1 + DEFS_SEC_COND_FLOW_1_DELAY(x, 1);                                                           \
    DEFS_SEC_COND_CHECK_DELAY_LOOP(__d, cond, failureFunc, (__VA_ARGS__));                                 \
    DEFS_FLOW_MONITOR_INCREMENT();                                                                         \
                                                                                                           \
    __d =  1 + DEFS_SEC_COND_FLOW_1_DELAY(x, 2);                                                           \
    DEFS_SEC_COND_CHECK_DELAY_LOOP(__d, cond, failureFunc, (__VA_ARGS__));                                 \
    DEFS_FLOW_MONITOR_INCREMENT();                                                                         \
                                                                                                           \
    DEFS_FLOW_MONITOR_COMPARE(3, failureFunc, __VA_ARGS__);                                                \
    action;     /* Execute 'Yes' path */                                                                   \
    DEFS_FLOW_MONITOR_INCREMENT();                                                                         \
    DEFS_FLOW_MONITOR_COMPARE(4, failureFunc, __VA_ARGS__);                                                \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Delay computing for condition flow 2: takes 2 bits of (x) number from an offset that is based on (n)    */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COND_FLOW_2_DELAY(x,n)   (((x) >> (n)) & 0x03)


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COND_FLOW_2                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  x           - a 8 bits variable used for increment operations                          */
/*                  cond        - a condition to check                                                     */
/*                  failureFunc - a function that should be called in case the flow has been compromised.  */
/*                                The function receives a variable number of parameters                    */
/*                  ...         - parameters to the failure function                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This macro checks a condition according to the "secured condition 2" flow.             */
/*                  This macro checks assume to be true condition and verify it is constantly succeeded    */
/*                                                                                                         */
/* Example:                                                                                                */
/*                                                                                                         */
/*    void FailureFunc (arg1, arg2, ..., argn);                                                            */
/*                                                                                                         */
/*    {                                                                                                    */
/*        ...                                                                           // Some code       */
/*        DEFS_SEC_COND_FLOW_2( x, cond, action, FailureFunc, arg1, arg2, ..., argn )   // Call macro      */
/*        ...                                                                                              */
/*    }                                                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COND_FLOW_2(x, cond, failureFunc, ...)                                                    \
{                                                                                                          \
    volatile BOOLEAN __tempRes = (cond);    /* to avoid double execution in case 'cond' is a function */   \
    volatile UINT8   __j = 0;                                                                              \
    volatile UINT8   __d;                                                                                  \
                                                                                                           \
    __d = 1 + DEFS_SEC_COND_FLOW_2_DELAY(x, __j);                                                          \
    DEFS_SEC_COND_DELAY_LOOP_AND_CHECK(__d, __tempRes, failureFunc, (__VA_ARGS__));                        \
    __j += 2;                                                                                              \
                                                                                                           \
    __d =  1 + DEFS_SEC_COND_FLOW_2_DELAY(x, __j);                                                         \
    DEFS_SEC_COND_DELAY_LOOP_AND_CHECK(__d, __tempRes, failureFunc, (__VA_ARGS__));                        \
    __j += 2;                                                                                              \
                                                                                                           \
    __d =  1 + DEFS_SEC_COND_FLOW_2_DELAY(x, __j);                                                         \
    DEFS_SEC_COND_DELAY_LOOP_AND_CHECK(__d, __tempRes, failureFunc, (__VA_ARGS__));                        \
    __j += 2;                                                                                              \
                                                                                                           \
    __d =  1 + DEFS_SEC_COND_FLOW_2_DELAY(x, __j);                                                         \
    DEFS_SEC_COND_DELAY_LOOP_AND_CHECK(__d, __tempRes, failureFunc, (__VA_ARGS__));                        \
    __j += 2;                                                                                              \
                                                                                                           \
    if (__j != 8 || !__tempRes)                                                                            \
    {                                                                                                      \
        failureFunc(__VA_ARGS__);                                                                          \
    }                                                                                                      \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Delay computing for condition flow 1: takes 3 bits of (x) number from an offset that is based on (n)    */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COND_FLOW_3_DELAY(x,n)   (((x) >> ((n) * 3)) & 0x07)


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_COND_FLOW_3                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  x           - a 9 bits variable used for increment operations                          */
/*                  cond        - a condition to check                                                     */
/*                  action      - action done if condition is eventually met                               */
/*                  failureFunc - a function that should be called in case the flow has been compromised.  */
/*                                The function receives a variable number of parameters                    */
/*                  ...         - parameters to the failure function                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/* Side effects:    This macro defines its own flowCounter                                                 */
/* Description:                                                                                            */
/*                  This macro checks a condition according to the "secured condition 1" flow,             */
/*                  and executes 'Yes' path accordingly.                                                   */
/*                                                                                                         */
/* Example:                                                                                                */
/*                                                                                                         */
/*    void FailureFunc (arg1, arg2, ..., argn);                                                            */
/*                                                                                                         */
/*    {                                                                                                    */
/*        ...                                                                           // Some code       */
/*        DEFS_SEC_COND_FLOW_1( x, cond, action, FailureFunc, arg1, arg2, ..., argn )   // Call macro      */
/*        ...                                                                                              */
/*    }                                                                                                    */
/*                                                                                                         */
/*lint -esym(578,flowCounter)                                                                              */
/*    Suppress declaration of symbol 'flowCounter' hides symbol 'flowCounter' (from earlier code)          */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_COND_FLOW_3(x, cond, action, failureFunc, ...)                                            \
{                                                                                                          \
    volatile UINT8 __d;                                                                                    \
                                                                                                           \
    DEFS_FLOW_MONITOR_DECLARE(0);                                                                          \
                                                                                                           \
    __d = 2 + DEFS_SEC_COND_FLOW_3_DELAY(x, 0);                                                            \
    DEFS_SEC_COND_CHECK_DELAY_LOOP(__d, cond, failureFunc, (__VA_ARGS__));                                 \
    DEFS_FLOW_MONITOR_INCREMENT();                                                                         \
                                                                                                           \
    __d =  2 + DEFS_SEC_COND_FLOW_3_DELAY(x, 1);                                                           \
    DEFS_SEC_COND_CHECK_DELAY_LOOP(__d, cond, failureFunc, (__VA_ARGS__));                                 \
    DEFS_FLOW_MONITOR_INCREMENT();                                                                         \
                                                                                                           \
    __d =  2 + DEFS_SEC_COND_FLOW_3_DELAY(x, 2);                                                           \
    DEFS_SEC_COND_CHECK_DELAY_LOOP(__d, cond, failureFunc, (__VA_ARGS__));                                 \
    DEFS_FLOW_MONITOR_INCREMENT();                                                                         \
                                                                                                           \
    DEFS_FLOW_MONITOR_COMPARE(3, failureFunc, __VA_ARGS__);                                                \
    action;     /* Execute 'Yes' path */                                                                   \
    DEFS_FLOW_MONITOR_INCREMENT();                                                                         \
    DEFS_FLOW_MONITOR_COMPARE(4, failureFunc, __VA_ARGS__);                                                \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_BRANCH_ADDRESS_CHECK                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  var         - a variable used for the branch address operations                        */
/*                  addr        - an address to check (to compare with a saved address)                    */
/*                  failureFunc - a function that should be called in case the flow has been compromised.  */
/*                                The function receives a variable number of parameters                    */
/*                              - parameters to the failure function                                       */
/*                                                                                                         */
/* Returns:         None                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine checks if the given address is equal to the address saved in 'var'        */
/*                  (a global variable), i.e. we reached that address intentionally from a knwon caller.   */
/*                  In case the addresses are not equal, an attack is assumed and the failure function is  */
/*                  called.                                                                                */
/*                  Use this macro inside the function that you want to protect.                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_BRANCH_ADDRESS_CHECK(_var, _addr, failureFunc, ...)                                        \
{                                                                                                           \
    DEFS_SEC_COND_CHECK_FAIL((void*)(_var) == (void*)(_addr), failureFunc, __VA_ARGS__);                    \
                                                                                                            \
    _var = NULL;                                                                                            \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_BRANCH_ADDRESS_SET                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  var         - a variable used for the branch address operations                        */
/*                  addr        - an address to set                                                        */
/*                                                                                                         */
/* Returns:         None                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine saves the given address in 'var', which should be a global variable.      */
/*                  This macro should be use before calling any function that is protected with the        */
/*                  DEFS_SEC_BRANCH_ADDRESS_CHECK macro (with the function name as 'addr').                */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_BRANCH_ADDRESS_SET(_var, _addr)                                                            \
{                                                                                                           \
    _var = (void*)(_addr);                                                                                  \
}


/*---------------------------------------------------------------------------------------------------------*/
/* Macro:           DEFS_SEC_RUN_TWICE                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  func        - function to call twice                                                   */
/* Returns:         None                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This macro runs a function twice usually used to avoid glitching attack                */
/*---------------------------------------------------------------------------------------------------------*/
#define DEFS_SEC_RUN_TWICE(func)    {{func;}{func;}}


#endif /* __DEFS_SECURED_H__ */

