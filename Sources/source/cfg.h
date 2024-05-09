/*----------------------------------------------------------------------------*/
/*   SPDX-License-Identifier: GPL-2.0                                         */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   cfg.h                                                                    */
/*            This file contains API of configuration routines for ROM code   */
/*  Project:                                                                  */
/*            Arbel Bootblock                                                 */
/*----------------------------------------------------------------------------*/
#ifndef CFG_H
#define CFG_H

#include "hal.h"

/*---------------------------------------------------------------------------------------------------------*/
/* CFG module exported functions                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void    CFG_SHM_ReleaseHostWait (void);
int     CFG_PrintResetType (void);


#endif /* CFG_H */
