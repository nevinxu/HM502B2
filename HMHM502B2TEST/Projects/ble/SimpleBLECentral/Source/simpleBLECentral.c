#include <stdio.h>
#include <string.h>
#include "bcomdef.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"
#include "OnBoard.h"
#include "hal_led.h"
#include "hal_uart.h"
#include "gatt.h"
#include "ll.h"
#include "hci.h"
#include "gapgattserver.h"
#include "gattservapp.h"
#include "central.h"
#include "gapbondmgr.h"
#include "ECGGATTprofile.h"
#include "simpleBLECentral.h"
#include "serialInterface.h"

/*********************************************************************
 * MACROS
 */

// Length of bd addr as a string
#define B_ADDR_STR_LEN                        15

/*********************************************************************
 * CONSTANTS
 */

// Maximum number of scan responses
#define DEFAULT_MAX_SCAN_RES                  8

// Scan duration in ms
#define DEFAULT_SCAN_DURATION                 2000

// Discovey mode (limited, general, all)
#define DEFAULT_DISCOVERY_MODE                DEVDISC_MODE_ALL

// TRUE to use active scan
#define DEFAULT_DISCOVERY_ACTIVE_SCAN         TRUE

// TRUE to use white list during discovery
#define DEFAULT_DISCOVERY_WHITE_LIST          FALSE

// TRUE to use high scan duty cycle when creating link
#define DEFAULT_LINK_HIGH_DUTY_CYCLE          FALSE

// TRUE to use white list when creating link
#define DEFAULT_LINK_WHITE_LIST               FALSE

// Default RSSI polling period in ms
#define DEFAULT_RSSI_PERIOD                   1000

// Whether to enable automatic parameter update request when a connection is formed
#define DEFAULT_ENABLE_UPDATE_REQUEST         TRUE

// Minimum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_UPDATE_MIN_CONN_INTERVAL      200

// Maximum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_UPDATE_MAX_CONN_INTERVAL      2000

// Slave latency to use if automatic parameter update request is enabled
#define DEFAULT_UPDATE_SLAVE_LATENCY          0

// Supervision timeout value (units of 10ms) if automatic parameter update request is enabled
#define DEFAULT_UPDATE_CONN_TIMEOUT           600

// Default passcode
#define DEFAULT_PASSCODE                      19655

// Default GAP pairing mode
#define DEFAULT_PAIRING_MODE                  GAPBOND_PAIRING_MODE_WAIT_FOR_REQ

// Default MITM mode (TRUE to require passcode or OOB when pairing)
#define DEFAULT_MITM_MODE                     FALSE

// Default bonding mode, TRUE to bond
#define DEFAULT_BONDING_MODE                  TRUE

// Default GAP bonding I/O capabilities
#define DEFAULT_IO_CAPABILITIES               GAPBOND_IO_CAP_DISPLAY_ONLY

// Default service discovery timer delay in ms
#define DEFAULT_SVC_DISCOVERY_DELAY           500
#define DEFAULT_SCAN_DELAY                      3000
#define DEFAULT_CONNECT_DELAY                   500
#define DEFAULT_NOTIFY_DELAY                    300

// TRUE to filter discovery results on desired service UUID
#define DEFAULT_DEV_DISC_BY_SVC_UUID          FALSE

// Application states
enum
{
  BLE_STATE_IDLE,
  BLE_STATE_CONNECTING,
  BLE_STATE_CONNECTED,
  BLE_STATE_DISCONNECTING
};

// Discovery states
enum
{
  BLE_DISC_STATE_IDLE,                // Idle
  BLE_DISC_STATE_SVC,                 // Service discovery
  BLE_DISC_STATE_CHAR                 // Characteristic discovery
};

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

// Task ID for internal task/event processing
uint8 simpleBLETaskId;

// GAP GATT Attributes
static const uint8 simpleBLEDeviceName[GAP_DEVICE_NAME_LEN] = "HM502B2 Central";

// Number of scan results and scan result index
static uint8 simpleBLEScanRes;
uint8 simpleBLEScanIdx;

// Scan result list
static gapDevRec_t simpleBLEDevList[DEFAULT_MAX_SCAN_RES];

// Scanning state
static uint8 simpleBLEScanning = FALSE;

// RSSI polling state
static uint8 simpleBLERssi = FALSE;

// Connection handle of current connection 
static uint16 simpleBLEConnHandle = GAP_CONNHANDLE_INIT;

// Application state
static uint8 simpleBLEState = BLE_STATE_IDLE;

// Discovery state
static uint8 simpleBLEDiscState = BLE_DISC_STATE_IDLE;

// Discovered service start and end handle
static uint16 simpleBLESvcStartHdl = 0;
static uint16 simpleBLESvcEndHdl = 0;

// Discovered characteristic handle
static uint16 BLEDataCharHdl = 0;
static uint16 IDValueCharHdl = 0;
static uint8 GetHalflag = 0;

// Value to write
static uint8 simpleBLECharVal = 0;

// Value read/write toggle
static bool simpleBLEDoWrite = FALSE;

// GATT read/write procedure state
static bool simpleBLEProcedureInProgress = FALSE;


extern BLEPacket_t  rxSerialPkt;
extern BLEPacket_t  txSerialPkt;

uint8 AutoConnectFlag = 0;

uint8 IDValue[9];

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void simpleBLECentralProcessGATTMsg( gattMsgEvent_t *pMsg );
static void simpleBLECentralRssiCB( uint16 connHandle, int8  rssi );
static void simpleBLECentralEventCB( gapCentralRoleEvent_t *pEvent );
static void simpleBLECentralPasscodeCB( uint8 *deviceAddr, uint16 connectionHandle,
                                        uint8 uiInputs, uint8 uiOutputs );
static void simpleBLECentralPairStateCB( uint16 connHandle, uint8 state, uint8 status );
static void simpleBLECentral_ProcessOSALMsg( osal_event_hdr_t *pMsg );
static void simpleBLEGATTDiscoveryEvent( gattMsgEvent_t *pMsg );
static void simpleBLECentralStartDiscovery( void );
static bool simpleBLEFindSvcUuid( uint16 uuid, uint8 *pData, uint8 dataLen );
static void simpleBLEAddDeviceInfo( uint8 *pAddr, uint8 addrType );
char *bdAddr2Str ( uint8 *pAddr );

/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static const gapCentralRoleCB_t simpleBLERoleCB =
{
  simpleBLECentralRssiCB,       // RSSI callback
  simpleBLECentralEventCB       // Event callback
};

// Bond Manager Callbacks
static const gapBondCBs_t simpleBLEBondCB =
{
  simpleBLECentralPasscodeCB,
  simpleBLECentralPairStateCB
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SimpleBLECentral_Init
 *
 * @brief   Initialization function for the Simple BLE Central App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notification).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void SimpleBLECentral_Init( uint8 task_id )
{
  simpleBLETaskId = task_id;

  // Setup Central Profile
  {
    uint8 scanRes = DEFAULT_MAX_SCAN_RES;
    GAPCentralRole_SetParameter ( GAPCENTRALROLE_MAX_SCAN_RES, sizeof( uint8 ), &scanRes );
  }
  
  // Setup GAP
  GAP_SetParamValue( TGAP_GEN_DISC_SCAN, DEFAULT_SCAN_DURATION );
  GAP_SetParamValue( TGAP_LIM_DISC_SCAN, DEFAULT_SCAN_DURATION );
  GGS_SetParameter( GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, (uint8 *) simpleBLEDeviceName );
  
#if 1
  GAP_SetParamValue(TGAP_CONN_PARAM_TIMEOUT,1);
  GAP_SetParamValue(TGAP_CONN_ADV_INT_MIN,16);
  GAP_SetParamValue(TGAP_CONN_ADV_INT_MAX,16);
  GAP_SetParamValue(TGAP_CONN_EST_ADV_TIMEOUT,100);
  GAP_SetParamValue(TGAP_CONN_EST_LATENCY,1);
#endif
  GAP_SetParamValue(TGAP_CONN_EST_SUPERV_TIMEOUT,50);   //2s

  // Setup the GAP Bond Manager
  {
    uint32 passkey = DEFAULT_PASSCODE;
    uint8 pairMode = DEFAULT_PAIRING_MODE;
    uint8 mitm = DEFAULT_MITM_MODE;
    uint8 ioCap = DEFAULT_IO_CAPABILITIES;
    uint8 bonding = DEFAULT_BONDING_MODE;
    GAPBondMgr_SetParameter( GAPBOND_DEFAULT_PASSCODE, sizeof( uint32 ), &passkey );
    GAPBondMgr_SetParameter( GAPBOND_PAIRING_MODE, sizeof( uint8 ), &pairMode );
    GAPBondMgr_SetParameter( GAPBOND_MITM_PROTECTION, sizeof( uint8 ), &mitm );
    GAPBondMgr_SetParameter( GAPBOND_IO_CAPABILITIES, sizeof( uint8 ), &ioCap );
    GAPBondMgr_SetParameter( GAPBOND_BONDING_ENABLED, sizeof( uint8 ), &bonding );
  }  

  // Initialize GATT Client
  VOID GATT_InitClient();

  // Register to receive incoming ATT Indications/Notifications
  GATT_RegisterForInd( simpleBLETaskId );

  // Initialize GATT attributes
  GGS_AddService( GATT_ALL_SERVICES );         // GAP
  GATTServApp_AddService( GATT_ALL_SERVICES ); // GATT attributes

  // Register for all key events - This app will handle all key events
  RegisterForKeys( simpleBLETaskId );
  
  HalLedSet( (HAL_LED_BLUE | HAL_LED_YELLOW), HAL_LED_MODE_OFF );
  HalLedSet( HAL_LED_BLUE, HAL_LED_MODE_ON );
 // HalLedSet( HAL_LED_YELLOW, HAL_LED_MODE_ON );
  
  // Setup a delayed profile startup
  osal_set_event( simpleBLETaskId, START_DEVICE_EVT );
}

/*********************************************************************
 * @fn      SimpleBLECentral_ProcessEvent
 *
 * @brief   Simple BLE Central Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
uint16 SimpleBLECentral_ProcessEvent( uint8 task_id, uint16 events )
{
  VOID task_id; // OSAL required parameter that isn't used in this function
/********************************************************************************/  
  if ( events & SYS_EVENT_MSG )
  {
    uint8 *pMsg;

    if ( (pMsg = osal_msg_receive( simpleBLETaskId )) != NULL )
    {
      simpleBLECentral_ProcessOSALMsg( (osal_event_hdr_t *)pMsg );

      // Release the OSAL message
      VOID osal_msg_deallocate( pMsg );
    }
    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }
  
/********************************************************************************/
  if ( events & START_DEVICE_EVT )
  {
    // Start the Device
    VOID GAPCentralRole_StartDevice( (gapCentralRoleCB_t *) &simpleBLERoleCB );

    // Register with bond manager after starting device
    GAPBondMgr_Register( (gapBondCBs_t *) &simpleBLEBondCB );
    osal_start_timerEx( simpleBLETaskId, simpleBLE_PERIODIC_EVT, 500 );
    return ( events ^ START_DEVICE_EVT );
  }
  if ( events & START_SCAN_EVT )
  {
    if ( simpleBLEState != BLE_STATE_CONNECTED )
    {
      if ( !simpleBLEScanning )
      {
        HalLedSet( HAL_LED_BLUE, HAL_LED_MODE_FLASH );
        simpleBLEScanning = TRUE;
        simpleBLEScanRes = 0;
      
        GAPCentralRole_StartDiscovery( DEFAULT_DISCOVERY_MODE,
                                       DEFAULT_DISCOVERY_ACTIVE_SCAN,
                                       DEFAULT_DISCOVERY_WHITE_LIST );      
      }
    }   
    return ( events ^ START_SCAN_EVT );
  }
  
/********************************************************************************/  
  if(events & START_CONNECT_EVT)
  {
    static uint8 addrType;
    static uint8 *peerAddr;
    GetHalflag = 0;
// Connect or disconnect
    if ( simpleBLEState == BLE_STATE_IDLE )
    {
      // if there is a scan result
      if ( simpleBLEScanRes > 0 )
      {
        // connect to current device in scan result
        peerAddr = simpleBLEDevList[simpleBLEScanIdx].addr;
        addrType = simpleBLEDevList[simpleBLEScanIdx].addrType;
      
        simpleBLEState = BLE_STATE_CONNECTING;
        
        GAPCentralRole_EstablishLink( DEFAULT_LINK_HIGH_DUTY_CYCLE,
                                      DEFAULT_LINK_WHITE_LIST,
                                      addrType, peerAddr );
      }
    }
    return ( events ^ START_CONNECT_EVT );
  }
   if ( events & simpleBLE_PERIODIC_EVT )
  {
    osal_start_timerEx( simpleBLETaskId, simpleBLE_PERIODIC_EVT, 4000 );
    if ( simpleBLEState == BLE_STATE_IDLE )
    {
      if(AutoConnectFlag == 1)
      {
        txSerialPkt.header.identifier = rxSerialPkt.header.identifier;
        txSerialPkt.header.opCode = APP_CMD_ADVERTISEBEGIN;
        txSerialPkt.header.status = 0x00;
        txSerialPkt.length = 0;
        sendSerialEvt();
        osal_set_event( simpleBLETaskId, START_SCAN_EVT);
      }
    }
    if(simpleBLEState == BLE_STATE_CONNECTED)
    {
      if ( !simpleBLERssi )
      {
        simpleBLERssi = TRUE;
        GAPCentralRole_StartRssi( simpleBLEConnHandle, DEFAULT_RSSI_PERIOD );
      }
    }
    return (events ^ simpleBLE_PERIODIC_EVT);
  }
/********************************************************************************/  
  
  if ( events & START_DISCOVERY_EVT )
  {
    simpleBLECentralStartDiscovery( );
    return ( events ^ START_DISCOVERY_EVT );
  }
  
  if ( events & START_DISCONNECT_EVT )
  {
    if ( simpleBLEState == BLE_STATE_CONNECTED )
    {   
      GAPCentralRole_TerminateLink( simpleBLEConnHandle );
    }
    
    else
    {
          txSerialPkt.header.identifier = SERIAL_IDENTIFIER;
          txSerialPkt.header.opCode = APP_CMD_DISCONNECTBLEACK;
          txSerialPkt.header.status = 0x00;
          txSerialPkt.length = 0;
          sendSerialEvt(); 
    }   

    return ( events ^ START_DISCONNECT_EVT );
  }
  // Discard unknown events
  return 0;
}
/*
void SendCommand2Peripheral()
{
        attWriteReq_t req;
        
        req.handle = simpleBLECharHdl;
        req.len = 1;
        req.value[0] = simpleBLECharVal;
        req.sig = 0;
        req.cmd = 0;
        status = GATT_WriteCharValue( simpleBLEConnHandle, &req, simpleBLETaskId );   
}  
*/
/*********************************************************************
 * @fn      simpleBLECentral_ProcessOSALMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void simpleBLECentral_ProcessOSALMsg( osal_event_hdr_t *pMsg )
{
  switch ( pMsg->event )
  {
    case GATT_MSG_EVENT:
      simpleBLECentralProcessGATTMsg( (gattMsgEvent_t *) pMsg );
      break;
  }
}

/*********************************************************************
 * @fn      simpleBLECentralProcessGATTMsg
 *
 * @brief   Process GATT messages
 *
 * @return  none
 */
static void simpleBLECentralProcessGATTMsg( gattMsgEvent_t *pMsg )
{
  if ( simpleBLEState != BLE_STATE_CONNECTED )
  {
    // In case a GATT message came after a connection has dropped,
    // ignore the message
    return;
  }
  
  if ( ( pMsg->method == ATT_READ_RSP ) ||
       ( ( pMsg->method == ATT_ERROR_RSP ) &&
         ( pMsg->msg.errorRsp.reqOpcode == ATT_READ_REQ ) ) )
  {
    if ( pMsg->method == ATT_ERROR_RSP )
    {
     // uint8 status = pMsg->msg.errorRsp.errCode;
      
    }
    
    simpleBLEProcedureInProgress = FALSE;
  }
  else if ( ( pMsg->method == ATT_WRITE_RSP ) ||
       ( ( pMsg->method == ATT_ERROR_RSP ) &&
         ( pMsg->msg.errorRsp.reqOpcode == ATT_WRITE_REQ ) ) )
  {
    
    if ( pMsg->method == ATT_ERROR_RSP == ATT_ERROR_RSP )
    {
   //   uint8 status = pMsg->msg.errorRsp.errCode;
      
    }
    
    simpleBLEProcedureInProgress = FALSE;    

  }
  else if ( simpleBLEDiscState != BLE_DISC_STATE_IDLE )
  {
    simpleBLEGATTDiscoveryEvent( pMsg );
  }
#if 1
  if ((pMsg->method == ATT_HANDLE_VALUE_NOTI) )
  {
    
    if(simpleBLEProcedureInProgress == FALSE )
    {
      txSerialPkt.header.identifier = SERIAL_IDENTIFIER;
      txSerialPkt.header.opCode = APP_CMD_DATASEND;
      txSerialPkt.header.status = 0x00;
      txSerialPkt.length = pMsg->msg.handleValueNoti.len;
      osal_memcpy(txSerialPkt.data,pMsg->msg.handleValueNoti.value,txSerialPkt.length);
      sendSerialEvt();
    }
  }
#endif
}

/*********************************************************************
 * @fn      simpleBLECentralRssiCB
 *
 * @brief   RSSI callback.
 *
 * @param   connHandle - connection handle
 * @param   rssi - RSSI
 *
 * @return  none
 */
static void simpleBLECentralRssiCB( uint16 connHandle, int8 rssi )
{
  txSerialPkt.header.identifier = SERIAL_IDENTIFIER;
  txSerialPkt.header.opCode = APP_CMD_RSSIVALUE;
  txSerialPkt.header.status = 0x00;
  txSerialPkt.length = 1;
  txSerialPkt.data[0] = rssi;
  sendSerialEvt();
 //   LCD_WRITE_STRING_VALUE( "RSSI -dB:", (uint8) (-rssi), 10, HAL_LCD_LINE_1 );
}

/*********************************************************************
 * @fn      simpleBLECentralEventCB
 *
 * @brief   Central event callback function.
 *
 * @param   pEvent - pointer to event structure
 *
 * @return  none
 */
static void simpleBLECentralEventCB( gapCentralRoleEvent_t *pEvent )
{
  switch ( pEvent->gap.opcode )
  {
/***********************************************************************************************/    
    case GAP_DEVICE_INIT_DONE_EVENT:     //初始化完成回调函数
      {
        
      }
      break;
      
/***********************************************************************************************/ 
    case GAP_DEVICE_INFO_EVENT:     
      {
        // if filtering device discovery results based on service UUID
        if ( DEFAULT_DEV_DISC_BY_SVC_UUID == TRUE )
        {
          if ( simpleBLEFindSvcUuid( ECGPROFILE_SERV_UUID,
                                     pEvent->deviceInfo.pEvtData,
                                     pEvent->deviceInfo.dataLen ) )
          {
            simpleBLEAddDeviceInfo( pEvent->deviceInfo.addr, pEvent->deviceInfo.addrType );
          }
        }
      }
      break;
      
/***********************************************************************************************/       
    case GAP_DEVICE_DISCOVERY_EVENT:   //搜索结果回调函数   
      {
        // if not filtering device discovery results based on service UUID
        if ( DEFAULT_DEV_DISC_BY_SVC_UUID == FALSE )
        {
          // Copy results
          simpleBLEScanRes = pEvent->discCmpl.numDevs;
          osal_memcpy( simpleBLEDevList, pEvent->discCmpl.pDevList,
                       (sizeof( gapDevRec_t ) * pEvent->discCmpl.numDevs) );
        }
        simpleBLEScanning = FALSE;
        HalLedSet( HAL_LED_BLUE, HAL_LED_MODE_ON );    
        HalLedSet( HAL_LED_YELLOW, HAL_LED_MODE_OFF );
        
        txSerialPkt.header.identifier = SERIAL_IDENTIFIER;
        txSerialPkt.header.opCode = APP_CMD_ADVERTISEACK;
        txSerialPkt.header.status = 0x00;
        txSerialPkt.length = simpleBLEScanRes*6;
        for(int i = 0;i<simpleBLEScanRes; i++)
        {
          for(int j = 0;j<6;j++)
          {
            txSerialPkt.data[i*6+j] = simpleBLEDevList[i].addr[j];
          }
        }
        sendSerialEvt();
        
        // initialize scan index to last device
        simpleBLEScanIdx = simpleBLEScanRes;
        simpleBLEScanIdx = 0;   //选择第一个index
        if(simpleBLEScanRes > 0)
        {
          if(AutoConnectFlag == 1)
          {
            osal_start_timerEx( simpleBLETaskId, START_CONNECT_EVT,100);
          }
        }

      }
      break;
      
/***********************************************************************************************/ 
    case GAP_LINK_ESTABLISHED_EVENT:
      {
        if ( pEvent->gap.hdr.status == SUCCESS )   //连接成功   
        {          
          simpleBLEState = BLE_STATE_CONNECTED;   //连接状态    
          simpleBLEConnHandle = pEvent->linkCmpl.connectionHandle;
          
          simpleBLEProcedureInProgress = TRUE;      // 

          txSerialPkt.header.identifier = SERIAL_IDENTIFIER;
          txSerialPkt.header.opCode = APP_CMD_CONNECTBLEACK;
          txSerialPkt.header.status = 0x00;
          txSerialPkt.length = 0;
          sendSerialEvt();
          
          HalLedSet( HAL_LED_YELLOW, HAL_LED_MODE_ON );    
          HalLedSet( HAL_LED_BLUE, HAL_LED_MODE_OFF );
          simpleBLEProcedureInProgress = FALSE;
         // osal_start_timerEx( simpleBLETaskId, START_DISCOVERY_EVT, DEFAULT_SVC_DISCOVERY_DELAY );
        }
        else    //连接失败   
        {     
          simpleBLEState = BLE_STATE_IDLE;
          simpleBLEProcedureInProgress = FALSE;
          
          txSerialPkt.header.identifier = SERIAL_IDENTIFIER;
          txSerialPkt.header.opCode = APP_CMD_CONNECTBLEACK;
          txSerialPkt.header.status = 0x01;
          txSerialPkt.length = 0;
          sendSerialEvt();    
          HalLedSet( HAL_LED_BLUE, HAL_LED_MODE_ON ); 
          HalLedSet( HAL_LED_YELLOW, HAL_LED_MODE_OFF );
        }
      }
      break;
      
/***********************************************************************************************/
    case GAP_LINK_TERMINATED_EVENT:
      {
        
        simpleBLEState = BLE_STATE_IDLE;
        simpleBLEConnHandle = GAP_CONNHANDLE_INIT;
        simpleBLERssi = FALSE;
        simpleBLEDiscState = BLE_DISC_STATE_IDLE;
        BLEDataCharHdl = 0;
        IDValueCharHdl = 0;
        simpleBLEProcedureInProgress = FALSE;
        
        HalLedSet( HAL_LED_BLUE, HAL_LED_MODE_ON );
        HalLedSet( HAL_LED_YELLOW, HAL_LED_MODE_OFF );
        
        txSerialPkt.header.identifier = SERIAL_IDENTIFIER;
        txSerialPkt.header.opCode = APP_CMD_DISCONNECTBLEACK;
        txSerialPkt.header.status = 0x00;
        txSerialPkt.length = 0;
        sendSerialEvt();  
      }
      break;
/***********************************************************************************************/
    case GAP_LINK_PARAM_UPDATE_EVENT:
      break;
      
    default:
      break;
  }
}

/*********************************************************************
 * @fn      pairStateCB
 *
 * @brief   Pairing state callback.
 *
 * @return  none
 */
static void simpleBLECentralPairStateCB( uint16 connHandle, uint8 state, uint8 status )
{
  if ( state == GAPBOND_PAIRING_STATE_STARTED )
  {
//    LCD_WRITE_STRING( "Pairing started", HAL_LCD_LINE_1 );
  }
  else if ( state == GAPBOND_PAIRING_STATE_COMPLETE )
  {
    if ( status == SUCCESS )
    {
//      LCD_WRITE_STRING( "Pairing success", HAL_LCD_LINE_1 );
    }
    else
    {
//      LCD_WRITE_STRING_VALUE( "Pairing fail", status, 10, HAL_LCD_LINE_1 );
    }
  }
  else if ( state == GAPBOND_PAIRING_STATE_BONDED )
  {
    if ( status == SUCCESS )
    {
//      LCD_WRITE_STRING( "Bonding success", HAL_LCD_LINE_1 );
    }
  }
}

/*********************************************************************
 * @fn      simpleBLECentralPasscodeCB
 *
 * @brief   Passcode callback.
 *
 * @return  none
 */
static void simpleBLECentralPasscodeCB( uint8 *deviceAddr, uint16 connectionHandle,
                                        uint8 uiInputs, uint8 uiOutputs )
{
#if (HAL_LCD == TRUE)

  uint32  passcode;
  uint8   str[7];

  // Create random passcode
  LL_Rand( ((uint8 *) &passcode), sizeof( uint32 ));
  passcode %= 1000000;
  
  // Display passcode to user
  if ( uiOutputs != 0 )
  {
    LCD_WRITE_STRING( "Passcode:",  HAL_LCD_LINE_1 );
    LCD_WRITE_STRING( (char *) _ltoa(passcode, str, 10),  HAL_LCD_LINE_2 );
  }
  
  // Send passcode response
  GAPBondMgr_PasscodeRsp( connectionHandle, SUCCESS, passcode );
#endif
}

/*********************************************************************
 * @fn      simpleBLECentralStartDiscovery
 *
 * @brief   Start service discovery.
 *
 * @return  none
 */
static void simpleBLECentralStartDiscovery( void )
{
  uint8 uuid[ATT_BT_UUID_SIZE] = { LO_UINT16(ECGPROFILE_SERV_UUID),
                                   HI_UINT16(ECGPROFILE_SERV_UUID) };
  
  // Initialize cached handles
  simpleBLESvcStartHdl = simpleBLESvcEndHdl  = 0;

  simpleBLEDiscState = BLE_DISC_STATE_SVC;
  
  // Discovery simple BLE service
  GATT_DiscPrimaryServiceByUUID( simpleBLEConnHandle,
                                 uuid,
                                 ATT_BT_UUID_SIZE,
                                 simpleBLETaskId );
}

/*********************************************************************
 * @fn      simpleBLEGATTDiscoveryEvent
 *
 * @brief   Process GATT discovery event
 *
 * @return  none
 */
static void simpleBLEGATTDiscoveryEvent( gattMsgEvent_t *pMsg )
{

  attReadByTypeReq_t req;
  
  if ( simpleBLEDiscState == BLE_DISC_STATE_SVC )
  {
    // Service found, store handles
    if ( pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP &&
         pMsg->msg.findByTypeValueRsp.numInfo > 0 )
    {
      simpleBLESvcStartHdl = pMsg->msg.findByTypeValueRsp.handlesInfo[0].handle;
      simpleBLESvcEndHdl = pMsg->msg.findByTypeValueRsp.handlesInfo[0].grpEndHandle;
    }
    
    // If procedure complete
    if ( ( pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP  && 
           pMsg->hdr.status == bleProcedureComplete ) ||
         ( pMsg->method == ATT_ERROR_RSP ) )
    {
      if ( simpleBLESvcStartHdl != 0 )
      {
        // Discover characteristic
        simpleBLEDiscState = BLE_DISC_STATE_CHAR;
        
        req.startHandle = simpleBLESvcStartHdl;
        req.endHandle = simpleBLESvcEndHdl;
        req.type.len = ATT_BT_UUID_SIZE;
#if 1
        if(GetHalflag == 0)
        {
          req.type.uuid[0] = LO_UINT16(ECGPROFILE_CHAR5_UUID);
          req.type.uuid[1] = HI_UINT16(ECGPROFILE_CHAR5_UUID);
        }
        else if(GetHalflag == 1)
#endif
        {
          req.type.uuid[0] = LO_UINT16(ECGPROFILE_CHAR1_UUID);
          req.type.uuid[1] = HI_UINT16(ECGPROFILE_CHAR1_UUID);
        }
        GATT_ReadUsingCharUUID( simpleBLEConnHandle, &req, simpleBLETaskId );
        simpleBLEProcedureInProgress = TRUE;
      }
    }
  }
  else if ( simpleBLEDiscState == BLE_DISC_STATE_CHAR )
  {
    // Characteristic found, store handle
    if ( pMsg->method == ATT_READ_BY_TYPE_RSP && 
         pMsg->msg.readByTypeRsp.numPairs > 0 )
    {
#if 1
      if(GetHalflag == 0)
      {
        GetHalflag = 1;
        IDValueCharHdl = BUILD_UINT16( pMsg->msg.readByTypeRsp.dataList[0],
                                         pMsg->msg.readByTypeRsp.dataList[1] ); 
        
        simpleBLEProcedureInProgress = FALSE;
      //  osal_set_event( simpleBLETaskId, START_DISCOVERY_EVT);
        osal_start_timerEx( simpleBLETaskId, START_DISCOVERY_EVT, DEFAULT_NOTIFY_DELAY );
      }
      else if(GetHalflag == 1)
#endif
      {
         GetHalflag = 0;
         BLEDataCharHdl  = BUILD_UINT16( pMsg->msg.readByTypeRsp.dataList[0],
                                         pMsg->msg.readByTypeRsp.dataList[1] );
         
          simpleBLEProcedureInProgress = FALSE;
         // osal_set_event( simpleBLETaskId, START_NOTIFY_EVT);
          osal_start_timerEx( simpleBLETaskId, START_NOTIFY_EVT, 300 );
      }
    }
    simpleBLEDiscState = BLE_DISC_STATE_IDLE;

    
  }    
}


/*********************************************************************
 * @fn      simpleBLEFindSvcUuid
 *
 * @brief   Find a given UUID in an advertiser's service UUID list.
 *
 * @return  TRUE if service UUID found
 */
static bool simpleBLEFindSvcUuid( uint16 uuid, uint8 *pData, uint8 dataLen )
{
  uint8 adLen;
  uint8 adType;
  uint8 *pEnd;
  
  pEnd = pData + dataLen - 1;
  
  // While end of data not reached
  while ( pData < pEnd )
  {
    // Get length of next AD item
    adLen = *pData++;
    if ( adLen > 0 )
    {
      adType = *pData;
      
      // If AD type is for 16-bit service UUID
      if ( adType == GAP_ADTYPE_16BIT_MORE || adType == GAP_ADTYPE_16BIT_COMPLETE )
      {
        pData++;
        adLen--;
        
        // For each UUID in list
        while ( adLen >= 2 && pData < pEnd )
        {
          // Check for match
          if ( pData[0] == LO_UINT16(uuid) && pData[1] == HI_UINT16(uuid) )
          {
            // Match found
            return TRUE;
          }
          
          // Go to next
          pData += 2;
          adLen -= 2;
        }
        
        // Handle possible erroneous extra byte in UUID list
        if ( adLen == 1 )
        {
          pData++;
        }
      }
      else
      {
        // Go to next item
        pData += adLen;
      }
    }
  }
  
  // Match not found
  return FALSE;
}

/*********************************************************************
 * @fn      simpleBLEAddDeviceInfo
 *
 * @brief   Add a device to the device discovery result list
 *
 * @return  none
 */
static void simpleBLEAddDeviceInfo( uint8 *pAddr, uint8 addrType )
{
  uint8 i;
  
  // If result count not at max
  if ( simpleBLEScanRes < DEFAULT_MAX_SCAN_RES )
  {
    // Check if device is already in scan results
    for ( i = 0; i < simpleBLEScanRes; i++ )
    {
      if ( osal_memcmp( pAddr, simpleBLEDevList[i].addr , B_ADDR_LEN ) )
      {
        return;
      }
    }
    
    // Add addr to scan result list
    osal_memcpy( simpleBLEDevList[simpleBLEScanRes].addr, pAddr, B_ADDR_LEN );
    simpleBLEDevList[simpleBLEScanRes].addrType = addrType;
    
    // Increment scan result count
    simpleBLEScanRes++;
  }
}

/*********************************************************************
 * @fn      bdAddr2Str
 *
 * @brief   Convert Bluetooth address to string
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

/*********************************************************************
*********************************************************************/
