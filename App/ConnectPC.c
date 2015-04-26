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

MSG_QUEUE_DECLARE(mqpcData, 10, 1);  //��С��freertos����Ч   

extern msg_queue_handler_t hPCEcgMsgQueue;  //�ĵ����ݷ��Ͷ��� 



struct EcgDataPackage m_ecgdatapackage;   //�ĵ����ݽṹ

struct EcgDataTxPackage mEcgDataTxPackage;
    
lpuart_state_t s_pc_bt_lpuart;    

extern uint8_t     BLEConnectedFlag;

extern unsigned char MACEDR[12];
extern unsigned char MACBLE[12];
    
		
int EncodeData4WTo5B(uint16_t* pData,uint8_t* rtnData,int Count)
{
	uint16_t k = 0;
	uint8_t B = 0;
	uint16_t W = 0;
	for(int i=0;i<Count;i+=4)
	{
		B = 0;
		W = 0;
		for(int j=0;j<4;j++)
		{
		W = pData[i+j];
		// if(W>1000)
		// W= 1000;
		rtnData[k] = (uint8_t)W;
		B = B | ((W&0x0300)>>8)<<(6-j*2);//???????2?,??4?,???????
		k++;
		}
		rtnData[k] = B;//(BYTE)W;
		k++;
	}		
	return k;
}
		
void task_connnectpc_tx(task_param_t param)
{
	uint8_t i;
	while(1)
	{
		OSA_MsgQGet(hPCEcgMsgQueue,&m_ecgdatapackage,portMAX_DELAY);
        {
            i++;
            if(BLEConnectedFlag == 1)
            {
							mEcgDataTxPackage.start = 0x80;
							mEcgDataTxPackage.sequence = i&0x7f;
							
							if(m_ecgdatapackage.battery >= 317)
							{
								m_ecgdatapackage.battery = 100;
							}
							else if(m_ecgdatapackage.battery > 255)
							{
								m_ecgdatapackage.battery = 100 - 100/(m_ecgdatapackage.battery - 255);
							}
							else
							{
								m_ecgdatapackage.battery = 0;
							}
							mEcgDataTxPackage.battery = m_ecgdatapackage.battery&0x7f;
							mEcgDataTxPackage.leadoffstatus = m_ecgdatapackage.leadoffstatus&0x7f;
							
							EncodeData4WTo5B(m_ecgdatapackage.ecgdata,mEcgDataTxPackage.data,8);
							
              while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_DEBUG_UART_INSTANCE,(uint8_t*)&mEcgDataTxPackage,sizeof(mEcgDataTxPackage)));            
              while ( kStatus_LPUART_TxBusy ==  LPUART_DRV_SendData(BOARD_BT_UART_INSTANCE,(uint8_t*)&mEcgDataTxPackage,sizeof(mEcgDataTxPackage))); 
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
	}
}

void task_connnectpc_rx(task_param_t param)
{
    uint8_t pcrxbuffer[100];
   // hPCMsgQueue = OSA_MsgQCreate(mqpcData, 1, sizeof(uint8_t));  //
	while(1)
	{
		if(kStatus_LPUART_Timeout == LPUART_DRV_ReceiveDataBlocking(BOARD_DEBUG_UART_INSTANCE,pcrxbuffer,100,100))
        {
            if(pcrxbuffer[0] == 0x49 && pcrxbuffer[1] == 0x37)   //���ݰ�ͷ
            {
                if(pcrxbuffer[4] == STARTECGCAPTUREREQ)
                {
                    BLEConnectedFlag = 1;
                }
                else if(pcrxbuffer[4] == STOPECGCAPTUREREQ)
                {
                    BLEConnectedFlag = 0;
                }
                else if(pcrxbuffer[4] == GETBLEMACACK)
                {
                    BLEConnectedFlag = 0;
                    SendCommand2PC(GETBLEMACACK,COMMANDSUCCESS,MACBLE,12);
                }
                else if(pcrxbuffer[4] == GETEDRMACREQ)
                {
                    BLEConnectedFlag = 0;
                    SendCommand2PC(GETBLEMACACK,COMMANDSUCCESS,MACEDR,12);
                }
                pcrxbuffer[0] = 0;
            }
        }

        
	}
}

void SendCommand2PC(uint8_t command,uint8_t status, uint8_t *data,uint8_t datalength)
{
    static uint8_t pctxbuffer[100];
    static uint16_t sequence;
    sequence++;
    uint16_t checksum = 0;
    
    pctxbuffer[0] = 0x49;
    pctxbuffer[1] = 0x37;
    pctxbuffer[2] = sequence;
    pctxbuffer[3] = (sequence>>8);
    pctxbuffer[4] = command;
    pctxbuffer[5] = status;
    pctxbuffer[6] = datalength;
    pctxbuffer[7] = (datalength>>8);
    if(datalength > 0)
    {
        memcpy(pctxbuffer+8,data,datalength);
    }
    for(uint16_t i = 0;i <=(7+datalength);i++)
    {
        checksum += pctxbuffer[i];
    }
    pctxbuffer[8+datalength] = checksum;
    pctxbuffer[9+datalength] = (checksum>>8);
     
    LPUART_DRV_SendData(BOARD_DEBUG_UART_INSTANCE,pctxbuffer,10+datalength); 
    
}

