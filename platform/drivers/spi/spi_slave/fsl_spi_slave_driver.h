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

#if !defined(__FSL_SPI_SLAVE_DRIVER_H__)
#define __FSL_SPI_SLAVE_DRIVER_H__


#include "fsl_spi_hal.h"
#if FSL_FEATURE_SPI_HAS_DMA_SUPPORT
#include "fsl_dma_driver.h"
#endif

/*! @addtogroup spi_slave_driver*/
/*! @{*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief The set of callbacks used for SPI slave mode.*/
typedef struct SPISlaveCallbacks {
    /*! Callback used to get byte to transmit.*/
    spi_status_t (*dataSource)(uint8_t * sourceByte, uint32_t instance);

    /*! Callback used to put received byte.*/
    spi_status_t (*dataSink)(uint8_t sinkByte, uint32_t instance);

    /*! Callback used to report an SPI error.*/
    void (*onError)(spi_status_t error, uint32_t instance);

    /*! Callback to report the slave SPI DMA is done transferring data. Used only for
     * DMA enabled slave SPI operation and not used for interrupt operation.
     */
    void (*spi_slave_done_t)(void);

} spi_slave_callbacks_t;

/*!
 * @brief Runtime state of the SPI slave driver.
 *
 * This structure holds data that is used by the SPI slave peripheral driver to
 * communicate between the transfer function and the interrupt handler. The user
 * needs to pass in the memory for this structure and the driver  fills out
 * the members.
 */
typedef struct SPISlaveState {
    uint32_t instance;                  /*!< DSPI module instance number */
    spi_slave_callbacks_t callbacks;   /*!< Application/user callbacks */

#if FSL_FEATURE_SPI_HAS_DMA_SUPPORT
    /* DMA related members */
    uint32_t useDma;
    dma_channel_t dmaReceive;
    dma_channel_t dmaTransmit;
#endif
} spi_slave_state_t;


/*! @brief Definition of application implemented configuration and callback */
/*! functions used by the SPI slave driver.*/
typedef struct SPISlaveUserConfig {
    spi_slave_callbacks_t callbacks;    /*!< Application callbacks.*/
    spi_clock_phase_t phase;            /*!< Clock phase setting. */
    spi_clock_polarity_t polarity;      /*!< Clock polarity setting.*/
    spi_shift_direction_t direction;    /*!< Either LSB or MSB first.*/

    /* 16-bit support related members */
#if FSL_FEATURE_SPI_16BIT_TRANSFERS
    spi_data_bitcount_mode_t bitCount;  /*!< Number of bits (8 or 16) in a transfer */
#endif
} spi_slave_user_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 *@name SPI Slave
 *@{
 */

/*!
 * @brief Initializes the SPI module for slave mode.
 *
 * Saves the application callback info, turns on the clock to the module,
 * enables the device, and enables interrupts. Sets the SPI to a slave mode.
 *
 * @param instance Instance number of the SPI module.
 * @param spiState The pointer to the SPI slave driver state structure.
 * @param config Pointer to slave mode configuration.
 */
void SPI_DRV_SlaveInit(uint32_t instance, spi_slave_state_t * spiState,
                       const spi_slave_user_config_t * config);

#if FSL_FEATURE_SPI_HAS_DMA_SUPPORT
/*!
 * @brief Initializes an SPI instance for slave mode operation with the DMA support.
 *
 * This function is exactly like the SPI_DRV_SlaveInit function except that it also adds the DMA support.
 * This function saves the callbacks to the run-time state structure for later use in the
 * interrupt handler. However, unlike the CPU driven slave driver, there is no need to define
 * callbacks for the data sink or data source since the user passes in buffers
 * for the send and receive data, and the DMA engine uses those buffers.
 * An onError callback is needed to service potential errors seen during a Tx FIFO underflow or
 * Rx FIFO overflow.
 * The user also passes in a user defined callback for handling end of transfers of type
 * spi_slave_done_t.
 * These callbacks are set in the spi_slave_callbacks_t structure which is part of the
 * spi_slave_user_config_t structure.  See example below.
 * This function also ungates the clock to the SPI module, initializes the SPI
 * for slave mode, enables the module and corresponding interrupts, and sets up the DMA channels.
 * Once initialized, the SPI module is configured in slave mode and ready to receive data from the
 * SPI master. This is an example to set up the spi_slave_state_t and the
 * spi_slave_user_config_t parameters and to call the SPI_DRV_SlaveInit function by passing
 * in these parameters:
 *   instance = slaveInstance; <- the desired module instance number
 *   spi_slave_state_t spiSlaveState; <- the user simply allocates memory for this struct
 *   spi_slave_user_config_t slaveUserConfig;
 *   slaveUserConfig.callbacks.onError = on_error; <- set to user implementation of function
 *   slaveUserConfig.callbacks.spi_slave_done_t = spiDmaDone; <- user defined callback
 *   slaveUserConfig.dataConfig.clkPhase = kSpiClockPhase_FirstEdge; <- example setting
 *   slaveUserConfig.dataConfig.clkPolarity = kSpiClockPolarity_ActiveHigh; <- example setting
 *   sendBuffer <- (pointer) to the source data buffer, can be NULL
 *   receiveBuffer <- (pointer) to the receive data buffer, can be NULL
 *   transferCount <- number of bytes to transfer
 *
 *   SPI_DRV_SlaveInitDma(slaveInstance, &slaveUserConfig, &spiSlaveState,
 *                         &sendBuffer, &receiveBuffer, transferCount);
 *
 * @param instance The instance number of the SPI peripheral.
 * @param spiState The pointer to the SPI slave driver state structure.
 * @param slaveConfig The configuration structure dspi_slave_user_config_t, including the callbacks.
 * @param sendBuffer The pointer to the data buffer of the data to send. You may pass NULL for this
 *  parameter, in which case bytes with a value of 0 (zero) are sent.
 * @param receiveBuffer Pointer to the buffer where the received bytes are stored. If you pass NULL
 *  for this parameter, the received bytes are ignored.
 * @param transferByteCount The expected number of bytes to transfer.
 * @return An error code or kStatus_SPI_Success.
 */
spi_status_t SPI_DRV_SlaveInitDma(uint32_t instance,
                                  spi_slave_state_t * spiState,
                                  const spi_slave_user_config_t * slaveConfig,
                                  const uint8_t * sendBuffer,
                                  uint8_t * receiveBuffer,
                                  size_t transferByteCount);
#endif

/*!
 * @brief De-initializes the device.
 *
 * Clears the control register and turns off the clock to the module.
 *
 * @param instance Instance number of the SPI module.
 */
void SPI_DRV_SlaveDeinit(uint32_t instance);

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_SPI_SLAVE_DRIVER_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

