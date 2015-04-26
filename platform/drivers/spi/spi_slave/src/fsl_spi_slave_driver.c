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

#include <assert.h>
#include <string.h>
#include "fsl_device_registers.h"
#include "fsl_spi_slave_driver.h"
#include "fsl_spi_hal.h"
#include "fsl_spi_shared_function.h"
#include "fsl_spi_common.h"
#include "fsl_clock_manager.h"
#include "fsl_interrupt_manager.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! SPI slave constants*/
enum _spi_slave_constants
{
    kEmptyChar = 0,                        /*!< Empty character*/
};

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief SPI Slave Generic IRQ handler.
 *
 * @param instance Instance number of the SPI module.
 */
void SPI_DRV_SlaveIRQHandler(uint32_t instance)
{
    /* instantiate local variable of type spi_slave_state_t and equate it to the
     * pointer to state
     */
    spi_slave_state_t * spiState = (spi_slave_state_t *)g_spiStatePtr[instance];
    uint32_t baseAddr = g_spiBaseAddr[instance];

    /* Get the callback pointers from the run-time state structure */
    spi_slave_callbacks_t * callbacks = &spiState->callbacks;

    /* Service receive first since TX shift register is still shifting out data, we have
     * time before placing data into the TX data buffer.
     */
#if FSL_FEATURE_SPI_16BIT_TRANSFERS
    /* Architectural note: When developing the IRQ FIFO handling functionality, it was found that to
     * achieve more efficient run-time performance, it was better to first check the bits/frame
     * setting and then proceed with the FIFO fill/drain management process, rather than to clutter
     * the FIFO fill/drain process with continual checks of the bits/frame setting.
     */
    if (SPI_HAL_Get8or16BitMode(baseAddr) == kSpi16BitMode)
    {
        /* If SPI module supports FIFO */
        if (FSL_FEATURE_SPI_FIFO_SIZEn(instance) != 0)
        {
            /* Receive, 16-bit with FIFO */
            /* Do this while the RX FIFO is not empty */
            while (SPI_HAL_GetFifoStatusFlag(baseAddr, kSpiRxFifoEmpty) == 0)
            {
                /* Read the bytes from the RX FIFO */
                uint8_t rdHi = SPI_HAL_ReadDataHigh(baseAddr);
                uint8_t rdLo = SPI_HAL_ReadDataLow(baseAddr);

                /* Sink the first 8-bits */
                if (callbacks->dataSink)
                {
                    if (callbacks->dataSink(rdLo, instance) != kStatus_SPI_Success)
                    {
                        /* Report SPI receive overrun error*/
                        if (callbacks->onError)
                        {
                            callbacks->onError(kStatus_SPI_SlaveRxOverrun, instance);
                        }
                    }
                }
                /* Sink the next 8-bits */
                if (callbacks->dataSink)
                {
                    if (callbacks->dataSink(rdHi, instance) != kStatus_SPI_Success)
                    {
                        /* Report SPI receive overrun error*/
                        if (callbacks->onError)
                        {
                            callbacks->onError(kStatus_SPI_SlaveRxOverrun, instance);
                        }
                    }
                }
            }
            /* Transmit, 16-bit with FIFO */
            while (SPI_HAL_IsTxBuffEmptyPending(baseAddr))
            {
                /* SPI transmit interrupt*/
                uint8_t source_byte = kEmptyChar;
                if (callbacks->dataSource)
                {
                    /* get the first 8-bits of data and write it to the data register */
                    if (callbacks->dataSource(&source_byte, instance) == kStatus_SPI_Success)
                    {
                        /* Write the data to data register*/
                        SPI_HAL_WriteDataLow(baseAddr, source_byte);
                    }
                    else
                    {
                        /* Report SPI slave  transmit underrun error*/
                        if (callbacks->onError)
                        {
                            callbacks->onError(kStatus_SPI_SlaveTxUnderrun, instance);
                        }
                    }

                    /* get the next 8-bits of data and write it to the data register */
                    if (callbacks->dataSource(&source_byte, instance) == kStatus_SPI_Success)
                    {
                        /* Write the data to data register*/
                        SPI_HAL_WriteDataHigh(baseAddr, source_byte);
                    }
                    else
                    {
                        /* Report SPI slave  transmit underrun error*/
                        if (callbacks->onError)
                        {
                            callbacks->onError(kStatus_SPI_SlaveTxUnderrun, instance);
                        }
                    }
                }
                else
                {
                    /* Write empty char to data register*/
                    SPI_HAL_WriteDataLow(baseAddr, kEmptyChar);
                    SPI_HAL_WriteDataHigh(baseAddr, kEmptyChar);
                }
            }
        }
        /* No FIFO support */
        else
        {
            /* Receive, 16-bit no FIFO */
            if (SPI_HAL_IsReadBuffFullPending(baseAddr))
            {
                /* SPI receive interrupt*/
                uint8_t rdHi = SPI_HAL_ReadDataHigh(baseAddr);
                uint8_t rdLo = SPI_HAL_ReadDataLow(baseAddr);

                /* Sink the first 8-bits */
                if (callbacks->dataSink)
                {
                    if (callbacks->dataSink(rdLo, instance) != kStatus_SPI_Success)
                    {
                        /* Report SPI receive overrun error*/
                        if (callbacks->onError)
                        {
                            callbacks->onError(kStatus_SPI_SlaveRxOverrun, instance);
                        }
                    }
                }
                /* Sink the next 8-bits */
                if (callbacks->dataSink)
                {
                    if (callbacks->dataSink(rdHi, instance) != kStatus_SPI_Success)
                    {
                        /* Report SPI receive overrun error*/
                        if (callbacks->onError)
                        {
                            callbacks->onError(kStatus_SPI_SlaveRxOverrun, instance);
                        }
                    }
                }
            }
            /* Transmit, 16-bit no FIFO */
            if (SPI_HAL_IsTxBuffEmptyPending(baseAddr))
            {
                /* SPI transmit interrupt*/
                uint8_t source_byte = kEmptyChar;
                if (callbacks->dataSource)
                {
                    /* get the first 8-bits of data and write it to the data register */
                    if (callbacks->dataSource(&source_byte, instance) == kStatus_SPI_Success)
                    {
                        /* Write the data to data register*/
                        SPI_HAL_WriteDataLow(baseAddr, source_byte);
                    }
                    else
                    {
                        /* Report SPI slave  transmit underrun error*/
                        if (callbacks->onError)
                        {
                            callbacks->onError(kStatus_SPI_SlaveTxUnderrun, instance);
                        }
                    }

                    /* get the next 8-bits of data and write it to the data register */
                    if (callbacks->dataSource(&source_byte, instance) == kStatus_SPI_Success)
                    {
                        /* Write the data to data register*/
                        SPI_HAL_WriteDataHigh(baseAddr, source_byte);
                    }
                    else
                    {
                        /* Report SPI slave  transmit underrun error*/
                        if (callbacks->onError)
                        {
                            callbacks->onError(kStatus_SPI_SlaveTxUnderrun, instance);
                        }
                    }
                }
                else
                {
                    /* Write empty char to data register*/
                    SPI_HAL_WriteDataLow(baseAddr, kEmptyChar);
                    SPI_HAL_WriteDataHigh(baseAddr, kEmptyChar);
                }
            }

        }
    }
    else /* 8-bit mode */
    {
        /* Service receive first since TX shift register is still shifting out data, we have
         * time before placing data into the TX data buffer.
         */
        /* If SPI module supports FIFO */
        if (FSL_FEATURE_SPI_FIFO_SIZEn(instance) != 0)
        {
            /* Receive, 8-bit with FIFO */
            /* Do this while the RX FIFO is not empty */
            while (SPI_HAL_GetFifoStatusFlag(baseAddr, kSpiRxFifoEmpty) == 0)
            {
                uint8_t rd = SPI_HAL_ReadDataLow(baseAddr);
                if (callbacks->dataSink)
                {
                    if (callbacks->dataSink(rd, instance) != kStatus_SPI_Success)
                    {
                        /* Report SPI receive overrun error*/
                        if (callbacks->onError)
                        {
                            callbacks->onError(kStatus_SPI_SlaveRxOverrun, instance);
                        }
                    }
                }
            }

            /* Transmit, 8-bit with FIFO */
            if (SPI_HAL_IsTxBuffEmptyPending(baseAddr))
            {
                SPI_HAL_ClearFifoIntUsingBitWrite(baseAddr, kSpiTxNearEmptyClearInt);
                uint8_t source_byte = kEmptyChar;
                if (callbacks->dataSource)
                {
                    if (callbacks->dataSource(&source_byte, instance) == kStatus_SPI_Success)
                    {
                        /* Write the data to data register*/
                        SPI_HAL_WriteDataLow(baseAddr, source_byte);
                    }
                    else
                    {
                        /* Report SPI slave  transmit underrun error*/
                        if (callbacks->onError)
                        {
                            callbacks->onError(kStatus_SPI_SlaveTxUnderrun, instance);
                        }
                    }
                }
                else
                {
                    /* Write empty char to data register*/
                    SPI_HAL_WriteDataLow(baseAddr, kEmptyChar);
                }
            }
        }
        /* No FIFO support */
        else
        {
            /* Receive, 8-bit no FIFO  */
            if (SPI_HAL_IsReadBuffFullPending(baseAddr))
            {
                /* SPI receive interrupt*/
                uint8_t rd = SPI_HAL_ReadDataLow(baseAddr);
                if (callbacks->dataSink)
                {
                    if (callbacks->dataSink(rd, instance) != kStatus_SPI_Success)
                    {
                        /* Report SPI receive overrun error*/
                        if (callbacks->onError)
                        {
                            callbacks->onError(kStatus_SPI_SlaveRxOverrun, instance);
                        }
                    }
                }
            }
            /* Transmit, 8-bit no FIFO  */
            if (SPI_HAL_IsTxBuffEmptyPending(baseAddr))
            {
                /* SPI transmit interrupt*/
                uint8_t source_byte = kEmptyChar;
                if (callbacks->dataSource)
                {
                    if (callbacks->dataSource(&source_byte, instance) == kStatus_SPI_Success)
                    {
                        /* Write the data to data register*/
                        SPI_HAL_WriteDataLow(baseAddr, source_byte);
                    }
                    else
                    {
                        /* Report SPI slave  transmit underrun error*/
                        if (callbacks->onError)
                        {
                            callbacks->onError(kStatus_SPI_SlaveTxUnderrun, instance);
                        }
                    }
                }
                else
                {
                    /* Write empty char to data register*/
                    SPI_HAL_WriteDataLow(baseAddr, kEmptyChar);
                }
            }
        }
    }

#else
    /* Service receive first since TX shift register is still shifting out data, we have
     * time before placing data into the TX data buffer.
     */
    /* Receive data */
    if (SPI_HAL_IsReadBuffFullPending(baseAddr))
    {
        /* SPI receive interrupt*/
        uint8_t rd = SPI_HAL_ReadData(baseAddr);
        if (callbacks->dataSink)
        {
            if (callbacks->dataSink(rd, instance) != kStatus_SPI_Success)
            {
                /* Report SPI receive overrun error*/
                if (callbacks->onError)
                {
                    callbacks->onError(kStatus_SPI_SlaveRxOverrun, instance);
                }
            }
        }
    }

    /* Transmit data */
    if (SPI_HAL_IsTxBuffEmptyPending(baseAddr))
    {
        /* SPI transmit interrupt*/
        uint8_t source_byte = kEmptyChar;
        if (callbacks->dataSource)
        {
            if (callbacks->dataSource(&source_byte, instance) == kStatus_SPI_Success)
            {
                /* Write the data to data register*/
                SPI_HAL_WriteData(baseAddr, source_byte);
            }
            else
            {
                /* Report SPI slave  transmit underrun error*/
                if (callbacks->onError)
                {
                    callbacks->onError(kStatus_SPI_SlaveTxUnderrun, instance);
                }
            }
        }
        else
        {
            /* Write empty char to data register*/
            SPI_HAL_WriteData(baseAddr, kEmptyChar);
        }
    }
#endif

}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_DRV_SlaveInit
 * Description   : Initializes the SPI module for slave mode.
 * Saves the application callback info, turns on the clock to the module,
 * enables the device, and enables interrupts. Sets the SPI to a slave mode.
 *
 *END**************************************************************************/
void SPI_DRV_SlaveInit(uint32_t instance, spi_slave_state_t * spiState,
                       const spi_slave_user_config_t * config)
{
    assert(config);
    assert(instance < HW_SPI_INSTANCE_COUNT);

    /* Clear the state for this instance.*/
    memset(spiState, 0, sizeof(* spiState));

    uint32_t baseAddr = g_spiBaseAddr[instance];

    /* Save the application info. */
    spiState->callbacks = config->callbacks;

    /* configure the run-time state struct with the instance number */
    spiState->instance = instance;

    /* Enable clock for SPI*/
    CLOCK_SYS_EnableSpiClock(instance);

    /* Reset the SPI module to its default settings including disabling SPI */
    SPI_HAL_Init(baseAddr);

    /* Set SPI to slave mode */
    SPI_HAL_SetMasterSlave(baseAddr, kSpiSlave);

    /* Configure the slave clock polarity, phase and data direction */
    SPI_HAL_SetDataFormat(baseAddr, config->polarity, config->phase, config->direction);

    /* Set the SPI pin mode to normal mode */
    SPI_HAL_SetPinMode(baseAddr, kSpiPinMode_Normal);

#if FSL_FEATURE_SPI_16BIT_TRANSFERS
    SPI_HAL_Set8or16BitMode(baseAddr, config->bitCount);

    if (FSL_FEATURE_SPI_FIFO_SIZEn(instance) != 0)
    {
        /* If SPI module contains a FIFO, enable it and set watermarks to half full/empty */
        SPI_HAL_SetFifoMode(baseAddr, true, kSpiTxFifoOneHalfEmpty, kSpiRxFifoOneHalfFull);
    }

    SPI_HAL_SetTransmitIntCmd(baseAddr, true);
//    SPI_HAL_SetFifoIntCmd(baseAddr, kSpiTxFifoNearEmptyInt, true);
//    SPI_HAL_SetFifoIntCmd(baseAddr, kSpiRxFifoNearFullInt, true);
#else

    /* Enable transmit interrupt only.  Doing so ensures that everytime the TX data buffer
     * empties into the TX shift register, we'll get an interrupt. Then in ISR, we service the
     * receive first to get data out of the receive data buffer since data is still being shifted
     * out of the TX shift register.
     */
    SPI_HAL_SetTransmitIntCmd(baseAddr, true);
#endif
    /* Save runtime structure pointers to irq handler can point to the correct state structure*/
    g_spiStatePtr[instance] = spiState;

    /* Enable SPI interrupt. The transmit interrupt should immediately cause an interrupt
     * which will fill in the transmit buffer and will be ready to send once the master initiates
     * transmission.
     */
    INT_SYS_EnableIRQ(g_spiIrqId[instance]);

    /* SPI system enable*/
    SPI_HAL_Enable(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_DRV_SlaveDeinit
 * Description   : De-initializes the device.
 * Clears the control register and turns off the clock to the module.
 *
 *END**************************************************************************/
void SPI_DRV_SlaveDeinit(uint32_t instance)
{
    uint32_t baseAddr = g_spiBaseAddr[instance];

#if FSL_FEATURE_SPI_HAS_DMA_SUPPORT
    /* instantiate local variable of type spi_slave_state_t and point to global state */
    spi_slave_state_t * spiState = (spi_slave_state_t *)g_spiStatePtr[instance];
#endif

    assert(instance < HW_SPI_INSTANCE_COUNT);

    /* Disable SPI interrupt*/
    INT_SYS_DisableIRQ(g_spiIrqId[instance]);

    /* Reset the SPI module to its default settings including disabling SPI and its interrupts */
    SPI_HAL_Init(baseAddr);

    /* Disable clock for SPI*/
    CLOCK_SYS_DisableSpiClock(instance);

#if FSL_FEATURE_SPI_HAS_DMA_SUPPORT
    /* If DMA-based operations were enabled, then do the following */
    if (spiState->useDma)
    {
        DMA_DRV_FreeChannel(&spiState->dmaReceive);
        DMA_DRV_FreeChannel(&spiState->dmaTransmit);
    }
#endif

    /* Clear state pointer. */
    g_spiStatePtr[instance] = NULL;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

