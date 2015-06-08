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
uint8_t     ECGDataSendFlag = 1;    //心电数据发送时能标志

static lpuart_state_t s_bt_lpuart[2];

extern msg_queue_handler_t hBTMsgQueue;  //心电数据发送队列 

extern uint8_t ECGPatchID[15];

uint8_t	HardWareVersion[4] = {"1.00"};
uint8_t	SoftWareVersion[4] = {"1.00"};
uint8_t	SucessFlag[1] = {0x01};

unsigned char MACEDR[12];
unsigned char MACBLE[12];

const unsigned char NameEDR[] = "HM502B2_EDR";
const unsigned char NameBLE[] = "HM502B2_BLE";

BTTransmitPackage m_btdatapackage;

extern FlashDataPackage flashdatapackage;


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
		if(memcmp(&rxbuffer[7],NameBLE,sizeof(NameBLE)))
		{
			while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,SETNAMB,strlen(SETNAMB), portMAX_DELAY));
			while ( kStatus_LPUART_RxBusy ==  LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
		}
		while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,GETNAME,strlen(GETNAME), portMAX_DELAY));
    while ( kStatus_LPUART_RxBusy ==  LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
		if(memcmp(rxbuffer,NameEDR,sizeof(NameEDR)))
		{
			while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,SETNAME,strlen(SETNAME), portMAX_DELAY));
			while ( kStatus_LPUART_RxBusy ==  LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
		}
		
		
	
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

void LedSet(uint8_t HighTime,uint8_t HighNum,uint16_t PeriodTime)
{
	static uint16_t time = 0;
	for(uint8_t i = 0;i<HighNum;i++)
	{
		if(time == (HighTime*i))
		{
			LED1_ON;
		}
		else if(time == (HighTime*i)+(HighTime/2))
		{
			LED1_OFF;
		}
//		else if(time == PeriodTime)
//		{
//			
//		}
		
	}
	time++;
	if(time >= PeriodTime)	
	{
		time = 0;
	}			
}


void task_bluetooth_tx(task_param_t param)
{
	if(BLEConnectedFlag == 0)
	{
		InitBlueTooth();
	}
    
	while(1)
	{
      OSA_MsgQGet(hBTMsgQueue,&m_btdatapackage,portMAX_DELAY); 
			{

					if(m_btdatapackage.code == ECGDATACODE)
					{
						if(ECGDataSendFlag == 1)
						{
							while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,(uint8_t*)&m_btdatapackage.data,m_btdatapackage.size)); 
							if(m_btdatapackage.data[5] == 1)
							{
								LedSet(2,1,25);
							}
							else
							{
								LedSet(2,3,25);
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
					else if(m_btdatapackage.code == SENDECGDISABLECODE)
					{
						while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,(uint8_t*)&m_btdatapackage.data,m_btdatapackage.size)); 
					}
					else if(m_btdatapackage.code == SENDSET1MVCODE)
					{
						while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,(uint8_t*)&m_btdatapackage.data,m_btdatapackage.size)); 
					}
					else if(m_btdatapackage.code == SENDSET0MVCODE)
					{
						while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,(uint8_t*)&m_btdatapackage.data,m_btdatapackage.size)); 
					}
					else if(m_btdatapackage.code == SENDGET1MVCODE)
					{
						while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,(uint8_t*)&m_btdatapackage.data,m_btdatapackage.size)); 
					}
					else if(m_btdatapackage.code == SENDGET0MVCODE)
					{
						while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,(uint8_t*)&m_btdatapackage.data,m_btdatapackage.size)); 
					}
			}
			
	}
}

void task_bluetooth_rx(task_param_t param)
{
	uint8_t bluerxbuffer[100];
    
	while(1)
	{
       
       if(kStatus_LPUART_Timeout == LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,bluerxbuffer,20,100))
       {
/************************************************************************************************/ 
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
/************************************************************************************************/	
						if(bluerxbuffer[0] == 	0x01)
						{
							bluerxbuffer[0] = 0;
						}							
					 if(bluerxbuffer[0] == SERIAL_IDENTIFIER)
					 {
						 if(bluerxbuffer[2] == SERIAL_STATUS_OK)
						 {
								if(bluerxbuffer[1] == APP_CMD_RECEIVEECGDATAACK)    //开始发送心电数据
								{
									uint8_t data[1] = {0x01};
									BlueToothSendCommand(SENDECGENABLECODE,APP_CMD_RECEIVEECGDATAREQ,SERIAL_DATASIZE_ONE,SucessFlag);
									ECGDataSendFlag  = 1;									
								}
								else if(bluerxbuffer[1] == APP_CMD_STOPRECEIVEECGDATAACK)				//停止发送心电数据
								{
									BlueToothSendCommand(SENDECGDISABLECODE,APP_CMD_STOPRECEIVEECGDATAREQ,SERIAL_DATASIZE_NONE,SERIAL_DATAADDR_NONE);
									ECGDataSendFlag  = 0;									
								}
								else if(bluerxbuffer[1] == APP_CMD_ECGPATCHIDACK)				//发送ID值
								{
									BlueToothSendCommand(SENDECGPATCHIDCODE,APP_CMD_ECGPATCHIDREQ,ECGPATCHIDSIZE,ECGPatchID); 							
								}
								else if(bluerxbuffer[1] == APP_CMD_ECGPATCHHARDVERSIONACK)				//发送硬件版本
								{
									BlueToothSendCommand(SENDHARDVERSIONCODE,APP_CMD_ECGPATCHHARDVERSIONREQ,ECGPATCHHARDVERSIONSIZE,HardWareVersion); 							
								}
								else if(bluerxbuffer[1] == APP_CMD_ECGPATCHSOFTVERSIONACK)				//发送软件版本
								{
									BlueToothSendCommand(SENDSOFTVERSIONCODE,APP_CMD_ECGPATCHSOFTVERSIONREQ,ECGPATCHSOFTVERSIONSIZE,SoftWareVersion); 							
								}
								else if(bluerxbuffer[1] == APP_CMD_SET1MVVALUE)				//保存1mv定标值
								{
									BlueToothSendCommand(SENDSET1MVCODE,APP_CMD_SET1MVVALUEACK,SERIAL_DATASIZE_ONE,SucessFlag); 
									flashdatapackage.amplification = bluerxbuffer[4] + (bluerxbuffer[5]<<8);
									WriteData2Flash();
									ReadData4Flash();	
									
								}
								else if(bluerxbuffer[1] == APP_CMD_SET0MVVALUE)				//保存0mv校准值
								{
									BlueToothSendCommand(SENDSET0MVCODE,APP_CMD_SET0MVVALUEACK,SERIAL_DATASIZE_ONE,SucessFlag); 
									flashdatapackage.difference_Value = bluerxbuffer[4] + (bluerxbuffer[5]<<8);
									WriteData2Flash();
									ReadData4Flash();									
								}
								else if(bluerxbuffer[1] == APP_CMD_GET1MVVALUE)				//获取1mv定标值
								{
									uint8_t data[2];
									data[0]= 	flashdatapackage.amplification;	
									data[1] = (flashdatapackage.amplification>>8);							
									BlueToothSendCommand(SENDGET1MVCODE,APP_CMD_GET1MVVALUEACK,SERIAL_DATASIZE_TWO,data); 	
								}
								else if(bluerxbuffer[1] == APP_CMD_GET0MVVALUE)				//获取0mv校准值
								{
									uint8_t data[2];
									data[0]= 	flashdatapackage.difference_Value;	
									data[1] = (flashdatapackage.difference_Value>>8);
									BlueToothSendCommand(SENDGET0MVCODE,APP_CMD_GET0MVVALUEACK,SERIAL_DATASIZE_TWO,data); 						
								}
						 }
					 }
/************************************************************************************************/	
        memset(bluerxbuffer,0,100);
       }
       
	}
}

void BlueToothSendCommand(uint8_t code,uint8_t command,uint8_t DataSize,uint8_t *Data)
{
		m_btdatapackage.code = code;
		m_btdatapackage.size = 4+DataSize;
		m_btdatapackage.data[0] = SERIAL_IDENTIFIER;
		m_btdatapackage.data[1] = command;
		m_btdatapackage.data[2] = SERIAL_STATUS_OK;
		m_btdatapackage.data[3] = SERIAL_DATASIZE_NONE + DataSize;
		memcpy(&m_btdatapackage.data[4],Data,DataSize);
		OSA_MsgQPut(hBTMsgQueue,&m_btdatapackage); 
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