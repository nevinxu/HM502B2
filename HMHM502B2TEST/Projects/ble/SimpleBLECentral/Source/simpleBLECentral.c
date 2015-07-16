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
#include "hal_flash.h"

/*********************************************************************
 * MACROS
 */

// Length of bd addr as a string
#define B_ADDR_STR_LEN                        15

/*********************************************************************
 * CONSTANTS
 */

// Maximum number of scan responses
#define DEFAULT_MAX_SCAN_RES                  8    //最大搜索设备数

// Scan duration in ms
#define DEFAULT_SCAN_DURATION                 1000

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
#define DEFAULT_RSSI_PERIOD                   10000

// Whether to enable automatic parameter update request when a connection is formed
#define DEFAULT_ENABLE_UPDATE_REQUEST         TRUE

// Minimum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_UPDATE_MIN_CONN_INTERVAL      50

// Maximum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_UPDATE_MAX_CONN_INTERVAL      100

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
#define DEFAULT_SVC_DISCOVERY_DELAY             500
#define DEFAULT_SCAN_DELAY                      3000
#define DEFAULT_CONNECT_DELAY                   500
#define DEFAULT_NOTIFY_DELAY                    300
#define DEFAULT_PERIOD_DELAY                   6000

// TRUE to filter discovery results on desired service UUID
#define DEFAULT_DEV_DISC_BY_SVC_UUID          FALSE

#define PairMACPage                     250
#define PairMACAddr                     PairMACPage*512


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
uint8 simpleBLEState = BLE_STATE_IDLE;

// Discovery state
static uint8 simpleBLEDiscState = BLE_DISC_STATE_IDLE;

// Discovered service start and end handle
static uint16 simpleBLESvcStartHdl = 0;
static uint16 simpleBLESvcEndHdl = 0;


// Value to write
static uint8 simpleBLECharVal = 0;

// Value read/write toggle
static bool simpleBLEDoWrite = FALSE;

// GATT read/write procedure state
static bool simpleBLEProcedureInProgress = FALSE;


extern BLEPacket_t  rxSerialPkt;
extern BLEPacket_t  txSerialPkt;

uint8 AutoConnectFlag = 0;   //自动连接初始化时能
uint8 DeviceMode = 0;     //303模式
//uint8 DeviceMode = 1;     //测试模式

uint8 IDValue[10] = {"D000000000"};
uint8 CentralMAC[6];
uint8 ECGPatchMAC[6];    //连接心电补丁MAC地址

uint8 PairMAC[6] = {0xE6,0x39,0x00,0x0B,0x0E,0x00};
uint8 PairFlag = 0;   //配对功能使能

long  ReceivePackageNum;
long  LostPackageNum;
int LastPackageNum = -1;



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
  
//  HalFlashErase(PairMACPage);
//  HalFlashWrite(PairMACAddr, PairMAC, 6);
  
  HalFlashRead(PairMACPage, 0, PairMAC, 6);

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
  if ( events & START_DEVICE_EVT ) //开始事件
  {
    // Start the Device
    VOID GAPCentralRole_StartDevice( (gapCentralRoleCB_t *) &simpleBLERoleCB );

    // Register with bond manager after starting device
    GAPBondMgr_Register( (gapBondCBs_t *) &simpleBLEBondCB );
    osal_start_timerEx( simpleBLETaskId, simpleBLE_PERIODIC_EVT, 500 );
    return ( events ^ START_DEVICE_EVT );
  }
/********************************************************************************/
  if ( events & START_SCAN_EVT )    //搜索事件
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
  if(events & START_CONNECT_EVT)        //连接事件
  {
    static uint8 addrType;
    static uint8 *peerAddr;
// Connect or disconnect
    if ( simpleBLEState == BLE_STATE_IDLE )
    {
      // if there is a scan result
      if ( simpleBLEScanRes > 0 )
      {
        // connect to current device in scan result
        if(memcmp(simpleBLEDevList[simpleBLEScanIdx].addr,PairMAC,6) == 0)
        {
          peerAddr = PairMAC;
          memcpy(ECGPatchMAC,PairMAC,6);
          addrType = simpleBLEDevList[simpleBLEScanIdx].addrType;
        
          simpleBLEState = BLE_STATE_CONNECTING;
          
          GAPCentralRole_EstablishLink( DEFAULT_LINK_HIGH_DUTY_CYCLE,
                                        DEFAULT_LINK_WHITE_LIST,
                                        addrType, peerAddr );
        }
        else
        {
          txSerialPkt.header.identifier = SERIAL_IDENTIFIER;
          txSerialPkt.header.opCode = APP_CMD_CONNECTBLEACK;
          txSerialPkt.header.status = 0x00;
          txSerialPkt.length = 1;
          txSerialPkt.data[0] = 0;
          sendSerialEvt(); 
        }
      }
    }
    return ( events ^ START_CONNECT_EVT );
  }
   if ( events & simpleBLE_PERIODIC_EVT ) //周期事件
  {
    osal_start_timerEx( simpleBLETaskId, simpleBLE_PERIODIC_EVT, DEFAULT_PERIOD_DELAY );
    
    if ( simpleBLEState == BLE_STATE_IDLE )  //未连接且为自动搜索连接模式
    {
      if(AutoConnectFlag == 1)
      {
        txSerialPkt.header.identifier = rxSerialPkt.header.identifier;
        txSerialPkt.header.opCode = APP_CMD_ADVERTISEBEGIN;    //开始搜索蓝牙设备
        txSerialPkt.header.status = 0x00;
        txSerialPkt.length = 0;
        sendSerialEvt();
        osal_set_event( simpleBLETaskId, START_SCAN_EVT);
      }
      
      if(simpleBLERssi)
      {
        simpleBLERssi = FALSE;
        GAPCentralRole_CancelRssi(simpleBLEConnHandle);
      }
      
    }
    else if(simpleBLEState == BLE_STATE_CONNECTED)  //连接状态 
    {
      if ( !simpleBLERssi )  //设置获取rssi周期事件
      {
        simpleBLERssi = TRUE;
        GAPCentralRole_StartRssi( simpleBLEConnHandle, DEFAULT_RSSI_PERIOD );
      }
      
      if(ReceivePackageNum>0)
      {
        txSerialPkt.header.identifier = rxSerialPkt.header.identifier;
        txSerialPkt.header.opCode = APP_CMD_RECEIVEPACKAGENUMVALUE;    //
        txSerialPkt.header.status = 0x00;
        txSerialPkt.length = 8;
        memcpy(txSerialPkt.data,&ReceivePackageNum,4);
        memcpy(&txSerialPkt.data[4],&LostPackageNum,4);
        
        sendSerialEvt();
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
  
  if ( events & START_DISCONNECT_EVT )  //断开事件
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
  if ( events & START_RECEIVEECGDATA_EVT )  //
  {
    if ( simpleBLEState == BLE_STATE_CONNECTED )
    { 
      SendCommand2Peripheral(APP_CMD_RECEIVEECGDATA,0,0);   //向心电补丁请求发送心电数据
    }
    return (events ^ START_RECEIVEECGDATA_EVT);
  }
  if ( events & START_GET0MVVALUE_EVT )  //
  {
    if ( simpleBLEState == BLE_STATE_CONNECTED )
    { 
      SendCommand2Peripheral(APP_CMD_GET0MVVALUE,0,0);
      osal_start_timerEx( simpleBLETaskId, START_RECEIVEECGDATA_EVT,1000);
    }
    return (events ^ START_GET0MVVALUE_EVT);
  }    

    if ( events & START_GETECGPatchMACVALUE_EVT )  //
  {
    if ( simpleBLEState == BLE_STATE_CONNECTED )
    { 
      txSerialPkt.header.identifier = rxSerialPkt.header.identifier;
      txSerialPkt.header.opCode = APP_CMD_ECGPatchMACACK;
      txSerialPkt.header.status = 0x00;
      txSerialPkt.length = 6; 
      osal_memcpy(txSerialPkt.data,ECGPatchMAC,6);
      sendSerialEvt();
      
      osal_start_timerEx( simpleBLETaskId, START_RECEIVEECGDATA_EVT,1000);
    }
    return (events ^ START_GETECGPatchMACVALUE_EVT);
  } 
  if ( events & START_STOREPAIRMAC_EVT ) 
  {
          HalFlashErase(PairMACPage);
      HalFlashWrite(PairMACAddr, PairMAC, 6);
      memset(PairMAC,0,6);
      HalFlashRead(PairMACPage, 0, PairMAC, 6);
    return (events ^ START_STOREPAIRMAC_EVT);
  }
  if ( events & START_ECGRECEIVEREQ_EVT ) 
  {
    SendCommand2Peripheral(APP_CMD_ECGDATAOKREQ,0,0);
    return (events ^ START_ECGRECEIVEREQ_EVT);
  }
  
  // Discard unknown events
  return 0;
}

void SendCommand2Peripheral(uint8 command,uint8 *data,uint8 length)
{
        attHandleValueNoti_t req; 

#if 1
        req.len = 4 + length;
        req.value[0] = SERIAL_IDENTIFIER;
        req.value[1] = command;
        req.value[2] = 0;
        req.value[3] = length;
        if(length > 0)
        {
          osal_memcpy(&req.value[4],data,length);
        }
#endif
        GATT_Notification(simpleBLEConnHandle,&req,false);
}  

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
  if ((pMsg->method == ATT_HANDLE_VALUE_NOTI) )   //通知事件
  {
    if(DeviceMode == 0)  //HM303接收数据格式
    {
     //   uint8 valueReadSize = pMsg->msg.handleValueNoti.len; 
        static uint8 rxpacketnum = 0;
        static uint8 serial_txbuffer[100];
        {
          osal_memcpy(&serial_txbuffer[3+(rxpacketnum*10)],&pMsg->msg.handleValueNoti.value[8],10);
          rxpacketnum++;
          if(rxpacketnum >= 8)
          {
           // uint16 batteryvalue;
            rxpacketnum = 0;
            serial_txbuffer[0] = '[';
            serial_txbuffer[83] = ']';
            serial_txbuffer[1] = 84;
          //  batteryvalue = pMsg->msg.handleValueNoti.value[7] + (pMsg->msg.handleValueNoti.value[6]<<8);
            serial_txbuffer[2] = ((pMsg->msg.handleValueNoti.value[5]&0x01)<<7)+ (pMsg->msg.handleValueNoti.value[6]&0x7f);
            NPI_WriteTransport(serial_txbuffer,84);
          }
        }
    }
    else if(DeviceMode == 1) //测试上位机接收数据格式
    { 
      if(simpleBLEProcedureInProgress == FALSE )
        {
          if(pMsg->msg.handleValueNoti.value[0] == SERIAL_IDENTIFIER)
          {
            if(pMsg->msg.handleValueNoti.value[2] == 0)
            {
              if(pMsg->msg.handleValueNoti.value[1] == APP_CMD_ECGPATCHIDACK)
              {
                HalUARTWrite(NPI_UART_PORT, pMsg->msg.handleValueNoti.value, pMsg->msg.handleValueNoti.len);
              }
              else if(pMsg->msg.handleValueNoti.value[1] == APP_CMD_DATASEND)
              {
               // if(pMsg->msg.handleValueNoti.len == 0x12)     // 数据完整性
                {
                  HalUARTWrite(NPI_UART_PORT, pMsg->msg.handleValueNoti.value, 0x12);
               //   osal_set_event( simpleBLETaskId, START_ECGRECEIVEREQ_EVT);
             //     SendCommand2Peripheral(APP_CMD_ECGDATAOKREQ,0,0);
                  
                  ReceivePackageNum++;
                  
                  if (LastPackageNum != -1)  //不是第一次  
                  {
                    int buffer = LastPackageNum + 1;
                    if(buffer == 256)
                    {
                      buffer = 0;
                    }
                    if (pMsg->msg.handleValueNoti.value[4] != buffer)
                    {
                      LostPackageNum++;
                    }
                  }
                  LastPackageNum = pMsg->msg.handleValueNoti.value[4];
                }
              }
              else if(pMsg->msg.handleValueNoti.value[1] == APP_CMD_RECEIVEECGDATAACK)
              {
                ReceivePackageNum = 0;
                LostPackageNum = 0;
                LastPackageNum = -1;
                HalUARTWrite(NPI_UART_PORT, pMsg->msg.handleValueNoti.value, pMsg->msg.handleValueNoti.len);
              }
              else if(pMsg->msg.handleValueNoti.value[1] == APP_CMD_SET1MVVALUEACK)
              {
                HalUARTWrite(NPI_UART_PORT, pMsg->msg.handleValueNoti.value, pMsg->msg.handleValueNoti.len);
              }
              else if(pMsg->msg.handleValueNoti.value[1] == APP_CMD_SET0MVVALUEACK)
              {
                HalUARTWrite(NPI_UART_PORT, pMsg->msg.handleValueNoti.value, pMsg->msg.handleValueNoti.len);
              }
              else if(pMsg->msg.handleValueNoti.value[1] == APP_CMD_GET0MVVALUEACK)
              {
                HalUARTWrite(NPI_UART_PORT, pMsg->msg.handleValueNoti.value, pMsg->msg.handleValueNoti.len);
              }
              else if(pMsg->msg.handleValueNoti.value[1] == APP_CMD_GET1MVVALUEACK)
              {
                HalUARTWrite(NPI_UART_PORT, pMsg->msg.handleValueNoti.value, pMsg->msg.handleValueNoti.len);
              }
              else if(pMsg->msg.handleValueNoti.value[1] == APP_CMD_GET0MVVALUEACK)
              {
                HalUARTWrite(NPI_UART_PORT, pMsg->msg.handleValueNoti.value, pMsg->msg.handleValueNoti.len);
              }
              else if(pMsg->msg.handleValueNoti.value[1] == APP_CMD_GET1MVVALUEACK)
              {
                HalUARTWrite(NPI_UART_PORT, pMsg->msg.handleValueNoti.value, pMsg->msg.handleValueNoti.len);
              }
              else if(pMsg->msg.handleValueNoti.value[1] == APP_CMD_ECGPATCHHARDVERSIONREQ)
              {
                HalUARTWrite(NPI_UART_PORT, pMsg->msg.handleValueNoti.value, pMsg->msg.handleValueNoti.len);
              }
              else if(pMsg->msg.handleValueNoti.value[1] == APP_CMD_ECGPATCHSOFTVERSIONREQ)
              {
                HalUARTWrite(NPI_UART_PORT, pMsg->msg.handleValueNoti.value, pMsg->msg.handleValueNoti.len);
              }
              else if(pMsg->msg.handleValueNoti.value[1] == APP_CMD_STOPRECEIVEECGDATAACK)
              {
                HalUARTWrite(NPI_UART_PORT, pMsg->msg.handleValueNoti.value, pMsg->msg.handleValueNoti.len);
              }
              else if(pMsg->msg.handleValueNoti.value[1] == APP_CMD_SETECGPATCHIDACK)
              {
                HalUARTWrite(NPI_UART_PORT, pMsg->msg.handleValueNoti.value, pMsg->msg.handleValueNoti.len);
              }
              else if(pMsg->msg.handleValueNoti.value[1] == APP_CMD_STARTSET1MVVALUEACK)
              {
                HalUARTWrite(NPI_UART_PORT, pMsg->msg.handleValueNoti.value, pMsg->msg.handleValueNoti.len);
              }
              else if(pMsg->msg.handleValueNoti.value[1] == APP_CMD_STARTSET0MVVALUEACK)
              {
                HalUARTWrite(NPI_UART_PORT, pMsg->msg.handleValueNoti.value, pMsg->msg.handleValueNoti.len);
              }
              
            }
          }
        }
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
  if(simpleBLEState == BLE_STATE_CONNECTED)
  {
#if 1
    if(DeviceMode == 1)  //上位机测试模式下
    {
        txSerialPkt.header.identifier = SERIAL_IDENTIFIER;
        txSerialPkt.header.opCode = APP_CMD_RSSIVALUE;
        txSerialPkt.header.status = 0x00;
        txSerialPkt.length = 1;
        txSerialPkt.data[0] = rssi;
        sendSerialEvt();
    }
#endif
  }
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
        osal_memcpy(CentralMAC,pEvent->initDone.devAddr,6);   //获取主机MAC地址
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
#if 1
        txSerialPkt.header.identifier = SERIAL_IDENTIFIER;
        txSerialPkt.header.opCode = APP_CMD_ADVERTISEOVER;
        txSerialPkt.header.status = 0x00;
        txSerialPkt.length = 0;
        sendSerialEvt();
#endif
#if 1
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
      //  sendSerialEvt();
        
        // initialize scan index to last device
      //  simpleBLEScanIdx = 0;   //选择第一个index
   //     osal_memcpy(ECGPatchMAC,simpleBLEDevList[0].addr,6);
        
        if(simpleBLEScanRes > 0)   //
        {
          if(PairFlag == 1)   //配对功能使能
          {
            for(int i = 0;i<simpleBLEScanRes; i++)
            {
                if(memcmp(simpleBLEDevList[i].addr, PairMAC,6) == 0)
                {
                  osal_memcpy(ECGPatchMAC,simpleBLEDevList[i].addr,6);
                  simpleBLEScanIdx = i;
                  osal_start_timerEx( simpleBLETaskId, START_CONNECT_EVT,500);
                  break;
                }
            }
          }
          else
          {
            if(AutoConnectFlag == 1)  //自动搜索连接
            {
              
            }
          }
        }
#endif
      }
      break;
      
/***********************************************************************************************/ 
    case GAP_LINK_ESTABLISHED_EVENT:   //连接事件回调函数
      {
        if ( pEvent->gap.hdr.status == SUCCESS )   //连接成功   
        {          
          simpleBLEState = BLE_STATE_CONNECTED;   //连接状态    
          simpleBLEConnHandle = pEvent->linkCmpl.connectionHandle;
          
          simpleBLEProcedureInProgress = TRUE;      // 

          txSerialPkt.header.identifier = SERIAL_IDENTIFIER;
          txSerialPkt.header.opCode = APP_CMD_CONNECTBLEACK;
          txSerialPkt.header.status = 0x00;
          txSerialPkt.length = 1;
          txSerialPkt.data[0] = 1;
          sendSerialEvt();
          
       //   osal_memcpy(ECGPatchMAC,simpleBLEDevList[simpleBLEScanIdx].addr,6);
          
          HalLedSet( HAL_LED_YELLOW, HAL_LED_MODE_ON );    
          HalLedSet( HAL_LED_BLUE, HAL_LED_MODE_OFF );
          simpleBLEProcedureInProgress = FALSE;
          
        //  osal_start_timerEx( simpleBLETaskId, START_GET0MVVALUE_EVT,500);
       //   osal_start_timerEx( simpleBLETaskId, START_GETECGPatchMACVALUE_EVT,500);
          
       //   SendCommand2Peripheral(APP_CMD_RECEIVEECGDATA,0,0);   //向心电补丁请求发送心电数据
          
        }
        else    //连接失败   
        {     
          simpleBLEState = BLE_STATE_IDLE;
          simpleBLEProcedureInProgress = FALSE;
          
          txSerialPkt.header.identifier = SERIAL_IDENTIFIER;
          txSerialPkt.header.opCode = APP_CMD_CONNECTBLEACK;
          txSerialPkt.header.status = 0x00;
          txSerialPkt.length = 1;
          txSerialPkt.data[0] = 0;
          sendSerialEvt();    
          
          HalLedSet( HAL_LED_BLUE, HAL_LED_MODE_ON ); 
          HalLedSet( HAL_LED_YELLOW, HAL_LED_MODE_OFF );
          osal_memset(ECGPatchMAC,0,6);   //清除
        }
      }
      break;
      
/***********************************************************************************************/
    case GAP_LINK_TERMINATED_EVENT:   //断开事件回调函数
      {
        
        simpleBLEState = BLE_STATE_IDLE;
        simpleBLEConnHandle = GAP_CONNHANDLE_INIT;
        simpleBLERssi = FALSE;
        simpleBLEDiscState = BLE_DISC_STATE_IDLE;
        simpleBLEProcedureInProgress = FALSE;
        
        HalLedSet( HAL_LED_BLUE, HAL_LED_MODE_ON );
        HalLedSet( HAL_LED_YELLOW, HAL_LED_MODE_OFF );
        
        txSerialPkt.header.identifier = SERIAL_IDENTIFIER;
        txSerialPkt.header.opCode = APP_CMD_DISCONNECTBLEACK;
        txSerialPkt.header.status = 0x00;
        txSerialPkt.length = 0;
        sendSerialEvt(); 
        osal_memset(ECGPatchMAC,0,6);   //清除 
      }
      break;
/***********************************************************************************************/
    case GAP_LINK_PARAM_UPDATE_EVENT:
      break;
      
    default:
      break;
  }
#if 1
  if(pEvent->deviceInfo.eventType == GAP_ADRPT_ADV_IND )//判断是否是广播包

  {
   // osal_memset(ECGPatchMAC,0,6); 

  }

  else if(pEvent->deviceInfo.eventType == GAP_ADRPT_SCAN_RSP)//判断是否是扫描Rsp

  {
#if 1
    txSerialPkt.header.identifier = SERIAL_IDENTIFIER;
    txSerialPkt.header.opCode = APP_CMD_ADVERTISEACK;
    txSerialPkt.header.status = 0x00;
    txSerialPkt.length = pEvent->deviceInfo.dataLen + 4;
    osal_memcpy(txSerialPkt.data,pEvent->deviceInfo.addr,txSerialPkt.length);
    sendSerialEvt();
#endif
  }
#endif
  
  
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
;  
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
