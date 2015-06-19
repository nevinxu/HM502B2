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


uint16_t Ecgbuffer[256][8];   //8s
uint8_t Ecg_Row = 0;
uint8_t Ecg_Col = 0;
uint8_t BTSendNum = 0;
uint8_t BTSendSuccessFlag = 1;


/* SIM base address */
const uint32_t gSimBaseAddr[] = SIM_BASE_ADDRS;
adc_state_t gAdcState;

extern uint8_t     BLEConnectedFlag;    //BLE连接状态
extern uint8_t     ECGDataSendFlag;    //心电数据发送时能标志

extern msg_queue_handler_t hBTMsgQueue;  //
extern msg_queue_handler_t hPCMsgQueue;  //

BTTransmitPackage 	m_bttransmitpackage;   //蓝牙发送包
static PCTransmitPackage m_pctransmitpackage;

uint8_t BT_RunLedStatus = 0;   //蓝牙指示灯状态
uint8_t BT_LedStatusNum = 0;

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
		static uint8_t i,ii = 128;
		static uint16_t j =0;
		static uint16_t batterybuffer[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		uint16_t buffer = 0;
		adc_chn_config_t adcChnConfig;
		
		static uint16_t batteryValue_Mv;
	
    if(i % 2)  //心电采集
    {
			if(GPIO_DRV_ReadPinInput(kGpioLEADOFF_CHECK) == 0)
			{
				//ecgdatapackage.ecgdata[i/2] = ECGBASEVALUE;
				Ecgbuffer[Ecg_Row][Ecg_Col] = ECGBASEVALUE;
			}
			else
			{
      //  ecgdatapackage.ecgdata[i/2] = ADC_DRV_GetConvValueRAWInt(ECG_INST, ECGCHNGROUP);
				Ecgbuffer[Ecg_Row][Ecg_Col] = ADC_DRV_GetConvValueRAWInt(ECG_INST, ECGCHNGROUP);
			}
			
			Ecg_Col++;
			if(Ecg_Col >= 8)
			{
				Ecg_Col = 0;
				Ecg_Row ++;				
			}
			

			//	ecgdatapackage.ecgdata[i/2] = sinTab[j++];
			//ecgdatapackage.ecgdata[i/2] = j++;
			adcChnConfig.chnNum = BATTERY_ADC_INPUT_CHAN;
			adcChnConfig.diffEnable = false;
			adcChnConfig.intEnable = true;
			adcChnConfig.chnMux = kAdcChnMuxOfB;
			ADC_DRV_ConfigConvChn(0, ECGCHNGROUP, &adcChnConfig);
    }
    else  //电量采集
    {
			ii++;
			if(ii>=128)   //一秒钟一次
			{
				ii = 0;
				for(uint8_t j = 15;j > 0; j--)
				{
					batterybuffer[j] = batterybuffer[j-1];
				}
        batterybuffer[0] = ADC_DRV_GetConvValueRAWInt(ECG_INST, ECGCHNGROUP);
			}
        adcChnConfig.chnNum = ECG_ADC_INPUT_CHAN;
        adcChnConfig.diffEnable = false;
        adcChnConfig.intEnable = true;
        adcChnConfig.chnMux = kAdcChnMuxOfB;
        ADC_DRV_ConfigConvChn(0, ECGCHNGROUP, &adcChnConfig);
    }
		i++;
//		if(i == ECGNUMPACKAGE)
//		{
//			if(BTSendSuccessFlag == 0)
//			{
//						m_bttransmitpackage.code = ECGDATACODE;
//						m_bttransmitpackage.size = sizeof(ecgdatapackage);
//						ecgdatapackage.start = SERIAL_IDENTIFIER;
//						ecgdatapackage.command = APP_CMD_ECGDATASEND;
//						ecgdatapackage.status = SERIAL_STATUS_OK;
//						ecgdatapackage.length = ECGDATASIZE;
//						memcpy(m_bttransmitpackage.data,&ecgdatapackage,sizeof(ecgdatapackage));
//					//	memcpy(m_bttransmitpackage.data,Ecgbuffer[BTSendNum],sizeof(ecgdatapackage));

//						
//				//压缩		
//						EncodeData4WTo5B(ecgdatapackage.ecgdata,&m_bttransmitpackage.data[6],8);
//						m_bttransmitpackage.size = m_bttransmitpackage.size-6;
//						
//						OSA_MsgQPut(hBTMsgQueue,&m_bttransmitpackage);  
//			}
//		}

		if(i >= (ECGNUMPACKAGE<<1))
		{
			i = 0;
			BT_RunLedStatus = GPIO_DRV_ReadPinInput(kGpioBTPIO1);
			if(BT_RunLedStatus == 0)  //  灭  代表有闪烁   蓝牙未连接
			{
				ECGDataSendFlag = 0;
				BLEConnectedFlag = 0;
				BT_LedStatusNum = 0;
				LED1_OFF;					//心电运行指示灯灭 
			}
			else
			{
				BT_LedStatusNum++;
				if(BT_LedStatusNum >=10)
				{
					BLEConnectedFlag = 1;   //长时间亮   代表蓝牙已连接 
					if(ECGDataSendFlag == 0)   //未发送数据  
					{
					//	ECGDataSendFlag = 1;
						LED1_OFF;	   //
					}
				}
			}
			ecgdatapackage.leadoffstatus_battery = 0;
			
				if(GPIO_DRV_ReadPinInput(kGpioLEADOFF_CHECK))
				{
					ecgdatapackage.leadoffstatus_battery |= 0x80; 
				}
				else
				{
					ecgdatapackage.leadoffstatus_battery &= 0x7F; 
				}
				ecgdatapackage.sequence++;
				if(batterybuffer[15]>0)
				{
					for(uint8_t j = 0;j < 16; j++)
					{
						buffer += batterybuffer[j];
					}
					buffer = (buffer>>4);
				}
				else
				{
					buffer = batterybuffer[0];
				}
				
				
				
				if(buffer <= BATTERYTOOLOW)   //电压过低  
				{
					ecgdatapackage.leadoffstatus_battery = 0;   //导联脱离  数据发0  
					memset(ecgdatapackage.ecgdata,0,8);
					LedSet(3,2,25);
				}
				if(buffer <= BATTERYLOW)   //电压低  
				{ 
					LedSet(3,2,25);
				}
				
				if(buffer >318)
				{
					ecgdatapackage.leadoffstatus_battery += 100; 
				}
				else if(buffer <256)
				{
					ecgdatapackage.leadoffstatus_battery += 0;
				}
				else
				{
					ecgdatapackage.leadoffstatus_battery += 1000*(buffer-256)/620;
				}
				
				batteryValue_Mv = 3300 *4 * buffer /1024 ;
				
			if(ECGDataSendFlag  == 1)
			{
//				if(BTSendSuccessFlag == 1)
				{
						BTSendSuccessFlag = 0;
						m_bttransmitpackage.code = ECGDATACODE;
						m_bttransmitpackage.size = sizeof(ecgdatapackage);
						ecgdatapackage.start = SERIAL_IDENTIFIER;
						ecgdatapackage.command = APP_CMD_ECGDATASEND;
						ecgdatapackage.status = SERIAL_STATUS_OK;
						ecgdatapackage.length = ECGDATASIZE;
						memcpy(ecgdatapackage.ecgdata,Ecgbuffer[BTSendNum++],16);
						memcpy(m_bttransmitpackage.data,&ecgdatapackage,sizeof(ecgdatapackage));

						
				//压缩		
						EncodeData4WTo5B(ecgdatapackage.ecgdata,&m_bttransmitpackage.data[6],8);
						m_bttransmitpackage.size = m_bttransmitpackage.size-6;
						
						OSA_MsgQPut(hBTMsgQueue,&m_bttransmitpackage);   


						m_pctransmitpackage.start = STARTHEAD;
						m_pctransmitpackage.command = ECGDATACODE;
						m_pctransmitpackage.size = sizeof(ecgdatapackage) + 3;
						memcpy(m_pctransmitpackage.data,&ecgdatapackage,sizeof(ecgdatapackage));
						OSA_MsgQPut(hPCMsgQueue,&m_pctransmitpackage);  
				}
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

