/*
 * led_effect.c
 *
 *  Created on: Apr 15, 2021
 *      Author: nieki
 */


#include "main.h"

void led_effect_stop(void)
{
	for(int i = 0 ; i < 4 ; i++)
		xTimerStop(handle_led_timer[i],portMAX_DELAY);
}

void led_effect(int n )
{
	led_effect_stop();
	xTimerStart(handle_led_timer[n-1], portMAX_DELAY);

}

void turn_off_all_leds(void)
{
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin,GPIO_PIN_RESET);
}


void turn_on_all_leds(void)
{
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin,GPIO_PIN_SET);
}

void turn_on_odd_leds(void)
{
	HAL_GPIO_WritePin(LD3_GPIO_Port, LED1,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD4_GPIO_Port, LED2,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD5_GPIO_Port, LED3,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD6_GPIO_Port, LED4,GPIO_PIN_RESET);
}


void turn_on_even_leds(void)
{
	HAL_GPIO_WritePin(LD3_GPIO_Port, LED1,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD4_GPIO_Port, LED2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD5_GPIO_Port, LED3,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD6_GPIO_Port, LED4,GPIO_PIN_SET);
}

void LED_control( int value )
{
  for(int i = 0 ; i < 4 ; i++)
	  HAL_GPIO_WritePin(LD3_GPIO_Port, (LED1 << i), ((value >> i)& 0x1));
}


void LED_effect1(void)
{
	static int flag = 1;
	(flag ^= 1) ? turn_off_all_leds() : turn_on_all_leds();
}


void LED_effect2(void)
{
	static int flag = 1;
	(flag ^= 1) ? turn_on_even_leds() : turn_on_odd_leds();
}

void LED_effect3(void)
{
	static int i = 0;
	LED_control( 0x1 << (i++ % 4) );
}


void LED_effect4(void)
{
	static int i = 0;
	LED_control( 0x08 >> (i++ % 4) );
}




