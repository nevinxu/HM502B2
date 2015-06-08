#ifndef NPI_H
#define NPI_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * INCLUDES
 */

#include "hal_types.h"
#include "hal_board.h"
#include "hal_uart.h"

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */

/* UART port */

#define NPI_UART_PORT                  HAL_UART_PORT_0   //¿ª·¢°å
//#define NPI_UART_PORT                  HAL_UART_PORT_1   //HM502B1

#define NPI_UART_FC                    FALSE

#define NPI_UART_FC_THRESHOLD          50
#define NPI_UART_RX_BUF_SIZE           128
#define NPI_UART_TX_BUF_SIZE           128
#define NPI_UART_IDLE_TIMEOUT          200
#define NPI_UART_INT_ENABLE            TRUE

#if !defined( NPI_UART_BR )
#define NPI_UART_BR                    HAL_UART_BR_115200
#endif // !NPI_UART_BR

/*******************************************************************************
 * TYPEDEFS
 */

typedef void (*npiCBack_t) ( uint8 port, uint8 event );

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

/*******************************************************************************
 * FUNCTIONS
 */

//
// Network Processor Interface APIs
//

extern void   NPI_InitTransport( npiCBack_t npiCBack );
extern uint16 NPI_ReadTransport( uint8 *buf, uint16 len );
extern uint16 NPI_WriteTransport( uint8 *, uint16 );
extern uint16 NPI_RxBufLen( void );
extern uint16 NPI_GetMaxRxBufSize( void );
extern uint16 NPI_GetMaxTxBufSize( void );

/*******************************************************************************
*/

#ifdef __cplusplus
}
#endif

#endif /* NPI_H */
