/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   gic_if.c                                                                 */
/*            This file contains GIC module driver selector                   */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/

#include __CHIP_H_FROM_IF()

#if defined(GIC_MODULE_TYPE)
    #include __MODULE_DRIVER(gic, GIC_MODULE_TYPE)
#endif

