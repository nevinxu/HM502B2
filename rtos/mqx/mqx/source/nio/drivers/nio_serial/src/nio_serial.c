/*HEADER**********************************************************************
 *
 * Copyright 2013 Freescale Semiconductor, Inc.
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
 *
 *END************************************************************************/

#include <stdarg.h>
#include <stdint.h>
#include <assert.h>
#include <fsl_os_abstraction.h>
#include "nio_serial.h"
#include "nio.h"
#include "ioctl.h"
#include "errno.h"

#if defined ( __IAR_SYSTEMS_ICC__ )
/* MISRA C 2004 rule 20.5 suppress: Error[Pm101]: The error indicator errno shall not be used */
_Pragma ("diag_suppress= Pm101")
#endif

#ifdef HW_UART_INSTANCE_COUNT
extern const IRQn_Type g_uartRxTxIrqId[HW_UART_INSTANCE_COUNT];
#endif

#ifdef HW_LPUART_INSTANCE_COUNT
extern const IRQn_Type g_lpuartRxTxIrqId[HW_LPUART_INSTANCE_COUNT];
#endif

static int nio_serial_open(void *dev_context, const char *dev_name, int flags, void **fp_context);
static int nio_serial_read(void *dev_context, void *fp_context, void *buf, size_t nbytes);
static int nio_serial_write(void *dev_context, void *fp_context, const void *buf, size_t nbytes);
static off_t nio_serial_lseek(void *dev_context, void *fp_context, off_t offset, int whence);
static int nio_serial_ioctl(void *dev_context, void *fp_context, unsigned long int request, va_list ap);
static int nio_serial_close(void *dev_context, void *fp_context);
static int nio_serial_init(void *init_data, void **dev_context);
static int nio_serial_deinit(void *dev_context);

const NIO_DEV_FN_STRUCT nio_serial_dev_fn =
{
    .OPEN = nio_serial_open,
    .READ = nio_serial_read,
    .WRITE = nio_serial_write,
    .LSEEK = nio_serial_lseek,
    .IOCTL = nio_serial_ioctl,
    .CLOSE = nio_serial_close,
    .INIT = nio_serial_init,
    .DEINIT = nio_serial_deinit,
};

typedef struct
{
    #if PLATFORM_LPUART_ENABLED
    lpuart_state_t uart_state;
    #else
    uart_state_t uart_state;
    #endif
    uint32_t instance;
} NIO_SERIAL_DEV_CONTEXT_STRUCT;

typedef struct
{
    size_t rcnt;
    size_t wcnt;
} NIO_SERIAL_FP_CONTEXT_STRUCT;

static int nio_serial_open(void *dev_context, const char *dev_name, int flags, void **fp_context)
{
    return 0;
}

static int nio_serial_read(void *dev_context, void *fp_context, void *buf, size_t nbytes)
{

    NIO_SERIAL_DEV_CONTEXT_STRUCT *serial_dev_context = (NIO_SERIAL_DEV_CONTEXT_STRUCT *)dev_context;
    #if PLATFORM_LPUART_ENABLED
    if (kStatus_UART_Success != LPUART_DRV_ReceiveDataBlocking(serial_dev_context->instance, buf, nbytes, OSA_WAIT_FOREVER))
    #else
    if (kStatus_UART_Success != UART_DRV_ReceiveDataBlocking(serial_dev_context->instance, buf, nbytes, OSA_WAIT_FOREVER))
    #endif
    {
        errno = EIO;
    }
    return (nbytes - serial_dev_context->uart_state.rxSize);
}

static int nio_serial_write(void *dev_context, void *fp_context, const void *buf, size_t nbytes)
{

    NIO_SERIAL_DEV_CONTEXT_STRUCT *serial_dev_context = (NIO_SERIAL_DEV_CONTEXT_STRUCT *)dev_context;
    #if PLATFORM_LPUART_ENABLED
    if (kStatus_UART_Success != LPUART_DRV_SendDataBlocking(serial_dev_context->instance, buf, nbytes, OSA_WAIT_FOREVER))
    #else
    if (kStatus_UART_Success != UART_DRV_SendDataBlocking(serial_dev_context->instance, buf, nbytes, OSA_WAIT_FOREVER))
    #endif
    {
        errno = EIO;
    }

    return (nbytes - serial_dev_context->uart_state.txSize);
}

static off_t nio_serial_lseek(void *dev_context, void *fp_context, off_t offset, int whence)
{
    return 0;
}

static int nio_serial_ioctl(void *dev_context, void *fp_context, unsigned long int request, va_list ap) {

    NIO_SERIAL_DEV_CONTEXT_STRUCT *serial_dev_context = (NIO_SERIAL_DEV_CONTEXT_STRUCT *)dev_context;

    switch (request) {
    case IOCTL_ABORT:
        #if PLATFORM_LPUART_ENABLED
        LPUART_DRV_AbortSendingData(serial_dev_context->instance);
        LPUART_DRV_AbortReceivingData(serial_dev_context->instance);
        #else
        UART_DRV_AbortSendingData(serial_dev_context->instance);
        UART_DRV_AbortReceivingData(serial_dev_context->instance);
        #endif
        break;
    default:
        break;
    }

    return 0;
}

static int nio_serial_close(void *dev_context, void *fp_context)
{
    return 0;
}

static int nio_serial_init(void *init_data, void **dev_context)
{
    osa_status_t status;

    NIO_SERIAL_DEV_CONTEXT_STRUCT *serial_dev_context = (NIO_SERIAL_DEV_CONTEXT_STRUCT *)dev_context;

    NIO_SERIAL_INIT_DATA_STRUCT *init = (NIO_SERIAL_INIT_DATA_STRUCT*)init_data;

    #if PLATFORM_LPUART_ENABLED

    assert(init->UART_INSTANCE < HW_LPUART_INSTANCE_COUNT);

    lpuart_user_config_t uartConfig =
    #else
    assert(init->UART_INSTANCE < HW_UART_INSTANCE_COUNT);

    uart_user_config_t uartConfig =
    #endif
    {
        .baudRate = init->BAUDRATE,
        .parityMode = init->PARITY_MODE,
        .stopBitCount = init->STOPBIT_COUNT,
        .bitCountPerChar = init->BITCOUNT_PERCHAR,
    };

    serial_dev_context = (NIO_SERIAL_DEV_CONTEXT_STRUCT*) OSA_MemAlloc(sizeof(NIO_SERIAL_DEV_CONTEXT_STRUCT));

    /* SDK HAL init */
    #if PLATFORM_LPUART_ENABLED
    if ( kStatus_UART_Success != LPUART_DRV_Init(init->UART_INSTANCE, &serial_dev_context->uart_state, &uartConfig))
    {
        errno = ENXIO;
    }

    /* LPUART handler interrupt installation */
    status = OSA_InstallIntHandler(g_lpuartRxTxIrqId[init->UART_INSTANCE], init->handler);
    if (kStatus_OSA_Success != status)
    {
      errno = ENXIO;
    }
    NVIC_SetPriority(g_lpuartRxTxIrqId[init->UART_INSTANCE], init->RXTX_PRIOR);
    NVIC_EnableIRQ(g_lpuartRxTxIrqId[init->UART_INSTANCE]);

    #else
    if ( kStatus_UART_Success != UART_DRV_Init(init->UART_INSTANCE, &serial_dev_context->uart_state, &uartConfig))
    {
        errno = ENXIO;
    }

    /* UART handler interrupt installation */
    status = OSA_InstallIntHandler(g_uartRxTxIrqId[init->UART_INSTANCE], init->handler);
    if (kStatus_OSA_Success != status)
    {
      errno = ENXIO;
    }
    NVIC_SetPriority(g_uartRxTxIrqId[init->UART_INSTANCE], init->RXTX_PRIOR);
    NVIC_EnableIRQ(g_uartRxTxIrqId[init->UART_INSTANCE]);
    #endif
    serial_dev_context->instance = init->UART_INSTANCE;
    *dev_context = (void*)serial_dev_context;
    return 0;
}

static int nio_serial_deinit(void *dev_context)
{
    NIO_SERIAL_DEV_CONTEXT_STRUCT *serial_dev_context = (NIO_SERIAL_DEV_CONTEXT_STRUCT *)dev_context;
    #if PLATFORM_LPUART_ENABLED
    LPUART_DRV_Deinit(serial_dev_context->instance);
    #else
    UART_DRV_Deinit(serial_dev_context->instance);
    #endif
    OSA_MemFree(dev_context);
    return 0;
}
