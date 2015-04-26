#include "stdlib.h"
#include "stdio.h"
#include "fsl_adc_driver.h"

#include "EcgCapture.h"
#include "gpio_pins.h"


extern void init_trigger_source(uint32_t instance);
extern void deinit_trigger_source(uint32_t instance);

/* SIM base address */
const uint32_t gSimBaseAddr[] = SIM_BASE_ADDRS;

adc_state_t gAdcState;

struct EcgDataPackage ecgdatapackage;   //心电数据结构

msg_queue_handler_t hPCEcgMsgQueue;  //心电数据发送队列  

MSG_QUEUE_DECLARE(mqPCEcgData, 10, 1);  //大小在freertos上无效  

/*!
 * @brief ADC channel0 callback for fetching sample data.
 */
 

	
	
static void ecg_adc_isr_callback(void)
{
	static uint8_t i;
	static uint16_t batterybuffer[16];
	uint16_t buffer = 0;
    
    adc_chn_config_t adcChnConfig;
    if(i % 2)
    {
        ecgdatapackage.ecgdata[i/2] = ADC_DRV_GetConvValueRAWInt(ECG_INST, ECGCHNGROUP);
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
		ecgdatapackage.leadoffstatus = GPIO_DRV_ReadPinInput(kGpioLEADOFF_CHECK);
		ecgdatapackage.sequence++;
		ecgdatapackage.battery = 0;
		for(uint8_t j = 0;j < 16; j++)
		{
			buffer += batterybuffer[j];
		}
		ecgdatapackage.battery = (buffer>>4);
        
		OSA_MsgQPut(hPCEcgMsgQueue,&ecgdatapackage);
          
	}
}

static void battery_adc_isr_callback(void)
{
	static uint8_t i;
    init_ecg(ECG_INST);
}

int32_t init_battery_adc(uint32_t instance)
{
    #if FSL_FEATURE_ADC_HAS_CALIBRATION
    adc_calibration_param_t batteryadcCalibraitionParam;
#endif
    adc_user_config_t batteryadcUserConfig;
    adc_chn_config_t batteryadcChnConfig;

#if FSL_FEATURE_ADC_HAS_CALIBRATION
    /* Auto calibraion. */   
    ADC_DRV_GetAutoCalibrationParam(instance, &batteryadcCalibraitionParam);
    ADC_DRV_SetCalibrationParam(instance, &batteryadcCalibraitionParam);
#endif

    /*
     * Initialization ADC for
     * 12bit resolution, interrrupt mode, hw trigger enabled.
     * normal convert speed, VREFH/L as reference, 
     * disable continuouse convert mode.
     */ 
    ADC_DRV_StructInitUserConfigForIntMode(&batteryadcUserConfig);
    //ecgadcUserConfig.hwTriggerEnable = true;
    batteryadcUserConfig.continuousConvEnable = false;
    batteryadcUserConfig.resolutionMode = kAdcResolutionBitOfSingleEndAs10;
	//	adcUserConfig.refVoltSrcMode = kAdcRefVoltSrcOfValt;
    ADC_DRV_Init(instance, &batteryadcUserConfig, &gAdcState);
    
    ADC_DRV_EnableHwAverage(instance,kAdcHwAverageCountOf32);

    /* Install Callback function into ISR. */
    ADC_DRV_InstallCallback(instance, BATTERYCHNGROUP, battery_adc_isr_callback);

    batteryadcChnConfig.chnNum = BATTERY_ADC_INPUT_CHAN;
    batteryadcChnConfig.diffEnable = false;
    batteryadcChnConfig.intEnable = true;
    batteryadcChnConfig.chnMux = kAdcChnMuxOfA;
    
    /* Configure channel0. */
    ADC_DRV_ConfigConvChn(instance, BATTERYCHNGROUP, &batteryadcChnConfig);
		
    return 0;
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
    
    
//    ecgadcChnConfig.chnNum = BATTERY_ADC_INPUT_CHAN;
//    ecgadcChnConfig.diffEnable = false;
//    ecgadcChnConfig.intEnable = true;
//    ecgadcChnConfig.chnMux = kAdcChnMuxOfB;
//    ADC_DRV_ConfigConvChn(instance, ECGCHNGROUP, &ecgadcChnConfig);	
    
    return 0;
}

void task_ecgcapture(task_param_t param)
{
	hPCEcgMsgQueue = OSA_MsgQCreate(mqPCEcgData, ECGPACKAGEDEEP, sizeof(ecgdatapackage));  //定义心电数据传输队列  
   // hBTEcgMsgQueue = OSA_MsgQCreate(mqBTEcgData, ECGPACKAGEDEEP, sizeof(ecgdatapackage));  //定义心电数据传输队列  
	
    //printf("To do the ADC init\n");
    
	if(init_ecg(ECG_INST))
	{
        //printf("Failed to do the ADC init\n");
        return;
	}
	init_trigger_source(ECG_INST);
	while(1)
	{
		OSA_TimeDelay(100);
	}
}
