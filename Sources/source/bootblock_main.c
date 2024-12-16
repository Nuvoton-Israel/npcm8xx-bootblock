/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   bootblock_main.c                                                         */
/*            This file contains the main bootblock flow and helper functions */
/*  Project:  Arbel                                                           */
/*----------------------------------------------------------------------------*/
#define MAIN_C

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "hal.h"
#include "../SWC_HAL/hal.h"
#include "../SWC_HAL/hal_regs.h"
#include "boot.h"
#include "mailbox.h"
#include "cfg.h"
#include <string.h>
#include "apps/serial_printf/serial_printf.h"
#include "../SWC_HAL/Chips/npcm850/npcm850_fuse_wrapper.h"
#include "hal_regs.h"

#ifdef _NOTIP_
#include "images.h"
#endif


extern void asm_jump_to_address (UINT32 address);
extern void disable_highvecs (void);
extern void enable_highvecs (void);


#ifdef BOOTBLOCK_STACK_PROFILER
/**
 * Print TIP stack profiler.
 *
 * This function prints the MSP stack usage stats.
 * For L0 the stats will be for the MSP.
 * For L1 the stats are only for the RTOS stack.
 */
static void stack_profiler (void)
{
	extern unsigned long _stack_end;
	extern unsigned long _stack_start;
	unsigned long stack_used;
	unsigned long stack_total;
	extern uint32_t _ram_start;
	extern uint32_t _ram_end;
	unsigned long *p;

	serial_printf(KGRN "\nImage stats\n" KNRM);
	serial_printf("code start = %#010lx \n" , &_ram_start);
	serial_printf("code end   = %#010lx \n" , &_ram_end);
	serial_printf("code size  = %#010lx \n\n" , 
		(uint32_t) &_ram_end - (uint32_t) &_ram_start);
	
	p = &_stack_end;
	while (p < &_stack_start) {
		if (*p == (unsigned long)0xAAAAAAAAAAAAAAAA) {
			p++;
			}
		else {
			break;
		}
	}

	stack_used = (unsigned long)(&_stack_start - p);
	stack_total = (unsigned long)(&_stack_start - &_stack_end);
	
	serial_printf ("stack top       %#010lx \n", &_stack_end);
	serial_printf ("stack watermark %#010lx \n", p);
	serial_printf ("stack bottom    %#010lx \n", &_stack_start);
	serial_printf ("stack usage     %#010lx \n", stack_used);
	serial_printf ("stack usage percentage: %lu%% \n\n\n", (stack_used * 100) / stack_total);
}
#endif // BOOTBLOCK_STACK_PROFILER

/*---------------------------------------------------------------------------------------------------------*/
/* Prototypes                                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void bootblock_main (void);
static void bootblock_PrintClocks (void);
#ifdef _NOTIP_
static void platform_reset (void);
#endif

extern const BOOTBLOCK_Version_T bb_version;

static DDR_Setup ddr_setup;


typedef __attribute__((noreturn)) void (*jumpFunction)();

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        errHandler                                                                             */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void errHandler (void)
{
	uint64_t regs[35];
	uint64_t lr[3], sp;
	uint64_t esr[3];
	uint64_t scltr[3];
	uint64_t far[3];
	int i;

	__asm__ __volatile__("mrs %0, elr_el1\n" : "=r"(lr[0])::);
	__asm__ __volatile__("mrs %0, elr_el2\n" : "=r"(lr[1])::);
	__asm__ __volatile__("mrs %0, elr_el3\n" : "=r"(lr[2])::);

	__asm__ __volatile__("mrs %0, esr_el1\n" : "=r"(esr[0])::);
	__asm__ __volatile__("mrs %0, esr_el2\n" : "=r"(esr[1])::);
	__asm__ __volatile__("mrs %0, esr_el3\n" : "=r"(esr[2])::);

	__asm__ __volatile__("mrs %0, far_el1\n" : "=r"(far[0])::);
	__asm__ __volatile__("mrs %0, far_el2\n" : "=r"(far[1])::);
	__asm__ __volatile__("mrs %0, far_el3\n" : "=r"(far[2])::);

	__asm__ __volatile__("mov %0, sp\n" : "=r"(sp)::);

	__asm__ __volatile__("mrs %0, SCTLR_EL1 \n" : "=r"(scltr[0])::);
	__asm__ __volatile__("mrs %0, SCTLR_EL2" : "=r"(scltr[1])::);
	__asm__ __volatile__("mrs %0, SCTLR_EL3" : "=r"(scltr[2])::);

	__asm__ __volatile__("mov %0, x0" : "=r"(regs[0])::);
	__asm__ __volatile__("mov %0, x1" : "=r"(regs[1])::);
	__asm__ __volatile__("mov %0, x2" : "=r"(regs[2])::);
	__asm__ __volatile__("mov %0, x3" : "=r"(regs[3])::);
	__asm__ __volatile__("mov %0, x4" : "=r"(regs[4])::);
	__asm__ __volatile__("mov %0, x5" : "=r"(regs[5])::);
	__asm__ __volatile__("mov %0, x6" : "=r"(regs[6])::);
	__asm__ __volatile__("mov %0, x7" : "=r"(regs[7])::);
	__asm__ __volatile__("mov %0, x8" : "=r"(regs[8])::);
	__asm__ __volatile__("mov %0, x9" : "=r"(regs[9])::);
	__asm__ __volatile__("mov %0, x10" : "=r"(regs[10])::);
	__asm__ __volatile__("mov %0, x11" : "=r"(regs[11])::);
	__asm__ __volatile__("mov %0, x12" : "=r"(regs[12])::);
	__asm__ __volatile__("mov %0, x13" : "=r"(regs[13])::);
	__asm__ __volatile__("mov %0, x14" : "=r"(regs[14])::);
	__asm__ __volatile__("mov %0, x15" : "=r"(regs[15])::);
	__asm__ __volatile__("mov %0, x16" : "=r"(regs[16])::);
	__asm__ __volatile__("mov %0, x17" : "=r"(regs[17])::);
	__asm__ __volatile__("mov %0, x18" : "=r"(regs[18])::);
	__asm__ __volatile__("mov %0, x19" : "=r"(regs[19])::);
	__asm__ __volatile__("mov %0, x20" : "=r"(regs[20])::);
	__asm__ __volatile__("mov %0, x21" : "=r"(regs[21])::);
	__asm__ __volatile__("mov %0, x22" : "=r"(regs[22])::);
	__asm__ __volatile__("mov %0, x23" : "=r"(regs[23])::);
	__asm__ __volatile__("mov %0, x24" : "=r"(regs[24])::);
	__asm__ __volatile__("mov %0, x25" : "=r"(regs[25])::);
	__asm__ __volatile__("mov %0, x26" : "=r"(regs[26])::);
	__asm__ __volatile__("mov %0, x27" : "=r"(regs[27])::);
	__asm__ __volatile__("mov %0, x28" : "=r"(regs[28])::);
	__asm__ __volatile__("mov %0, x29" : "=r"(regs[29])::);
	__asm__ __volatile__("mov %0, x30" : "=r"(regs[30])::);

	serial_printf_init();
	serial_printf(KRED "\n\nA35 Bootblock: trap error handler\n" KBLU);

	for (i = 0; i < 30; i++)
	{
		if ((i % 4) == 0)
			serial_printf("\n");
		serial_printf("X%d =  %#010lx \t", i, regs[i]);
	}
	serial_printf("\n");

	for (i = 0; i < 3; i++)
	{
		serial_printf("SCTLR_EL%d = %#010lx\n", i + 1, scltr[i]);
	}
	serial_printf("\n");

	for (i = 0; i < 3; i++)
	{
		serial_printf("ELR_EL%d = %#010lx\n", i + 1, lr[i]);
	}
	serial_printf("\n");

	for (i = 0; i < 3; i++)
	{
		serial_printf("FAR_EL%d = %#010lx\n", i + 1, far[i]);
	}
	serial_printf("\n");

	for (i = 0; i < 3; i++)
	{
		serial_printf("ESR_EL%d = %#010lx\n", i + 1, esr[i]);
	}

	serial_printf("\nSP = %#010lx\n" KNRM, sp);

	MC_ClearInterrupts();

	serial_printf(KRED "\n ===============   RESTART BOOTBLOCK AFTER PANIC   ================== \n" KNRM);

	CLK_Delay_Sec(5);

#ifdef _NOTIP_
	/* NO_TIP: after a TRAP, nothing to do but FSW reset */
	REG_WRITE(FSWCR, BUILD_FIELD_VAL(FSWCR_WTE, 1) |
			BUILD_FIELD_VAL(FSWCR_WTRE, 1) |
			BUILD_FIELD_VAL(FSWCR_WDT_CNT, 1));
#else
	/* notify TIP to retry */
	REG_WRITE(SCRPAD_10_41(0), 0x02);
	REG_WRITE(B2CPNT2, 0x02);
	while (1);
#endif
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        irqHandler                                                                             */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void irqHandler (void)
{
	// Get interrupt ID:
	unsigned int int_id = READ_REG_FIELD(GIC_C_IAR, GIC_C_IAR_InterruptID);

	serial_printf_init();
	serial_printf(KRED "\n\nA35 Bootblock: IRQ handler\n");
	MC_PrintRegs();
	MC_ClearInterrupts();

	// Signal end of interrupt:
	SET_REG_FIELD (GIC_C_EOIR, GIC_C_EOIR_InterruptID, int_id);

}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        fiqHandler                                                                             */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void fiqHandler (void)
{
	// Get interrupt ID:
	unsigned int int_id = READ_REG_FIELD(GIC_C_IAR, GIC_C_IAR_InterruptID);

	serial_printf_init();
	serial_printf(KRED "\n\nA35 Bootblock: FIQ handler\n");
	MC_PrintRegs();
	MC_ClearInterrupts();

	// Signal end of interrupt:
	SET_REG_FIELD (GIC_C_EOIR, GIC_C_EOIR_InterruptID, int_id);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:        TIP_ROM_StrapCKFRQ                                                                     */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:                                                                                                */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine gets straps clock value.                                                  */
/*---------------------------------------------------------------------------------------------------------*/
static UINT32 TIP_ROM_StrapCKFRQ (void)
{
	UINT32 ret_val;

	// read STRAP1, 2, 14 to check the clock settings + Fast skip init STRAP
	ret_val = ((UINT16)(READ_REG_FIELD(PWRON, PWRON_SKIP_INIT_FAST) << 2) |
			   READ_REG_FIELD(PWRON, PWRON_CKFRQ));

	// SKIP_INIT_FAST strap ignored in normal clocks modes:
	if ((ret_val > 5) || (ret_val == 4))
		ret_val = ret_val & 0x03;

	return ret_val;
}

/*----------------------------------------------------------------------------*/
/* Function:        bootblock_ChangeClocks                                    */
/*                                                                            */
/* Parameters:      none                                                      */
/* Returns:         none                                                      */
/* Side effects:                                                              */
/* Description:                                                               */
/*                  Change clocks according to header                         */
/*----------------------------------------------------------------------------*/
static void bootblock_ChangeClocks (DDR_Setup *ddr_setup)
{
	volatile UINT32 cpuFreq = BOOTBLOCK_Get_CPU_freq();
	volatile UINT32 mcFreq = BOOTBLOCK_Get_MC_freq();
	BOOLEAN bUpdate = FALSE;
	DEFS_STATUS status;
	UINT32 straps = 0;
	UINT32 clk4Freq;
	UINT32 pll0_override = 0;
	UINT8 div_50MHz;
	UINT8 div;
	volatile UINT cntfrq_val;

	straps = TIP_ROM_StrapCKFRQ();

	// in skip init or fast skip init need to start the PLLs first
	if ((straps == 5) || (straps == 1))
	{
		CLK_ConfigureClocks(3);
	}

	serial_printf(KMAG "\nPrevious PLL settings:\n" KNRM);
	serial_printf(">PLL0 = %d.%d MHz \n", PRINT_FLOAT(CLK_GetPll0Freq()));
	serial_printf(">PLL1 = %d.%d MHz \n", PRINT_FLOAT(CLK_GetPll1Freq()));
	serial_printf(">PLL2 = %d.%d MHz \n\n", PRINT_FLOAT(CLK_GetPll2Freq()));

	if ((cpuFreq != 0) && (cpuFreq != 0xFFFF))
	{
		bUpdate = TRUE;
		serial_printf(KYEL "Change CPU freq to %d \n" KNRM, cpuFreq);
	}
	else
	{
		serial_printf(" CPU freq in header %d\n" KNRM, cpuFreq);
	}

	if ((mcFreq != 0) && (mcFreq != 0xFFFF))
	{
		bUpdate = TRUE;
		serial_printf(KYEL "Change MC freq to %d \n" KNRM, mcFreq);
	}
	else
	{
		serial_printf(" MC freq in header %d\n" KNRM, mcFreq);
	}

	if (ddr_setup->ECC_enable == TRUE)
	{
		UINT32 minimum_frequency = MIN(cpuFreq, mcFreq);
		cpuFreq = minimum_frequency;
		mcFreq = minimum_frequency;
		serial_printf(KYEL "When ECC enabled CPU and MC are set to the same value: %d\n" KNRM, minimum_frequency);
	}

	if (bUpdate == TRUE)
	{
		// if not PORST don't update PLLs
		if (READ_REG_FIELD(INTCR2, INTCR2_MC_INIT))
		{
			bUpdate = FALSE;
			serial_printf(KMAG "WARNING: change PLLs without PORST not allowed\n" KNRM);
		}
		else
		{
			serial_printf("Change freq cpu %d mc %d\n", cpuFreq, mcFreq);
			cpuFreq = cpuFreq * 1000000;
			mcFreq = mcFreq * 1000000;

			// if MC==CPU frequency, can change pll0 to other values according to header value:
			if (cpuFreq == mcFreq)
			{
				pll0_override = BOOTBLOCK_Get_pll0_override();
				if (pll0_override != 0)
				{
					serial_printf (KMAG "Override PLL0 to %d\n" KNRM, pll0_override);
				}
				pll0_override = pll0_override * 1000000;
			}
			
			status = CLK_Configure_CPU_MC_Clock(mcFreq, cpuFreq, pll0_override);
			if ((status != DEFS_STATUS_OK) && (status != DEFS_STATUS_SYSTEM_NOT_INITIALIZED))
			{
				serial_printf("can't set plls, stat = %d\n", status);
			}
		}
	}

	cpuFreq = CLK_GetCPUFreq();

	// in PORST only, update dividers without changing the PLLs:
	if (READ_REG_FIELD(INTCR2, INTCR2_MC_INIT) == 0)
	{
		CLK_Verify_and_update_dividers();

		// update AHBCKFI.AHB_CLK_FRQ according AHB frequency (same as CP freq)
		SET_REG_FIELD(AHBCKFI, AHBCKFI_AHB_CLK_FRQ, CLK_GetCPFreq() / _1MHz_);
		serial_printf_init();

		BOOTBLOCK_Get_pll0_override ();

		CLK_ConfigureGMACClock(0);
		CLK_SetPixelClock();

		// override i3c\RC divider from header:
		div = BOOTBLOCK_Get_i3c_RC_clk_divider();
		if ((div != 0) && (div != 0xFF))
		{
			serial_printf ("Override i3c and RC clk div %d\nWarning: RC not supported anymore\n", div);
			CLK_Configure_I3C_Clock (div);
		}
		else
		{
			CLK_ConfigureRootComplexClock();
		}
	}

	//  Update FIU dividers:
	clk4Freq = cpuFreq / 2 / (READ_REG_FIELD(CLKDIV1, CLKDIV1_CLK4DIV) + 1);
	div_50MHz = DIV_ROUND(clk4Freq, (50 * _1MHz_));

	for (int fiu = 0; fiu < 3; fiu++)
	{
		div = BOOTBLOCK_Get_SPI_clk_divider(fiu);
		if ((div == 0) || (div == 0xFF))
		{
			div = div_50MHz;
		}
		serial_printf("Set FIU%d divider to %d (%d.%d MHz)\n", fiu, div, PRINT_FLOAT((clk4Freq / div)));
		CLK_ConfigureFIUClock(fiu, div);
	}

	// update cntfrq_el0
	if (cpuFreq != 1000000000)
	{
		cntfrq_val = DIV_ROUND(cpuFreq, 4);

		serial_printf("Set cntfrq_val to %d cpufreq %d\n", cntfrq_val, cpuFreq);
		__asm__ __volatile__("ISB\n");
		__asm__ __volatile__("msr cntfrq_el0, %0\n" :: "r"(cntfrq_val));
	}

	return;
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:        bootblock_PrintLogo                                                                    */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  This routine prints header of bootblock with version number and date of compilation    */
/*---------------------------------------------------------------------------------------------------------*/
static void bootblock_PrintLogo (void)
{
	UINT32 bootblockVersion = bb_version.BootblockVersion;

	serial_printf_init();

	serial_printf(KMAG "\n>==========================================================================================\n");
	serial_printf("> Arbel A35 BootBlock by Nuvoton Technology Corp. Ver ");
	serial_printf("%x.", (UINT8)(bootblockVersion >> 16));
	serial_printf("%x.", (UINT8)(bootblockVersion >> 8));
	serial_printf("%x", (UINT8)(bootblockVersion));
#ifdef _NOTIP_
	serial_printf("      NO TIP MODE");
#endif
	serial_printf("\n>==========================================================================================\n" KNRM);
	serial_printf("%s \n" KNRM, bb_version.VersionDescription);

	serial_printf(KGRN "\n\n=========\nArbel ");
	if (CHIP_Get_Version() == 0x08)
		serial_printf("A2");
	else if (CHIP_Get_Version() == 0x04)
		serial_printf("A1");
	else
		serial_printf("Z1");
	serial_printf("\n=========\n" KNRM);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        bootblock_PrintClocks                                                                  */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Print clocks                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
static void bootblock_PrintClocks (void)
{
	serial_printf("\n\nPWRON = %#010lx\n\nFinal clk settings:\n", REG_READ(PWRON));

	serial_printf("PLL0 = %d.%d MHz \n", PRINT_FLOAT(CLK_GetPll0Freq()));
	serial_printf("PLL1 = %d.%d MHz \n", PRINT_FLOAT(CLK_GetPll1Freq()));
	serial_printf("PLL2/2 = %d.%d MHz \n", PRINT_FLOAT(CLK_GetPll2Freq()));

	serial_printf("PLLCON0  = %#010lx\n", REG_READ(PLLCON0));
	serial_printf("PLLCON1  = %#010lx\n", REG_READ(PLLCON1));
	serial_printf("PLLCON2  = %#010lx\n", REG_READ(PLLCON2));
	serial_printf("CLKSEL   = %#010lx\n", REG_READ(CLKSEL));
	serial_printf("CLKDIV1  = %#010lx\n", REG_READ(CLKDIV1));
	serial_printf("CLKDIV2  = %#010lx\n", REG_READ(CLKDIV2));
	serial_printf("CLKDIV3  = %#010lx\n", REG_READ(CLKDIV3));
	serial_printf("CLKDIV4  = %#010lx\n", REG_READ(CLKDIV4));

	serial_printf("\nCPU CLK = %d.%d MHz \n", PRINT_FLOAT(CLK_GetCPUFreq()));
	serial_printf("MC CLK = %d.%d MHz\n", PRINT_FLOAT(CLK_GetMCFreq()));

	serial_printf("APB1 = %d.%d MHz  \n", PRINT_FLOAT(CLK_GetAPBFreq(CLK_APB1)));
	serial_printf("APB2 = %d.%d MHz \n", PRINT_FLOAT(CLK_GetAPBFreq(CLK_APB2)));
	serial_printf("APB3 = %d.%d MHz \n", PRINT_FLOAT(CLK_GetAPBFreq(CLK_APB3)));
	serial_printf("APB4 = %d.%d MHz \n", PRINT_FLOAT(CLK_GetAPBFreq(CLK_APB4)));
	serial_printf("APB5 = %d.%d MHz \n", PRINT_FLOAT(CLK_GetAPBFreq(CLK_APB5)));
	serial_printf("SPI0 = %d.%d MHz \n", PRINT_FLOAT(CLK_GetAPBFreq(CLK_SPI0)));
	serial_printf("SPI1 = %d.%d MHz \n", PRINT_FLOAT(CLK_GetAPBFreq(CLK_SPI1)));
	serial_printf("SPI3 = %d.%d MHz \n", PRINT_FLOAT(CLK_GetAPBFreq(CLK_SPI3_AHB3)));
	serial_printf("SPIX = %d.%d MHz \n", PRINT_FLOAT(CLK_GetAPBFreq(CLK_SPIX)));
	serial_printf("ADC  = %d.%d MHz \n", PRINT_FLOAT(CLK_GetADCClock()));
	serial_printf("CP/TIP= %d.%d MHz \n", PRINT_FLOAT(CLK_GetCPFreq()));
	serial_printf("GFX  = %d.%d MHz \n", PRINT_FLOAT(CLK_GetGFXClock()));
	serial_printf("PCI  = %d.%d MHz \n", PRINT_FLOAT(CLK_GetPCIClock()));
	serial_printf("MMC  = %d.%d MHz \n", PRINT_FLOAT(CLK_GetSDClock(0)));
	serial_printf("SD   = %d.%d MHz \n", PRINT_FLOAT(CLK_GetSDClock(1)));
	serial_printf("CLKOUT= %d.%d MHz \n", PRINT_FLOAT(CLK_GetClkoutFreq()));
	serial_printf("PIXEL = %d.%d MHz \n", PRINT_FLOAT(CLK_GetPixelClock()));
	serial_printf("OHCI  = %d.%d MHz \n", PRINT_FLOAT(CLK_GetUSB_OHCI_Clock()));
	serial_printf("UTMI  = %d.%d MHz \n", PRINT_FLOAT(CLK_GetUSB_UTMI_Clock()));
	serial_printf("RC,I3C= %d.%d MHz \n", PRINT_FLOAT(CLK_Get_RC_Phy_and_I3C_Clock()));
	serial_printf("UART  = %d.%d MHz \n", PRINT_FLOAT(CLK_GetUartClock()));
	serial_printf("GMAC  = %d.%d MHz \n\n", PRINT_FLOAT(CLK_GetGMACClock()));

	if (CLK_GetGMACClock() != 125000000) {
		serial_printf(KRED "ERROR GMAC value not 125MHz\n" KNRM);
	}

	if (CLK_Get_RC_Phy_and_I3C_Clock() != 100000000) {
		serial_printf(KMAG "warning RC value not 100MHz, PCI root complex not supported\n" KNRM);
	}

	return;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        bootblock_set_fiu_cfg_drd                                                              */
/*                                                                                                         */
/* Parameters:      none                                                                                   */
/* Returns:         none                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Init FIU_DRD_CFG according to header                                                   */
/*---------------------------------------------------------------------------------------------------------*/
static void bootblock_set_fiu_cfg_drd (void)
{
	UINT32 drd;
	UINT fiu[3] = {0, 1, 3};
	for (int i = 0; i < 3; i++)
	{
		drd = BOOTBLOCK_Get_FIU_DRD_CFG(i);

		if ((drd != 0) && (drd != 0xFFFFFFFF))
		{
			serial_printf(KMAG "Set FIU%d_CFG_DRD to %#010lx\n" KNRM, fiu[i], drd);
			CLK_Delay_MicroSec(1000);
			REG_WRITE(FIU_DRD_CFG(i), drd);
		}
	}
}

#ifdef _NOTIP_
/**
 * @brief Reset TIP
 *
 * @param reset_type 0: SW Reset
 */
static void platform_reset()
{
	serial_printf(KRED "\n\n==========    RESET BMC %#010lx ==========\n" KNRM);
	/* Reset everything.*/
	REG_WRITE(SWRSTC3, 0xFFFFFFFF);
	REG_WRITE(SWRSTC3B, 0xFFFFFFFF);

	/* Don't reset MC.*/
	SET_REG_FIELD(SWRSTC3, WD0RCR_MC, 0);
	SET_REG_FIELD(SWRSTR, SWRSTR_SWRST3, 1);

	/* Un-reachable line */
	while (1);
}
#endif

#define DIE_INFORMATION_ALL 168, 5, FUSE_ECC_NONE

/*----------------------------------------------------------------------------*/
/* Function:        bootblock_main                                            */
/*                                                                            */
/* Parameters:      none                                                      */
/* Returns:         none                                                      */
/* Side effects:                                                              */
/* Description:                                                               */
/*                  This is the Arbel main bootblock flow                     */
/*----------------------------------------------------------------------------*/
__attribute__((noreturn)) void bootblock_main (void)
{
	DEFS_STATUS status;
	UINT32 t;
	UINT32 error = 0;
	UINT32 bb_src_flash_addr = 0;
	UINT32 uboot_src_flash_addr = 0;

	BOARD_T eBoard;
	BOARD_VENDOR_T eVendor; // Board vendor.
	HOST_IF_T eHostIf;
	int resetNum;
	uint64_t addr64 = 0;
	UINT8 buff[8]     __attribute__((aligned(16))); 
	// clear notification status from TIP
	REG_WRITE(CP2BST2, 0xFFFFFFFF);

	REG_WRITE(SCRPAD_10_41(0), 0xAAAAAAAA);

	for (int i = 4; i <= 9; i++)
		REG_WRITE(SCRPAD_10_41(i), 0);

	bootblock_PrintLogo();
#ifdef BOOTBLOCK_STACK_PROFILER
	stack_profiler ();
#endif

	serial_printf ("uptime %d.%d \n", PRINT_FLOAT2(CLK_GetUpTimeMiliseconds()));

	TMC_StopWatchDog(0);
	TMC_StopWatchDog(1);
	TMC_StopWatchDog(2);

	*(UINT32 *)buff = 0;
	*(UINT32 *)(buff + 4) = 0;
	
	/*--------------------------------------------------------------------*/
	/* Read Die information and send to OPTEE (HUK)                       */
	/*--------------------------------------------------------------------*/
	status = FUSE_WRPR_get(DIE_INFORMATION_ALL, buff);

	REG_WRITE(SCRPAD_42_73(30), *(UINT32 *)buff);
	REG_WRITE(SCRPAD_42_73(31), *(UINT32 *)(buff + 4));

	if (status == DEFS_STATUS_OK)
	{
		serial_printf(KCYN "DIE LOCATION: %#010lx %#010lx \n" KNRM,
					  REG_READ(SCRPAD_42_73(30)), REG_READ(SCRPAD_42_73(31)));
	}

	/*--------------------------------------------------------------------*/
	/* identify board according to flash header                           */
	/*--------------------------------------------------------------------*/
	eBoard = BOOTBLOCK_GetBoardType();
	eVendor = BOOTBLOCK_GetVendorType();

	/*-----------------------------------------------------------------------------------------------------*/
	/* Print board type and vendor                                                                         */
	/*-----------------------------------------------------------------------------------------------------*/
	if (eBoard == BOARD_EB)
	{
		serial_printf(KGRN ">EB\n" KNRM);
	}
	else if (eBoard == BOARD_SVB)
	{
		serial_printf(KGRN ">SVB\n" KNRM);
	}
	else if (eBoard == BOARD_RUN_BMC)
	{
		serial_printf(KGRN ">RUN_BMC\n" KNRM);
	}

	resetNum = CFG_PrintResetType();


	BOOTBLOCK_Get_DDR_Setup(&ddr_setup);

	// for debug and tester, allow delay of 3 seconds here
	if (ddr_setup.mc_config & MC_CAPABILITY_3_SEC_DELAY)
	{
		serial_printf("delay 3 sec\n");
		CLK_Delay_MicroSec(3 * 1000 * 1000);
	}

	bootblock_ChangeClocks(&ddr_setup);

	/*--------------------------------------------------------------------*/
	/* init FIU_DRD_CFG according to bootblock header                     */
	/*--------------------------------------------------------------------*/
	bootblock_set_fiu_cfg_drd();

	bootblock_PrintClocks();

	MC_Init_DDR_Setup(&ddr_setup);
	MC_Init_DDR_Setup_re_calc(&ddr_setup);
	status = MC_ConfigureDDR(&ddr_setup);

	if (status == DEFS_STATUS_SYSTEM_NOT_INITIALIZED)
	{
		serial_printf("MC already configured\n");
		MC_ClearInterrupts();
		status = DEFS_STATUS_OK;
	}
#ifdef _NOTIP_
	else if (status != DEFS_STATUS_OK)
	{
		serial_printf(KRED "\n\nNO_TIP: DDR training failed. FSW reset\n\n\n");

		CLK_Delay_MicroSec(1000);
		REG_WRITE(FSWCR, BUILD_FIELD_VAL(FSWCR_WTE, 1) |
							 BUILD_FIELD_VAL(FSWCR_WTRE, 1) |
							 BUILD_FIELD_VAL(FSWCR_WDT_CNT, 1));
	}
#endif

	serial_printf(KNRM "A35 Bootblock: configure DDR done \n");

	/*-----------------------------------------------------------------------------------------------------*/
	/* eSPI HOST INDEPENDENCE Register write 0x0001_111F  ()                                               */
	/* set:  AUTO_SBLD, AUTO_FCARDY, AUTO_OOBCRDY, AUTO_VWCRDY,AUTO_PCRDY, AUTO_HS[1:3[]                   */
	/* eSPI ESPICFG Register write 0x0300_0B10                                                             */
	/* set:        ESPICFG.MAXFREQ = 33MHz                                                                 */
	/* ESPICFG.IOMODE = Quad (11)                                                                          */
	/*                                                                                                     */
	/* SHM SMC_CTL Register write 0x80 // clear HOSTWAIT bit                                               */
	/*-----------------------------------------------------------------------------------------------------*/
	eHostIf = BOOTBLOCK_Get_host_if();

	if (READ_REG_FIELD(INTCR2, INTCR2_HOST_INIT) == 0)
	{
		if (eHostIf == HOST_IF_ESPI)
		{
			if (READ_REG_FIELD(INTCR2, INTCR2_DIS_ESPI_AUTO_INIT) == 0)
			{
				serial_printf(KNRM "\nEnable ESPI auto handshake\n\n");
				CHIP_MuxESPI(24); // 24mA
				ESPI_ConfigAutoHandshake(BUILD_FIELD_VAL(ESPIHINDP_AUTO_SBLD, 1) |
										 BUILD_FIELD_VAL(ESPIHINDP_AUTO_FCARDY, 1) |
										 BUILD_FIELD_VAL(ESPIHINDP_AUTO_OOBCRDY, 1) |
										 BUILD_FIELD_VAL(ESPIHINDP_AUTO_VWCRDY, 1) |
										 BUILD_FIELD_VAL(ESPIHINDP_AUTO_PCRDY, 1) |
										 BUILD_FIELD_VAL(ESPIHINDP_AUTO_HS1, 1) |
										 BUILD_FIELD_VAL(ESPIHINDP_AUTO_HS2, 1) |
										 BUILD_FIELD_VAL(ESPIHINDP_AUTO_HS3, 1));

				ESPI_Config(ESPI_IO_MODE_SINGLE_DUAL_QUAD, ESPI_MAX_33_MHz, ESPI_RST_OUT_LOW);
				SHM_ReleaseHostWait();

				/* Errata fix: 1.7 eSPI FATAL_ERROR response */
				REG_WRITE(ESPI_ESPI_TEN, 0x55);
				REG_WRITE(ESPI_ESPI_ENG, 0X40);
				REG_WRITE(ESPI_ESPI_TEN, 0x6C);
			}
		}

		else if (eHostIf == HOST_IF_LPC)
		{
			serial_printf(KYEL "\n>HOST IF: LPC\n");
			GPIO_Init(168, GPIO_DIR_INPUT, GPIO_PULL_NONE, GPIO_OTYPE_OPEN_DRAIN, FALSE); // ALERT_N
			CHIP_MuxLPC(12);															  // 12mA
			CFG_SHM_ReleaseHostWait();
			serial_printf(KNRM "\n>Host LPC Released\n");
		}

		else if (eHostIf == HOST_IF_GPIO)
		{
			serial_printf(KYEL "\n>HOST IF: None" KNRM);

			GPIO_Init(161, GPIO_DIR_INPUT, GPIO_PULL_NONE, GPIO_OTYPE_OPEN_DRAIN, FALSE);
			GPIO_Init(164, GPIO_DIR_INPUT, GPIO_PULL_NONE, GPIO_OTYPE_OPEN_DRAIN, FALSE);
			GPIO_Init(165, GPIO_DIR_INPUT, GPIO_PULL_NONE, GPIO_OTYPE_OPEN_DRAIN, FALSE);
			GPIO_Init(166, GPIO_DIR_INPUT, GPIO_PULL_NONE, GPIO_OTYPE_OPEN_DRAIN, FALSE);
			GPIO_Init(163, GPIO_DIR_INPUT, GPIO_PULL_NONE, GPIO_OTYPE_OPEN_DRAIN, FALSE);
			GPIO_Init(95, GPIO_DIR_INPUT, GPIO_PULL_NONE, GPIO_OTYPE_OPEN_DRAIN, FALSE);
			GPIO_Init(168, GPIO_DIR_INPUT, GPIO_PULL_NONE, GPIO_OTYPE_OPEN_DRAIN, FALSE);
			GPIO_Init(162, GPIO_DIR_INPUT, GPIO_PULL_NONE, GPIO_OTYPE_OPEN_DRAIN, FALSE);
			GPIO_Init(170, GPIO_DIR_INPUT, GPIO_PULL_NONE, GPIO_OTYPE_OPEN_DRAIN, FALSE);
			GPIO_Init(190, GPIO_DIR_INPUT, GPIO_PULL_NONE, GPIO_OTYPE_OPEN_DRAIN, FALSE);
			GPIO_Init(169, GPIO_DIR_INPUT, GPIO_PULL_NONE, GPIO_OTYPE_OPEN_DRAIN, FALSE);
		}

		else if (eHostIf == HOST_IF_RELEASE_HOST_WAIT)
		{
			/*-----------------------------------------------------------------------------------------------------*/
			/* Release Host Wait (LPC only)                                                                        */
			/*-----------------------------------------------------------------------------------------------------*/
			SET_REG_FIELD(MFSEL4, MFSEL4_ESPISEL, 0);
			SET_REG_FIELD(INTCR2, INTCR2_DIS_ESPI_AUTO_INIT, 1);
			CFG_SHM_ReleaseHostWait();
			serial_printf(KNRM "\n>Host wait released\n");
		}

		SET_REG_FIELD(INTCR2, INTCR2_HOST_INIT, 1);
	}



#ifndef _NOTIP_
	// Signal to TIP that BootBlock is done:
	if (status == DEFS_STATUS_OK)
	{
		serial_printf(KGRN "=============\nBootblock notify to TIP that DDR is ready \n===============\n\n" KNRM);
		REG_WRITE(B2CPNT2, 0x01);
		REG_WRITE(SCRPAD_10_41(0), 0x01);
	}
	else
	{
		serial_printf(KRED "=============\nBootblock notify to TIP that DDR is NOT ready \n===============\n\n" KNRM);
		REG_WRITE(B2CPNT2, 0x02);
		REG_WRITE(SCRPAD_10_41(0), 0x02);
	}

	// wait for TIP to copy UBOOT:
	while(REG_READ(CP2BST2) == 0);

	REG_WRITE(CP2BST2, 0xFFFFFFFF);

	addr64 = (uint64_t)REG_READ(SCRPAD_10_41(2));
#endif

#ifdef _NOTIP_
	if (bmc_firmware_init(&addr64) != 0)
	{
		serial_printf(KRED "=============\nA35 BOOTBLOCK FAILED TO LOAD IMAGE! \n===============\n\n" KNRM);
		CLK_Delay_MicroSec(2000 * 1000); /*delay 2 seconds */
		platform_reset();
	}
	serial_printf(KGRN "=============\nA35 BOOTBLOCK succeeded to load images \n===============\n\n" KNRM);

	/* wake core 1: */
	CLK_Delay_MicroSec(5);
	REG_WRITE(SCRPAD_10_41(5), 0);
	REG_WRITE(SCRPAD_10_41(4), addr64);

	/* wake core 2: */
	CLK_Delay_MicroSec(5);
	REG_WRITE(SCRPAD_10_41(7), 0);
	REG_WRITE(SCRPAD_10_41(6), addr64);

	/* wake core 3: */
	CLK_Delay_MicroSec(5);
	REG_WRITE(SCRPAD_10_41(9), 0);
	REG_WRITE(SCRPAD_10_41(8), addr64);

	CLK_Delay_MicroSec(20);

	for (int i = 2; i <= 9; i++)
		REG_WRITE(SCRPAD_10_41(i), 0);

#endif

#ifdef BOOTBLOCK_STACK_PROFILER
	stack_profiler ();
#endif

	// Go to BL31
	((jumpFunction)(uint64_t)addr64)();

	REG_WRITE(SCRPAD_10_41(0), 0xDEADBEAF);

	while(1);

}
