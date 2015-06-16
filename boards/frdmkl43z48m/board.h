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

#if !defined(__BOARD_H__)
#define __BOARD_H__

#include <stdint.h>

#include "gpio_pins.h"
#include "pin_mux.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
/* The board name */
#define BOARD_NAME                      "HM502B2"
/*! The LPUART to use for debug messages. */
#define BOARD_DEBUG_UART_INSTANCE  0
#define BOARD_DEBUG_UART_BASEADDR   LPUART0_BASE
#define BOARD_DEBUG_UART_BAUD       9600

#define BOARD_BT_UART_INSTANCE   1
#define BOARD_BT_UART_BASEADDR   LPUART1_BASE
#define BOARD_BT_UART_BAUD       115200

/* board led color mapping */
#define BOARD_GPIO_LED_GREEN              kGpioLED1
     
        
#define LED1_EN (PORT_HAL_SetMuxMode(PORTE_BASE, 29, kPortMuxAsGpio)) 		/*!< Enable target LED0 */       
#define LED1_DIS (PORT_HAL_SetMuxMode(PORTE_BASE,29, kPortPinDisabled)) 	/*!< Disable target LED0 */
    
#define CHARG_EN (PORT_HAL_SetMuxMode(PORTE_BASE, 25, kPortMuxAsGpio),GPIO_DRV_WritePinOutput(kGpioCHARG_EN, 0)) 		/*!< Enable target LED0 */       
#define CHARG_DIS (PORT_HAL_SetMuxMode(PORTE_BASE,25, kPortPinDisabled)) 	/*!< Disable target LED0 */    

#define LED1_OFF (GPIO_DRV_WritePinOutput(kGpioLED1, 1))       	/*!< Turn off target LED0 */
#define LED1_ON (GPIO_DRV_WritePinOutput(kGpioLED1, 0))        	/*!< Turn on target LED0 */


/******************************************************************************
 * Externs
 ******************************************************************************/

/******************************************************************************
 * Prototypes
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void hardware_init(void);
void dbg_uart_init(void);
void BT_uart_init(void);


#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __BOARD_H__ */
/******************************************************************************
 * EOF
 ******************************************************************************/
