/*****************************************************************************
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
******************************************************************************

******************************************************************************
*                                                                            *
*        Standard Software Flash Driver For FTFx                             *
*                                                                            *
* FILE NAME     :  FlashEraseBlock.c                                         *
* DATE          :  April 08, 2014                                            *
*                                                                            *
* AUTHOR        :  FPT Team                                                  *
* E-mail        :  r56611@freescale.com                                      *
*                                                                            *
******************************************************************************/

/************************** CHANGES ******************************************
0.0.1       06.09.2010      FPT Team      Initial Version
1.0.0       12.25.2013      FPT Team      Optimize code 
1.0.2       08.04.2014      FPT Team      Update to follow SDK convention(MISRA-C)
                                          Add uint32_t temp variable
******************************************************************************/
/* include the header files */
#include "SSD_FTFx.h"

/************************************************************************
*
*  Function Name    : FlashVerifyBlock.c
*  Description      : This function will check to see if an entire 
*                     P-Flash or D-Flash block has been erased to the
*                     specified margin level.
*  Arguments        : PFLASH_SSD_CONFIG, uint32_t,uint8_t, pFLASHCOMMANDSEQUENCE
*  Return Value     : uint32_t
*
*************************************************************************/
#if (!(defined(FTFA_M)) || (defined(BLOCK_COMMANDS)))

/* Enable size optimization */
#if(ARM_CORTEX_M != CPU_CORE)
#pragma optimize_for_size on
#pragma optimization_level 4
#endif /* End of CPU_CORE */

uint32_t SIZE_OPTIMIZATION FlashVerifyBlock(PFLASH_SSD_CONFIG pSSDConfig, \
                                   uint32_t dest, \
                                   uint8_t marginLevel, \
                                   pFLASHCOMMANDSEQUENCE pFlashCommandSequence)
{
    uint32_t ret = FTFx_OK;      /* return code variable */
    uint32_t temp;
    
    /* convert to byte address */
    dest = WORD2BYTE(dest);
    /* check if the destination is aligned or not */
#if (DEBLOCK_SIZE)
    temp = WORD2BYTE(pSSDConfig->DFlashBlockBase);
    if((dest >= temp) && (dest < (temp + pSSDConfig->DFlashBlockSize)))
    {
        dest = dest - temp + 0x800000U;
    }  
    else
#endif
    {
        temp = WORD2BYTE(pSSDConfig->PFlashBlockBase);
        if((dest >= temp) && (dest < (temp + pSSDConfig->PFlashBlockSize)))    
        {
            dest -= temp;
        }
        else
        {
        ret = FTFx_ERR_ACCERR;
        }
    }
    if(FTFx_OK == ret)
    {
        /* clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear*/
        temp = pSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET;
        REG_WRITE(temp, FTFx_SSD_FSTAT_ERROR_BITS);
        
        /* passing parameter to the command */
        temp = pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB0_OFFSET;
        REG_WRITE(temp, FTFx_VERIFY_BLOCK);
        
        temp = pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB1_OFFSET;
        REG_WRITE(temp, GET_BIT_16_23(dest));
        
        temp = pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB2_OFFSET;
        REG_WRITE(temp, GET_BIT_8_15(dest));
        
        temp = pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB3_OFFSET;
        REG_WRITE(temp, GET_BIT_0_7(dest));
        
        temp = pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB4_OFFSET;
        REG_WRITE(temp, marginLevel);
        
        /* calling flash command sequence function to execute the command */
        ret = pFlashCommandSequence(pSSDConfig);
    }
    /* Enter Debug state if enabled */
     if (TRUE == (pSSDConfig->DebugEnable))
    {
        ENTER_DEBUG_MODE;
    }

    return(ret);
}
#endif /* End of  FTFE_M and BLOCK_COMMANDS*/
/* End of file */
