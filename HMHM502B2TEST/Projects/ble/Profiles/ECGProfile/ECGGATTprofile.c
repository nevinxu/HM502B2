#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include "ECGGATTprofile.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define SERVAPP_NUM_ATTR_SUPPORTED        19

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Simple GATT Profile Service UUID: 0xFFF0
CONST uint8 ECGProfileServUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ECGPROFILE_SERV_UUID), HI_UINT16(ECGPROFILE_SERV_UUID)
};

// Characteristic 1 UUID: 0xFFF1
CONST uint8 ECGProfilechar1UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ECGPROFILE_CHAR1_UUID), HI_UINT16(ECGPROFILE_CHAR1_UUID)
};

// Characteristic 2 UUID: 0xFFF2
CONST uint8 ECGProfilechar2UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ECGPROFILE_CHAR2_UUID), HI_UINT16(ECGPROFILE_CHAR2_UUID)
};

// Characteristic 3 UUID: 0xFFF3
CONST uint8 ECGProfilechar3UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ECGPROFILE_CHAR3_UUID), HI_UINT16(ECGPROFILE_CHAR3_UUID)
};

// Characteristic 4 UUID: 0xFFF4
CONST uint8 ECGProfilechar4UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ECGPROFILE_CHAR4_UUID), HI_UINT16(ECGPROFILE_CHAR4_UUID)
};

// Characteristic 5 UUID: 0xFFF5
CONST uint8 ECGProfilechar5UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ECGPROFILE_CHAR5_UUID), HI_UINT16(ECGPROFILE_CHAR5_UUID)
};

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

static ECGProfileCBs_t *ECGProfile_AppCBs = NULL;

/*********************************************************************
 * Profile Attributes - variables
 */

// Simple Profile Service attribute
static CONST gattAttrType_t ECGProfileService = { ATT_BT_UUID_SIZE, ECGProfileServUUID };


// Simple Profile Characteristic 1 Properties
static uint8 ECGProfileChar1Props = GATT_PROP_READ | GATT_PROP_WRITE | GATT_PROP_NOTIFY;

// Characteristic 1 Value
static uint8 ECGProfileChar1[ECGPROFILE_CHAR1_LEN] = { 0, 0, 0, 0, 0 };

// Simple Profile Characteristic 1 User Description
static uint8 ECGProfileChar1UserDesp[17] = "ECG Data\0";

static gattCharCfg_t ECGProfileChar1Config[GATT_MAX_NUM_CONN] ;


// Simple Profile Characteristic 2 Properties
static uint8 ECGProfileChar2Props = GATT_PROP_READ;

// Characteristic 2 Value
static uint8 ECGProfileChar2 = 0;

// Simple Profile Characteristic 2 User Description
static uint8 ECGProfileChar2UserDesp[17] = "Characteristic 2\0";


// Simple Profile Characteristic 3 Properties
static uint8 ECGProfileChar3Props = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic 3 Value
static uint8 ECGProfileChar3 = 0;

// Simple Profile Characteristic 3 User Description
static uint8 ECGProfileChar3UserDesp[17] = "ECG Rate\0";


// Simple Profile Characteristic 4 Properties
static uint8 ECGProfileChar4Props = GATT_PROP_NOTIFY;

// Characteristic 4 Value
static uint8 ECGProfileChar4 = 0;

// Simple Profile Characteristic 4 Configuration Each client has its own
// instantiation of the Client Characteristic Configuration. Reads of the
// Client Characteristic Configuration only shows the configuration for
// that client and writes only affect the configuration of that client.
static gattCharCfg_t ECGProfileChar4Config[GATT_MAX_NUM_CONN];
                                        
// Simple Profile Characteristic 4 User Description
static uint8 ECGProfileChar4UserDesp[17] = "Characteristic 4\0";


// Simple Profile Characteristic 5 Properties
static uint8 ECGProfileChar5Props = GATT_PROP_WRITE | GATT_PROP_READ | GATT_PROP_NOTIFY;

// Characteristic 5 Value
static uint8 ECGProfileChar5[ECGPROFILE_CHAR5_LEN];

static gattCharCfg_t ECGProfileChar5Config[GATT_MAX_NUM_CONN] = {0x01,0x00};

// Simple Profile Characteristic 5 User Description
static uint8 ECGProfileChar5UserDesp[17] = "ID_Value\0";


/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t ECGProfileAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] = 
{
  // Simple Profile Service
  { 
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&ECGProfileService            /* pValue */
  },

    // Characteristic 1 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &ECGProfileChar1Props 
    },

      // Characteristic Value 1
      { 
        { ATT_BT_UUID_SIZE, ECGProfilechar1UUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        ECGProfileChar1 
      },
      // Characteristic 1 configuration
      { 
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        (uint8 *)ECGProfileChar1Config 
      },
      // Characteristic 1 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        ECGProfileChar1UserDesp 
      },      

    // Characteristic 2 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &ECGProfileChar2Props 
    },

      // Characteristic Value 2
      { 
        { ATT_BT_UUID_SIZE, ECGProfilechar2UUID },
        GATT_PERMIT_READ, 
        0, 
        &ECGProfileChar2 
      },

      // Characteristic 2 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        ECGProfileChar2UserDesp 
      },           
      
    // Characteristic 3 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &ECGProfileChar3Props 
    },

      // Characteristic Value 3
      { 
        { ATT_BT_UUID_SIZE, ECGProfilechar3UUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        &ECGProfileChar3 
      },

      // Characteristic 3 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        ECGProfileChar3UserDesp 
      },

    // Characteristic 4 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &ECGProfileChar4Props 
    },

      // Characteristic Value 4
      { 
        { ATT_BT_UUID_SIZE, ECGProfilechar4UUID },
        GATT_PERMIT_READ, 
        0, 
        &ECGProfileChar4 
      },

      // Characteristic 4 configuration
      { 
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        (uint8 *)ECGProfileChar4Config 
      },
      
      // Characteristic 4 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        ECGProfileChar4UserDesp 
      },
      
    // Characteristic 5 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &ECGProfileChar5Props 
    },

      // Characteristic Value 5
      { 
        { ATT_BT_UUID_SIZE, ECGProfilechar5UUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        ECGProfileChar5 
      },
      // Characteristic 5 configuration
      { 
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        (uint8 *)ECGProfileChar5Config 
      },
      // Characteristic 5 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        ECGProfileChar5UserDesp 
      },


};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8 ECGProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen );
static bStatus_t ECGProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset );

static void ECGProfile_HandleConnStatusCB( uint16 connHandle, uint8 changeType );


/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t ECGProfileCBs =
{
  ECGProfile_ReadAttrCB,  // Read callback function pointer
  ECGProfile_WriteAttrCB, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SimpleProfile_AddService
 *
 * @brief   Initializes the Simple Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t SimpleProfile_AddService( uint32 services )
{
  uint8 status = SUCCESS;

  // Initialize Client Characteristic Configuration attributes
 
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, ECGProfileChar1Config );
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, ECGProfileChar5Config );
  // Register with Link DB to receive link status change callback
  VOID linkDB_Register( ECGProfile_HandleConnStatusCB );  
  
  if ( services & ECGPROFILE_SERVICE )
  {
    // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService( ECGProfileAttrTbl, 
                                          GATT_NUM_ATTRS( ECGProfileAttrTbl ),
                                          &ECGProfileCBs );
  }
  
  return ( status );
}


/*********************************************************************
 * @fn      SimpleProfile_RegisterAppCBs
 *
 * @brief   Registers the application callback function. Only call 
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t SimpleProfile_RegisterAppCBs( ECGProfileCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    ECGProfile_AppCBs = appCallbacks;
    
    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}
  

/*********************************************************************
 * @fn      SimpleProfile_SetParameter
 *
 * @brief   Set a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to right
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t SimpleProfile_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case ECGPROFILE_CHAR1:
      if ( len <= ECGPROFILE_CHAR1_LEN ) 
      {
        VOID osal_memcpy( ECGProfileChar1, value, ECGPROFILE_CHAR1_LEN );
        GATTServApp_ProcessCharCfg( ECGProfileChar1Config, ECGProfileChar1, FALSE,
                                    ECGProfileAttrTbl, GATT_NUM_ATTRS( ECGProfileAttrTbl ),
                                    INVALID_TASK_ID );
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case ECGPROFILE_CHAR2:
      if ( len == sizeof ( uint8 ) ) 
      {
        ECGProfileChar2 = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case ECGPROFILE_CHAR3:
      if ( len == sizeof ( uint8 ) ) 
      {
        ECGProfileChar3 = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case ECGPROFILE_CHAR4:
      if ( len == sizeof ( uint8 ) ) 
      {
        ECGProfileChar4 = *((uint8*)value);
        
        // See if Notification has been enabled
        GATTServApp_ProcessCharCfg( ECGProfileChar4Config, &ECGProfileChar4, FALSE,
                                    ECGProfileAttrTbl, GATT_NUM_ATTRS( ECGProfileAttrTbl ),
                                    INVALID_TASK_ID );
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case ECGPROFILE_CHAR5:
      if ( len <= ECGPROFILE_CHAR5_LEN ) 
      {
        VOID osal_memcpy( ECGProfileChar5, value, ECGPROFILE_CHAR5_LEN );
                // See if Notification has been enabled
        GATTServApp_ProcessCharCfg( ECGProfileChar5Config, ECGProfileChar5, FALSE,
                                    ECGProfileAttrTbl, GATT_NUM_ATTRS( ECGProfileAttrTbl ),
                                    INVALID_TASK_ID );
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn      SimpleProfile_GetParameter
 *
 * @brief   Get a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t SimpleProfile_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case ECGPROFILE_CHAR1:
      VOID osal_memcpy( value, ECGProfileChar1, ECGPROFILE_CHAR1_LEN );
      break;

    case ECGPROFILE_CHAR2:
      *((uint8*)value) = ECGProfileChar2;
      break;      

    case ECGPROFILE_CHAR3:
      *((uint8*)value) = ECGProfileChar3;
      break;  

    case ECGPROFILE_CHAR4:
      *((uint8*)value) = ECGProfileChar4;
      break;

    case ECGPROFILE_CHAR5:
      VOID osal_memcpy( value, ECGProfileChar5, ECGPROFILE_CHAR5_LEN );
      break;      
      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn          ECGProfile_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 *
 * @return      Success or Failure
 */
static uint8 ECGProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen )
{
  bStatus_t status = SUCCESS;

  // If attribute permissions require authorization to read, return error
  if ( gattPermitAuthorRead( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  // Make sure it's not a blob operation (no attributes in the profile are long)
  if ( offset > 0 )
  {
    return ( ATT_ERR_ATTR_NOT_LONG );
  }
 
  //if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {
      // No need for "GATT_SERVICE_UUID" or "GATT_CLIENT_CHAR_CFG_UUID" cases;
      // gattserverapp handles those reads

      // characteristics 1 and 2 have read permissions
      // characteritisc 3 does not have read permissions; therefore it is not
      //   included here
      // characteristic 4 does not have read permissions, but because it
      //   can be sent as a notification, it is included here
      case ECGPROFILE_CHAR1_UUID:
        *pLen = ECGPROFILE_CHAR1_LEN;
        VOID osal_memcpy( pValue, pAttr->pValue, ECGPROFILE_CHAR1_LEN );
        break;
      case ECGPROFILE_CHAR2_UUID:
      case ECGPROFILE_CHAR3_UUID:
      case ECGPROFILE_CHAR4_UUID:
        *pLen = 1;
        pValue[0] = *pAttr->pValue;
        break;

      case ECGPROFILE_CHAR5_UUID:
        *pLen = ECGPROFILE_CHAR5_LEN;
        VOID osal_memcpy( pValue, pAttr->pValue, ECGPROFILE_CHAR5_LEN );
        break;
        
      default:
        // Should never get here! (characteristics 3 and 4 do not have read permissions)
        *pLen = 0;
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
#if 0
  else
  {
    // 128-bit UUID
    *pLen = 0;
    status = ATT_ERR_INVALID_HANDLE;
  }
#endif

  return ( status );
}

/*********************************************************************
 * @fn      ECGProfile_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 *
 * @return  Success or Failure
 */
static bStatus_t ECGProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset )
{
  bStatus_t status = SUCCESS;
  uint8 notifyApp = 0xFF;
  
  // If attribute permissions require authorization to write, return error
  if ( gattPermitAuthorWrite( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
//  if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {
      case ECGPROFILE_CHAR1_UUID:
      case ECGPROFILE_CHAR2_UUID:
      case ECGPROFILE_CHAR3_UUID:
      case ECGPROFILE_CHAR4_UUID:
      case ECGPROFILE_CHAR5_UUID:
        //Validate the value
        // Make sure it's not a blob oper
        if ( offset == 0 )
        {
          ;
#if 0
          if ( len != 1 )
          {
            status = ATT_ERR_INVALID_VALUE_SIZE;
          }
#endif
        }
        else
        {
          status = ATT_ERR_ATTR_NOT_LONG;
        }
        
        //Write the value
        if ( status == SUCCESS )
        {
          uint8 *pCurValue = (uint8 *)pAttr->pValue;        
          //*pCurValue = pValue[0];
          VOID osal_memcpy( pCurValue, pValue, ECGPROFILE_CHAR1_LEN );

          if( pAttr->pValue == ECGProfileChar1 )
          {
            notifyApp = ECGPROFILE_CHAR1;        
          }
          else if( pAttr->pValue == &ECGProfileChar2 )
          {
            notifyApp = ECGPROFILE_CHAR2;           
          }
          else if( pAttr->pValue == &ECGProfileChar3 )
          {
            notifyApp = ECGPROFILE_CHAR3;           
          }
          else if( pAttr->pValue == ECGProfileChar5 )
          {
            notifyApp = ECGPROFILE_CHAR5;           
          }
        }
             
        break;

      case GATT_CLIENT_CHAR_CFG_UUID:
        status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
                                                 offset, GATT_CLIENT_CFG_NOTIFY );
        break;
        
      default:
        // Should never get here! (characteristics 2 and 4 do not have write permissions)
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
#if 0
  else
  {
    // 128-bit UUID
    status = ATT_ERR_INVALID_HANDLE;
  }
#endif
  // If a charactersitic value changed then callback function to notify application of change
  if ( (notifyApp != 0xFF ) && ECGProfile_AppCBs && ECGProfile_AppCBs->pfnSimpleProfileChange )
  {
    ECGProfile_AppCBs->pfnSimpleProfileChange( notifyApp );  
  }
  
  return ( status );
}

/*********************************************************************
 * @fn          ECGProfile_HandleConnStatusCB
 *
 * @brief       Simple Profile link status change handler function.
 *
 * @param       connHandle - connection handle
 * @param       changeType - type of change
 *
 * @return      none
 */
static void ECGProfile_HandleConnStatusCB( uint16 connHandle, uint8 changeType )
{ 
  // Make sure this is not loopback connection
  if ( connHandle != LOOPBACK_CONNHANDLE )
  {
    // Reset Client Char Config if connection has dropped
    if ( ( changeType == LINKDB_STATUS_UPDATE_REMOVED )      ||
         ( ( changeType == LINKDB_STATUS_UPDATE_STATEFLAGS ) && 
           ( !linkDB_Up( connHandle ) ) ) )
    { 
      GATTServApp_InitCharCfg( connHandle, ECGProfileChar4Config );
    }
  }
}


/*********************************************************************
*********************************************************************/
