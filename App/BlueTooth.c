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

uint8_t     BLEConnectedFlag = 0;    //BLE连接状态
uint8_t     ECGDataSendFlag = 0;    //心电数据发送时能标志

static lpuart_state_t s_bt_lpuart[2];

extern msg_queue_handler_t hBTMsgQueue;  //心电数据发送队列 

extern uint8_t ECGPatchID[15];

BTDataPackage m_btdatapackage;

                                                                                                                                                                                                                                                            
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



void InitBlueTooth()
{
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
}


void task_bluetooth_tx(task_param_t param)
{

	uint8_t i;
	//InitBlueTooth();
    
	while(1)
	{
      OSA_MsgQGet(hBTMsgQueue,&m_btdatapackage,portMAX_DELAY); 
			{

					if(m_btdatapackage.code == ECGDATACODE)
					{
						i++;
						if(ECGDataSendFlag == 1)
						{
							while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,(uint8_t*)&m_btdatapackage.data,m_btdatapackage.size)); 
							if(i%2)
							{
								LED1_ON;    
							}
							else
							{
								LED1_OFF;
							} 
						}
					}
					else if(m_btdatapackage.code == SENDECGPATCHIDCODE)
					{
						while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,(uint8_t*)&m_btdatapackage.data,m_btdatapackage.size)); 
					}
					else if(m_btdatapackage.code == SENDECGENABLECODE)
					{
						while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,(uint8_t*)&m_btdatapackage.data,m_btdatapackage.size)); 
					}
			}
	}
}

void task_bluetooth_rx(task_param_t param)
{
	BTDataPackage m_btdatapackage;
	uint8_t bluerxbuffer[100];
    
	while(1)
	{
        
       if(kStatus_LPUART_Timeout == LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,bluerxbuffer,20,100))
       {
           if((bluerxbuffer[0] == 'O') && (bluerxbuffer[1] == 'K') && (bluerxbuffer[2] == '+'))
           {
                if((bluerxbuffer[3] == 'C') && (bluerxbuffer[4] == 'O') && (bluerxbuffer[5] == 'N')&& (bluerxbuffer[6] == 'B'))
                {
                  //  BLEConnectedFlag = 1;
                }
                else if((bluerxbuffer[3] == 'L') && (bluerxbuffer[4] == 'S') && (bluerxbuffer[5] == 'T')&& (bluerxbuffer[6] == 'B'))
                {
                    BLEConnectedFlag = 0;
                    LED1_OFF;
                }
           }
					 if(bluerxbuffer[0] == 0x77)
					 {
						 if(bluerxbuffer[2] == 0x00)
						 {
								if(bluerxbuffer[1] == 0x14)    //开始发送心电数据
								{
									m_btdatapackage.code = SENDECGENABLECODE;
									m_btdatapackage.size = 4;
									m_btdatapackage.data[0] = 0x77;
									m_btdatapackage.data[1] = 0x15;
									m_btdatapackage.data[2] = 0;
									m_btdatapackage.data[3] = 0;
									OSA_MsgQPut(hBTMsgQueue,&m_btdatapackage); 
									ECGDataSendFlag  = 1;									
								}
								if(bluerxbuffer[1] == 0x16)				//停止发送心电数据
								{
									m_btdatapackage.code = SENDECGENABLECODE;
									m_btdatapackage.size = 4;
									m_btdatapackage.data[0] = 0x77;
									m_btdatapackage.data[1] = 0x17;
									m_btdatapackage.data[2] = 0;
									m_btdatapackage.data[3] = 0;
									OSA_MsgQPut(hBTMsgQueue,&m_btdatapackage); 
									ECGDataSendFlag  = 0;									
								}
								if(bluerxbuffer[1] == 0x18)				//发送ID值
								{
									m_btdatapackage.code = SENDECGPATCHIDCODE;
									m_btdatapackage.data[0] = 0x77;
									m_btdatapackage.data[1] = 0x19;
									m_btdatapackage.data[2] = 0;
									m_btdatapackage.data[3] = 15;
									m_btdatapackage.size = 4+m_btdatapackage.data[3];
									memcpy(&m_btdatapackage.data[4],ECGPatchID,15);
									OSA_MsgQPut(hBTMsgQueue,&m_btdatapackage); 							
								}
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