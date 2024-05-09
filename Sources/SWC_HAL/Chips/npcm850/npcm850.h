/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   npcm850.h                                                                */
/*            This file contains chip definitions for the given project       */
/* Project:                                                                   */
/*            SWC HAL                                                         */
/*----------------------------------------------------------------------------*/

#ifndef __NPCM850_CHIP_H_
#define __NPCM850_CHIP_H_

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                              CHIP                                                       */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------*/
/* Common MACROS (overriding HAL definitions)                                                              */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/* Microsecond delay loop                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#define DELAY_USEC(microSecDelay, coreClk)     CLK_Delay_MicroSec(microSecDelay)


/*---------------------------------------------------------------------------------------------------------*/
/* Chip Name                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define NPCM850
#define CHIP_INTERRUPT_PROVIDER     INTERRUPT_PROVIDER_GIC

#define EXT_CLOCK_FREQUENCY_MHZ     25
#define EXT_CLOCK_FREQUENCY_KHZ     (EXT_CLOCK_FREQUENCY_MHZ*_1KHz_)
#define EXT_CLOCK_FREQUENCY_HZ      (EXT_CLOCK_FREQUENCY_MHZ*_1MHz_)

#define AHB18_BASE_ADDR                                         0x80000000
#define AHB1_PHYS_BASE_ADDR                                     0xF0000000
#define A9_DEBUG_ACCESS_PORT_SLAVE_APB6_PHYS_BASE_ADDR          0xF0500000
#define MC_AHB_PHYS_BASE_ADDR                                   0xF0600000
#define AHB8_PHYS_BASE_ADDR                                     0xF0800000
#define GCR_PHYS_BASE_ADDR                                      0xF0800000
#define GCR_CP_CTRL_PHYS_BASE_ADDR                              0xF080C000
#define GCR_TIP_CTRL_PHYS_BASE_ADDR                             0xF080D000
#define CLK_PHYS_BASE_ADDR                                      0xF0801000
#define GMAC1_PHYS_BASE_ADDR                                    0xF0802000
#define GMAC2_PHYS_BASE_ADDR                                    0xF0804000
#define GMAC3_PHYS_BASE_ADDR                                    0xF0806000
#define GMAC4_PHYS_BASE_ADDR                                    0xF0808000
#define VCD_PHYS_BASE_ADDR                                      0xF0810000
#define ECE_PHYS_BASE_ADDR                                      0xF0820000
#define VDMA_PHYS_BASE_ADDR                                     0xF0822000
#define USBH1_PHYS_BASE_ADDR                                    0xF0828000
#define USBH2_PHYS_BASE_ADDR                                    0xF082A000
#define USB0_PHYS_BASE_ADDR                                     0xF0830000
#define USB1_PHYS_BASE_ADDR                                     0xF0831000
#define USB2_PHYS_BASE_ADDR                                     0xF0832000
#define USB3_PHYS_BASE_ADDR                                     0xF0833000
#define USB4_PHYS_BASE_ADDR                                     0xF0834000
#define USB5_PHYS_BASE_ADDR                                     0xF0835000
#define USB6_PHYS_BASE_ADDR                                     0xF0836000
#define USB7_PHYS_BASE_ADDR                                     0xF0837000
#define USB8_PHYS_BASE_ADDR                                     0xF0838000
#define USB9_PHYS_BASE_ADDR                                     0xF0839000
#define MMC_PHYS_BASE_ADDR                                      0xF0842000
#define PCIMBX_PHYS_BASE_ADDR                                   0xF0848000
#define GDMA0_PHYS_BASE_ADDR                                    0xF0850000
#define GDMA1_PHYS_BASE_ADDR                                    0xF0851000
#define GDMA2_PHYS_BASE_ADDR                                    0xF0852000
#define GDMA3_PHYS_BASE_ADDR                                    0xF0853000
#define AES_PHYS_BASE_ADDR                                      0xF0858000
#define DES_PHYS_BASE_ADDR                                      0xF0859000
#define SHA_PHYS_BASE_ADDR                                      0xF085A000
#define PKA_PHYS_BASE_ADDR                                      0xF085B000
#define DVC_PHYS_BASE_ADDR                                      0xF0868000
#define AHB3_PHYS_BASE_ADDR                                     0xA0000000
#define SPI3_CS0_PHYS_BASE_ADDR                                 0xA0000000
#define SPI3_CS1_PHYS_BASE_ADDR                                 0xA8000000
#define SPI3_CS2_PHYS_BASE_ADDR                                 0xB0000000
#define SPI3_CS3_PHYS_BASE_ADDR                                 0xB8000000
#define SPI3_PHYS_BASE_ADDR                                     0xC0000000
#define SHM_PHYS_BASE_ADDRESS                                   0xC0001000
#define SAF_SPI_PHYS_BASE_ADDR                                  0xC0007000
#define RAM3_PHYS_BASE_ADDR                                     0xC0008000
#define AHB14_PHYS_BASE_ADDR                                    0xE0000000
#define APB14_PHYS_BASE_ADDR                                    0xE0000000
#define VDMX_PHYS_BASE_ADDR                                     0xE0800000
#define PCIE_RC_PHYS_BASE_ADDR                                  0xE8000000
#define PCIE_RC_CONFIG_PHYS_BASE_ADDR                           0xE1000000
#define APBS_PHYS_BASE_ADDR                                     0xF0000000
#define APB1_PHYS_BASE_ADDR                                     0xF0000000
#define UART0_BASE_ADDR                                         0xF0000000
#define UART1_BASE_ADDR                                         0xF0001000
#define UART2_BASE_ADDR                                         0xF0002000
#define UART3_BASE_ADDR                                         0xF0003000
#define UART4_BASE_ADDR                                         0xF0004000
#define UART5_BASE_ADDR                                         0xF0005000
#define UART6_BASE_ADDR                                         0xF0006000
#define KCS_PHYS_BASE_ADDR                                      0xF0007000
#define RNG_PHYS_BASE_ADDR                                      0xF000B000
#define ADC1_PHYS_BASE_ADDR                                     0xF000C000
#define ADC2_PHYS_BASE_ADDR                                     0xF000D000
#define GFX_PHYS_BASE_ADDR                                      0xF000E000
#define FSW_PHYS_BASE_ADDR                                      0xF000F000
#define GPIO0_BASE_ADDR                                         0xF0010000
#define GPIO1_BASE_ADDR                                         0xF0011000
#define GPIO2_BASE_ADDR                                         0xF0012000
#define GPIO3_BASE_ADDR                                         0xF0013000
#define GPIO4_BASE_ADDR                                         0xF0014000
#define GPIO5_BASE_ADDR                                         0xF0015000
#define GPIO6_BASE_ADDR                                         0xF0016000
#define GPIO7_BASE_ADDR                                         0xF0017000
#define BT1_PHYS_BASE_ADDR                                      0xF0030000
#define BT2_PHYS_BASE_ADDR                                      0xF0031000
#define APB2_PHYS_BASE_ADDR                                     0xF0080000
#define SMB0_BASE_ADDR                                          0xF0080000
#define SMB1_BASE_ADDR                                          0xF0081000
#define SMB2_BASE_ADDR                                          0xF0082000
#define SMB3_BASE_ADDR                                          0xF0083000
#define SMB4_BASE_ADDR                                          0xF0084000
#define SMB5_BASE_ADDR                                          0xF0085000
#define SMB6_BASE_ADDR                                          0xF0086000
#define SMB7_BASE_ADDR                                          0xF0087000
#define SMB8_BASE_ADDR                                          0xF0088000
#define SMB9_BASE_ADDR                                          0xF0089000
#define SMB10_BASE_ADDR                                         0xF008A000
#define SMB11_BASE_ADDR                                         0xF008B000
#define SMB12_BASE_ADDR                                         0xF008C000
#define SMB13_BASE_ADDR                                         0xF008D000
#define SMB14_BASE_ADDR                                         0xF008E000
#define SMB15_BASE_ADDR                                         0xF008F000
#define ESPI_PHYS_BASE_ADDR                                     0xF009F000
#define APB3_PHYS_BASE_ADDR                                     0xF0100000
#define PECI_PHYS_BASE_ADDR                                     0xF0100000
#define SERIAL_GPIO_EXPANSION1_PHYS_BASE_ADDR                   0xF0101000
#define SERIAL_GPIO_EXPANSION2_PHYS_BASE_ADDR                   0xF0102000
#define PWM0_PHYS_BASE_ADDR                                     0xF0103000
#define PWM1_PHYS_BASE_ADDR                                     0xF0104000
#define PWM2_PHYS_BASE_ADDR                                     0xF0105000
#define APB4_PHYS_BASE_ADDR                                     0xF0180000
#define MFT0_PHYS_BASE_ADDR                                     0xF0180000
#define MFT1_PHYS_BASE_ADDR                                     0xF0181000
#define MFT2_PHYS_BASE_ADDR                                     0xF0182000
#define MFT3_PHYS_BASE_ADDR                                     0xF0183000
#define MFT4_PHYS_BASE_ADDR                                     0xF0184000
#define MFT5_PHYS_BASE_ADDR                                     0xF0185000
#define MFT6_PHYS_BASE_ADDR                                     0xF0186000
#define MFT7_PHYS_BASE_ADDR                                     0xF0187000
#define THERMAL_SENSOR_PHYS_BASE_ADDR                           0xF0188000
#define OTP1_ROM_PHYS_BASE_ADDR                                 0xF0189000
#define OTP2_ROM_PHYS_BASE_ADDR                                 0xF018A000
#define APB2SIB_PHYS_BASE_ADDR                                  0xF018C000
#define APB5_PHYS_BASE_ADDR                                     0xF0200000
#define PSPI1_PHYS_BASE_ADDR                                    0xF0200000
#define PSPI2_PHYS_BASE_ADDR                                    0xF0201000
#define VUART1_PHYS_BASE_ADDR                                   0xF0204000
#define VUART2_PHYS_BASE_ADDR                                   0xF0205000
#define JTAG_MASTER_1_PHYS_BASE_ADDR                            0xF0208000
#define JTAG_MASTER_2_PHYS_BASE_ADDR                            0xF0209000

#define AHB19_PHYS_BASE_ADDR                                    0xFFF00000
#define APB19_PHYS_BASE_ADDR                                    0xFFF00000
#define SMB16_PHYS_BASE_ADDR                                    0xFFF00000
#define SMB17_PHYS_BASE_ADDR                                    0xFFF01000
#define SMB18_PHYS_BASE_ADDR                                    0xFFF02000
#define SMB19_PHYS_BASE_ADDR                                    0xFFF03000
#define SMB20_PHYS_BASE_ADDR                                    0xFFF04000
#define SMB21_PHYS_BASE_ADDR                                    0xFFF05000
#define SMB22_PHYS_BASE_ADDR                                    0xFFF06000
#define SMB23_PHYS_BASE_ADDR                                    0xFFF07000
#define SMB24_PHYS_BASE_ADDR                                    0xFFF08000
#define SMB25_PHYS_BASE_ADDR                                    0xFFF09000
#define SMB26_PHYS_BASE_ADDR                                    0xFFF0A000
#define I2CXM0_PHYS_BASE_ADDR                                   0xFFF10000
#define I2CXM1_PHYS_BASE_ADDR                                   0xFFF11000
#define I2CXM2_PHYS_BASE_ADDR                                   0xFFF12000
#define I2CXM3_PHYS_BASE_ADDR                                   0xFFF13000
#define I2CXM4_PHYS_BASE_ADDR                                   0xFFF14000
#define I2CXM5_PHYS_BASE_ADDR                                   0xFFF15000
#define I2CXM6_PHYS_BASE_ADDR                                   0xFFF16000
#define I2CXM7_PHYS_BASE_ADDR                                   0xFFF17000
#define SHI_PHYS_BASE_ADDR                                      0xFFF1F000
#define RAM2_PHYS_BASE_ADDR                                     0xFFFB0000
#define SRAM_PHYS_BASE_ADDR                                     0xFFFEF800

#define ROM1_PHYS_BASE_ADDR_RAMV0                               0xFFFF0000 /*(RAMV=0)*/
#define ROM1_PHYS_BASE_ADDR                                     0xFFFF0100

#define L2_PHYS_BASE_ADDR                                       0xF03FC000
#define CPU_PHYS_BASE_ADDR                                      0xF03FE000

#define GIC_PHYS_BASE_ADDR                                      0xF03FE100


/*---------------------------------------------------------------------------------------------------------*/
/* Legacy from BMC_HAL, will be removed                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define ROM_BASE_ADDR                  0xFFFF0000
#define ROM_MEMORY_SIZE                _64KB_
#define ROMCODE_VER_ADDR               0xFFFF00FC


/*---------------------------------------------------------------------------------------------------------*/
/* External SDRAM Space DDR4                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define SDRAM_BASE_ADDR                 0x00000000
#define SDRAM_MAPPED_SIZE               (_2GB_)

/*---------------------------------------------------------------------------------------------------------*/
/* External Coprocessor (CR16C+) area                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#define COPR_MAPPED_SIZE               (_2MB_)

/*---------------------------------------------------------------------------------------------------------*/
/* XBus                                                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

#define XBUS_MAPPED_SIZE                _32MB_

/*---------------------------------------------------------------------------------------------------------*/
/* Memory Mapped IO                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

#define IOMEMORY_SIZE                   0x0F004000

/*---------------------------------------------------------------------------------------------------------*/
/* OTP                                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/

#define OTPROM_MAPPED_SIZE              _128B_

/*---------------------------------------------------------------------------------------------------------*/
/* On-Chip SRAM     (SRAM2)                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
#define RAM2_BASE_ADDR                  0xFFFB0000
#define RAM2_MEMORY_SIZE                _256KB_

/*---------------------------------------------------------------------------------------------------------*/
/* SRAM2 lockable  (Lockable in 2x 1KB pieces)                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#define SRAM_LOCK_BASE_ADDR             0xFFFEF800      /* SRAM, lockable  */
#define SRAM_LOCK_MAPPED_SIZE           _2KB_

/*---------------------------------------------------------------------------------------------------------*/
/* first 256 byte in RAM2 mirrored to this address. if RAMV = 1, default 0 (ROM)                           */
/*---------------------------------------------------------------------------------------------------------*/
#define SRAM_VECOTR_BASE_ADDR           0xFFFF0000      /* SRAM, mapped to vector area  */
#define SRAM_VECOTR_MAPPED_SIZE         _256B_

#define CONFIG_NR_DRAM_BANKS            2                           // we have 2 banks: Chip SRAM2 (internal) and SDRAM DDR (external)


#ifdef _LINUX_KERNEL // TODO: find a good define for this ( defs)
/*---------------------------------------------------------------------------------------------------------*/
/* Defining IO Memory continuously mapped blocks                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define IOMEMORY_BLOCKS             {                                                   \
                                        /*  Block Base Address | Block Size     */      \
                                        /*  ----------------------------------  */      \
                                        {   0xF0000000,          0x00040000   },        \
                                        {   0xF0400000,          0x00020000   },        \
                                        {   0xF0440000,          0x00040000   },        \
                                        {   0xF1F00000,          0x00002000   },        \
                                        {   0xF1FFE000,          0x00004000   },        \
                                        {   0xF4200000,          0x00300000   },        \
                                        {   0xF8000000,          0x00003000   },        \
                                        {   0xFF000000,          0x00004000   },        \
                                    }
#endif // _LINUX_KERNEL





/*---------------------------------------------------------------------------------------------------------*/
/* KCS Module                                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define KCS_ACCESS                      MEM

/*---------------------------------------------------------------------------------------------------------*/
/* Multi-Function Timer Module                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#define MFT_MODULE_TYPE                 1
#define MFT_ACCESS                      MEM

#define MFT_BASE_ADDR(module)           (MFT0_BASE_ADDR + ((module) * 0x1000L))
#define MFT_INTERRUPT(module)           (MFT_INTERRUPT_1 + module)

#define MFT_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define MFT_INTERRUPT_PRIORITY          0
#define MFT_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#define MFT_NUM_OF_MODULES              2
#define MFT_PRESCALER_CLOCK             PLL2
#define MFT_MUX(module, timer)          CHIP_MuxMFT(module, timer)

/*---------------------------------------------------------------------------------------------------------*/
/* General Ethernet configuration                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define ETH_MODULE_TYPE                 1
#define ETH_NUM_OF_MODULES              2
#define ETH0_TYPE                       EMC
#define ETH1_TYPE                       GMAC

#define ETH_NUM_OF_INPUTS               3
#define ETH_CLK                         2000000
#define ETH_SOURCE_CLOCK                PLL2
#define ETH_MUX(input)                  CHIP_MuxETH(input)

/*---------------------------------------------------------------------------------------------------------*/
/* General Module  : System Global Registers : (Found under chip file)                                     */
/*---------------------------------------------------------------------------------------------------------*/
// #define GCR_MODULE_TYPE                 3
#define GCR_ACCESS                      MEM

#ifndef DYNAMIC_BASE_ADDRESS
#define GCR_BASE_ADDR                   GCR_PHYS_BASE_ADDR
#define GCR_CP_CTRL_BASE_ADDR           GCR_CP_CTRL_PHYS_BASE_ADDR
#define GCR_TIP_CTRL_BASE_ADDR          GCR_TIP_CTRL_PHYS_BASE_ADDR
#else
#define GCR_BASE_ADDR                   GCR_VIRT_BASE_ADDR
#define GCR_CP_CTRL_BASE_ADDR           GCR_CP_CTRL_VIRT_BASE_ADDR
#define GCR_TIP_CTRL_BASE_ADDR          GCR_TIP_CTRL_VIRT_BASE_ADDR
#endif

#define GCR_NUM_OF_INPUTS               3
#define GCR_CLK                         2000000
#define GCR_SOURCE_CLOCK                PLL0
#define GCR_MUX(input)                  CHIP_MuxGCR(input)

/*---------------------------------------------------------------------------------------------------------*/
/* UART Module                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#define UART_MODULE_TYPE                1
#define UART_ACCESS                     MEM
#define UART_PHYS_BASE_ADDR(module)     (UART0_BASE_ADDR + ((module) * 0x1000))
#define UART_INTERRUPT(module)          (UART_INTERRUPT_0 + module)
#define UART_TIP_DEV                    UART7_DEV
#define UART_CP_DEV                     UART8_DEV
#define UART_NUM_OF_MODULES             9   // UART0-6 + UART_TIP
#ifdef NPCM850_TIP
#define UART_BASE_ADDR(module)          ((module) == UART_TIP_DEV ? TIP_UART_BASE_ADDR : UART_PHYS_BASE_ADDR(module))
#else
#define UART_BASE_ADDR(module)          UART_PHYS_BASE_ADDR(module)
#endif
#define UART_INTERRUPT_POLARITY         INTERRUPT_POLARITY_LEVEL_HIGH
#define UART_INTERRUPT_PRIORITY         0
#define UART_INTERRUPT_PROVIDER         CHIP_INTERRUPT_PROVIDER
#define UART_NUM_OF_INPUTS              3
#define UART_CLK                        24000000
#define UART_SOURCE_CLOCK               PLL2
#define UART_MUX(input)                 CHIP_MuxUART(input)

/*---------------------------------------------------------------------------------------------------------*/
/* PCI MailBox Module                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#define PCIMBX_ACCESS                   MEM
#ifndef DYNAMIC_BASE_ADDRESS
#define PCIMBX_BASE_ADDR(module)        (PCIMBX_PHYS_BASE_ADDR + ((module)*0x20000))
#else
#define PCIMBX_BASE_ADDR                (PCIMBX_VIRT_BASE_ADDR + ((module)*0x20000))
#endif
#define PCIMBX_RAM_SIZE                 _16KB_
#define PCIMBX_INTERRUPT_POLARITY       INTERRUPT_POLARITY_LEVEL_HIGH
#define PCIMBX_INTERRUPT_PRIORITY       0
#define PCIMBX_INTERRUPT_PROVIDER       CHIP_INTERRUPT_PROVIDER
#define PCIMBX_NUM_OF_INPUTS            3
#define PCIMBX_CLK                      2000000
#define PCIMBX_SOURCE_CLOCK             PLL2
#define PCIMBX_MUX(input)               CHIP_MuxPCIMBX(input)

/*---------------------------------------------------------------------------------------------------------*/
/* FSW: full system WD (can use TMC driver)                                                                */
/*---------------------------------------------------------------------------------------------------------*/
#define FSW_PHYS_BASE_ADDR              0xF000F000
#define FWS_BASE_ADDR                   (FSW_PHYS_BASE_ADDR)

/*---------------------------------------------------------------------------------------------------------*/
/* (TIM) TIMER CONTROLLER   AKA TMC - timer module                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#define TMC_BASE_ADDR(module)           (0xF0008000 + (0x1000 * module))
#define TMC_MODULE_TYPE                 4
#define TMC_ACCESS                      MEM
#define TMC_NUM_OF_MODULES              3
#define TMC_CLK                         2000000
#define TMC_SOURCE_CLOCK                PLL2
#define TMC_MUX(input)                  CHIP_MuxTIMER(input)



/*---------------------------------------------------------------------------------------------------------*/
/* FIU Module                                                                                              */
/*---------------------------------------------------------------------------------------------------------*/


/*  8000_0000h – 87FF_FFFFh SPI0CS0_BA SPI0 with chip select 0 (128 MB)   */
/*  8800_0000h – 8FFF_FFFFh SPI0CS1_BA SPI0 with chip select 1 (128 MB)   */

/*  9000_0000h – 90FF_FFFFh SPI1CS0_BA SPI1 with chip select 0 (16 MB)    */
/*  9100_0000h – 91FF_FFFFh SPI1CS1_BA SPI1 with chip select 1 (16 MB)    */
/*  9200_0000h – 92FF_FFFFh SPI1CS2_BA SPI1 with chip select 2 (16 MB)    */
/*  9300_0000h – 93FF_FFFFh SPI1CS3_BA SPI1 with chip select 3 (16 MB)    */

/*  A000_0000h – A7FF_FFFFh SPI3CS0_BA SPI3 with chip select 0 (128 MB)   */
/*  A800_0000h – AFFF_FFFFh SPI3CS1_BA SPI3 with chip select 1 (128 MB)   */
/*  B000_0000h – B7FF_FFFFh SPI3CS2_BA SPI3 with chip select 2 (128 MB)   */
/*  B800_0000h – BFFF_FFFFh SPI3CS3_BA SPI3 with chip select 3 (128 MB)   */

#define SPI0CS0_BASE_ADDR                                       0x80000000
#define SPI0CS1_BASE_ADDR                                       0x88000000
#define SPI1CS0_BASE_ADDR                                       0x90000000
#define SPI1CS1_BASE_ADDR                                       0x91000000
#define SPI1CS2_BASE_ADDR                                       0x92000000
#define SPI1CS3_BASE_ADDR                                       0x93000000
#define SPI3CS0_BASE_ADDR                                       0xA0000000
#define SPI3CS1_BASE_ADDR                                       0xA8000000
#define SPI3CS2_BASE_ADDR                                       0xB0000000
#define SPI3CS3_BASE_ADDR                                       0xB8000000
#define SPIXCS0_BASE_ADDR                                       0xF8000000
#define SPIXCS1_BASE_ADDR                                       0xF9000000

#define SPI0CS0_SIZE                                       _128MB_
#define SPI0CS1_SIZE                                       _128MB_
#define SPI1CS0_SIZE                                       _16MB_
#define SPI1CS1_SIZE                                       _16MB_
#define SPI1CS2_SIZE                                       _16MB_
#define SPI1CS3_SIZE                                       _16MB_
#define SPI3CS0_SIZE                                       _128MB_
#define SPI3CS1_SIZE                                       _128MB_
#define SPI3CS2_SIZE                                       _128MB_
#define SPI3CS3_SIZE                                       _128MB_
#define SPIXCS0_SIZE                                       _16MB_
#define SPIXCS1_SIZE                                       _16MB_


//FIU registers:
#define FIU0_BASE_ADDR                                          0xFB000000
#define FIUX_BASE_ADDR                                          0xFB001000
#define FIU1_BASE_ADDR                                          0xFB002000
#define FIU3_BASE_ADDR                                          0xC0000000

#define FIU_MODULE_TYPE                 41
#define FIU_ACCESS                      MEM
#define FIU_PHYS_BASE_ADDR(n)          ( ( (n) == FIU_MODULE_0 ) ? FIU0_BASE_ADDR :    \
                                          (((n) == FIU_MODULE_1 ) ? FIU1_BASE_ADDR :    \
                                          (((n) == FIU_MODULE_2 ) ? FIU3_BASE_ADDR :    \
                                          (((n) == FIU_MODULE_3 ) ? FIU3_BASE_ADDR :    \
                                          FIUX_BASE_ADDR))))


#define FIU_NUM_OF_MODULES              5   /* including SPI-X */

#ifndef DYNAMIC_BASE_ADDRESS
#define FIU_BASE_ADDR(module)           FIU_PHYS_BASE_ADDR(module)
#else
#define FIU_BASE_ADDR(module)           FIU_VIRT_BASE_ADDR(module)
#endif

#define FIU_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define FIU_INTERRUPT_PRIORITY          0
#define FIU_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#define FIU_NUM_OF_INPUTS               3
#define FIU_CLK                         19200000

// Note: the next define is used in the flash_dev to do delay loops.
#ifdef NPCM850_TIP
#define FIU_SOURCE_CLOCK                CLK_GetCPFreq()
#else
#define FIU_SOURCE_CLOCK                CLK_GetCPUFreq()
#endif
#define FIU_MUX(input)                  CHIP_MuxFIU(input)

/*---------------------------------------------------------------------------------------------------------*/
/* SPI Flash                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define FLASH_MEMORY_SIZE(fiu)          _128MB_  /* total address space */

#define FLASH_NUM_OF_MODULES            (FIU_NUM_OF_MODULES)
#define FLASH_NUM_OF_DEVICES             4           /* Number of Chip select per FIU module */



#define SPI0_MEMORY_SIZE                _256MB_
#define SPI1_MEMORY_SIZE                _64MB_
#define SPI3_MEMORY_SIZE                _512MB_


#define FLASH_ADDR_ARRAY(fiu, cs)          \
        FLASH_ADDR_ARRAY_NPCM850[fiu * FLASH_NUM_OF_MODULES + cs]


/*---------------------------------------------------------------------------------------------------------*/
/* SPI Expansion Flash                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/

#define SPIX_MEMORY_SIZE                _16MB_




/*---------------------------------------------------------------------------------------------------------*/
/* Clock Module                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define CLK_MODULE_TYPE                 4
#define CLK_ACCESS                      MEM

#ifndef DYNAMIC_BASE_ADDRESS
#define CLK_BASE_ADDR                   CLK_PHYS_BASE_ADDR
#else
#define CLK_BASE_ADDR                   CLK_VIRT_BASE_ADDR
#endif

#define CLK_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define CLK_INTERRUPT_PRIORITY          0
#define CLK_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#define CLK_NUM_OF_INPUTS               3

#define CLK_SOURCE_CLOCK                PLL0
#define CLK_MUX(input)                  CHIP_MuxCLK(input)

/*---------------------------------------------------------------------------------------------------------*/
/* ARM core registers                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
// #define SCU_PHYS_BASE_ADDR             0xF03FE000    /* F03F_E000h – F03F_E0FFh ARM SCU Registers                        */
// #define G_TIMER_PHYS_BASE_ADDR         0xF03FE200    /* F03F_E200h – F03F_E2FFh ARM Global Timer Registers               */
// #define PTIMER_WD_BASE_ADDR            0xF03FE600    /* F03F_E600h – F03F_E6FFh ARM Private Timer and Watchdog Registers */
// #define INT_DIST_BASE_ADDR             0xF03FF000    /* F03F_F000h – F03F_FFFFh ARM Interrupt Distribution Registers     */

/*---------------------------------------------------------------------------------------------------------*/
/* Generic interrupt manager ( GIC)                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#define GIC_MODULE_TYPE                 2
#define GIC_ACCESS                      MEM

#ifndef DYNAMIC_BASE_ADDRESS
#define GIC_BASE_ADDR                   GIC_PHYS_BASE_ADDR
#else
#define GIC_BASE_ADDR                   GIC_VIRT_BASE_ADDR
#endif

#define GIC_TRAP_NUM                    32
#define GIC_MAX_PRIORITY                256

/*---------------------------------------------------------------------------------------------------------*/
/* Shared Memory (SHM) Module                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define SHM_MODULE_TYPE                 5   /* Should be 11 : to be written. Using 5 in the meanwhile */
#define SHM_ACCESS                      MEM
#ifndef DYNAMIC_BASE_ADDRESS
#define SHM_BASE_ADDR                   SHM_PHYS_BASE_ADDRESS
#else
#define SHM_BASE_ADDR                   SHM_VIRT_BASE_ADDRESS
#endif

#define SHM_RAM_BASE_ADDR               0  /* TODO */
#define SHM_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define SHM_INTERRUPT_PRIORITY          0
#define SHM_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#define SHM_NUM_OF_INPUTS               3
#define SHM_CLK                         2000000
#define SHM_SOURCE_CLOCK                PLL2
#define SHM_MUX(input)                  CHIP_MuxSHM(input)
#define SHM_NUM_OF_WINDOWS              2

/*---------------------------------------------------------------------------------------------------------*/
/* Ethernet 100 Mac Controller    (10/100 Ethernet Controller)                                             */
/*---------------------------------------------------------------------------------------------------------*/
// #define EMC_MODULE_TYPE                 1 /* Driver not updated. Taken from Yarkon. */
#define EMC_ACCESS                      MEM
#define EMC_PHYS_BASE_ADDR(module)      (EMC1_BASE_ADDR + ((module)*0x1000))
#define EMC_RX_INTERRUPT(module)        ((module == 1 ) ? (EMC1RX_INTERRUPT) : (EMC2RX_INTERRUPT))
#define EMC_TX_INTERRUPT(module)        ((module == 1 ) ? (EMC1TX_INTERRUPT) : (EMC2TX_INTERRUPT))
#define EMC_NUM_OF_MODULES              2

#ifndef DYNAMIC_BASE_ADDRESS
#define EMC_BASE_ADDR(module)           EMC_PHYS_BASE_ADDR(module)
#else
#define EMC_BASE_ADDR(module)           EMC_VIRT_BASE_ADDR(module)
#endif
#define EMC_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define EMC_INTERRUPT_PRIORITY          0
#define EMC_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#define EMC_NUM_OF_INPUTS               3
#define EMC_CLK                         2000000
#define EMC_SOURCE_CLOCK                PLL2
#define EMC_MUX(input)                  CHIP_MuxEMC(input)

/*---------------------------------------------------------------------------------------------------------*/
/* GMAC Module                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
// #define GMAC_MODULE_TYPE                4  /* driver from Yarkon. Need to update */
#define GMAC_ACCESS                     MEM
#define GMAC_PHYS_BASE_ADDR(module)     (GMAC1_BASE_ADDR + ((module)*0x2000))
#define GMAC_INTERRUPT(module)          ((module == 1) ? GMAC1_INTERRUPT : GMAC2_INTERRUPT)
#define GMAC_NUM_OF_MODULES             1

#ifndef DYNAMIC_BASE_ADDRESS
#define GMAC_BASE_ADDR(module)          GMAC_PHYS_BASE_ADDR(module)
#else
#define GMAC_BASE_ADDR(module)          GMAC_VIRT_BASE_ADDR(module)
#endif

#define GMAC_INTERRUPT_POLARITY         INTERRUPT_POLARITY_LEVEL_HIGH
#define GMAC_INTERRUPT_PRIORITY         0
#define GMAC_INTERRUPT_PROVIDER         CHIP_INTERRUPT_PROVIDER
#define GMAC_NUM_OF_INPUTS              3
#define GMAC_CLK                        2000000
#define GMAC_SOURCE_CLOCK               PLL2
#define GMAC_MUX(input)                 CHIP_MuxGMAC(input)

/*---------------------------------------------------------------------------------------------------------*/
/* Memory Control                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define MC_MODULE_TYPE                  4
#define MC_ACCESS                       MEM

/* Uniquify MC Phy controller */
#define MCPHY_PHYS_BASE_ADDR            0xF05F0000

/* Cadence memory controller (Denaly) */
#define MC_PHYS_BASE_ADDR               0xF0824000   /* Cadence denaly*/

#ifndef DYNAMIC_BASE_ADDRESS
#define MC_BASE_ADDR                    MC_PHYS_BASE_ADDR
#else
#define MC_BASE_ADDR                    MC_VIRT_BASE_ADDR
#endif

#define MC_INTERRUPT_POLARITY           INTERRUPT_POLARITY_LEVEL_HIGH
#define MC_INTERRUPT_PRIORITY           0
#define MC_INTERRUPT_PROVIDER           CHIP_INTERRUPT_PROVIDER
#define MC_NUM_OF_INPUTS                1
#define MC_CLK                          2000000
#define MC_SOURCE_CLOCK                 PLL1
#define MC_MUX(input)                   CHIP_MuxMC(input)

#define MCPHY_ACCESS                    MEM

#ifndef DYNAMIC_BASE_ADDRESS
#define MCPHY_BASE_ADDR                 MCPHY_PHYS_BASE_ADDR
#else
#define MCPHY_BASE_ADDR                 MCPHY_VIRT_BASE_ADDR
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* SD Module                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define SD_MODULE_TYPE                  10 /* Driver is not updated. Taken from Yarkon */
#define SD_ACCESS                       MEM
#define SD_PHYS_BASE_ADDR(module)       (0xF0840000 + (0x2000 * (module)))
#define SD_GROUP_INTERRUPT(module)      (14 + (module))
#define SD_INTERRUPT(module)            (((module) == 0)? SDRDR_INTERRUPT : MMC_INTERRUPT)
#define SD_NUM_OF_MODULES               2
#define SD_CLK_TARGET_FREQ              (50 * _1MHz_)
#define SD_SD_MODULE                    0
#define SD_MMC_MODULE                   1

#ifndef DYNAMIC_BASE_ADDRESS
#define SD_BASE_ADDR(module)            SD_PHYS_BASE_ADDR(module)
#else
#define SD_BASE_ADDR(module)            SD_VIRT_BASE_ADDR(module)
#endif

#define SD_INTERRUPT_POLARITY           INTERRUPT_POLARITY_LEVEL_HIGH
#define SD_INTERRUPT_PRIORITY           0
#define SD_INTERRUPT_PROVIDER           CHIP_INTERRUPT_PROVIDER
#define SD_NUM_OF_INPUTS                3
#define SD_CLK                          2000000
#define SD_SOURCE_CLOCK                 PLL2
#define SD_MUX(input)                   CHIP_MuxSD(input)

/*---------------------------------------------------------------------------------------------------------*/
/* AES Module. Note: MP has AES engine version 6. TIP has version 7. Those are separate modules            */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef AES_MODULE_TYPE
#define AES_MODULE_TYPE                 6
#define AES_ACCESS                      MEM
#define AES_BASE_ADDR                   AES_PHYS_BASE_ADDR
#define AES_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define AES_INTERRUPT_PRIORITY          0
#define AES_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#define AES_NUM_OF_INPUTS               3
#define AES_CLK                         2000000
#define AES_SOURCE_CLOCK                PLL2
#define AES_NUM_OF_KEYS                 6
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* DES Module                                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define DES_MODULE_TYPE                 0
#define DES_ACCESS                      MEM

#ifndef DYNAMIC_BASE_ADDRESS
#define DES_BASE_ADDR                   DES_PHYS_BASE_ADDR
#else
#define DES_BASE_ADDR                   DES_VIRT_BASE_ADDR
#endif
#define DES_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define DES_INTERRUPT_PRIORITY          0
#define DES_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#define DES_NUM_OF_INPUTS               3
#define DES_CLK                         2000000
#define DES_SOURCE_CLOCK                PLL2
#define DES_MUX(input)                  CHIP_MuxDES(input)

/*---------------------------------------------------------------------------------------------------------*/
/* STRAP Module                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define STRP_MODULE_TYPE                2

#define STRP_INTERRUPT_POLARITY         INTERRUPT_POLARITY_LEVEL_HIGH
#define STRP_INTERRUPT_PRIORITY         0
#define STRP_INTERRUPT_PROVIDER         CHIP_INTERRUPT_PROVIDER
#define STRP_NUM_OF_INPUTS              3
#define STRP_CLK                        2000000
#define STRP_SOURCE_CLOCK               PLL2
#define STRP_MUX(input)                 CHIP_MuxSTRP(input)

/*---------------------------------------------------------------------------------------------------------*/
/* FUSE Module                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#define FUSE_MODULE_TYPE                2
#define FUSE_ACCESS                     MEM
#define FUSE_PHYS_BASE_ADDR             0xF0189000

#ifndef DYNAMIC_BASE_ADDRESS
#define FUSE_BASE_ADDR                  FUSE_PHYS_BASE_ADDR
#else
#define FUSE_BASE_ADDR                  FUSE_VIRT_BASE_ADDR
#endif

#define FUSE_INTERRUPT_POLARITY         INTERRUPT_POLARITY_LEVEL_HIGH
#define FUSE_INTERRUPT_PRIORITY         0
#define FUSE_INTERRUPT_PROVIDER         CHIP_INTERRUPT_PROVIDER
#define FUSE_NUM_OF_INPUTS              3
#define FUSE_CLK                        2000000
#define FUSE_SOURCE_CLOCK               PLL2
#define FUSE_MUX(input)                 CHIP_MuxFUSE(input)

/*---------------------------------------------------------------------------------------------------------*/
/* VCD Module  (VIDEO CAPTURE AND DIFFERENTIATION)  : Driver from Yarkon. Need to update. Registers are    */
/*                                                    already updated                                      */
/*---------------------------------------------------------------------------------------------------------*/
// #define VCD_MODULE_TYPE                 3
#define VCD_ACCESS                      MEM
#define VCD_INTERRUPT                   VCD_INTERRUPT
#define VCD_GROUP_INTERRUPT             12

#define VCD_MEM_PORT1                   3
#define VCD_MEM_PORT2                   4

#define VCD_MAX_WIDTH                   2047
#define VCD_MAX_HIGHT                   1536
#define USE_INTERNAL_GFX

#define VCD_FRAME_A_PHYS_BASE_ADDRESS   0x6C00000

#ifndef DYNAMIC_BASE_ADDRESS
#define VCD_BASE_ADDR                   VCD_PHYS_BASE_ADDR
#else
#define VCD_BASE_ADDR                   VCD_VIRT_BASE_ADDR
#endif

#define VCD_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define VCD_INTERRUPT_PRIORITY          0
#define VCD_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#define VCD_NUM_OF_INPUTS               3
#define VCD_CLK                         2000000
#define VCD_SOURCE_CLOCK                PLL2
#define VCD_MUX(input)                  CHIP_MuxVCD(input)

/*---------------------------------------------------------------------------------------------------------*/
/* GFX Module                                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define GFX_MODULE_TYPE                 3
#define GFX_ACCESS                      MEM

//#define GFXI_BASE_ADDR                  0xF000E000      /* Graphics Core information registers  */
//#define GFXOL_BASE_ADDR                 0xF0828000      /* GFX Overlay FIFO and registers  */

//#define GFX_PHYS_BASE_ADDR              GFXI_BASE_ADDR

#ifndef DYNAMIC_BASE_ADDRESS
#define GFX_BASE_ADDR                   GFX_PHYS_BASE_ADDR
#else
#define GFX_BASE_ADDR                   GFX_VIRT_BASE_ADDR
#endif
#define GFX_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define GFX_INTERRUPT_PRIORITY          0
#define GFX_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#define GFX_NUM_OF_INPUTS               3
#define GFX_CLK                         2000000
#define GFX_SOURCE_CLOCK                PLL_GFX
#define GFX_MUX(input)                  CHIP_MuxGFX(input)

/*---------------------------------------------------------------------------------------------------------*/
/* ECE Module:  Video Compression map ECE                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#define ECE_MODULE_TYPE                 2   /* Poleg: same as Yarkon */
#define ECE_ACCESS                      MEM

#define ECE_INTERRUPT                   ECE_INTERRUPT
#define ECE_GROUP_INTERRUPT             11
#define ECE_ED_PHYS_BASE_ADDRESS        0x6800000
#ifndef DYNAMIC_BASE_ADDRESS
#define ECE_BASE_ADDR                   ECE_PHYS_BASE_ADDR
#else
#define ECE_BASE_ADDR                   ECE_VIRT_BASE_ADDR
#endif

#define ECE_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define ECE_INTERRUPT_PRIORITY          0
#define ECE_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#define ECE_NUM_OF_INPUTS               3
#define ECE_CLK                         2000000
#define ECE_SOURCE_CLOCK                PLL2
#define ECE_MUX(input)                  CHIP_MuxECE(input)

/*---------------------------------------------------------------------------------------------------------*/
/* GPIO Module                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#define GPIO_MODULE_TYPE                40
#define GPIO_ACCESS                     MEM

#define GPIO_PHYS_BASE_ADDR(module)     (GPIO0_BASE_ADDR + ((module)*0x1000))
#define GPIO_INTERRUPT(gpio)            ((gpio) < 32  ? GPIO_INTERRUPT0 : (gpio) < 64  ? GPIO_INTERRUPT1 : (gpio) < 96  ? GPIO_INTERRUPT2 :   \
                                         (gpio) < 128 ? GPIO_INTERRUPT3 : (gpio) < 160 ? GPIO_INTERRUPT4 : (gpio) < 192 ? GPIO_INTERRUPT5 :  \
                                         (gpio) < 224 ? GPIO_INTERRUPT6 :  GPIO_INTERRUPT7)
#define GPIO_NUM_OF_MODULES             8
#define GPIO_NUM_OF_PORTS               8
#define GPIO_NUM_OF_GPIOS               256

#ifndef DYNAMIC_BASE_ADDRESS
#define GPIO_BASE_ADDR(module)          GPIO_PHYS_BASE_ADDR(module)
#else
#define GPIO_BASE_ADDR(module)          GPIO_VIRT_BASE_ADDR(module)
#endif

#define GPIO_INTERRUPT_POLARITY         INTERRUPT_POLARITY_LEVEL_HIGH
#define GPIO_INTERRUPT_PRIORITY         0
#define GPIO_INTERRUPT_PROVIDER         CHIP_INTERRUPT_PROVIDER
#define GPIO_NUM_OF_INPUTS              3
#define GPIO_CLK                        2000000
#define GPIO_SOURCE_CLOCK               PLL2
#define GPIO_MUX(gpio)                  CHIP_Mux_GPIO(gpio)
#define GPIO_MUX_PORT(port)

/*---------------------------------------------------------------------------------------------------------*/
/* SPI Module (aka PSPI)                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define SPI_MODULE_TYPE                 1
#define SPI_ACCESS                      MEM
#define SPI_PHYS_BASE_ADDR(module)      (SPI1_BASE_ADDR + ((module) * 0x1000))
#define SPI_NUM_OF_MODULES              2

#ifndef DYNAMIC_BASE_ADDRESS
#define SPI_BASE_ADDR(module)           SPI_PHYS_BASE_ADDR(module)
#else
#define SPI_BASE_ADDR(module)           SPI_VIRT_BASE_ADDR(module)
#endif

#define SPI_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define SPI_INTERRUPT_PRIORITY          0
#define SPI_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#define SPI_NUM_OF_INPUTS               3
#define SPI_CLK                         2000000
#define SPI_SOURCE_CLOCK                PLL2
#define SPI_MUX(input)                  CHIP_MuxSPI(input)

/*---------------------------------------------------------------------------------------------------------*/
/* eSPI Module                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#define ESPI_MODULE_TYPE                3   /* Z1: 1, Z2: 3 */
#define ESPI_ACCESS                     MEM

#ifndef DYNAMIC_BASE_ADDRESS
#define ESPI_BASE_ADDR                  ESPI_PHYS_BASE_ADDR
#else
#define ESPI_BASE_ADDR                  ESPI_VIRT_BASE_ADDR
#endif

#define ESPI_INTERRUPT_POLARITY         INTERRUPT_POLARITY_LEVEL_HIGH
#define ESPI_INTERRUPT_PRIORITY         0
#define ESPI_INTERRUPT_PROVIDER         CHIP_INTERRUPT_PROVIDER
#define ESPI_MUX(input)                 CHIP_MuxESPI(input)
#define ESPI_FLASH_GDMA_CHANNEL         0
#define ESPI_PC_BM_GDMA_CHANNEL         1
#define ESPI_GDMA_MODULE                0

/*---------------------------------------------------------------------------------------------------------*/
/* AHB2 SRAM Module                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#define SRAM2_ACCESS                    MEM
#define SRAM2_PHYS_BASE_ADDR            RAM2_BASE_ADDR
#define SRAM2_MEMORY_SIZE               RAM2_MEMORY_SIZE

#ifndef DYNAMIC_BASE_ADDRESS
#define SRAM2_BASE_ADDR                 SRAM2_PHYS_BASE_ADDR
#else
#define SRAM2_BASE_ADDR                 SRAM2_VIRT_BASE_ADDR
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* AHB7 SRAM Module                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/* RAM3                                                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define RAM3_MEMORY_SIZE               _16KB_
#define RAM3_ACCESS                    MEM

#ifndef DYNAMIC_BASE_ADDRESS
#define RAM3_BASE_ADDR                 RAM3_PHYS_BASE_ADDR
#else
#define RAM3_BASE_ADDR                 RAM3_VIRT_BASE_ADDR
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* USB Module                                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define USB_MODULE_TYPE                 0xE4A1
#define USB_ACCESS                      MEM
#define USB_PHYS_BASE_ADDR(module)      (USB0_BASE_ADDR + ((module) * 0x1000))
// #define USB_INTERRUPT(module)           (((module == 1) || (module == 4)) ? USBD14_INT : ((module == 2) || (module == 5)) ? USBD25_INT : USBD36_INT)
#define USB_IS_FULL_SPEED(module)       ((module) == 1 ? 1 : 0)
#define USB_DESC_PHYS_BASE_ADDR(module) ((module % 2) ? SRAM2_PHYS_BASE_ADDR + 0x800 * (module / 2) : SRAM3_PHYS_BASE_ADDR + 0x800 * ((module-1) / 2))
#define USB_DESC_VIRT_BASE_ADDR(module) ((module % 2) ? SRAM2_BASE_ADDR + 0x800 * (module / 2) : SRAM3_BASE_ADDR + 0x800 * ((module-1) / 2))
#define USB_NUM_OF_MODULES              10

#ifndef DYNAMIC_BASE_ADDRESS
#define USB_BASE_ADDR(module)           USB_PHYS_BASE_ADDR(module)
#else
#define USB_BASE_ADDR(module)           USB_VIRT_BASE_ADDR(module)
#endif
#define USB_INTERRUPT(module)           (USB_DEV_INTERRUPT_0 + module)
#define USB_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define USB_INTERRUPT_PRIORITY          0
#define USB_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#define USB_NUM_OF_INPUTS               10
#define USB_CLK                         2000000
#define USB_SOURCE_CLOCK                PLL2
#define USB_MUX(input)                  CHIP_MuxUSB(input)

/*---------------------------------------------------------------------------------------------------------*/
/* ADC Module   : Driver not ready. Version will be 20                                                     */
/*---------------------------------------------------------------------------------------------------------*/
// #define ADC_MODULE_TYPE                 20
#define ADC_ACCESS                      MEM
#define ADC_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define ADC_INTERRUPT_PRIORITY          0
#define ADC_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#define ADC_NUM_OF_INPUTS               3
#define ADC_CLK                         25000000
#define ADC_SOURCE_CLOCK                PLL2
#define ADC_MUX(input)                  CHIP_MuxADC(input)

/*---------------------------------------------------------------------------------------------------------*/
/* System Management Bus Module                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define SMB_MODULE_TYPE                 7



#define SMB_ACCESS                      MEM
#define SMB_BASE_ADDRRES(module)        ((module < 16) ? SMB0_BASE_ADDR : SMB16_PHYS_BASE_ADDR)
#define SMB_PHYS_BASE_ADDR(module)      (SMB_BASE_ADDRRES(module) + ((module % 16) * 0x1000L))

// #define SMB_SCL_GPIOS                  114, 116, 118, 31, 29, 27, 171, 173, 128, 130, 132, 134, 220, 222, 23, 21   /* todo add 12 to 16 */

#ifndef DYNAMIC_BASE_ADDRESS
#define SMB_BASE_ADDR(module)          SMB_PHYS_BASE_ADDR(module)
#else
#define SMB_BASE_ADDR(module)          SMB_VIRT_BASE_ADDR(module)
#endif

#ifndef DYNAMIC_BASE_ADDRESS  /* BMC handles the CP interrupts */
#define SMB_INTERRUPT(module)           (SMB_INTERRUPT_0 + module)
#define SMB_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define SMB_INTERRUPT_PRIORITY          0
#define SMB_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#else  /* use CP to handle the SMB */

#define SMB_INTERRUPT(module)           SMB_INT[module]
#define SMB_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define SMB_INTERRUPT_PRIORITY          0
#define SMB_INTERRUPT_PROVIDER          INTERRUPT_PROVIDER_NVIC
#endif
#define SMB_NUM_OF_SEGMENT              4
#define SMB_NUM_OF_MODULES              27

#define SMB_SOURCE_CLOCK(module)        (CLK_GetAPBFreq(CLK_APB2))

#define SMB_MUX(module)                 CHIP_MuxSMB(module, TRUE)
#define SMB_NUM_OF_ADDR                 10
#define SMB_FIFO(module)                TRUE   /* All modules support FIFO */
#define SMB_CAPABILITY_32B_FIFO
#define SMB_CAPABILITY_FORCE_SCL_SDA

// override issue #614:  TITLE :CP_FW: SMBus may fail to supply stop condition in Master Write operation
#define SMB_SW_BYPASS_HW_ISSUE_SMB_STOP

// if end device reads more data than avalilable, ask issuer or request for more data.
#define SMB_WRAP_AROUND_BUFFER

/*---------------------------------------------------------------------------------------------------------*/
/* Pulse-Width Modulator Module                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define PWM_MODULE_TYPE                 1  /* Should be 10 : to be written. Using 1 in the meanwhile */
#define PWM_ACCESS                      MEM

#define PWM_BASE_ADDR(module)           (PWM0_BASE_ADDR + ((module) * 0x1000L))
#define PWM_NUM_OF_MODULES              2
#define PWM_SOURCE_CLOCK(source)        CLK_APB3
#define PWM_MUX(module)                 CHIP_MuxPWM(module)

#define PWM_SOURCE_CLOCK_CORE           PWM_SOURCE_CLOCK_1
#define PWM_SOURCE_CLOCK_32K            PWM_SOURCE_CLOCK_2

/*---------------------------------------------------------------------------------------------------------*/
/* L2 cache Module                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#define L2_CACHE_PHYS_BASE_ADDR         A9_BASE_ADDR

#ifndef DYNAMIC_BASE_ADDRESS
#define L2_CACHE_BASE_ADDR              L2_CACHE_PHYS_BASE_ADDR
#else
#define L2_CACHE_BASE_ADDR              L2_CACHE_VIRT_BASE_ADDR
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Public Key Accelerator Module                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define PKA_MODULE_TYPE                 86
#define PKA_ACCESS                      MEM
#define PKA_BASE_ADDR                   PKA_PHYS_BASE_ADDR
#define PKA_INTERRUPT_NUM               PKA_INTERRUPT
#define PKA_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define PKA_INTERRUPT_PRIORITY          0
#define PKA_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#define PKA_NUM_OF_MODULES              1
#define PKA_SOURCE_CLOCK                PLL2
#define PKA_MUX                         CHIP_MuxPKA(0)
#define PKA_POWER_DOWN(power)

/*---------------------------------------------------------------------------------------------------------*/
/* SHA-1 and SHA-256 Module                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
#define SHA_MODULE_TYPE                 3
#define SHA_ACCESS                      MEM
#define SHA_BASE_ADDR                   SHA_PHYS_BASE_ADDR
//#define SHA_INTERRUPT(module)          (SHA_INTERRUPT)
#define SHA_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define SHA_INTERRUPT_PRIORITY          0
#define SHA_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#define SHA_NUM_OF_MODULES              1
#define SHA_SOURCE_CLOCK(module)        PLL2
#define SHA_MUX(module)                 CHIP_MuxSHA(module)
#define SHA_POWER_DOWN(power)

/*---------------------------------------------------------------------------------------------------------*/
/* General DMA Controller Module                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define GDMA_MODULE_TYPE                3
#define GDMA_BASE_ADDR(module)          (GDMA0_PHYS_BASE_ADDR + ((module) * 0x01000L))
#define GDMA_ACCESS                     MEM
#define GDMA_INTERRUPT(module)          (GDMA_INTERRUPT_0 + (module))
#define GDMA_INTERRUPT_MODULE(int_num)  ((int_num) - GDMA_INTERRUPT_0)
#define GDMA_INTERRUPT_POLARITY         INTERRUPT_POLARITY_RISING_EDGE
#define GDMA_INTERRUPT_PROVIDER         INTERRUPT_PROVIDER_NVIC
#define GDMA_INTERRUPT_PRIORITY         0
#define GDMA_NUM_OF_CHANNELS            2
#define GDMA_NUM_OF_MODULES             3

/*---------------------------------------------------------------------------------------------------------*/
/* RNG                                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#define RNG_MODULE_TYPE                 3
#define RNG_ACCESS                      MEM
#define RNG_BASE_ADDR(n)                RNG_PHYS_BASE_ADDR
#define RNG_INTERRUPT_POLARITY          INTERRUPT_POLARITY_LEVEL_HIGH
#define RNG_INTERRUPT_PROVIDER          CHIP_INTERRUPT_PROVIDER
#define RNG_INTERRUPT_PRIORITY          0
#define RNG_POWER_DOWN(power)
#define RNG_NUM_OF_RING_OSCILLATORS     22
#define RNG_MAX_SAMPLING_RATE           2500
#define RNG_NUM_OF_MODULES              1
#define RNG_DEFAULT_MODE                RNG_MODE_M4
#define RNGROEN_DEFAULT_0               0x3F
#define RNGROEN_DEFAULT_1               0x06
#define RNGROEN_DEFAULT_2               0x00
#define RNG_1BIT_PER_RNGD
#define RNG_ENTROPY_RATE                6     //0.6, normalized


/* Notice: RNG does not have an intterupt. current version of RNG driver works only with interrupts.
   Before using the driver need to change it to do polling too */

#define RNG_PRESCALER_CLOCK             CLK_GetAPBFreq(CLK_APB1)

/*---------------------------------------------------------------------------------------------------------*/
/* Interrupts                                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum GIC_INT_SRC_T_tag
{
    ADC_INTERRUPT             = 0/*+32*/,      /* ADC Module                                         */
    COPR_INTERRUPT            = 1/*+32*/,      /* Coprocessor subsystem                              */
    UART_INTERRUPT_0          = 2/*+32*/,      /* UART 0 Module                                      */
    UART_INTERRUPT_1          = 3/*+32*/,      /* UART 1 Module                                      */
    UART_INTERRUPT_2          = 4/*+32*/,      /* UART 2 Module                                      */
    UART_INTERRUPT_3          = 5/*+32*/,      /* UART 3 Module                                      */
    PECI_INTERRUPT            = 6/*+32*/,      /* PECI module                                        */
    VSYNC_INTERRUPT           = 7/*+32*/,      /* Graphics module via System Manager module          */
    PCIMBX_INTERRUPT          = 8/*+32*/,      /* PCI mailbox module                                 */
    KCS_HIB_INTERRUPT         = 9/*+32*/,      /* KCS/HIB  (from host interface) modules             */
    LPC_MBX_INTERRUPT         = 10/*+32*/,     /* LPC or eSPI Mailbox (new, if we do)                */
    SHM_INTERRUPT             = 11/*+32*/,     /* SHM module                                         */
    PS2_INTERRUPT             = 12/*+32*/,     /* PS/2 Module ???                                    */
    BT_INTERRUPT              = 13/*+32*/,     /* Block transfer, If we do                           */
    GMAC1_INTERRUPT           = 14/*+32*/,     /* GMAC1 module                                       */
    EMC1RX_INTERRUPT          = 15/*+32*/,     /* EMC1 Rx module                                     */
    EMC1TX_INTERRUPT          = 16/*+32*/,     /* EMC1 Tx module                                     */
    GMAC2_INTERRUPT           = 17/*+32*/,     /* GMAC2 module                                       */
    ESPI_INTERRUPT            = 18/*+32*/,     /* eSPI Module                                        */
    SIOX_INTERRUPT_1          = 19/*+32*/,     /* SIOX Serial GPIO Expander module 1                 */
    SIOX_INTERRUPT_2          = 20/*+32*/,     /* SIOX Serial GPIO Expander module 2                 */
    L2_CACHE_ERR              = 21/*+32*/,     /* A combined interrupt on L2 Cahche errors           */
    VCD_INTERRUPT             = 22/*+32*/,     /* VCD module                                         */
    DVC_INTERRUPT             = 23/*+32*/,     /* DVC module                                         */
    ECE_INTERRUPT             = 24/*+32*/,     /* ECE module                                         */
    MC_INTERRUPT              = 25/*+32*/,     /* Memory Controller Interrupt                        */
    MMC_INTERRUPT             = 26/*+32*/,     /* MMC Module (SDHC2)                                 */
    SDRDR_INTERRUPT           = 27/*+32*/,     /* SDHC1 - SD Card Reader side interface (if required)*/
    PSPI2_INTERRUPT           = 28/*+32*/,     /* Slow Peripheral SPI2                               */
    VDMA_INTERRUPT            = 29/*+32*/,     /* VDMA module                                        */
    MCTP_INTERRUPT            = 30/*+32*/,     /* VDMX module. Not used if VDMA used                 */
    PSPI1_INTERRUPT           = 31/*+32*/,     /* Slow Peripheral SPI1                               */
    TMC_INTERRUPT_0           = 32/*+32*/,     /* Timer Module 0 Timer 0                             */
    TMC_INTERRUPT_1           = 33/*+32*/,     /* Timer Module 0 Timer 1                             */
    TMC_INTERRUPT_2           = 34/*+32*/,     /* Timer Module 0 Timer 2                             */
    TMC_INTERRUPT_3           = 35/*+32*/,     /* Timer Module 0 Timer 3                             */
    TMC_INTERRUPT_4           = 36/*+32*/,     /* Timer Module 0 Timer 4                             */
    TMC_INTERRUPT_5           = 37/*+32*/,     /* Timer Module 1 Timer 0                             */
    TMC_INTERRUPT_6           = 38/*+32*/,     /* Timer Module 1 Timer 1                             */
    TMC_INTERRUPT_7           = 39/*+32*/,     /* Timer Module 1 Timer 2                             */
    TMC_INTERRUPT_8           = 40/*+32*/,     /* Timer Module 1 Timer 3                             */
    TMC_INTERRUPT_9           = 41/*+32*/,     /* Timer Module 1 Timer 4                             */
    TMC_INTERRUPT_10          = 42/*+32*/,     /* Timer Module 2 Timer 0                             */
    TMC_INTERRUPT_11          = 43/*+32*/,     /* Timer Module 2 Timer 1                             */
    TMC_INTERRUPT_12          = 44/*+32*/,     /* Timer Module 2 Timer 2                             */
    TMC_INTERRUPT_13          = 45/*+32*/,     /* Timer Module 2 Timer 3                             */
    TMC_INTERRUPT_14          = 46/*+32*/,     /* Timer Module 2 Timer 4                             */
    WDG_INTERRUPT0            = 47/*+32*/,     /* Timer Module 0 watchdog (also on NMI)              */
    WDG_INTERRUPT1            = 48/*+32*/,     /* Timer Module 1 watchdog (also on NMI)              */
    WDG_INTERRUPT2            = 49/*+32*/,     /* Timer Module 2 watchdog (also on NMI)              */
    USB_DEV_INTERRUPT_0       = 51/*+32*/,     /* USB Device 0                                       */
    USB_DEV_INTERRUPT_1       = 52/*+32*/,     /* USB Device 1                                       */
    USB_DEV_INTERRUPT_2       = 53/*+32*/,     /* USB Device 2                                       */
    USB_DEV_INTERRUPT_3       = 54/*+32*/,     /* USB Device 3                                       */
    USB_DEV_INTERRUPT_4       = 55/*+32*/,     /* USB Device 4                                       */
    USB_DEV_INTERRUPT_5       = 56/*+32*/,     /* USB Device 5                                       */
    USB_DEV_INTERRUPT_6       = 57/*+32*/,     /* USB Device 6                                       */
    USB_DEV_INTERRUPT_7       = 58/*+32*/,     /* USB Device 7                                       */
    USB_DEV_INTERRUPT_8       = 59/*+32*/,     /* USB Device 8                                       */
    USB_DEV_INTERRUPT_9       = 60/*+32*/,     /* USB Device 9                                       */
    USB_HST_INTERRUPT_0       = 61/*+32*/,     /* USB Host 0                                         */
    USB_HST_INTERRUPT_1       = 62/*+32*/,     /* USB Host 1                                         */

    SMB_INTERRUPT_0           = 64/*+32*/,     /* SMBus and I2C Module 0                             */
    SMB_INTERRUPT_1           = 65/*+32*/,     /* SMBus and I2C Module 1                             */
    SMB_INTERRUPT_2           = 66/*+32*/,     /* SMBus and I2C Module 2                             */
    SMB_INTERRUPT_3           = 67/*+32*/,     /* SMBus and I2C Module 3                             */
    SMB_INTERRUPT_4           = 68/*+32*/,     /* SMBus and I2C Module 4                             */
    SMB_INTERRUPT_5           = 69/*+32*/,     /* SMBus and I2C Module 5                             */
    SMB_INTERRUPT_6           = 70/*+32*/,     /* SMBus and I2C Module 6                             */
    SMB_INTERRUPT_7           = 71/*+32*/,     /* SMBus and I2C Module 7                             */
    SMB_INTERRUPT_8           = 72/*+32*/,     /* SMBus and I2C Module 8                             */
    SMB_INTERRUPT_9           = 73/*+32*/,     /* SMBus and I2C Module 9                             */
    SMB_INTERRUPT_10          = 74/*+32*/,     /* SMBus and I2C Module 10                            */
    SMB_INTERRUPT_11          = 75/*+32*/,     /* SMBus and I2C Module 11                            */
    SMB_INTERRUPT_12          = 76/*+32*/,     /* SMBus and I2C Module 12                            */
    SMB_INTERRUPT_13          = 77/*+32*/,     /* SMBus and I2C Module 13                            */
    SMB_INTERRUPT_14          = 78/*+32*/,     /* SMBus and I2C Module 14                            */
    SMB_INTERRUPT_15          = 79/*+32*/,     /* SMBus and I2C Module 15                            */

    AES_INTERRUPT             = 80/*+32*/,     /* AES Module                                         */
    DES_INTERRUPT             = 81/*+32*/,     /* 3DES Module                                        */
    SHA_INTERRUPT             = 82/*+32*/,     /* SHA module                                         */
    PKA_INTERRUPT             = 83/*+32*/,     /* (SECACC) ECC and RSA accelerator module            */
    RNG_INTERRUPT             = 84/*+32*/,     /* RNG module                                         */

    SPI_INTERRUPT_0           = 85/*+32*/,     /* FIU module 0 if required                           */

    SPI_INTERRUPT_3           = 87/*+32*/,     /* FIU module 3 if required                           */
    GDMA_INTERRUPT_0          = 88/*+32*/,     /* GDMA Module 0                                      */
    GDMA_INTERRUPT_1          = 89/*+32*/,     /* GDMA Module 1                                      */
    GDMA_INTERRUPT_2          = 90/*+32*/,     /* GDMA Module 2                                      */
    GDMA_INTERRUPT_3          = 91/*+32*/,     /* GDMA Module 3 If required                          */
    OTP_INTERRUPT             = 92/*+32*/,     /* Fustraps and Key arrays                            */
    PWM_INTERRUPT_0_PWM0_3    = 93/*+32*/,     /* PWM Module 0 outputing PWM0-3                      */
    PWM_INTERRUPT_1_PWM4_7    = 94/*+32*/,     /* PWM Module 1 outputing PWM4-7                      */

    MFT_INTERRUPT_0           = 96/*+32*/,     /* MFT Module 0                                       */
    MFT_INTERRUPT_1           = 97/*+32*/,     /* MFT Module 1                                       */
    MFT_INTERRUPT_2           = 98/*+32*/,     /* MFT Module 2                                       */
    MFT_INTERRUPT_3           = 99/*+32*/,     /* MFT Module 3                                       */
    MFT_INTERRUPT_4           = 100/*+32*/,    /* MFT Module 4                                       */
    MFT_INTERRUPT_5           = 101/*+32*/,    /* MFT Module 5                                       */
    MFT_INTERRUPT_6           = 102/*+32*/,    /* MFT Module 6                                       */
    MFT_INTERRUPT_7           = 103/*+32*/,    /* MFT Module 7                                       */
    PWM_INTERRUPT_0           = 104/*+32*/,    /* PWM module 0                                       */
    PWM_INTERRUPT_1           = 105/*+32*/,    /* PWM module 1                                       */
    PWM_INTERRUPT_2           = 106/*+32*/,    /* PWM module 2                                       */
    PWM_INTERRUPT_3           = 107/*+32*/,    /* PWM module 3                                       */
    PWM_INTERRUPT_4           = 108/*+32*/,    /* PWM module 4                                       */
    PWM_INTERRUPT_5           = 109/*+32*/,    /* PWM module 5                                       */
    PWM_INTERRUPT_6           = 110/*+32*/,    /* PWM module 6                                       */
    PWM_INTERRUPT_7           = 111/*+32*/,    /* PWM module 7                                       */

    EMC2RX_INTERRUPT          = 114/*+32*/,    /* EMC2 Rx module                                     */
    EMC2TX_INTERRUPT          = 115/*+32*/,    /* EMC2 Tx module                                     */
    GPIO_INTERRUPT0           = 116/*+32*/,    /* GPIO module outputing GPIO0-31                     */
    GPIO_INTERRUPT1           = 117/*+32*/,    /* GPIO module outputing GPIO32-63                    */
    GPIO_INTERRUPT2           = 118/*+32*/,    /* GPIO module outputing GPIO64-95                    */
    GPIO_INTERRUPT3           = 119/*+32*/,    /* GPIO module outputing GPIO96-127                   */
    GPIO_INTERRUPT4           = 120/*+32*/,    /* GPIO module outputing GPIO128-159                  */
    GPIO_INTERRUPT5           = 121/*+32*/,    /* GPIO module outputing GPIO160-191                  */
    GPIO_INTERRUPT6           = 122/*+32*/,    /* GPIO module outputing GPIO192-223                  */
    GPIO_INTERRUPT7           = 123/*+32*/,    /* GPIO module outputing GPIO224-255                  */

    PCIe_RC_INTERRUPT         = 127/*+32*/,    /* PCIe Root Complex  (combined A-D and MSI)          */

    GIC_INTERRUPT_NUM

} GIC_INT_SRC_T;

/*---------------------------------------------------------------------------------------------------------*/
/* TIMER Allocation Table                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#define TIMER_DEFINITION_TABLE                                                              \
{                                                                                           \
    { TIMER_HW_TMC,   TMC_MODULE_0  },          /* TIMER_CHANNEL_1  */                  \
    { TIMER_HW_TMC,   TMC_MODULE_1  },          /* TIMER_CHANNEL_2  */                  \
    { TIMER_HW_TMC,   TMC_MODULE_2  },          /* TIMER_CHANNEL_3  */                  \
}

#define TIMER_DELAY_CHANNEL  TIMER_CHANNEL_3

/*---------------------------------------------------------------------------------------------------------*/
/* GPIOs definition table                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#define GPIO_DEFINITION_TABLE                                                                                                                                                                       \
{                                                                                                                                                                                                   \
    /* Port 0: GPIO 0-15 */                                                                                                                                                                         \
    GPIO_TABLE_ENTRY( 0, 0, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(0),  GPIO_CAP_DEBOUNCE(0),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 30, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 0, 1, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(1),  GPIO_CAP_DEBOUNCE(1),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 30, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 0, 2, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(2),  GPIO_CAP_DEBOUNCE(2),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 30, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 0, 3, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(3),  GPIO_CAP_DEBOUNCE(3),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 30, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 0, 4, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(4),  GPIO_CAP_DEBOUNCE(4),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 14, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 0, 5, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(5),  GPIO_CAP_DEBOUNCE(5),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 14, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 0, 6, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(6),  GPIO_CAP_DEBOUNCE(6),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 14, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 0, 7, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(7),  GPIO_CAP_DEBOUNCE(7),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 14, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 0, 8, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(8),  GPIO_CAP_DEBOUNCE(8),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 0, 9, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(9),  GPIO_CAP_DEBOUNCE(9),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 0,10, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(10), GPIO_CAP_DEBOUNCE(10), GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 18, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 0,11, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(11), GPIO_CAP_DEBOUNCE(11), GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 18, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 0,12, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(12), GPIO_CAP_DEBOUNCE(12), GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 24, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 0,13, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(13), GPIO_CAP_DEBOUNCE(13), GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 24, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 0,14, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(14), GPIO_CAP_DEBOUNCE(14), GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 24, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 0,15, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(15), GPIO_CAP_DEBOUNCE(15), GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 24, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
                                                                                                                                                                                                    \
    /* Port 1: GPIO 16-31 */                                                                                                                                                                        \
    GPIO_TABLE_ENTRY( 1, 0, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_BLINK(0),  GPIO_MUX_REGID_MFSL3, 15, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 1, 1, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_BLINK(4),  GPIO_MUX_REGID_MFSL3, 13, 1, 0,  GPIO_MUX_REGID_MFSL3, 15, 1, 0), \
    GPIO_TABLE_ENTRY( 1, 2, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_BLINK(8),  GPIO_MUX_REGID_MFSL3, 13, 1, 0,  GPIO_MUX_REGID_MFSL3, 15, 1, 0), \
    GPIO_TABLE_ENTRY( 1, 3, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_BLINK(12), GPIO_MUX_REGID_MFSL3, 13, 1, 0,  GPIO_MUX_REGID_MFSL3, 15, 1, 0), \
    GPIO_TABLE_ENTRY( 1, 4, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_BLINK(16), GPIO_MUX_REGID_MFSL2, 24, 1, 0,  GPIO_MUX_REGID_MFSL3, 15, 1, 0), \
    GPIO_TABLE_ENTRY( 1, 5, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_BLINK(20), GPIO_MUX_REGID_MFSL2, 25, 1, 0,  GPIO_MUX_REGID_MFSL3, 15, 1, 0), \
    GPIO_TABLE_ENTRY( 1, 6, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_BLINK(24), GPIO_MUX_REGID_MFSL2, 26, 1, 0,  GPIO_MUX_REGID_MFSL3, 15, 1, 0), \
    GPIO_TABLE_ENTRY( 1, 7, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_BLINK(28), GPIO_MUX_REGID_MFSL2, 27, 1, 0,  GPIO_MUX_REGID_MFSL3, 15, 1, 0), \
    GPIO_TABLE_ENTRY( 1, 8, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(16), GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2, 28, 1, 0,  GPIO_MUX_REGID_MFSL3, 18, 1, 0), \
    GPIO_TABLE_ENTRY( 1, 9, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(17), GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2, 29, 1, 0,  GPIO_MUX_REGID_MFSL3, 18, 1, 0), \
    GPIO_TABLE_ENTRY( 1,10, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  2, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 1,11, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  2, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 1,12, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  1, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 1,13, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  1, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 1,14, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  0, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 1,15, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  0, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
                                                                                                                                                                                                    \
    /* Port 2: GPIO 32-47 */                                                                                                                                                                        \
    GPIO_TABLE_ENTRY( 2, 0, GPIO_CAP_NO_INPUT, GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  3, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 2, 1, GPIO_CAP_NO_INPUT, GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  4, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 2, 2, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  5, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 2, 3, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 29, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 2, 4, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 28, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 2, 5, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 2, 6, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 2, 7, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 2, 8, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 2, 9, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  9, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 2,10, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  9, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 2,11, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 10, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 2,12, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 10, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 2,13, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 10, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 2,14, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 10, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 2,15, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 10, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
                                                                                                                                                                                                    \
    /* Port 3: GPIO 48-63 */                                                                                                                                                                        \
    GPIO_TABLE_ENTRY( 3, 0, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 11, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 3, 1, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 11, 1, 0,  GPIO_MUX_REGID_MFSL3, 19, 1, 0), \
    GPIO_TABLE_ENTRY( 3, 2, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 11, 1, 0,  GPIO_MUX_REGID_MFSL3, 19, 1, 0), \
    GPIO_TABLE_ENTRY( 3, 3, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 11, 1, 0,  GPIO_MUX_REGID_MFSL3, 19, 1, 0), \
    GPIO_TABLE_ENTRY( 3, 4, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 11, 1, 0,  GPIO_MUX_REGID_MFSL3, 19, 1, 0), \
    GPIO_TABLE_ENTRY( 3, 5, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 11, 1, 0,  GPIO_MUX_REGID_MFSL3, 19, 1, 0), \
    GPIO_TABLE_ENTRY( 3, 6, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 11, 1, 0,  GPIO_MUX_REGID_MFSL3, 19, 1, 0), \
    GPIO_TABLE_ENTRY( 3, 7, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 11, 1, 0,  GPIO_MUX_REGID_MFSL3, 19, 1, 0), \
    GPIO_TABLE_ENTRY( 3, 8, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 12, 1, 0,  GPIO_MUX_REGID_MFSL3, 19, 1, 0), \
    GPIO_TABLE_ENTRY( 3, 9, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 13, 1, 0,  GPIO_MUX_REGID_MFSL3, 19, 1, 0), \
    GPIO_TABLE_ENTRY( 3,10, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 13, 1, 0,  GPIO_MUX_REGID_MFSL3, 19, 1, 0), \
    GPIO_TABLE_ENTRY( 3,11, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2, 30, 1, 0,  GPIO_MUX_REGID_MFSL3, 19, 1, 0), \
    GPIO_TABLE_ENTRY( 3,12, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2, 31, 1, 0,  GPIO_MUX_REGID_MFSL3, 19, 1, 0), \
    GPIO_TABLE_ENTRY( 3,13, GPIO_CAP_NO_INPUT, GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 10, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 3,14, GPIO_CAP_NO_INPUT, GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 10, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 3,15, GPIO_CAP_NO_INPUT, GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
                                                                                                                                                                                                    \
    /* Port 4: GPIO 64-79 */                                                                                                                                                                        \
    GPIO_TABLE_ENTRY( 4, 0, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2,  0, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 4, 1, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2,  1, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 4, 2, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2,  2, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 4, 3, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2,  3, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 4, 4, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2,  4, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 4, 5, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2,  5, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 4, 6, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2,  6, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 4, 7, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2,  7, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 4, 8, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2,  8, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 4, 9, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2,  9, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 4,10, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2, 10, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 4,11, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2, 11, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 4,12, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2, 12, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 4,13, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2, 13, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 4,14, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2, 14, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 4,15, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2, 15, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
                                                                                                                                                                                                    \
    /* Port 5: GPIO 80-94 */                                                                                                                                                                        \
    GPIO_TABLE_ENTRY( 5, 0, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2, 16, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 5, 1, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2, 17, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 5, 2, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2, 18, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 5, 3, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL2, 19, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 5, 4, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 14, 1, 0,  GPIO_MUX_REGID_MFSL3, 15, 1, 0), \
    GPIO_TABLE_ENTRY( 5, 5, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 14, 1, 0,  GPIO_MUX_REGID_MFSL3, 15, 1, 0), \
    GPIO_TABLE_ENTRY( 5, 6, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 14, 1, 0,  GPIO_MUX_REGID_MFSL3, 15, 1, 0), \
    GPIO_TABLE_ENTRY( 5, 7, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 14, 1, 0,  GPIO_MUX_REGID_MFSL3, 15, 1, 0), \
    GPIO_TABLE_ENTRY( 5, 8, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 14, 1, 0,  GPIO_MUX_REGID_MFSL3, 15, 1, 0), \
    GPIO_TABLE_ENTRY( 5, 9, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 14, 1, 0,  GPIO_MUX_REGID_MFSL3, 15, 1, 0), \
    GPIO_TABLE_ENTRY( 5,10, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 15, 1, 0,  GPIO_MUX_REGID_MFSL3, 15, 1, 0), \
    GPIO_TABLE_ENTRY( 5,11, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 16, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 5,12, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 16, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 5,13, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 17, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 5,14, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 17, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
                                                                                                                                                                                                    \
    /* GPIO 95 Undefined */                                                                                                                                                                         \
    GPIO_TABLE_ENTRY( 5,15, GPIO_CAP_NO_INPUT, GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
                                                                                                                                                                                                    \
    /* Port 6: GPIO 96-113 */                                                                                                                                                                       \
    GPIO_TABLE_ENTRY( 6, 0, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6, 1, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6, 2, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6, 3, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6, 4, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6, 5, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6, 6, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6, 7, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6, 8, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6, 9, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6,10, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6,11, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6,12, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6,13, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6,14, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6,15, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6,16, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 6,17, GPIO_CAP_INPUT,    GPIO_CAP_NO_OUTPUT, GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
                                                                                                                                                                                                    \
    /* Port 7: GPIO 114-127 */                                                                                                                                                                      \
    GPIO_TABLE_ENTRY( 7, 0, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  6, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 7, 1, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  6, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 7, 2, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  7, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 7, 3, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  7, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 7, 4, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  8, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 7, 5, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1,  8, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 7, 6, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 7, 7, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 7, 8, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 7, 9, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 7,10, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 7,11, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 7,12, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 7,13, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
                                                                                                                                                                                                    \
    /* Port 8: GPIO 128-143 */                                                                                                                                                                      \
    GPIO_TABLE_ENTRY( 8, 0, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(0),  GPIO_CAP_DEBOUNCE(0),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  6, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 8, 1, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(1),  GPIO_CAP_DEBOUNCE(1),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  7, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 8, 2, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(2),  GPIO_CAP_DEBOUNCE(2),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  7, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 8, 3, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(3),  GPIO_CAP_DEBOUNCE(3),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  3, 2, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 8, 4, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(4),  GPIO_CAP_DEBOUNCE(4),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 15, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 8, 5, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(5),  GPIO_CAP_DEBOUNCE(5),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 15, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 8, 6, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(6),  GPIO_CAP_DEBOUNCE(6),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 15, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 8, 7, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(7),  GPIO_CAP_DEBOUNCE(7),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 15, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 8, 8, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(8),  GPIO_CAP_DEBOUNCE(8),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 12, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 8, 9, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(9),  GPIO_CAP_DEBOUNCE(9),  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 12, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 8,10, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(10), GPIO_CAP_DEBOUNCE(10), GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 12, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 8,11, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(11), GPIO_CAP_DEBOUNCE(11), GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 12, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 8,12, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(12), GPIO_CAP_DEBOUNCE(12), GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 12, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 8,13, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(13), GPIO_CAP_DEBOUNCE(13), GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 12, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 8,14, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(14), GPIO_CAP_DEBOUNCE(14), GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 12, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 8,15, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(15), GPIO_CAP_DEBOUNCE(15), GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 12, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
                                                                                                                                                                                                    \
    /* Port 9: GPIO 144-159 */                                                                                                                                                                      \
    GPIO_TABLE_ENTRY( 9, 0, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_BLINK(0),  GPIO_MUX_REGID_MFSL2, 20, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 9, 1, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_BLINK(4),  GPIO_MUX_REGID_MFSL2, 21, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 9, 2, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_BLINK(8),  GPIO_MUX_REGID_MFSL2, 22, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 9, 3, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_BLINK(12), GPIO_MUX_REGID_MFSL2, 23, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 9, 4, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_BLINK(16), GPIO_MUX_REGID_MFSL3, 11, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 9, 5, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_BLINK(20), GPIO_MUX_REGID_MFSL3, 11, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 9, 6, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_BLINK(24), GPIO_MUX_REGID_MFSL3, 11, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 9, 7, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_BLINK(28), GPIO_MUX_REGID_MFSL3, 11, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 9, 8, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(16), GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 10, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 9, 9, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_EVENT(17), GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 10, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 9,10, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 10, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 9,11, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 10, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 9,12, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 10, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 9,13, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 10, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 9,14, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 10, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY( 9,15, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 10, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
                                                                                                                                                                                                    \
    /* Port 10: GPIO 160-175 */                                                                                                                                                                     \
    GPIO_TABLE_ENTRY(10, 0, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 21, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(10, 1, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 26, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(10, 2, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 31, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(10, 3, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 26, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(10, 4, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 26, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(10, 5, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 26, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(10, 6, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 26, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(10, 7, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 26, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(10, 8, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 16, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(10, 9, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  0, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(10,10, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 22, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(10,11, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  1, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(10,12, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  1, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(10,13, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  2, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(10,14, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  2, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(10,15, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  3, 2, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
                                                                                                                                                                                                    \
    /* Port 11: GPIO 176-191 */                                                                                                                                                                     \
    GPIO_TABLE_ENTRY(11, 0, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  3, 2, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(11, 1, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  3, 2, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(11, 2, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  9, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(11, 3, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  9, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(11, 4, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  9, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(11, 5, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  9, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(11, 6, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  9, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(11, 7, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  8, 1, 0,  GPIO_MUX_REGID_MFSL3, 31, 1, 0), \
    GPIO_TABLE_ENTRY(11, 8, GPIO_CAP_NO_INPUT, GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  5, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(11, 9, GPIO_CAP_NO_INPUT, GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  5, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(11,10, GPIO_CAP_NO_INPUT, GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  5, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(11,11, GPIO_CAP_NO_INPUT, GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  5, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(11,12, GPIO_CAP_NO_INPUT, GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  5, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(11,13, GPIO_CAP_NO_INPUT, GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  5, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(11,14, GPIO_CAP_NO_INPUT, GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  5, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(11,15, GPIO_CAP_NO_INPUT, GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  5, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
                                                                                                                                                                                                    \
    /* Port 12: GPIO 192-207 */                                                                                                                                                                     \
    GPIO_TABLE_ENTRY(12, 0, GPIO_CAP_NO_INPUT, GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 17, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(12, 1, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  9, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(12, 2, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(12, 3, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(12, 4, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(12, 5, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(12, 6, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(12, 7, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(12, 8, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL1, 14, 1, 0,  GPIO_MUX_REGID_MFSL3, 15, 1, 0), \
    GPIO_TABLE_ENTRY(12, 9, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3,  9, 1, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(12,10, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_NONE,   0, 0, 0,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(12,11, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 15, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(12,12, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 22, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(12,13, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 22, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(12,14, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 22, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
    GPIO_TABLE_ENTRY(12,15, GPIO_CAP_INPUT,    GPIO_CAP_OUTPUT,    GPIO_CAP_NO_EVENT,  GPIO_CAP_NO_DEBOUNCE,  GPIO_CAP_NO_BLINK,  GPIO_MUX_REGID_MFSL3, 22, 1, 1,  GPIO_MUX_REGID_NONE,   0, 0, 0), \
}

#endif //__NPCM850_CHIP_H_

