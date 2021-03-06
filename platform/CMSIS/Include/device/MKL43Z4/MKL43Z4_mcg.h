/*
** ###################################################################
**     Compilers:           Keil ARM C/C++ Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    KL43P64M48SF6RM, Rev.3, Aug 2014
**     Version:             rev. 1.3, 2014-08-21
**     Build:               b140821
**
**     Abstract:
**         Extension to the CMSIS register access layer header.
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
**     - rev. 1.0 (2014-03-27)
**         Initial version.
**     - rev. 1.1 (2014-05-26)
**         I2S registers TCR2/RCR2 and others were changed.
**         FLEXIO register FLEXIO_VERID has now bitfields: FEATURE, MINOR, MAJOR.
**         Names of the bitfields of the FLEXIO_SHIFTBUF have been changed to the appropriate register name e.g.: FLEXIO_SHIFTBUFBBS_SHIFTBUFBBS.
**         Peripheral_BASES macros has been changed to Peripheral_BASE_PTRS, e.g.: ADC_BASES to ADC_BASE_PTRS.
**         Clock configuration for high range external oscillator has been added.
**         RFSYS module access has been added.
**     - rev. 1.2 (2014-07-10)
**         GPIO - Renamed modules PTA,PTB,PTC,PTD,PTE to GPIOA,GPIOB,GPIOC,GPIOD,GPIOE.
**         UART0 - UART0 module renamed to UART2.
**         I2S - removed MDR register.
**     - rev. 1.3 (2014-08-21)
**         UART2 - Removed ED register.
**         UART2 - Removed MODEM register.
**         UART2 - Removed IR register.
**         UART2 - Removed PFIFO register.
**         UART2 - Removed CFIFO register.
**         UART2 - Removed SFIFO register.
**         UART2 - Removed TWFIFO register.
**         UART2 - Removed TCFIFO register.
**         UART2 - Removed RWFIFO register.
**         UART2 - Removed RCFIFO register.
**         USB - Removed bitfield REG_EN in CLK_RECOVER_IRC_EN register.
**         SIM - Changed bitfield value MCGIRCLK to LIRC_CLK of bitfield CLKOUTSEL in SOPT2 register.
**         SIM - Removed bitfield DIEID in SDID register.
**
** ###################################################################
*/

/*
 * WARNING! DO NOT EDIT THIS FILE DIRECTLY!
 *
 * This file was generated automatically and any changes may be lost.
 */
#ifndef __HW_MCG_REGISTERS_H__
#define __HW_MCG_REGISTERS_H__

#include "MKL43Z4.h"
#include "fsl_bitaccess.h"

/*
 * MKL43Z4 MCG
 *
 * Multipurpose Clock Generator Lite
 *
 * Registers defined in this header file:
 * - HW_MCG_C1 - MCG Control Register 1
 * - HW_MCG_C2 - MCG Control Register 2
 * - HW_MCG_S - MCG Status Register
 * - HW_MCG_SC - MCG Status and Control Register
 * - HW_MCG_HCTRIM - MCG High-frequency IRC Coarse Trim Register
 * - HW_MCG_HTTRIM - MCG High-frequency IRC Tempco (Temperature Coefficient) Trim Register
 * - HW_MCG_HFTRIM - MCG High-frequency IRC Fine Trim Register
 * - HW_MCG_MC - MCG Miscellaneous Control Register
 * - HW_MCG_LTRIMRNG - MCG Low-frequency IRC Trim Range Register
 * - HW_MCG_LFTRIM - MCG Low-frequency IRC8M Trim Register
 * - HW_MCG_LSTRIM - MCG Low-frequency IRC2M Trim Register
 *
 * - hw_mcg_t - Struct containing all module registers.
 */

#define HW_MCG_INSTANCE_COUNT (1U) /*!< Number of instances of the MCG module. */

/*******************************************************************************
 * HW_MCG_C1 - MCG Control Register 1
 ******************************************************************************/

/*!
 * @brief HW_MCG_C1 - MCG Control Register 1 (RW)
 *
 * Reset value: 0x40U
 */
typedef union _hw_mcg_c1
{
    uint8_t U;
    struct _hw_mcg_c1_bitfields
    {
        uint8_t IREFSTEN : 1;          /*!< [0] Internal Reference Stop Enable */
        uint8_t IRCLKEN : 1;           /*!< [1] Internal Reference Clock Enable */
        uint8_t RESERVED0 : 4;         /*!< [5:2]  */
        uint8_t CLKS : 2;              /*!< [7:6] Clock Source Select */
    } B;
} hw_mcg_c1_t;

/*!
 * @name Constants and macros for entire MCG_C1 register
 */
/*@{*/
#define HW_MCG_C1_ADDR(x)        ((x) + 0x0U)

#define HW_MCG_C1(x)             (*(__IO hw_mcg_c1_t *) HW_MCG_C1_ADDR(x))
#define HW_MCG_C1_RD(x)          (HW_MCG_C1(x).U)
#define HW_MCG_C1_WR(x, v)       (HW_MCG_C1(x).U = (v))
#define HW_MCG_C1_SET(x, v)      (BME_OR8(HW_MCG_C1_ADDR(x), (uint8_t)(v)))
#define HW_MCG_C1_CLR(x, v)      (BME_AND8(HW_MCG_C1_ADDR(x), (uint8_t)(~(v))))
#define HW_MCG_C1_TOG(x, v)      (BME_XOR8(HW_MCG_C1_ADDR(x), (uint8_t)(v)))
/*@}*/

/*
 * Constants & macros for individual MCG_C1 bitfields
 */

/*!
 * @name Register MCG_C1, field IREFSTEN[0] (RW)
 *
 * Controls whether the IRC source remains enabled when the MCG_Lite enters Stop
 * mode.
 *
 * Values:
 * - 0 - LIRC is disabled in Stop mode.
 * - 1 - LIRC is enabled in Stop mode, if IRCLKEN is set.
 */
/*@{*/
#define BP_MCG_C1_IREFSTEN   (0U)          /*!< Bit position for MCG_C1_IREFSTEN. */
#define BM_MCG_C1_IREFSTEN   (0x01U)       /*!< Bit mask for MCG_C1_IREFSTEN. */
#define BS_MCG_C1_IREFSTEN   (1U)          /*!< Bit field size in bits for MCG_C1_IREFSTEN. */

/*! @brief Read current value of the MCG_C1_IREFSTEN field. */
#define BR_MCG_C1_IREFSTEN(x) (BME_UBFX8(HW_MCG_C1_ADDR(x), BP_MCG_C1_IREFSTEN, BS_MCG_C1_IREFSTEN))

/*! @brief Format value for bitfield MCG_C1_IREFSTEN. */
#define BF_MCG_C1_IREFSTEN(v) ((uint8_t)((uint8_t)(v) << BP_MCG_C1_IREFSTEN) & BM_MCG_C1_IREFSTEN)

/*! @brief Set the IREFSTEN field to a new value. */
#define BW_MCG_C1_IREFSTEN(x, v) (BME_BFI8(HW_MCG_C1_ADDR(x), ((uint8_t)(v) << BP_MCG_C1_IREFSTEN), BP_MCG_C1_IREFSTEN, 1))
/*@}*/

/*!
 * @name Register MCG_C1, field IRCLKEN[1] (RW)
 *
 * Enables the IRC source.
 *
 * Values:
 * - 0 - LIRC is disabled.
 * - 1 - LIRC is enabled.
 */
/*@{*/
#define BP_MCG_C1_IRCLKEN    (1U)          /*!< Bit position for MCG_C1_IRCLKEN. */
#define BM_MCG_C1_IRCLKEN    (0x02U)       /*!< Bit mask for MCG_C1_IRCLKEN. */
#define BS_MCG_C1_IRCLKEN    (1U)          /*!< Bit field size in bits for MCG_C1_IRCLKEN. */

/*! @brief Read current value of the MCG_C1_IRCLKEN field. */
#define BR_MCG_C1_IRCLKEN(x) (BME_UBFX8(HW_MCG_C1_ADDR(x), BP_MCG_C1_IRCLKEN, BS_MCG_C1_IRCLKEN))

/*! @brief Format value for bitfield MCG_C1_IRCLKEN. */
#define BF_MCG_C1_IRCLKEN(v) ((uint8_t)((uint8_t)(v) << BP_MCG_C1_IRCLKEN) & BM_MCG_C1_IRCLKEN)

/*! @brief Set the IRCLKEN field to a new value. */
#define BW_MCG_C1_IRCLKEN(x, v) (BME_BFI8(HW_MCG_C1_ADDR(x), ((uint8_t)(v) << BP_MCG_C1_IRCLKEN), BP_MCG_C1_IRCLKEN, 1))
/*@}*/

/*!
 * @name Register MCG_C1, field CLKS[7:6] (RW)
 *
 * Selects the clock source for MCGOUTCLK.
 *
 * Values:
 * - 00 - Selects HIRC clock as the main clock source. This is HIRC mode.
 * - 01 - Selects LIRC clock as the main clock source. This is LIRC2M or LIRC8M
 *     mode.
 * - 10 - Selects external clock as the main clock source. This is EXT mode.
 * - 11 - Reserved. Writing 11 takes no effect.
 */
/*@{*/
#define BP_MCG_C1_CLKS       (6U)          /*!< Bit position for MCG_C1_CLKS. */
#define BM_MCG_C1_CLKS       (0xC0U)       /*!< Bit mask for MCG_C1_CLKS. */
#define BS_MCG_C1_CLKS       (2U)          /*!< Bit field size in bits for MCG_C1_CLKS. */

/*! @brief Read current value of the MCG_C1_CLKS field. */
#define BR_MCG_C1_CLKS(x)    (BME_UBFX8(HW_MCG_C1_ADDR(x), BP_MCG_C1_CLKS, BS_MCG_C1_CLKS))

/*! @brief Format value for bitfield MCG_C1_CLKS. */
#define BF_MCG_C1_CLKS(v)    ((uint8_t)((uint8_t)(v) << BP_MCG_C1_CLKS) & BM_MCG_C1_CLKS)

/*! @brief Set the CLKS field to a new value. */
#define BW_MCG_C1_CLKS(x, v) (BME_BFI8(HW_MCG_C1_ADDR(x), ((uint8_t)(v) << BP_MCG_C1_CLKS), BP_MCG_C1_CLKS, 2))
/*@}*/

/*******************************************************************************
 * HW_MCG_C2 - MCG Control Register 2
 ******************************************************************************/

/*!
 * @brief HW_MCG_C2 - MCG Control Register 2 (RW)
 *
 * Reset value: 0x01U
 */
typedef union _hw_mcg_c2
{
    uint8_t U;
    struct _hw_mcg_c2_bitfields
    {
        uint8_t IRCS : 1;              /*!< [0] Low-frequency Internal Reference Clock
                                        * Select */
        uint8_t RESERVED0 : 1;         /*!< [1]  */
        uint8_t EREFS0 : 1;            /*!< [2] External Clock Source Select */
        uint8_t HGO0 : 1;              /*!< [3] Crystal Oscillator Operation Mode Select */
        uint8_t RANGE0 : 2;            /*!< [5:4] External Clock Source Frequency Range
                                        * Select */
        uint8_t RESERVED1 : 2;         /*!< [7:6]  */
    } B;
} hw_mcg_c2_t;

/*!
 * @name Constants and macros for entire MCG_C2 register
 */
/*@{*/
#define HW_MCG_C2_ADDR(x)        ((x) + 0x1U)

#define HW_MCG_C2(x)             (*(__IO hw_mcg_c2_t *) HW_MCG_C2_ADDR(x))
#define HW_MCG_C2_RD(x)          (HW_MCG_C2(x).U)
#define HW_MCG_C2_WR(x, v)       (HW_MCG_C2(x).U = (v))
#define HW_MCG_C2_SET(x, v)      (BME_OR8(HW_MCG_C2_ADDR(x), (uint8_t)(v)))
#define HW_MCG_C2_CLR(x, v)      (BME_AND8(HW_MCG_C2_ADDR(x), (uint8_t)(~(v))))
#define HW_MCG_C2_TOG(x, v)      (BME_XOR8(HW_MCG_C2_ADDR(x), (uint8_t)(v)))
/*@}*/

/*
 * Constants & macros for individual MCG_C2 bitfields
 */

/*!
 * @name Register MCG_C2, field IRCS[0] (RW)
 *
 * Controls the LIRC to work at 2 MHz or 8 MHz mode.
 *
 * Values:
 * - 0 - LIRC is in 2 MHz mode.
 * - 1 - LIRC is in 8 MHz mode.
 */
/*@{*/
#define BP_MCG_C2_IRCS       (0U)          /*!< Bit position for MCG_C2_IRCS. */
#define BM_MCG_C2_IRCS       (0x01U)       /*!< Bit mask for MCG_C2_IRCS. */
#define BS_MCG_C2_IRCS       (1U)          /*!< Bit field size in bits for MCG_C2_IRCS. */

/*! @brief Read current value of the MCG_C2_IRCS field. */
#define BR_MCG_C2_IRCS(x)    (BME_UBFX8(HW_MCG_C2_ADDR(x), BP_MCG_C2_IRCS, BS_MCG_C2_IRCS))

/*! @brief Format value for bitfield MCG_C2_IRCS. */
#define BF_MCG_C2_IRCS(v)    ((uint8_t)((uint8_t)(v) << BP_MCG_C2_IRCS) & BM_MCG_C2_IRCS)

/*! @brief Set the IRCS field to a new value. */
#define BW_MCG_C2_IRCS(x, v) (BME_BFI8(HW_MCG_C2_ADDR(x), ((uint8_t)(v) << BP_MCG_C2_IRCS), BP_MCG_C2_IRCS, 1))
/*@}*/

/*!
 * @name Register MCG_C2, field EREFS0[2] (RW)
 *
 * Selects the source for the external reference clock. See the Oscillator (OSC)
 * chapter for more details.
 *
 * Values:
 * - 0 - External clock requested.
 * - 1 - Oscillator requested.
 */
/*@{*/
#define BP_MCG_C2_EREFS0     (2U)          /*!< Bit position for MCG_C2_EREFS0. */
#define BM_MCG_C2_EREFS0     (0x04U)       /*!< Bit mask for MCG_C2_EREFS0. */
#define BS_MCG_C2_EREFS0     (1U)          /*!< Bit field size in bits for MCG_C2_EREFS0. */

/*! @brief Read current value of the MCG_C2_EREFS0 field. */
#define BR_MCG_C2_EREFS0(x)  (BME_UBFX8(HW_MCG_C2_ADDR(x), BP_MCG_C2_EREFS0, BS_MCG_C2_EREFS0))

/*! @brief Format value for bitfield MCG_C2_EREFS0. */
#define BF_MCG_C2_EREFS0(v)  ((uint8_t)((uint8_t)(v) << BP_MCG_C2_EREFS0) & BM_MCG_C2_EREFS0)

/*! @brief Set the EREFS0 field to a new value. */
#define BW_MCG_C2_EREFS0(x, v) (BME_BFI8(HW_MCG_C2_ADDR(x), ((uint8_t)(v) << BP_MCG_C2_EREFS0), BP_MCG_C2_EREFS0, 1))
/*@}*/

/*!
 * @name Register MCG_C2, field HGO0[3] (RW)
 *
 * Selects the crystal oscillator mode of operation. See the Oscillator (OSC)
 * chapter for more details.
 *
 * Values:
 * - 0 - Configure crystal oscillator for low-power operation.
 * - 1 - Configure crystal oscillator for high-gain operation.
 */
/*@{*/
#define BP_MCG_C2_HGO0       (3U)          /*!< Bit position for MCG_C2_HGO0. */
#define BM_MCG_C2_HGO0       (0x08U)       /*!< Bit mask for MCG_C2_HGO0. */
#define BS_MCG_C2_HGO0       (1U)          /*!< Bit field size in bits for MCG_C2_HGO0. */

/*! @brief Read current value of the MCG_C2_HGO0 field. */
#define BR_MCG_C2_HGO0(x)    (BME_UBFX8(HW_MCG_C2_ADDR(x), BP_MCG_C2_HGO0, BS_MCG_C2_HGO0))

/*! @brief Format value for bitfield MCG_C2_HGO0. */
#define BF_MCG_C2_HGO0(v)    ((uint8_t)((uint8_t)(v) << BP_MCG_C2_HGO0) & BM_MCG_C2_HGO0)

/*! @brief Set the HGO0 field to a new value. */
#define BW_MCG_C2_HGO0(x, v) (BME_BFI8(HW_MCG_C2_ADDR(x), ((uint8_t)(v) << BP_MCG_C2_HGO0), BP_MCG_C2_HGO0, 1))
/*@}*/

/*!
 * @name Register MCG_C2, field RANGE0[5:4] (RW)
 *
 * Selects the frequency for the crystal oscillator or the external clock
 * source. See the Oscillator (OSC) chapter for more details and refer to the chip
 * datasheet for the frequency ranges used.
 *
 * Values:
 * - 00 - Low frequency range selected for the crystal oscillator or the
 *     external clock source.
 * - 01 - High frequency range selected for the crystal oscillator or the
 *     external clock source.
 * - 10 - Very high frequency range selected for the crystal oscillator or the
 *     external clock source.
 * - 11 - Very high frequency range selected for the crystal oscillator or the
 *     external clock source. Same effect as 10.
 */
/*@{*/
#define BP_MCG_C2_RANGE0     (4U)          /*!< Bit position for MCG_C2_RANGE0. */
#define BM_MCG_C2_RANGE0     (0x30U)       /*!< Bit mask for MCG_C2_RANGE0. */
#define BS_MCG_C2_RANGE0     (2U)          /*!< Bit field size in bits for MCG_C2_RANGE0. */

/*! @brief Read current value of the MCG_C2_RANGE0 field. */
#define BR_MCG_C2_RANGE0(x)  (BME_UBFX8(HW_MCG_C2_ADDR(x), BP_MCG_C2_RANGE0, BS_MCG_C2_RANGE0))

/*! @brief Format value for bitfield MCG_C2_RANGE0. */
#define BF_MCG_C2_RANGE0(v)  ((uint8_t)((uint8_t)(v) << BP_MCG_C2_RANGE0) & BM_MCG_C2_RANGE0)

/*! @brief Set the RANGE0 field to a new value. */
#define BW_MCG_C2_RANGE0(x, v) (BME_BFI8(HW_MCG_C2_ADDR(x), ((uint8_t)(v) << BP_MCG_C2_RANGE0), BP_MCG_C2_RANGE0, 2))
/*@}*/

/*******************************************************************************
 * HW_MCG_S - MCG Status Register
 ******************************************************************************/

/*!
 * @brief HW_MCG_S - MCG Status Register (RO)
 *
 * Reset value: 0x04U
 */
typedef union _hw_mcg_s
{
    uint8_t U;
    struct _hw_mcg_s_bitfields
    {
        uint8_t RESERVED0 : 1;         /*!< [0]  */
        uint8_t OSCINIT0 : 1;          /*!< [1] OSC Initialization Status */
        uint8_t CLKST : 2;             /*!< [3:2] Clock Mode Status */
        uint8_t RESERVED1 : 4;         /*!< [7:4]  */
    } B;
} hw_mcg_s_t;

/*!
 * @name Constants and macros for entire MCG_S register
 */
/*@{*/
#define HW_MCG_S_ADDR(x)         ((x) + 0x6U)

#define HW_MCG_S(x)              (*(__I hw_mcg_s_t *) HW_MCG_S_ADDR(x))
#define HW_MCG_S_RD(x)           (HW_MCG_S(x).U)
/*@}*/

/*
 * Constants & macros for individual MCG_S bitfields
 */

/*!
 * @name Register MCG_S, field OSCINIT0[1] (RO)
 *
 * This flag, which resets to 0, is set to 1 after the initialization cycles of
 * the crystal oscillator clock are completed. After being set, the bit is
 * cleared to 0 if the OSC is subsequently disabled. See the Oscillator (OSC) chapter
 * for more information.
 *
 * Values:
 * - 0 - OSC is not ready.
 * - 1 - OSC clock is ready.
 */
/*@{*/
#define BP_MCG_S_OSCINIT0    (1U)          /*!< Bit position for MCG_S_OSCINIT0. */
#define BM_MCG_S_OSCINIT0    (0x02U)       /*!< Bit mask for MCG_S_OSCINIT0. */
#define BS_MCG_S_OSCINIT0    (1U)          /*!< Bit field size in bits for MCG_S_OSCINIT0. */

/*! @brief Read current value of the MCG_S_OSCINIT0 field. */
#define BR_MCG_S_OSCINIT0(x) (BME_UBFX8(HW_MCG_S_ADDR(x), BP_MCG_S_OSCINIT0, BS_MCG_S_OSCINIT0))
/*@}*/

/*!
 * @name Register MCG_S, field CLKST[3:2] (RO)
 *
 * Indicates the current clock mode. This field does not update immediately
 * after a write to MCG_C1[CLKS] due to internal synchronization between clock
 * domains.
 *
 * Values:
 * - 00 - HIRC clock is selected as the main clock source, and MCG_Lite works at
 *     HIRC mode.
 * - 01 - LIRC clock is selected as the main clock source, and MCG_Lite works at
 *     LIRC2M or LIRC8M mode.
 * - 10 - External clock is selected as the main clock source, and MCG_Lite
 *     works at EXT mode.
 * - 11 - Reserved.
 */
/*@{*/
#define BP_MCG_S_CLKST       (2U)          /*!< Bit position for MCG_S_CLKST. */
#define BM_MCG_S_CLKST       (0x0CU)       /*!< Bit mask for MCG_S_CLKST. */
#define BS_MCG_S_CLKST       (2U)          /*!< Bit field size in bits for MCG_S_CLKST. */

/*! @brief Read current value of the MCG_S_CLKST field. */
#define BR_MCG_S_CLKST(x)    (BME_UBFX8(HW_MCG_S_ADDR(x), BP_MCG_S_CLKST, BS_MCG_S_CLKST))
/*@}*/

/*******************************************************************************
 * HW_MCG_SC - MCG Status and Control Register
 ******************************************************************************/

/*!
 * @brief HW_MCG_SC - MCG Status and Control Register (RW)
 *
 * Reset value: 0x00U
 */
typedef union _hw_mcg_sc
{
    uint8_t U;
    struct _hw_mcg_sc_bitfields
    {
        uint8_t RESERVED0 : 1;         /*!< [0]  */
        uint8_t FCRDIV : 3;            /*!< [3:1] Low-frequency Internal Reference Clock
                                        * Divider */
        uint8_t RESERVED1 : 4;         /*!< [7:4]  */
    } B;
} hw_mcg_sc_t;

/*!
 * @name Constants and macros for entire MCG_SC register
 */
/*@{*/
#define HW_MCG_SC_ADDR(x)        ((x) + 0x8U)

#define HW_MCG_SC(x)             (*(__IO hw_mcg_sc_t *) HW_MCG_SC_ADDR(x))
#define HW_MCG_SC_RD(x)          (HW_MCG_SC(x).U)
#define HW_MCG_SC_WR(x, v)       (HW_MCG_SC(x).U = (v))
#define HW_MCG_SC_SET(x, v)      (BME_OR8(HW_MCG_SC_ADDR(x), (uint8_t)(v)))
#define HW_MCG_SC_CLR(x, v)      (BME_AND8(HW_MCG_SC_ADDR(x), (uint8_t)(~(v))))
#define HW_MCG_SC_TOG(x, v)      (BME_XOR8(HW_MCG_SC_ADDR(x), (uint8_t)(v)))
/*@}*/

/*
 * Constants & macros for individual MCG_SC bitfields
 */

/*!
 * @name Register MCG_SC, field FCRDIV[3:1] (RW)
 *
 * Selects the factor value to divide the LIRC source.
 *
 * Values:
 * - 000 - Division factor is 1.
 * - 001 - Division factor is 2.
 * - 010 - Division factor is 4.
 * - 011 - Division factor is 8.
 * - 100 - Division factor is 16.
 * - 101 - Division factor is 32.
 * - 110 - Division factor is 64.
 * - 111 - Division factor is 128.
 */
/*@{*/
#define BP_MCG_SC_FCRDIV     (1U)          /*!< Bit position for MCG_SC_FCRDIV. */
#define BM_MCG_SC_FCRDIV     (0x0EU)       /*!< Bit mask for MCG_SC_FCRDIV. */
#define BS_MCG_SC_FCRDIV     (3U)          /*!< Bit field size in bits for MCG_SC_FCRDIV. */

/*! @brief Read current value of the MCG_SC_FCRDIV field. */
#define BR_MCG_SC_FCRDIV(x)  (BME_UBFX8(HW_MCG_SC_ADDR(x), BP_MCG_SC_FCRDIV, BS_MCG_SC_FCRDIV))

/*! @brief Format value for bitfield MCG_SC_FCRDIV. */
#define BF_MCG_SC_FCRDIV(v)  ((uint8_t)((uint8_t)(v) << BP_MCG_SC_FCRDIV) & BM_MCG_SC_FCRDIV)

/*! @brief Set the FCRDIV field to a new value. */
#define BW_MCG_SC_FCRDIV(x, v) (BME_BFI8(HW_MCG_SC_ADDR(x), ((uint8_t)(v) << BP_MCG_SC_FCRDIV), BP_MCG_SC_FCRDIV, 3))
/*@}*/

/*******************************************************************************
 * HW_MCG_HCTRIM - MCG High-frequency IRC Coarse Trim Register
 ******************************************************************************/

/*!
 * @brief HW_MCG_HCTRIM - MCG High-frequency IRC Coarse Trim Register (RO)
 *
 * Reset value: 0x00U
 */
typedef union _hw_mcg_hctrim
{
    uint8_t U;
    struct _hw_mcg_hctrim_bitfields
    {
        uint8_t COARSE_TRIM : 6;       /*!< [5:0] High-frequency IRC Coarse Trim */
        uint8_t RESERVED0 : 2;         /*!< [7:6]  */
    } B;
} hw_mcg_hctrim_t;

/*!
 * @name Constants and macros for entire MCG_HCTRIM register
 */
/*@{*/
#define HW_MCG_HCTRIM_ADDR(x)    ((x) + 0x14U)

#define HW_MCG_HCTRIM(x)         (*(__I hw_mcg_hctrim_t *) HW_MCG_HCTRIM_ADDR(x))
#define HW_MCG_HCTRIM_RD(x)      (HW_MCG_HCTRIM(x).U)
/*@}*/

/*
 * Constants & macros for individual MCG_HCTRIM bitfields
 */

/*!
 * @name Register MCG_HCTRIM, field COARSE_TRIM[5:0] (RO)
 *
 * Loads from a factory programmed location when out of reset.
 */
/*@{*/
#define BP_MCG_HCTRIM_COARSE_TRIM (0U)     /*!< Bit position for MCG_HCTRIM_COARSE_TRIM. */
#define BM_MCG_HCTRIM_COARSE_TRIM (0x3FU)  /*!< Bit mask for MCG_HCTRIM_COARSE_TRIM. */
#define BS_MCG_HCTRIM_COARSE_TRIM (6U)     /*!< Bit field size in bits for MCG_HCTRIM_COARSE_TRIM. */

/*! @brief Read current value of the MCG_HCTRIM_COARSE_TRIM field. */
#define BR_MCG_HCTRIM_COARSE_TRIM(x) (BME_UBFX8(HW_MCG_HCTRIM_ADDR(x), BP_MCG_HCTRIM_COARSE_TRIM, BS_MCG_HCTRIM_COARSE_TRIM))
/*@}*/

/*******************************************************************************
 * HW_MCG_HTTRIM - MCG High-frequency IRC Tempco (Temperature Coefficient) Trim Register
 ******************************************************************************/

/*!
 * @brief HW_MCG_HTTRIM - MCG High-frequency IRC Tempco (Temperature Coefficient) Trim Register (RO)
 *
 * Reset value: 0x00U
 */
typedef union _hw_mcg_httrim
{
    uint8_t U;
    struct _hw_mcg_httrim_bitfields
    {
        uint8_t TEMPCO_TRIM : 5;       /*!< [4:0] High-frequency IRC Tempco Trim */
        uint8_t RESERVED0 : 3;         /*!< [7:5]  */
    } B;
} hw_mcg_httrim_t;

/*!
 * @name Constants and macros for entire MCG_HTTRIM register
 */
/*@{*/
#define HW_MCG_HTTRIM_ADDR(x)    ((x) + 0x15U)

#define HW_MCG_HTTRIM(x)         (*(__I hw_mcg_httrim_t *) HW_MCG_HTTRIM_ADDR(x))
#define HW_MCG_HTTRIM_RD(x)      (HW_MCG_HTTRIM(x).U)
/*@}*/

/*
 * Constants & macros for individual MCG_HTTRIM bitfields
 */

/*!
 * @name Register MCG_HTTRIM, field TEMPCO_TRIM[4:0] (RO)
 *
 * Loads from a factory programmed location when out of reset.
 */
/*@{*/
#define BP_MCG_HTTRIM_TEMPCO_TRIM (0U)     /*!< Bit position for MCG_HTTRIM_TEMPCO_TRIM. */
#define BM_MCG_HTTRIM_TEMPCO_TRIM (0x1FU)  /*!< Bit mask for MCG_HTTRIM_TEMPCO_TRIM. */
#define BS_MCG_HTTRIM_TEMPCO_TRIM (5U)     /*!< Bit field size in bits for MCG_HTTRIM_TEMPCO_TRIM. */

/*! @brief Read current value of the MCG_HTTRIM_TEMPCO_TRIM field. */
#define BR_MCG_HTTRIM_TEMPCO_TRIM(x) (BME_UBFX8(HW_MCG_HTTRIM_ADDR(x), BP_MCG_HTTRIM_TEMPCO_TRIM, BS_MCG_HTTRIM_TEMPCO_TRIM))
/*@}*/

/*******************************************************************************
 * HW_MCG_HFTRIM - MCG High-frequency IRC Fine Trim Register
 ******************************************************************************/

/*!
 * @brief HW_MCG_HFTRIM - MCG High-frequency IRC Fine Trim Register (RO)
 *
 * Reset value: 0x00U
 */
typedef union _hw_mcg_hftrim
{
    uint8_t U;
    struct _hw_mcg_hftrim_bitfields
    {
        uint8_t FINE_TRIM : 7;         /*!< [6:0] High-frequency IRC Fine Trim */
        uint8_t RESERVED0 : 1;         /*!< [7]  */
    } B;
} hw_mcg_hftrim_t;

/*!
 * @name Constants and macros for entire MCG_HFTRIM register
 */
/*@{*/
#define HW_MCG_HFTRIM_ADDR(x)    ((x) + 0x16U)

#define HW_MCG_HFTRIM(x)         (*(__I hw_mcg_hftrim_t *) HW_MCG_HFTRIM_ADDR(x))
#define HW_MCG_HFTRIM_RD(x)      (HW_MCG_HFTRIM(x).U)
/*@}*/

/*
 * Constants & macros for individual MCG_HFTRIM bitfields
 */

/*!
 * @name Register MCG_HFTRIM, field FINE_TRIM[6:0] (RO)
 *
 * Loads from a factory programmed location when out of reset.
 */
/*@{*/
#define BP_MCG_HFTRIM_FINE_TRIM (0U)       /*!< Bit position for MCG_HFTRIM_FINE_TRIM. */
#define BM_MCG_HFTRIM_FINE_TRIM (0x7FU)    /*!< Bit mask for MCG_HFTRIM_FINE_TRIM. */
#define BS_MCG_HFTRIM_FINE_TRIM (7U)       /*!< Bit field size in bits for MCG_HFTRIM_FINE_TRIM. */

/*! @brief Read current value of the MCG_HFTRIM_FINE_TRIM field. */
#define BR_MCG_HFTRIM_FINE_TRIM(x) (BME_UBFX8(HW_MCG_HFTRIM_ADDR(x), BP_MCG_HFTRIM_FINE_TRIM, BS_MCG_HFTRIM_FINE_TRIM))
/*@}*/

/*******************************************************************************
 * HW_MCG_MC - MCG Miscellaneous Control Register
 ******************************************************************************/

/*!
 * @brief HW_MCG_MC - MCG Miscellaneous Control Register (RW)
 *
 * Reset value: 0x00U
 */
typedef union _hw_mcg_mc
{
    uint8_t U;
    struct _hw_mcg_mc_bitfields
    {
        uint8_t LIRC_DIV2 : 3;         /*!< [2:0] Second Low-frequency Internal
                                        * Reference Clock Divider */
        uint8_t RESERVED0 : 4;         /*!< [6:3]  */
        uint8_t HIRCEN : 1;            /*!< [7] High-frequency IRC Enable */
    } B;
} hw_mcg_mc_t;

/*!
 * @name Constants and macros for entire MCG_MC register
 */
/*@{*/
#define HW_MCG_MC_ADDR(x)        ((x) + 0x18U)

#define HW_MCG_MC(x)             (*(__IO hw_mcg_mc_t *) HW_MCG_MC_ADDR(x))
#define HW_MCG_MC_RD(x)          (HW_MCG_MC(x).U)
#define HW_MCG_MC_WR(x, v)       (HW_MCG_MC(x).U = (v))
#define HW_MCG_MC_SET(x, v)      (BME_OR8(HW_MCG_MC_ADDR(x), (uint8_t)(v)))
#define HW_MCG_MC_CLR(x, v)      (BME_AND8(HW_MCG_MC_ADDR(x), (uint8_t)(~(v))))
#define HW_MCG_MC_TOG(x, v)      (BME_XOR8(HW_MCG_MC_ADDR(x), (uint8_t)(v)))
/*@}*/

/*
 * Constants & macros for individual MCG_MC bitfields
 */

/*!
 * @name Register MCG_MC, field LIRC_DIV2[2:0] (RW)
 *
 * Selects the factor value to further divide the LIRC source.
 *
 * Values:
 * - 000 - Division factor is 1.
 * - 001 - Division factor is 2.
 * - 010 - Division factor is 4.
 * - 011 - Division factor is 8.
 * - 100 - Division factor is 16.
 * - 101 - Division factor is 32.
 * - 110 - Division factor is 64.
 * - 111 - Division factor is 128.
 */
/*@{*/
#define BP_MCG_MC_LIRC_DIV2  (0U)          /*!< Bit position for MCG_MC_LIRC_DIV2. */
#define BM_MCG_MC_LIRC_DIV2  (0x07U)       /*!< Bit mask for MCG_MC_LIRC_DIV2. */
#define BS_MCG_MC_LIRC_DIV2  (3U)          /*!< Bit field size in bits for MCG_MC_LIRC_DIV2. */

/*! @brief Read current value of the MCG_MC_LIRC_DIV2 field. */
#define BR_MCG_MC_LIRC_DIV2(x) (BME_UBFX8(HW_MCG_MC_ADDR(x), BP_MCG_MC_LIRC_DIV2, BS_MCG_MC_LIRC_DIV2))

/*! @brief Format value for bitfield MCG_MC_LIRC_DIV2. */
#define BF_MCG_MC_LIRC_DIV2(v) ((uint8_t)((uint8_t)(v) << BP_MCG_MC_LIRC_DIV2) & BM_MCG_MC_LIRC_DIV2)

/*! @brief Set the LIRC_DIV2 field to a new value. */
#define BW_MCG_MC_LIRC_DIV2(x, v) (BME_BFI8(HW_MCG_MC_ADDR(x), ((uint8_t)(v) << BP_MCG_MC_LIRC_DIV2), BP_MCG_MC_LIRC_DIV2, 3))
/*@}*/

/*!
 * @name Register MCG_MC, field HIRCEN[7] (RW)
 *
 * Enables the HIRC, even when MCG_Lite is not working at HIRC mode.
 *
 * Values:
 * - 0 - HIRC source is not enabled.
 * - 1 - HIRC source is enabled.
 */
/*@{*/
#define BP_MCG_MC_HIRCEN     (7U)          /*!< Bit position for MCG_MC_HIRCEN. */
#define BM_MCG_MC_HIRCEN     (0x80U)       /*!< Bit mask for MCG_MC_HIRCEN. */
#define BS_MCG_MC_HIRCEN     (1U)          /*!< Bit field size in bits for MCG_MC_HIRCEN. */

/*! @brief Read current value of the MCG_MC_HIRCEN field. */
#define BR_MCG_MC_HIRCEN(x)  (BME_UBFX8(HW_MCG_MC_ADDR(x), BP_MCG_MC_HIRCEN, BS_MCG_MC_HIRCEN))

/*! @brief Format value for bitfield MCG_MC_HIRCEN. */
#define BF_MCG_MC_HIRCEN(v)  ((uint8_t)((uint8_t)(v) << BP_MCG_MC_HIRCEN) & BM_MCG_MC_HIRCEN)

/*! @brief Set the HIRCEN field to a new value. */
#define BW_MCG_MC_HIRCEN(x, v) (BME_BFI8(HW_MCG_MC_ADDR(x), ((uint8_t)(v) << BP_MCG_MC_HIRCEN), BP_MCG_MC_HIRCEN, 1))
/*@}*/

/*******************************************************************************
 * HW_MCG_LTRIMRNG - MCG Low-frequency IRC Trim Range Register
 ******************************************************************************/

/*!
 * @brief HW_MCG_LTRIMRNG - MCG Low-frequency IRC Trim Range Register (RO)
 *
 * Reset value: 0x00U
 */
typedef union _hw_mcg_ltrimrng
{
    uint8_t U;
    struct _hw_mcg_ltrimrng_bitfields
    {
        uint8_t STRIMRNG : 2;          /*!< [1:0] LIRC Slow TRIM (2 MHz) Range */
        uint8_t FTRIMRNG : 2;          /*!< [3:2] LIRC Fast TRIM (8 MHz) Range */
        uint8_t RESERVED0 : 4;         /*!< [7:4]  */
    } B;
} hw_mcg_ltrimrng_t;

/*!
 * @name Constants and macros for entire MCG_LTRIMRNG register
 */
/*@{*/
#define HW_MCG_LTRIMRNG_ADDR(x)  ((x) + 0x19U)

#define HW_MCG_LTRIMRNG(x)       (*(__I hw_mcg_ltrimrng_t *) HW_MCG_LTRIMRNG_ADDR(x))
#define HW_MCG_LTRIMRNG_RD(x)    (HW_MCG_LTRIMRNG(x).U)
/*@}*/

/*
 * Constants & macros for individual MCG_LTRIMRNG bitfields
 */

/*!
 * @name Register MCG_LTRIMRNG, field STRIMRNG[1:0] (RO)
 *
 * Trim effect level of LSTRIM can be enlarged by setting this field. Loads from
 * a factory programmed location when out of reset.
 *
 * Values:
 * - 00 - Frequency shift by 10%.
 * - 01 - No frequency shift.
 * - 10 - No frequency shift.
 * - 11 - Frequency shift by -10%.
 */
/*@{*/
#define BP_MCG_LTRIMRNG_STRIMRNG (0U)      /*!< Bit position for MCG_LTRIMRNG_STRIMRNG. */
#define BM_MCG_LTRIMRNG_STRIMRNG (0x03U)   /*!< Bit mask for MCG_LTRIMRNG_STRIMRNG. */
#define BS_MCG_LTRIMRNG_STRIMRNG (2U)      /*!< Bit field size in bits for MCG_LTRIMRNG_STRIMRNG. */

/*! @brief Read current value of the MCG_LTRIMRNG_STRIMRNG field. */
#define BR_MCG_LTRIMRNG_STRIMRNG(x) (BME_UBFX8(HW_MCG_LTRIMRNG_ADDR(x), BP_MCG_LTRIMRNG_STRIMRNG, BS_MCG_LTRIMRNG_STRIMRNG))
/*@}*/

/*!
 * @name Register MCG_LTRIMRNG, field FTRIMRNG[3:2] (RO)
 *
 * Trim effect level of LFTRIM can be enlarged by setting this field. Loads from
 * a factory programmed location when out of reset.
 *
 * Values:
 * - 00 - Frequency shift by 10%.
 * - 01 - No frequency shift.
 * - 10 - No frequency shift.
 * - 11 - Frequency shift by -10%.
 */
/*@{*/
#define BP_MCG_LTRIMRNG_FTRIMRNG (2U)      /*!< Bit position for MCG_LTRIMRNG_FTRIMRNG. */
#define BM_MCG_LTRIMRNG_FTRIMRNG (0x0CU)   /*!< Bit mask for MCG_LTRIMRNG_FTRIMRNG. */
#define BS_MCG_LTRIMRNG_FTRIMRNG (2U)      /*!< Bit field size in bits for MCG_LTRIMRNG_FTRIMRNG. */

/*! @brief Read current value of the MCG_LTRIMRNG_FTRIMRNG field. */
#define BR_MCG_LTRIMRNG_FTRIMRNG(x) (BME_UBFX8(HW_MCG_LTRIMRNG_ADDR(x), BP_MCG_LTRIMRNG_FTRIMRNG, BS_MCG_LTRIMRNG_FTRIMRNG))
/*@}*/

/*******************************************************************************
 * HW_MCG_LFTRIM - MCG Low-frequency IRC8M Trim Register
 ******************************************************************************/

/*!
 * @brief HW_MCG_LFTRIM - MCG Low-frequency IRC8M Trim Register (RO)
 *
 * Reset value: 0x00U
 */
typedef union _hw_mcg_lftrim
{
    uint8_t U;
    struct _hw_mcg_lftrim_bitfields
    {
        uint8_t LIRC_FTRIM : 7;        /*!< [6:0] LIRC8M TRIM */
        uint8_t RESERVED0 : 1;         /*!< [7]  */
    } B;
} hw_mcg_lftrim_t;

/*!
 * @name Constants and macros for entire MCG_LFTRIM register
 */
/*@{*/
#define HW_MCG_LFTRIM_ADDR(x)    ((x) + 0x1AU)

#define HW_MCG_LFTRIM(x)         (*(__I hw_mcg_lftrim_t *) HW_MCG_LFTRIM_ADDR(x))
#define HW_MCG_LFTRIM_RD(x)      (HW_MCG_LFTRIM(x).U)
/*@}*/

/*
 * Constants & macros for individual MCG_LFTRIM bitfields
 */

/*!
 * @name Register MCG_LFTRIM, field LIRC_FTRIM[6:0] (RO)
 *
 * Loads from a factory programmed location when out of reset.
 */
/*@{*/
#define BP_MCG_LFTRIM_LIRC_FTRIM (0U)      /*!< Bit position for MCG_LFTRIM_LIRC_FTRIM. */
#define BM_MCG_LFTRIM_LIRC_FTRIM (0x7FU)   /*!< Bit mask for MCG_LFTRIM_LIRC_FTRIM. */
#define BS_MCG_LFTRIM_LIRC_FTRIM (7U)      /*!< Bit field size in bits for MCG_LFTRIM_LIRC_FTRIM. */

/*! @brief Read current value of the MCG_LFTRIM_LIRC_FTRIM field. */
#define BR_MCG_LFTRIM_LIRC_FTRIM(x) (BME_UBFX8(HW_MCG_LFTRIM_ADDR(x), BP_MCG_LFTRIM_LIRC_FTRIM, BS_MCG_LFTRIM_LIRC_FTRIM))
/*@}*/

/*******************************************************************************
 * HW_MCG_LSTRIM - MCG Low-frequency IRC2M Trim Register
 ******************************************************************************/

/*!
 * @brief HW_MCG_LSTRIM - MCG Low-frequency IRC2M Trim Register (RO)
 *
 * Reset value: 0x00U
 */
typedef union _hw_mcg_lstrim
{
    uint8_t U;
    struct _hw_mcg_lstrim_bitfields
    {
        uint8_t LIRC_STRIM : 7;        /*!< [6:0] LIRC2M TRIM */
        uint8_t RESERVED0 : 1;         /*!< [7]  */
    } B;
} hw_mcg_lstrim_t;

/*!
 * @name Constants and macros for entire MCG_LSTRIM register
 */
/*@{*/
#define HW_MCG_LSTRIM_ADDR(x)    ((x) + 0x1BU)

#define HW_MCG_LSTRIM(x)         (*(__I hw_mcg_lstrim_t *) HW_MCG_LSTRIM_ADDR(x))
#define HW_MCG_LSTRIM_RD(x)      (HW_MCG_LSTRIM(x).U)
/*@}*/

/*
 * Constants & macros for individual MCG_LSTRIM bitfields
 */

/*!
 * @name Register MCG_LSTRIM, field LIRC_STRIM[6:0] (RO)
 *
 * Loads from a factory programmed location when out of reset.
 */
/*@{*/
#define BP_MCG_LSTRIM_LIRC_STRIM (0U)      /*!< Bit position for MCG_LSTRIM_LIRC_STRIM. */
#define BM_MCG_LSTRIM_LIRC_STRIM (0x7FU)   /*!< Bit mask for MCG_LSTRIM_LIRC_STRIM. */
#define BS_MCG_LSTRIM_LIRC_STRIM (7U)      /*!< Bit field size in bits for MCG_LSTRIM_LIRC_STRIM. */

/*! @brief Read current value of the MCG_LSTRIM_LIRC_STRIM field. */
#define BR_MCG_LSTRIM_LIRC_STRIM(x) (BME_UBFX8(HW_MCG_LSTRIM_ADDR(x), BP_MCG_LSTRIM_LIRC_STRIM, BS_MCG_LSTRIM_LIRC_STRIM))
/*@}*/

/*******************************************************************************
 * hw_mcg_t - module struct
 ******************************************************************************/
/*!
 * @brief All MCG module registers.
 */
#pragma pack(1)
typedef struct _hw_mcg
{
    __IO hw_mcg_c1_t C1;                   /*!< [0x0] MCG Control Register 1 */
    __IO hw_mcg_c2_t C2;                   /*!< [0x1] MCG Control Register 2 */
    uint8_t _reserved0[4];
    __I hw_mcg_s_t S;                      /*!< [0x6] MCG Status Register */
    uint8_t _reserved1[1];
    __IO hw_mcg_sc_t SC;                   /*!< [0x8] MCG Status and Control Register */
    uint8_t _reserved2[11];
    __I hw_mcg_hctrim_t HCTRIM;            /*!< [0x14] MCG High-frequency IRC Coarse Trim Register */
    __I hw_mcg_httrim_t HTTRIM;            /*!< [0x15] MCG High-frequency IRC Tempco (Temperature Coefficient) Trim Register */
    __I hw_mcg_hftrim_t HFTRIM;            /*!< [0x16] MCG High-frequency IRC Fine Trim Register */
    uint8_t _reserved3[1];
    __IO hw_mcg_mc_t MC;                   /*!< [0x18] MCG Miscellaneous Control Register */
    __I hw_mcg_ltrimrng_t LTRIMRNG;        /*!< [0x19] MCG Low-frequency IRC Trim Range Register */
    __I hw_mcg_lftrim_t LFTRIM;            /*!< [0x1A] MCG Low-frequency IRC8M Trim Register */
    __I hw_mcg_lstrim_t LSTRIM;            /*!< [0x1B] MCG Low-frequency IRC2M Trim Register */
} hw_mcg_t;
#pragma pack()

/*! @brief Macro to access all MCG registers. */
/*! @param x MCG module instance base address. */
/*! @return Reference (not a pointer) to the registers struct. To get a pointer to the struct,
 *     use the '&' operator, like <code>&HW_MCG(MCG_BASE)</code>. */
#define HW_MCG(x)      (*(hw_mcg_t *)(x))

#endif /* __HW_MCG_REGISTERS_H__ */
/* EOF */
