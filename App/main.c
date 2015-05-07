#include <stdio.h>
#include <stdlib.h>

#include "fsl_device_registers.h"
#include "fsl_clock_manager.h"
#include "fsl_smc_features.h"
#include "fsl_smc_hal.h"
#include "board.h"

#include "fsl_lpuart_driver.h"
#include "fsl_adc_driver.h"

#include "EcgCapture.h"
#include "ConnectPC.h"
#include "BlueTooth.h"
#include "flash_demo.h"
#include "SSD_FTFx.h"

uint8_t     EcgCaptureEnableFlag = 0;

OSA_TASK_DEFINE(task_ecg_capture, TASK_ECG_CAPTURE_STACK_SIZE);
OSA_TASK_DEFINE(task_connect_pc_rx, TASK_CONNECT_PC_STACK_SIZE);
OSA_TASK_DEFINE(task_connect_pc_tx, TASK_CONNECT_PC_STACK_SIZE);
OSA_TASK_DEFINE(task_bluetooth_rx, TASK_BLUETOOTH_STACK_SIZE);
OSA_TASK_DEFINE(task_bluetooth_tx, TASK_BLUETOOTH_STACK_SIZE);


uint32_t runBootloaderAddress;    //在线升级相关
void (*runBootloader)(void * arg);

uint8_t program_buffer[BUFFER_SIZE_BYTE];
pFLASHCOMMANDSEQUENCE g_FlashLaunchCommand = (pFLASHCOMMANDSEQUENCE)0xFFFFFFFF;
/* array to copy __Launch_Command func to RAM */
uint16_t __ram_func[LAUNCH_CMD_SIZE/2];
uint16_t __ram_for_callback[CALLBACK_SIZE/2]; /* length of this array depends on total size of the functions need to be copied to RAM*/
FLASH_SSD_CONFIG flashSSDConfig =
{
    FTFx_REG_BASE,          /* FTFx control register base */
    PFLASH_BLOCK_BASE,      /* base address of PFlash block */
    PBLOCK_SIZE,            /* size of PFlash block */
    DEFLASH_BLOCK_BASE,     /* base address of DFlash block */
    0,                      /* size of DFlash block */
    EERAM_BLOCK_BASE,       /* base address of EERAM block */
    0,                      /* size of EEE block */
    DEBUGENABLE,            /* background debug mode enable bit */
    NULL_CALLBACK           /* pointer to callback function */
};

void ErrorTrap(uint32_t ret)
{
    while (1)
    {
        ;
    }
}

void callback(void)
{
}


int main(void)
{	
	int32_t ret;          /* Return code from each SSD function */
	uint32_t destination;         /* Address of the target location */
	uint32_t size;
	uint32_t end;  
	
	// Read the function address from the ROM API tree.
runBootloaderAddress = **(uint32_t **)(0x1c00001c);
runBootloader = (void (*)(void * arg))runBootloaderAddress;
// Start the bootloader.
//runBootloader(NULL);
	
//	smc_power_mode_protection_config_t smc_power_prot_cfg =
//	{
//		.vlpProt = true,
//		.llsProt = false,
//		.vllsProt = false
//	};
	
	CACHE_DISABLE;
	
	hardware_init();  //硬件管教的初始化    主要是 gpio   tpm   lpuart
	

	ret = FlashInit(&flashSSDConfig);
	if (FTFx_OK != ret)
	{
		ErrorTrap(ret);
	} 

	flashSSDConfig.CallBack = (PCALLBACK)RelocateFunction((uint32_t)__ram_for_callback , CALLBACK_SIZE , (uint32_t)callback);     
	g_FlashLaunchCommand = (pFLASHCOMMANDSEQUENCE)RelocateFunction((uint32_t)__ram_func , LAUNCH_CMD_SIZE ,(uint32_t)FlashCommandSequence);   
	destination = flashSSDConfig.PFlashBlockBase + BYTE2WORD(flashSSDConfig.PFlashBlockSize - 6*FTFx_PSECTOR_SIZE);
	size = FTFx_PSECTOR_SIZE;
	ret = FlashEraseSector(&flashSSDConfig, destination, size, g_FlashLaunchCommand);        
	if (FTFx_OK != ret)
	{
		ErrorTrap(ret);
	}
	
	destination = flashSSDConfig.PFlashBlockBase + BYTE2WORD(flashSSDConfig.PFlashBlockSize - 6*FTFx_PSECTOR_SIZE);
	end = flashSSDConfig.PFlashBlockBase + BYTE2WORD(flashSSDConfig.PFlashBlockSize - 4*FTFx_PSECTOR_SIZE);
	for (uint16_t i = 0; i < BUFFER_SIZE_BYTE; i++)
	{
        /* Set source buffer */
		program_buffer[i] = i;
	}
	size = BUFFER_SIZE_BYTE;
	
	ret = FlashProgram(&flashSSDConfig, destination, size, \
                                       program_buffer, g_FlashLaunchCommand);
	if (FTFx_OK != ret)
	{
		ErrorTrap(ret);
	}
	
	/* Configure the power mode protection */
//	SMC_HAL_SetProtection(SMC_BASE, &smc_power_prot_cfg);	
	
	OSA_Init();   //直接返回 OSA_Success   实时操作系统的初始化     freertos没有函数体  直接返回   
	
	dbg_uart_init();

	BT_uart_init();
	
	OSA_TaskCreate(task_ecgcapture,
                   (uint8_t*) "ecgcapture",
                    TASK_ECG_CAPTURE_STACK_SIZE,
                    task_ecg_capture_stack,
                    TASK_ECG_CAPTURE_PRIO,
                    (task_param_t)0,
                    false,
                    &task_ecg_capture_task_handler);
	OSA_TaskCreate(task_bluetooth_rx,
                   (uint8_t*) "bluetooth_rx",
                    TASK_BLUETOOTH_STACK_SIZE,
                    task_bluetooth_rx_stack,
                    TASK_BLUETOOTH_RX_PRIO,
                    (task_param_t)0,
                    false,
                    &task_bluetooth_rx_task_handler);
	OSA_TaskCreate(task_bluetooth_tx,
                   (uint8_t*) "bluetooth_tx",
                    TASK_BLUETOOTH_STACK_SIZE,
                    task_bluetooth_tx_stack,
                    TASK_BLUETOOTH_TX_PRIO,
                    (task_param_t)0,
                    false,
                    &task_bluetooth_tx_task_handler);
	OSA_TaskCreate(task_connnectpc_rx,
                   (uint8_t*) "connectpc_rx",
                    TASK_CONNECT_PC_STACK_SIZE,
                    task_connect_pc_rx_stack,
                    TASK_CONNECT_PC_PRIO,
                    (task_param_t)0,
                    false,
                    &task_connect_pc_rx_task_handler);
    OSA_TaskCreate(task_connnectpc_tx,
                   (uint8_t*) "connectpc_tx",
                    TASK_CONNECT_PC_STACK_SIZE,
                    task_connect_pc_tx_stack,
                    TASK_CONNECT_PC_PRIO,
                    (task_param_t)0,
                    false,
                    &task_connect_pc_tx_task_handler);
	
	OSA_Start();
	for(;;) {}                    /* Should not achieve here */								
}
