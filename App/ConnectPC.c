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

extern msg_queue_handler_t hPCMsgQueue;;  //心电数据发送队列 

PCTransmitPackage m_pctransmitpackage;
  
extern uint8_t     BLEConnectedFlag;

extern unsigned char MACEDR[12];
extern unsigned char MACBLE[12];
    
void task_connnectpc_tx(task_param_t param)
{

	while(1)
	{
		OSA_MsgQGet(hPCMsgQueue,&m_pctransmitpackage,portMAX_DELAY);
        {
							
              while ( kStatus_LPUART_TxBusy == LPUART_DRV_SendData(BOARD_DEBUG_UART_INSTANCE,(uint8_t*)&m_pctransmitpackage,m_pctransmitpackage.size));            
        }
	}
}

void task_connnectpc_rx(task_param_t param)
{
	uint8_t pcrxbuffer[100];
	while(1)
	{
		
		if(kStatus_LPUART_Timeout == LPUART_DRV_ReceiveDataBlocking(BOARD_DEBUG_UART_INSTANCE,pcrxbuffer,100,10))
        {
            if(pcrxbuffer[0] == 0x49 && pcrxbuffer[1] == 0x37)   //数据包头
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

lpuart_status_t DEBUGlpuart_rx_callback_t(uint8_t * rxByte, void * param)
{

}