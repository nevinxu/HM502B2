/*
** ###################################################################
**     Version:             rev. 1.0, 2014-05-14
**     Build:               b140715
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

#if !defined(__FSL_COP_FEATURES_H__)
#define __FSL_COP_FEATURES_H__

#if defined(CPU_MKL03Z32CAF4) || defined(CPU_MKL03Z8VFG4) || defined(CPU_MKL03Z16VFG4) || defined(CPU_MKL03Z32VFG4) || \
    defined(CPU_MKL03Z8VFK4) || defined(CPU_MKL03Z16VFK4) || defined(CPU_MKL03Z32VFK4) || defined(CPU_MKL17Z128VFM4) || \
    defined(CPU_MKL17Z256VFM4) || defined(CPU_MKL17Z128VFT4) || defined(CPU_MKL17Z256VFT4) || defined(CPU_MKL17Z128VLH4) || \
    defined(CPU_MKL17Z256VLH4) || defined(CPU_MKL17Z128VMP4) || defined(CPU_MKL17Z256VMP4) || defined(CPU_MKL17Z32VLH4) || \
    defined(CPU_MKL17Z64VLH4) || defined(CPU_MKL27Z128VFM4) || defined(CPU_MKL27Z256VFM4) || defined(CPU_MKL27Z128VFT4) || \
    defined(CPU_MKL27Z256VFT4) || defined(CPU_MKL27Z128VLH4) || defined(CPU_MKL27Z256VLH4) || defined(CPU_MKL27Z128VMP4) || \
    defined(CPU_MKL27Z256VMP4) || defined(CPU_MKL27Z32VLH4) || defined(CPU_MKL27Z64VLH4) || defined(CPU_MKL33Z128VLH4) || \
    defined(CPU_MKL33Z256VLH4) || defined(CPU_MKL33Z128VMP4) || defined(CPU_MKL33Z256VMP4) || defined(CPU_MKL43Z128VLH4) || \
    defined(CPU_MKL43Z256VLH4) || defined(CPU_MKL43Z128VMP4) || defined(CPU_MKL43Z256VMP4)
    /* @brief Has the COP Debug Enable bit (COPC[COPDBGEN]) */
    #define FSL_FEATURE_COP_HAS_DEBUG_ENABLE (1)
    /* @brief Has the COP Stop mode Enable bit (COPC[COPSTPEN]) */
    #define FSL_FEATURE_COP_HAS_STOP_ENABLE (1)
    /* @brief Has more clock sources like MCGIRC */
    #define FSL_FEATURE_COP_HAS_MORE_CLKSRC (1)
    /* @brief Has the timeout long and short mode bit (COPC[COPCLKS]) */
    #define FSL_FEATURE_COP_HAS_LONGTIME_MODE (1)
#else
    #error "No valid CPU defined!"
#endif

#endif /* __FSL_COP_FEATURES_H__ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
