/*
** ###################################################################
**     Version:             rev. 1.0, 2014-05-14
**     Build:               b140821
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

#if !defined(__FSL_DAC_FEATURES_H__)
#define __FSL_DAC_FEATURES_H__

#if defined(CPU_MK02FN128VFM10) || defined(CPU_MK02FN64VFM10) || defined(CPU_MK02FN128VLF10) || defined(CPU_MK02FN64VLF10) || \
    defined(CPU_MK02FN128VLH10) || defined(CPU_MK02FN64VLH10) || defined(CPU_MK22FN128VDC10) || defined(CPU_MK22FN128VLH10) || \
    defined(CPU_MK22FN128VLL10) || defined(CPU_MK22FN128VMP10) || defined(CPU_MK22FN256VDC12) || defined(CPU_MK22FN256VLH12) || \
    defined(CPU_MK22FN256VLL12) || defined(CPU_MK22FN256VMP12) || defined(CPU_MK22FN512VDC12) || defined(CPU_MK22FN512VLH12) || \
    defined(CPU_MK22FN512VLL12) || defined(CPU_MKV30F128VFM10) || defined(CPU_MKV30F64VFM10) || defined(CPU_MKV30F128VLF10) || \
    defined(CPU_MKV30F64VLF10) || defined(CPU_MKV30F128VLH10) || defined(CPU_MKV30F64VLH10) || defined(CPU_MKV31F128VLH10) || \
    defined(CPU_MKV31F128VLL10) || defined(CPU_MKV31F256VLH12) || defined(CPU_MKV31F256VLL12) || defined(CPU_MKV31F512VLH12) || \
    defined(CPU_MKV31F512VLL12) || defined(CPU_MKV44F128VLH15) || defined(CPU_MKV44F128VLL15) || defined(CPU_MKV44F64VLH15) || \
    defined(CPU_MKV46F128VLH15) || defined(CPU_MKV46F128VLL15) || defined(CPU_MKV46F256VLH15) || defined(CPU_MKV46F256VLL15)
    /* @brief Define the size of hardware buffer */
    #define FSL_FEATURE_DAC_BUFFER_SIZE (16)
    /* @brief Define whether the buffer supports watermark event detection or not. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION (1)
    /* @brief Define whether the buffer supports watermark event 1 word before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_1_WORD (1)
    /* @brief Define whether the buffer supports watermark event 2 words before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_2_WORDS (1)
    /* @brief Define whether the buffer supports watermark event 3 words before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_3_WORDS (1)
    /* @brief Define whether the buffer supports watermark event 4 words before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_4_WORDS (1)
    /* @brief Define whether FIFO buffer mode is available or not. */
    #define FSL_FEATURE_DAC_HAS_BUFFER_FIFO_MODE (1)
    /* @brief Define whether swing buffer mode is available or not.. */
    #define FSL_FEATURE_DAC_HAS_BUFFER_SWING_MODE (1)
#elif defined(CPU_MK10DN512VLK10) || defined(CPU_MK10DN512VLL10) || defined(CPU_MK10DX128VLQ10) || defined(CPU_MK10DX256VLQ10) || \
    defined(CPU_MK10DN512VLQ10) || defined(CPU_MK10DN512VMC10) || defined(CPU_MK10DX128VMD10) || defined(CPU_MK10DX256VMD10) || \
    defined(CPU_MK10DN512VMD10) || defined(CPU_MK11DX128AVMC5) || defined(CPU_MK11DX256AVMC5) || defined(CPU_MK11DN512AVMC5) || \
    defined(CPU_MK20DN512VLK10) || defined(CPU_MK20DN512VLL10) || defined(CPU_MK20DX128VLQ10) || defined(CPU_MK20DX256VLQ10) || \
    defined(CPU_MK20DN512VLQ10) || defined(CPU_MK20DX256VMC10) || defined(CPU_MK20DN512VMC10) || defined(CPU_MK20DX128VMD10) || \
    defined(CPU_MK20DX256VMD10) || defined(CPU_MK20DN512VMD10) || defined(CPU_MK21DX128AVMC5) || defined(CPU_MK21DX256AVMC5) || \
    defined(CPU_MK21DN512AVMC5) || defined(CPU_MK21FX512AVLQ12) || defined(CPU_MK21FN1M0AVLQ12) || defined(CPU_MK21FX512AVMC12) || \
    defined(CPU_MK21FN1M0AVMC12) || defined(CPU_MK21FX512AVMD12) || defined(CPU_MK21FN1M0AVMD12) || defined(CPU_MK24FN1M0VDC12) || \
    defined(CPU_MK24FN1M0VLL12) || defined(CPU_MK24FN1M0VLQ12) || defined(CPU_MK24FN256VDC12) || defined(CPU_MK30DN512VLK10) || \
    defined(CPU_MK30DN512VLL10) || defined(CPU_MK30DX128VLQ10) || defined(CPU_MK30DX256VLQ10) || defined(CPU_MK30DN512VLQ10) || \
    defined(CPU_MK30DN512VMC10) || defined(CPU_MK30DX128VMD10) || defined(CPU_MK30DX256VMD10) || defined(CPU_MK30DN512VMD10) || \
    defined(CPU_MK40DN512VLK10) || defined(CPU_MK40DN512VLL10) || defined(CPU_MK40DX128VLQ10) || defined(CPU_MK40DX256VLQ10) || \
    defined(CPU_MK40DN512VLQ10) || defined(CPU_MK40DN512VMC10) || defined(CPU_MK40DX128VMD10) || defined(CPU_MK40DX256VMD10) || \
    defined(CPU_MK40DN512VMD10) || defined(CPU_MK50DX256CLL10) || defined(CPU_MK50DN512CLL10) || defined(CPU_MK50DN512CLQ10) || \
    defined(CPU_MK50DX256CMC10) || defined(CPU_MK50DN512CMC10) || defined(CPU_MK50DN512CMD10) || defined(CPU_MK50DX256CMD10) || \
    defined(CPU_MK50DX256CLK10) || defined(CPU_MK51DX256CLL10) || defined(CPU_MK51DN512CLL10) || defined(CPU_MK51DN256CLQ10) || \
    defined(CPU_MK51DN512CLQ10) || defined(CPU_MK51DX256CMC10) || defined(CPU_MK51DN512CMC10) || defined(CPU_MK51DN256CMD10) || \
    defined(CPU_MK51DN512CMD10) || defined(CPU_MK51DX256CLK10) || defined(CPU_MK52DN512CLQ10) || defined(CPU_MK52DN512CMD10) || \
    defined(CPU_MK53DN512CLQ10) || defined(CPU_MK53DX256CLQ10) || defined(CPU_MK53DN512CMD10) || defined(CPU_MK53DX256CMD10) || \
    defined(CPU_MK60DN256VLL10) || defined(CPU_MK60DX256VLL10) || defined(CPU_MK60DN512VLL10) || defined(CPU_MK60DN256VLQ10) || \
    defined(CPU_MK60DX256VLQ10) || defined(CPU_MK60DN512VLQ10) || defined(CPU_MK60DN256VMC10) || defined(CPU_MK60DX256VMC10) || \
    defined(CPU_MK60DN512VMC10) || defined(CPU_MK60DN256VMD10) || defined(CPU_MK60DX256VMD10) || defined(CPU_MK60DN512VMD10) || \
    defined(CPU_MK63FN1M0VLQ12) || defined(CPU_MK63FN1M0VMD12) || defined(CPU_MK64FX512VDC12) || defined(CPU_MK64FN1M0VDC12) || \
    defined(CPU_MK64FX512VLL12) || defined(CPU_MK64FN1M0VLL12) || defined(CPU_MK64FX512VLQ12) || defined(CPU_MK64FN1M0VLQ12) || \
    defined(CPU_MK64FX512VMD12) || defined(CPU_MK64FN1M0VMD12) || defined(CPU_MK65FN2M0CAC18) || defined(CPU_MK65FX1M0CAC18) || \
    defined(CPU_MK65FN2M0VMI18) || defined(CPU_MK65FX1M0VMI18) || defined(CPU_MK66FN2M0VLQ18) || defined(CPU_MK66FX1M0VLQ18) || \
    defined(CPU_MK66FN2M0VMD18) || defined(CPU_MK66FX1M0VMD18) || defined(CPU_MK70FN1M0VMJ12) || defined(CPU_MK70FX512VMJ12) || \
    defined(CPU_MK70FN1M0VMJ15) || defined(CPU_MK70FX512VMJ15)
    /* @brief Define the size of hardware buffer */
    #define FSL_FEATURE_DAC_BUFFER_SIZE (16)
    /* @brief Define whether the buffer supports watermark event detection or not. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION (1)
    /* @brief Define whether the buffer supports watermark event 1 word before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_1_WORD (1)
    /* @brief Define whether the buffer supports watermark event 2 words before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_2_WORDS (1)
    /* @brief Define whether the buffer supports watermark event 3 words before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_3_WORDS (1)
    /* @brief Define whether the buffer supports watermark event 4 words before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_4_WORDS (1)
    /* @brief Define whether FIFO buffer mode is available or not. */
    #define FSL_FEATURE_DAC_HAS_BUFFER_FIFO_MODE (0)
    /* @brief Define whether swing buffer mode is available or not.. */
    #define FSL_FEATURE_DAC_HAS_BUFFER_SWING_MODE (1)
#elif defined(CPU_MKL05Z8VFK4) || defined(CPU_MKL05Z16VFK4) || defined(CPU_MKL05Z32VFK4) || defined(CPU_MKL05Z8VLC4) || \
    defined(CPU_MKL05Z16VLC4) || defined(CPU_MKL05Z32VLC4) || defined(CPU_MKL05Z8VFM4) || defined(CPU_MKL05Z16VFM4) || \
    defined(CPU_MKL05Z32VFM4) || defined(CPU_MKL05Z16VLF4) || defined(CPU_MKL05Z32VLF4) || defined(CPU_MKL15Z32VFM4) || \
    defined(CPU_MKL15Z64VFM4) || defined(CPU_MKL15Z128VFM4) || defined(CPU_MKL15Z32VFT4) || defined(CPU_MKL15Z64VFT4) || \
    defined(CPU_MKL15Z128VFT4) || defined(CPU_MKL15Z32VLH4) || defined(CPU_MKL15Z64VLH4) || defined(CPU_MKL15Z128VLH4) || \
    defined(CPU_MKL15Z32VLK4) || defined(CPU_MKL15Z64VLK4) || defined(CPU_MKL15Z128VLK4) || defined(CPU_MKL16Z32VFM4) || \
    defined(CPU_MKL16Z64VFM4) || defined(CPU_MKL16Z128VFM4) || defined(CPU_MKL16Z32VFT4) || defined(CPU_MKL16Z64VFT4) || \
    defined(CPU_MKL16Z128VFT4) || defined(CPU_MKL16Z32VLH4) || defined(CPU_MKL16Z64VLH4) || defined(CPU_MKL16Z128VLH4) || \
    defined(CPU_MKL16Z256VLH4) || defined(CPU_MKL16Z256VMP4) || defined(CPU_MKL25Z32VFM4) || defined(CPU_MKL25Z64VFM4) || \
    defined(CPU_MKL25Z128VFM4) || defined(CPU_MKL25Z32VFT4) || defined(CPU_MKL25Z64VFT4) || defined(CPU_MKL25Z128VFT4) || \
    defined(CPU_MKL25Z32VLH4) || defined(CPU_MKL25Z64VLH4) || defined(CPU_MKL25Z128VLH4) || defined(CPU_MKL25Z32VLK4) || \
    defined(CPU_MKL25Z64VLK4) || defined(CPU_MKL25Z128VLK4) || defined(CPU_MKL26Z32VFM4) || defined(CPU_MKL26Z64VFM4) || \
    defined(CPU_MKL26Z128VFM4) || defined(CPU_MKL26Z32VFT4) || defined(CPU_MKL26Z64VFT4) || defined(CPU_MKL26Z128VFT4) || \
    defined(CPU_MKL26Z32VLH4) || defined(CPU_MKL26Z64VLH4) || defined(CPU_MKL26Z128VLH4) || defined(CPU_MKL26Z256VLH4) || \
    defined(CPU_MKL26Z256VLK4) || defined(CPU_MKL26Z128VLL4) || defined(CPU_MKL26Z256VLL4) || defined(CPU_MKL26Z128VMC4) || \
    defined(CPU_MKL26Z256VMC4) || defined(CPU_MKL26Z256VMP4) || defined(CPU_MKL36Z64VLH4) || defined(CPU_MKL36Z128VLH4) || \
    defined(CPU_MKL36Z256VLH4) || defined(CPU_MKL36Z64VLL4) || defined(CPU_MKL36Z128VLL4) || defined(CPU_MKL36Z256VLL4) || \
    defined(CPU_MKL36Z128VMC4) || defined(CPU_MKL36Z256VMC4) || defined(CPU_MKL36Z256VMP4) || defined(CPU_MKL46Z128VLH4) || \
    defined(CPU_MKL46Z256VLH4) || defined(CPU_MKL46Z128VLL4) || defined(CPU_MKL46Z256VLL4) || defined(CPU_MKL46Z128VMC4) || \
    defined(CPU_MKL46Z256VMC4) || defined(CPU_MKL46Z256VMP4)
    /* @brief Define the size of hardware buffer */
    #define FSL_FEATURE_DAC_BUFFER_SIZE (2)
    /* @brief Define whether the buffer supports watermark event detection or not. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION (0)
    /* @brief Define whether the buffer supports watermark event 1 word before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_1_WORD (0)
    /* @brief Define whether the buffer supports watermark event 2 words before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_2_WORDS (0)
    /* @brief Define whether the buffer supports watermark event 3 words before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_3_WORDS (0)
    /* @brief Define whether the buffer supports watermark event 4 words before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_4_WORDS (0)
    /* @brief Define whether FIFO buffer mode is available or not. */
    #define FSL_FEATURE_DAC_HAS_BUFFER_FIFO_MODE (0)
    /* @brief Define whether swing buffer mode is available or not.. */
    #define FSL_FEATURE_DAC_HAS_BUFFER_SWING_MODE (0)
#elif defined(CPU_MKL17Z128VFM4) || defined(CPU_MKL17Z256VFM4) || defined(CPU_MKL17Z128VFT4) || defined(CPU_MKL17Z256VFT4) || \
    defined(CPU_MKL17Z128VLH4) || defined(CPU_MKL17Z256VLH4) || defined(CPU_MKL17Z128VMP4) || defined(CPU_MKL17Z256VMP4) || \
    defined(CPU_MKL27Z128VFM4) || defined(CPU_MKL27Z256VFM4) || defined(CPU_MKL27Z128VFT4) || defined(CPU_MKL27Z256VFT4) || \
    defined(CPU_MKL27Z128VLH4) || defined(CPU_MKL27Z256VLH4) || defined(CPU_MKL27Z128VMP4) || defined(CPU_MKL27Z256VMP4) || \
    defined(CPU_MKL33Z128VLH4) || defined(CPU_MKL33Z256VLH4) || defined(CPU_MKL33Z128VMP4) || defined(CPU_MKL33Z256VMP4) || \
    defined(CPU_MKL43Z128VLH4) || defined(CPU_MKL43Z256VLH4) || defined(CPU_MKL43Z128VMP4) || defined(CPU_MKL43Z256VMP4)
    /* @brief Define the size of hardware buffer */
    #define FSL_FEATURE_DAC_BUFFER_SIZE (2)
    /* @brief Define whether the buffer supports watermark event detection or not. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION (0)
    /* @brief Define whether the buffer supports watermark event 1 word before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_1_WORD (0)
    /* @brief Define whether the buffer supports watermark event 2 words before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_2_WORDS (0)
    /* @brief Define whether the buffer supports watermark event 3 words before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_3_WORDS (0)
    /* @brief Define whether the buffer supports watermark event 4 words before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_4_WORDS (0)
    /* @brief Define whether FIFO buffer mode is available or not. */
    #define FSL_FEATURE_DAC_HAS_BUFFER_FIFO_MODE (1)
    /* @brief Define whether swing buffer mode is available or not.. */
    #define FSL_FEATURE_DAC_HAS_BUFFER_SWING_MODE (0)
#elif defined(CPU_MKV10Z16VFM7) || defined(CPU_MKV10Z16VLC7) || defined(CPU_MKV10Z16VLF7) || defined(CPU_MKV10Z32VFM7) || \
    defined(CPU_MKV10Z32VLC7) || defined(CPU_MKV10Z32VLF7)
    /* @brief Define the size of hardware buffer */
    #define FSL_FEATURE_DAC_BUFFER_SIZE (2)
    /* @brief Define whether the buffer supports watermark event detection or not. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION (1)
    /* @brief Define whether the buffer supports watermark event 1 word before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_1_WORD (1)
    /* @brief Define whether the buffer supports watermark event 2 words before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_2_WORDS (1)
    /* @brief Define whether the buffer supports watermark event 3 words before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_3_WORDS (0)
    /* @brief Define whether the buffer supports watermark event 4 words before buffer upper limit. */
    #define FSL_FEATURE_DAC_HAS_WATERMARK_4_WORDS (0)
    /* @brief Define whether FIFO buffer mode is available or not. */
    #define FSL_FEATURE_DAC_HAS_BUFFER_FIFO_MODE (0)
    /* @brief Define whether swing buffer mode is available or not.. */
    #define FSL_FEATURE_DAC_HAS_BUFFER_SWING_MODE (0)
#else
    #error "No valid CPU defined!"
#endif

#endif /* __FSL_DAC_FEATURES_H__ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
