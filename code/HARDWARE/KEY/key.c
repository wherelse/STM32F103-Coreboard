#include "includeFile.h"

								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA6.7

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB0.1.10.11

}


unsigned char Key_Scan(void)
{
	unsigned char num = 0x00;
	if ((KEY_A) == 0)
	{
		num = SOLGUI_KEY_BACK;
	}
	if ((KEY_B) == 0)
	{
		num = SOLGUI_KEY_OK;
	}
	if ((KEY_D) == 0)
	{
		num = SOLGUI_KEY_DOWN;
	}
	if ((KEY_U) == 0)
	{
		num = SOLGUI_KEY_UP;
	}
	if ((KEY_L) == 0)
	{
		num = SOLGUI_KEY_LEFT;
	}
	if ((KEY_R) == 0)
	{
		num = SOLGUI_KEY_RIGHT;
	}

	return num;
}
