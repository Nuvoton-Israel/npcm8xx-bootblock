/*----------------------------------------------------------------------------*/
/*   SPDX-License-Identifier: GPL-2.0                                         */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   mailbox.c                                                                */
/* This file contains implementation of routines for handling PCI MailBox     */
/*  Project:  Arbel                                                           */
/*----------------------------------------------------------------------------*/

#include <string.h>

#include "mailbox.h"


ROM_STATUS_MSG  *ROM_msgPtr;

extern char __mailbox_start[];
/*---------------------------------------------------------------------------------------------------------*/
/* MailBox module function implementation                                                                  */
/*---------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        PCIMBX_Reset                                                                           */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Resets the PCI MailBox internal structure                                              */
/*---------------------------------------------------------------------------------------------------------*/
void PCIMBX_Reset (void)
{
#ifndef PCIMBX_DISABLED
    ROM_msgPtr = (ROM_STATUS_MSG*)ROM_STATUS_MSG_ADDR;
#else
    ROM_msgPtr = (ROM_STATUS_MSG*)__mailbox_start;
#endif

    // Reset the Mailbox
    //memset((void*)(ROM_msgPtr), 0x0, sizeof(ROM_STATUS_MSG));
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        PCIMBX_Print                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Print the content of PCIMBX                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void PCIMBX_Print (void)
{
	ROM_msgPtr = (ROM_STATUS_MSG*)ROM_STATUS_MSG_ADDR;
	serial_printf(KCYN "\nMBX\n" KNRM);

	serial_printf("version %#010lx \n", ROM_msgPtr->version);
	serial_printf("status %#010lx \n", ROM_msgPtr->status);
	serial_printf("info %#010lx \n", ROM_msgPtr->info);
	serial_printf("resetCounter %#010lx \n", ROM_msgPtr->resetCounter);
	serial_printf("lastBmcReset %#010lx \n", ROM_msgPtr->lastBmcReset);
	serial_printf("lastTipReset %#010lx \n", ROM_msgPtr->lastTipReset);
	serial_printf("spiDevice %#010lx \n", ROM_msgPtr->spiDevice);
	serial_printf("fustrap2 %#010lx \n", ROM_msgPtr->fustrap2);
	serial_printf("headerAddress %#010lx \n\n", ROM_msgPtr->headerAddress);
	//serial_printf("imageKeyValid;
	//serial_printf("imageKeyInalid;
	//serial_printf("otpKnVAL[FUSE_WRAPPER_NUM_OF_ECC_KEYS];

}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        PCIMBX_LogVersion                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  version - TIP ROM version                                                              */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Updates the tip rom version field in PCI MailBox                                       */
/*---------------------------------------------------------------------------------------------------------*/
void PCIMBX_LogVersion (UINT32 version)
{
    ROM_msgPtr->version = version;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        PCIMBX_UpdateStatus                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  status -    Status DWORD to be updated in SRAM PCI MailBox                             */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Updates the status field in PCI MailBox                                                */
/*---------------------------------------------------------------------------------------------------------*/
void PCIMBX_UpdateStatus (UINT32 status)
{
    ROM_msgPtr->status = status;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        PCIMBX_UpdateStatusAndInfo                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  status -    Status DWORD to be updated in SRAM PCI MailBox                             */
/*                  status -    Further information regarding the speciifed status                         */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Updates the status field in PCI MailBox and some additional information                */
/*---------------------------------------------------------------------------------------------------------*/
void PCIMBX_UpdateStatusAndInfo (UINT32 status, UINT32 info)
{
    ROM_msgPtr->status = status;
    ROM_msgPtr->info = info;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        PCIMBX_UpdateLastReset                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  bmcReset - BMC reset status                                                            */
/*                  tipReset - TIP reset status                                                            */
/*                  resetCounter - current value of reset counter                                          */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Stores current (last) secondery reset type                                             */
/*---------------------------------------------------------------------------------------------------------*/
void PCIMBX_UpdateLastReset (UINT32 bmcReset, UINT32 tipReset, UINT32 resetCounter)
{
    ROM_msgPtr->lastBmcReset = bmcReset;
    ROM_msgPtr->lastTipReset = tipReset;
    ROM_msgPtr->resetCounter = resetCounter;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        PCIMBX_UpdateSpiDevice                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  spiDevice - SPI/CS device number as they are numberized in boot module                 */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Store the spi device number where the booted image was located                         */
/*---------------------------------------------------------------------------------------------------------*/
void PCIMBX_UpdateSpiDevice (UINT8 spiDevice)
{
    ROM_msgPtr->spiDevice = spiDevice;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        PCIMBX_StoreFustrap2                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  fustrap2 - curent value of fustrap2 field in OTP                                       */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Stores the curent value of fustrap2 field in OTP                                       */
/*---------------------------------------------------------------------------------------------------------*/
void PCIMBX_StoreFustrap2 (UINT32 fustrap2)
{
    ROM_msgPtr->fustrap2 = fustrap2;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        PCIMBX_StoreHeaderAddress                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  headerAddress - The header address of the booted image                                 */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Stores The header address of the booted image                                          */
/*---------------------------------------------------------------------------------------------------------*/
void PCIMBX_StoreHeaderAddress (UINT32 headerAddress)
{
    ROM_msgPtr->headerAddress = headerAddress;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        PCIMBX_StoreHeaderKeyStatus                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  imageKeyInvalid - bit mask of the key that are invalid (taken from the image header)   */
/*                  imageKeyValid   - bit mask of the key that are valid (taken from the image header)     */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Stores the in/valid key bit masks of the booted image header                           */
/*---------------------------------------------------------------------------------------------------------*/
void PCIMBX_StoreHeaderKeyStatus (UINT16 imageKeyInvalid, UINT16 imageKeyValid)
{
    ROM_msgPtr->imageKeyInalid = imageKeyInvalid;
    ROM_msgPtr->imageKeyValid = imageKeyValid;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        PCIMBX_StoreOtpKnVAL                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  key - key index                                                                        */
/*                  validFlag - the valid flag of the key from the OTP                                     */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Stores the key valid flag from OTP                                                     */
/*---------------------------------------------------------------------------------------------------------*/
void PCIMBX_StoreOtpKnVAL (UINT8 key, UINT8 validFlag)
{
    ROM_msgPtr->otpKnVAL[key] = validFlag;
}

