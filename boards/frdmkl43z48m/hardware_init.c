/*
 * Copyright (c) 2013-2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_clock_manager.h"
#include "fsl_os_abstraction.h"
#include "fsl_debug_console.h"
#include "fsl_sim_hal.h"
#include "fsl_lpuart_hal.h"
#include "fsl_lpuart_common.h"
#include "fsl_lpuart_driver.h"
#include "pin_mux.h"

#include "BlueTooth.h"
#include "EcgCapture.h"



static lpuart_state_t lpuartState[2];
//static lpuart_state_t s_bt_lpuart;

void hardware_init(void)
{
    int32_t i;

    /* enable clock for PORTs */
    for (i = 0; i < HW_PORT_INSTANCE_COUNT; i++)
    {
        CLOCK_SYS_EnablePortClock(i);
    }

#if (CLOCK_SETUP == 0)   
    CLOCK_SYS_SetSource(kClockLpuart0Src, 3);
	CLOCK_SYS_SetSource(kClockLpuart1Src, 3);

#else
    CLOCK_SYS_SetSource(kClockLpuart0Src, 1);//
    CLOCK_SYS_SetSource(kClockLpuart1Src, 1);
#endif
    
    for (i = 0; i < HW_PORT_INSTANCE_COUNT; i++)
    {
        configure_gpio_pins(i);
    }

  //  configure_tpm_pins(0);  //定时器/脉宽调制模块
    configure_lpuart_pins(BOARD_DEBUG_UART_INSTANCE);
	configure_lpuart_pins(BOARD_BT_UART_INSTANCE);
	
    GPIO_DRV_Init(statusPins,controlPins);
    
    LED1_EN;
    LED1_OFF;
    
    CHARG_EN;
}

void dbg_uart_init(void)
{
    configure_lpuart_pins(BOARD_DEBUG_UART_INSTANCE);
    lpuart_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUD);
}

void BT_uart_init(void)
{
    configure_lpuart_pins(BOARD_BT_UART_INSTANCE);
    lpuart_Init(BOARD_BT_UART_INSTANCE, BOARD_BT_UART_BAUD);
}


