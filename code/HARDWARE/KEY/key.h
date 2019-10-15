#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


#define KEY_A GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) //PA6
#define KEY_B GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) //PB11
#define KEY_D GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) //PB1
#define KEY_U GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) //PB0
#define KEY_L GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) //PA7
#define KEY_R GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) //PB10

#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	2	//KEY1按下
#define KEY2_PRES	3	//KEY2按下
#define WKUP_PRES   4	//KEY_UP按下(即WK_UP/KEY_UP)


void KEY_Init(void);//IO初始化
unsigned char Key_Scan(void);
#endif
