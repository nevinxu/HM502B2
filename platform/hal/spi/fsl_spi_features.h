/*
** ###################################################################
**     Version:             rev. 1.0, 2014-05-14
**     Build:               b140826
**
**     Abstract:
**         Chip specific module features.
**
**     Copyright (c) 2014 Freescale Semiconductor, Inc.
**     All rights reserved.
**
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2014-05-14)
**         Customer release.
**
** ###################################################################
*/

#if !defined(__FSL_SPI_FEATURES_H__)
#define __FSL_SPI_FEATURES_H__

#if defined(CPU_MKE02Z64VLC2) || defined(CPU_MKE02Z32VLC2) || defined(CPU_MKE02Z16VLC2) || defined(CPU_SKEAZN64MLC2) || \
    defined(CPU_SKEAZN32MLC2) || defined(CPU_SKEAZN16MLC2) || defined(CPU_MKE02Z64VLC4) || defined(CPU_MKE02Z32VLC4) || \
    defined(CPU_MKE02Z16VLC4) || defined(CPU_MKE02Z64VLD2) || defined(CPU_MKE02Z32VLD2) || defined(CPU_MKE02Z16VLD2) || \
    defined(CPU_SKEAZN64MLD2) || defined(CPU_SKEAZN32MLD2) || defined(CPU_SKEAZN16MLD2) || defined(CPU_MKE02Z64VLD4) || \
    defined(CPU_MKE02Z32VLD4) || defined(CPU_MKE02Z16VLD4) || defined(CPU_MKE02Z64VLH2) || defined(CPU_MKE02Z64VQH2) || \
    defined(CPU_MKE02Z32VLH2) || defined(CPU_MKE02Z32VQH2) || defined(CPU_SKEAZN64MLH2) || defined(CPU_SKEAZN32MLH2) || \
    defined(CPU_MKE02Z64VLH4) || defined(CPU_MKE02Z64VQH4) || defined(CPU_MKE02Z32VLH4) || defined(CPU_MKE02Z32VQH4) || \
    defined(CPU_MKE04Z128VLD4) || defined(CPU_MKE04Z64VLD4) || defined(CPU_MKE04Z128VLK4) || defined(CPU_MKE04Z64VLK4) || \
    defined(CPU_MKE04Z128VQH4) || defined(CPU_MKE04Z64VQH4) || defined(CPU_MKE04Z128VLH4) || defined(CPU_MKE04Z64VLH4) || \
    defined(CPU_MKE06Z128VLD4) || defined(CPU_MKE06Z64VLD4) || defined(CPU_MKE06Z128VLK4) || defined(CPU_MKE06Z64VLK4) || \
    defined(CPU_MKE06Z128VQH4) || defined(CPU_MKE06Z64VQH4) || defined(CPU_MKE06Z128VLH4) || defined(CPU_MKE06Z64VLH4) || \
    defined(CPU_SKEAZ128MLH) || defined(CPU_SKEAZ64MLH) || defined(CPU_SKEAZ128MLK) || defined(CPU_SKEAZ64MLK)
    /* @brief Has DMA support (register bit fields C2[RXDMAE] and C2[TXDMAE]). */
    #define FSL_FEATURE_SPI_HAS_DMA_SUPPORT (0)
    /* @brief Receive/transmit FIFO size in number of 16-bit communication items. */
    #define FSL_FEATURE_SPI_FIFO_SIZE (0)
    #define FSL_FEATURE_SPI_FIFO_SIZEn(x) \
        ((x) == 0 ? (0) : \
        ((x) == 1 ? (0) : (-1)))
    /* @brief Maximum transfer data width in bits. */
    #define FSL_FEATURE_SPI_MAX_DATA_WIDTH (8)
    /* @brief The data register name has postfix (L as low and H as high). */
    #define FSL_FEATURE_SPI_DATA_REGISTER_HAS_POSTFIX (0)
    /* @brief Has 16-bit data transfer support. */
    #define FSL_FEATURE_SPI_16BIT_TRANSFERS (0)
#elif defined(CPU_MKE04Z8VFK4) || defined(CPU_SKEAZN8MFK) || defined(CPU_MKE04Z8VTG4) || defined(CPU_SKEAZN8MTG) || \
    defined(CPU_MKE04Z8VWJ4) || defined(CPU_MKL02Z32CAF4) || defined(CPU_MKL02Z8VFG4) || defined(CPU_MKL02Z16VFG4) || \
    defined(CPU_MKL02Z32VFG4) || defined(CPU_MKL02Z16VFK4) || defined(CPU_MKL02Z32VFK4) || defined(CPU_MKL02Z16VFM4) || \
    defined(CPU_MKL02Z32VFM4) || defined(CPU_MKL03Z32CAF4) || defined(CPU_MKL03Z8VFG4) || defined(CPU_MKL03Z16VFG4) || \
    defined(CPU_MKL03Z32VFG4) || defined(CPU_MKL03Z8VFK4) || defined(CPU_MKL03Z16VFK4) || defined(CPU_MKL03Z32VFK4)
    /* @brief Has DMA support (register bit fields C2[RXDMAE] and C2[TXDMAE]). */
    #define FSL_FEATURE_SPI_HAS_DMA_SUPPORT (0)
    /* @brief Receive/transmit FIFO size in number of 16-bit communication items. */
    #define FSL_FEATURE_SPI_FIFO_SIZE (0)
    #define FSL_FEATURE_SPI_FIFO_SIZEn(x) \
        ((x) == 0 ? (0) : (-1))
    /* @brief Maximum transfer data width in bits. */
    #define FSL_FEATURE_SPI_MAX_DATA_WIDTH (8)
    /* @brief The data register name has postfix (L as low and H as high). */
    #define FSL_FEATURE_SPI_DATA_REGISTER_HAS_POSTFIX (0)
    /* @brief Has 16-bit data transfer support. */
    #define FSL_FEATURE_SPI_16BIT_TRANSFERS (0)
#elif defined(CPU_MKL04Z8VFK4) || defined(CPU_MKL04Z16VFK4) || defined(CPU_MKL04Z32VFK4) || defined(CPU_MKL04Z8VLC4) || \
    defined(CPU_MKL04Z16VLC4) || defined(CPU_MKL04Z32VLC4) || defined(CPU_MKL04Z8VFM4) || defined(CPU_MKL04Z16VFM4) || \
    defined(CPU_MKL04Z32VFM4) || defined(CPU_MKL04Z16VLF4) || defined(CPU_MKL04Z32VLF4) || defined(CPU_MKL05Z8VFK4) || \
    defined(CPU_MKL05Z16VFK4) || defined(CPU_MKL05Z32VFK4) || defined(CPU_MKL05Z8VLC4) || defined(CPU_MKL05Z16VLC4) || \
    defined(CPU_MKL05Z32VLC4) || defined(CPU_MKL05Z8VFM4) || defined(CPU_MKL05Z16VFM4) || defined(CPU_MKL05Z32VFM4) || \
    defined(CPU_MKL05Z16VLF4) || defined(CPU_MKL05Z32VLF4)
    /* @brief Has DMA support (register bit fields C2[RXDMAE] and C2[TXDMAE]). */
    #define FSL_FEATURE_SPI_HAS_DMA_SUPPORT (1)
    /* @brief Receive/transmit FIFO size in number of 16-bit communication items. */
    #define FSL_FEATURE_SPI_FIFO_SIZE (0)
    #define FSL_FEATURE_SPI_FIFO_SIZEn(x) \
        ((x) == 0 ? (0) : (-1))
    /* @brief Maximum transfer data width in bits. */
    #define FSL_FEATURE_SPI_MAX_DATA_WIDTH (8)
    /* @brief The data register name has postfix (L as low and H as high). */
    #define FSL_FEATURE_SPI_DATA_REGISTER_HAS_POSTFIX (0)
    /* @brief Has 16-bit data transfer support. */
    #define FSL_FEATURE_SPI_16BIT_TRANSFERS (0)
#elif defined(CPU_MKL14Z32VFM4) || defined(CPU_MKL14Z64VFM4) || defined(CPU_MKL14Z32VFT4) || defined(CPU_MKL14Z64VFT4) || \
    defined(CPU_MKL14Z32VLH4) || defined(CPU_MKL14Z64VLH4) || defined(CPU_MKL14Z32VLK4) || defined(CPU_MKL14Z64VLK4) || \
    defined(CPU_MKL15Z32VFM4) || defined(CPU_MKL15Z64VFM4) || defined(CPU_MKL15Z128VFM4) || defined(CPU_MKL15Z32VFT4) || \
    defined(CPU_MKL15Z64VFT4) || defined(CPU_MKL15Z128VFT4) || defined(CPU_MKL15Z32VLH4) || defined(CPU_MKL15Z64VLH4) || \
    defined(CPU_MKL15Z128VLH4) || defined(CPU_MKL15Z32VLK4) || defined(CPU_MKL15Z64VLK4) || defined(CPU_MKL15Z128VLK4) || \
    defined(CPU_MKL24Z32VFM4) || defined(CPU_MKL24Z64VFM4) || defined(CPU_MKL24Z32VFT4) || defined(CPU_MKL24Z64VFT4) || \
    defined(CPU_MKL24Z32VLH4) || defined(CPU_MKL24Z64VLH4) || defined(CPU_MKL24Z32VLK4) || defined(CPU_MKL24Z64VLK4) || \
    defined(CPU_MKL25Z32VFM4) || defined(CPU_MKL25Z64VFM4) || defined(CPU_MKL25Z128VFM4) || defined(CPU_MKL25Z32VFT4) || \
    defined(CPU_MKL25Z64VFT4) || defined(CPU_MKL25Z128VFT4) || defined(CPU_MKL25Z32VLH4) || defined(CPU_MKL25Z64VLH4) || \
    defined(CPU_MKL25Z128VLH4) || defined(CPU_MKL25Z32VLK4) || defined(CPU_MKL25Z64VLK4) || defined(CPU_MKL25Z128VLK4)
    /* @brief Has DMA support (register bit fields C2[RXDMAE] and C2[TXDMAE]). */
    #define FSL_FEATURE_SPI_HAS_DMA_SUPPORT (1)
    /* @brief Receive/transmit FIFO size in number of 16-bit communication items. */
    #define FSL_FEATURE_SPI_FIFO_SIZE (0)
    #define FSL_FEATURE_SPI_FIFO_SIZEn(x) \
        ((x) == 0 ? (0) : \
        ((x) == 1 ? (0) : (-1)))
    /* @brief Maximum transfer data width in bits. */
    #define FSL_FEATURE_SPI_MAX_DATA_WIDTH (8)
    /* @brief The data register name has postfix (L as low and H as high). */
    #define FSL_FEATURE_SPI_DATA_REGISTER_HAS_POSTFIX (0)
    /* @brief Has 16-bit data transfer support. */
    #define FSL_FEATURE_SPI_16BIT_TRANSFERS (0)
#elif defined(CPU_MKL16Z32VFM4) || defined(CPU_MKL16Z64VFM4) || defined(CPU_MKL16Z128VFM4) || defined(CPU_MKL16Z32VFT4) || \
    defined(CPU_MKL16Z64VFT4) || defined(CPU_MKL16Z128VFT4) || defined(CPU_MKL16Z32VLH4) || defined(CPU_MKL16Z64VLH4) || \
    defined(CPU_MKL16Z128VLH4) || defined(CPU_MKL16Z256VLH4) || defined(CPU_MKL16Z256VMP4) || defined(CPU_MKL17Z128VFM4) || \
    defined(CPU_MKL17Z256VFM4) || defined(CPU_MKL17Z128VFT4) || defined(CPU_MKL17Z256VFT4) || defined(CPU_MKL17Z128VLH4) || \
    defined(CPU_MKL17Z256VLH4) || defined(CPU_MKL17Z128VMP4) || defined(CPU_MKL17Z256VMP4) || defined(CPU_MKL17Z32VLH4) || \
    defined(CPU_MKL17Z64VLH4) || defined(CPU_MKL26Z32VFM4) || defined(CPU_MKL26Z64VFM4) || defined(CPU_MKL26Z128VFM4) || \
    defined(CPU_MKL26Z32VFT4) || defined(CPU_MKL26Z64VFT4) || defined(CPU_MKL26Z128VFT4) || defined(CPU_MKL26Z32VLH4) || \
    defined(CPU_MKL26Z64VLH4) || defined(CPU_MKL26Z128VLH4) || defined(CPU_MKL26Z256VLH4) || defined(CPU_MKL26Z256VLK4) || \
    defined(CPU_MKL26Z128VLL4) || defined(CPU_MKL26Z256VLL4) || defined(CPU_MKL26Z128VMC4) || defined(CPU_MKL26Z256VMC4) || \
    defined(CPU_MKL26Z256VMP4) || defined(CPU_MKL27Z128VFM4) || defined(CPU_MKL27Z256VFM4) || defined(CPU_MKL27Z128VFT4) || \
    defined(CPU_MKL27Z256VFT4) || defined(CPU_MKL27Z128VLH4) || defined(CPU_MKL27Z256VLH4) || defined(CPU_MKL27Z128VMP4) || \
    defined(CPU_MKL27Z256VMP4) || defined(CPU_MKL27Z32VLH4) || defined(CPU_MKL27Z64VLH4) || defined(CPU_MKL33Z128VLH4) || \
    defined(CPU_MKL33Z256VLH4) || defined(CPU_MKL33Z128VMP4) || defined(CPU_MKL33Z256VMP4) || defined(CPU_MKL34Z64VLH4) || \
    defined(CPU_MKL34Z64VLL4) || defined(CPU_MKL36Z64VLH4) || defined(CPU_MKL36Z128VLH4) || defined(CPU_MKL36Z256VLH4) || \
    defined(CPU_MKL36Z64VLL4) || defined(CPU_MKL36Z128VLL4) || defined(CPU_MKL36Z256VLL4) || defined(CPU_MKL36Z128VMC4) || \
    defined(CPU_MKL36Z256VMC4) || defined(CPU_MKL36Z256VMP4) || defined(CPU_MKL43Z128VLH4) || defined(CPU_MKL43Z256VLH4) || \
    defined(CPU_MKL43Z128VMP4) || defined(CPU_MKL43Z256VMP4) || defined(CPU_MKL46Z128VLH4) || defined(CPU_MKL46Z256VLH4) || \
    defined(CPU_MKL46Z128VLL4) || defined(CPU_MKL46Z256VLL4) || defined(CPU_MKL46Z128VMC4) || defined(CPU_MKL46Z256VMC4) || \
    defined(CPU_MKL46Z256VMP4)
    /* @brief Has DMA support (register bit fields C2[RXDMAE] and C2[TXDMAE]). */
    #define FSL_FEATURE_SPI_HAS_DMA_SUPPORT (1)
    /* @brief Receive/transmit FIFO size in number of 16-bit communication items. */
    #define FSL_FEATURE_SPI_FIFO_SIZE (4)
    #define FSL_FEATURE_SPI_FIFO_SIZEn(x) \
        ((x) == 0 ? (0) : \
        ((x) == 1 ? (4) : (-1)))
    /* @brief Maximum transfer data width in bits. */
    #define FSL_FEATURE_SPI_MAX_DATA_WIDTH (16)
    /* @brief The data register name has postfix (L as low and H as high). */
    #define FSL_FEATURE_SPI_DATA_REGISTER_HAS_POSTFIX (1)
    /* @brief Has 16-bit data transfer support. */
    #define FSL_FEATURE_SPI_16BIT_TRANSFERS (1)
#else
    #error "No valid CPU defined!"
#endif

#endif /* __FSL_SPI_FEATURES_H__ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
