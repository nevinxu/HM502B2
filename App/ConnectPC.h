#ifndef _CONNECTPC_
#define _CONNECTPC_
#include "fsl_os_abstraction.h"

#include "device/fsl_device_registers.h"
#include "fsl_sim_hal.h"

#define TASK_CONNECT_PC_STACK_SIZE    512
#define TASK_CONNECT_PC_PRIO          9

#define PCFIFODEEP              100
#define MINSIZEPCPACKAGE          8

#define PCPACKAGEDEEP											1
#define PCDATEPACKETSIZE									40

typedef struct _PCTransmitPackage
{
    uint16_t    start;
    uint8_t     command;
    uint8_t    	size;
		uint8_t			data[PCDATEPACKETSIZE];
}PCTransmitPackage;

struct EcgDataTxPackage
{
	uint8_t	start;
	uint8_t sequence;
	uint8_t leadoffstatus;
	uint8_t battery;
	uint8_t data[10];

};


#define     STARTHEAD               0x3749


#define     STARTECGCAPTUREREQ      0x01
#define     STARTECGCAPTUREACK      0x02
#define     GETEDRMACREQ            0x03
#define     GETBLEMACACK            0x04
#define     STOPECGCAPTUREREQ       0x05
#define     STOPECGCAPTUREACK       0x06


#define     COMMANDSUCCESS          0x00

extern void task_connnectpc_rx(task_param_t param);
extern void task_connnectpc_tx(task_param_t param);

extern lpuart_status_t PC_lpuart_Init(
        uint32_t uartInstance, uint32_t baudRate);
extern lpuart_status_t PC_lpuart_DeInit(uint32_t uartInstance);

extern void SendCommand2PC(uint8_t command,uint8_t status, uint8_t *data,uint8_t datalength);
#endif
