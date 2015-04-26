/****************************************************************************
 (c) Copyright 2013-2014 Freescale Semiconductor, Inc.
 ALL RIGHTS RESERVED.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY DIRECT, 
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  FTFx_KX_(512_256_128)K_0K_0K_2K_0K.h                 *
* DATE          :  April 08, 2014                                       *
*                                                                       *
* AUTHOR        :  FPT Team                                             *
* E-mail        :  r56611@freescale.com                                 *
*                                                                       *
*************************************************************************/

/************************** CHANGES *************************************
1.0.0       11.25.2013      FPT Team      Initial version for Senna
                                          based on FTFx_KX_(512_256)K_0K_0K_1K_0K.h
1.0.1       01.16.2014      FPT Team      Change prototype of PFlashSwapCtl function
1.0.2       01.16.2014      FPT Team      Change prototype of PFlashSwapCtl function
1.0.2       08.04.2014      FPT Team      Update to follow SDK convention(MISRA-C)
                                          Delete function prototypes for Flash SSD
                                          Change define ARM_CM4 to ARM_CORTEX_M
*************************************************************************/

#ifndef _FTFx_KX_512_256_128K_0K_0K_2K_0K_H_
#define _FTFx_KX_512_256_128K_0K_0K_2K_0K_H_

#include "SSD_FTFx_Common.h"

#define FTFA_M 
#define BLOCK_COMMANDS /* this is FTFA module, so some commands are not available */

#define BYTE2WORD(x) (x)
#define WORD2BYTE(x) (x)

/* Endianness */
#define ENDIANNESS              LITTLE_ENDIAN

/* cpu core */
#define CPU_CORE                ARM_CORTEX_M

/* PFlash sector size */ 
#define FTFx_PSECTOR_SIZE       0x00000800U      /* 2 KB size */
#define DEBLOCK_SIZE            0x00000000U      /* 0 KB size */

/* Address offset and size of PFlash IFR and DFlash IFR */
#define PFLASH_IFR_OFFSET       0x00000000U
#define PFLASH_IFR_SIZE         0x00000100U   

/* Size for checking alignment of a section */
#define PERSBLK_ALIGN_SIZE      FTFx_LONGWORD_SIZE      /* check align of erase block function */

#define PERSSEC_ALIGN_SIZE      FTFx_PHRASE_SIZE        /* check align of erase sector on PFlash function */

#define PRD1BLK_ALIGN_SIZE      FTFx_LONGWORD_SIZE      /* check align of verify block function */

#define PRD1SEC_ALIGN_SIZE      FTFx_PHRASE_SIZE        /* check align of verify section on PFlash function */

#define PGMCHK_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of program check function */
#define RDRSRC_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of read resource function */

#define PGM_SIZE_BYTE           FTFx_LONGWORD_SIZE

#define RESUME_WAIT_CNT         0x20U

#endif  /* _FTFx_KX_512_256_128K_0K_0K_2K_0K_H_ */
