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
* FILE NAME     :  FTFx_KX_512K_128K_4K_4K_4K.h                         *
* DATE          :  April 08, 2014                                       *
*                                                                       *
* AUTHOR        :  FPT Team                                             *
* E-mail        :  r56611@freescale.com                                 *
*                                                                       *
*************************************************************************/

/************************** CHANGES *************************************
0.1.0       03.16.2013      FPT Team      Initial Version
0.1.1       06.05.2013      FPT Team      Add define FTFE
                                          Correct RDRSRC_SIZE = 8 (instead of 4).
0.1.2       06.20.2013      FPT Team      Remove declaration for swap functions since this 
                                          derivative has no swap.    
                                          Update function prototype of 
                                          FlashProgramCheck by removing pFailData        
1.0.0       12.25.2013      FPT Team      Modify define macros
1.0.2       08.04.2014      FPT Team      Update to follow SDK convention(MISRA-C)
                                          Delete function prototypeS for Flash SSD
                                          Change define ARM_CM4 to ARM_CORTEX_M
*************************************************************************/

#ifndef _FTFx_KX_512K_128K_4K_4K_4K_H_
#define _FTFx_KX_512K_128K_4K_4K_4K_H_

#include "SSD_FTFx_Common.h"

#define FTFE_M /* this is FTFE module */

#define BYTE2WORD(x) (x)
#define WORD2BYTE(x) (x)

/* Endianness */
#define ENDIANNESS              LITTLE_ENDIAN

/* cpu core */
#define CPU_CORE                ARM_CORTEX_M            

/* PFlash sector size */ 
#define FTFx_PSECTOR_SIZE       0x00001000U      /* 4 KB size */
/* DFlash sector size */ 
#define FTFx_DSECTOR_SIZE       0x00001000U      /* 4 KB size */
#define DEBLOCK_SIZE            0x00020000U      /* 128 KB size */

/* EEE Data Set Size Field Description */
#define EEESIZE_0000            0xFFFFFFFFU      /* Reserved */
#define EEESIZE_0001            0xFFFFFFFFU      /* Reserved */
#define EEESIZE_0010            0x00001000U      /* 4096 bytes */
#define EEESIZE_0011            0x00000800U      /* 2048 bytes */
#define EEESIZE_0100            0x00000400U      /* 1024 bytes */
#define EEESIZE_0101            0x00000200U      /* 512 bytes */
#define EEESIZE_0110            0x00000100U      /* 256 bytes */
#define EEESIZE_0111            0x00000080U      /* 128 bytes */
#define EEESIZE_1000            0x00000040U      /* 64 bytes */
#define EEESIZE_1001            0x00000020U      /* 32 bytes */
#define EEESIZE_1010            0xFFFFFFFFU      /* Reserved */
#define EEESIZE_1011            0xFFFFFFFFU      /* Reserved */
#define EEESIZE_1100            0xFFFFFFFFU      /* Reserved */
#define EEESIZE_1101            0xFFFFFFFFU      /* Reserved */
#define EEESIZE_1110            0xFFFFFFFFU      /* Reserved */
#define EEESIZE_1111            0x00000000U      /* 0 byte */

/* D/E-Flash Partition Code Field Description */
#define DEPART_0000             0x00020000U      /* 128 KB */
#define DEPART_0001             0xFFFFFFFFU      /* Reserved */
#define DEPART_0010             0xFFFFFFFFU      /* Reserved */
#define DEPART_0011             0x00018000U      /* 96 KB */
#define DEPART_0100             0x00010000U      /* 64 KB */
#define DEPART_0101             0x00000000U      /* 0 KB */
#define DEPART_0110             0xFFFFFFFFU      /* Reserved */
#define DEPART_0111             0xFFFFFFFFU      /* Reserved */
#define DEPART_1000             0x00000000U      /* 0 KB */
#define DEPART_1001             0xFFFFFFFFU      /* Reserved */
#define DEPART_1010             0xFFFFFFFFU      /* Reserved */
#define DEPART_1011             0x00008000U      /* 32 KB */
#define DEPART_1100             0x00010000U      /* 64 KB */
#define DEPART_1101             0x00020000U      /* 128 KB */
#define DEPART_1110             0xFFFFFFFFU      /* Reserved */
#define DEPART_1111             0x00020000U      /* 128 KB */

/* Address offset (compare to start addr of P/D flash) and size of PFlash IFR and DFlash IFR */
#define PFLASH_IFR_OFFSET       0x00000000U
#define PFLASH_IFR_SIZE         0x00000400U
#define DFLASH_IFR_OFFSET       0x00000000U
#define DFLASH_IFR_SIZE         0x00000400U

/*destination to read Dflash IFR area*/
#define DFLASH_IFR_READRESOURCE_ADDRESS       0x8003F8U  

/* Size for checking alignment of a section */
#define PERSBLK_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of erase block on PFlash function */
#define DERSBLK_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of erase block on DFlash function */

#define PERSSEC_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of erase sector on PFlash function */
#define DERSSEC_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of erase sector on DFlash function */

#define PPGMSEC_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of program section on PFlash function */
#define DPGMSEC_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of program section on DFlash function */

#define PRD1BLK_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of verify block on PFlash function */
#define DRD1BLK_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of verify block on DFlash function */

#define PRD1SEC_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of verify section on PFlash function */
#define DRD1SEC_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of verify section on DFlash function */

#define PGMCHK_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of program check function */
#define RDRSRC_ALIGN_SIZE       FTFx_PHRASE_SIZE        /* check align of read resource function */
#define SWAP_ALIGN_SIZE         FTFx_DPHRASE_SIZE       /* check align of swap function*/

#define PGM_SIZE_BYTE           FTFx_PHRASE_SIZE
                     
#define RESUME_WAIT_CNT         0x20U

#endif  /* _FTFx_KX_512K_128K_4K_4K_4K_H_ */
