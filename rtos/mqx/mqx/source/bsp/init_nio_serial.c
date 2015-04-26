/*HEADER**********************************************************************
*
* Copyright 20013 Freescale Semiconductor, Inc.
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
*   This file contains the definition for the baud rate for the serial
*   channel
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "nio_serial.h"
#include "fsl_uart_driver.h"
#if PLATFORM_LPUART_ENABLED
#include <fsl_lpuart_driver.h>
#endif

extern void UART0_RX_TX_IRQHandler(void);
extern void UART1_RX_TX_IRQHandler(void);
extern void UART2_RX_TX_IRQHandler(void);
extern void UART3_RX_TX_IRQHandler(void);
extern void UART4_RX_TX_IRQHandler(void);
extern void UART5_RX_TX_IRQHandler(void);
extern void UART6_RX_TX_IRQHandler(void);
extern void UART7_RX_TX_IRQHandler(void);

extern void LPUART0_IRQHandler(void);
extern void LPUART1_IRQHandler(void);
extern void LPUART2_IRQHandler(void);
extern void LPUART3_IRQHandler(void);
extern void LPUART4_IRQHandler(void);
extern void LPUART5_IRQHandler(void);
extern void LPUART6_IRQHandler(void);
extern void LPUART7_IRQHandler(void);

#if ((0 < HW_UART_INSTANCE_COUNT) || ((0 < HW_LPUART_INSTANCE_COUNT) && PLATFORM_LPUART0_ENABLED))
const NIO_SERIAL_INIT_DATA_STRUCT _bsp_nserial0_init =
{
        .UART_INSTANCE       = 0,
        .BAUDRATE            = 115200,
        .PARITY_MODE         = kUartParityDisabled,
        .STOPBIT_COUNT       = kUartOneStopBit,
        .BITCOUNT_PERCHAR    = kUart8BitsPerChar,
        #if PLATFORM_LPUART0_ENABLED
        .RXTX_PRIOR          = 2,
        .handler             = LPUART0_IRQHandler,
        #else
        .RXTX_PRIOR          = 4,
        .handler             = UART0_RX_TX_IRQHandler,
        #endif
};
#endif

#if ((1 < HW_UART_INSTANCE_COUNT) || ((1 < HW_LPUART_INSTANCE_COUNT) && PLATFORM_LPUART1_ENABLED))
const NIO_SERIAL_INIT_DATA_STRUCT _bsp_nserial1_init =
{
        .UART_INSTANCE       = 1,
        .BAUDRATE            = 115200,
        .PARITY_MODE         = kUartParityDisabled,
        .STOPBIT_COUNT       = kUartOneStopBit,
        .BITCOUNT_PERCHAR    = kUart8BitsPerChar,
        #if PLATFORM_LPUART1_ENABLED
        .RXTX_PRIOR          = 2,
        .handler             = LPUART1_IRQHandler,
        #else
        .RXTX_PRIOR          = 4,
        .handler             = UART1_RX_TX_IRQHandler,
        #endif
};
#endif

#if ((2 < HW_UART_INSTANCE_COUNT) || ((2 < HW_LPUART_INSTANCE_COUNT) && PLATFORM_LPUART2_ENABLED))
const NIO_SERIAL_INIT_DATA_STRUCT _bsp_nserial2_init =
{
        .UART_INSTANCE       = 2,
        .BAUDRATE            = 115200,
        .PARITY_MODE         = kUartParityDisabled,
        .STOPBIT_COUNT       = kUartOneStopBit,
        .BITCOUNT_PERCHAR    = kUart8BitsPerChar,
        .RXTX_PRIOR          = 4,
        #if PLATFORM_LPUART2_ENABLED
        .RXTX_PRIOR          = 2,
        .handler             = LPUART2_IRQHandler,
        #else
        .RXTX_PRIOR          = 4,
        .handler             = UART2_RX_TX_IRQHandler,
        #endif
};
#endif

#if ((3 < HW_UART_INSTANCE_COUNT) || ((3 < HW_LPUART_INSTANCE_COUNT) && PLATFORM_LPUART3_ENABLED))
const NIO_SERIAL_INIT_DATA_STRUCT _bsp_nserial3_init =
{
        .UART_INSTANCE       = 3,
        .BAUDRATE            = 115200,
        .PARITY_MODE         = kUartParityDisabled,
        .STOPBIT_COUNT       = kUartOneStopBit,
        .BITCOUNT_PERCHAR    = kUart8BitsPerChar,
        .RXTX_PRIOR          = 4,
        #if PLATFORM_LPUART3_ENABLED
        .RXTX_PRIOR          = 2,
        .handler             = LPUART3_IRQHandler,
        #else
        .RXTX_PRIOR          = 4,
        .handler             = UART3_RX_TX_IRQHandler,
        #endif
};
#endif

#if ((4 < HW_UART_INSTANCE_COUNT) || ((4 < HW_LPUART_INSTANCE_COUNT) && PLATFORM_LPUART4_ENABLED))
const NIO_SERIAL_INIT_DATA_STRUCT _bsp_nserial4_init =
{
        .UART_INSTANCE       = 4,
        .BAUDRATE            = 115200,
        .PARITY_MODE         = kUartParityDisabled,
        .STOPBIT_COUNT       = kUartOneStopBit,
        .BITCOUNT_PERCHAR    = kUart8BitsPerChar,
        .RXTX_PRIOR          = 4,
        #if PLATFORM_LPUART4_ENABLED
        .RXTX_PRIOR          = 2,
        .handler             = LPUART4_IRQHandler,
        #else
        .RXTX_PRIOR          = 4,
        .handler             = UART4_RX_TX_IRQHandler,
        #endif
};
#endif

#if ((5 < HW_UART_INSTANCE_COUNT) || ((5 < HW_LPUART_INSTANCE_COUNT) && PLATFORM_LPUART5_ENABLED))
const NIO_SERIAL_INIT_DATA_STRUCT _bsp_nserial5_init =
{
        .UART_INSTANCE       = 5,
        .BAUDRATE            = 115200,
        .PARITY_MODE         = kUartParityDisabled,
        .STOPBIT_COUNT       = kUartOneStopBit,
        .BITCOUNT_PERCHAR    = kUart8BitsPerChar,
        .RXTX_PRIOR          = 4,
        #if PLATFORM_LPUART5_ENABLED
        .RXTX_PRIOR          = 2,
        .handler             = LPUART5_IRQHandler,
        #else
        .RXTX_PRIOR          = 4,
        .handler             = UART5_RX_TX_IRQHandler,
        #endif
};
#endif

#if ((6 < HW_UART_INSTANCE_COUNT) || ((6 < HW_LPUART_INSTANCE_COUNT) && PLATFORM_LPUART6_ENABLED))
const NIO_SERIAL_INIT_DATA_STRUCT _bsp_nserial6_init =
{
        .UART_INSTANCE       = 6,
        .BAUDRATE            = 115200,
        .PARITY_MODE         = kUartParityDisabled,
        .STOPBIT_COUNT       = kUartOneStopBit,
        .BITCOUNT_PERCHAR    = kUart8BitsPerChar,
        .RXTX_PRIOR          = 4,
        #if PLATFORM_LPUART6_ENABLED
        .RXTX_PRIOR          = 2,
        .handler             = LPUART6_IRQHandler,
        #else
        .RXTX_PRIOR          = 4,
        .handler             = UART6_RX_TX_IRQHandler,
        #endif
};
#endif

#if ((7 < HW_UART_INSTANCE_COUNT) || ((7 < HW_LPUART_INSTANCE_COUNT) && PLATFORM_LPUART7_ENABLED))
const NIO_SERIAL_INIT_DATA_STRUCT _bsp_nserial7_init =
{
        .UART_INSTANCE       = 7,
        .BAUDRATE            = 115200,
        .PARITY_MODE         = kUartParityDisabled,
        .STOPBIT_COUNT       = kUartOneStopBit,
        .BITCOUNT_PERCHAR    = kUart8BitsPerChar,
        .RXTX_PRIOR          = 4,
        #if PLATFORM_LPUART7_ENABLED
        .RXTX_PRIOR          = 2,
        .handler             = LPUART7_IRQHandler,
        #else
        .RXTX_PRIOR          = 4,
        .handler             = UART7_RX_TX_IRQHandler,
        #endif
};
#endif
