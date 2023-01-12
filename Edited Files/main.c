/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"
#include "queue.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"


/*-----------------------------------------------------------*/
#define Periodic_String 1
#define Btn_Rising	2
#define Btn_Falling 3

/*   Global data */
#define Task1_Period 50
#define Task2_Period 50
#define Task3_Period 100
#define Task4_Period 20
#define Task5_Period 10
#define Task6_Period 100
/*- - - - -  - - - - -  - - */

int Task_1_IN=0 , Task_1_OUT=0 , Task_1_Total=0;
int Task_2_IN=0 , Task_2_OUT=0 , Task_2_Total=0;
int Task_3_IN=0 , Task_3_OUT=0 , Task_3_Total=0;
int Task_4_IN=0 , Task_4_OUT=0 , Task_4_Total=0;
int Task_5_IN=0 , Task_5_OUT=0 , Task_5_Total=0;
int Task_6_IN=0 , Task_6_OUT=0 , Task_6_Total=0;
int Systm_Time = 0 , CPU_Load = 0;

/* Tasks Handler */


TaskHandle_t Btn_1TaskHandle = NULL;
TaskHandle_t Btn_2TaskHandle = NULL;
TaskHandle_t Tx_TaskHandle = NULL;
TaskHandle_t Uart_TaskHandle = NULL;
TaskHandle_t L1TaskHandle = NULL;
TaskHandle_t L2TaskHandle = NULL;
QueueHandle_t QueueBtn_1=NULL,QueueBtn_2=NULL ,QueueTx=NULL;


/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )

/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/

/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */

/* Tasks Prototypes */

void vApplicationTickHook( void )
{
	GPIO_write(PORT_0 , PIN8 , PIN_IS_HIGH);
	GPIO_write(PORT_0 , PIN8 , PIN_IS_LOW);
}

void vApplicationIdleHook( void )
{
	GPIO_write(PORT_0 , PIN9 , PIN_IS_HIGH);
	GPIO_write(PORT_0 , PIN9 , PIN_IS_LOW);
}


/* -------------------   Task 1   -----------------------------*/
/* ------------------------------------------------------------*/
void Btn1(void * pvParameters)
{
	volatile unsigned char prvState , nxtState , Btn1_State;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	vTaskSetApplicationTaskTag(NULL , (void *) 1);
	
	prvState = GPIO_read(PORT_0 , PIN0);

	while(1)
	{
	if(prvState == 0)
	{
		nxtState =  GPIO_read(PORT_0 , PIN0);
		if(prvState == 0 && nxtState ==1)
		{
			Btn1_State = Btn_Rising;
		}
		else
		{
			Btn1_State = 0;
		}
	}
	else if(prvState == 1)
	{
		nxtState =  GPIO_read(PORT_0 , PIN0);
		if(prvState == 1 && nxtState == 0)
		{
			Btn1_State = Btn_Falling;
		}
		else
		{
			Btn1_State = 0;
		}
	}
	prvState = nxtState;
	if(QueueBtn_1 != NULL)
	{
		xQueueSend(QueueBtn_1, (void *) &Btn1_State,0);
	}
  vTaskDelayUntil( &xLastWakeTime, Task1_Period );
	GPIO_write(PORT_0 , PIN9 , PIN_IS_LOW);
	}
	
}
/* -------------------   Task 2   -----------------------------*/
/* ------------------------------------------------------------*/


void Btn2(void * pvParameters)
{
	volatile unsigned char prvState , nxtState , Btn2_State;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	vTaskSetApplicationTaskTag(NULL , (void *) 2);
	
	prvState = GPIO_read(PORT_0 , PIN1);

	while(1)
	{
	
	if(prvState == 0)
	{
		nxtState =  GPIO_read(PORT_0 , PIN1);
		if(prvState == 0 && nxtState ==1)
		{
			Btn2_State = Btn_Rising;
		}
		else
		{
			Btn2_State = 0;
		}
	}
	else if(prvState == 1)
	{
		nxtState =  GPIO_read(PORT_0 , PIN1);
		if(prvState == 1 && nxtState == 0)
		{
			Btn2_State =Btn_Falling;
		}
		else
		{
			Btn2_State = 0;
		}
	}
	prvState = nxtState;
	if(QueueBtn_2 != NULL)
	{
		xQueueSend(QueueBtn_2, (void *) &Btn2_State,0);
	}
  vTaskDelayUntil( &xLastWakeTime, Task2_Period );
	GPIO_write(PORT_0 , PIN9 , PIN_IS_LOW);
	}
	
}



/* -------------------   Task 3   -----------------------------*/
/* ------------------------------------------------------------*/

void Task_Tx( void * pvParameters )
{
	TickType_t xLastWakeTime = xTaskGetTickCount();
	unsigned char periodic_Str = Periodic_String;
	vTaskSetApplicationTaskTag(NULL , (void *) 3);
	
	while(1)
	{
		if(QueueTx != NULL)
		{
			xQueueSend(QueueTx, (void *) &periodic_Str,0);
		}
		vTaskDelayUntil( &xLastWakeTime, Task3_Period );
		GPIO_write(PORT_0 , PIN9 , PIN_IS_LOW);
	}
	
	
}

/****************   Task 4  ************/
/***************************************/

void Uart_Rx( void * pvParameters )
{
	TickType_t xLastWakeTime = xTaskGetTickCount();
	unsigned char Buffer;
	vTaskSetApplicationTaskTag(NULL , (void *) 4);

	while(1)
	{
	if(QueueTx != NULL)
	{
		if((xQueueReceive(QueueTx , (void *)&Buffer ,(TickType_t)10)) == pdPASS)
		{
			if(Buffer == Periodic_String)
			{
				vSerialPutString((const signed char *const)"I'm Task 3\n" , 12);
			}
		}
	}
	if((xQueueReceive(QueueBtn_1 , (void *)&Buffer ,(TickType_t)10)) == pdPASS)
		{
			if(Buffer == Btn_Rising)
			{
				vSerialPutString((const signed char *const)"Btn(1)Rising\n" , 13);
			}
			else if(Buffer == Btn_Falling)
			{
				vSerialPutString((const signed char *const)"Btn(1)Falling\n" , 14);
			}
			else
			{
				/* Do Nothing */
			}
		}
		if((xQueueReceive(QueueBtn_2 , (void *)&Buffer ,(TickType_t)10)) == pdPASS)
		{
			if(Buffer == Btn_Rising)
			{
				vSerialPutString((const signed char *const)"Btn(2)Rising\n" , 13);
			}
			else if(Buffer == Btn_Falling)
			{
				vSerialPutString((const signed char *const)"Btn(2)Falling\n" , 14);
			}
			else
			{
				/* Do Nothing */
			}
		}
	
		vTaskDelayUntil( &xLastWakeTime, Task4_Period );
		GPIO_write(PORT_0 , PIN9 , PIN_IS_LOW);
	}
	
}


void Load_1_Simulation( void * pvParameters )
{
		int i = 0;
		TickType_t xLastWakeTime = xTaskGetTickCount();
		vTaskSetApplicationTaskTag(NULL , (void *) 5);
		while(1)
		{
			for(i = 0 ; i<33300 ;i++)
			{
				i=i;
			}
			vTaskDelayUntil( &xLastWakeTime, Task5_Period );
			GPIO_write(PORT_0 , PIN9 , PIN_IS_LOW);
		}
		
}

void Load_2_Simulation( void * pvParameters )
{
		int i = 0;
		TickType_t xLastWakeTime = xTaskGetTickCount();
		vTaskSetApplicationTaskTag(NULL , (void *) 6);
		while(1)
		{
			for(i = 0 ; i<79900 ;i++)
			{
				i=i;
			}
			vTaskDelayUntil( &xLastWakeTime, Task6_Period );
			GPIO_write(PORT_0 , PIN9 , PIN_IS_LOW);
		}
		
}

int main( void )
{
		xSerialPortInitMinimal(ser9600);
		
		QueueBtn_1    = xQueueCreate( 1,sizeof(char*) );
	
		QueueBtn_2    = xQueueCreate( 1,sizeof(char*) );	

		QueueTx				= xQueueCreate( 1,sizeof(char*) );

	
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();
	
	
	
    /* Create Tasks here */
	xTaskPeriodicCreate(
                    Btn1,       /* Function that implements the task. */
                    "Button_1",          /* Text name for the task. */
                     100 ,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Btn_1TaskHandle ,
										Task1_Period); 
	xTaskPeriodicCreate(
                    Btn2,       /* Function that implements the task. */
                    "Button_2",          /* Text name for the task. */
                     100 ,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Btn_2TaskHandle ,/* Used to pass out the created task's handle. */	
										Task2_Period); 		/* Period of Task */								
										
										
	xTaskPeriodicCreate(
                    Task_Tx,       /* Function that implements the task. */
                    "Task_Tx",          /* Text name for the task. */
                     100 ,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Tx_TaskHandle ,
										Task3_Period);
	xTaskPeriodicCreate(
                    Uart_Rx,       /* Function that implements the task. */
                    "Rx",          /* Text name for the task. */
                     100 ,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Uart_TaskHandle ,
										Task4_Period);
	 										
  xTaskPeriodicCreate(
                    Load_1_Simulation,       /* Function that implements the task. */
                    "Load_1_Simulation",          /* Text name for the task. */
                     100 ,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &L1TaskHandle ,
										Task5_Period);
	xTaskPeriodicCreate(
                    Load_2_Simulation,       /* Function that implements the task. */
                    "Load_2_Simulation",          /* Text name for the task. */
                     100 ,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &L2TaskHandle ,
										Task6_Period); 
      /* Used to pass out the created task's handle. */

	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


