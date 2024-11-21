/* Linker script to place sections and symbol values.
 * It references following symbols, which must be defined in code:
 *   start64 : Entry point
 * 
 * It defines following symbols, which code can use without definition:
 *   __cs3_peripherals
 *   __code_start
 *   __exidx_start
 *   __exidx_end
 *   __data_start
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   __stack
 *   __el3_stack
 *   __ttb0_l1
 *   __ttb0_l2_ram
 *   __ttb0_l2_private
 *   __ttb0_l2_periph
 *	 __ttb0_l3
 *   __top_of_ram
 */

MEMORY
{
	SPI0CS0   :  ORIGIN = 0x80000000,  LENGTH = 0x08000000
	SPI0CS1   :  ORIGIN = 0x88000000,  LENGTH = 0x08000000
	SPI1CS0   :  ORIGIN = 0x90000000,  LENGTH = 0x01000000
	SPI1CS1   :  ORIGIN = 0x91000000,  LENGTH = 0x01000000
	SPI1CS2   :  ORIGIN = 0x92000000,  LENGTH = 0x01000000
	SPI1CS3   :  ORIGIN = 0x93000000,  LENGTH = 0x01000000
	SPIXCS0   :  ORIGIN = 0xF8000000,  LENGTH = 0x01000000
	SPIXCS1   :  ORIGIN = 0xF9000000,  LENGTH = 0x01000000
	SPI3CS0   :  ORIGIN = 0xA0000000,  LENGTH = 0x08000000
	SPI3CS1   :  ORIGIN = 0xA8000000,  LENGTH = 0x08000000
	SPI3CS2   :  ORIGIN = 0xB0000000,  LENGTH = 0x08000000
	SPI3CS3   :  ORIGIN = 0xB8000000,  LENGTH = 0x08000000
	SHM       :  ORIGIN = 0xC0001000,  LENGTH = 0x00000800
	RAM3      :  ORIGIN = 0xC0008000,  LENGTH = 0x00004000
	GICD      :  ORIGIN = 0xDFFF9000,  LENGTH = 0x00001000
	GICC      :  ORIGIN = 0xDFFFA000,  LENGTH = 0x00006000
	ROMa      :  ORIGIN = 0xFFF40000,  LENGTH = 0x00010000
	ROMb      :  ORIGIN = 0xFFF50000,  LENGTH = 0x00010000
	
	RAM2  (R)    :  ORIGIN = 0xFFFB0200,  LENGTH = 0x00020000

	RAM2_DATA (RW):  ORIGIN = 0xFFFD0200,  LENGTH = 0x0001FD00
	
	VECTa     :  ORIGIN = 0xFFFF0000,  LENGTH = 0x00000100
	VECTb     :  ORIGIN = 0xFFFF0000,  LENGTH = 0x00000100
	ROM       :  ORIGIN = 0xFFFF0100,  LENGTH = 0x0000FF00
	null_data	:	ORIGIN = 0x00100000,	LENGTH = 0x0		/* to make sure .data does not exist */
}

ENTRY(start64)

SECTIONS
{

    .StartUp :
    {
	    PROVIDE (_ram_start = .);
    	./source/startup.o(.text.start64*)
    	./source/startup.o(.text.el3_primary*)
    } > RAM2

    .StartUp_secondary :
    {
    	./source/bootblock_main.o(.text.*)    
    	./source/startup.o(.text.el3_secondary*)
    	./source/startup.o(.data.el3_secondary*)
    } > RAM2

    .text :
    {
        *(.text*)
	. = ALIGN(64);
    } > RAM2

    .rodata :
    {
	    __bootblock_start = .;
        *(.data .data.* .rodata .rodata.* )
        __bootblock_end = .;
    } > RAM2



    .ttb0_l1 :
    {
        . = ALIGN(4096);
        __ttb0_l1 = .;
        *(ttb0_l1)
        __ttb0_l1_end = .;
    } >RAM2
    
    .bss  (NOLOAD):
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM2_DATA

    .hardening_table  (NOLOAD):
    {
        . = ALIGN(16);
        __hardening_table_start__ = .;
        . = ALIGN(16*100);
        __hardening_table_end__ = .;
        PROVIDE (_ram_end = .);
    } > RAM2_DATA


	/* stack */
    .el3_stack (NOLOAD):
    {
    	PROVIDE (_stack_end = .);
        . = ALIGN(64);
        . = . + 0xC00;
        __el3_stack = .;
        PROVIDE ( _stack_start = . );
    } > RAM2_DATA

    /*
     * GICv2 distributor
     */
    .gicd (NOLOAD):
    {
        __gicd = .;
    } > GICD

    /*
     * GICv2 CPU interface
     */
    .gicc (NOLOAD):
    {
         __gicc = .;
    } > GICC   
}
