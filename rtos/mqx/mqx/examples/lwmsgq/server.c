/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
* Copyright 1989-2008 ARC International
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale MQX RTOS License
* distributed with this Material.
* See the MQX_RTOS_LICENSE file distributed for more details.
*
* Brief License Summary:
* This software is provided in source form for you to use free of charge,
* but it is not open source software. You are allowed to use this software
* but you cannot redistribute it or derivative works of it in source form.
* The software may be used only in connection with a product containing
* a Freescale microprocessor, microcontroller, or digital signal processor.
* See license agreement file for full license terms including other
* restrictions.
*****************************************************************************
*
* Comments:
*
*   This file contains the code for the server_task
*
*
*END************************************************************************/

#include <stdio.h>
#include <mqx.h>
#include <bsp.h>
#include <lwmsgq.h>
#include "server.h"

MUTEX_STRUCT   print_mutex;

/*TASK*--------------------------------------------------------
*
* Task Name : server_task
* Comments  : This task initializes the message queues,
*  creates three client tasks, and then waits for a message.
*  After recieving a message, the task returns the message to 
*  the sender.
*END*--------------------------------------------------------*/

void server_task 
   (
      uint32_t param
   )
{
   _mqx_uint          msg[MSG_SIZE];
   _mqx_uint          i;
   _mqx_uint          result;
   MUTEX_ATTR_STRUCT mutexattr;
   
   result = _lwmsgq_init((void *)server_queue, NUM_MESSAGES, MSG_SIZE);
   if (result != MQX_OK) {
      // lwmsgq_init failed
   } /* Endif */
   result = _lwmsgq_init((void *)client_queue, NUM_MESSAGES, MSG_SIZE);
   if (result != MQX_OK) {
      // lwmsgq_init failed
   } /* Endif */
   
   /* create the client tasks */
   for (i = 0; i < NUM_CLIENTS; i++) {
      _task_create(0, CLIENT_TASK, (uint32_t)i);
   }
      
   /* Initialize mutex attributes */
   if (_mutatr_init(&mutexattr) != MQX_OK) {
      printf("Initialize mutex attributes failed.\n");
      _task_block();
   }
   
   /* Initialize the mutex */ 
   if (_mutex_init(&print_mutex, &mutexattr) != MQX_OK) {
      printf("Initialize print mutex failed.\n");
      _task_block();
   }
   
   while (TRUE) {
      _lwmsgq_receive((void *)server_queue, msg, LWMSGQ_RECEIVE_BLOCK_ON_EMPTY, 0, 0);
      _mutex_lock(&print_mutex);
      printf(" %c \n", msg[0]);
      _mutex_unlock(&print_mutex);
      
      _lwmsgq_send((void *)client_queue, msg, LWMSGQ_SEND_BLOCK_ON_FULL);
   }

}
      
/* EOF */
