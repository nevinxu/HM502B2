#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include "hal_led.h"
#include "ECGBLEPeripheral.h"
#include "BLE_SEND.h"
#include "OSAL_Memory.h"

#include "battservice.h"
#include "ECGGATTprofile.h"

#include "npi.h"


#define ECG_DR_125      125
#define ECG_DR_250      250
#define ECG_DR_500      500
#define ECG_DR_1000     1000

#define SAMPLENUM       128



STRREV strrev;


void RevdataInit(void)
{
  osal_memset((uint8*)&strrev,0,sizeof(STRREV));
}

///*************************************************************************************************



extern uint8 ECGSensor_TaskID; 
void Rev_data_analy(uint8* data)
{
  static unsigned char j;
  static unsigned char Sequence;
  unsigned char i;
  
  attHandleValueNoti_t SendData;
    SendData.len = 19;
    SendData.handle = 37;
 // unsigned char SendData[19];
  static uint8  packnum = 0;
  uint8 LOFF,LEAD_II_H,LEAD_II_L;
  
  //提取电极脱落状态和I导联数据；
  LOFF    = ((data[1]&0xf0)|((data[2]&0xf0)>>4));
  LEAD_II_H  = data[5];
  LEAD_II_L  = data[6];

  strrev.strsenddata.data[packnum][strrev.len++] = LEAD_II_H;
  strrev.strsenddata.data[packnum][strrev.len++] = LEAD_II_L;

  if(strrev.len >= MAXNUM)
  {   
      packnum++;
      if(packnum >= PACKSIZE)
      {
        packnum = 0;
        for(i = 0; i < PACKSIZE; i++)
        {
          //每16 Bit的数据发送一次电量和电极状态；
          strrev.strsenddata.state[0] = Sequence++;
          strrev.strsenddata.state[1] = battMeasure();
          strrev.strsenddata.state[2] = LOFF;
          osal_memcpy(SendData.value,strrev.strsenddata.state,3);
          osal_memcpy(&(SendData.value[3]),strrev.strsenddata.data[i],16);
          GATT_Notification( 0, &SendData, FALSE );
         // SimpleProfile_SetParameter(SIMPLEPROFILE_CHAR1,19,SendData);
        //  NPI_WriteTransport(SendData.value,8);
        }
        
        osal_set_event( ECGSensor_TaskID, ECG_PERIODIC_EVT);
        RevdataInit();
        j++;
        if(j>=2)
        {
          j = 0;
          
          HalLedSet( HAL_LED_YELLOW, HAL_LED_MODE_TOGGLE );
        }
      }
      strrev.len=0;
  }

}

//**************************************************************************************************/


