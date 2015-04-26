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
#ifndef __HW_SMC_REGISTERS_H__
#define __HW_SMC_REGISTERS_H__

#include "MKL43Z4.h"
#include "fsl_bitaccess.h"

/*
 * MKL43Z4 SMC
 *
 * System Mode Controller
 *
 * Registers defined in this header file:
 * - HW_SMC_PMPROT - Power Mode Protection register
 * - HW_SMC_PMCTRL - Power Mode Control register
 * - HW_SMC_STOPCTRL - Stop Control Register
 * - HW_SMC_PMSTAT - Power Mode Status register
 *
 * - hw_smc_t - Struct containing all module registers.
 */

#define HW_SMC_INSTANCE_COUNT (1U) /*!< Number of instances of the SMC module. */

/*******************************************************************************
 * HW_SMC_PMPROT - Power Mode Protection register
 ******************************************************************************/

/*!
 * @brief HW_SMC_PMPROT - Power Mode Protection register (RW)
 *
 * Reset value: 0x00U
 *
 * This register provides protection for entry into any low-power run or stop
 * mode. The enabling of the low-power run or stop mode occurs by configuring the
 * Power Mode Control register (PMCTRL). The PMPROT register can be written only
 * once after any system reset. If the MCU is configured for a disallowed or
 * reserved power mode, the MCU remains in its current power mode. For example, if the
 * MCU is in normal RUN mode and AVLP is 0, an attempt to enter VLPR mode using
 * PMCTRL[RUNM] is blocked and PMCTRL[RUNM] remains 00b, indicating the MCU is
 * still in Normal Run mode. This register is reset on Chip Reset not VLLS and by
 * reset types that trigger Chip Reset not VLLS. It is unaffected by reset types
 * that do not trigger Chip Reset not VLLS. See the Reset section details for more
 * information.
 */
typedef union _hw_smc_pmprot
{
    uint8_t U;
    struct _hw_smc_pmprot_bitfields
    {
        uint8_t RESERVED0 : 1;         /*!< [0]  */
        uint8_t AVLLS : 1;             /*!< [1] Allow Very-Low-Leakage Stop Mode */
        uint8_t RESERVED1 : 1;         /*!< [2]  */
        uint8_t ALLS : 1;              /*!< [3] Allow Low-Leakage Stop Mode */
        uint8_t RESERVED2 : 1;         /*!< [4]  */
        uint8_t AVLP : 1;              /*!< [5] Allow Very-Low-Power Modes */
        uint8_t RESERVED3 : 2;         /*!< [7:6]  */
    } B;
} hw_smc_pmprot_t;

/*!
 * @name Constants and macros for entire SMC_PMPROT register
 */
/*@{*/
#define HW_SMC_PMPROT_ADDR(x)    ((x) + 0x0U)

#define HW_SMC_PMPROT(x)         (*(__IO hw_smc_pmprot_t *) HW_SMC_PMPROT_ADDR(x))
#define HW_SMC_PMPROT_RD(x)      (HW_SMC_PMPROT(x).U)
#define HW_SMC_PMPROT_WR(x, v)   (HW_SMC_PMPROT(x).U = (v))
#define HW_SMC_PMPROT_SET(x, v)  (BME_OR8(HW_SMC_PMPROT_ADDR(x), (uint8_t)(v)))
#define HW_SMC_PMPROT_CLR(x, v)  (BME_AND8(HW_SMC_PMPROT_ADDR(x), (uint8_t)(~(v))))
#define HW_SMC_PMPROT_TOG(x, v)  (BME_XOR8(HW_SMC_PMPROT_ADDR(x), (uint8_t)(v)))
/*@}*/

/*
 * Constants & macros for individual SMC_PMPROT bitfields
 */

/*!
 * @name Register SMC_PMPROT, field AVLLS[1] (RW)
 *
 * Provided the appropriate control bits are set up in PMCTRL, this write once
 * bit allows the MCU to enter any very-low-leakage stop mode (VLLSx).
 *
 * Values:
 * - 0 - Any VLLSx mode is not allowed
 * - 1 - Any VLLSx mode is allowed
 */
/*@{*/
#define BP_SMC_PMPROT_AVLLS  (1U)          /*!< Bit position for SMC_PMPROT_AVLLS. */
#define BM_SMC_PMPROT_AVLLS  (0x02U)       /*!< Bit mask for SMC_PMPROT_AVLLS. */
#define BS_SMC_PMPROT_AVLLS  (1U)          /*!< Bit field size in bits for SMC_PMPROT_AVLLS. */

/*! @brief Read current value of the SMC_PMPROT_AVLLS field. */
#define BR_SMC_PMPROT_AVLLS(x) (BME_UBFX8(HW_SMC_PMPROT_ADDR(x), BP_SMC_PMPROT_AVLLS, BS_SMC_PMPROT_AVLLS))

/*! @brief Format value for bitfield SMC_PMPROT_AVLLS. */
#define BF_SMC_PMPROT_AVLLS(v) ((uint8_t)((uint8_t)(v) << BP_SMC_PMPROT_AVLLS) & BM_SMC_PMPROT_AVLLS)

/*! @brief Set the AVLLS field to a new value. */
#define BW_SMC_PMPROT_AVLLS(x, v) (BME_BFI8(HW_SMC_PMPROT_ADDR(x), ((uint8_t)(v) << BP_SMC_PMPROT_AVLLS), BP_SMC_PMPROT_AVLLS, 1))
/*@}*/

/*!
 * @name Register SMC_PMPROT, field ALLS[3] (RW)
 *
 * Provided the appropriate control bits are set up in PMCTRL, this write-once
 * field allows the MCU to enter any low-leakage stop mode (LLS).
 *
 * Values:
 * - 0 - LLS is not allowed
 * - 1 - LLS is allowed
 */
/*@{*/
#define BP_SMC_PMPROT_ALLS   (3U)          /*!< Bit position for SMC_PMPROT_ALLS. */
#define BM_SMC_PMPROT_ALLS   (0x08U)       /*!< Bit mask for SMC_PMPROT_ALLS. */
#define BS_SMC_PMPROT_ALLS   (1U)          /*!< Bit field size in bits for SMC_PMPROT_ALLS. */

/*! @brief Read current value of the SMC_PMPROT_ALLS field. */
#define BR_SMC_PMPROT_ALLS(x) (BME_UBFX8(HW_SMC_PMPROT_ADDR(x), BP_SMC_PMPROT_ALLS, BS_SMC_PMPROT_ALLS))

/*! @brief Format value for bitfield SMC_PMPROT_ALLS. */
#define BF_SMC_PMPROT_ALLS(v) ((uint8_t)((uint8_t)(v) << BP_SMC_PMPROT_ALLS) & BM_SMC_PMPROT_ALLS)

/*! @brief Set the ALLS field to a new value. */
#define BW_SMC_PMPROT_ALLS(x, v) (BME_BFI8(HW_SMC_PMPROT_ADDR(x), ((uint8_t)(v) << BP_SMC_PMPROT_ALLS), BP_SMC_PMPROT_ALLS, 1))
/*@}*/

/*!
 * @name Register SMC_PMPROT, field AVLP[5] (RW)
 *
 * Provided the appropriate control bits are set up in PMCTRL, this write-once
 * field allows the MCU to enter any very-low-power mode (VLPR, VLPW, and VLPS).
 *
 * Values:
 * - 0 - VLPR, VLPW, and VLPS are not allowed.
 * - 1 - VLPR, VLPW, and VLPS are allowed.
 */
/*@{*/
#define BP_SMC_PMPROT_AVLP   (5U)          /*!< Bit position for SMC_PMPROT_AVLP. */
#define BM_SMC_PMPROT_AVLP   (0x20U)       /*!< Bit mask for SMC_PMPROT_AVLP. */
#define BS_SMC_PMPROT_AVLP   (1U)          /*!< Bit field size in bits for SMC_PMPROT_AVLP. */

/*! @brief Read current value of the SMC_PMPROT_AVLP field. */
#define BR_SMC_PMPROT_AVLP(x) (BME_UBFX8(HW_SMC_PMPROT_ADDR(x), BP_SMC_PMPROT_AVLP, BS_SMC_PMPROT_AVLP))

/*! @brief Format value for bitfield SMC_PMPROT_AVLP. */
#define BF_SMC_PMPROT_AVLP(v) ((uint8_t)((uint8_t)(v) << BP_SMC_PMPROT_AVLP) & BM_SMC_PMPROT_AVLP)

/*! @brief Set the AVLP field to a new value. */
#define BW_SMC_PMPROT_AVLP(x, v) (BME_BFI8(HW_SMC_PMPROT_ADDR(x), ((uint8_t)(v) << BP_SMC_PMPROT_AVLP), BP_SMC_PMPROT_AVLP, 1))
/*@}*/

/*******************************************************************************
 * HW_SMC_PMCTRL - Power Mode Control register
 ******************************************************************************/

/*!
 * @brief HW_SMC_PMCTRL - Power Mode Control register (RW)
 *
 * Reset value: 0x00U
 *
 * The PMCTRL register controls entry into low-power Run and Stop modes,
 * provided that the selected power mode is allowed via an appropriate setting of the
 * protection (PMPROT) register. This register is reset on Chip POR not VLLS and by
 * reset types that trigger Chip POR not VLLS. It is unaffected by reset types
 * that do not trigger Chip POR not VLLS. See the Reset section details for more
 * information.
 */
typedef union _hw_smc_pmctrl
{
    uint8_t U;
    struct _hw_smc_pmctrl_bitfields
    {
        uint8_t STOPM : 3;             /*!< [2:0] Stop Mode Control */
        uint8_t STOPA : 1;             /*!< [3] Stop Aborted */
        uint8_t RESERVED0 : 1;         /*!< [4]  */
        uint8_t RUNM : 2;              /*!< [6:5] Run Mode Control */
        uint8_t RESERVED1 : 1;         /*!< [7]  */
    } B;
} hw_smc_pmctrl_t;

/*!
 * @name Constants and macros for entire SMC_PMCTRL register
 */
/*@{*/
#define HW_SMC_PMCTRL_ADDR(x)    ((x) + 0x1U)

#define HW_SMC_PMCTRL(x)         (*(__IO hw_smc_pmctrl_t *) HW_SMC_PMCTRL_ADDR(x))
#define HW_SMC_PMCTRL_RD(x)      (HW_SMC_PMCTRL(x).U)
#define HW_SMC_PMCTRL_WR(x, v)   (HW_SMC_PMCTRL(x).U = (v))
#define HW_SMC_PMCTRL_SET(x, v)  (BME_OR8(HW_SMC_PMCTRL_ADDR(x), (uint8_t)(v)))
#define HW_SMC_PMCTRL_CLR(x, v)  (BME_AND8(HW_SMC_PMCTRL_ADDR(x), (uint8_t)(~(v))))
#define HW_SMC_PMCTRL_TOG(x, v)  (BME_XOR8(HW_SMC_PMCTRL_ADDR(x), (uint8_t)(v)))
/*@}*/

/*
 * Constants & macros for individual SMC_PMCTRL bitfields
 */

/*!
 * @name Register SMC_PMCTRL, field STOPM[2:0] (RW)
 *
 * When written, controls entry into the selected stop mode when Sleep-Now or
 * Sleep-On-Exit mode is entered with SLEEPDEEP=1 . Writes to this field are
 * blocked if the protection level has not been enabled using the PMPROT register.
 * After any system reset, this field is cleared by hardware on any successful write
 * to the PMPROT register. When set to VLLSx, the VLLSM field in the STOPCTRL
 * register is used to further select the particular VLLS submode which will be
 * entered. When set to STOP, the PSTOPO bits in the STOPCTRL register can be used to
 * select a Partial Stop mode if desired.
 *
 * Values:
 * - 000 - Normal Stop (STOP)
 * - 001 - Reserved
 * - 010 - Very-Low-Power Stop (VLPS)
 * - 011 - Low-Leakage Stop (LLS)
 * - 100 - Very-Low-Leakage Stop (VLLSx)
 * - 101 - Reserved
 * - 110 - Reseved
 * - 111 - Reserved
 */
/*@{*/
#define BP_SMC_PMCTRL_STOPM  (0U)          /*!< Bit position for SMC_PMCTRL_STOPM. */
#define BM_SMC_PMCTRL_STOPM  (0x07U)       /*!< Bit mask for SMC_PMCTRL_STOPM. */
#define BS_SMC_PMCTRL_STOPM  (3U)          /*!< Bit field size in bits for SMC_PMCTRL_STOPM. */

/*! @brief Read current value of the SMC_PMCTRL_STOPM field. */
#define BR_SMC_PMCTRL_STOPM(x) (BME_UBFX8(HW_SMC_PMCTRL_ADDR(x), BP_SMC_PMCTRL_STOPM, BS_SMC_PMCTRL_STOPM))

/*! @brief Format value for bitfield SMC_PMCTRL_STOPM. */
#define BF_SMC_PMCTRL_STOPM(v) ((uint8_t)((uint8_t)(v) << BP_SMC_PMCTRL_STOPM) & BM_SMC_PMCTRL_STOPM)

/*! @brief Set the STOPM field to a new value. */
#define BW_SMC_PMCTRL_STOPM(x, v) (BME_BFI8(HW_SMC_PMCTRL_ADDR(x), ((uint8_t)(v) << BP_SMC_PMCTRL_STOPM), BP_SMC_PMCTRL_STOPM, 3))
/*@}*/

/*!
 * @name Register SMC_PMCTRL, field STOPA[3] (RO)
 *
 * When set, this read-only status bit indicates an interrupt occured during the
 * previous stop mode entry sequence, preventing the system from entering that
 * mode. This field is cleared by reset or by hardware at the beginning of any
 * stop mode entry sequence and is set if the sequence was aborted.
 *
 * Values:
 * - 0 - The previous stop mode entry was successsful.
 * - 1 - The previous stop mode entry was aborted.
 */
/*@{*/
#define BP_SMC_PMCTRL_STOPA  (3U)          /*!< Bit position for SMC_PMCTRL_STOPA. */
#define BM_SMC_PMCTRL_STOPA  (0x08U)       /*!< Bit mask for SMC_PMCTRL_STOPA. */
#define BS_SMC_PMCTRL_STOPA  (1U)          /*!< Bit field size in bits for SMC_PMCTRL_STOPA. */

/*! @brief Read current value of the SMC_PMCTRL_STOPA field. */
#define BR_SMC_PMCTRL_STOPA(x) (BME_UBFX8(HW_SMC_PMCTRL_ADDR(x), BP_SMC_PMCTRL_STOPA, BS_SMC_PMCTRL_STOPA))
/*@}*/

/*!
 * @name Register SMC_PMCTRL, field RUNM[6:5] (RW)
 *
 * When written, causes entry into the selected run mode. Writes to this field
 * are blocked if the protection level has not been enabled using the PMPROT
 * register. RUNM may be set to VLPR only when PMSTAT=RUN. After being written to
 * VLPR, RUNM should not be written back to RUN until PMSTAT=VLPR.
 *
 * Values:
 * - 00 - Normal Run mode (RUN)
 * - 01 - Reserved
 * - 10 - Very-Low-Power Run mode (VLPR)
 * - 11 - Reserved
 */
/*@{*/
#define BP_SMC_PMCTRL_RUNM   (5U)          /*!< Bit position for SMC_PMCTRL_RUNM. */
#define BM_SMC_PMCTRL_RUNM   (0x60U)       /*!< Bit mask for SMC_PMCTRL_RUNM. */
#define BS_SMC_PMCTRL_RUNM   (2U)          /*!< Bit field size in bits for SMC_PMCTRL_RUNM. */

/*! @brief Read current value of the SMC_PMCTRL_RUNM field. */
#define BR_SMC_PMCTRL_RUNM(x) (BME_UBFX8(HW_SMC_PMCTRL_ADDR(x), BP_SMC_PMCTRL_RUNM, BS_SMC_PMCTRL_RUNM))

/*! @brief Format value for bitfield SMC_PMCTRL_RUNM. */
#define BF_SMC_PMCTRL_RUNM(v) ((uint8_t)((uint8_t)(v) << BP_SMC_PMCTRL_RUNM) & BM_SMC_PMCTRL_RUNM)

/*! @brief Set the RUNM field to a new value. */
#define BW_SMC_PMCTRL_RUNM(x, v) (BME_BFI8(HW_SMC_PMCTRL_ADDR(x), ((uint8_t)(v) << BP_SMC_PMCTRL_RUNM), BP_SMC_PMCTRL_RUNM, 2))
/*@}*/

/*******************************************************************************
 * HW_SMC_STOPCTRL - Stop Control Register
 ******************************************************************************/

/*!
 * @brief HW_SMC_STOPCTRL - Stop Control Register (RW)
 *
 * Reset value: 0x03U
 *
 * The STOPCTRL register provides various control bits allowing the user to fine
 * tune power consumption during the stop mode selected by the STOPM field. This
 * register is reset on Chip POR not VLLS and by reset types that trigger Chip
 * POR not VLLS. It is unaffected by reset types that do not trigger Chip POR not
 * VLLS. See the Reset section details for more information.
 */
typedef union _hw_smc_stopctrl
{
    uint8_t U;
    struct _hw_smc_stopctrl_bitfields
    {
        uint8_t VLLSM : 3;             /*!< [2:0] VLLS Mode Control */
        uint8_t RESERVED0 : 2;         /*!< [4:3]  */
        uint8_t PORPO : 1;             /*!< [5] POR Power Option */
        uint8_t PSTOPO : 2;            /*!< [7:6] Partial Stop Option */
    } B;
} hw_smc_stopctrl_t;

/*!
 * @name Constants and macros for entire SMC_STOPCTRL register
 */
/*@{*/
#define HW_SMC_STOPCTRL_ADDR(x)  ((x) + 0x2U)

#define HW_SMC_STOPCTRL(x)       (*(__IO hw_smc_stopctrl_t *) HW_SMC_STOPCTRL_ADDR(x))
#define HW_SMC_STOPCTRL_RD(x)    (HW_SMC_STOPCTRL(x).U)
#define HW_SMC_STOPCTRL_WR(x, v) (HW_SMC_STOPCTRL(x).U = (v))
#define HW_SMC_STOPCTRL_SET(x, v) (BME_OR8(HW_SMC_STOPCTRL_ADDR(x), (uint8_t)(v)))
#define HW_SMC_STOPCTRL_CLR(x, v) (BME_AND8(HW_SMC_STOPCTRL_ADDR(x), (uint8_t)(~(v))))
#define HW_SMC_STOPCTRL_TOG(x, v) (BME_XOR8(HW_SMC_STOPCTRL_ADDR(x), (uint8_t)(v)))
/*@}*/

/*
 * Constants & macros for individual SMC_STOPCTRL bitfields
 */

/*!
 * @name Register SMC_STOPCTRL, field VLLSM[2:0] (RW)
 *
 * This field controls which VLLS sub-mode to enter if STOPM = VLLSx.
 *
 * Values:
 * - 000 - VLLS0
 * - 001 - VLLS1
 * - 010 - Reserved
 * - 011 - VLLS3
 * - 100 - Reserved
 * - 101 - Reserved
 * - 110 - Reserved
 * - 111 - Reserved
 */
/*@{*/
#define BP_SMC_STOPCTRL_VLLSM (0U)         /*!< Bit position for SMC_STOPCTRL_VLLSM. */
#define BM_SMC_STOPCTRL_VLLSM (0x07U)      /*!< Bit mask for SMC_STOPCTRL_VLLSM. */
#define BS_SMC_STOPCTRL_VLLSM (3U)         /*!< Bit field size in bits for SMC_STOPCTRL_VLLSM. */

/*! @brief Read current value of the SMC_STOPCTRL_VLLSM field. */
#define BR_SMC_STOPCTRL_VLLSM(x) (BME_UBFX8(HW_SMC_STOPCTRL_ADDR(x), BP_SMC_STOPCTRL_VLLSM, BS_SMC_STOPCTRL_VLLSM))

/*! @brief Format value for bitfield SMC_STOPCTRL_VLLSM. */
#define BF_SMC_STOPCTRL_VLLSM(v) ((uint8_t)((uint8_t)(v) << BP_SMC_STOPCTRL_VLLSM) & BM_SMC_STOPCTRL_VLLSM)

/*! @brief Set the VLLSM field to a new value. */
#define BW_SMC_STOPCTRL_VLLSM(x, v) (BME_BFI8(HW_SMC_STOPCTRL_ADDR(x), ((uint8_t)(v) << BP_SMC_STOPCTRL_VLLSM), BP_SMC_STOPCTRL_VLLSM, 3))
/*@}*/

/*!
 * @name Register SMC_STOPCTRL, field PORPO[5] (RW)
 *
 * This bit controls whether the POR detect circuit is enabled in VLLS0 mode.
 *
 * Values:
 * - 0 - POR detect circuit is enabled in VLLS0
 * - 1 - POR detect circuit is disabled in VLLS0
 */
/*@{*/
#define BP_SMC_STOPCTRL_PORPO (5U)         /*!< Bit position for SMC_STOPCTRL_PORPO. */
#define BM_SMC_STOPCTRL_PORPO (0x20U)      /*!< Bit mask for SMC_STOPCTRL_PORPO. */
#define BS_SMC_STOPCTRL_PORPO (1U)         /*!< Bit field size in bits for SMC_STOPCTRL_PORPO. */

/*! @brief Read current value of the SMC_STOPCTRL_PORPO field. */
#define BR_SMC_STOPCTRL_PORPO(x) (BME_UBFX8(HW_SMC_STOPCTRL_ADDR(x), BP_SMC_STOPCTRL_PORPO, BS_SMC_STOPCTRL_PORPO))

/*! @brief Format value for bitfield SMC_STOPCTRL_PORPO. */
#define BF_SMC_STOPCTRL_PORPO(v) ((uint8_t)((uint8_t)(v) << BP_SMC_STOPCTRL_PORPO) & BM_SMC_STOPCTRL_PORPO)

/*! @brief Set the PORPO field to a new value. */
#define BW_SMC_STOPCTRL_PORPO(x, v) (BME_BFI8(HW_SMC_STOPCTRL_ADDR(x), ((uint8_t)(v) << BP_SMC_STOPCTRL_PORPO), BP_SMC_STOPCTRL_PORPO, 1))
/*@}*/

/*!
 * @name Register SMC_STOPCTRL, field PSTOPO[7:6] (RW)
 *
 * These bits control whether a Partial Stop mode is entered when STOPM=STOP.
 * When entering a Partial Stop mode from RUN mode, the PMC, MCG and flash remain
 * fully powered, allowing the device to wakeup almost instantaneously at the
 * expense of higher power consumption. In PSTOP2, only system clocks are gated
 * allowing peripherals running on bus clock to remain fully functional. In PSTOP1,
 * both system and bus clocks are gated.
 *
 * Values:
 * - 00 - STOP - Normal Stop mode
 * - 01 - PSTOP1 - Partial Stop with both system and bus clocks disabled
 * - 10 - PSTOP2 - Partial Stop with system clock disabled and bus clock enabled
 * - 11 - Reserved
 */
/*@{*/
#define BP_SMC_STOPCTRL_PSTOPO (6U)        /*!< Bit position for SMC_STOPCTRL_PSTOPO. */
#define BM_SMC_STOPCTRL_PSTOPO (0xC0U)     /*!< Bit mask for SMC_STOPCTRL_PSTOPO. */
#define BS_SMC_STOPCTRL_PSTOPO (2U)        /*!< Bit field size in bits for SMC_STOPCTRL_PSTOPO. */

/*! @brief Read current value of the SMC_STOPCTRL_PSTOPO field. */
#define BR_SMC_STOPCTRL_PSTOPO(x) (BME_UBFX8(HW_SMC_STOPCTRL_ADDR(x), BP_SMC_STOPCTRL_PSTOPO, BS_SMC_STOPCTRL_PSTOPO))

/*! @brief Format value for bitfield SMC_STOPCTRL_PSTOPO. */
#define BF_SMC_STOPCTRL_PSTOPO(v) ((uint8_t)((uint8_t)(v) << BP_SMC_STOPCTRL_PSTOPO) & BM_SMC_STOPCTRL_PSTOPO)

/*! @brief Set the PSTOPO field to a new value. */
#define BW_SMC_STOPCTRL_PSTOPO(x, v) (BME_BFI8(HW_SMC_STOPCTRL_ADDR(x), ((uint8_t)(v) << BP_SMC_STOPCTRL_PSTOPO), BP_SMC_STOPCTRL_PSTOPO, 2))
/*@}*/

/*******************************************************************************
 * HW_SMC_PMSTAT - Power Mode Status register
 ******************************************************************************/

/*!
 * @brief HW_SMC_PMSTAT - Power Mode Status register (RO)
 *
 * Reset value: 0x01U
 *
 * PMSTAT is a read-only, one-hot register which indicates the current power
 * mode of the system. This register is reset on Chip POR not VLLS and by reset
 * types that trigger Chip POR not VLLS. It is unaffected by reset types that do not
 * trigger Chip POR not VLLS. See the Reset section details for more information.
 */
typedef union _hw_smc_pmstat
{
    uint8_t U;
    struct _hw_smc_pmstat_bitfields
    {
        uint8_t PMSTAT : 8;            /*!< [7:0] Power Mode Status */
    } B;
} hw_smc_pmstat_t;

/*!
 * @name Constants and macros for entire SMC_PMSTAT register
 */
/*@{*/
#define HW_SMC_PMSTAT_ADDR(x)    ((x) + 0x3U)

#define HW_SMC_PMSTAT(x)         (*(__I hw_smc_pmstat_t *) HW_SMC_PMSTAT_ADDR(x))
#define HW_SMC_PMSTAT_RD(x)      (HW_SMC_PMSTAT(x).U)
/*@}*/

/*
 * Constants & macros for individual SMC_PMSTAT bitfields
 */

/*!
 * @name Register SMC_PMSTAT, field PMSTAT[7:0] (RO)
 *
 * When debug is enabled, the PMSTAT will not update to STOP or VLPS When a
 * PSTOP mode is enabled, the PMSTAT will not update to STOP or VLPS
 */
/*@{*/
#define BP_SMC_PMSTAT_PMSTAT (0U)          /*!< Bit position for SMC_PMSTAT_PMSTAT. */
#define BM_SMC_PMSTAT_PMSTAT (0xFFU)       /*!< Bit mask for SMC_PMSTAT_PMSTAT. */
#define BS_SMC_PMSTAT_PMSTAT (8U)          /*!< Bit field size in bits for SMC_PMSTAT_PMSTAT. */

/*! @brief Read current value of the SMC_PMSTAT_PMSTAT field. */
#define BR_SMC_PMSTAT_PMSTAT(x) (HW_SMC_PMSTAT(x).U)
/*@}*/

/*******************************************************************************
 * hw_smc_t - module struct
 ******************************************************************************/
/*!
 * @brief All SMC module registers.
 */
#pragma pack(1)
typedef struct _hw_smc
{
    __IO hw_smc_pmprot_t PMPROT;           /*!< [0x0] Power Mode Protection register */
    __IO hw_smc_pmctrl_t PMCTRL;           /*!< [0x1] Power Mode Control register */
    __IO hw_smc_stopctrl_t STOPCTRL;       /*!< [0x2] Stop Control Register */
    __I hw_smc_pmstat_t PMSTAT;            /*!< [0x3] Power Mode Status register */
} hw_smc_t;
#pragma pack()

/*! @brief Macro to access all SMC registers. */
/*! @param x SMC module instance base address. */
/*! @return Reference (not a pointer) to the registers struct. To get a pointer to the struct,
 *     use the '&' operator, like <code>&HW_SMC(SMC_BASE)</code>. */
#define HW_SMC(x)      (*(hw_smc_t *)(x))

#endif /* __HW_SMC_REGISTERS_H__ */
/* EOF */
