/*************************************************************************************
* Test-program for Olimex “STM32-H103”, header board for “STM32F103RBT6”.
* After program start green LED (LED_E) will blink.
*
* Program has to be compiled with optimizer setting "-O0".
* Otherwise delay via while-loop will not work correctly.
*************************************************************************************/
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#ifndef __LED_H_
#define __LED_H_

#include"stm32f10x.h"

/*************定义所使用的IO口**************/
#define RCC_LED 	RCC_APB2Periph_GPIOB	
#define GPIO_LED 	GPIOB

/**************8位LED的IO口定义************/
#define PIN_LED 	(GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12\
| GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15)

/**************单独LED的IO口定义***********/
#define PIN_LED1 	GPIO_Pin_15

/**********打开IO时钟和初始化GPIO**********/
void LED_Init(void);

/**********设置八位LED的状态***************/
void LED_Sets(uint8_t data);

/**********LED打开或者关闭的函数 **********/
#define LED1(a) if(a)\
GPIO_WriteBit(GPIO_LED, PIN_LED1, Bit_SET);\
else GPIO_WriteBit(GPIO_LED, PIN_LED1, Bit_RESET);

#endif
/****************************************************************************
* Function Name  : LED_Init
* Description    : Configures the used GPIO of LED .
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void LED_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = PIN_LED;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
		RCC_APB2PeriphClockCmd(RCC_LED, ENABLE);//使能GPIO_LED

		GPIO_Init(GPIO_LED, &GPIO_InitStructure);	 //初始化GPIO_LED

}
/****************************************************************************
* Function Name  : LED_Sets
* Description    : 设置LED的状态：因我们只是用了8位，而STM32一次得设置16位，所
*以先读取在写入。
* Input          : data 
* Output         : None
* Return         : None
****************************************************************************/

void LED_Sets(uint8_t data)
{
	uint16_t setValue;

	setValue = GPIO_ReadOutputData(GPIO_LED);
	setValue &= 0x00ff;//我们使用的是16位中的高8位					
	setValue |= ((uint16_t)data << 8);
	GPIO_Write(GPIO_LED, setValue);
}


void Delay(uint16_t c);
/****************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

int main(void)
{
	uint8_t endValue = 0;
	uint8_t count = 0;
	endValue = 0x01;
	LED_Init();
	while(1)
	{	
		/****左移16次****/
		for(count=0; count<15; count++)
		{												 
			LED_Sets(endValue);

			/****当左移到最左边之后重新赋值****/
			if(endValue == 0x80)
			{
				endValue = 0x01;
			}
			else
			{
				endValue <<= 1;
			}
			
			Delay(10);
		}
		/****右移16次****/
		for(count=0; count<15; count++)
		{												 
			LED_Sets(endValue);

			/****当右移移到最右边之后重新赋值****/
			if(endValue == 0x01)
			{
				endValue = 0x80;
			}
			else
			{
				endValue >>= 1;
			}
			
			Delay(10);
		}
	}
}
/*******************************************************************************
* Function Name  : Delay
* Description    : 延时程序
* Input          : c
* Output         : None
* Return         : None
*******************************************************************************/

void Delay(uint16_t c)
{
	uint16_t a, b;
	for(; c>0; c--)
		for(a=1000; a>0; a--)
			for(b=1000; b>0; b--);
}

