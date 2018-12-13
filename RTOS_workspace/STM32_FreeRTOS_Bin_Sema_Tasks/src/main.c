/* Library includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "stm32f4xx.h"

//global space for some variable
char usr_msg[250]={0};

TaskHandle_t xTaskHandleM=NULL;
TaskHandle_t xTaskHandleE=NULL;


//function prototypes
static void prvSetupHardware(void);
void printmsg(char *msg);
static void prvSetupUart(void);
void prvSetupGpio(void);


/* The tasks to be created. */
static void vManagerTask( void *pvParameters );
static void vEmployeeTask( void *pvParameters );

/*-----------------------------------------------------------*/

/* Declare a variable of type xSemaphoreHandle.  This is used to reference the
semaphore that is used to synchronize both manager and employee task */
xSemaphoreHandle xWork;

/* this is the queue which manager uses to put the work ticket id */
xQueueHandle xWorkQueue;


int main( void )
{
	//this is here to collect segger event time stamp
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


	sprintf(usr_msg,"Demo of Binary semaphore usage between 2 Tasks \r\n");
	printmsg(usr_msg);


    /* Before a semaphore is used it must be explicitly created.
     * In this example a binary semaphore is created . */
    vSemaphoreCreateBinary( xWork );

	/* The queue is created to hold a maximum of 1 Element. */
    xWorkQueue = xQueueCreate( 1, sizeof( unsigned int ) );

    /* Check the semaphore and queue was created successfully. */
    if( (xWork != NULL) && (xWorkQueue != NULL) )
    {

		/* Create the 'Manager' task.  This is the task that will be synchronized with the Employee task.  The Manager task is created with a high priority  */
        xTaskCreate( vManagerTask, "Manager", 500, NULL, 3, NULL );

        /* Create a employee task with less priority than manager */
        xTaskCreate( vEmployeeTask, "Employee", 500, NULL, 1, NULL );

        /* Start the scheduler so the created tasks start executing. */
        vTaskStartScheduler();
    }

    sprintf(usr_msg,"Queue/Sema create failed.. \r\n");
    printmsg(usr_msg);

    /* If all is well we will never reach here as the scheduler will now be
    running the tasks.  If we do reach here then it is likely that there was
    insufficient heap memory available for a resource to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

 void vManagerTask( void *pvParameters )
{

	 unsigned int xWorkTicketId;
	 portBASE_TYPE xStatus;

    /* The semaphore is created in the 'empty' state, meaning the semaphore must
	 first be given using the xSemaphoreGive() API function before it
	 can subsequently be taken (obtained) */
    xSemaphoreGive( xWork);

    for( ;; )
    {
        /* get a work ticket id(some random number) */
        xWorkTicketId = ( rand() & 0x1FF );

		/* Sends work ticket id to the work queue */
		xStatus = xQueueSend( xWorkQueue, &xWorkTicketId , portMAX_DELAY ); //Post an item on back of the queue

		if( xStatus != pdPASS )
		{
			sprintf(usr_msg,"Could not send to the queue.\r\n");
		    printmsg(usr_msg);

		}else
		{
			/* Manager notifying the employee by "Giving" semaphore */
			xSemaphoreGive( xWork);
			/* after assigning the work , just yield the processor because nothing to do */
			taskYIELD();
			
		}
    }
}
/*-----------------------------------------------------------*/

void EmployeeDoWork(unsigned char TicketId)
{
	/* implement the work according to TickedID */
	sprintf(usr_msg,"Employee task : Working on Ticked id : %d\r\n",TicketId);
	printmsg(usr_msg);
	vTaskDelay(TicketId);
}

static void vEmployeeTask( void *pvParameters )
{

	unsigned char xWorkTicketId;
	portBASE_TYPE xStatus;
    /* As per most tasks, this task is implemented within an infinite loop. */
    for( ;; )
    {
		/* First Employee tries to take the semaphore, if it is available that means there is a task assigned by manager, otherwise employee task will be blocked */
		xSemaphoreTake( xWork, 0 );

		/* get the ticket id from the work queue */
		xStatus = xQueueReceive( xWorkQueue, &xWorkTicketId, 0 );

		if( xStatus == pdPASS )
		{
		  /* employee may decode the xWorkTicketId in this function to do the work*/
			EmployeeDoWork(xWorkTicketId);
		}
		else
		{
			/* We did not receive anything from the queue.  This must be an error as this task should only run when the manager assigns at least one work. */
			sprintf(usr_msg,"Employee task : Queue is empty , nothing to do.\r\n");
		    printmsg(usr_msg);
		}
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




