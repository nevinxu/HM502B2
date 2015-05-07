#ifndef _SERIALINTERFACE_
#define _SERIALINTERFACE_
#include "hal_uart.h"
#include "OSAL.h"
#include "npi.h"

#define SI_CMD_RX                                 0x0001
#define SI_EVT_TX                                 0x0002

#define MAX_PKT_SIZE 128
#define CMD_RESPONSE 2

#define SERIAL_IDENTIFIER       0x77


#define APP_CMD_SERIALCONNECT   0x01
#define APP_CMD_SERIALCONNECTACK   0x02
#define APP_CMD_SERIALDISCONNECT   0x0C
#define APP_CMD_SERIALDISCONNECTACK   0x0D
#define APP_CMD_ADVERTISE       0x03
#define APP_CMD_ADVERTISEACK       0x04
#define APP_CMD_ADVERTISEBEGIN       0x0E
#define APP_CMD_CONNECTBLE       0x05
#define APP_CMD_CONNECTBLEACK       0x06
#define APP_CMD_DISCONNECTBLE      0x0A 
#define APP_CMD_DISCONNECTBLEACK      0x0B 
#define APP_CMD_NOTIFYBLE      0x07 
#define APP_CMD_NOTIFYBLEACK      0x08 
#define APP_CMD_DATASEND                0x09
#define APP_CMD_IDRECEIVEACK                0x0F
#define APP_CMD_IDRECEIVE                0x10
#define APP_CMD_AUTOCONNECT                0x11
#define APP_CMD_AUTOCONNECTACK                0x12
#define APP_CMD_RSSIVALUE                0x13
#define APP_CMD_RECEIVEECGDATA                0x14
#define APP_CMD_RECEIVEECGDATAACK                0x15
#define APP_CMD_STOPRECEIVEECGDATA                0x16
#define APP_CMD_STOPRECEIVEECGDATAACK                0x17
#define APP_CMD_ECGPATCHID                      0x18
#define APP_CMD_ECGPATCHIDACK                0x19
// Serial Event Mnemonic        Opcode
#define	APP_EVT_CMD_RESPONSE	0x81
#define APP_EVT_CONNECT	        0x82 // Not used
#define	APP_EVT_DISCONNECT	0x83 // Not used


typedef struct _BLEPacketHeader
{
uint8 identifier;	
uint8 opCode;
uint8 status;
} BLEPacketHeader_t;


typedef struct _BLEPacket
{
BLEPacketHeader_t header;
// payload
uint8 length;  // opCode specific, 
uint8 data[64];  // command or event specific data, 10 Bytes currently
} BLEPacket_t;

typedef struct _SerialEventPacket
{
BLEPacketHeader_t header;
// payload
uint8 length;  // Length 
uint8 cmdCode; // Command Code
uint8 status;  // Status
} SerialEventPacket_t;


/* States for CRC parser */
typedef enum {
  NPI_SERIAL_STATE_ID,
  NPI_SERIAL_STATE_OPCODE,
  NPI_SERIAL_STATE_STATUS,
  NPI_SERIAL_STATE_LEN,
  NPI_SERIAL_STATE_DATA,
} npi_serial_parse_state_t;

extern void cSerialPacketParser( uint8 port, uint8 events );
extern void parseCmd(void);
extern void sendSerialEvt(void);
extern void SendPCCommand(uint8 command);


/*******************************************************************************
 * MACROS
 */


/*********************************************************************
 * FUNCTIONS
 */

/*
 * Task Initialization for the BLE Application
 */
extern void SerialInterface_Init( uint8 task_id );

/*
 * Task Event Processor for the BLE Application
 */
extern uint16 SerialInterface_ProcessEvent( uint8 task_id, uint16 events );

#endif
