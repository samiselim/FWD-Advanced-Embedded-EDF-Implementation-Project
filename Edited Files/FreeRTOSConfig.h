/*
 * FreeRTOS V202212.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <lpc21xx.h>
#include "GPIO.h"

/* Global Variables */
extern int Task_1_IN , Task_1_OUT , Task_1_Total;
extern int Task_2_IN , Task_2_OUT , Task_2_Total;
extern int Task_3_IN , Task_3_OUT , Task_3_Total;
extern int Task_4_IN , Task_4_OUT , Task_4_Total;
extern int Task_5_IN , Task_5_OUT , Task_5_Total;
extern int Task_6_IN , Task_6_OUT , Task_6_Total;
extern int Systm_Time , CPU_Load;
/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* EDF Configuration */
#define configUSE_EDF_SCHEDULER 1
/****************************************/
#define configSUPPORT_DYNAMIC_ALLOCATION 1
/****************************************/


#define configUSE_PREEMPTION		1
#define configUSE_IDLE_HOOK			1
#define configUSE_TICK_HOOK			1
#define configCPU_CLOCK_HZ			( ( unsigned long ) 60000000 )	/* =12.0MHz xtal multiplied by 5 using the PLL. */
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES		( 4 )
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 90 )
#define configTOTAL_HEAP_SIZE		( ( size_t ) 13 * 1024 )
#define configMAX_TASK_NAME_LEN		( 8 )
#define configUSE_TRACE_FACILITY  1
#define configUSE_16_BIT_TICKS		0
#define configIDLE_SHOULD_YIELD		1

#define configUSE_TIME_SLICING    1
#define configQUEUE_REGISTRY_SIZE 	0
#define configUSE_STATS_FORMATTING_FUNCTIONS 1
#define configGENERATE_RUN_TIME_STATS             1




/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

#define configUSE_APPLICATION_TASK_TAG 1

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1

/* Trace Hooks */
#define traceTASK_SWITCHED_OUT() do{\
																if((int)pxCurrentTCB->pxTaskTag == 1 )\
																{\
																	GPIO_write(PORT_0 , PIN2 , PIN_IS_LOW );\
																	Task_1_Total += (T1TC - Task_1_IN );\
																}\
																else if((int)pxCurrentTCB->pxTaskTag == 2 )\
																{\
																	GPIO_write(PORT_0 , PIN3 , PIN_IS_LOW );\
																	Task_2_Total += (T1TC - Task_2_IN );\
																}\
																else if((int)pxCurrentTCB->pxTaskTag == 3 )\
																{\
																	GPIO_write(PORT_0 , PIN4 , PIN_IS_LOW );\
																	Task_3_Total += (T1TC - Task_3_IN );\
																}\
																else if((int)pxCurrentTCB->pxTaskTag == 4 )\
																{\
																	GPIO_write(PORT_0 , PIN5 , PIN_IS_LOW );\
																	Task_4_Total += (T1TC - Task_4_IN );\
																}\
																else if((int)pxCurrentTCB->pxTaskTag == 5 )\
																{\
																	GPIO_write(PORT_0 , PIN6 , PIN_IS_LOW );\
																	Task_5_Total += (T1TC - Task_5_IN );\
																}\
																else if((int)pxCurrentTCB->pxTaskTag == 6 )\
																{\
																	GPIO_write(PORT_0 , PIN7 , PIN_IS_LOW );\
																	Task_6_Total += (T1TC - Task_6_IN );\
																}\
																Systm_Time = T1TC;\
																CPU_Load = (((Task_1_Total + Task_2_Total + Task_3_Total + Task_4_Total + Task_5_Total + Task_6_Total) / (float)Systm_Time  )) * 100;\
																}while(0);
#define traceTASK_SWITCHED_IN() do{\
																if((int)pxCurrentTCB->pxTaskTag == 1)\
																{\
																	GPIO_write(PORT_0 , PIN2 , PIN_IS_HIGH);\
																	Task_1_IN = T1TC;\
																}\
																else if((int)pxCurrentTCB->pxTaskTag == 2)\
																{\
																	GPIO_write(PORT_0 , PIN3 , PIN_IS_HIGH);\
																	Task_2_IN = T1TC;\
																}\
																else if((int)pxCurrentTCB->pxTaskTag == 3)\
																{\
																	GPIO_write(PORT_0 , PIN4 , PIN_IS_HIGH);\
																	Task_3_IN = T1TC;\
																}\
																else if((int)pxCurrentTCB->pxTaskTag == 4)\
																{\
																	GPIO_write(PORT_0 , PIN5 , PIN_IS_HIGH);\
																	Task_4_IN = T1TC;\
																}\
																else if((int)pxCurrentTCB->pxTaskTag == 5)\
																{\
																	GPIO_write(PORT_0 , PIN6 , PIN_IS_HIGH);\
																	Task_5_IN = T1TC;\
																}\
																else if((int)pxCurrentTCB->pxTaskTag == 6)\
																{\
																	GPIO_write(PORT_0 , PIN7 , PIN_IS_HIGH);\
																	Task_6_IN = T1TC;\
																}\
																}while(0);
#endif /* FREERTOS_CONFIG_H */
