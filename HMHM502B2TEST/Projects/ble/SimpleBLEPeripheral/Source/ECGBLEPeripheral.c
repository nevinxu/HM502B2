#include <stdio.h>
#include <string.h>
#include "bcomdef.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"

#include "OnBoard.h"
#include "hal_adc.h"
#include "hal_led.h"
#include "hal_ecg.h"          // ECG spi驱动
#include "hal_flash.h"
#include "hal_eeprom.h"

#include "gatt.h"

#include "hci.h"

#include "gapgattserver.h"
#include "gattservapp.h"
#include "devinfoservice.h"
#include "ECGGATTprofile.h"
#include "battservice.h"


#include "peripheral.h"

#include "gapbondmgr.h"

#include "ECGBLEPeripheral.h"

#if defined FEATURE_OAD
  #include "oad.h"
  #include "oad_target.h"
#endif

#include "npi.h"

//#if defined (SERIAL_INTERFACE)
#include "serialInterface.h"
//#endif

#include "BLE_SEND.h"
#include "ads1194.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

// How often to perform periodic event
#define SBP_PERIODIC_EVT_PERIOD                      2000
#define DEFAULT_BATT_PERIOD                          2000
#define CONNECT1_ECG_PERIOD                          500
#define CONNECT2_ECG_PERIOD                          100
#define DISCONNECT_ECG_PERIOD                        100

// What is the advertising interval when device is discoverable (units of 625us, 160=100ms)
#define DEFAULT_ADVERTISING_INTERVAL          160

// Limited discoverable mode advertises for 30.72s, and then stops
// General discoverable mode advertises indefinitely

//#define DEFAULT_DISCOVERABLE_MODE             GAP_ADTYPE_FLAGS_LIMITED

#define DEFAULT_DISCOVERABLE_MODE             GAP_ADTYPE_FLAGS_GENERAL


// Minimum connection interval (units of 1.25ms, 80=100ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     8

// Maximum connection interval (units of 1.25ms, 800=1000ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     80

// Slave latency to use if automatic parameter update request is enabled
#define DEFAULT_DESIRED_SLAVE_LATENCY         0

// Supervision timeout value (units of 10ms, 1000=10s) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_CONN_TIMEOUT          100

// Whether to enable automatic parameter update request when a connection is formed
#define DEFAULT_ENABLE_UPDATE_REQUEST         TRUE

// Connection Pause Peripheral time value (in seconds)
#define DEFAULT_CONN_PAUSE_PERIPHERAL         5

// Company Identifier: Texas Instruments Inc. (13)
#define TI_COMPANY_ID                         0x000D

#define INVALID_CONNHANDLE                    0xFFFF

// Length of bd addr as a string
#define B_ADDR_STR_LEN                        15

// Battery level is critical when it is less than this %
#define DEFAULT_BATT_CRITICAL_LEVEL           100 
   
  

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
uint8 ECGSensor_TaskID;   // Task ID for internal task/event processing

gaprole_States_t gapProfileState = GAPROLE_INIT;

// GAP - SCAN RSP data (max size = 31 bytes)
static uint8 scanRspData[] =
{
  // complete name
  0x11,   // length of this data
  GAP_ADTYPE_LOCAL_NAME_COMPLETE,
  'H',
  'M',
  '5',
  '0',
  '2',
  'B',
  '1',
  '0',
  '0',
  '0',
  '0',
  '0',
  '0',
  '0',
  '0',
  '0',

  
  // connection interval range
  0x05,   // length of this data
  GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE,
  LO_UINT16( DEFAULT_DESIRED_MIN_CONN_INTERVAL ),   // 100ms
  HI_UINT16( DEFAULT_DESIRED_MIN_CONN_INTERVAL ),
  LO_UINT16( DEFAULT_DESIRED_MAX_CONN_INTERVAL ),   // 1s
  HI_UINT16( DEFAULT_DESIRED_MAX_CONN_INTERVAL ),

  // Tx power level
  0x02,   // length of this data
  GAP_ADTYPE_POWER_LEVEL,
  0       // 0dBm
};

// GAP - Advertisement data (max size = 31 bytes, though this is
// best kept short to conserve power while advertisting)
static uint8 advertData[] =
{
  // Flags; this sets the device to use limited discoverable
  // mode (advertises for 30 seconds at a time) instead of general
  // discoverable mode (advertises indefinitely)
  0x02,   // length of this data
  GAP_ADTYPE_FLAGS,
  DEFAULT_DISCOVERABLE_MODE | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

  // service UUID, to notify central devices what services are included
  // in this peripheral
  0x03,   // length of this data
  GAP_ADTYPE_16BIT_MORE,      // some of the UUID's, but not all
  LO_UINT16( ECGPROFILE_SERV_UUID ),
  HI_UINT16( ECGPROFILE_SERV_UUID ),

};


// GAP GATT Attributes
static uint8 attDeviceName[GAP_DEVICE_NAME_LEN];

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void ECGBLEPeripheral_ProcessOSALMsg( osal_event_hdr_t *pMsg );
static void peripheralStateNotificationCB( gaprole_States_t newState );
static void performPeriodicTask( void );
static void ECGProfileChangeCB( uint8 paramID );
static void ECGSensorBattCB(uint8 event);


#if (defined HAL_LCD) && (HAL_LCD == TRUE)
static char *bdAddr2Str ( uint8 *pAddr );
#endif // (defined HAL_LCD) && (HAL_LCD == TRUE)


uint8 ECGRate = 1;
uint8 ECGGain = 1;
uint8 ECGNum = 0;

uint8 SerialApp_Buf[150];
extern uint8 DataReceiveSuccessFlag;
uint8 ECGBuffer[8];


uint8 SendDataLedFlag = 0;

/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static gapRolesCBs_t ECGBLEPeripheral_PeripheralCBs =
{
  peripheralStateNotificationCB,  // Profile State Change Callbacks
  NULL                            // When a valid RSSI is read from controller (not used by application)
};

// GAP Bond Manager Callbacks
static gapBondCBs_t ECGBLEPeripheral_BondMgrCBs =
{
  NULL,                     // Passcode callback (not used by application)
  NULL                      // Pairing / Bonding state Callback (not used by application)
};

// Simple GATT Profile Callbacks
static ECGProfileCBs_t ECGBLEPeripheral_SimpleProfileCBs =
{
  ECGProfileChangeCB    // Charactersitic value change callback
};
/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SimpleBLEPeripheral_Init
 *
 * @brief   Initialization function for the Simple BLE Peripheral App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void SimpleBLEPeripheral_Init( uint8 task_id )
{
  const uint8 buf[20] = "HM502B1000000000";
  uint8 ECGRateBuf[1];
  
  EEPROM_init();
  
/******************************************************************************/
  User_GetI2C_Data(0,17,attDeviceName);
  if((attDeviceName[0] != 'H') ||(attDeviceName[1] != 'M') ||(attDeviceName[2] != '5') ||(attDeviceName[3] != '0') ||(attDeviceName[4] != '2') ||(attDeviceName[5] != 'B') ||(attDeviceName[6] != '1'))
  {
    memcpy(attDeviceName,buf,17);
  }
  else
  {
    for(uint8 i = 0;i<9;i++)
    {
      if(((attDeviceName[7+i]>='0') && (attDeviceName[7+i]<='9')) || ((attDeviceName[7+i]>='A') && (attDeviceName[7+i]<='Z') ) )
      {
        ;
      }
      else
      {
        memcpy(attDeviceName,buf,17);
        break;
      }  
    }
  }
  memcpy(scanRspData+2,attDeviceName,17);
/******************************************************************************/
  
/******************************************************************************/ 
  User_GetI2C_Data(64,1,ECGRateBuf);
  if(ECGRateBuf[0]<=0 || (ECGRateBuf[0]>5))
  {
    ECGRate = 1;
  }
  else
  {
    ECGRate = ECGRateBuf[0];
  }
/******************************************************************************/
  
  ECGSensor_TaskID = task_id;

  // Setup the GAP
  VOID GAP_SetParamValue( TGAP_CONN_PAUSE_PERIPHERAL, DEFAULT_CONN_PAUSE_PERIPHERAL );
  
  // Setup the GAP Peripheral Role Profile
  {
    uint8 initial_advertising_enable = TRUE;
    uint16 gapRole_AdvertOffTime = 0;

    uint8 enable_update_request = DEFAULT_ENABLE_UPDATE_REQUEST;
    uint16 desired_min_interval = DEFAULT_DESIRED_MIN_CONN_INTERVAL;
    uint16 desired_max_interval = DEFAULT_DESIRED_MAX_CONN_INTERVAL;
    uint16 desired_slave_latency = DEFAULT_DESIRED_SLAVE_LATENCY;
    uint16 desired_conn_timeout = DEFAULT_DESIRED_CONN_TIMEOUT;

    // Set the GAP Role Parameters
    GAPRole_SetParameter( GAPROLE_ADVERT_ENABLED, sizeof( uint8 ), &initial_advertising_enable );
    GAPRole_SetParameter( GAPROLE_ADVERT_OFF_TIME, sizeof( uint16 ), &gapRole_AdvertOffTime );

    GAPRole_SetParameter( GAPROLE_SCAN_RSP_DATA, sizeof ( scanRspData ), scanRspData );
    GAPRole_SetParameter( GAPROLE_ADVERT_DATA, sizeof( advertData ), advertData );

    GAPRole_SetParameter( GAPROLE_PARAM_UPDATE_ENABLE, sizeof( uint8 ), &enable_update_request );
    GAPRole_SetParameter( GAPROLE_MIN_CONN_INTERVAL, sizeof( uint16 ), &desired_min_interval );
    GAPRole_SetParameter( GAPROLE_MAX_CONN_INTERVAL, sizeof( uint16 ), &desired_max_interval );
    GAPRole_SetParameter( GAPROLE_SLAVE_LATENCY, sizeof( uint16 ), &desired_slave_latency );
    GAPRole_SetParameter( GAPROLE_TIMEOUT_MULTIPLIER, sizeof( uint16 ), &desired_conn_timeout );
  }

  // Set the GAP Characteristics
  GGS_SetParameter( GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, attDeviceName );

  // Set advertising interval
  {
    uint16 advInt = DEFAULT_ADVERTISING_INTERVAL;

    GAP_SetParamValue( TGAP_LIM_DISC_ADV_INT_MIN, advInt );
    GAP_SetParamValue( TGAP_LIM_DISC_ADV_INT_MAX, advInt );
    GAP_SetParamValue( TGAP_GEN_DISC_ADV_INT_MIN, advInt );
    GAP_SetParamValue( TGAP_GEN_DISC_ADV_INT_MAX, advInt );
  }

  // Setup the GAP Bond Manager
  {
    uint32 passkey = 0; // passkey "000000"
    uint8 pairMode = GAPBOND_PAIRING_MODE_WAIT_FOR_REQ;
    uint8 mitm = TRUE;
    uint8 ioCap = GAPBOND_IO_CAP_DISPLAY_ONLY;
    uint8 bonding = TRUE;
    GAPBondMgr_SetParameter( GAPBOND_DEFAULT_PASSCODE, sizeof ( uint32 ), &passkey );
    GAPBondMgr_SetParameter( GAPBOND_PAIRING_MODE, sizeof ( uint8 ), &pairMode );
    GAPBondMgr_SetParameter( GAPBOND_MITM_PROTECTION, sizeof ( uint8 ), &mitm );
    GAPBondMgr_SetParameter( GAPBOND_IO_CAPABILITIES, sizeof ( uint8 ), &ioCap );
    GAPBondMgr_SetParameter( GAPBOND_BONDING_ENABLED, sizeof ( uint8 ), &bonding );
  }

  // Initialize GATT attributes
  GGS_AddService( GATT_ALL_SERVICES );            // GAP
  GATTServApp_AddService( GATT_ALL_SERVICES );    // GATT attributes
  DevInfo_AddService();                           // Device Information Service
  SimpleProfile_AddService( GATT_ALL_SERVICES );  // Simple GATT Profile
#if defined FEATURE_OAD
  VOID OADTarget_AddService();                    // OAD Profile
#endif

   // 设置Battery Characteristic Values
  uint8 critical = DEFAULT_BATT_CRITICAL_LEVEL;
  Batt_SetParameter( BATT_PARAM_CRITICAL_LEVEL, sizeof (uint8 ), &critical ); 
  Batt_AddService();                             // 电池电量服务
  //Register for Battery service callback;
  Batt_Register ( ECGSensorBattCB );
  
  
  // makes sure LEDs are off
  HalLedSet( (HAL_LED_YELLOW|HAL_LED_BLUE ), HAL_LED_MODE_OFF );
  
  
  ADS1194_PowerDown();
 // ADS1194_PowerUp();
 

  // Register callback with SimpleGATTprofile
  VOID SimpleProfile_RegisterAppCBs( &ECGBLEPeripheral_SimpleProfileCBs );
  SimpleProfile_SetParameter(ECGPROFILE_CHAR5,9,attDeviceName+7);
  SimpleProfile_SetParameter(ECGPROFILE_CHAR3,1,&ECGRate);

  // Enable clock divide on halt
  // This reduces active current while radio is active and CC254x MCU
  // is halted
  HCI_EXT_ClkDivOnHaltCmd( HCI_EXT_ENABLE_CLK_DIVIDE_ON_HALT );
  
  // Setup a delayed profile startup
  osal_set_event( ECGSensor_TaskID, SBP_START_DEVICE_EVT );
  
}

/*********************************************************************
 * @fn      ECGSensorBattCB
 *
 * @brief   Callback function for battery service.
 *
 * @param   event - service event
 *
 * @return  none
 */
static void ECGSensorBattCB(uint8 event)
{
  if (event == BATT_LEVEL_NOTI_ENABLED)
  {
    // if connected start periodic measurement
    if (gapProfileState == GAPROLE_CONNECTED)
    {
      Batt_MeasLevel();
      osal_start_timerEx( ECGSensor_TaskID, BATT_PERIODIC_EVT, DEFAULT_BATT_PERIOD );
    } 
  }
  else if (event == BATT_LEVEL_NOTI_DISABLED)
  {
    // stop periodic measurement
    osal_stop_timerEx( ECGSensor_TaskID, BATT_PERIODIC_EVT );
  }
}


/*********************************************************************
 * @fn      SimpleBLEPeripheral_ProcessEvent
 *
 * @brief   Simple BLE Peripheral Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
uint16 SimpleBLEPeripheral_ProcessEvent( uint8 task_id, uint16 events )
{
  static unsigned char i,j;
  VOID task_id; // OSAL required parameter that isn't used in this function
  
  if ( events & SYS_EVENT_MSG )
  {
    uint8 *pMsg;

    if ( (pMsg = osal_msg_receive( ECGSensor_TaskID )) != NULL )
    {
      ECGBLEPeripheral_ProcessOSALMsg( (osal_event_hdr_t *)pMsg );

      // Release the OSAL message
      VOID osal_msg_deallocate( pMsg );
    }
    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }

  if ( events & SBP_START_DEVICE_EVT )
  {
    // Start the Device
    VOID GAPRole_StartDevice( &ECGBLEPeripheral_PeripheralCBs );

    // Start Bond Manager
    VOID GAPBondMgr_Register( &ECGBLEPeripheral_BondMgrCBs );

    // Set timer for first periodic event
    osal_start_timerEx( ECGSensor_TaskID, SBP_PERIODIC_EVT, SBP_PERIODIC_EVT_PERIOD );   //5s  周期时间
    osal_start_timerEx( ECGSensor_TaskID, BATT_PERIODIC_EVT , 1000 );
    return ( events ^ SBP_START_DEVICE_EVT );
  }

  if ( events & ECG_PERIODIC_EVT )
  {
    // Perform periodic ECG task
    if (gapProfileState == GAPROLE_CONNECTED)
    {
      NPI_WriteTransport("Peripheral Send a ECGPacket to Central!\n",strlen("Peripheral Send a ECGPacket to Central!\n"));
    }  
    return (events ^ ECG_PERIODIC_EVT);
  } 
  
  if ( events & SBP_PERIODIC_EVT )
  {
    if(gapProfileState == GAPROLE_ADVERTISING)
    {
      NPI_WriteTransport("BLE Peripheral Advetising...\n",strlen("BLE Peripheral Advetising...\n"));
    }
    // Restart timer
    if ( SBP_PERIODIC_EVT_PERIOD )
    {
      osal_start_timerEx( ECGSensor_TaskID, SBP_PERIODIC_EVT, SBP_PERIODIC_EVT_PERIOD );
    }
    return (events ^ SBP_PERIODIC_EVT);
  }
  
  if ( events & BATT_PERIODIC_EVT )
  {
    Batt_MeasLevel();
    i = battMeasure();
      j++;
      if(i <= 1)
      { 
        if(j%2)
        {
          HalLedSet( HAL_LED_BLUE, HAL_LED_MODE_OFF );
          HalLedSet( HAL_LED_YELLOW, HAL_LED_MODE_OFF );
          ADS1194_PowerDown();
        }
        else
        {
          HalLedSet( HAL_LED_BLUE, HAL_LED_MODE_ON );
          HalLedSet( HAL_LED_YELLOW, HAL_LED_MODE_OFF );
          ADS1194_PowerDown();
        }
        osal_start_timerEx( ECGSensor_TaskID, BATT_PERIODIC_EVT, 100 );
      }
      else
      {
        HalLedSet( HAL_LED_BLUE, HAL_LED_MODE_OFF );
        osal_start_timerEx( ECGSensor_TaskID, BATT_PERIODIC_EVT, DEFAULT_BATT_PERIOD );
      }
    return (events ^ BATT_PERIODIC_EVT);
  }
  
  
  if ( events & CONNECT1_ECG_EVT )
  {
    ADS1194_PowerUp();   //上电需要延时
    
    NPI_WriteTransport("ECG PowerUp!\n",strlen("ECG PowerUp!\n"));
    
    osal_start_timerEx( ECGSensor_TaskID, CONNECT2_ECG_EVT, CONNECT2_ECG_PERIOD );
    return (events ^ CONNECT1_ECG_EVT);
  }
  
  if ( events & CONNECT2_ECG_EVT )
  {
    // 初始化ADS1194
    ADS1194_Init(ECGRate,ECGGain);
    
    NPI_WriteTransport("ECG Init!\n",strlen("ECG Init!\n"));
    
    return (events ^ CONNECT2_ECG_EVT);
  }
  
  if ( events & DISCONNECT_ECG_EVT )
  {
    ADS1194_PowerDown();
    NPI_WriteTransport("ECG PowerDown!\n",strlen("ECG PowerDown!\n"));
    HalLedSet( (HAL_LED_YELLOW ), HAL_LED_MODE_OFF );
    return (events ^ DISCONNECT_ECG_EVT);
  }
  
  return 0;
}

/*********************************************************************
 * @fn      ECGBLEPeripheral_ProcessOSALMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void ECGBLEPeripheral_ProcessOSALMsg( osal_event_hdr_t *pMsg )
{
  switch ( pMsg->event )
  {
  default:
    // do nothing
    break;
  }
}

/*********************************************************************
 * @fn      peripheralStateNotificationCB
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void peripheralStateNotificationCB( gaprole_States_t newState )
{
#ifdef PLUS_BROADCASTER
  static uint8 first_conn_flag = 0;
#endif // PLUS_BROADCASTER
  
  
  switch ( newState )
  {
    case GAPROLE_STARTED:
      {
        uint8 ownAddress[B_ADDR_LEN];
        uint8 systemId[DEVINFO_SYSTEM_ID_LEN];

        GAPRole_GetParameter(GAPROLE_BD_ADDR, ownAddress);

        // use 6 bytes of device address for 8 bytes of system ID value
        systemId[0] = ownAddress[0];
        systemId[1] = ownAddress[1];
        systemId[2] = ownAddress[2];

        // set middle bytes to zero
        systemId[4] = 0x00;
        systemId[3] = 0x00;

        // shift three bytes up
        systemId[7] = ownAddress[5];
        systemId[6] = ownAddress[4];
        systemId[5] = ownAddress[3];

        DevInfo_SetParameter(DEVINFO_SYSTEM_ID, DEVINFO_SYSTEM_ID_LEN, systemId);
        
        HalLedSet( (HAL_LED_BLUE ), HAL_LED_MODE_ON);
        NPI_WriteTransport("BLE Peripheral Start...\n",strlen("BLE Peripheral Start...\n"));

      }
      break;

    case GAPROLE_ADVERTISING:
      {
        NPI_WriteTransport("BLE Peripheral Advetising...\n",strlen("BLE Peripheral Advetising...\n"));
      }
      break;

    case GAPROLE_CONNECTED:
      {              
        NPI_WriteTransport("BLE Peripheral Connected!!!\n",strlen("BLE Peripheral Connected!!!\n"));
        osal_start_timerEx( ECGSensor_TaskID, CONNECT1_ECG_EVT, CONNECT1_ECG_PERIOD );
        HalLedSet( (HAL_LED_BLUE ), HAL_LED_MODE_OFF);
        HalLedSet( (HAL_LED_YELLOW ), HAL_LED_MODE_ON);
        
#ifdef PLUS_BROADCASTER
        // Only turn advertising on for this state when we first connect
        // otherwise, when we go from connected_advertising back to this state
        // we will be turning advertising back on.
        if ( first_conn_flag == 0 ) 
        {
          uint8 adv_enabled_status = 1;
          GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8), &adv_enabled_status); // Turn on Advertising
          first_conn_flag = 1;
        }
#endif // PLUS_BROADCASTER
      }
      break;

    case GAPROLE_CONNECTED_ADV:
      {
        
      }
      break;      
    case GAPROLE_WAITING:
      {
        NPI_WriteTransport("BLE Peripheral Disconnected!\n",strlen("BLE Peripheral Disconnected!\n"));
        osal_start_timerEx( ECGSensor_TaskID, DISCONNECT_ECG_EVT, DISCONNECT_ECG_PERIOD );
      }
      break;

    case GAPROLE_WAITING_AFTER_TIMEOUT:
      {
        NPI_WriteTransport("BLE Peripheral Disconnected!\n",strlen("BLE Peripheral Disconnected!\n"));
        osal_start_timerEx( ECGSensor_TaskID, DISCONNECT_ECG_EVT, DISCONNECT_ECG_PERIOD );
          
#ifdef PLUS_BROADCASTER
        // Reset flag for next connection.
        first_conn_flag = 0;
#endif //#ifdef (PLUS_BROADCASTER)
      }
      break;

    case GAPROLE_ERROR:
      {
        
      }
      break;

    default:
      {

      }
      break;

  }

  gapProfileState = newState;

#if !defined( CC2540_MINIDK )
  VOID gapProfileState;     // added to prevent compiler warning with
                            // "CC2540 Slave" configurations
#endif


}


/*********************************************************************
 * @fn      ECGProfileChangeCB
 *
 * @brief   Callback from SimpleBLEProfile indicating a value change
 *
 * @param   paramID - parameter ID of the value that was changed.
 *
 * @return  none
 */
static void ECGProfileChangeCB( uint8 paramID )
{
  uint8 newValue;
  uint8 newValuePtr[19];
#if 1
  switch( paramID )
  {
    case ECGPROFILE_CHAR1:
      SimpleProfile_GetParameter( ECGPROFILE_CHAR1, newValuePtr );
      break;

    case ECGPROFILE_CHAR3:
      SimpleProfile_GetParameter( ECGPROFILE_CHAR3, &newValue );
      newValuePtr[0] = newValue;
      Write_Page(1,1,newValuePtr);

      break;
    case ECGPROFILE_CHAR5:
      SimpleProfile_GetParameter( ECGPROFILE_CHAR5, newValuePtr );
      memcpy(attDeviceName+7,newValuePtr,9);
      Write_Page(0,17,attDeviceName);
      break;

    default:
      // should not reach here!
      break;
  }
#endif
}

#if (defined HAL_LCD) && (HAL_LCD == TRUE)
/*********************************************************************
 * @fn      bdAddr2Str
 *
 * @brief   Convert Bluetooth address to string. Only needed when
 *          LCD display is used.
 *
 * @return  none
 */
char *bdAddr2Str( uint8 *pAddr )
{
  uint8       i;
  char        hex[] = "0123456789ABCDEF";
  static char str[B_ADDR_STR_LEN];
  char        *pStr = str;

  *pStr++ = '0';
  *pStr++ = 'x';

  // Start from end of addr
  pAddr += B_ADDR_LEN;

  for ( i = B_ADDR_LEN; i > 0; i-- )
  {
    *pStr++ = hex[*--pAddr >> 4];
    *pStr++ = hex[*pAddr & 0x0F];
  }

  *pStr = 0;

  return str;
}
#endif // (defined HAL_LCD) && (HAL_LCD == TRUE)

/*********************************************************************
*********************************************************************/
