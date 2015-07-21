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
#include "ConnectPC.h"

OSA_TASK_DEFINE(task_bluetooth_rx, TASK_BLUETOOTH_STACK_SIZE);

const uint8_t  AT[2]  = "AT";   
static uint8_t  SETNAME[18]   = "AT+NAMEED000000000";
static uint8_t  SETNAMB[18]   = "AT+NAMBBD000000000";

uint8_t     BLEConnectedFlag = 0;    //BLE连接状态
uint8_t     ECGDataSendFlag = 0;    //心电数据发送时能标志

static lpuart_state_t s_bt_lpuart[2];

extern msg_queue_handler_t hBTMsgQueue;  //心电数据发送队列 

uint8_t	HardWareVersion[5] = {"1.1.0"};
uint8_t	SoftWareVersion[12] = {"1.0.3.150716"};   //sdk1.0.0   2015.06.17

unsigned char MACEDR[12];
unsigned char MACBLE[12];

BTTransmitPackage m_btdatapackage;

extern FlashDataPackage flashdatapackage;

uint8_t g_RxBuffer[20];
uint8_t RxSuccessFlag = 0;

extern uint8_t BTSendNum;
extern uint8_t BTSendSuccessFlag;

extern EcgDataPackage ecgdatapackage;

extern int32_t WriteData2Flash();
extern int32_t ReadData4Flash();

uint8_t BTInitFlag = 0;

extern uint8_t CalibrationFlag;

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
	uint32_t bytesRemaining;
	
	while(GPIO_DRV_ReadPinInput(kGpioBTPIO1) == 0);
    
	//连接断开   未连接返回OK
	while ( kStatus_LPUART_Success != LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,AT,sizeof(AT), portMAX_DELAY));
  while ( kStatus_LPUART_Timeout !=  LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100)); 

	LPUART_DRV_GetReceiveStatus(BOARD_BT_UART_INSTANCE,&bytesRemaining);
	memset(rxbuffer,0,100);	

	//单模收发
	while ( kStatus_LPUART_Success != LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,GETDUAL,strlen(GETDUAL), portMAX_DELAY));
  while ( kStatus_LPUART_Timeout !=  LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 1000));  
	if(OKGetReturn(rxbuffer) == '0')
	{
        while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,SETDUAL1,strlen(SETDUAL1),portMAX_DELAY));
        while ( kStatus_LPUART_RxBusy == LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
	}
	LPUART_DRV_GetReceiveStatus(BOARD_BT_UART_INSTANCE,&bytesRemaining);
	memset(rxbuffer,0,100);	
	
	//透传+远控模式
	while ( kStatus_LPUART_Success != LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,GETMODE,strlen(GETMODE), portMAX_DELAY));
  while ( kStatus_LPUART_Timeout !=  LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));  
	if(OKGetReturn(rxbuffer) == '0')
	{
        while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,SETMODE1,strlen(SETMODE1),portMAX_DELAY));
        while ( kStatus_LPUART_RxBusy == LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
	}
	LPUART_DRV_GetReceiveStatus(BOARD_BT_UART_INSTANCE,&bytesRemaining);
	memset(rxbuffer,0,100);	
	
	//连接后通知上位机
	while ( kStatus_LPUART_Success != LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,GETNOTI,strlen(GETNOTI), portMAX_DELAY));
  while ( kStatus_LPUART_Timeout !=  LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));  
	if(OKGetReturn(rxbuffer) == '0')
	{
        while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,SETNOTI1,strlen(SETNOTI1),portMAX_DELAY));
        while ( kStatus_LPUART_RxBusy == LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
	}
	LPUART_DRV_GetReceiveStatus(BOARD_BT_UART_INSTANCE,&bytesRemaining);
	memset(rxbuffer,0,100);	
	
		//获取EDR设备名称
	while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,GETNAME,strlen(GETNAME), portMAX_DELAY));
	while ( kStatus_LPUART_RxBusy ==  LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
	
	if(memcmp(&rxbuffer[8],flashdatapackage.IDValue,10) != 0)
	{
		memset(rxbuffer,0,100);
		memcpy(&SETNAME[8],flashdatapackage.IDValue,10);
		uint8_t i =  sizeof(SETNAME);
			while ( kStatus_LPUART_Success != LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,SETNAME,sizeof(SETNAME), portMAX_DELAY));
			while(kStatus_LPUART_Timeout !=LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
	}
	
	memset(rxbuffer,0,100);	
	
	//获取BLE设备名称
	while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,GETNAMB,strlen(GETNAMB), portMAX_DELAY));
	while ( kStatus_LPUART_RxBusy ==  LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
	if(memcmp(&rxbuffer[8],flashdatapackage.IDValue,10) != 0)
	{
		memcpy(&SETNAMB[8],flashdatapackage.IDValue,10);
			while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,SETNAMB,sizeof(SETNAMB), portMAX_DELAY));
			while ( kStatus_LPUART_RxBusy ==  LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
		while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,RESET,strlen(RESET), portMAX_DELAY));
	}
	
	memset(rxbuffer,0,100);	
	
	//LED灯显示状态	
	while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,GETPI10,strlen(GETPI10))); //
	while ( kStatus_LPUART_RxBusy == LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
	if(OKGetReturn(rxbuffer) == '1')
	{
        while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,SETPI100,strlen(SETPI100))); //待机慢闪，连接后常亮
        while ( kStatus_LPUART_RxBusy == LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
	}
	
	memset(rxbuffer,0,100);	
	
//  //获取 EDR MAC  
//	while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,GETADDE,strlen(GETADDE))); 
//	while ( kStatus_LPUART_RxBusy ==  LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
//	memcpy(MACEDR,rxbuffer+7,12);
//	
//	memset(rxbuffer,0,100);	

//	//获取 BLE MAC
//	while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,GETADDB,strlen(GETADDB))); 
//	while ( kStatus_LPUART_RxBusy == LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,rxbuffer,100, 100));
//	memcpy(MACBLE,rxbuffer+7,12);
	
	BTInitFlag = 1;
	 
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
		else if(time == ((HighTime*i)+1))
		{
			LED1_OFF;
		}	
	}
	time++;
	if(time >= PeriodTime)	
	{
		time = 0;
	}			
}


void task_bluetooth_tx(task_param_t param)   //优先级高  
{
	InitBlueTooth();
	
	while(GPIO_DRV_ReadPinInput(kGpioBTPIO1) == 0);
		while ( kStatus_LPUART_Success != LPUART_DRV_SendDataBlocking(BOARD_BT_UART_INSTANCE,AT,sizeof(AT), portMAX_DELAY));


	OSA_TaskCreate(task_bluetooth_rx,
                   (uint8_t*) "bluetooth_rx",
                    TASK_BLUETOOTH_STACK_SIZE,
                    task_bluetooth_rx_stack,
                    TASK_BLUETOOTH_RX_PRIO,
                    (task_param_t)0,
                    false,
                    &task_bluetooth_rx_task_handler);
										
	while(1)
	{
      OSA_MsgQGet(hBTMsgQueue,&m_btdatapackage,portMAX_DELAY); 
				switch(m_btdatapackage.code)
				{
					case ECGDATACODE:
						if(ECGDataSendFlag == 1)
						{
							while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,(uint8_t*)&m_btdatapackage.data,m_btdatapackage.size)); 

						}
						break;
					case  SENDECGPATCHIDCODE:
					case	SENDECGENABLECODE:
					case  SENDECGDISABLECODE:
					case  SENDSET1MVCODE:
					case  SENDSET0MVCODE:
					case  SENDGET1MVCODE:
					case  SENDGET0MVCODE:
					case  SENDHARDVERSIONCODE:
					case  SENDSOFTVERSIONCODE:
					case  SENDSETECGPATCHIDCODE:
					case STARTSENDSET1MVCODE:
					case STARTSENDSET0MVCODE:
						while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,(uint8_t*)&m_btdatapackage.data,m_btdatapackage.size)); 
						break;
				}		
				OSA_TimeDelay(50);
	}
}

void task_bluetooth_rx(task_param_t param)
{
	uint8_t	SucessFlag[1] = {0x01};
	uint8_t bluerxbuffer[20];
	uint32_t bytesRemaining;
    
	while(1)
	{
			OSA_TimeDelay(5);
			if(RxSuccessFlag == 1)
       {
				 RxSuccessFlag = 0;
				 memcpy(bluerxbuffer,g_RxBuffer,20);
				 memset(g_RxBuffer,0,20);
//		    if(kStatus_LPUART_Timeout == LPUART_DRV_ReceiveDataBlocking(BOARD_BT_UART_INSTANCE,bluerxbuffer,20,80))
//				{
/************************************************************************************************/						
					 if(bluerxbuffer[0] == SERIAL_IDENTIFIER)
					 {
						 if(bluerxbuffer[2] == SERIAL_STATUS_OK)
						 {
								if(bluerxbuffer[1] == APP_CMD_RECEIVEECGDATAACK)    //开始发送心电数据
								{
									BlueToothSendCommand(SENDECGENABLECODE,APP_CMD_RECEIVEECGDATAREQ,SERIAL_DATASIZE_ONE,SucessFlag);
									OSA_TimeDelay(5);
									ECGDataSendFlag  = 1;		
									ecgdatapackage.sequence = 0;								
								}
								else if(bluerxbuffer[1] == APP_CMD_STOPRECEIVEECGDATAACK)				//停止发送心电数据
								{
									ECGDataSendFlag  = 0;
									OSA_TimeDelay(1);
									BlueToothSendCommand(SENDECGDISABLECODE,APP_CMD_STOPRECEIVEECGDATAREQ,SERIAL_DATASIZE_NONE,SERIAL_DATAADDR_NONE);
																		
								}
								else if(bluerxbuffer[1] == APP_CMD_ECGPATCHIDACK)				//发送ID值
								{
									BlueToothSendCommand(SENDECGPATCHIDCODE,APP_CMD_ECGPATCHIDREQ,ECGPATCHIDSIZE,flashdatapackage.IDValue); 							
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
									CalibrationFlag = 2;
									flashdatapackage.amplification = bluerxbuffer[4] + (bluerxbuffer[5]<<8);
									WriteData2Flash();
									ReadData4Flash();	
									BlueToothSendCommand(SENDSET1MVCODE,APP_CMD_SET1MVVALUEACK,SERIAL_DATASIZE_ONE,SucessFlag); 
									CalibrationFlag = 0;
								}
								else if(bluerxbuffer[1] == APP_CMD_SET0MVVALUE)				//保存0mv校准值
								{
									CalibrationFlag = 2;
									flashdatapackage.difference_Value = bluerxbuffer[4] + (bluerxbuffer[5]<<8);
									WriteData2Flash();
									ReadData4Flash();	
									BlueToothSendCommand(SENDSET0MVCODE,APP_CMD_SET0MVVALUEACK,SERIAL_DATASIZE_ONE,SucessFlag); 									
									CalibrationFlag = 0;									
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
								else if(bluerxbuffer[1] == APP_CMD_SETECGPATCHIDACK)				//设置ID值
								{
									memcpy(flashdatapackage.IDValue,&bluerxbuffer[4],ECGPATCHIDSIZE);		
									WriteData2Flash();
									ReadData4Flash();
									BlueToothSendCommand(SENDSETECGPATCHIDCODE,APP_CMD_SETECGPATCHIDREQ,SERIAL_DATASIZE_TEN,flashdatapackage.IDValue); 
								}
								else if(bluerxbuffer[1] == APP_CMD_ECGDATAOKREQ)				//心电数据接收回应
								{
									BTSendSuccessFlag = 1;
								}
								else if(bluerxbuffer[1] == APP_CMD_STARTSET1MVVALUE)	
								{
									CalibrationFlag = 1;
									BlueToothSendCommand(STARTSENDSET1MVCODE,APP_CMD_STARTSET1MVVALUEACK,SERIAL_DATASIZE_ONE,SucessFlag); 

								}
								else if(bluerxbuffer[1] == APP_CMD_STARTSET0MVVALUE)	
								{
									CalibrationFlag = 1;
									BlueToothSendCommand(STARTSENDSET0MVCODE,APP_CMD_STARTSET0MVVALUEACK,SERIAL_DATASIZE_ONE,SucessFlag); 

								}
						 }
					 }
/************************************************************************************************/	
        memset(bluerxbuffer,0,20);
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


lpuart_status_t BTlpuart_rx_callback_t(uint8_t * rxByte, void * param)
{
	static uint8_t i= 0;
	static uint8_t m_RxBuffer[20];
	m_RxBuffer[i] = rxByte[0];
	uint8_t DataLength = 0;
	i++;
	if(m_RxBuffer[0] != SERIAL_IDENTIFIER)
	{
		i = 0;
	}
	if(i >= 4)
	{
			if(m_RxBuffer[2] == SERIAL_STATUS_OK)
			{
				DataLength = m_RxBuffer[3];
				if(DataLength > 10)
				{
					i = 0;
				}
			}
			if(i >= (4+DataLength))
			{
				i = 0;
				memcpy(g_RxBuffer,m_RxBuffer,20);
				RxSuccessFlag = 1;
			}
			if(i>20)
			{
				i = 0;
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

		if(BOARD_BT_UART_INSTANCE == uartInstance)
	{
		s_bt_lpuart[BOARD_BT_UART_INSTANCE].rxCallback = BTlpuart_rx_callback_t;
		
	}
	else if(BOARD_DEBUG_UART_INSTANCE == uartInstance)
	{
		s_bt_lpuart[BOARD_DEBUG_UART_INSTANCE].rxCallback = DEBUGlpuart_rx_callback_t;
	}
	
   // s_bt_lpuart.instance = uartInstance;
    return kStatus_LPUART_Success;
}
/* See fsl_debug_console.h for documentation of this function.*/
lpuart_status_t lpuart_DeInit(uint32_t uartInstance)
{
    LPUART_DRV_Deinit(uartInstance);
    return kStatus_LPUART_Success;
}