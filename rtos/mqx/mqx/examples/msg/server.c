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
#include "server.h"

/* declaration of a global message pool */
_pool_id   message_pool;
_pool_id fp_message_pool;
MUTEX_STRUCT   print_mutex;

/*TASK*--------------------------------------------------------
*
* Task Name : server_task
* Comments  : This task creates a message queue for itself,
*  creates a message pool, creates three client tasks, and 
*  then waits for a message.  After recieving a message, 
*  the task returns the message to the sender. 
*END*--------------------------------------------------------*/

void server_task 
   (
      uint32_t param
   )
{
   SERVER_MESSAGE_PTR msg_ptr;
   _mqx_uint          i;
   _queue_id          server_qid;
   bool            result;
   _task_id           task_id;
   MUTEX_ATTR_STRUCT mutexattr;
   
   /* open a message queue */
   server_qid = _msgq_open(SERVER_QUEUE, 0);
   
   if (server_qid == 0) {
      printf("\nCould not open the server message queue\n");
      _task_block();
   }

   /* create a message pool */   
   message_pool = _msgpool_create(sizeof(SERVER_MESSAGE), 
      NUM_CLIENTS, 0, 0);
      
   if (message_pool == MSGPOOL_NULL_POOL_ID) {
      printf("\nCount not create a message pool\n");
      _task_block();
   }

   /* create the client tasks */
   for (i = 0; i < NUM_CLIENTS; i++) {
      task_id = _task_create(0, CLIENT_TASK, (uint32_t)i);
      
      if (task_id == 0) {
         printf("\nCould not create a client task\n");
         _task_block();
      }
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
      msg_ptr = _msgq_receive(server_qid, 0);
      
      if (msg_ptr == NULL) {
         printf("\nCould not receive a message\n");
         _task_block();
      }
      
      _mutex_lock(&print_mutex);
      printf(" %c \n", msg_ptr->DATA[0]);
      _mutex_unlock(&print_mutex);
      
      /* return the message */   
      msg_ptr->HEADER.TARGET_QID = msg_ptr->HEADER.SOURCE_QID;
      msg_ptr->HEADER.SOURCE_QID = server_qid;
      
      result = _msgq_send(msg_ptr);
      
      if (result != TRUE) {
         printf("\nCould not send a message\n");
         _task_block();
      }
      _time_delay_ticks(1);
   }
}
      
/* EOF */
