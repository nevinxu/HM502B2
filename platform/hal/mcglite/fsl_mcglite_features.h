/*
** ###################################################################
**     Version:             rev. 1.0, 2014-05-14
**     Build:               b140808
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

#if !defined(__FSL_MCGLITE_FEATURES_H__)
#define __FSL_MCGLITE_FEATURES_H__

#if defined(CPU_MKL03Z32CAF4) || defined(CPU_MKL03Z8VFG4) || defined(CPU_MKL03Z16VFG4) || defined(CPU_MKL03Z32VFG4) || \
    defined(CPU_MKL03Z8VFK4) || defined(CPU_MKL03Z16VFK4) || defined(CPU_MKL03Z32VFK4)
    /* @brief Defines that clock generator is MCG Lite. */
    #define FSL_FEATURE_MCGLITE_MCGLITE (1)
    /* @brief Has Crystal Oscillator Operation Mode Selection. */
    #define FSL_FEATURE_MCGLITE_HAS_HGO0 (0)
    /* @brief Has HCTRIM register available. */
    #define FSL_FEATURE_MCGLITE_HAS_HCTRIM (0)
    /* @brief Has HTTRIM register available. */
    #define FSL_FEATURE_MCGLITE_HAS_HTTRIM (0)
    /* @brief Has HFTRIM register available. */
    #define FSL_FEATURE_MCGLITE_HAS_HFTRIM (0)
    /* @brief Has LTRIMRNG register available. */
    #define FSL_FEATURE_MCGLITE_HAS_LTRIMRNG (0)
    /* @brief Has LFTRIM register available. */
    #define FSL_FEATURE_MCGLITE_HAS_LFTRIM (0)
    /* @brief Has LSTRIM register available. */
    #define FSL_FEATURE_MCGLITE_HAS_LSTRIM (0)
    /* @brief Has External Clock Source Frequency Range Selection. */
    #define FSL_FEATURE_MCGLITE_HAS_RANGE0 (0)
#elif defined(CPU_MKL17Z128VFM4) || defined(CPU_MKL17Z256VFM4) || defined(CPU_MKL17Z128VFT4) || defined(CPU_MKL17Z256VFT4) || \
    defined(CPU_MKL17Z128VLH4) || defined(CPU_MKL17Z256VLH4) || defined(CPU_MKL17Z128VMP4) || defined(CPU_MKL17Z256VMP4) || \
    defined(CPU_MKL27Z128VFM4) || defined(CPU_MKL27Z256VFM4) || defined(CPU_MKL27Z128VFT4) || defined(CPU_MKL27Z256VFT4) || \
    defined(CPU_MKL27Z128VLH4) || defined(CPU_MKL27Z256VLH4) || defined(CPU_MKL27Z128VMP4) || defined(CPU_MKL27Z256VMP4) || \
    defined(CPU_MKL33Z128VLH4) || defined(CPU_MKL33Z256VLH4) || defined(CPU_MKL33Z128VMP4) || defined(CPU_MKL33Z256VMP4) || \
    defined(CPU_MKL43Z128VLH4) || defined(CPU_MKL43Z256VLH4) || defined(CPU_MKL43Z128VMP4) || defined(CPU_MKL43Z256VMP4)
    /* @brief Defines that clock generator is MCG Lite. */
    #define FSL_FEATURE_MCGLITE_MCGLITE (1)
    /* @brief Has Crystal Oscillator Operation Mode Selection. */
    #define FSL_FEATURE_MCGLITE_HAS_HGO0 (1)
    /* @brief Has HCTRIM register available. */
    #define FSL_FEATURE_MCGLITE_HAS_HCTRIM (1)
    /* @brief Has HTTRIM register available. */
    #define FSL_FEATURE_MCGLITE_HAS_HTTRIM (1)
    /* @brief Has HFTRIM register available. */
    #define FSL_FEATURE_MCGLITE_HAS_HFTRIM (1)
    /* @brief Has LTRIMRNG register available. */
    #define FSL_FEATURE_MCGLITE_HAS_LTRIMRNG (1)
    /* @brief Has LFTRIM register available. */
    #define FSL_FEATURE_MCGLITE_HAS_LFTRIM (1)
    /* @brief Has LSTRIM register available. */
    #define FSL_FEATURE_MCGLITE_HAS_LSTRIM (1)
    /* @brief Has External Clock Source Frequency Range Selection. */
    #define FSL_FEATURE_MCGLITE_HAS_RANGE0 (1)
#elif defined(CPU_MKL17Z32VLH4) || defined(CPU_MKL17Z64VLH4) || defined(CPU_MKL27Z32VLH4) || defined(CPU_MKL27Z64VLH4)
    /* @brief Defines that clock generator is MCG Lite. */
    #define FSL_FEATURE_MCGLITE_MCGLITE (1)
    /* @brief Has Crystal Oscillator Operation Mode Selection. */
    #define FSL_FEATURE_MCGLITE_HAS_HGO0 (1)
    /* @brief Has HCTRIM register available. */
    #define FSL_FEATURE_MCGLITE_HAS_HCTRIM (0)
    /* @brief Has HTTRIM register available. */
    #define FSL_FEATURE_MCGLITE_HAS_HTTRIM (0)
    /* @brief Has HFTRIM register available. */
    #define FSL_FEATURE_MCGLITE_HAS_HFTRIM (0)
    /* @brief Has LTRIMRNG register available. */
    #define FSL_FEATURE_MCGLITE_HAS_LTRIMRNG (0)
    /* @brief Has LFTRIM register available. */
    #define FSL_FEATURE_MCGLITE_HAS_LFTRIM (0)
    /* @brief Has LSTRIM register available. */
    #define FSL_FEATURE_MCGLITE_HAS_LSTRIM (0)
    /* @brief Has External Clock Source Frequency Range Selection. */
    #define FSL_FEATURE_MCGLITE_HAS_RANGE0 (1)
#else
    #error "No valid CPU defined!"
#endif

#endif /* __FSL_MCGLITE_FEATURES_H__ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
