/*
 * Copyright (c) 2013-2014, Freescale Semiconductor, Inc.
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

/* MODULE pin_mux. */

#include "fsl_device_registers.h"
#include "fsl_port_hal.h"
#include "fsl_gpio_common.h"
#include "pin_mux.h"


void configure_gpio_pins(uint32_t instance)
{
  switch(instance) {    
    case 0:                             /* PTA */
      break;

    case 1:                             /* PTB */
	PORT_HAL_SetMuxMode(g_portBaseAddr[1],3u,kPortMuxAsGpio);
	PORT_HAL_SetMuxMode(g_portBaseAddr[1],17u,kPortMuxAsGpio);
      break;

    case 2:                             /* PTC */
      /* PORTB_PCR3 */
	PORT_HAL_SetMuxMode(g_portBaseAddr[2],0u,kPortMuxAsGpio);
      /* PORTB_PCR6 */
	PORT_HAL_SetMuxMode(g_portBaseAddr[2],1u,kPortMuxAsGpio);
	PORT_HAL_SetMuxMode(g_portBaseAddr[2],2u,kPortMuxAsGpio);
      break;

    case 3:                             /* PTD */
      /* PORTC_PCR5 */
     // PORT_HAL_SetMuxMode(g_portBaseAddr[3],5u,kPortMuxAsGpio);
      break;

    case 4:                             /* PTE */
      /* PORTE_PCR31 */
    PORT_HAL_SetMuxMode(g_portBaseAddr[4], 18u,kPortMuxAsGpio);
	PORT_HAL_SetMuxMode(g_portBaseAddr[4], 29u, kPortMuxAsGpio);
	PORT_HAL_SetMuxMode(g_portBaseAddr[4], 30u, kPortMuxAsGpio);
	PORT_HAL_SetMuxMode(g_portBaseAddr[4], 24u, kPortMuxAsGpio);
	PORT_HAL_SetMuxMode(g_portBaseAddr[4], 25u, kPortMuxAsGpio);
      break;

    default:
      break;
  }
}

void configure_i2c_pins(uint32_t instance)
{
  switch(instance) {
    case 0:                             /* I2C0 */
#if  defined(DEMO_I2C_ACCELEROMETER) 

      /* PORTE_PCR24 */
      PORT_HAL_SetMuxMode(g_portBaseAddr[4],24u,kPortMuxAlt5);
      /* PORTE_PCR25 */
      PORT_HAL_SetMuxMode(g_portBaseAddr[4],25u,kPortMuxAlt5);

#elif defined(DEMO_I2C_RTOS_BM_MASTER) || defined(DEMO_I2C_RTOS_BM_SLAVE) || \
      defined(DEMO_I2C_RTOS_FREERTOS) || defined(DEMO_I2C_RTOS_MQX) || \
      defined(DEMO_I2C_RTOS_UCOSII) || defined(DEMO_I2C_RTOS_UCOSIII)

      /* PORTB_PCR0 */
      PORT_HAL_SetMuxMode(g_portBaseAddr[1],0u,kPortMuxAlt2);
      /* Enable pull up */
      PORT_HAL_SetPullMode(g_portBaseAddr[1],0u,kPortPullUp);
      PORT_HAL_SetPullCmd(g_portBaseAddr[1],0u,true);

      /* PORTB_PCR1 */
      PORT_HAL_SetMuxMode(g_portBaseAddr[1],1u,kPortMuxAlt2);
      /* Enable pull up */
      PORT_HAL_SetPullMode(g_portBaseAddr[1],1u,kPortPullUp);
      PORT_HAL_SetPullCmd(g_portBaseAddr[1],1u,true);

#endif
      break;

    case 1:                             /* I2C1 */
      /* PORTE_PCR0 */
      PORT_HAL_SetMuxMode(g_portBaseAddr[4],0u,kPortMuxAlt6);
      /* Enable pull up */
      PORT_HAL_SetPullMode(g_portBaseAddr[4],0u,kPortPullUp);
      PORT_HAL_SetPullCmd(g_portBaseAddr[4],0u,true);
      
      /* PORTE_PCR1 */
      PORT_HAL_SetMuxMode(g_portBaseAddr[4],1u,kPortMuxAlt6);
      /* Enable pull up */
      PORT_HAL_SetPullMode(g_portBaseAddr[4],1u,kPortPullUp);
      PORT_HAL_SetPullCmd(g_portBaseAddr[4],1u,true);
      break;

    default:
      break;
  }
}

void configure_rtc_pins(uint32_t instance)
{
  /* PORTE_PCR0 */
  PORT_HAL_SetMuxMode(g_portBaseAddr[4],0u,kPortMuxAlt4);
}

void configure_spi_pins(uint32_t instance)
{
  switch(instance) {    
    case 0:                             /* SPI0 */
      /* PORTD_PCR0 */
      PORT_HAL_SetMuxMode(g_portBaseAddr[3],0u,kPortMuxAlt2);
      /* PORTD_PCR1 */
      PORT_HAL_SetMuxMode(g_portBaseAddr[3],1u,kPortMuxAlt2);
      /* PORTD_PCR2 */
      PORT_HAL_SetMuxMode(g_portBaseAddr[3],2u,kPortMuxAlt2);
      /* PORTD_PCR3 */
      PORT_HAL_SetMuxMode(g_portBaseAddr[3],3u,kPortMuxAlt2);
      break;

    default:
      break;
  }
}

void configure_lpuart_pins(uint32_t instance)
{
  switch(instance) {    
    case 0:                             /* LPUART0 */
      /* PORTA_PCR1 */
      PORT_HAL_SetMuxMode(g_portBaseAddr[0],1u,kPortMuxAlt2);
      /* PORTA_PCR2 */
      PORT_HAL_SetMuxMode(g_portBaseAddr[0],2u,kPortMuxAlt2);
      break;

    case 1:                             /* LPUART1 */
      /* PORTC_PCR3 */
      PORT_HAL_SetMuxMode(g_portBaseAddr[2],3u,kPortMuxAlt3);
      /* PORTC_PCR3 */
      PORT_HAL_SetMuxMode(g_portBaseAddr[2],4u,kPortMuxAlt3);
      break;
      
    default:
      break;
  }
}


void configure_uart_pins(uint32_t instance)
{
  switch(instance) {    
    case 0:                             /* UART2 */
      /* PORTD_PCR2 */
      PORT_HAL_SetMuxMode(g_portBaseAddr[3],2u,kPortMuxAlt3);
      /* PORTD_PCR3 */
      PORT_HAL_SetMuxMode(g_portBaseAddr[3],3u,kPortMuxAlt3);
      break;
      
    default:
      break;
  }
}

void configure_gpio_i2c_pins(uint32_t instance)
{
  PORT_HAL_SetMuxMode(g_portBaseAddr[4],24u,kPortMuxAsGpio);
  PORT_HAL_SetMuxMode(g_portBaseAddr[4],25u,kPortMuxAsGpio);
}

void configure_spi_cs0_pins(uint32_t instance)
{
  PORT_HAL_SetMuxMode(g_portBaseAddr[3],0u,kPortMuxAsGpio);
}

void configure_spi_cs1_pins(uint32_t instance)
{
  PORT_HAL_SetMuxMode(g_portBaseAddr[3],4u,kPortMuxAsGpio);
}

void configure_tpm_pins(uint32_t instance)
{
  switch(instance) {
    case 0:                             /* TPM0 */
        /* PTC1 TPM0 channel 0 */
        PORT_HAL_SetMuxMode(g_portBaseAddr[2], 1u,kPortMuxAlt4);
      break;
    default:
      break;
  }
}

void configure_cmp_pins(uint32_t instance)
{
  switch(instance) {
    case 0:                             /* CMP0 */
        /* PTE30 CMP0 input channel 4 */
        PORT_HAL_SetMuxMode(g_portBaseAddr[4],30u,kPortPinDisabled);
        /* PTE0 CMP0 output */
        PORT_HAL_SetMuxMode(g_portBaseAddr[4], 0u,kPortMuxAlt5);
        break;
    default:
        break;
    }
}

/* END pin_mux. */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
