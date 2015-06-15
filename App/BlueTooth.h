#ifndef _BLUETOOTH_
#define _BLUETOOTH_
#include "fsl_os_abstraction.h"

#include "device/fsl_device_registers.h"
#include "fsl_sim_hal.h"

#define TASK_BLUETOOTH_STACK_SIZE    					512
#define TASK_BLUETOOTH_TX_PRIO          			10
#define TASK_BLUETOOTH_RX_PRIO          			11
#define BLUEDATEPACKETSIZE										20
#define BTPACKAGEDEEP													10   //BLUE数据队列保存深度  


#define ECGDATACODE												0x01      //心电数据发送
#define SENDECGDISABLECODE								0x04			//停止发送心电数据
#define SENDECGENABLECODE									0x02			//使能发送心电数据
#define SENDECGPATCHIDCODE								0x03			//发送心电补丁ID
#define SENDHARDVERSIONCODE								0x05			//硬件版本
#define SENDSOFTVERSIONCODE								0x06			//软件版本
#define SENDSET1MVCODE										0x07			//设置1mv定标
#define SENDSET0MVCODE										0x08			//设置0mv校准
#define SENDGET1MVCODE										0x09			//获取1mv定标
#define SENDGET0MVCODE										0x0A			//获取0mv校准
#define SENDSETECGPATCHIDCODE								0x0B			//设置心电补丁ID回应


#define SERIAL_IDENTIFIER       									0x77

#define SERIAL_STATUS_OK       										0x00
#define SERIAL_STATUS_FAIL       									0x01

#define SERIAL_DATASIZE_NONE       								0x00
#define SERIAL_DATASIZE_ONE       								0x01
#define SERIAL_DATASIZE_TWO       								0x02
#define SERIAL_DATASIZE_TEN       								0x0A

#define SERIAL_DATAADDR_NONE       								0x00

#define ECGPATCHIDSIZE														10
#define ECGPATCHHARDVERSIONSIZE										4
#define ECGPATCHSOFTVERSIONSIZE										4
#define ECGDATASIZE																12

#define APP_CMD_ECGDATASEND                				0x09
#define APP_CMD_RECEIVEECGDATAACK                	0x14
#define APP_CMD_RECEIVEECGDATAREQ                	0x15
#define APP_CMD_STOPRECEIVEECGDATAACK            	0x16
#define APP_CMD_STOPRECEIVEECGDATAREQ           	0x17
#define APP_CMD_ECGPATCHIDACK                    	0x18
#define APP_CMD_ECGPATCHIDREQ                			0x19

#define APP_CMD_SETECGPATCHIDACK                    	0x34
#define APP_CMD_SETECGPATCHIDREQ                			0x35

#define APP_CMD_ECGPATCHHARDVERSIONACK          	0x30
#define APP_CMD_ECGPATCHHARDVERSIONREQ        		0x31
#define APP_CMD_ECGPATCHSOFTVERSIONACK          	0x32
#define APP_CMD_ECGPATCHSOFTVERSIONREQ        		0x33

#define APP_CMD_SET1MVVALUE               				0x28
#define APP_CMD_SET1MVVALUEACK               			0x29
#define APP_CMD_SET0MVVALUE               				0x2C
#define APP_CMD_SET0MVVALUEACK               			0x2D

#define APP_CMD_GET1MVVALUE               				0x26
#define APP_CMD_GET1MVVALUEACK               			0x27
#define APP_CMD_GET0MVVALUE               				0x2A
#define APP_CMD_GET0MVVALUEACK               			0x2B


                     
#define     GETATOB                     "AT+ATOB?"                      //查询/设置模块三通模式
#define     SETATOB0                    "AT+ATOB0" 
#define     SETATOB1                    "AT+ATOB1" 

#define     GETAUTH                     "AT+AUTH?"                     //查询/设置模块 EDR 鉴权模式
#define     SETAUTH0                    "AT+AUTH0"
#define     SETAUTH1                    "AT+AUTH1"

#define     GETADDE                     "AT+ADDE?"                       //查询模块 EDR 模式  MAC 地址
#define     GETADDB                     "AT+ADDB?"                       //查询模块 BLE 模式  MAC 地址

#define     GETBAUD                     "AT+BAUD?"                      //查询、设置模块波特率
#define     SETBAUD9600                 "AT+BAUD2"
#define     SETBAUD115200               "AT+BAUD6"
#define     SETBAUD230400               "AT+BAUD7"


#define     SETBONDE                    "AT+BONDE"                     //清除配对信息
#define     SETBONDB                    "AT+BONDB"


#define     SETCLEAE                    "AT+CLEAE"                      //清除最后成功连接过的地址信息
#define     SETCLEAB                    "AT+CLEAB"


#define     GETDUAL                     "AT+DUAL?"                      //设置模块连接模式
#define     SETDUAL0                    "AT+DUAL0"
#define     SETDUAL1                    "AT+DUAL1"


#define     GETHIGH                     "AT+HIGH?" 
#define     SETHIGH0                    "AT+HIGH0"
#define     SETHIGH1                    "AT+HIGH1"

#define     GETHELP                     "AT+HELP?" 


#define     GETINIT                     "AT+INIT?" 
#define     SETINIT0                    "AT+INIT0" 
#define     SETINIT1                    "AT+INIT1" 

#define     GETIMME                     "AT+IMME?" 
#define     SETIMME0                    "AT+IMME0"
#define     SETIMME1                    "AT+IMME1"

#define     GETIMMB                     "AT+IMMB?" 
#define     SETIMMB0                    "AT+IMMB0"
#define     SETIMMB1                    "AT+IMMB1"

#define     GETMODE                     "AT+MODE?"                  //设置模块工作模式
#define     SETMODE0                    "AT+MODE0" 
#define     SETMODE1                    "AT+MODE1" 

#define     GETNOTI                     "AT+NOTI?"                  //设置是否通知上位机连接状态
#define     SETNOTI0                    "AT+NOTI0" 
#define     SETNOTI1                    "AT+NOTI1" 

#define     GETNOTP                     "AT+NOTP?"                  //设置通知上位机连接状态的提示类型
#define     SETNOTP0                    "AT+NOTP0" 
#define     SETNOTP1                    "AT+NOTP1" 

#define     GETNAME                     "AT+NAME?"                           //查询、设置 EDR 设备名称
#define     GETNAMB                     "AT+NAMB?"                           //查询、设置 BLE 设备名称
//#define     SETNAME                     "AT+NAMELJS_EDR"
//#define     SETNAMB                     "AT+NAMBLJS_BLE"


#define     GETPIO0                     "AT+PIO0?"                  //PIO0 按键管脚功能设置
#define     SETPIO00                     "AT+PIO00"
#define     SETPIO01                     "AT+PIO01"

#define     GETPI10                     "AT+PIO1?"                  //PIO1 口输出状态
#define     SETPI100                     "AT+PIO10"
#define     SETPI101                     "AT+PIO11"

#define     GETPINE                     "AT+PINE?"                      //查询、设置 EDR 配对密码
#define     SETPINE                     "AT+PINE000000"                 //查询、设置 EDR 配对密码

#define     GETPINB                     "AT+PINB?"
#define     SETPINB                     "AT+PINB000000"

#define     SETRENEW                    "AT+RENEW"                     //恢复默认设置(Renew) 
#define     SETRESET                    "AT+RESET"                     // 模块复位，重启

#define     GETROLB                     "AT+ROLB?"                      //查询、设置 BLE 主从模式
#define     SETROLB0                    "AT+ROLB0"
#define     SETROLB1                    "AT+ROLB1"

#define     GETRSSE                     "AT+RSSE?"                      //查询 EDR 连接 RSSI 值
#define     GETRSSB                     "AT+RSSB?"                      //查询 BLE 连接 RSSI 值

#define     GETRADE                     "AT+RADE?"                      //查询 EDR 成功连接过的远程主机地址
#define     GETRADB                     "AT+RADB?"                      //查询 EDR 成功连接过的远程主机地址

#define     SETSTARE                    "AT+STARE"                      //模块 EDR 模式开始工作指令
#define     SETSTARB                    "AT+STARB"                      //模块 BLE 模式开始工作指令

#define     GETSCAN                     "AT+SCAN?"                      //查询/设置模块 EDR 模式待机参数
#define     SETSCAN0                    "AT+SCAN0"
#define     SETSCAN1                    "AT+SCAN1"

#define     GETVERS                     "AT+VERS?"                      //查询软件本


typedef struct _BTTransmitPackage
{
	uint8_t	code;
	uint8_t size;
	uint8_t	data[BLUEDATEPACKETSIZE];
}BTTransmitPackage;
    
extern void task_bluetooth_tx(task_param_t param);
extern void task_bluetooth_rx(task_param_t param);
extern void BlueToothSendCommand(uint8_t code,uint8_t command,uint8_t DataSize,uint8_t *Data);

extern lpuart_status_t lpuart_Init(
        uint32_t uartInstance, uint32_t baudRate);
extern lpuart_status_t lpuart_DeInit(uint32_t uartInstance);

extern void LedSet(uint8_t HighTime,uint8_t HighNum,uint16_t PeriodTime);
#endif
