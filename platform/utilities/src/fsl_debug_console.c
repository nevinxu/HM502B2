/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "fsl_debug_console.h"
#include "fsl_clock_manager.h"
#include "fsl_os_abstraction.h"
#if defined(HW_LPUART_INSTANCE_COUNT)
#include "fsl_lpuart_hal.h"
#include "fsl_lpuart_common.h"
#include "fsl_lpuart_driver.h"
#endif
#if defined(HW_UART_INSTANCE_COUNT)
#include "fsl_uart_hal.h"
#include "fsl_uart_common.h"
#include "fsl_uart_driver.h"
#endif


#if __ICCARM__
#include <yfuns.h>
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Operation functions definiations for debug console. */
typedef struct DebugConsoleOperationFunctions {
   uint32_t (* Send)(uint32_t instance, const uint8_t *buf, uint32_t count, uint32_t timeout);
   uint32_t (* Receive)(uint32_t instance, uint8_t *buf, uint32_t count, uint32_t timeout);
} debug_console_ops_t;

/*! @brief State structure storing debug console. */
typedef struct DebugConsoleState {
    debug_console_device_type_t type;/*<! Indicator telling whether the debug console is inited. */
    union {
#if defined(HW_UART_INSTANCE_COUNT)
        uart_state_t uartState;
#endif
#if defined(HW_LPUART_INSTANCE_COUNT)
        lpuart_state_t lpuartState;
#endif
    } state;                        /*<! State structure for the hardware uart state. */
    uint8_t instance;               /*<! Instance number indicator. */
    debug_console_ops_t ops;        /*<! Operation function pointers for debug uart operations. */
} debug_console_state_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Debug UART state information.*/
static debug_console_state_t s_debugConsole;


lpuart_status_t pclpuart_receive_isr_callback(uint8_t * rxByte, void * param)
{
    return kStatus_LPUART_Success;
}


/* See fsl_debug_console.h for documentation of this function.*/
debug_console_status_t DbgConsole_Init(
        uint32_t uartInstance, uint32_t baudRate, debug_console_device_type_t device)
{
    if (s_debugConsole.type != kDebugConsoleNone)
    {
        return kStatus_DEBUGCONSOLE_Failed;
    }

    /* Set debug console to initialized to avoid duplicated init operation.*/
    s_debugConsole.type = device;

                /* Declare config sturcuture to initialize a uart instance. */
                lpuart_user_config_t lpuartConfig;
                lpuart_status_t status;

                /* Config the structure. */
                lpuartConfig.baudRate = baudRate;
                lpuartConfig.bitCountPerChar = kLpuart8BitsPerChar;
                lpuartConfig.parityMode = kLpuartParityDisabled;
                lpuartConfig.stopBitCount = kLpuartOneStopBit;

                /* Init LPUART device. */
                status =
                    LPUART_DRV_Init(uartInstance, &s_debugConsole.state.lpuartState, &lpuartConfig);
                if ((status != kStatus_LPUART_Success)&&(status != kStatus_LPUART_Initialized))
                {
                    s_debugConsole.type = kDebugConsoleNone;
                    return kStatus_DEBUGCONSOLE_Failed;
                }

                /* Set the funciton pointer for send and receive for this kind of device. */
               // s_debugConsole.ops.Send = LPUART_DRV_SendPollBlocking;
               // s_debugConsole.ops.Receive = LPUART_DRV_ReceivePollBlocking;
               // s_debugConsole.state.lpuartState.rxCallback = (lpuart_rx_callback_t)pclpuart_receive_isr_callback;
               // LPUART_DRV_InstallRxCallback(uartInstance,pclpuart_receive_isr_callback,0,true);
               // LPUART_DRV_ReceiveData(uartInstance,&debegrxbuf,1);


    /* Configure the s_debugConsole structure only when the inti operation is successful. */
    s_debugConsole.instance = uartInstance;

    return kStatus_DEBUGCONSOLE_Success;
}
/* See fsl_debug_console.h for documentation of this function.*/
debug_console_status_t DbgConsole_DeInit(void)
{
    if (s_debugConsole.type == kDebugConsoleNone)
    {
        return kStatus_DEBUGCONSOLE_Success;
    }

    switch(s_debugConsole.type)
    {
#if defined(HW_UART_INSTANCE_COUNT)
        case kDebugConsoleUART:
            UART_DRV_Deinit(s_debugConsole.instance);
            break;
#endif
#if defined(HW_LPUART_INSTANCE_COUNT)
        case kDebugConsoleLPUART:
                LPUART_DRV_Deinit(s_debugConsole.instance);
            break;
#endif
        default:
            return kStatus_DEBUGCONSOLE_InvalidDevice;
    }

    s_debugConsole.type = kDebugConsoleNone;

    return kStatus_DEBUGCONSOLE_Success;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
