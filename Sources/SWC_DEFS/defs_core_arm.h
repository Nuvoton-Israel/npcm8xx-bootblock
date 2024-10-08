/*----------------------------------------------------------------------------*/
/* SPDX-License-Identifier: GPL-2.0                                           */
/*                                                                            */
/* Copyright (c) 2010-2024 by Nuvoton Technology Corporation                  */
/* All rights reserved                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* File Contents:                                                             */
/*   defs_core_arm.h                                                          */
/*            This file contains definitions for ARM core and its compilers   */
/* Project:                                                                   */
/*            SWC DEFS                                                        */
/*----------------------------------------------------------------------------*/
#ifndef _DEFS_ARM_H_
#define _DEFS_ARM_H_

/*---------------------------------------------------------------------------------------------------------*/
/* Core dependent types                                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
typedef UINT32              UINT;                       /* Native type of the core that fits the core's    */
typedef INT32               INT;                        /* internal registers                              */
typedef UINT                BOOLEAN;

/*---------------------------------------------------------------------------------------------------------*/
/* Core dependent PTR definitions                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define REG8                volatile  UINT8
#define REG16               volatile  UINT16
#define REG32               volatile  UINT32
#define REG64               volatile  UINT64

#define PTR8                REG8 *
#define PTR16               REG16 *
#define PTR32               REG32 *
#define PTR64               REG64 *

/*---------------------------------------------------------------------------------------------------------*/
/* Core dependent MEM definitions                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define MEMR8(a)            (*(PTR8) (a))
#define MEMR16(a)           (*(PTR16)(a))
#define MEMR32(a)           (*(PTR32)(a))
#define MEMR64(a)           (*(PTR64)(a))

#define MEMW8(a,v)          ((*((PTR8)  (a))) = ((UINT8)(v)))
#define MEMW16(a,v)         ((*((PTR16) (a))) = ((UINT16)(v)))
#define MEMW32(a,v)         ((*((PTR32) (a))) = ((UINT32)(v)))
#define MEMW64(a,v)         ((*((PTR64) (a))) = ((UINT64)(v)))

/*---------------------------------------------------------------------------------------------------------*/
/* Core dependent IO definitions                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define IOR8(a)             MEMR8(a)
#define IOR16(a)            MEMR16(a)
#define IOR32(a)            MEMR32(a)
#define IOR64(a)            MEMR64(a)

#define IOW8(a,v)           MEMW8(a, v)
#define IOW16(a,v)          MEMW16(a, v)
#define IOW32(a,v)          MEMW32(a, v)
#define IOW64(a,v)          MEMW64(a, v)

/*---------------------------------------------------------------------------------------------------------*/
/* Inline functions                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#define _INLINE_                                __inline

/*---------------------------------------------------------------------------------------------------------*/
/* Declare an argument/variable/function as unused                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#if defined (__GNUC__)
#define _UNUSED_                                __attribute__((unused))
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Interrupt Service Routines                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#if defined (__ARMCC_VERSION)
#define _ISR_                                   __irq
#define TYPEDEF_ISR_PTR(type_name)              typedef _ISR_ void (* type_name)(void)
#elif defined (__GNUC__) && defined(__ARM_ARCH_8__) 
#define _ISR_                                   
#define TYPEDEF_ISR_PTR(type_name)              typedef void (*type_name)(void)
#elif defined (__GNUC__)
#define _ISR_                                   __attribute__((interrupt))
#define TYPEDEF_ISR_PTR(type_name)              typedef void (* _ISR_ type_name)(void)
#else
#warning Warning: _ISR_ macro is not defined for current compiler
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Variables alignment                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#define _ALIGN_(x, decl)                        decl __attribute__((aligned(x)))

/*---------------------------------------------------------------------------------------------------------*/
/* Variables packing                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef _PACK_
#define _PACK_(decl)                            decl __attribute__((packed))
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Declare a variable as zero-initialized                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#define _ZI_(decl)                              decl __attribute__((zero_init))

/*---------------------------------------------------------------------------------------------------------*/
/* Declare a variable within a user-defined section                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#define _SECTION_(secname, decl)                decl __attribute__((section(secname)))

/*---------------------------------------------------------------------------------------------------------*/
/* Define a variable at a fixed memory address                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#define _AT_(addr, decl)                        decl __attribute__((at(addr)))

/*---------------------------------------------------------------------------------------------------------*/
/* Inform the compiler that the function does not return                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define _NORET_(decl)                           decl __attribute__ ((noreturn))

/*---------------------------------------------------------------------------------------------------------*/
/* Inform the compiler that the function does not return even if it reaches an explicit or implicit return */
/*---------------------------------------------------------------------------------------------------------*/
#define _NORET_SPEC_(decl)                      __declspec (noreturn) decl

/*---------------------------------------------------------------------------------------------------------*/
/* Call a function at addr                                                                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#if defined (__thumb__)
#define FUNC_CALL(addr)                         (((void (*)(void))(addr | 0x01))())
#else
#define FUNC_CALL(addr)                         (((void (*)(void))(addr | 0x00))())
#endif

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                    ARMCC windows compiler(s) support                                    */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#if defined (__ARMCC_VERSION)

    /*-----------------------------------------------------------------------------------------------------*/
    /* Interrrupt macros                                                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
    #define ENABLE_INTERRUPTS()             (void)__enable_irq()
    #define DISABLE_INTERRUPTS()            (void)__disable_irq()
    #define INTERRUPTS_SAVE_DISABLE(var)    { var = __disable_irq(); }
    #define INTERRUPTS_RESTORE(var)         { if (!var) { __enable_irq(); } }

    /*-----------------------------------------------------------------------------------------------------*/
    /* Assertion macros                                                                                    */
    /*-----------------------------------------------------------------------------------------------------*/
    #ifndef ASSERT
    #ifdef DEBUG
        #if defined (__TARGET_ARCH_4)   || (__TARGET_ARCH_4T)
            #define ASSERT(cond)    {if (!(cond)) for(;;) ;}
        #else
            #define ASSERT(cond)    {if (!(cond)) {__breakpoint(0);}}
        #endif
    #else
        #define ASSERT(cond)
    #endif
    #endif

    #if defined (__thumb__)
        /*-------------------------------------------------------------------------------------------------*/
        /*                                      THUMB INSTRUCTION SET                                      */
        /*                          Inline assembly instructions are NOT allowed                           */
        /*-------------------------------------------------------------------------------------------------*/

        /*-------------------------------------------------------------------------------------------------*/
        /* CPU power management                                                                            */
        /*-------------------------------------------------------------------------------------------------*/
        #if (__TARGET_ARCH_THUMB > 2)
            #define CPU_WAIT_FOR_EVENT()                __wfe()
            #define CPU_IDLE()                          __wfi()
        #endif

        /*-------------------------------------------------------------------------------------------------*/
        /* Cache macros                                                                                    */
        /*-------------------------------------------------------------------------------------------------*/
        #if (__TARGET_ARCH_THUMB > 3)
            /*---------------------------------------------------------------------------------------------*/
            /* Cache macros                                                                                */
            /*---------------------------------------------------------------------------------------------*/
            #define ICACHE_SAVE_DISABLE(var)    {                                                           \
                                                    UINT32 __armcc_temp_var;                                \
                                                    __asm{ MRC  p15, 0, var, c1, c0;                        \
                                                           BIC  __armcc_temp_var, var, 0x1000;              \
                                                           MCR  p15, 0, __armcc_temp_var, c1, c0; }         \
                                                }

            #define ICACHE_SAVE_ENABLE(var)     {                                                           \
                                                    UINT32 __armcc_temp_var;                                \
                                                    __asm{ MRC  p15, 0, var, c1, c0;                        \
                                                           ORR  __armcc_temp_var, var, 0x1000;              \
                                                           MCR  p15, 0, __armcc_temp_var, c1, c0; }         \
                                                }


            #define ICACHE_RESTORE(var)             __asm{ MCR  p15, 0, var, c1, c0 }
        #endif
    #else
        /*-------------------------------------------------------------------------------------------------*/
        /*                                       ARM INSTRUCTION SET                                       */
        /*                             Inline assemly instructions are allowed                             */
        /*-------------------------------------------------------------------------------------------------*/

        /*-------------------------------------------------------------------------------------------------*/
        /* CPU power management                                                                            */
        /*-------------------------------------------------------------------------------------------------*/
        #if defined (__TARGET_ARCH_4)   || (__TARGET_ARCH_4T) || (__TARGET_ARCH_5T) || (__TARGET_ARCH_5TE) || \
                    (__TARGET_ARCH_5TEJ)|| (__TARGET_ARCH_6)  || (__TARGET_ARCH_6_M)|| (__TARGET_ARCH_6S_M)|| \
                    (__TARGET_ARCH_6K)  || (__TARGET_ARCH_6T2)|| (__TARGET_ARCH_6Z) || (__ARM_ARCH_7J__)
            #define CPU_IDLE()          {                                                       \
                                            UINT32 __armcc_temp_var = 0;                        \
                                            __asm   {                                           \
                                                        MCR p15, 0, __armcc_temp_var, c7, c0, 4;\
                                                    }                                           \
                                        }
        #else

            #define CPU_WAIT_FOR_EVENT()    __wfe()
            #define CPU_IDLE()              __wfi()
        #endif

        /*-------------------------------------------------------------------------------------------------*/
        /* Cache macros                                                                                    */
        /*-------------------------------------------------------------------------------------------------*/
        #define ICACHE_SAVE_DISABLE(var)    {                                                   \
                                                UINT32 __armcc_temp_var;                        \
                                                __asm{ MRC  p15, 0, var, c1, c0;                \
                                                       BIC  __armcc_temp_var, var, 0x1000;      \
                                                       MCR  p15, 0, __armcc_temp_var, c1, c0; } \
                                            }

        #define ICACHE_SAVE_ENABLE(var)     {                                                   \
                                                UINT32 __armcc_temp_var;                        \
                                                __asm{ MRC  p15, 0, var, c1, c0;                \
                                                       ORR  __armcc_temp_var, var, 0x1000;      \
                                                       MCR  p15, 0, __armcc_temp_var, c1, c0; } \
                                            }


        #define ICACHE_RESTORE(var)         __asm{ MCR  p15, 0, var, c1, c0 }


        #define SET_CR(var)                 __asm{ MCR  p15, 0, var, c1, c0 }

        #define ISB()                       __asm{ ISB }
        #define DSB()                       __asm{ DSB }
        #define NOP()                       __asm{ NOP }

    #endif

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*                                     GNU GCC based compilers support                                     */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
#elif defined (__GNUC__)

    /*-----------------------------------------------------------------------------------------------------*/
    /* Assertion macros                                                                                    */
    /*-----------------------------------------------------------------------------------------------------*/
    #ifndef ASSERT
    #ifdef DEBUG
        #if defined (__ARM_ARCH_2__)   || (__ARM_ARCH_3__)   || (__ARM_ARCH_3M__)  || (__ARM_ARCH_4__)   || \
                    (__ARM_ARCH_4T__)
            #define ASSERT(cond)    {if (!(cond)) for(;;) ;}
        #else
            #define ASSERT(cond)    {if (!(cond))  {__asm   __volatile__ ( "BKPT #0" );} }
        #endif
    #else
        #define ASSERT(cond)
    #endif
    #endif

    #if defined (__thumb__)
        /*-------------------------------------------------------------------------------------------------*/
        /*                                      THUMB INSTRUCTION SET                                      */
        /*                          Some assembly instructions are not available                           */
        /*-------------------------------------------------------------------------------------------------*/

        /*-------------------------------------------------------------------------------------------------*/
        /* CPU power management                                                                            */
        /*-------------------------------------------------------------------------------------------------*/
        #if defined (__ARM_ARCH_2__)   || (__ARM_ARCH_3__)   || (__ARM_ARCH_3M__)  || (__ARM_ARCH_4__)   || \
                    (__ARM_ARCH_4T__)  || (__ARM_ARCH_5T__)  || (__ARM_ARCH_5TE__) || (__ARM_ARCH_5TEJ__)|| \
                    (__ARM_ARCH_6J__)  || (__ARM_ARCH_7J__)
            /*---------------------------------------------------------------------------------------------*/
            /* Not supported by architecture with Thumb instructions                                       */
            /*---------------------------------------------------------------------------------------------*/
            #define CPU_WAIT_FOR_EVENT()
            #define CPU_IDLE()

        #elif defined (__ARM_ARCH_6_M__) || (__ARM_ARCH_6__)  || (__ARM_ARCH_6S_M__) ||                     \
                      (__ARM_ARCH_6K__)  || (__ARM_ARCH_6T2__)

            /*---------------------------------------------------------------------------------------------*/
            /* Supported via CP15                                                                          */
            /*---------------------------------------------------------------------------------------------*/
            #define CPU_IDLE()                          __asm   __volatile__ (                              \
                                                                                "MOV r0, #0 \n\t"           \
                                                                                "MCR p15, 0, r0, c7, c0, 4" \
                                                                                ::: "r0", "cc" )
        #else
            /*---------------------------------------------------------------------------------------------*/
            /* Supported with WFI instruction                                                              */
            /*---------------------------------------------------------------------------------------------*/
            #define CPU_WAIT_FOR_EVENT()                __asm__ __volatile__("WFE")
            #define CPU_IDLE()                          __asm__ __volatile__("WFI")
        #endif


        /*-------------------------------------------------------------------------------------------------*/
        /* Interrrupt macros                                                                               */
        /*-------------------------------------------------------------------------------------------------*/
        #if defined (__ARM_ARCH_4__) || (__ARM_ARCH_4T__) || (__ARM_ARCH_5T__) || (__ARM_ARCH_5TE__) || (__ARM_ARCH_5TEJ__)

            /*---------------------------------------------------------------------------------------------*/
            /* Not supported with Thumb instructions                                                       */
            /*---------------------------------------------------------------------------------------------*/

        #elif defined (__ARM_ARCH_6J__) || (__ARM_ARCH_6ZK__)  || (__ARM_ARCH_7J__)
            /*---------------------------------------------------------------------------------------------*/
            /* Support for CPSIE instruction                                                               */
            /*---------------------------------------------------------------------------------------------*/
            #define ENABLE_INTERRUPTS()             __asm__ __volatile__("CPSIE if")
            #define DISABLE_INTERRUPTS()            __asm__ __volatile__("CPSID if")

            #define INTERRUPTS_SAVE_DISABLE(var)    {                                       \
                                                        (void)(var);                        \
                                                        DISABLE_INTERRUPTS();               \
                                                    }

            #define INTERRUPTS_RESTORE(var)         {                                       \
                                                        (void)(var);                        \
                                                        ENABLE_INTERRUPTS();                \
                                                    }
        #else
            /*---------------------------------------------------------------------------------------------*/
            /* Full assembly support with Thumb2                                                           */
            /*---------------------------------------------------------------------------------------------*/
            #define ENABLE_INTERRUPTS()       __asm__ __volatile__(                         \
                                                    "MRS    r6, APSR     \n\t"              \
                                                    "MOV    r5, #0x80    \n\t"              \
                                                    "BIC    r6,r6,r5     \n\t"              \
                                                    "MSR    APSR_nzcvq,r6  " ::: "r6", "r5", "cc" )

            #define DISABLE_INTERRUPTS()      __asm__ __volatile__(                         \
                                                    "MRS    r6, APSR     \n\t"              \
                                                    "MOV    r5, #0x80    \n\t"              \
                                                    "ORR    r6, r6,r5    \n\t"              \
                                                    "MSR    APSR_nzcvq, r6 " ::: "r6", "r5", "cc")

            #define INTERRUPTS_SAVE_DISABLE(var)  __asm__ __volatile__(                     \
                                                    "MRS    %[val], APSR     \n\t"          \
                                                    "MOV    r6, #0x80        \n\t"          \
                                                    "ORR    r6, %[val],r6    \n\t"          \
                                                    "MSR    APSR_nzcvq,r6  "                \
                                                    :[val] "+r" (var):: "r6", "cc")

            #define INTERRUPTS_RESTORE(var)   __asm__ __volatile__(                         \
                                                    "MSR    APSR_nzcvq,%[val]  "            \
                                                    : [val]"+r"(var) )
        #endif

        /*-------------------------------------------------------------------------------------------------*/
        /* Cache macros                                                                                    */
        /*-------------------------------------------------------------------------------------------------*/
        #if defined (__ARM_ARCH_2__)  || (__ARM_ARCH_3__)   || (__ARM_ARCH_3M__) || (__ARM_ARCH_4__)    ||  \
                    (__ARM_ARCH_4T__) || (__ARM_ARCH_5TE__) || (__ARM_ARCH_5T__) || (__ARM_ARCH_5TEJ__ )||  \
                    (__ARM_ARCH_6J__) || (__ARM_ARCH_6ZK__) || (__ARM_ARCH_6K__) || (__ARM_ARCH_6M__)  || (__ARM_ARCH_7J__)
            /*---------------------------------------------------------------------------------------------*/
            /* Not supported by architecture                                                               */
            /*---------------------------------------------------------------------------------------------*/
        #else
            #define ICACHE_SAVE_DISABLE(var)  __asm__ __volatile__(                         \
                                                    "MRC  p15, 0, %[val], c1, c0 \n\t"      \
                                                    "BIC  r6, %[val], #0x1000     \n\t"     \
                                                    "MCR  p15, 0, r6, c1, c0 "              \
                                                    :[val] "+r" (var):: "r6", "cc")

            #define ICACHE_SAVE_ENABLE(var)   __asm__ __volatile__(                         \
                                                    "MRC  p15, 0, %[val], c1, c0 \n\t"      \
                                                    "ORR  r6, %[val], #0x1000     \n\t"     \
                                                    "MCR  p15, 0, r6, c1, c0 "              \
                                                    :[val] "+r" (var):: "r6", "cc")

            #define ICACHE_RESTORE(var)       __asm__ __volatile__(                         \
                                                    "MCR  p15, 0, %[val], c1, c0"           \
                                                    : [val]"+r"(var) )
        #endif
    #else

        /*-------------------------------------------------------------------------------------------------*/
        /*                                       ARM INSTRUCTION SET                                       */
        /*-------------------------------------------------------------------------------------------------*/

        /*-------------------------------------------------------------------------------------------------*/
        /* CPU power management                                                                            */
        /*-------------------------------------------------------------------------------------------------*/
        #if defined (__ARM_ARCH_2__)   || (__ARM_ARCH_3__)   || (__ARM_ARCH_3M__)   || (__ARM_ARCH_4__)   ||\
                    (__ARM_ARCH_4T__)  || (__ARM_ARCH_5T__)  || (__ARM_ARCH_5TE__)  || (__ARM_ARCH_5TEJ__)||\
                    (__ARM_ARCH_6_M__) || (__ARM_ARCH_6__)   || (__ARM_ARCH_6S_M__) || (__ARM_ARCH_6J__)  ||\
                    (__ARM_ARCH_6T2__)  || (__ARM_ARCH_7J__)
            #define CPU_IDLE()                          __asm   __volatile__ (                              \
                                                                                "MOV r0, #0 \n\t"           \
                                                                                "MCR p15, 0, r0, c7, c0, 4" \
                                                                                ::: "r0", "cc" )
        #elif defined(__ARM_ARCH_8__)
		#define CPU_WAIT_FOR_EVENT()			    __asm   __volatile__ ( \
				"wait_loop_wfe_:  \n\t"  \
					 "wfe  \n\t"  \
					 "mov	x7, #0x13C  \n\t"  \
					 "movk	x7, #0xF080, lsl #0x10  \n\t"  \
					 "ldr   w8, [x7]  \n\t"  \
					 "cbz	w8, wait_loop_wfe  \n\t" 
		#define CPU_IDLE()                          __asm__ __volatile__("WFI")
					 

        #else
            #define CPU_WAIT_FOR_EVENT()                __asm__ __volatile__("WFE")
            #define CPU_IDLE()                          __asm__ __volatile__("WFI")
        #endif

        /*-------------------------------------------------------------------------------------------------*/
        /* Interrrupt macros                                                                               */
        /*-------------------------------------------------------------------------------------------------*/


#if defined (__ARM_ARCH_2__)   || (__ARM_ARCH_3__)   || (__ARM_ARCH_3M__)   || (__ARM_ARCH_4__)   ||\
							    (__ARM_ARCH_4T__)  || (__ARM_ARCH_5T__)  || (__ARM_ARCH_5TE__)  || (__ARM_ARCH_5TEJ__)||\
							    (__ARM_ARCH_6_M__) || (__ARM_ARCH_6__)   || (__ARM_ARCH_6S_M__) || (__ARM_ARCH_6J__)  ||\
							    (__ARM_ARCH_6T2__)	|| (__ARM_ARCH_7J__)


        #define ENABLE_INTERRUPTS()       __asm__ __volatile__(                         \
                                                "MRS    r6, APSR     \n\t"              \
                                                "BIC    r6,r6,#0x80  \n\t"              \
                                                "MSR    APSR_nzcvq,r6  " ::: "r6", "cc" )

        #define DISABLE_INTERRUPTS()      __asm__ __volatile__(                         \
                                                "MRS    r6, APSR     \n\t"              \
                                                "ORR    r6, r6,#0x80 \n\t"              \
                                                "MSR    APSR_nzcvq, r6 " ::: "r6", "cc")

        #define INTERRUPTS_SAVE_DISABLE(var)  __asm__ __volatile__(                     \
                                                "MRS    %[val], APSR     \n\t"          \
                                                "ORR    r6, %[val],#0x80 \n\t"          \
                                                "MSR    APSR_nzcvq,r6  "                \
                                                :[val] "+r" (var):: "r6", "cc")

        #define INTERRUPTS_RESTORE(var)   __asm__ __volatile__(                         \
                                                "MSR    APSR_nzcvq,%[val]  "            \
                                                : [val]"+r"(var) )


        /*-------------------------------------------------------------------------------------------------*/
        /* Cache macros                                                                                    */
        /*-------------------------------------------------------------------------------------------------*/
        #define ICACHE_SAVE_DISABLE(var)  __asm__ __volatile__(                         \
                                                "MRC  p15, 0, %[val], c1, c0 \n\t"      \
                                                "BIC  r6, %[val], #0x1000     \n\t"     \
                                                "MCR  p15, 0, r6, c1, c0 "              \
                                                :[val] "+r" (var):: "r6", "cc")

        #define ICACHE_SAVE_ENABLE(var)   __asm__ __volatile__(                         \
                                                "MRC  p15, 0, %[val], c1, c0 \n\t"      \
                                                "ORR  r6, %[val], #0x1000     \n\t"     \
                                                "MCR  p15, 0, r6, c1, c0 "              \
                                                :[val] "+r" (var):: "r6", "cc")

        #define ICACHE_RESTORE(var)       __asm__ __volatile__(                         \
                                                "MCR  p15, 0, %[val], c1, c0"           \
                                                : [val]"+r"(var) )

#elif defined(__ARM_ARCH_8__) // AARCH64

	#define ENABLE_INTERRUPTS()	  __asm__ __volatile__("msr DAIFClr, #0x2 ")


	#define DISABLE_INTERRUPTS()	  __asm__ __volatile__( 			\
					"MRS	x9, DAIF     \n\t"		\
					"ORR	x9, x9, #0xC0 \n\t"		\
					"MSR	DAIF, x9 " ::: "x9", "cc")

	#define INTERRUPTS_SAVE_DISABLE(var)  __asm__ __volatile__(			\
					"MRS	%[val], DAIF	 \n\t"		\
					"ORR	x9, %[val], #0xC0 \n\t"		\
					"MSR	DAIF,x9  "		\
					:[val] "+r" (var):: "x9", "cc")

	#define INTERRUPTS_RESTORE(var)   __asm__ __volatile__( 			\
					"MSR	DAIF, %[val]  "		\
					: [val]"+r"(var) )


	/*-------------------------------------------------------------------------------------------------*/
	/* Cache macros 										   */
	/*-------------------------------------------------------------------------------------------------*/
	#define ICACHE_SAVE_DISABLE(var)  __asm__ __volatile__( 			\
					"MRC  p15, 0, %[val], c1, c0 \n\t"	\
					"BIC  r6, %[val], #0x1000     \n\t"	\
					"MCR  p15, 0, r6, c1, c0 "		\
					:[val] "+r" (var):: "r6", "cc")

	#define ICACHE_SAVE_ENABLE(var)   __asm__ __volatile__( 			\
					"MRC  p15, 0, %[val], c1, c0 \n\t"	\
					"ORR  r6, %[val], #0x1000     \n\t"	\
					"MCR  p15, 0, r6, c1, c0 "		\
					:[val] "+r" (var):: "r6", "cc")

	#define ICACHE_RESTORE(var)	  __asm__ __volatile__( 			\
					"MCR  p15, 0, %[val], c1, c0"		\
					: [val]"+r"(var) )




#endif

    #endif

#else
    #warning Warning: x_INTERRUPTS and ICHACHE_x macros are not defined for current compiler
#endif


#endif /* _DEFS_ARM_H_ */
