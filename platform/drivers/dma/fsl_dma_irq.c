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
 
#include "fsl_dma_driver.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(KL43Z4_SERIES) || defined(KL33Z4_SERIES) || \
    defined(KL27Z4_SERIES) || defined(KL17Z4_SERIES) || \
    defined(KL25Z4_SERIES)
      
/* DMA IRQ handler with the same name in startup code */
void DMA0_IRQHandler(void)
{
    DMA_DRV_IRQhandler(0);
}

/* DMA IRQ handler with the same name in startup code */
void DMA1_IRQHandler(void)
{
    DMA_DRV_IRQhandler(1);
}

/* DMA IRQ handler with the same name in startup code */
void DMA2_IRQHandler(void)
{
    DMA_DRV_IRQhandler(2);
}

/* DMA IRQ handler with the same name in startup code */
void DMA3_IRQHandler(void)
{
    DMA_DRV_IRQhandler(3);
}

#endif
/*******************************************************************************
 * EOF
 ******************************************************************************/


