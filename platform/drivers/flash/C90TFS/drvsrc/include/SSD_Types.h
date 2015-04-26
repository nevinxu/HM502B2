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

/************************** CHANGES *****************************************
0.0.1       06.09.2010      FPT Team        Initial Version
1.0.0       12.25.2013      FPT Team        Finalize to 1.0.0
1.0.2       08.04.2014      FPT Team        Update to follow SDK 
                                            convention(MISRA-C)
****************************************************************************/

#ifndef _SSD_TYPES_H_
#define _SSD_TYPES_H_

#ifndef FALSE
#define FALSE 0x0U
#endif

#ifndef TRUE
#define TRUE 0x01U
#endif

#include <stdint.h>
#include <stdbool.h>

typedef volatile signed char vint8_t;
typedef volatile unsigned char vuint8_t;
typedef volatile signed short vint16_t;
typedef volatile unsigned short vuint16_t;
typedef volatile signed long vint32_t;
typedef volatile unsigned long vuint32_t;

#if (defined __MWERKS__)
typedef signed char int8_t;
typedef unsigned char uint8_t; 
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed long int32_t;
typedef unsigned long uint32_t;
#endif


/*-------------------- Callback function prototype ---------------------*/
typedef void (* PCALLBACK)(void);
typedef bool (* PFLASH_SWAP_CALLBACK)(uint8_t function);


/*---------------- Flash SSD Configuration Structure -------------------*/
typedef struct _ssd_config
{
    uint32_t      ftfxRegBase;        /* FTFx control register base */
    uint32_t      PFlashBlockBase;    /* base address of PFlash block */
    uint32_t      PFlashBlockSize;    /* size of PFlash block */
    uint32_t      DFlashBlockBase;    /* base address of DFlash block */
    uint32_t      DFlashBlockSize;    /* size of DFlash block */
    uint32_t      EERAMBlockBase;     /* base address of EERAM block */
    uint32_t      EEEBlockSize;       /* size of EEE block */
    bool          DebugEnable;        /* background debug mode enable bit */
    PCALLBACK     CallBack;           /* pointer to callback function */
} FLASH_SSD_CONFIG, *PFLASH_SSD_CONFIG;

/* -------------------- Function Pointer ------------------------------- */
typedef uint32_t (*pFLASHCOMMANDSEQUENCE) (PFLASH_SSD_CONFIG pSSDConfig);

/* Flash initialization */
typedef uint32_t (*pFLASHINIT) (PFLASH_SSD_CONFIG pSSDConfig);

/* P-Flash get protection */
typedef uint32_t (*pPFLASHGETPROTECTION) (PFLASH_SSD_CONFIG pSSDConfig, \
                                        uint32_t*  protectStatus);
                                              
/* P-Flash set protection */
typedef uint32_t (*pPFLASHSETPROTECTION) (PFLASH_SSD_CONFIG  pSSDConfig, \
                                        uint32_t  protectStatus);
                                                        
/* Flash get security state */
typedef uint32_t (*pFLASHGETSECURITYSTATE) (PFLASH_SSD_CONFIG pSSDConfig, \
                                          uint8_t* securityState);
                                                        
/* Flash security bypass */
typedef uint32_t (*pFLASHSECURITYBYPASS) (PFLASH_SSD_CONFIG pSSDConfig, \
                                        uint8_t* keyBuffer, \
                                        pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

/* Flash erase all Block */
typedef uint32_t (*pFLASHERASEALLBLOCK) (PFLASH_SSD_CONFIG pSSDConfig, \
                                       pFLASHCOMMANDSEQUENCE pFlashCommandSequence);
                                
/* Flash erase Block */
typedef uint32_t (*pFLASHERASEBLOCK) (PFLASH_SSD_CONFIG pSSDConfig, \
                                    uint32_t dest, \
                                    pFLASHCOMMANDSEQUENCE pFlashCommandSequence);
                                    
/* Flash erase sector */
typedef uint32_t (*pFLASHERASESECTOR) (PFLASH_SSD_CONFIG pSSDConfig, \
                                     uint32_t dest, \
                                     uint32_t size, \
                                     pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

/* Flash erase suspend */
typedef uint32_t (*pFLASHERASESUSPEND) (PFLASH_SSD_CONFIG pSSDConfig);

/* Flash erase resume */
typedef uint32_t (*pFLASHERASERESUME) (PFLASH_SSD_CONFIG pSSDConfig);                                                       
    
/* Flash program Section */
typedef uint32_t (*pFLASHPROGRAMSECTION) (PFLASH_SSD_CONFIG pSSDConfig, \
                                        uint32_t dest, \
                                        uint16_t number, \
                                        pFLASHCOMMANDSEQUENCE pFlashCommandSequence);                                                
                                   
/* Flash checksum */
typedef uint32_t (*pFLASHCHECKSUM) (PFLASH_SSD_CONFIG pSSDConfig, \
                                  uint32_t dest, \
                                  uint32_t size, \
                                  uint32_t* pSum);
                                           
/* Flash verify all block */
typedef uint32_t (*pFLASHVERIFYALLBLOCK) (PFLASH_SSD_CONFIG pSSDConfig, \
                                        uint8_t marginLevel, \
                                        pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

/* Flash verify block */
typedef uint32_t (*pFLASHVERIFYBLOCK) (PFLASH_SSD_CONFIG pSSDConfig, \
                                     uint32_t dest, \
                                     uint8_t marginLevel, \
                                     pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

/* Flash verify section*/
typedef uint32_t (*pFLASHVERIFYSECTION) (PFLASH_SSD_CONFIG pSSDConfig, \
                                       uint32_t dest, \
                                       uint16_t number, \
                                       uint8_t marginLevel, \
                                       pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

/* Flash read once */
typedef uint32_t (*pFLASHREADONCE) (PFLASH_SSD_CONFIG pSSDConfig, \
                                  uint8_t* pDataArray, \
                                  pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

/* Flash program once */
typedef uint32_t (*pFLASHPROGRAMONCE) (PFLASH_SSD_CONFIG pSSDConfig, \
                                     uint8_t* pDataArray, \
                                     pFLASHCOMMANDSEQUENCE pFlashCommandSequence);
/* Flash program check */
typedef uint32_t (*pFLASHPROGRAMCHECK) (PFLASH_SSD_CONFIG pSSDConfig, \
                                      uint32_t  dest, \
                                      uint32_t  size, \
                                      uint8_t*  pExpectedData, \
                                      uint32_t* pFailAddr, \
                                      uint8_t   marginLevel, \
                                      pFLASHCOMMANDSEQUENCE pFlashCommandSequence);
                                                      
/* Flash read resource */
typedef uint32_t (*pFLASHREADRESOURCE) (PFLASH_SSD_CONFIG pSSDConfig, \
                                      uint32_t dest, \
                                      uint8_t* pDataArray, \
                                      uint8_t  resourceSelectCode, \
                                      pFLASHCOMMANDSEQUENCE pFlashCommandSequence);              

/* Flash program Phrase */
typedef uint32_t (*pFLASHPROGRAM) (PFLASH_SSD_CONFIG pSSDConfig, \
                                         uint32_t dest, \
                                         uint32_t size, \
                                         uint8_t* pData, \
                                         pFLASHCOMMANDSEQUENCE pFlashCommandSequence);                                       

/* PFlash Get swap status */
typedef uint32_t (*pPFLASHGETSWAPSTATUS) (PFLASH_SSD_CONFIG pSSDConfig, \
                                        uint32_t flashAddress, \
                                        uint8_t* pCurrentSwapMode, \
                                        uint8_t* pCurrentSwapBlockStatus, \
                                        uint8_t* pNextSwapBlockStatus, \
                                        pFLASHCOMMANDSEQUENCE pFlashCommandSequence);

/* Swap the two P-Flash blocks with each other */
typedef uint32_t (*PFLASHSWAP)(PFLASH_SSD_CONFIG pSSDConfig, \
                             uint32_t flashAddress, \
                             PFLASH_SWAP_CALLBACK pSwapCallback, \
                             pFLASHCOMMANDSEQUENCE pFlashCommandSequence);
                             
/* D-Flash get protection */
typedef uint32_t (*pDFLASHGETPROTECTION) (PFLASH_SSD_CONFIG pSSDConfig, \
                                        uint8_t*  protectStatus);    
/* D-Flash set protection */
typedef uint32_t (*pDFLASHSETPROTECTION) (PFLASH_SSD_CONFIG pSSDConfig, \
                                        uint8_t  protectStatus);
                                    
/* EERAM get protection */
typedef uint32_t (*pEERAMGETPROTECTION) (PFLASH_SSD_CONFIG pSSDConfig, \
                                       uint8_t*  protectStatus);      

/* EERAM set protection */
typedef uint32_t (*pEERAMSETPROTECTION) (PFLASH_SSD_CONFIG pSSDConfig, \
                                       uint8_t  protectStatus);
/* Flash D/E-Flash Partition */
typedef uint32_t (*pDEFLASHPARTITION) (PFLASH_SSD_CONFIG pSSDConfig, \
                                     uint8_t EEEDataSizeCode, \
                                     uint8_t DEPartitionCode, \
                                     pFLASHCOMMANDSEQUENCE pFlashCommandSequence);
/* Flash Set EEEEnable */
typedef uint32_t (*pSETEEEENABLE) (PFLASH_SSD_CONFIG pSSDConfig, \
                                 uint8_t EEEEnable,pFLASHCOMMANDSEQUENCE pFlashCommandSequence);                                    
/* EEPROM Emulator Write */
typedef uint32_t (*pEEEWRITE) (PFLASH_SSD_CONFIG pSSDConfig, \
                             uint32_t dest, \
                             uint32_t size, \
                             uint8_t* pData);      
typedef void (*FLASHLAUNCHCOMMAND) (PFLASH_SSD_CONFIG pSSDConfig); 
#endif /* _SSD_TYPES_H_ */
