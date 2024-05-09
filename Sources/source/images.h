/*----------------------------------------------------------------------------*/
/*   SPDX-License-Identifier: GPL-2.0                                         */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   images.h                                                                 */
/*            This file contains API of routines to boot an image             */
/*  Project:                                                                  */
/*            Arbel BootBlock A35                                             */
/*----------------------------------------------------------------------------*/

#ifndef _IMAGES_H
#define _IMAGES_H

#include "hal.h"

#define INTCR2_RESET1                23, 9    /* Bits 23-31 */
#define INTCR2_RESET2                12, 4    /* Bits 12-15 */

/**
 * General purpose image header structure
 */
#pragma pack(push, 1)
typedef union HEADER_GENERAL_tag
{
	struct
	{
		                                         /* Offset       Size(bytes) Description                                                       */
		uint8_t   startTag[8];                   /* 0            8           AA55_0850h, 424F_4F54h (‘BOOT’)                                   */
		uint32_t  reserved;                      /* 8            4                                                                             */
		uint32_t  img_crc;                       /* 12           4                                                                             */
		uint8_t   signature[96];                 /* 16           96         Boot Block ECC signature decrypted with Customer’s Private Key     */

		/* Start signed area: */
		uint8_t   reservedSigned[28];            /* 0x70        0x188        Reserved, signed.                                                 */
		uint32_t  KeyIndex;                      /* 0x8C         140                                                                           */
		uint8_t   reservedSigned1[8];            /* 0x90         8        Reserved, signed.                                                    */
		uint16_t  version;                       /* 0x98                                                                                       */
		uint8_t   reservedSigned2[0x22];         /* 0x9A         ( challenge, IV..  ROM only)                                                  */
		uint32_t  timestamp;                     /* 0xBC                                                                                       */
		uint8_t   reservedSigned3[0x138];        /* 0xC0         0x138       Reserved, signed.                                                 */
		uint32_t  destAddr;                      /* 0x1F8                                                                                      */
		uint32_t  codeSize;                      /* 0x1FC                                                                                      */
	} header;

	uint8_t    bytes[512];
	uint32_t   words[128];

}  HEADER_GENERAL_T;
#pragma pack(pop)


/**
 * TIP FW image type
 */
typedef enum {
	IMG_UNKNOWN = 0,
	IMG_KMT = 0x01,
	IMG_TFT_L0 = 0x02,
	IMG_SKMT = 0x04,
	IMG_TFT_L1 = 0x08,
	IMG_BOOTBLOCK = 0x10,
	IMG_BL31 = 0x20,
	IMG_OPTEE = 0x40,
	IMG_UBOOT = 0x80,
	IMG_COMBO0 = 0x0F,
	IMG_COMBO1 = 0xF0,
	IMG_LINUX_KERNEL,
	IMG_LINUX_DTS,
	IMG_LINUX_FS,
	IMG_LINUX_OPENBMC,
	IMG_FULL
} IMG_TYPE_E;

/**
 * TIP firmware image header definition
 */
struct tip_firmware_header
{
	HEADER_GENERAL_T *header_flash;		/**< Original header image on flash. */
	HEADER_GENERAL_T *header_ram;     	/**< Copy addres of header in RAM. If 0 then image not copied yet. */
};

/**
 * TIP firmware image definition
 */
struct tip_firmware_image
{
//	struct firmware_image base_img;

	struct tip_firmware_header *header;
	uint32_t size;
	uint32_t offset_minimum; 	/**< end of previous image */
	IMG_TYPE_E img_type;
};

int bmc_firmware_init (uint64_t *addr64);

#define COMPARE_START_TAG(a, b) \
	((*(uint32_t *) a == *(uint32_t *) b) && (*((uint32_t *) a + 1) == *((uint32_t *) b + 1)))

/*---------------------------------------------------------------------------------------------------------*/
/* Round (up) the number (val) on the (n) boundary. (n) must be power of 2                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define ROUND_UP(val, n)        (((val)+(n)-1) & ~((n)-1))

#define NEWLINE  "\n"

#ifndef true
#define true (1)
#endif

#ifndef false
#define false (0)
#endif

#define platform_printf  serial_printf

/**
 * Error codes that can be generated when accessing the firmware image.
 */
enum {
	FIRMWARE_IMAGE_INVALID_ARGUMENT =  (0x00),		/**< Input parameter is null or not valid. */
	FIRMWARE_IMAGE_NO_MEMORY =  (0x01),				/**< Memory allocation failed. */
	FIRMWARE_IMAGE_LOAD_FAILED =  (0x02),			/**< The firmware image was not parsed. */
	FIRMWARE_IMAGE_VERIFY_FAILED =  (0x03),			/**< An error not related to image validity caused verification to fail. */
	FIRMWARE_IMAGE_GET_SIZE_FAILED =  (0x04),		/**< The image size could not be determined. */
	FIRMWARE_IMAGE_GET_MANIFEST_FAILED =  (0x05),	/**< No key manifest could be retrieved. */
	FIRMWARE_IMAGE_INVALID_FORMAT =  (0x06),		/**< The image structure is not valid. */
	FIRMWARE_IMAGE_BAD_CHECKSUM=  (0x07),			/**< A checksum within in the image structure failed verification. */
	FIRMWARE_IMAGE_NOT_LOADED =  (0x08),			/**< No firmware image has been loaded by the instance. */
	FIRMWARE_IMAGE_NO_APP_KEY=  (0x09),			/**< Could not get the application signing key. */
	FIRMWARE_IMAGE_MANIFEST_REVOKED =  (0x0a),		/**< The key manifest contained in the image has been revoked. */
	FIRMWARE_IMAGE_NOT_AVAILABLE =  (0x0b),			/**< A firmware component is not available in the image. */
	FIRMWARE_IMAGE_INVALID_SIGNATURE =  (0x0c),		/**< An image signature is malformed. */
	FIRMWARE_IMAGE_FORCE_RECOVERY =  (0x0d),		/**< Force loading the recovery firmware image. */
	FIRMWARE_IMAGE_BAD_SIGNATURE=  (0x0e),			/**< Signature verification of the image failed. */
};
	

#endif /* _IMAGES_H */

