/*HEADER**********************************************************************
*
* Copyright 2010-2013 Freescale Semiconductor, Inc.
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale MQX RTOS License
* distributed with this Material.
* See the MQX_RTOS_LICENSE file distributed for more details.
*
* Brief License Summary:
* This software is provided in source form for you to use free of charge,
* but it is not open source software. You are allowed to use this software
* but you cannot redistribute it or derivative works of it in source form.
* The software may be used only in connection with a product containing
* a Freescale microprocessor, microcontroller, or digital signal processor.
* See license agreement file for full license terms including other
* restrictions.
*****************************************************************************
*
* Comments:
*

*
*END************************************************************************/

#include "asm_mac.h"
#include "mqx_cnfg.h"
#include "types.inc"
#include "psp_prv.inc"

#define __ASM__

#ifdef __CWARM__
#include "kinetis.h"
#else
#include "psp_cpu.h"
#endif /* __CWARM__ */

#undef __ASM__

    ASM_COMP_SPECIFIC_DIRECTIVES

    ASM_CODE_SECTION(.text)

    SET_FUNCTION_ALIGNMENT

    ASM_PUBLIC(__boot)

/*FUNCTION*-------------------------------------------------------------------

 Function Name    : __boot
 Returned Value   :
 Comments         : startup sequence

 END*-----------------------------------------------------------------------*/

ASM_EQUATE(NVIC_ICER, 0xE000E180)
ASM_EQUATE(NVIC_ICPR, 0xE000E280)


    ASM_PUBLIC_BEGIN(__boot)
    ASM_PUBLIC_FUNC(__boot)
ASM_LABEL(__boot)
        /* Disable interrupts and clear pending flags */
        ldr r0, =0xFFFFFFFF
        /* NVIC_ICER - Interrupt Clear-Enable Register */
        ldr r1, =NVIC_ICER
        str r0, [r1]
        /* NVIC_ICPR - Interrupt Clear-Pending Register */
        ldr r1, =NVIC_ICPR
        str r0, [r1]

        /* Prepare process stack pointer */
        mrs r0, MSP
        msr PSP, r0

        /* Switch to proccess stack (PSP) */
        mrs r0, CONTROL
        movs r1, #0x2
        orrs r0, r0, r1
        msr CONTROL, r0
        isb

        /* Perform toolchain startup routines */
        ASM_EXTERN(toolchain_startup)
        bl ASM_PREFIX(toolchain_startup)

    ASM_PUBLIC_END(__boot)

        ASM_ALIGN(4)
        ASM_END
