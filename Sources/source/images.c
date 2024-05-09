/*----------------------------------------------------------------------------*/
/*   SPDX-License-Identifier: GPL-2.0                                         */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   images.c                                                                 */
/* This file contains API of routines to boot the BMC for NO_TIP devices only. */
/*  Project:                                                                  */
/*            Arbel BootBlock A35                                             */
/*----------------------------------------------------------------------------*/
#ifdef _NOTIP_
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "hal.h"
#include "../SWC_HAL/hal_regs.h"
#include "boot.h"
#include "images.h"

/**
 * start tag of all possible images
 */
const uint8_t kmt_header_tag[8] = { 0x5E, 0x4D, 0x3B, 0x2A, 0xE1, 0x54, 0xF2, 0x57 }; __attribute__ ((aligned (16)))
const uint8_t kmt_header_tag_crc[8] = { 0x5E, 0x4D, 0x3B, 0x2A, 0x1E, 0xAB, 0xF2, 0x57 }; __attribute__ ((aligned (16)))
const uint8_t tip_fw_l0_header_tag[8] = { 0x5E, 0x4D, 0x7A, 0x9B, 0xE1, 0x54, 0xF2, 0x57 }; __attribute__ ((aligned (16)))
const uint8_t tip_fw_l0_header_tag_crc[8] = { 0x5E, 0x4D, 0x7A, 0x9B, 0x1E, 0xAB, 0xF2, 0x57 }; __attribute__ ((aligned (16)))
const uint8_t skmt_header_tag[8] = { 0x73, 0x6B, 0x6D, 0x74, 0x50, 0x08, 0x0D, 0x0A }; __attribute__ ((aligned (16)))
const uint8_t tip_fw_l1_header_tag[8] = { 0x0A, 0x54, 0x49, 0x50, 0x5F, 0x4C, 0x31, 0x0A }; __attribute__ ((aligned (16)))
const uint8_t bb_header_tag[8] = { 0x0A, 0x50, 0x08, 0x55, 0xAA, 0x54, 0x4F, 0x4F }; __attribute__ ((aligned (16)))
const uint8_t uboot_header_tag[8] = { 0x0A, 0x55, 0x42, 0x4F, 0x4F, 0x54, 0x42, 0x4C }; __attribute__ ((aligned (16)))
const uint8_t bl31_header_tag[8] = { 0x0A, 0x42, 0x4C, 0x33, 0x31, 0x4E, 0x50, 0x43 }; __attribute__ ((aligned (16)))
const uint8_t optee_header_tag[8] = { 0x0A, 0x54, 0x45, 0x45, 0x5F, 0x4E, 0x50, 0x43 }; __attribute__ ((aligned (16)))

static int firmware_image_load_func (struct tip_firmware_image *tip_fw_im, uint32_t base_addr);
static int firmware_image_scan_flash (struct tip_firmware_image *tip_fw_im);
static int component_start (struct tip_firmware_image	*fw,
						 struct tip_firmware_header *tip_header,
						 IMG_TYPE_E					 img_type,
						 int					*start_offset);

/**
 * Copy data from flash to memory via direct read. Only working on aligned data.
 *
 * @param dst_addr Destination address
 * @param src_flash_addr  The flash data
 * @param src_size The size of data to be copy
 * @param dram If 1 destination address is DRAM
 * @param print If 1 debug prints enabled
 * @return The number of bytes copied
 */
uint32_t image_memcpy (uint32_t dst_addr, uint32_t src_flash_addr, uint32_t src_size, _Bool print)
{
	uint32_t		  cnt;
	uint32_t		  block_cnt;
	uint32_t		  size = 0;
	uint32_t		  src_cnt = 0;
	uint32_t		  cs;
	uint32_t		  fiu;
	uint32_t		  spi;
	uint32_t		  offset;
	
	/* Align the address. */
	dst_addr &= 0xFFFFFFFC;
	src_flash_addr &= 0xFFFFFFFC;

	/* Align the size. */
	if (src_size & 0x3) {
		src_size += 4 - (src_size & 0x3);
	}

	if (print) {
		serial_printf ("\ncopy     %#010lx ", src_flash_addr);
		serial_printf ("size  %#010lx ", src_size);
		serial_printf ("to    %#010lx \t", dst_addr);
	}
	
	/* direct mapping, assuming GLBLEN is set. */
	for (cnt = 0; cnt < src_size; cnt += 4) {
		*(uint32_t *) (dst_addr + cnt) = *(uint32_t *) (src_flash_addr + cnt);
		if ((cnt % 1024 == 0) && print)
			serial_printf (".");
	}
	if (print) {
		serial_printf ("\n");
	}

	return size;
}


/**
 * Get TIP firmware image name
 *
 * @param img Image type
 * @return FW iamge name
 */
char *image_firmware_get_fw_name (IMG_TYPE_E img)
{
	switch (img) {
		case (IMG_KMT):
			return "KMT          ";
		case (IMG_TFT_L0):
			return "TFT_L0       ";
		case (IMG_SKMT):
			return "SKMT         ";
		case (IMG_TFT_L1):
			return "TFT_L1       ";
		case (IMG_BOOTBLOCK):
			return "BOOTBLOCK    ";
		case (IMG_BL31):
			return "BL31         ";
		case (IMG_OPTEE):
			return "OPTEE        ";
		case (IMG_UBOOT):
			return "UBOOT        ";
		case (IMG_COMBO0):
			return "COMBO0       ";
		case (IMG_COMBO1):
			return "COMBO1       ";
		case (IMG_LINUX_KERNEL):
			return "LINUX_KERNEL ";
		case (IMG_LINUX_DTS):
			return "LINUX_DTS    ";
		case (IMG_LINUX_FS):
			return "LINUX_FS     ";
		case (IMG_LINUX_OPENBMC):
			return "LINUX_OPENBMC";
		case (IMG_FULL):
			return "FULL         ";
		default:
			return "UNKNOWN ";
	}
}

	
/**
 * Update the image referenced by an instance.
 *
 * @param fw The firmware image instance to update.
 * @param flash The flash device that contains the firmware image.
 * @param base_addr The starting address of the new firmware image.
 *
 * @return 0 if the image reference was updated successfully or an error code.
 */
int firmware_image_load_func (struct tip_firmware_image *tip_fw_im, uint32_t			 base_addr)
{
	
	uint32_t				   src_flash_addr = 0;
	uint32_t				   src_size = -1;
	uint32_t				   dst_addr = -1;
	uint32_t				   addr_min = 0;
	uint32_t				   addr_max = 0xFFFFFFFF; 
	uint32_t				   size_max = _2GB_;

	
	if (tip_fw_im == NULL) {
		return -1;
	}

	src_size = tip_fw_im->header->header_flash->header.codeSize + sizeof (HEADER_GENERAL_T);
	dst_addr = tip_fw_im->header->header_flash->header.destAddr;
	src_flash_addr = (uint32_t) (uint8_t *) tip_fw_im->header->header_flash;

	switch (tip_fw_im->img_type) {

		case IMG_BOOTBLOCK:
			addr_min = RAM2_BASE_ADDR;
			addr_max = RAM2_BASE_ADDR + RAM2_MEMORY_SIZE;
			break;

		case IMG_BL31:
		case IMG_OPTEE:
		case IMG_UBOOT:
			addr_min = 0;
			addr_max = 0x80000000;
			break;

		default:
			serial_printf ("%s: Invalid image type" NEWLINE, __func__);
			return -1;
	}

	size_max = addr_max - addr_min;

	if ((src_size < sizeof (HEADER_GENERAL_T)) || (src_size > size_max)) {
		serial_printf (
			KRED
			"image size is invalid, flash addr=%#010lx	dst=%#010lx, size=%#010lx" NEWLINE KNRM,
			src_flash_addr, dst_addr, src_size);
		return FIRMWARE_IMAGE_INVALID_FORMAT;
	}

	if ((dst_addr == 0xFFFFFFFF) || (dst_addr == 0) || (dst_addr < addr_min) ||
		(dst_addr + src_size) > addr_max) {
		serial_printf (KRED "image out of range  %#010lx : %#010lx" NEWLINE KNRM, addr_min,
						 addr_max);

		return FIRMWARE_IMAGE_LOAD_FAILED;
	}

	serial_printf (KMAG NEWLINE "bootblock: copy fw %#010lx size %#010lx to %#010lx" NEWLINE KNRM,
					 src_flash_addr, src_size, dst_addr);

	/* copy the image from flash to RAM */
	image_memcpy (dst_addr, src_flash_addr, src_size, true);


	return 0;
}



/**
 * Print the type of the last reset.
 */
void print_reset_indication (void)
{
	uint32_t intcr2 = REG_READ (INTCR2);
	serial_printf (KBLU "RESSR    =  %#010lx\n", REG_READ (RESSR));

	serial_printf ("INTCR2   =  %#010lx\n" KNRM, REG_READ (INTCR2));

	if (READ_VAR_FIELD(intcr2, INTCR2_WD2RST))
		serial_printf (KBLU "Last reset was WD2\n" KNRM);
	if (READ_VAR_FIELD(intcr2, INTCR2_WD1RST))
		serial_printf (KBLU "Last reset was WD1\n" KNRM);
	if (READ_VAR_FIELD(intcr2, INTCR2_TIP_RESET))
		serial_printf (KBLU "Last reset was TIP RESET\n" KNRM);
	if (READ_VAR_FIELD(intcr2, INTCR2_SWR3ST))
		serial_printf (KBLU "Last reset was SW3\n" KNRM);
	if (READ_VAR_FIELD(intcr2, INTCR2_SWR2ST))
		serial_printf (KBLU "Last reset was SW2\n" KNRM);
	if (READ_VAR_FIELD(intcr2, INTCR2_SWR1ST))
		serial_printf (KBLU "Last reset was SW1\n" KNRM);
	if (READ_VAR_FIELD(intcr2, INTCR2_WD0RST))
		serial_printf (KBLU "Last reset was WD0\n" KNRM);
	if (READ_VAR_FIELD(intcr2, INTCR2_CORST))
		serial_printf (KBLU "Last reset was CORST\n" KNRM);
	if (READ_VAR_FIELD(intcr2, INTCR2_PORST))
		serial_printf (KBLU "Last reset was PORST\n" KNRM);

}

/**
 * Update TIP reset indication
 *
 * @param updateIntcr2 if true, update also INTCR2 so the BMC will have the access in case
 * we jump directly from TIP ROM to BMC.
 */
 
void update_reset_indication (void)
{
	uint32_t rcr1;
	uint32_t rcr2;
	uint32_t intcr2 = REG_READ (INTCR2);

	print_reset_indication ();

	/* in PORTST only: init RCR registers. PORST and CFGDONE are set */
	if (READ_VAR_FIELD(intcr2, INTCR2_PORST)) {
		/* configure warm reset handling */
		rcr1 = 0xFFFFFFFF;
		rcr2 = 0xFFFFFFFF;

		SET_VAR_FIELD (rcr1, WD0RCR_BMCDBG, 0);
		SET_VAR_FIELD (rcr1, WD0RCR_MC, 0);
		SET_VAR_FIELD (rcr1, WD0RCR_CLKS, 0);
		//SET_VAR_FIELD (rcr1, WD0RCR_TIP_Reset, 0);
		SET_VAR_FIELD (rcr1, WD0RCR_GPIOM0, 0);
		SET_VAR_FIELD (rcr1, WD0RCR_GPIOM1, 0);
		SET_VAR_FIELD (rcr1, WD0RCR_GPIOM2, 0);
		SET_VAR_FIELD (rcr1, WD0RCR_GPIOM3, 0);
		SET_VAR_FIELD (rcr1, WD0RCR_GPIOM4, 0);
		SET_VAR_FIELD (rcr1, WD0RCR_GPIOM5, 0);
		SET_VAR_FIELD (rcr1, WD0RCR_GPIOM6, 0);
		SET_VAR_FIELD (rcr1, WD0RCR_GPIOM7, 0);
		SET_VAR_FIELD (rcr1, WD0RCR_SPIBMC, 0);
		SET_VAR_FIELD (rcr1, WD0RCR_SPER, 0);
		SET_VAR_FIELD (rcr1, WD0RCR_PWM, 0);
		SET_VAR_FIELD (rcr1, WD0RCR_SHM, 0);
		SET_VAR_FIELD (rcr1, WD0RCR_PCIERC, 0);
		SET_VAR_FIELD (rcr1, WD0RCR_ESPI, 0);

		if (CHIP_Get_Version() >= 0x04 ) {
			SET_VAR_FIELD (rcr2, WD0RCRB_SEC_REG_RST_A1, 0);
		} else {
			/* for Z1 only. In A1 this bit is locked by ROM, except for TIP RESET (TIPRSTCB) */
			SET_VAR_FIELD (rcr2, WD0RCRB_CP2_TIP_Z1, 1);
		}

		SET_VAR_FIELD (rcr2, WD0RCRB_HGPIO, 0);
		SET_VAR_FIELD (rcr2, WD0RCRB_FLM, 0);
		SET_VAR_FIELD (rcr2, WD0RCRB_PCIGFX, 0);
		SET_VAR_FIELD (rcr2, WD0RCRB_HOSTPER, 1);

		REG_WRITE (WD0RCR, rcr1);
		REG_WRITE (WD1RCR, rcr1);
		REG_WRITE (WD2RCR, rcr1);

		REG_WRITE (WD0RCRB, rcr2);
		REG_WRITE (WD1RCRB, rcr2);
		REG_WRITE (WD2RCRB, rcr2);


		REG_WRITE (SWRSTC1, rcr1);
		REG_WRITE (SWRSTC2, rcr1);
		REG_WRITE (SWRSTC3, rcr1);

		REG_WRITE (SWRSTC1B, rcr2);
		REG_WRITE (SWRSTC2B, rcr2);
		REG_WRITE (SWRSTC3B, rcr2);


		REG_WRITE (CORSTC, rcr1);
		REG_WRITE (CORSTCB, rcr2);

		serial_printf ("Init reset control regs: WD0RCR = %#010lx WD0RCRB = %#010lx" NEWLINE,
						 rcr1, rcr2);

		SET_VAR_FIELD (rcr1, WD0RCR_SPIBMC, 1);

		if (CHIP_Get_Version () >= 0x04 ) { 
			SET_VAR_FIELD (rcr2, WD0RCRB_SEC_REG_RST_A1, 1);
		} 
		else {
			SET_VAR_FIELD (rcr2, WD0RCRB_CP2_TIP_Z1, 1);
		}
		
		REG_WRITE (TIPRSTC, rcr1);
		REG_WRITE (TIPRSTCB, rcr2);

		serial_printf ("Init reset control regs: TIPRSTC = %#010lx TIPRSTCB = %#010lx" NEWLINE,
						 REG_READ (TIPRSTC), REG_READ (TIPRSTCB));
	}
	else {
		serial_printf ("Skip RCR init" NEWLINE);
	}

	/* update reset counter */
	REG_WRITE(SCRPAD_10_41(10), REG_READ(SCRPAD_10_41(10)) + 1);

	serial_printf ("SCRPAD_20=  %#010lx (%#010lx)\n", REG_READ (SCRPAD_10_41(10)), REG_ADDR(SCRPAD_10_41(10)));
	serial_printf ("INTCR2   =  %#010lx\n", REG_READ (INTCR2));
	serial_printf ("RESSR    =  %#010lx\n", REG_READ (RESSR));
}

/**
 *  @function   firmware_image_scan_flash
 *
 *  @param [in]  tip_fw_im    - fw handler
 *  @return
 *
 *  @details     scan the flash, searching for start tag.
 */
static int firmware_image_scan_flash (struct tip_firmware_image *tip_fw_im)
{
	int						 status = FIRMWARE_IMAGE_NOT_AVAILABLE;

	const uint8_t		*tag = tip_fw_l1_header_tag;
	TIP_HEADER_STRUCT_T *rom_header;
	uint32_t			 dst_addr = -1;
	uint32_t			 addr = 0;
	uint32_t			 scan_from;
	uint32_t			 scan_to;
	uint32_t			base_addr = 0x80000000;

	switch (tip_fw_im->img_type) {
		case IMG_BOOTBLOCK:
			tag = bb_header_tag;
			break;
		case IMG_BL31:
			tag = bl31_header_tag;
			break;
		case IMG_OPTEE:
			tag = optee_header_tag;
			break;
		case IMG_UBOOT:
			tag = uboot_header_tag;
			break;
		default:
			serial_printf ("%s: Invalid image type" NEWLINE, __func__);
			return status;
	};

	
	/* check overflow: */
	if (tip_fw_im->offset_minimum >= SPI0CS0_SIZE) {
		tip_fw_im->offset_minimum = 0;
	}

	scan_from = base_addr + tip_fw_im->offset_minimum;
	scan_to = base_addr + SPI0CS0_SIZE;

	serial_printf ("tag %c%c%c. scan from %#010lx to %#010lx" NEWLINE, tag[1], tag[2], tag[3],
					 scan_from, scan_to);

	/* scan the flash, search for tags. */
	for (addr = scan_from; addr < scan_to; addr += 0x1000) {
		if (COMPARE_START_TAG (addr, tag)) 
		{
			tip_fw_im->header->header_flash = (HEADER_GENERAL_T *) addr;
			dst_addr = tip_fw_im->header->header_flash->header.destAddr;
			tip_fw_im->header->header_ram = (HEADER_GENERAL_T *) dst_addr;
			tip_fw_im->offset_minimum = addr - base_addr;
			tip_fw_im->size =
				tip_fw_im->header->header_flash->header.codeSize + sizeof (HEADER_GENERAL_T);
			

			serial_printf (KGRN "%s %s found addr %#010lx dst %#010lx " NEWLINE KNRM, __func__,
							 image_firmware_get_fw_name (tip_fw_im->img_type), addr, dst_addr);

			status = 0;

			return status;
		}
	}
	return status;
}

/**
 *  @function   component_start
 *
 *  @param [in]  fw          - fw handler
 *  @param [in]  tip_header  - header handler
 *  @param [in]  flash       - flash to boot from (active flash)
 *  @param [in]  img_type    -
 *  @param [in]  start_offset
 *  @return
 *  @details     Load any application from the active flash, verify and take measurements
 */
int component_start (struct tip_firmware_image	*fw,
						 struct tip_firmware_header *header,
						 IMG_TYPE_E					 img_type,
						 int					*start_offset)
{
	int status_load = 0;
	
	/* Initialize HW instance  */
	fw->header = header;
	/* assume image is not loaded */
	fw->header->header_ram = 0;
	fw->img_type = img_type;
	fw->offset_minimum = *start_offset;
	fw->size = 0;
	
	/* scan for a specific image, if we know what we are*/
	if ((img_type != IMG_UNKNOWN) && (*start_offset >= 0)) {
		status_load = firmware_image_scan_flash (fw);
		if (status_load) {
			goto exit;
		}
	}

	
	/* start search for the next BMC component address. images are all alligned to 4KB*/
	*start_offset = fw->offset_minimum + ROUND_UP (fw->size, 0x1000);

	serial_printf ("==== LOAD %s ====" NEWLINE, image_firmware_get_fw_name (img_type));

	status_load = firmware_image_load_func (fw, (uint32_t) fw->header->header_flash);

	if (status_load != 0) {
		goto exit;
	}
	return status_load;

exit:
	
	serial_printf (KRED "no image found on flash" NEWLINE KNRM);
	while(1);
}

int bmc_firmware_init (uint64_t *addr64)
{
	struct tip_firmware_image  fw;
	struct tip_firmware_header tip_header;
	uint32_t				   img_type;
	int start_offset = 0;
	/* status per image */
	int status[4];
	int i = 0;
	
	update_reset_indication();
	
	/*
	 * Iterate on the BMC images ( bl31, optee, uboot)
	 * note: image enums are bitwise.
	 */
	for (img_type =  IMG_BL31; img_type <=  IMG_UBOOT; img_type = img_type << 1){
		
		serial_printf (KCYN "\n==========\nStart %s\n==========" NEWLINE KNRM,	image_firmware_get_fw_name ((IMG_TYPE_E) img_type));
		status[i] = component_start (&fw, &tip_header, (IMG_TYPE_E) img_type, &start_offset);
		
		if (status[i]) {
			serial_printf (KRED "\n%s not started\n\n" NEWLINE KNRM,
			image_firmware_get_fw_name ((IMG_TYPE_E) img_type));
		}
		if (img_type ==  IMG_BL31)
		{
			*addr64 = tip_header.header_ram->header.destAddr + sizeof(HEADER_GENERAL_T);
		}
		i++;
	}
	
	if ((status[0] | status[1] | status[2] | status[3]) == 0)
		return 0;
	else
		return FIRMWARE_IMAGE_LOAD_FAILED;

}
#endif
