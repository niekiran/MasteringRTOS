/*
    FreeRTOS V6.0.5 - Copyright (C) 2009 Real Time Engineers Ltd.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT		( 0xfffff )

/* The task function. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

/* Define the strings that will be passed in as the task parameters.  These are
defined const and off the stack to ensure they remain valid when the tasks are
executing. */
const char *pcTextForTask1 = "Task 1 is running\n";
const char *pcTextForTask2 = "Task 2 is running\n";

/* Declare a variable that is used to hold the handle of Task2. */
xTaskHandle xTask2Handle;


void vAssertCalled( unsigned long ulLine, const char * const pcFileName );

/*-----------------------------------------------------------*/


int main( void )
{
	/* Create the first task at priority 1... */
	xTaskCreate( vTask1, "Task 1", 240, (void*)pcTextForTask1, 2, NULL );

	/* ... and the second task at priority 2.  The priority is the second to
	last parameter. */
	xTaskCreate( vTask2, "Task 2", 240, (void*)pcTextForTask2, 1, &xTask2Handle );

	printf( "About to launch the scheduler\r\n" );

	/* Start the scheduler so our tasks start executing. */
	vTaskStartScheduler();

	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vTask1( void *pvParameters )
{
	unsigned portBASE_TYPE uxPriority;

	/* This task will always run before Task2 as it is created with the higher
	priority. Neither Task1 nor Task2 ever block so both will always be in either
	the Running or the Ready state.
	Query the priority at which this task is running - passing in NULL means
	"return my priority". */
	uxPriority = uxTaskPriorityGet( xTask2Handle );

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
		printf( "Task1 is running\r\n" );

		/* Setting the Task2 priority above the Task1 priority will cause
		Task2 to immediately start running (as then Task2 will have the higher
		priority of the two created tasks). Note the use of the handle to task
		2 (xTask2Handle) in the call to vTaskPrioritySet(). Listing 24 shows how
		the handle was obtained. */
		printf( "About to raise the Task2 priority\r\n" );
		vTaskPrioritySet( xTask2Handle, ( uxPriority + 2 ) );

		/* Task1 will only run when it has a priority higher than Task2.
		Therefore, for this task to reach this point Task2 must already have
		executed and set its priority back down to below the priority of this
		task. */
	}
}
/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
	unsigned portBASE_TYPE uxPriority;

	/* Task1 will always run before this task as Task1 is created with the
	higher priority. Neither Task1 nor Task2 ever block so will always be
	in either the Running or the Ready state.
	Query the priority at which this task is running - passing in NULL means
	"return my priority". */
	uxPriority = uxTaskPriorityGet( NULL );

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* For this task to reach this point Task1 must have already run and
		set the priority of this task higher than its own.

		Print out the name of this task. */
		printf( "Task2 is running\r\n" );

		/* Set our priority back down to its original value. Passing in NULL
		as the task handle means "change my priority". Setting the
		priority below that of Task1 will cause Task1 to immediately start
		running again – pre-empting this task. */
		printf( "About to lower the Task2 priority\r\n" );
		vTaskPrioritySet( NULL, ( uxPriority - 2 ) );
	}
}


/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* This function will only be called if an API call to create a task, queue
	or semaphore fails because there is too little heap RAM remaining. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
	/* This function will only be called if a task overflows its stack.  Note
	that stack overflow checking does slow down the context switch
	implementation. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* This example does not use the idle hook to perform any processing. */
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This example does not use the tick hook to perform any processing. */
}


static portBASE_TYPE xTraceRunning = pdFALSE;
void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
static portBASE_TYPE xPrinted = pdFALSE;
volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

	/* Parameters are not used. */
	( void ) ulLine;
	( void ) pcFileName;

 	taskENTER_CRITICAL();
	{
		/* Stop the trace recording. */
		if( xPrinted == pdFALSE )
		{
			xPrinted = pdTRUE;
			if( xTraceRunning == pdTRUE )
			{
			//	vTraceStop();
				//prvSaveTraceFile();
			}
		}

		/* You can step out of this function to debug the assertion by using
		the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
		value. */
		while( ulSetToNonZeroInDebuggerToContinue == 0 )
		{
			__asm volatile( "NOP" );
			__asm volatile( "NOP" );
		}
	}
	taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/
