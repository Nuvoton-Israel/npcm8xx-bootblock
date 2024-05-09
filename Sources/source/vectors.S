// ------------------------------------------------------------
// Armv8-A Vector tables
//
// Copyright (c) 2014-2016 Arm Limited (or its affiliates). All rights reserved.
// Use, modification and redistribution of this file is subject to your possession of a
// valid End User License Agreement for the Arm Product of which these examples are part of 
// and your compliance with all applicable terms and conditions of such licence agreement.
// ------------------------------------------------------------


    .global el1_vectors
    .global el2_vectors
    .global el3_vectors
    .global c0sync1
    .global irqHandler
    .global fiqHandler
    .global errHandler
    .global irqFirstLevelHandler
    .global fiqFirstLevelHandler
    .global l64serr3Handler
    .global l32serr3Handler
    .global start64

    .section  EL1VECTORS, "ax"
    .align 11

//
// Current EL with SP0
//
el1_vectors:
c0sync1: B start64 //    start64c0sync1

    .balign 0x80
c0irq1: B start64 //    irqFirstLevelHandler

    .balign 0x80
c0fiq1: B start64 //    fiqFirstLevelHandler

    .balign 0x80
c0serr1: B start64 //    c0serr1

//
// Current EL with SPx
//
    .balign 0x80
cxsync1: B start64 //    cxsync1

    .balign 0x80
cxirq1: B start64 //    irqFirstLevelHandler

    .balign 0x80
cxfiq1: B start64 //    fiqFirstLevelHandler

    .balign 0x80
cxserr1: B start64 //    cxserr1

//
// Lower EL using AArch64
//
    .balign 0x80
l64sync1: B start64 //    l64sync1

    .balign 0x80
l64irq1: B start64 //    irqFirstLevelHandler

    .balign 0x80
l64fiq1: B start64 //    fiqFirstLevelHandler

    .balign 0x80
l64serr1: B start64 //    l64serr1

//
// Lower EL using AArch32
//
    .balign 0x80
l32sync1: B start64 //    l32sync1

    .balign 0x80
l32irq1: B start64 //    irqFirstLevelHandler

    .balign 0x80
l32fiq1: B start64 //    fiqFirstLevelHandler

    .balign 0x80
l32serr1: B start64 //    l32serr1

//----------------------------------------------------------------

    .section  EL2VECTORS, "ax"
    .align 11

//
// Current EL with SP0
//
el2_vectors:
c0sync2: B start64 //    c0sync2

    .balign 0x80
c0irq2: B start64 //    irqFirstLevelHandler

    .balign 0x80
c0fiq2: B start64 //    fiqFirstLevelHandler

    .balign 0x80
c0serr2: B start64 //    c0serr2

//
// Current EL with SPx
//
    .balign 0x80
cxsync2: B start64 //    cxsync2

    .balign 0x80
cxirq2: B start64 //    irqFirstLevelHandler

    .balign 0x80
cxfiq2: B start64 //    fiqFirstLevelHandler

    .balign 0x80
cxserr2: B start64 //    cxserr2

//
// Lower EL using AArch64
//
    .balign 0x80
l64sync2: B start64 //    l64sync2

    .balign 0x80
l64irq2: B start64 //    irqFirstLevelHandler

    .balign 0x80
l64fiq2: B start64 //    fiqFirstLevelHandler

    .balign 0x80
l64serr2: B start64 //    l64serr2

//
// Lower EL using AArch32
//
    .balign 0x80
l32sync2: B start64 //    l32sync2

    .balign 0x80
l32irq2: B start64 //    irqFirstLevelHandler

    .balign 0x80
l32fiq2: B start64 //    fiqFirstLevelHandler

    .balign 0x80
l32serr2: B start64 //    l32serr2

//----------------------------------------------------------------

    .section  EL3VECTORS, "ax"
    .align 11

//
// Current EL with SP0
//
el3_vectors:
c0sync3: BL l64serr3Handler
	B start64 //    c0sync3

    .balign 0x80
c0irq3: BL irqFirstLevelHandler
        B start64

    .balign 0x80
c0fiq3: BL fiqFirstLevelHandler
        B start64

    .balign 0x80
c0serr3: BL l64serr3Handler
         B start64

//
// Current EL with SPx
//
    .balign 0x80
cxsync3: BL l64serr3Handler
         B start64 //    cxsync3

    .balign 0x80
cxirq3: BL irqFirstLevelHandler
        B start64

    .balign 0x80
cxfiq3: BL fiqFirstLevelHandler
        B start64

    .balign 0x80
cxserr3: BL l64serr3Handler   // cxserr3
         B start64

//
// Lower EL using AArch64
//
    .balign 0x80
l64sync3: BL l64serr3Handler
          B start64 //    l64sync3

    .balign 0x80
l64irq3: BL irqFirstLevelHandler
         B start64

    .balign 0x80
l64fiq3: BL fiqFirstLevelHandler
         B start64

    .balign 0x80
l64serr3: BL l64serr3Handler
          B start64

//
// Lower EL using AArch32
//
    .balign 0x80
l32sync3: B start64 //    l32sync3  TODO: what happens if swiched to 32 bits?

    .balign 0x80
l32irq3: BL irqFirstLevelHandler
         B start64

    .balign 0x80
l32fiq3: BL fiqFirstLevelHandler
         B start64

    .balign 0x80
l32serr3: BL l32serr3Handler
          B start64


    .section InterruptHandlers, "ax"
    .balign 4

    .type irqFirstLevelHandler, "function"
irqFirstLevelHandler:
   bl GetCPUID
   cbz x0,   irqHandler
   b start64
   ERET

    .type fiqFirstLevelHandler, "function"
fiqFirstLevelHandler:
    bl GetCPUID
    cbz x0,   fiqHandler
    b start64  
    ERET

.type l64serr3Handler, "function"
l64serr3Handler:
   bl GetCPUID
   cbz x0,  errHandler
   b start64
   ERET

.type l32serr3Handler, "function"
l32serr3Handler:
  bl GetCPUID
  cbz x0,  errHandler
  b start64
  ERET
