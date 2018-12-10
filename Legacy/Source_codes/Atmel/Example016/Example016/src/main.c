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

#include <asf.h>
#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.c"

/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT		( 0xfffff )

/* The task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

//binary semaphore handle
xSemaphoreHandle xBinarySemaphore;

/**
 *  Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONSOLE_UART, &uart_serial_options);
}

int main( void )
{
	/* This function initializes the MCU clock  */
	sysclk_init();
	/* Board initialization */
	board_init();
	
	/* Initialize the serial I/O(console ) */
	configure_console();

	// first lets create the binary semaphore. 
	vSemaphoreCreateBinary( xBinarySemaphore );

if(xBinarySemaphore != NULL)
{
	/* Create one of the two tasks. */
	xTaskCreate(	vTask1,		/* Pointer to the function that implements the task. */
					"Task 1",	/* Text name for the task.  This is to facilitate debugging only. */
					240,		/* Stack depth in words. */
					NULL,		/* We are not using the task parameter. */
					1,			/* This task will run at priority 1. */
					NULL );		/* We are not using the task handle. */

	/* Create the other task in exactly the same way. */
	xTaskCreate( vTask2, "Task 2", 240, NULL, 2, NULL );

	/* Start the scheduler so our tasks start executing. */
	vTaskStartScheduler();
}
else
{

	printf("binary semaphore creation failed\n");
}

	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vTask1( void *pvParameters )
{
const char *pcTaskName = "Task 1 is running\n";
volatile unsigned long ul;

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
	   //before printing , lets own the semaphore or take the semaphore */
	   xSemaphoreTake( xBinarySemaphore, 0 );
		/* Print out the name of this task. */
		printf( "%s",pcTaskName);
		//our printing is done, so lets give the semaphore */
		xSemaphoreGive( xBinarySemaphore );

		/*Now this task will be blocked for 500ticks */
		vTaskDelay( 500 / portTICK_RATE_MS );
	}
}
/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
const char *pcTaskName = "Task 2 is running\n";
volatile unsigned long ul;

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
	   //before printing , lets own the semaphore or take the semaphore */
	   xSemaphoreTake( xBinarySemaphore, 0 );
	   /* Print out the name of this task. */
	   printf( "%s",pcTaskName);
	   //our printing is done, so lets give the semaphore */
	   xSemaphoreGive( xBinarySemaphore );

		/*Now this task will be blocked for 500ticks */
		vTaskDelay( 500 / portTICK_RATE_MS );
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


