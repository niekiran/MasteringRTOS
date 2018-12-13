/* Library includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "stm32f4xx.h"

//global space for some variable
char usr_msg[250]={0};
TaskHandle_t xTaskHandle1=NULL;
TaskHandle_t xTaskHandle2=NULL;

//function prototypes
static void prvSetupHardware(void);
void printmsg(char *msg);
static void prvSetupUart(void);
void prvSetupGpio(void);

/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT		( 0xfffff )

/* The task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

//binary semaphore handle
xSemaphoreHandle xBinarySemaphore;


int main( void )
{
	DWT->CTRL |= (1 << 0);//Enable CYCCNT in DWT_CTRL.

	//1.  Reset the RCC clock configuration to the default reset state.
	//HSI ON, PLL OFF, HSE OFF, system clock = 16MHz, cpu_clock = 16MHz
	RCC_DeInit();

	//2. update the SystemCoreClock variable
	SystemCoreClockUpdate();

	prvSetupHardware();


	//Start Recording
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();


	sprintf(usr_msg,"Demo of Mutual exclusion using binary semaphore\r\n");
	printmsg(usr_msg);

	//Creating a binary semaphore
	vSemaphoreCreateBinary(xBinarySemaphore);

	 if(xBinarySemaphore != NULL)
	 {
			/* Create one of the two tasks. */
			xTaskCreate(	vTask1,		/* Pointer to the function that implements the task. */
							"Task 1",	/* Text name for the task.  This is to facilitate debugging only. */
							500,		/* Stack depth in words. */
							NULL,		/* We are not using the task parameter. */
							1,			/* This task will run at priority 1. */
							NULL );		/* We are not using the task handle. */

			/* Create the other task in exactly the same way. */
			xTaskCreate( vTask2, "Task 2", 500, NULL, 1, NULL );

			//makes sema available for the first time
			xSemaphoreGive(xBinarySemaphore);

			/* Start the scheduler so our tasks start executing. */
			vTaskStartScheduler();
	 }else
	 {
		 sprintf(usr_msg,"binary semaphore creation failed\r\n");
		 printmsg(usr_msg);

	 }


	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vTask1( void *pvParameters )
{
	const char *pcTaskName = "Task 1 is running\r\n";

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{

		//before printing , lets own the semaphore or take the semaphore */
		xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );

		/* Print out the name of this task. */
		sprintf( usr_msg,"%s",pcTaskName);
		printmsg(usr_msg);

		//give the semaphore here. give operation increases the bin sema value back to 1
		xSemaphoreGive(xBinarySemaphore);

		/*Now this task will be blocked for 500ticks */
		vTaskDelay( pdMS_TO_TICKS(500) );
	}
}
/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{

	const char *pcTaskName = "Task 2 is running\r\n";

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{

		//before printing , lets own the semaphore or take the semaphore */
		xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );

		/* Print out the name of this task. */
		sprintf( usr_msg,"%s",pcTaskName);
		printmsg(usr_msg);

		//give the semaphore here. give operation increases the bin sema value back to 1
		xSemaphoreGive(xBinarySemaphore);

		/*Now this task will be blocked for 500ticks */
		vTaskDelay( pdMS_TO_TICKS(500));
	}
}



static void prvSetupHardware(void)
{
	//Setup Button and LED
	prvSetupGpio();

	//setup UART2
	prvSetupUart();
}

void printmsg(char *msg)
{
	for(uint32_t i=0; i < strlen(msg); i++)
	{
		while ( USART_GetFlagStatus(USART2,USART_FLAG_TXE) != SET);
		USART_SendData(USART2,msg[i]);
	}

	while ( USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);

}


static void prvSetupUart(void)
{
	GPIO_InitTypeDef gpio_uart_pins;
	USART_InitTypeDef uart2_init;

	//1. Enable the UART2  and GPIOA Peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	//PA2 is UART2_TX, PA3 is UART2_RX

	//2. Alternate function configuration of MCU pins to behave as UART2 TX and RX

	//zeroing each and every member element of the structure
	memset(&gpio_uart_pins,0,sizeof(gpio_uart_pins));

	gpio_uart_pins.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	gpio_uart_pins.GPIO_Mode = GPIO_Mode_AF;
	gpio_uart_pins.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_uart_pins.GPIO_OType= GPIO_OType_PP;
	gpio_uart_pins.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOA, &gpio_uart_pins);


	//3. AF mode settings for the pins
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //PA2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //PA3

	//4. UART parameter initializations
	//zeroing each and every member element of the structure
	memset(&uart2_init,0,sizeof(uart2_init));

	uart2_init.USART_BaudRate = 115200;
	uart2_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart2_init.USART_Mode =  USART_Mode_Tx | USART_Mode_Rx;
	uart2_init.USART_Parity = USART_Parity_No;
	uart2_init.USART_StopBits = USART_StopBits_1;
	uart2_init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&uart2_init);


	//5. Enable the UART2 peripheral
	USART_Cmd(USART2,ENABLE);

}


void prvSetupGpio(void)
{
	// this function is board specific

	//Peripheral clock enable for GPIOA and GPIOC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);

	GPIO_InitTypeDef led_init, button_init;
	led_init.GPIO_Mode = GPIO_Mode_OUT;
	led_init.GPIO_OType = GPIO_OType_PP;
	led_init.GPIO_Pin = GPIO_Pin_5;
	led_init.GPIO_Speed = GPIO_Low_Speed;
	led_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&led_init);

	button_init.GPIO_Mode = GPIO_Mode_IN;
	button_init.GPIO_OType = GPIO_OType_PP;
	button_init.GPIO_Pin = GPIO_Pin_13;
	button_init.GPIO_Speed = GPIO_Low_Speed;
	button_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&button_init);

}
