#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "board.h"

#include "fsl_clock_manager.h"
#include "fsl_os_abstraction.h"

#include "fsl_lpuart_hal.h"
#include "fsl_lpuart_common.h"
#include "fsl_lpuart_driver.h"

#include "BlueTooth.h"
#include "EcgCapture.h"

uint8_t     BLEConnectedFlag = 1;    //BLE连接状态

static lpuart_state_t s_bt_lpuart[2];

extern msg_queue_handler_t hBTEcgMsgQueue;  //心电数据发送队列 

struct EcgDataPackage m_ecgdatapackage2;   //心电数据结构
extern msg_queue_handler_t hPCEcgMsgQueue;  //心电数据发送队列 

                                                                                                                                                                                                                                                            
unsigned char MACEDR[12];
unsigned char MACBLE[12];
unsigned char NAMEEDR[64];
unsigned char NAMEBLE[64];


uint8_t OKGetReturn(uint8_t *buffer)
{
    if(buffer[0] == 'O' && (buffer[1] == 'K') && (buffer[2] == '+') && (buffer[3] == 'G') && (buffer[4] == 'e')&& (buffer[5] == 't'))
    {
        return buffer[7];
    }
    return 0xff;
}

void task_bluetooth_tx(task_param_t param)
{
    struct EcgDataPackage m_ecgdatapackage;   //心电数据结构
    lpuart_status_t ret;
    
    uint8_t rxbuffer[100];
    
    while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,GETNAMB,strlen(GETNAMB), portMAX_DELAY));
    while ( kStatus_LPUART_RxBusy ==  LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
    
    while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,GETPI10,strlen(GETPI10))); //
    while ( kStatus_LPUART_RxBusy == LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
    if(OKGetReturn(rxbuffer) == '1')
    {
        while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,SETPI100,strlen(SETPI100))); //待机慢闪，连接后常亮
        while ( kStatus_LPUART_RxBusy == LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
    }
    
    while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,GETADDE,strlen(GETADDE))); //获取 EDR MAC
    while ( kStatus_LPUART_RxBusy ==  LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
    memcpy(MACEDR,rxbuffer+7,12);

    while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,GETADDB,strlen(GETADDB))); //获取 BLE MAC
    while ( kStatus_LPUART_RxBusy == LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
    memcpy(MACBLE,rxbuffer+7,12);
    
    while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,GETNOTI,strlen(GETNOTI))); //
    while ( kStatus_LPUART_RxBusy == LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
    if(OKGetReturn(rxbuffer) == '0')
    {
        while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,SETNOTI1,strlen(SETNOTI1))); //连接后通知上位机
        while ( kStatus_LPUART_RxBusy == LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
    }
    
    
	while(1)
	{
      OSA_MsgQGet(hPCEcgMsgQueue,&m_ecgdatapackage2,portMAX_DELAY);  
			while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_DEBUG_UART_INSTANCE,(uint8_t*)&m_ecgdatapackage2,12));            
		
	}
}

void task_bluetooth_rx(task_param_t param)
{
    uint8_t bluerxbuffer[100];
    
	while(1)
	{
        
       if(kStatus_LPUART_Timeout == LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,bluerxbuffer,20,200))
       {
           if((bluerxbuffer[0] == 'O') && (bluerxbuffer[1] == 'K') && (bluerxbuffer[2] == '+'))
           {
                if((bluerxbuffer[3] == 'C') && (bluerxbuffer[4] == 'O') && (bluerxbuffer[5] == 'N')&& (bluerxbuffer[6] == 'B'))
                {
                    BLEConnectedFlag = 1;
                }
                else if((bluerxbuffer[3] == 'L') && (bluerxbuffer[4] == 'S') && (bluerxbuffer[5] == 'T')&& (bluerxbuffer[6] == 'B'))
                {
                    BLEConnectedFlag = 0;
                    LED1_OFF;
                }
           }
        memset(bluerxbuffer,0,100);
       }
       
	}
}


lpuart_status_t lpuart_Init(
        uint32_t uartInstance, uint32_t baudRate)
{
    /* Declare config sturcuture to initialize a uart instance. */
    lpuart_user_config_t lpuartConfig;

    /* Config the structure. */
    lpuartConfig.baudRate = baudRate;
    lpuartConfig.bitCountPerChar = kLpuart8BitsPerChar;
    lpuartConfig.parityMode = kLpuartParityDisabled;
    lpuartConfig.stopBitCount = kLpuartOneStopBit;

    /* Init LPUART device. */
    LPUART_DRV_Init(uartInstance, &s_bt_lpuart[uartInstance], &lpuartConfig);

   // s_bt_lpuart.instance = uartInstance;
    return kStatus_LPUART_Success;
}
/* See fsl_debug_console.h for documentation of this function.*/
lpuart_status_t lpuart_DeInit(uint32_t uartInstance)
{
    LPUART_DRV_Deinit(uartInstance);
    return kStatus_LPUART_Success;
}