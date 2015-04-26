#ifndef _BLUETOOTH_
#define _BLUETOOTH_
#include "fsl_os_abstraction.h"

#include "device/fsl_device_registers.h"
#include "fsl_sim_hal.h"

#define TASK_BLUETOOTH_STACK_SIZE    512
#define TASK_BLUETOOTH_TX_PRIO          10
#define TASK_BLUETOOTH_RX_PRIO          11

#define     AT                          "AT"
#define     GETATOB                     "AT+ATOB?"                      //��ѯ/����ģ����ͨģʽ
#define     SETATOB0                    "AT+ATOB0" 
#define     SETATOB1                    "AT+ATOB1" 

#define     GETAUTH                     "AT+AUTH?"                     //��ѯ/����ģ�� EDR ��Ȩģʽ
#define     SETAUTH0                    "AT+AUTH0"
#define     SETAUTH1                    "AT+AUTH1"

#define     GETADDE                     "AT+ADDE?"                       //��ѯģ�� EDR ģʽ  MAC ��ַ
#define     GETADDB                     "AT+ADDB?"                       //��ѯģ�� BLE ģʽ  MAC ��ַ

#define     GETBAUD                     "AT+BAUD?"                      //��ѯ������ģ�鲨����
#define     SETBAUD9600                 "AT+BAUD2"
#define     SETBAUD115200               "AT+BAUD6"
#define     SETBAUD230400               "AT+BAUD7"


#define     SETBONDE                    "AT+BONDE"                     //��������Ϣ
#define     SETBONDB                    "AT+BONDB"


#define     SETCLEAE                    "AT+CLEAE"                      //������ɹ����ӹ��ĵ�ַ��Ϣ
#define     SETCLEAB                    "AT+CLEAB"


#define     GETDUAL                     "AT+DUAL?"                      //����ģ������ģʽ
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

#define     GETMODE                     "AT+MODE?"                  //����ģ�鹤��ģʽ
#define     SETMODE0                    "AT+MODE0" 
#define     SETMODE1                    "AT+MODE1" 

#define     GETNOTI                     "AT+NOTI?"                  //�����Ƿ�֪ͨ��λ������״̬
#define     SETNOTI0                    "AT+NOTI0" 
#define     SETNOTI1                    "AT+NOTI1" 

#define     GETNOTP                     "AT+NOTP?"                  //����֪ͨ��λ������״̬����ʾ����
#define     SETNOTP0                    "AT+NOTP0" 
#define     SETNOTP1                    "AT+NOTP1" 

#define     GETNAME                     "AT+NAME?"                           //��ѯ������ EDR �豸����
#define     GETNAMB                     "AT+NAMB?"                           //��ѯ������ BLE �豸����
#define     SETNAME                     "AT+NAMEHM502B2"
#define     SETNAMB                     "AT+NAMBHM502B2"

#define     GETPIO0                     "AT+PIO0?"                  //PIO0 �����ܽŹ�������
#define     SETPIO00                     "AT+PIO00"
#define     SETPIO01                     "AT+PIO01"

#define     GETPI10                     "AT+PIO1?"                  //PIO1 �����״̬
#define     SETPI100                     "AT+PIO10"
#define     SETPI101                     "AT+PIO11"

#define     GETPINE                     "AT+PINE?"                      //��ѯ������ EDR �������
#define     SETPINE                     "AT+PINE000000"                 //��ѯ������ EDR �������

#define     GETPINB                     "AT+PINB?"
#define     SETPINB                     "AT+PINB000000"

#define     SETRENEW                    "AT+RENEW"                     //�ָ�Ĭ������(Renew) 
#define     SETRESET                    "AT+RESET"                     // ģ�鸴λ������

#define     GETROLB                     "AT+ROLB?"                      //��ѯ������ BLE ����ģʽ
#define     SETROLB0                    "AT+ROLB0"
#define     SETROLB1                    "AT+ROLB1"

#define     GETRSSE                     "AT+RSSE?"                      //��ѯ EDR ���� RSSI ֵ
#define     GETRSSB                     "AT+RSSB?"                      //��ѯ BLE ���� RSSI ֵ

#define     GETRADE                     "AT+RADE?"                      //��ѯ EDR �ɹ����ӹ���Զ��������ַ
#define     GETRADB                     "AT+RADB?"                      //��ѯ EDR �ɹ����ӹ���Զ��������ַ

#define     SETSTARE                    "AT+STARE"                      //ģ�� EDR ģʽ��ʼ����ָ��
#define     SETSTARB                    "AT+STARB"                      //ģ�� BLE ģʽ��ʼ����ָ��

#define     GETSCAN                     "AT+SCAN?"                      //��ѯ/����ģ�� EDR ģʽ��������
#define     SETSCAN0                    "AT+SCAN0"
#define     SETSCAN1                    "AT+SCAN1"

#define     GETVERS                     "AT+VERS?"                      //��ѯ�����
    
extern void task_bluetooth_tx(task_param_t param);
extern void task_bluetooth_rx(task_param_t param);

extern lpuart_status_t lpuart_Init(
        uint32_t uartInstance, uint32_t baudRate);
extern lpuart_status_t lpuart_DeInit(uint32_t uartInstance);
#endif
