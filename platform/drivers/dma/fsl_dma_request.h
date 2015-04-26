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

#if !defined(__FSL_DMA_REQUEST_H__)
#define __FSL_DMA_REQUEST_H__

#include "fsl_device_registers.h"

/*! 
 * @addtogroup dma_request
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Structure for the DMA hardware Request.
 *
 * Defines structure for a collection of the DMA hardware request. The user can configure the
 * hardware request into DMAMUX to trigger the DMA transfer accordingly. The index 
 * of hardware requests varies according the to the SOC.
 */
#if defined(CPU_MKL25Z128VLK4)
typedef enum _dma_request_source {
    kDmaRequestMux0Disable = 0,         /*!< Disable the DMA request. */
    kDmaRequestMux0Reserved1 = 1,       /*!< Reserved slot. */
    kDmaRequestMux0UART0Rx = 2,         /*!< UART 0 receiver. */
    kDmaRequestMux0UART0Tx = 3,         /*!< UART0 transmitter. */
    kDmaRequestMux0UART1Rx = 4,         /*!< UART1 receiver. */
    kDmaRequestMux0UART1Tx = 5,         /*!< UART1 transmitter.. */
    kDmaRequestMux0UART2Rx = 6,         /*!< UART2 receiver. */
    kDmaRequestMux0UART2Tx = 7,         /*!< UART2 transmitter. */
    kDmaRequestMux0Reserved2 = 8,       /*!< Reserved slot. */
    kDmaRequestMux0Reserved3 = 9,       /*!< Reserved slot. */
    kDmaRequestMux0Reserved4 = 10,      /*!< Reserved slot. */
    kDmaRequestMux0Reserved5 = 11,      /*!< Reserved slot. */
    kDmaRequestMux0Reserved6 = 12,      /*!< Reserved slot. */
    kDmaRequestMux0Reserved7 = 13,      /*!< Reserved slot. */
    kDmaRequestMux0Reserved8 = 14,      /*!< Reserved slot. */
    kDmaRequestMux0Reserved9 = 15,      /*!< Reserved slot. */
    kDmaRequestMux0SPI0Rx = 16,         /*!< SPI0 receiver. */
    kDmaRequestMux0SPI0Tx = 17,         /*!< SPI0 transmitter. */
    kDmaRequestMux0SPI1Rx = 18,         /*!< SPI1 receiver. */
    kDmaRequestMux0SPI1Tx = 19,         /*!< SPI1 transmitter. */
    kDmaRequestMux0Reserved10 = 20,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved11 = 21,     /*!< Reserved slot. */
    kDmaRequestMux0I2C0 = 22,           /*!< I2C0. */
    kDmaRequestMux0I2C1 = 23,           /*!< I2C1. */
    kDmaRequestMux0TPM0Channel0 = 24,   /*!< Channel 0 in TPM0. */
    kDmaRequestMux0TPM0Channel1 = 25,   /*!< Channel 1 in TPM0. */
    kDmaRequestMux0TPM0Channel2 = 26,   /*!< Channel 2 in TPM0. */
    kDmaRequestMux0TPM0Channel3 = 27,   /*!< Channel 3 in TPM0. */
    kDmaRequestMux0TPM0Channel4 = 28,   /*!< Channel 4 in TPM0. */
    kDmaRequestMux0TPM0Channel5 = 29,   /*!< Channel 5 in TPM0. */
    kDmaRequestMux0Reserved12 = 30,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved13 = 31,     /*!< Reserved slot. */
    kDmaRequestMux0TPM1Channel0 = 32,   /*!< Channel 0 in TPM1. */
    kDmaRequestMux0TPM1Channel1 = 33,   /*!< Channel 1 in TPM1. */
    kDmaRequestMux0TPM2Channel0 = 34,   /*!< Channel 0 in TPM2. */
    kDmaRequestMux0TPM2Channel1 = 35,    /*!< Channel 1 in TPM2. */
    kDmaRequestMux0Reserved14 = 36,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved15 = 37,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved16 = 38,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved17 = 39,     /*!< Reserved slot. */
    kDmaRequestMux0ADC0 = 40,           /*!< ADC0. */
    kDmaRequestMux0Reserved18 = 41,     /*!< Reserved slot. */
    kDmaRequestMux0CMP0 = 42,           /*!< CMP0. */
    kDmaRequestMux0Reserved19 = 43,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved20 = 44,     /*!< Reserved slot. */
    kDmaRequestMux0DAC0 = 45,           /*!< DAC0. */
    kDmaRequestMux0Reserved21 = 46,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved22 = 47,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved23 = 48,     /*!< Reserved slot. */
    kDmaRequestMux0PortA = 49,          /*!< Port A. */
    kDmaRequestMux0Reserved24 = 50,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved25 = 51,     /*!< Reserved slot. */
    kDmaRequestMux0PortD = 52,          /*!< Port D. */
    kDmaRequestMux0Reserved26 = 53,     /*!< Reserved slot. */
    kDmaRequestMux0TPM0Overflow = 54,   /*!< Overflow for TPM0. */
    kDmaRequestMux0TPM1Overflow = 55,   /*!< Overflow for TPM1. */
    kDmaRequestMux0TPM2Overflow = 56,   /*!< Overflow for TPM2. */
    kDmaRequestMux0TSI = 57,            /*!< TSI. */
    kDmaRequestMux0Reserved27 = 58,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved28 = 59,     /*!< Reserved slot. */
    kDmaRequestMux0AlwaysOn60 = 60,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn61 = 61,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn62 = 62,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn63 = 63      /*!< Always on. */
} dma_request_source_t;
#elif defined(KL43Z4_SERIES) || defined(KL33Z4_SERIES) || \
      defined(KL27Z4_SERIES) || defined(KL17Z4_SERIES)
typedef enum _dma_request_source {
    kDmaRequestMux0Disable = 0,         /*!< Disable the DMA request. */
    kDmaRequestMux0Reserved1 = 1,       /*!< Reserved slot. */
    kDmaRequestMux0LPUART0Rx = 2,       /*!< LPUART0 receiver.    */
    kDmaRequestMux0LPUART0Tx = 3,       /*!< LPUART0 transmitter. */
    kDmaRequestMux0LPUART1Rx = 4,       /*!< LPUART1 receiver.    */
    kDmaRequestMux0LPUART1Tx = 5,       /*!< LPUART1 transmitter. */
    kDmaRequestMux0UART2Rx = 6,         /*!< UART2 receiver.    */
    kDmaRequestMux0UART2Tx = 7,         /*!< UART2 transmitter. */
    kDmaRequestMux0Reserved2 = 8,       /*!< Reserved slot. */
    kDmaRequestMux0Reserved3 = 9,       /*!< Reserved slot. */
    kDmaRequestMux0Reserved4 = 10,      /*!< Reserved slot. */
    kDmaRequestMux0Reserved5 = 11,      /*!< Reserved slot. */
    kDmaRequestMux0Reserved6 = 12,      /*!< Reserved slot. */
    kDmaRequestMux0Reserved7 = 13,      /*!< Reserved slot. */
    kDmaRequestMux0Reserved8 = 14,      /*!< Reserved slot. */
    kDmaRequestMux0Reserved9 = 15,      /*!< Reserved slot. */
    kDmaRequestMux0SPI0Rx = 16,         /*!< SPI0 receiver. */
    kDmaRequestMux0SPI0Tx = 17,         /*!< SPI0 transmitter. */
    kDmaRequestMux0SPI1Rx = 18,         /*!< SPI1 receiver. */
    kDmaRequestMux0SPI1Tx = 19,         /*!< SPI1 transmitter. */
    kDmaRequestMux0Reserved10 = 20,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved11 = 21,     /*!< Reserved slot. */
    kDmaRequestMux0I2C0 = 22,           /*!< I2C0. */
    kDmaRequestMux0I2C1 = 23,           /*!< I2C1. */
    kDmaRequestMux0TPM0Channel0 = 24,   /*!< Channel 0 in TPM0. */
    kDmaRequestMux0TPM0Channel1 = 25,   /*!< Channel 1 in TPM0. */
    kDmaRequestMux0TPM0Channel2 = 26,   /*!< Channel 2 in TPM0. */
    kDmaRequestMux0TPM0Channel3 = 27,   /*!< Channel 3 in TPM0. */
    kDmaRequestMux0TPM0Channel4 = 28,   /*!< Channel 4 in TPM0. */
    kDmaRequestMux0TPM0Channel5 = 29,   /*!< Channel 5 in TPM0. */
    kDmaRequestMux0Reserved12 = 30,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved13 = 31,     /*!< Reserved slot. */
    kDmaRequestMux0TPM1Channel0 = 32,   /*!< Channel 0 in TPM1. */
    kDmaRequestMux0TPM1Channel1 = 33,   /*!< Channel 1 in TPM1. */
    kDmaRequestMux0TPM2Channel0 = 34,   /*!< Channel 0 in TPM2. */
    kDmaRequestMux0TPM2Channel1 = 35,   /*!< Channel 1 in TPM2. */
    kDmaRequestMux0Reserved14 = 36,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved15 = 37,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved16 = 38,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved17 = 39,     /*!< Reserved slot. */
    kDmaRequestMux0ADC0 = 40,           /*!< ADC0. */
    kDmaRequestMux0Reserved18 = 41,     /*!< Reserved slot. */
    kDmaRequestMux0CMP0 = 42,           /*!< CMP0. */
    kDmaRequestMux0Reserved19 = 43,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved20 = 44,     /*!< Reserved slot. */
    kDmaRequestMux0DAC0 = 45,           /*!< DAC0. */
    kDmaRequestMux0Reserved21 = 46,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved22 = 47,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved23 = 48,     /*!< Reserved slot. */
    kDmaRequestMux0PortA = 49,          /*!< Port A. */
    kDmaRequestMux0Reserved24 = 50,     /*!< Reserved slot. */
    kDmaRequestMux0PortC = 51,          /*!< Port C. */
    kDmaRequestMux0PortD = 52,          /*!< Port D. */
    kDmaRequestMux0Reserved26 = 53,     /*!< Reserved slot. */
    kDmaRequestMux0TPM0Overflow = 54,   /*!< Overflow for TPM0. */
    kDmaRequestMux0TPM1Overflow = 55,   /*!< Overflow for TPM1. */
    kDmaRequestMux0TPM2Overflow = 56,   /*!< Overflow for TPM2. */
    kDmaRequestMux0Reserved27 = 57,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved28 = 58,     /*!< Reserved slot. */
    kDmaRequestMux0Reserved29 = 59,     /*!< Reserved slot. */
    kDmaRequestMux0AlwaysOn60 = 60,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn61 = 61,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn62 = 62,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn63 = 63      /*!< Always on. */
} dma_request_source_t;
#else
	#error "No valid CPU defined"
#endif

/* @} */

#endif /* __FSL_DMA_REQUEST_H__ */
/*******************************************************************************
 * EOF
 ******************************************************************************/

