/*----------------------------------------------------------------------------*/
/*   SPDX-License-Identifier: GPL-2.0                                         */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   bootblock_version.c                                                      */
/*            This file contains bootblock version constants                  */
/*  Project:  Arbel                                                           */
/*----------------------------------------------------------------------------*/
#define BOOTBLOCK_VERSION_C

#include "boot.h"

/*
BootblockVersion = N7N6N5N4N3N2N1N0

Where:

-         N7N6  BOOTBLOCK code type
-         N5N4  BOOTBLOCK code Major Version.
-         N3N2  BOOTBLOCK code Minor Version.
-         N1N0  BOOTBLOCK code Revision.
*/

//Dont forget the --keep=Bootblock_Version.o(*) in linker command line when building
//the project if "Remove Unused Sections" is used, otherwise the linker removes
//this object from the BOOTBLOCK images since the constants here are not referenced.

const BOOTBLOCK_Version_T bb_version = {
	.VersionDescription =  "\n"
					__DATE__
					"\n"
					__TIME__
                                       "\n\0",

	.BootBlockTag    = 'B' | 'O'<<8 | 'O'<<16 | 'T'<<24,

    #define BOOT_DEBUG_VAL 0x00000000

	.BootblockVersion = (BOOT_DEBUG_VAL | 0x000409)     //ver 00.04.09
};

#undef BOOTBLOCK_VERSION_C

