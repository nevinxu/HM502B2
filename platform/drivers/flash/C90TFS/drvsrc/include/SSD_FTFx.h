/****************************************************************************
 (c) Copyright 2010-2014 Freescale Semiconductor, Inc.
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

*****************************************************************************
*                                                                           *
*        Standard Software Flash Driver For FTFx                            *
*                                                                           *
* FILE NAME     :  SSD_FTFx.h                                               *
* DATE          :  April 08, 2014                                           *
*                                                                           *
* AUTHOR        :  FPT Team                                                 *
* E-mail        :  r56611@freescale.com                                     *
*                                                                           *
*****************************************************************************/

/************************** CHANGES *************************************
0.0.1       06.09.2010      FPT Team      Initial Version
0.1.0       06.11.2010      FPT Team      Finalize to 0.1.0 
0.1.1       08.16.2010      FPT Team      Finalize to 0.1.1
0.1.2       08.26.2010      FPT Team      Finalize to 0.1.2
0.1.3       09.16.2010      FPT Team      Updated to support little Indian
0.2.0       06.27.2010      FPT Team      Finalize to 0.2.0 
0.2.1       01.28.2011      FPT Team      Updated to support 
                                          FTFx_KX_512K_0K_0K,
                                          FTFx_JX_128K_32K_2K,
                                          and FTFx_FX_256K_32K_2K
                                          derivatives.
0.2.2       04.18.2011      FPT Team      Add swap control code definitions
                                          of FTFx_PFLASH_SWAP.
0.2.3       09.15.2011      FPT Team      Add command for program phrase
0.2.4       03.16.2013      FPT Team      Remove define Flash margin read settings
                                          Add GETINDEX macro.
1.0.0       12.25.2013      FPT Team      Swap content of SSD_FTFx_Internal.h and SSD_FTFx.h to optimize include structure in c source file
										  Update to simplify including driver 
                                          header files for source files
                                          Add definitions FTFx_SSD_FSTAT_ERROR_BITS
1.0.2       08.04.2014      FPT Team      Update to follow SDK convention(MISRA-C)
                                          Add function Prototypes for Flash SSD
*************************************************************************/
#ifndef _SSD_FTFx_INTERNAL_H_
#define _SSD_FTFx_INTERNAL_H_

#include "SSD_FTFx_Internal.h"

#define FTFx_SSD_FSTAT_CCIF                 0x80U
#define FTFx_SSD_FSTAT_RDCOLERR             0x40U
#define FTFx_SSD_FSTAT_ACCERR               0x20U
#define FTFx_SSD_FSTAT_FPVIOL               0x10U
#define FTFx_SSD_FSTAT_MGSTAT0              0x01U
#define FTFx_SSD_FSTAT_ERROR_BITS           (FTFx_SSD_FSTAT_ACCERR \
                                            |FTFx_SSD_FSTAT_FPVIOL \
                                            |FTFx_SSD_FSTAT_MGSTAT0)

#define FTFx_SSD_FCNFG_CCIE                 0x80U
#define FTFx_SSD_FCNFG_RDCOLLIE             0x40U
#define FTFx_SSD_FCNFG_ERSAREQ              0x20U
#define FTFx_SSD_FCNFG_ERSSUSP              0x10U
#define FTFx_SSD_FCNFG_RAMRDY               0x02U
#define FTFx_SSD_FCNFG_EEERDY               0x01U    
                                                
#define FTFx_SSD_FSEC_KEYEN                 0xC0U
#define FTFx_SSD_FSEC_FSLACC                0x0CU
#define FTFx_SSD_FSEC_SEC                   0x03U    
    
/*--------------- FTFx Flash Module Memory Offset Map -----------------*/
#if(BIG_ENDIAN == ENDIANNESS)    /* Big Endian - coldfire CPU */
    /* Flash Status Register (FSTAT)*/
    #define FTFx_SSD_FSTAT_OFFSET               0x00000003U
    /* Flash configuration register (FCNFG)*/
    #define FTFx_SSD_FCNFG_OFFSET               0x00000002U
    /* Flash security register (FSEC) */
    #define FTFx_SSD_FSEC_OFFSET                0x00000001U
    /* Flash Option Register (FOPT) */
    #define FTFx_SSD_FOPT_OFFSET                0x00000000U
    /* Flash common command object registers (FCCOB0-B) */
    #define FTFx_SSD_FCCOB0_OFFSET              0x00000004U
    #define FTFx_SSD_FCCOB1_OFFSET              0x00000005U
    #define FTFx_SSD_FCCOB2_OFFSET              0x00000006U
    #define FTFx_SSD_FCCOB3_OFFSET              0x00000007U
    #define FTFx_SSD_FCCOB4_OFFSET              0x00000008U
    #define FTFx_SSD_FCCOB5_OFFSET              0x00000009U
    #define FTFx_SSD_FCCOB6_OFFSET              0x0000000AU
    #define FTFx_SSD_FCCOB7_OFFSET              0x0000000BU
    #define FTFx_SSD_FCCOB8_OFFSET              0x0000000CU
    #define FTFx_SSD_FCCOB9_OFFSET              0x0000000DU
    #define FTFx_SSD_FCCOBA_OFFSET              0x0000000EU
    #define FTFx_SSD_FCCOBB_OFFSET              0x0000000FU
    /* P-Flash protection registers (FPROT0-3) */
    #define FTFx_SSD_FPROT0_OFFSET              0x00000010U
    #define FTFx_SSD_FPROT1_OFFSET              0x00000011U
    #define FTFx_SSD_FPROT2_OFFSET              0x00000012U
    #define FTFx_SSD_FPROT3_OFFSET              0x00000013U
    /* D-Flash protection registers (FDPROT) */
    #define FTFx_SSD_FDPROT_OFFSET              0x00000014U
    /* EERAM Protection Register (FEPROT)  */
    #define FTFx_SSD_FEPROT_OFFSET              0x00000015U
   
#else /* Little Endian - kinetis CPU + Nevis2 CPU */
    /* Flash Status Register (FSTAT)*/
    #define FTFx_SSD_FSTAT_OFFSET               0x00000000U
    /* Flash configuration register (FCNFG)*/
    #define FTFx_SSD_FCNFG_OFFSET               0x00000001U
    /* Flash security register (FSEC) */
    #define FTFx_SSD_FSEC_OFFSET                0x00000002U
    /* Flash Option Register (FOPT) */
    #define FTFx_SSD_FOPT_OFFSET                0x00000003U
    /* Flash common command object registers (FCCOB0-B) */
    #define FTFx_SSD_FCCOB0_OFFSET              0x00000007U
    #define FTFx_SSD_FCCOB1_OFFSET              0x00000006U
    #define FTFx_SSD_FCCOB2_OFFSET              0x00000005U
    #define FTFx_SSD_FCCOB3_OFFSET              0x00000004U
    #define FTFx_SSD_FCCOB4_OFFSET              0x0000000BU
    #define FTFx_SSD_FCCOB5_OFFSET              0x0000000AU
    #define FTFx_SSD_FCCOB6_OFFSET              0x00000009U
    #define FTFx_SSD_FCCOB7_OFFSET              0x00000008U
    #define FTFx_SSD_FCCOB8_OFFSET              0x0000000FU
    #define FTFx_SSD_FCCOB9_OFFSET              0x0000000EU
    #define FTFx_SSD_FCCOBA_OFFSET              0x0000000DU
    #define FTFx_SSD_FCCOBB_OFFSET              0x0000000CU
    /* P-Flash protection registers (FPROT0-3) */
    #define FTFx_SSD_FPROT0_OFFSET              0x00000013U
    #define FTFx_SSD_FPROT1_OFFSET              0x00000012U
    #define FTFx_SSD_FPROT2_OFFSET              0x00000011U
    #define FTFx_SSD_FPROT3_OFFSET              0x00000010U
    /* D-Flash protection registers (FDPROT) */
    #define FTFx_SSD_FDPROT_OFFSET              0x00000017U
    /* EERAM Protection Register (FEPROT)  */
    #define FTFx_SSD_FEPROT_OFFSET              0x00000016U
#endif

/* fccob offset address to store resource code */
#if (PGM_SIZE_BYTE == FTFx_PHRASE_SIZE)
	#define RSRC_CODE_OFSSET        FTFx_SSD_FCCOB4_OFFSET            
#else
	#define RSRC_CODE_OFSSET        FTFx_SSD_FCCOB8_OFFSET            
#endif

/*------------- Flash hardware algorithm operation commands -------------*/
#define FTFx_VERIFY_BLOCK               0x00U
#define FTFx_VERIFY_SECTION             0x01U
#define FTFx_PROGRAM_CHECK              0x02U
#define FTFx_READ_RESOURCE              0x03U
#define FTFx_PROGRAM_LONGWORD           0x06U
#define FTFx_PROGRAM_PHRASE             0x07U
#define FTFx_ERASE_BLOCK                0x08U
#define FTFx_ERASE_SECTOR               0x09U
#define FTFx_PROGRAM_SECTION            0x0BU
#define FTFx_VERIFY_ALL_BLOCK           0x40U
#define FTFx_READ_ONCE                  0x41U
#define FTFx_PROGRAM_ONCE               0x43U
#define FTFx_ERASE_ALL_BLOCK            0x44U
#define FTFx_SECURITY_BY_PASS           0x45U
#define FTFx_PFLASH_SWAP                0x46U
#define FTFx_PROGRAM_PARTITION          0x80U
#define FTFx_SET_EERAM                  0x81U

/* PFlash swap control codes */
#define FTFx_SWAP_SET_INDICATOR_ADDR    0x01U
#define FTFx_SWAP_SET_IN_PREPARE        0x02U
#define FTFx_SWAP_SET_IN_COMPLETE       0x04U
#define FTFx_SWAP_REPORT_STATUS         0x08U


/* EERAM Function Control Code */
#define EEE_ENABLE                      0x00U
#define EEE_DISABLE                     0xFFU

/* PFlash swap states */
#define FTFx_SWAP_UNINIT                0x00U
#define FTFx_SWAP_READY                 0x01U
#define FTFx_SWAP_UPDATE                0x02U
#define FTFx_SWAP_UPDATE_ERASED         0x03U
#define FTFx_SWAP_COMPLETE              0x04U

/*------------------- Setting flash interrupt macro --------------------*/
#define SET_FLASH_INT_BITS(ftfxRegBase, value)  REG_WRITE((ftfxRegBase) + FTFx_SSD_FCNFG_OFFSET,\
                                                ((value)&(FTFx_SSD_FCNFG_CCIE | FTFx_SSD_FCNFG_RDCOLLIE)))
#define GET_FLASH_INT_BITS(ftfxRegBase)         REG_READ((ftfxRegBase) + FTFx_SSD_FCNFG_OFFSET) &\
                                                (FTFx_SSD_FCNFG_CCIE | FTFx_SSD_FCNFG_RDCOLLIE)

/*---------------- Function Prototypes for Flash SSD --------------------*/

/* Relocate FlashCommandSequence to another address */
extern uint32_t RelocateFunction(uint32_t dest, uint32_t size, uint32_t src);

/* Flash initialization */
extern uint32_t FlashInit(PFLASH_SSD_CONFIG pSSDConfig);

/* Internal function. Called by driver APIs only */
/* Flash command sequence */
extern uint32_t FlashCommandSequence(PFLASH_SSD_CONFIG pSSDConfig);

/* P-Flash get protection */
extern uint32_t PFlashGetProtection(PFLASH_SSD_CONFIG pSSDConfig, \
                                  uint32_t*  protectStatus);      

/* P-Flash set protection */
extern uint32_t PFlashSetProtection(PFLASH_SSD_CONFIG pSSDConfig, \
                                  uint32_t  protectStatus);                                                
 
 
/* Flash get security state */
extern uint32_t FlashGetSecurityState(PFLASH_SSD_CONFIG pSSDConfig, \
                                    uint8_t* securityState);

/* Flash security bypass */
extern uint32_t FlashSecurityBypass(PFLASH_SSD_CONFIG pSSDConfig, \
                                  uint8_t* keyBuffer, \
                                  pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

/* Flash erase all Block */
extern uint32_t FlashEraseAllBlock(PFLASH_SSD_CONFIG pSSDConfig, \
                                 pFLASHCOMMANDSEQUENCE pFlashCommandSequence);
                                

/* Flash verify all block */
extern uint32_t FlashVerifyAllBlock(PFLASH_SSD_CONFIG pSSDConfig, \
                                  uint8_t marginLevel, \
                                  pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

/* Flash erase sector */
extern uint32_t FlashEraseSector(PFLASH_SSD_CONFIG pSSDConfig, \
                               uint32_t dest, \
                               uint32_t size, \
                               pFLASHCOMMANDSEQUENCE pFlashCommandSequence);
/* Flash verify section*/
extern uint32_t FlashVerifySection(PFLASH_SSD_CONFIG pSSDConfig, \
                                 uint32_t dest, \
                                 uint16_t number, \
                                 uint8_t marginLevel, \
                                 pFLASHCOMMANDSEQUENCE pFlashCommandSequence);  

/* Flash erase suspend */
extern uint32_t FlashEraseSuspend(PFLASH_SSD_CONFIG pSSDConfig);

/* Flash erase resume */
extern uint32_t FlashEraseResume(PFLASH_SSD_CONFIG pSSDConfig);
                                                                                                

/* Flash read once */
extern uint32_t FlashReadOnce(PFLASH_SSD_CONFIG pSSDConfig, \
                            uint8_t recordIndex,\
                            uint8_t* pDataArray, \
                            pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

/* Flash program once */
extern uint32_t FlashProgramOnce(PFLASH_SSD_CONFIG pSSDConfig, \
                               uint8_t recordIndex,\
                               uint8_t* pDataArray, \
                               pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

/* Flash read resource */
extern uint32_t FlashReadResource(PFLASH_SSD_CONFIG pSSDConfig, \
                                uint32_t dest, \
                                uint8_t* pDataArray, \
                                uint8_t  resourceSelectCode, \
                                pFLASHCOMMANDSEQUENCE pFlashCommandSequence); 

/* Flash program phrase */
extern uint32_t FlashProgram(PFLASH_SSD_CONFIG pSSDConfig, \
                                   uint32_t dest, \
                                   uint32_t size, \
                                   uint8_t* pData, \
                                   pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

/* Flash program check */
extern uint32_t FlashProgramCheck(PFLASH_SSD_CONFIG pSSDConfig, \
                                uint32_t  dest, \
                                uint32_t  size, \
                                uint8_t*  pExpectedData, \
                                uint32_t* pFailAddr, \
                                uint8_t   marginLevel, \
                                pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

/* Flash checksum */
extern uint32_t FlashCheckSum(PFLASH_SSD_CONFIG pSSDConfig, \
                            uint32_t dest, \
                            uint32_t size, \
                            uint32_t* pSum);

#ifndef FTFA_M
/* Flash program Section */
extern uint32_t FlashProgramSection(PFLASH_SSD_CONFIG pSSDConfig, \
                                  uint32_t dest, \
                                  uint16_t number, \
                                  pFLASHCOMMANDSEQUENCE pFlashCommandSequence);
#endif

#if (!(defined(FTFA_M)) || (defined(BLOCK_COMMANDS)))
/* Flash erase Block */
extern uint32_t FlashEraseBlock(PFLASH_SSD_CONFIG pSSDConfig, \
                                   uint32_t dest, \
                                   pFLASHCOMMANDSEQUENCE pFlashCommandSequence);
                                   
/* Flash verify block */
extern uint32_t FlashVerifyBlock(PFLASH_SSD_CONFIG pSSDConfig, \
                               uint32_t dest, \
                               uint8_t marginLevel, \
                               pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

#endif

#if (DEBLOCK_SIZE != 0x0U)
/* EERAM get protection */
extern uint32_t EERAMGetProtection(PFLASH_SSD_CONFIG pSSDConfig, \
                                 uint8_t*  protectStatus);      

/* EERAM set protection */
extern uint32_t EERAMSetProtection(PFLASH_SSD_CONFIG pSSDConfig, \
                                 uint8_t  protectStatus);    

/* Flash Set EEEEnable */
extern uint32_t SetEEEEnable(PFLASH_SSD_CONFIG pSSDConfig, \
                           uint8_t EEEEnable,pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

/* EEPROM Emulator Write */
extern uint32_t EEEWrite(PFLASH_SSD_CONFIG pSSDConfig, \
                       uint32_t dest, \
                       uint32_t size, \
                       uint8_t* pData);

/* Flash D/E-Flash Partition */
extern uint32_t DEFlashPartition(PFLASH_SSD_CONFIG pSSDConfig, \
                               uint8_t EEEDataSizeCode, \
                               uint8_t DEPartitionCode, \
                               pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

/* D-Flash get protection */
extern uint32_t DFlashGetProtection(PFLASH_SSD_CONFIG pSSDConfig, \
                                  uint8_t*  protectStatus);      

/* D-Flash set protection */
extern uint32_t DFlashSetProtection(PFLASH_SSD_CONFIG pSSDConfig, \
                                  uint8_t  protectStatus);
#endif /* End of DEBLOCK_SIZE */

#ifdef SWAP_M
/* Swap the two P-Flash blocks with each other */
extern uint32_t PFlashSwap(PFLASH_SSD_CONFIG pSSDConfig, \
                  uint32_t addr, \
                  PFLASH_SWAP_CALLBACK pSwapCallback, \
                  pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

extern uint32_t PFlashSwapCtl(PFLASH_SSD_CONFIG pSSDConfig, \
                             uint32_t addr, \
                             uint8_t swapcmd, \
                             uint8_t* pCurrentSwapMode,\
                             uint8_t* pCurrentSwapBlockStatus, \
                             uint8_t* pNextSwapBlockStatus, \
                             pFLASHCOMMANDSEQUENCE pFlashCommandSequence);
#endif /* End of SWAP_M */

#endif  /* _SSD_FTFx_INTERNAL_H_ */
