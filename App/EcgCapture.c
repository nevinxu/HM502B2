#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "fsl_adc_driver.h"

#include "fsl_lpuart_hal.h"
#include "fsl_lpuart_common.h"
#include "fsl_lpuart_driver.h"

#include "board.h"

#include "EcgCapture.h"
#include "gpio_pins.h"
#include "BlueTooth.h"
#include "ConnectPC.h"
#include "ecg.c"

//�ĵ粹��IDֵ
uint8_t ECGPatchID[15] = {0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30};

/* SIM base address */
const uint32_t gSimBaseAddr[] = SIM_BASE_ADDRS;
adc_state_t gAdcState;

extern uint8_t     BLEConnectedFlag;    //BLE����״̬
extern uint8_t     ECGDataSendFlag;    //�ĵ����ݷ���ʱ�ܱ�־

extern msg_queue_handler_t hBTMsgQueue;  //
extern msg_queue_handler_t hPCMsgQueue;  //

BTTransmitPackage 	m_bttransmitpackage;
uint8_t BT_Status = 0;
uint8_t BT_StatusNum = 0;
EcgDataPackage ecgdatapackage;   //

FlashDataPackage flashdatapackage;
	
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
	
static void ecg_adc_isr_callback(void)
{
		static uint8_t i,j;
		static uint16_t batterybuffer[16];
		uint16_t buffer = 0;
		adc_chn_config_t adcChnConfig;
	
    if(i % 2)
    {
			if(GPIO_DRV_ReadPinInput(kGpioLEADOFF_CHECK) == 0)
			{
				ecgdatapackage.ecgdata[i/2] = 500;
			}
			else
			{
        ecgdatapackage.ecgdata[i/2] = ADC_DRV_GetConvValueRAWInt(ECG_INST, ECGCHNGROUP);
			}

		//		ecgdatapackage.ecgdata[i/2] = sinTab[j++];
		//	ecgdatapackage.ecgdata[i/2] = 511;
        adcChnConfig.chnNum = BATTERY_ADC_INPUT_CHAN;
        adcChnConfig.diffEnable = false;
        adcChnConfig.intEnable = true;
        adcChnConfig.chnMux = kAdcChnMuxOfB;
        ADC_DRV_ConfigConvChn(0, ECGCHNGROUP, &adcChnConfig);
    }
    else
    {
				for(uint8_t j = 15;j > 0; j--)
				{
					batterybuffer[j] = batterybuffer[j-1];
				}
        batterybuffer[0] = ADC_DRV_GetConvValueRAWInt(ECG_INST, ECGCHNGROUP);

        adcChnConfig.chnNum = ECG_ADC_INPUT_CHAN;
        adcChnConfig.diffEnable = false;
        adcChnConfig.intEnable = true;
        adcChnConfig.chnMux = kAdcChnMuxOfB;
        ADC_DRV_ConfigConvChn(0, ECGCHNGROUP, &adcChnConfig);
    }
		i++;
		if(i >= (ECGNUMPACKAGE<<1))
		{
			i = 0;
			BT_Status = GPIO_DRV_ReadPinInput(kGpioBTPIO1);
			if(BT_Status == 0)
			{
				ECGDataSendFlag = 0;
				BLEConnectedFlag = 0;
				BT_StatusNum = 0;
				LED1_OFF;
			}
			else
			{
				BT_StatusNum++;
				if(BT_StatusNum >=10)
				{
					BLEConnectedFlag = 1;
				}
			}
				ecgdatapackage.leadoffstatus = GPIO_DRV_ReadPinInput(kGpioLEADOFF_CHECK);
				ecgdatapackage.sequence++;
				ecgdatapackage.battery = 0;
				if(batterybuffer[15]>0)
				{
					for(uint8_t j = 0;j < 16; j++)
					{
						buffer += batterybuffer[j];
					}
					ecgdatapackage.battery = (buffer>>4);
				}
				else
				{
					ecgdatapackage.battery = batterybuffer[0];
				}
				if(ecgdatapackage.battery <= BATTERYLOW)
				{
					ECGDataSendFlag = 0;
					LedSet(2,2,25);
				}
			if(ECGDataSendFlag  == 1)
			{
				m_bttransmitpackage.code = ECGDATACODE;
				m_bttransmitpackage.size = sizeof(ecgdatapackage);
				ecgdatapackage.start = SERIAL_IDENTIFIER;
				ecgdatapackage.command = APP_CMD_ECGDATASEND;
				ecgdatapackage.status = SERIAL_STATUS_OK;
				ecgdatapackage.length = ECGDATASIZE;
				memcpy(m_bttransmitpackage.data,&ecgdatapackage,sizeof(ecgdatapackage));

				
		//ѹ��		
				EncodeData4WTo5B(ecgdatapackage.ecgdata,&m_bttransmitpackage.data[8],8);
				m_bttransmitpackage.size = m_bttransmitpackage.size-6;
				
				OSA_MsgQPut(hBTMsgQueue,&m_bttransmitpackage);   


//				pctransmitpackage.start = STARTHEAD;
//				pctransmitpackage.command = ECGDATACODE;
//				pctransmitpackage.size = sizeof(ecgdatapackage) + 3;
//				memcpy(pctransmitpackage.data,&ecgdatapackage,sizeof(ecgdatapackage));
//				OSA_MsgQPut(hPCMsgQueue,&pctransmitpackage);  
			}
		}
}

void ecgcaptureenable(uint32_t instance)
{
    adc_chn_config_t ecgadcChnConfig;
    ecgadcChnConfig.chnNum = ECG_ADC_INPUT_CHAN;
    ecgadcChnConfig.diffEnable = false;
    ecgadcChnConfig.intEnable = true;
    ecgadcChnConfig.chnMux = kAdcChnMuxOfB;
   
    /* Configure channel0. */
    ADC_DRV_ConfigConvChn(instance, ECGCHNGROUP, &ecgadcChnConfig);	
}


int32_t init_ecg(uint32_t instance)
{
#if FSL_FEATURE_ADC_HAS_CALIBRATION
    adc_calibration_param_t ecgadcCalibraitionParam;
#endif
    adc_user_config_t ecgadcUserConfig;
    adc_chn_config_t ecgadcChnConfig;

#if FSL_FEATURE_ADC_HAS_CALIBRATION
    /* Auto calibraion. */     
    ADC_DRV_GetAutoCalibrationParam(instance, &ecgadcCalibraitionParam);
    ADC_DRV_SetCalibrationParam(instance, &ecgadcCalibraitionParam);
#endif

    /*
     * Initialization ADC for
     * 12bit resolution, interrrupt mode, hw trigger enabled.
     * normal convert speed, VREFH/L as reference, 
     * disable continuouse convert mode.
     */ 
    ADC_DRV_StructInitUserConfigForIntMode(&ecgadcUserConfig);
    ecgadcUserConfig.hwTriggerEnable = true;
    ecgadcUserConfig.continuousConvEnable = false;
    ecgadcUserConfig.resolutionMode = kAdcResolutionBitOfSingleEndAs10;
	//	adcUserConfig.refVoltSrcMode = kAdcRefVoltSrcOfValt;
    ADC_DRV_Init(instance, &ecgadcUserConfig, &gAdcState);
    
    ADC_DRV_EnableHwAverage(instance,kAdcHwAverageCountOf32);

    /* Install Callback function into ISR. */
    ADC_DRV_InstallCallback(instance, ECGCHNGROUP, ecg_adc_isr_callback);

    ecgadcChnConfig.chnNum = ECG_ADC_INPUT_CHAN;
    ecgadcChnConfig.diffEnable = false;
    ecgadcChnConfig.intEnable = true;
    ecgadcChnConfig.chnMux = kAdcChnMuxOfB;
    ADC_DRV_ConfigConvChn(instance, ECGCHNGROUP, &ecgadcChnConfig);	

    return 0;
}

