#ifndef _ECG_CAPTURE_
#define _ECG_CAPTURE_

#include "stdlib.h"
#include "stdio.h"
#include "fsl_os_abstraction.h"

#include "device/fsl_device_registers.h"
#include "fsl_sim_hal.h"

#define ECG_INST          		0U   /*!< ADC instance */
#define BATTERY_INST          0U   /*!< ADC instance */
#define ECGCHNGROUP		        1u
#define BATTERYCHNGROUP		    0u


#define ECG_ADC_INPUT_CHAN          13U  /* default input signal channel */
#define BATTERY_ADC_INPUT_CHAN      23U  /* default input signal channel */

#define ECGNUMPACKAGE     									8

#define ECGPACKAGEDEEP											5   //心电数据队列保存深度   

#define BATTERYLOW							256

typedef struct _EcgDataPackage
{
	uint8_t	start;
	uint8_t	command;
	uint8_t	status;
	uint8_t	length;
	uint8_t sequence;
	uint8_t leadoffstatus;
	uint16_t battery;
	uint16_t ecgdata[ECGNUMPACKAGE];
}EcgDataPackage;

typedef struct _FlashDataPackage
{
	short amplification;
	short difference_Value;
	uint8_t IDValue[10]; 
	uint8_t None[2]; 
}FlashDataPackage;    //

void ecgcaptureenable(uint32_t instance);
extern int32_t init_ecg(uint32_t instance);

extern void init_trigger_source(uint32_t instance);
extern void deinit_trigger_source(uint32_t instance);


#endif
