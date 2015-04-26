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

uint8_t     EcgCaptureEnableFlag = 0;

OSA_TASK_DEFINE(task_ecg_capture, TASK_ECG_CAPTURE_STACK_SIZE);
OSA_TASK_DEFINE(task_connect_pc_rx, TASK_CONNECT_PC_STACK_SIZE);
OSA_TASK_DEFINE(task_connect_pc_tx, TASK_CONNECT_PC_STACK_SIZE);
OSA_TASK_DEFINE(task_bluetooth_rx, TASK_BLUETOOTH_STACK_SIZE);
OSA_TASK_DEFINE(task_bluetooth_tx, TASK_BLUETOOTH_STACK_SIZE);


uint32_t runBootloaderAddress;    //在线升级相关
void (*runBootloader)(void * arg);


int main(void)
{	
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
    
	hardware_init();  //硬件管教的初始化    主要是 gpio   tpm   lpuart
	
	
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
//	OSA_TaskCreate(task_connnectpc_rx,
//                   (uint8_t*) "connectpc_rx",
//                    TASK_CONNECT_PC_STACK_SIZE,
//                    task_connect_pc_rx_stack,
//                    TASK_CONNECT_PC_PRIO,
//                    (task_param_t)0,
//                    false,
//                    &task_connect_pc_rx_task_handler);
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
