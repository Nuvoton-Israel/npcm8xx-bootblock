/*----------------------------------------------------------------------------*/
/*   SPDX-License-Identifier: GPL-2.0                                         */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   mailbox.h                                                                */
/*            This file contains API of routines for handling the PCI MailBox */
/*  Project:                                                                  */
/*            Arbel                                                           */
/*----------------------------------------------------------------------------*/

#ifndef _MAILBOX_H_
#define _MAILBOX_H_


#include "hal.h"

#define NUM_OF_ECC_KEYS            8

/*---------------------------------------------------------------------------------------------------------*/
/* MailBox module internal definitions                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#define PCIMAILBOX_START_ADDR           PCIMBX_BASE_ADDR(0)
#define PCIMAILBOX_END_ADDR             (PCIMAILBOX_START_ADDR + PCIMBX_RAM_SIZE)
#define ROM_STATUS_MSG_ADDR             (PCIMAILBOX_END_ADDR - sizeof(ROM_STATUS_MSG))

/*---------------------------------------------------------------------------------------------------------*/
/* MailBox module internal structure definitions                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#pragma pack(1)
typedef struct _ROM_STATUS_MSG
{
    UINT32  version;
    UINT32  status;
    UINT32  info;
    UINT32  resetCounter;
    UINT32  lastBmcReset;
    UINT32  lastTipReset;
    UINT32  spiDevice;
    UINT32  fustrap2;
    UINT32  headerAddress;
    UINT16  imageKeyValid;
    UINT16  imageKeyInalid;
    UINT8   otpKnVAL[NUM_OF_ECC_KEYS];

} ROM_STATUS_MSG;

/*---------------------------------------------------------------------------------------------------------*/
/* Mailbox module exported functions                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void PCIMBX_Reset (void);
void PCIMBX_Print (void);

void PCIMBX_LogVersion (UINT32 version);
void PCIMBX_UpdateStatus (UINT32 status);
void PCIMBX_UpdateStatusAndInfo (UINT32 status, UINT32 info);
void PCIMBX_UpdateLastReset (UINT32 bmcReset, UINT32 tipReset, UINT32 resetCounter);
void PCIMBX_UpdateSpiDevice (UINT8 spiDevice);
void PCIMBX_StoreHeaderAddress (UINT32 headerAddress);


#endif /* _MAILBOX_H_ */

