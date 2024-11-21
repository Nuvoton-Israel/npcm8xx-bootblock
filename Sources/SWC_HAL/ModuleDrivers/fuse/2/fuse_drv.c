/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   fuse_drv.c                                                               */
/*            This file contains FUSE module routines                         */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/

#include __CHIP_H_FROM_DRV()

#include "hal.h"

#if defined AES_MODULE_TYPE
#include __MODULE_IF_HEADER_FROM_DRV(aes)
#endif

#if defined ECC_MODULE_TYPE
#include __LOGICAL_IF_HEADER_FROM_DRV(ecc)
#endif


#include "fuse_drv.h"
#include "fuse_regs.h"


/*---------------------------------------------------------------------------------------------------------*/
/* Fuse module constant definitions                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

// Read cycle initiation value:
#define READ_INIT                   0x02

// Program cycle initiation values (a sequence of two adjacent writes is required):
#define PROGRAM_ARM                 0x1
#define PROGRAM_INIT                0xBF79E5D0

// Value to clean FDATA contents:
#define FDATA_CLEAN_VALUE           0x01

// Default APB Clock Rate (in MHz):
#define DEFAULT_APB_RATE            0x30

#define FUSE_RDY_MAX_READY_PULSES   0x1000
#define MIN_PROGRAM_PULSES          4
#define MAX_PROGRAM_PULSES          20

#define FUSE_CAPABILITY_SMART_PROGRAMMING

#define FUSE_NUM_OF_AREAS           8
#define FUSE_NUM_OF_AES_KEYS        4

#define FUSE_AESKEY_SIZE            64
#define FUSE_NUM_OF_AESKEYS         4
#define FUSE_AESKEY_OFFSET(index)   (256 + (index) * FUSE_AESKEY_SIZE)
#define FUSE_AESKEY_BLOCK(index)    (6 + (index))

#define FUSE_ECCKEY_SIZE            128
#define FUSE_NUM_OF_ECCKEYS         16
#define FUSE_ECCKEY_OFFSET(index)   (3328 + (index) * FUSE_ECCKEY_SIZE)
#define FUSE_ECCKEY_BLOCK(index)    (32 + (index))

/*---------------------------------------------------------------------------------------------------------*/
/* Fuse module local macro definitions                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#define KEY_IS_VALID()              READ_REG_FIELD(FKEYIND, FKEYIND_KVAL)


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                                VARIABLES                                                */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#ifdef FUSE_CAPABILITY_SECURE_VALUE
static UINT32           FUSE_Prog_Magic_Word;
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Internal functions for this module                                                                      */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_WaitForOTPReadyWithTimeout                                                        */
/*                                                                                                         */
/* Parameters:      array - fuse array to wait for                                                         */
/* Parameters:      timeout - number of retries to wait for FST_RDY                                        */
/* Returns:         DEFS_STATUS_OK on successful read completion, DEFS_ERROR_* otherwise.                  */
/* Side effects:                                                                                           */
/* Description:     Initialize the Fuse HW module.                                                         */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FUSE_WaitForOTPReadyWithTimeout (_UNUSED_ FUSE_STORAGE_ARRAY_T array, UINT32 timeout)
{
    UINT32 time = timeout;

    while (--time > 1)
    {
        if (READ_REG_FIELD(FST, FST_RDY))
        {
            /* fuse is ready, clear the status. */
            SET_REG_FIELD(FST, FST_RDST, 1);

            return DEFS_STATUS_OK;
        }
    }

    /* try to clear the status in case it was set */
    SET_REG_FIELD(FST, FST_RDST, 1);

    return DEFS_STATUS_HARDWARE_ERROR;
}

/*---------------------------------------------------------------------------------------------------------*/
/* HW level functions                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/


#ifdef FUSE_CAPABILITY_SECURE_VALUE
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_Set_Prog_Magic_Word                                                               */
/*                                                                                                         */
/* Parameters:      value - value to init FUSE_Prog_Magic_Word or to add for it                            */
/*                  init  - if TRUE (init) FUSE_Prog_Magic_Word = value                                    */
/*                          else FUSE_Prog_Magic_Word += value                                             */
/* Returns:         DEFS_STATUS_OK on successful read completion, DEFS_ERROR_* otherwise.                  */
/* Description:     Set FUSE_Prog_Magic_Word.                                                              */
/*                  This feature protects against programming fuses when the code is not running as        */
/*                  expected due to a SW issue or malicious hacking of the code                            */
/* Usage Example:   See FUSE_ExampleCode() blow                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void FUSE_Set_Prog_Magic_Word (UINT32 value, BOOLEAN init)
{
    if (init)
    {
        FUSE_Prog_Magic_Word = value;
    }
    else
    {
        FUSE_Prog_Magic_Word += value;
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_Get_Prog_Magic_Word                                                               */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/*                                                                                                         */
/* Returns:         PROGRAM_INIT value                                                                     */
/* Description:     Return the PROGRAM_INIT to be used to set FUSE_Prog_Magic_Word.                        */
/* Usage Example:   See FUSE_ExampleCode() blow                                                            */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 FUSE_Get_Prog_Magic_Word (void)
{
    return PROGRAM_INIT;
}

#ifdef FUSE_EXAMPLE_CODE // this is just an example so don't define it

void FUSE_ExampleCode(BOOLEAN condition1, BOOLEAN condition2, UINT16 byteNum, UINT8 value)
{
    // Fuse Init does not allow you yet to program the Magic Word == 0
    FUSE_Init();

    // starting building the Magic Word but reduce 0xFFFFFF00 from it
    FUSE_Set_Prog_Magic_Word(FUSE_Get_Prog_Magic_Word() - 0xFFFFFF00, TRUE);

    if (condition1g)
    {
        // now we add to the Magic Word 0x0000FF00, so we are missing 0xFFFF0000
        FUSE_Set_Prog_Magic_Word(0x0000FF00, FALSE);
        if (condition2)
        {
            // now we add to the Magic Word 0xFFFF0000, so we finished building the Magic Word is ready
            FUSE_Set_Prog_Magic_Word(0xFFFF0000, FALSE);
            FUSE_ProgramByte(0, byteNum, value);
        }
    }

    // Set back Magic Word to 0, so programing will not be available
    FUSE_Set_Prog_Magic_Word(0, TRUE);
}

#endif // FUSE_EXAMPLE_CODE

#endif // FUSE_CAPABILITY_SECURE_VALUE


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_Init                                                                              */
/*                                                                                                         */
/* Parameters:      APBclock - APB clock rate in MHz                                                       */
/* Returns:         DEFS_STATUS_OK on successful read completion, DEFS_ERROR_* otherwise.                  */
/* Side effects:                                                                                           */
/* Description:     Initialize the Fuse HW module.                                                         */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FUSE_Init (void)
{
    // . Informs the fuse array state machine on the APB4 bus clock frequency in
    // MHz. The software must update this field with the clock frequency of the APB before reading or writing
    // to the OTP. The state machine contains an internal copy of this field, sampled at the beginning of every
    // read or program operation. Software should not write this field with 0. The reset value of this field is 19h
    // (25 MHz). The accuracy of the setting should be 10% for write operation; the setting must be equal or
    // higher than the actual APB4 bus frequency for read operations. APBRT field exists only in FCFG0, and
    // is reserved in FCFG1-7. This field exists but is not used in TIP port FCFG0 register.
    // For 62.5 MHz set this field to 3Fh (63 decimal); for 31.25 MHz set this field to 1Fh (31 decimal).
    // Note: The minimum APB4 allowed frequency for programming the fuse arrays is 20 MHz.


    UINT8 APBclock = (UINT8)DIV_CEILING(CLK_GetAPBFreq(CLK_APB4), _1MHz_) ; // round down

    /* Configure the APB Clock Rate. Note: this has to be done on the A35 address space! */
#ifdef NPCM850_TIP
    SET_REG_FIELD(FCFG0_7_TIP(0), FCFG0_7_APBRT, APBclock);
#endif
    SET_REG_FIELD(FCFG0_7_A35(0), FCFG0_7_APBRT, APBclock);

    HAL_PRINT("Fuse_Init: FCFG0_7 = 0x%lx (APBRT)\n", REG_READ(FCFG0_7_A35(0)));

#ifdef FUSE_CAPABILITY_SECURE_VALUE
    FUSE_Prog_Magic_Word = 0;
#endif

    return DEFS_STATUS_OK;
}

#if TIP_DUMMY_OTP_ON_RAM
void FUSE_Read (FUSE_STORAGE_ARRAY_T arr,
                UINT16               addr,
                UINT8               *data
)
{
    UINT8* virtual_otp = (UINT8*)RAM3_BASE_ADDR;
    UINT8  block       = FUSE_WRPR_PROP_BLOCK_ADDR(addr);
    if (TIP_FCFG_BLOCK_IS_R_LOCKED(block) == TRUE)
    {
        *data = 0;
    }
    *data = virtual_otp[addr];

}
#else
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_Read                                                                              */
/*                                                                                                         */
/* Parameters:      arr  - Storage Array type [input].                                                     */
/*                  addr - Byte-address to read from [input].                                              */
/*                  data - Pointer to result [output].                                                     */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:     Read 8-bit data from an OTP storage array.                                             */
/*---------------------------------------------------------------------------------------------------------*/
void FUSE_Read (FUSE_STORAGE_ARRAY_T arr,
                UINT16               addr,
                UINT8               *data
)
{
    /* Wait for the Fuse Box Idle */
    FUSE_WaitForOTPReadyWithTimeout(arr, FUSE_RDY_MAX_READY_PULSES);

    /* Configure the byte address in the fuse array for read operation */
    SET_REG_FIELD(FADDR, FADDR_BYTEADDR, addr);

    /* Initiate a read cycle from the byte in the fuse array, pointed by FADDR */
    REG_WRITE(FCTL, READ_INIT);

    /* Wait for read operation completion */
    FUSE_WaitForOTPReadyWithTimeout(arr, FUSE_RDY_MAX_READY_PULSES);

    /* Read the result */
    *data = READ_REG_FIELD(FDATA, FDATA_FDATA);

    /* Clean FDATA contents to prevent unauthorized software from reading sensitive information */
    SET_REG_FIELD(FDATA, FDATA_FDATA, FDATA_CLEAN_VALUE);
}
#endif

void FUSE_SetBlockAccess (_UNUSED_ FUSE_STORAGE_ARRAY_T array,
                          UINT32 block,
                          BOOLEAN lockForRead,
                          BOOLEAN lockForWrite,
                          BOOLEAN lockRegister)
{
    UINT32 fcfg = 0;
    UINT32 bit;

    bit = block % 8;

    SET_VAR_FIELD(fcfg, FCFG0_7_FRDLK,   (lockForRead & 0x01)  << bit);
    SET_VAR_FIELD(fcfg, FCFG0_7_FPRGDIS, (lockForWrite & 0x01) << bit);
    SET_VAR_FIELD(fcfg, FCFG0_7_FCFGLK,  (lockRegister & 0x01) << bit);

    SET_REG_MASK(FCFG0_7(block / 8), fcfg);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_BitIsProgrammed                                                                   */
/*                                                                                                         */
/* Parameters:      arr     - Storage Array type [input].                                                  */
/*                  byteNum - Byte offset in array [input].                                                */
/*                  bitNum  - Bit offset in byte [input].                                                  */
/* Returns:         Nonzero if bit is programmed, zero otherwise.                                          */
/* Side effects:                                                                                           */
/* Description:     Check if a bit is programmed in an OTP storage array.                                  */
/*---------------------------------------------------------------------------------------------------------*/
BOOLEAN FUSE_BitIsProgrammed (
    FUSE_STORAGE_ARRAY_T  arr,
    UINT16                byteNum,
    UINT8                 bitNum
)
{
    UINT8 data;

    /* Read the entire byte you wish to program */
    FUSE_Read(arr, byteNum, &data);

    /* Check whether the bit is already programmed */
    if (READ_VAR_BIT(data, bitNum))
    {
        return TRUE;
    }

    return FALSE;
}

#if TIP_DUMMY_OTP_ON_RAM
DEFS_STATUS FUSE_ProgramBit (
    FUSE_STORAGE_ARRAY_T arr,
    UINT16               byteNum,
    UINT8                bitNum
)
{
    UINT8* virtual_otp = (UINT8*)RAM3_BASE_ADDR;
    //take the block from the byteNum, (=address) , check if its blocked. if so, exit.
    UINT8  block       = FUSE_WRPR_PROP_BLOCK_ADDR(byteNum);
    HAL_PRINT ("block is %d\n ", block);
    //verify first that the block is open to read and not locked
    if (TIP_FCFG_BLOCK_IS_W_LOCKED(block) == TRUE)
    {
        HAL_PRINT (KRED" % is locked for write ! \n "KNRM,byteNum);
        return DEFS_STATUS_FAIL;
    }
    virtual_otp[byteNum] |= (1 << bitNum);

    return DEFS_STATUS_OK;
}
#else
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_ProgramBit                                                                        */
/*                                                                                                         */
/* Parameters:      arr     - Storage Array type [input].                                                  */
/*                  byteNum - Byte offset in array [input].                                                */
/*                  bitNum  - Bit offset in byte [input].                                                  */
/* Returns:         DEFS_STATUS_OK on successful read completion, HAL_ERROR_* otherwise.                   */
/* Side effects:                                                                                           */
/* Description:     Program (set to 1) a bit in an OTP storage array.                                      */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FUSE_ProgramBit (
    FUSE_STORAGE_ARRAY_T arr,
    UINT16               byteNum,
    UINT8                bitNum,
    BOOLEAN              CheckIfBitIsProgrammed
)
{
    DEFS_STATUS status = DEFS_STATUS_OK;
    BOOLEAN shouldProgram;
    /* Wait for the Fuse Box Idle */
    FUSE_WaitForOTPReadyWithTimeout(arr, FUSE_RDY_MAX_READY_PULSES);
    if (CheckIfBitIsProgrammed)
        shouldProgram = !FUSE_BitIsProgrammed(arr, byteNum, bitNum);/* Make sure the bit is not already programmed */
    else
        shouldProgram = TRUE;
    if (shouldProgram)
    {
#ifdef FUSE_CAPABILITY_SMART_PROGRAMMING

        /* Configure the bit address in the fuse array for program operation */
        REG_WRITE(FADDR, BUILD_FIELD_VAL(FADDR_BYTEADDR, byteNum) | BUILD_FIELD_VAL(FADDR_BITPOS, bitNum));

        /* perform smart programming algorithm */
        SET_REG_FIELD(FADDR, FADDR_SPEN, 1);
        SET_REG_FIELD(FST, FST_RIEN, 0);

        /* Arm the program operation */
        REG_WRITE(FCTL, PROGRAM_ARM);

        /* Initiate a program cycle to the bit in the fuse array, pointed by FADDR */
#ifdef FUSE_CAPABILITY_SECURE_VALUE
        REG_WRITE(FCTL, FUSE_Prog_Magic_Word);
        if (FUSE_Prog_Magic_Word != PROGRAM_INIT)
        {
            HAL_PRINT("Magic Word == 0x%08x instead of 0x%08x", FUSE_Prog_Magic_Word, PROGRAM_INIT);
        }
#else
        REG_WRITE(FCTL, PROGRAM_INIT);
#endif

        /* Wait for program operation completion */
        FUSE_WaitForOTPReadyWithTimeout(arr, FUSE_RDY_MAX_READY_PULSES);

        /* Clear smart programming algorithm */
        SET_REG_FIELD(FADDR, FADDR_SPEN, 0);

        if (FUSE_BitIsProgrammed(arr, byteNum, bitNum))
            return DEFS_STATUS_OK;
        else
            return DEFS_STATUS_HARDWARE_ERROR;
#else
        int count;

        /* Configure the bit address in the fuse array for program operation */
        REG_WRITE(FADDR, BUILD_FIELD_VAL(FADDR_BYTEADDR, byteNum) | BUILD_FIELD_VAL(FADDR_BITPOS, bitNum));

        /* perform regular programming algorithm */
        SET_REG_FIELD(FADDR, FADDR_IN_PROG, 1);

        // program up to MAX_PROGRAM_PULSES
        for (count = 1; count <= MAX_PROGRAM_PULSES; count++)
        {
            /* Arm the program operation */
            REG_WRITE(FCTL, PROGRAM_ARM);

            /* Initiate a program cycle to the bit in the fuse array, pointed by FADDR */
#ifdef FUSE_CAPABILITY_SECURE_VALUE
            REG_WRITE(FCTL, FUSE_Prog_Magic_Word);
            if (FUSE_Prog_Magic_Word != PROGRAM_INIT)
            {
                HAL_PRINT("Magic Word == 0x%08x instead of 0x%08x", FUSE_Prog_Magic_Word, PROGRAM_INIT);
            }
#else
            REG_WRITE(FCTL, PROGRAM_INIT);
#endif

            /* Wait for program operation completion */
            FUSE_WaitForOTPReadyWithTimeout(arr, FUSE_RDY_MAX_READY_PULSES);

            /* In regular programming, after MIN_PROGRAM_PULSES start verifying the result */
            if (count >= MIN_PROGRAM_PULSES)
            {
                if (FUSE_BitIsProgrammed(arr, byteNum, bitNum))
                    break;
            }
        }

        // check if programmking failed
        if (count > MAX_PROGRAM_PULSES)
            status = DEFS_STATUS_HARDWARE_ERROR;

        /* Clean FDATA contents to prevent unauthorized software from reading sensitive information */
        SET_REG_FIELD(FDATA, FDATA_FDATA, FDATA_CLEAN_VALUE);

        /* Clear FADDR_IN_PROG */
        SET_REG_FIELD(FADDR, FADDR_IN_PROG, 0);
#endif
    }

    return status;
}
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_ProgramByte                                                                       */
/*                                                                                                         */
/* Parameters:      arr     - Storage Array type [input].                                                  */
/*                  byteNum - Byte offset in array [input].                                                */
/*                  value   - Byte to program [input].                                                     */
/* Returns:         DEFS_STATUS_OK on successful read completion, HAL_ERROR_* otherwise.                   */
/* Side effects:                                                                                           */
/* Description:     Program (set to 1) a given byte's relevant bits in an OTP                              */
/*                  storage array.                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FUSE_ProgramByte (
    FUSE_STORAGE_ARRAY_T  arr,
    UINT16                byteNum,
    UINT8                 value
)
{
    UINT  i;

    UINT8 data;
    UINT8 bits_mask_to_program;
    DEFS_STATUS status = DEFS_STATUS_OK;

    /* Wait for the Fuse Box Idle */
    FUSE_WaitForOTPReadyWithTimeout(arr, FUSE_RDY_MAX_READY_PULSES);

    /* Read the entire byte you wish to program */
    FUSE_Read(arr, byteNum, &data);
    bits_mask_to_program = ~data & value;

    /* In case all relevant bits are already programmed - nothing to do */
    if ((bits_mask_to_program) == 0)
        return status;

    /* Program unprogrammed bits. */
    for (i = 0; i < 8; i++)
    {
        if (READ_VAR_BIT(bits_mask_to_program, i) == 1)
        {
            /* Program (set to 1) the relevant bit */
            DEFS_STATUS last_status = FUSE_ProgramBit(arr, byteNum, (UINT8)i, FALSE);
            if (last_status != DEFS_STATUS_OK)
            {
                status = last_status;
            }
        }
    }

    return status;
}

#if 1//(defined ECC_MODULE_TYPE) || (defined AES_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_SelectKey                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  keyIndex - AES key index in the key array (in 128-bit steps) [input].                  */
/* Returns:         DEFS_STATUS_OK on successful read completion, DEFS_STATUS_ERROR* otherwise.            */
/* Side effects:                                                                                           */
/* Description:     Select a key from the key storage array.                                               */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FUSE_SelectKey (
    UINT8            keyIndex  )
{

    UINT32  fKeyInd = 0;
    UINT32 time = 0xDAEDBEEF;

    /* Check valid AES key index */
    DEFS_STATUS_COND_CHECK (keyIndex < FUSE_NUM_OF_AES_KEYS , DEFS_STATUS_PARAMETER_OUT_OF_RANGE);

    /* Do not destroy ECCDIS bit */
    fKeyInd = REG_READ(FKEYIND);

    /* Configure the key size */
    SET_VAR_FIELD(fKeyInd, FKEYIND_KSIZE, FKEYIND_KSIZE_VALUE_256);

    /* Configure the key index (0 to 3) */
    SET_VAR_FIELD(fKeyInd, FKEYIND_KIND, keyIndex);

    REG_WRITE(FKEYIND, fKeyInd);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Wait for selection completetion                                                                     */
    /*-----------------------------------------------------------------------------------------------------*/
    while (--time > 1)
    {
        if (READ_REG_FIELD(FKEYIND, FKEYIND_KVAL))
            return DEFS_STATUS_OK;
    }

    return DEFS_STATUS_HARDWARE_ERROR;
}

#if defined (AES_MODULE_TYPE)
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_UploadKey                                                                         */
/*                                                                                                         */
/* Parameters:      keySize  - AES key size [input].                                                       */
/*                  keyIndex - AES key index in the key array (in 128-bit steps) [input].                  */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:     Upload key from key array OTP to AES engine through side-band port.                    */
/*                  The function assumes that the AES engine is IDLE.                                      */
/*---------------------------------------------------------------------------------------------------------*/
void FUSE_UploadKey (
    AES_KEY_SIZE_T keySize,
    UINT8          keyIndex
)
{
    UINT32  fKeyInd = 0;

    /* Do not destroy ECCDIS bit */
    fKeyInd = REG_READ(FKEYIND);

    /* Configure the key size */
    SET_VAR_FIELD(fKeyInd, FKEYIND_KSIZE, keySize);

    /* Configure the index of the key in the fuse array in 128-bit steps */
    SET_VAR_FIELD(fKeyInd, FKEYIND_KIND, keyIndex);

    /* FKEYIND must be configured in one operation */
    REG_WRITE(FKEYIND, fKeyInd);

    /* Wait for upload operation completion */
    while (! KEY_IS_VALID()) {};
}
#endif

#if defined ( AES_MODULE_TYPE ) ||  defined ( ECC_MODULE_TYPE )
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_ReadKey                                                                           */
/*                                                                                                         */
/* Parameters:      keySize  - AES key size [input].                                                       */
/*                  keyIndex - AES key index in the key array (in 128-bit steps) [input].                  */
/*                  output   - Byte-array to hold the key [output].                                        */
/* Returns:         DEFS_STATUS_OK on successful read completion, HAL_ERROR_* otherwise.                   */
/* Side effects:                                                                                           */
/* Description:     Read a key from the key storage array.                                                 */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FUSE_ReadKey (
#if (defined ECC_MODULE_TYPE) || (defined AES_MODULE_TYPE)
    FUSE_KEY_TYPE_T  keyType,
    UINT8            keyIndex,
#endif
    UINT8            *output)
{
    UINT  i;
    UINT16 keyAddr = 0 ;
    UINT16 keySizeBytes = 0;
    UINT8 fuseReadLock = 0;
    UINT8 keyBlock = 0;

    /*-----------------------------------------------------------------------------------------------------*/
    /* keyType can be either AES (4 keys, 32 bytes - AES_KEY_SIZE_256) or ECC (3 keys, 64 bytes)           */
    /*-----------------------------------------------------------------------------------------------------*/
#if defined ( AES_MODULE_TYPE )
    /* Verify that APB Read Access to the Key Storage Array is allowed */
    if (READ_REG_FIELD(FUSTRAP1, FUSTRAP1_OAESKEYACCLK))
        return DEFS_STATUS_FAIL;

    if (keyType == FUSE_KEY_AES)
    {
        keySizeBytes = FUSE_AESKEY_SIZE;
        keyAddr = FUSE_AESKEY_OFFSET(keyIndex);
        keyBlock = FUSE_AESKEY_BLOCK(keyIndex);
    }
#endif

#if defined ( ECC_MODULE_TYPE )
    if (keyType == FUSE_KEY_ECC)
    {
        keySizeBytes = FUSE_ECCKEY_SIZE;
        keyAddr = FUSE_ECCKEY_OFFSET(keyIndex);
        keyBlock = FUSE_ECCKEY_BLOCK(keyIndex);
    }
#endif

    /* Verify that read access to the key is allowed */
    fuseReadLock = READ_REG_FIELD(FCFG0_7(keyBlock / 8), FCFG0_7_FRDLK);
    if (READ_VAR_MASK(fuseReadLock,  MASK_BIT(keyBlock % 8)))
        return DEFS_STATUS_FAIL;

    /* Read Key */
    for (i = 0; i < keySizeBytes; i++, keyAddr++)
        FUSE_Read(KEY_SA, keyAddr, &output[i]);

    return DEFS_STATUS_OK;
}
#endif
#endif /* defined ( AES_MODULE_TYPE ) ||  defined ( ECC_MODULE_TYPE ) */
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_DisableKeyAccess                                                                  */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine disables read and write accees to key array                               */
/*---------------------------------------------------------------------------------------------------------*/
void FUSE_DisableKeyAccess ()
{
    SET_REG_FIELD(FCFG0_7(0), FCFG0_7_FDIS, 1);     /* Lock OTP module access  */
}

#if 0
/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_LockAccess                                                                        */
/*                                                                                                         */
/* Parameters:      lockForRead: bitwise, which block to lock for reading                                  */
/* Parameters:      lockForWrite: bitwise, which block to lock for program                                 */
/* Returns:         DEFS_STATUS_OK on successful read completion, HAL_ERROR_* otherwise.                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine lock the otp blocks                                                       */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FUSE_LockAccess (FUSE_STORAGE_ARRAY_T array, UINT8 lockForRead, UINT8 lockForWrite, BOOLEAN lockRegister)
{
    UINT32 FCFG_VAR = 0;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Read reg for modify all fields apart APBRT                                                          */
    /*-----------------------------------------------------------------------------------------------------*/
    FCFG_VAR = REG_READ(FCFG0_7(0));


    SET_VAR_FIELD(FCFG_VAR, FCFG_FRDLK,  lockForRead & 0x00FF);

    SET_VAR_FIELD(FCFG_VAR, FCFG_FPRGLK, lockForWrite & 0x00FF);

    /*-----------------------------------------------------------------------------------------------------*/
    /* Lock any access to this register (until next POR)                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
    if ( lockRegister == TRUE)
    {
        SET_VAR_FIELD(FCFG_VAR, FCFG0_7_FCFGLK, (lockForWrite | lockForRead) & 0x00FF);
    }
    else
    {
        SET_VAR_FIELD(FCFG_VAR, FCFG0_7_FCFGLK, 0);
    }

    /*----------------------------------------------------------------------------------------------------*/
    /* Lock the side band in case it's a key array, and read is locked                                    */
    /*----------------------------------------------------------------------------------------------------*/
    if (array == KEY_SA)
    {
        /* Set FDIS bit if oKAP bit 7 is set, to disable the side-band key loading. */
        if ( (lockForRead & 0x80) > 0 )
        {
            SET_VAR_FIELD(FCFG_VAR, FCFG0_7_FDIS, 1);  // 1: Access to the first 2048 bits of the fuse array is disabled.
        }
    }


    /*-----------------------------------------------------------------------------------------------------*/
    /* Return the moified value                                                                            */
    /*-----------------------------------------------------------------------------------------------------*/
    REG_WRITE(FCFG0_7(array), FCFG_VAR);

    return DEFS_STATUS_OK;
}

#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Logical level functions                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_NibParEccDecode                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  datain -       pointer to encoded data buffer (buffer size should be 2 x dataout)      */
/*                  dataout -      pointer to decoded data buffer                                          */
/*                  encoded_size - size of encoded data (decoded data x 2)                                 */
/*                                                                                                         */
/* Returns:         DEFS_STATUS_OK on successful read completion, HAL_ERROR_* otherwise.                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Decodes the data according to nibble parity ECC scheme.                                */
/*                  Size specifies the encoded data size.                                                  */
/*                  Decodes whole bytes only                                                               */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FUSE_NibParEccDecode (
    const UINT8  *datain,
    UINT8  *dataout,
    UINT32  encoded_size
)
{
    UINT32 i;
    DEFS_STATUS status = DEFS_STATUS_OK;

    /*
       nibble_decode
       LSB is the decode value
       MSB: if 0xf than more than 1 bit error
            else it is the corrected bit place + 1. For example:
              if MSB==0x1 the corrected bit is bit 0
              if MSB==0x8 the corrected bit is bit 7
    */
    const UINT8 nibble_decode[256] = {
        0x00, 0x10, 0x20, 0xf3, 0x30, 0xf5, 0xf6, 0x4f, 0x40, 0xf9, 0xfa, 0x3f, 0xfc, 0x2f, 0x1f, 0x0f,
        0x50, 0x71, 0x82, 0xf3, 0xf4, 0x65, 0xf6, 0xf7, 0xf8, 0xf9, 0x6a, 0xfb, 0xfc, 0x8d, 0x7e, 0x5f,
        0x60, 0xf1, 0xf2, 0xf3, 0x74, 0x55, 0xf6, 0x87, 0x88, 0xf9, 0x5a, 0x7b, 0xfc, 0xfd, 0xfe, 0x6f,
        0xf0, 0x35, 0x4a, 0xf3, 0x15, 0x05, 0xf6, 0x25, 0x2a, 0xf9, 0x0a, 0x1a, 0xfc, 0x45, 0x3a, 0xff,
        0x70, 0x51, 0xf2, 0x83, 0x64, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0x6b, 0x8c, 0xfd, 0x5e, 0x7f,
        0x11, 0x01, 0xf2, 0x21, 0xf4, 0x31, 0x4e, 0xf7, 0xf8, 0x41, 0x3e, 0xfb, 0x2e, 0xfd, 0x0e, 0x1e,
        0x34, 0xf1, 0xf2, 0x4b, 0x04, 0x14, 0x24, 0xf7, 0xf8, 0x2b, 0x1b, 0x0b, 0x44, 0xfd, 0xfe, 0x3b,
        0xf0, 0x61, 0xf2, 0xf3, 0x54, 0x75, 0x86, 0xf7, 0xf8, 0x89, 0x7a, 0x5b, 0xfc, 0xfd, 0x6e, 0xff,
        0x80, 0xf1, 0x52, 0x73, 0xf4, 0xf5, 0xf6, 0x67, 0x68, 0xf9, 0xfa, 0xfb, 0x7c, 0x5d, 0xfe, 0x8f,
        0x22, 0xf1, 0x02, 0x12, 0xf4, 0x4d, 0x32, 0xf7, 0xf8, 0x3d, 0x42, 0xfb, 0x1d, 0x0d, 0xfe, 0x2d,
        0x48, 0xf1, 0xf2, 0x37, 0xf4, 0x27, 0x17, 0x07, 0x08, 0x18, 0x28, 0xfb, 0x38, 0xfd, 0xfe, 0x47,
        0xf0, 0xf1, 0x62, 0xf3, 0xf4, 0x85, 0x76, 0x57, 0x58, 0x79, 0x8a, 0xfb, 0xfc, 0x6d, 0xfe, 0xff,
        0xf0, 0x23, 0x13, 0x03, 0x4c, 0xf5, 0xf6, 0x33, 0x3c, 0xf9, 0xfa, 0x43, 0x0c, 0x1c, 0x2c, 0xff,
        0xf0, 0x81, 0x72, 0x53, 0xf4, 0xf5, 0x66, 0xf7, 0xf8, 0x69, 0xfa, 0xfb, 0x5c, 0x7d, 0x8e, 0xff,
        0xf0, 0xf1, 0xf2, 0x63, 0x84, 0xf5, 0x56, 0x77, 0x78, 0x59, 0xfa, 0x8b, 0x6c, 0xfd, 0xfe, 0xff,
        0xf0, 0x49, 0x36, 0xf3, 0x26, 0xf5, 0x06, 0x16, 0x19, 0x09, 0xfa, 0x29, 0xfc, 0x39, 0x46, 0xff};

#define LSNF    0, 4
#define MSNF    4, 4

    for (i = 0; i < encoded_size; i++)
    {
        UINT8 decoded = nibble_decode[datain[i]];
        if (i % 2)
        {//Decode higher nibble
            SET_VAR_FIELD(dataout[i/2], MSNF, LSN(decoded));
        }
        else
        {//Decode lower nibble
            SET_VAR_FIELD(dataout[i/2], LSNF, LSN(decoded));
        }

        if (0xf == MSN(decoded))
            status = DEFS_STATUS_BAD_CHECKSUM;
    }

    return status;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_NibParEccEncode                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  datain -       pointer to decoded data buffer (buffer size should be 2 x dataout)      */
/*                  dataout -      pointer to encoded data buffer                                          */
/*                  encoded_size - size of encoded data (decoded data x 2)                                 */
/*                                                                                                         */
/* Returns:         DEFS_STATUS_OK on successful read completion, HAL_ERROR_* otherwise.                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Decodes the data according to nibble parity ECC scheme.                                */
/*                  Size specifies the encoded data size.                                                  */
/*                  Decodes whole bytes only                                                               */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FUSE_NibParEccEncode (
    const UINT8 *datain,
    UINT8 *dataout,
    UINT32 encoded_size
)
{
    UINT32 i;
    UINT32 decoded_size = encoded_size/2;
    const UINT8 nibble_encode[16] = {
        0x00, 0x51, 0x92, 0xc3, 0x64, 0x35, 0xf6, 0xa7, 0xa8, 0xf9, 0x3a, 0x6b, 0xcc, 0x9d, 0x5e, 0x0f};

    for (i = 0; i < decoded_size; i++)
    {
        dataout[i*2]   = nibble_encode[LSN(datain[i])];
        dataout[i*2+1] = nibble_encode[MSN(datain[i])];
    }

    return DEFS_STATUS_OK;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_MajRulEccDecode                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  datain -       pointer to encoded data buffer (buffer size should be 3 x dataout)      */
/*                  dataout -      pointer to decoded data buffer                                          */
/*                  encoded_size - size of encoded data (decoded data x 3)                                 */
/*                                                                                                         */
/* Returns:         DEFS_STATUS_OK on successful read completion, DEFS_ERROR_* otherwise.                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Decodes the data according to Major Rule ECC scheme.                                   */
/*                  Size specifies the encoded data size.                                                  */
/*                  Decodes whole bytes only                                                               */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FUSE_MajRulEccDecode (
    const UINT8 *datain,
    UINT8 *dataout,
    UINT32 encoded_size
)
{
    UINT  byte;
    UINT32 decoded_size;

    if (encoded_size % 3)
        return DEFS_STATUS_INVALID_PARAMETER;

    decoded_size = encoded_size/3;

    for (byte = 0; byte < decoded_size; byte++)
    {
        UINT8 byte0 = datain[decoded_size*0+byte];
        UINT8 byte1 = datain[decoded_size*1+byte];
        UINT8 byte2 = datain[decoded_size*2+byte];

        dataout[byte] = (byte1 & (byte0 | byte2)) | (byte0 & byte2);
    }
    return DEFS_STATUS_OK;

}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_MajRulEccEncode                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  datain -       pointer to decoded data buffer (buffer size should be 3 x dataout)      */
/*                  dataout -      pointer to encoded data buffer                                          */
/*                  encoded_size - size of encoded data (decoded data x 3)                                 */
/*                                                                                                         */
/* Returns:         DEFS_STATUS_OK on successful read completion, DEFS_ERROR_* otherwise.                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Decodes the data according to Major Rule ECC scheme.                                   */
/*                  Size specifies the encoded data size.                                                  */
/*                  Decodes whole bytes only                                                               */
/*---------------------------------------------------------------------------------------------------------*/
DEFS_STATUS FUSE_MajRulEccEncode (
    const UINT8 *datain,
    UINT8 *dataout,
    UINT32 encoded_size
)
{
    UINT  byte;
    UINT32 decoded_size;

    if (encoded_size % 3)
        return DEFS_STATUS_INVALID_PARAMETER;

    decoded_size = encoded_size/3;

    for (byte = 0; byte < decoded_size; byte++)
    {
        dataout[decoded_size*0+byte] = datain[byte];
        dataout[decoded_size*1+byte] = datain[byte];
        dataout[decoded_size*2+byte] = datain[byte];
    }

    return DEFS_STATUS_OK;

}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_ReadFustrap                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  oFuse - fuse value to read                                                             */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This is a getter for fustrap                                                           */
/*---------------------------------------------------------------------------------------------------------*/
UINT32 FUSE_Get_Fustrap(UINT8 fustrap)
{
    if (fustrap == 1)
        return REG_READ(FUSTRAP1);
    else if (fustrap == 2)
        return REG_READ(FUSTRAP2);

    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_PrintRegs                                                                         */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module registers                                               */
/*---------------------------------------------------------------------------------------------------------*/
void FUSE_PrintRegs (void)
{
    HAL_PRINT("/*--------------*/\n");
    HAL_PRINT("/*     FUSE     */\n");
    HAL_PRINT("/*--------------*/\n\n");

    FUSE_PrintModuleRegs(0);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_PrintModuleRegs                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  array - The Storage Array type module to be printed.                                   */
/*                                                                                                         */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module instance registers                                      */
/*lint -e{715}      Supress 'array' not referenced                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void FUSE_PrintModuleRegs (_UNUSED_ FUSE_STORAGE_ARRAY_T array)
{
    UINT i;

    HAL_PRINT("FUSE%1X:\n", (array+1));
    HAL_PRINT("------\n");
    HAL_PRINT("FST                = 0x%08X\n", REG_READ(FST));
    HAL_PRINT("FADDR              = 0x%08X\n", REG_READ(FADDR));
    HAL_PRINT("FDATA              = 0x%08X\n", REG_READ(FDATA));
    HAL_PRINT("FUSTRAP1           = 0x%08X\n", REG_READ(FUSTRAP1));
    HAL_PRINT("FUSTRAP2           = 0x%08X\n", REG_READ(FUSTRAP2));

    for (i = 0; i < 8 ; i++)
    {
        HAL_PRINT("FCFG%d              = 0x%08X\n", i, REG_READ(FCFG0_7(i)));
    }

    HAL_PRINT("FKEYIND            = 0x%08X\n", REG_READ(FKEYIND));

    HAL_PRINT("\n");
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        FUSE_PrintVersion                                                                      */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints the module version                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void FUSE_PrintVersion (void)
{

}


