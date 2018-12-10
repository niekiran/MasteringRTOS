/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @brief   Default main function.
  ******************************************************************************
*/

#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define TRUE 1
#define FALSE 0


//function prototypes
static void prvSetupHardware(void);
void printmsg(char *msg);
static void prvSetupUart(void);
void prvSetupGpio(void);

//tasks prototypes
void vTask1_handler(void *params);


void rtos_delay(uint32_t delay_in_ms);
void Led_demo_handle(uint8_t data);
//global space for some variable
char usr_msg[250]={0};
TaskHandle_t xTaskHandle1=NULL;
TaskHandle_t xTaskHandle2=NULL;

SemaphoreHandle_t binary_sema;
uint8_t command_buffer[30];


#define LED_WEIGHT_0  GPIO_Pin_5
#define LED_WEIGHT_1  GPIO_Pin_6
#define LED_WEIGHT_2  GPIO_Pin_7
#define LED_WEIGHT_4  GPIO_Pin_8

int main(void)
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

	sprintf(usr_msg,"Demo of synchronization between ISR and Task using binary sema\r\n");
	printmsg(usr_msg);

	binary_sema = xSemaphoreCreateBinary();

	if(! binary_sema)
	{
		sprintf(usr_msg,"Sema creation failed\r\n");
		printmsg(usr_msg);
		while(1);
	}

	//lets create task-1
    xTaskCreate(vTask1_handler,"LED-DEMO-TASK",500,NULL,2,&xTaskHandle1);

    //lets start the scheduler
    vTaskStartScheduler();

	for(;;);
}


void vTask1_handler(void *params)
{
uint32_t i =0;
	while(1)
	{
		xSemaphoreTake(binary_sema,portMAX_DELAY);
		sprintf(usr_msg,"LED Demo will start in , \r\n");
		printmsg(usr_msg);
		sprintf(usr_msg,"1\r");
		printmsg(usr_msg);
		rtos_delay(1000);
		sprintf(usr_msg,"2\r");
		printmsg(usr_msg);
		rtos_delay(1000);
		sprintf(usr_msg,"3\r");
		printmsg(usr_msg);
		rtos_delay(1000);
		while(command_buffer[i] != '\0')
		{
			Led_demo_handle(command_buffer[i]);
			rtos_delay(2000);
			i++;
		}

		sprintf(usr_msg,"\r\nDemo End\r\n");
		printmsg(usr_msg);
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




	//enable uart receive interrupt
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

	//do NVIC settings
	NVIC_SetPriority(USART2_IRQn,configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);

	//Enable IRQ
	NVIC_EnableIRQ(USART2_IRQn);


	//5. Enable the UART2 peripheral
	USART_Cmd(USART2,ENABLE);


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




void USART2_IRQHandler(void)
{
 static uint32_t command_len = 0, arg_len=5;
uint8_t data=0;

BaseType_t xHigherPriorityTaskWoken = pdFALSE;


	if ( USART_GetFlagStatus(USART2,USART_FLAG_RXNE) )
	{
		//date byte is waiting lets read it
		 data = USART_ReceiveData(USART2) & 0XFF;
		 command_buffer[command_len++] = data;
		 if(data == '\r')
		 {
			 //user has finished sending the data. wakeup led demo task
			 //make binary sema available
			 command_len=0;
			 xSemaphoreGiveFromISR(binary_sema,&xHigherPriorityTaskWoken);

		 }

	}

	 /* Now the buffer is empty we can switch context if necessary. */
	    if( xHigherPriorityTaskWoken )
	    {
	        /* Actual macro used here is port specific. */
	        taskYIELD();
	    }

}

void rtos_delay(uint32_t delay_in_ms)
{
	uint32_t delay_in_tick = pdMS_TO_TICKS(delay_in_ms);
	uint32_t current_tick_count = xTaskGetTickCount();
	while(xTaskGetTickCount() < (current_tick_count + delay_in_tick));
}


void Led_demo_handle(uint8_t data)
{

	switch(data)
	{
	case '0':
		GPIO_WriteBit(GPIOA,LED_WEIGHT_0,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_1,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_2,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_4,Bit_RESET);
		break;
	case '1':
		GPIO_WriteBit(GPIOA,LED_WEIGHT_0,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_1,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_2,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_4,Bit_RESET);
		break;
	case '2':
		GPIO_WriteBit(GPIOA,LED_WEIGHT_0,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_1,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_2,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_4,Bit_RESET);
		break;
	case '3':
		GPIO_WriteBit(GPIOA,LED_WEIGHT_0,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_1,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_2,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_4,Bit_RESET);
		break;
	case '4':
		GPIO_WriteBit(GPIOA,LED_WEIGHT_0,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_1,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_2,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_4,Bit_RESET);
		break;
	case '5':
		GPIO_WriteBit(GPIOA,LED_WEIGHT_0,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_1,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_2,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_4,Bit_RESET);
		break;
	case '6':
		GPIO_WriteBit(GPIOA,LED_WEIGHT_0,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_1,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_2,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_4,Bit_RESET);
		break;
	case '7':
		GPIO_WriteBit(GPIOA,LED_WEIGHT_0,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_1,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_2,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_4,Bit_RESET);
		break;
	case '8':
		GPIO_WriteBit(GPIOA,LED_WEIGHT_0,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_1,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_2,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_4,Bit_RESET);
		break;
	case '9':
		GPIO_WriteBit(GPIOA,LED_WEIGHT_0,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_1,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_2,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_4,Bit_RESET);
		break;
	case 'A':
		GPIO_WriteBit(GPIOA,LED_WEIGHT_0,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_1,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_2,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_4,Bit_RESET);
		break;
	case 'B':
		GPIO_WriteBit(GPIOA,LED_WEIGHT_0,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_1,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_2,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_4,Bit_RESET);
		break;
	case 'C':
		GPIO_WriteBit(GPIOA,LED_WEIGHT_0,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_1,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_2,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_4,Bit_RESET);
		break;
	case 'D':
		GPIO_WriteBit(GPIOA,LED_WEIGHT_0,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_1,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_2,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_4,Bit_RESET);
		break;
	case 'E':
		GPIO_WriteBit(GPIOA,LED_WEIGHT_0,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_1,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_2,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_4,Bit_RESET);
		break;
	case 'F':
		GPIO_WriteBit(GPIOA,LED_WEIGHT_0,Bit_RESET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_1,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_2,Bit_SET);
		GPIO_WriteBit(GPIOA,LED_WEIGHT_4,Bit_RESET);
		break;
	default:
		;

	}

}
