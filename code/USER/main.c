#include "includeFile.h"

/*------->定义FLASH相关变量和参数<----------------*/
//要写入到STM32 FLASH的参数数组
float Parameter_Buffer[8] = {0};				  //缓存数组
#define SIZE sizeof(Parameter_Buffer)			  //数组长度
#define FLASH_SAVE_ADDR 0X08000000 + (100 * 1024) //STM32F103CBT6 FLASH 空间为128K 将数据存储在100K-128K的区域，\
设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
float Datatemp[SIZE];
/*------------------------------------------------*/

float voltage = 0;
float lux;
int main(void)
{
	float lux=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	delay_init();									//延时函数初始化
	uart_init(115200);								//串口初始化为115200
	LED_Init();										//初始化LED
	Adc_Init();
	KEY_Init();										//初始化按键
	BEEP_Init();									//初始化蜂鸣器
	SOLGUI_Hardware_Init();							//OLED初始化
	SOLGUI_Init(&UI_MENU);							//菜单显示框架初始化
	SOLGUI_Refresh();								//OLED刷新
	TIM2_Int_Init(9999, 720 - 1);					//100ms中断，屏幕更新，按键读取
	MAX44009_Init();
	USB_Port_Set(1);	//USB连接
 	Set_USBClock();   
 	USB_Interrupts_Config();    
 	USB_Init();	
//	Beeptimes(1);
	while (1)
	{
		voltage = Get_Adc_Average(2,50) / 4096.0 * 3.45 * 2;
		lux=MAX44009_ReadLux();
		usb_printf("lux=%f\n",lux);
		delay_ms(1000);
	}
}



//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) //溢出中断
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除中断标志位
		SOLGUI_InputKey(Key_Scan());
		SOLGUI_Menu_PageStage();
		SOLGUI_Refresh(); //OLED刷新
	}
}

/*
*@菜单内容函数
*/
MENU_PAGE UI_MENU;
__M_PAGE(UI_MENU,"Menu", PAGE_NULL,
		 {
			 SOLGUI_Cursor(6, 0, 9);
			 SOLGUI_Widget_OptionText(0, "BAT:%fV",voltage);
			 SOLGUI_Widget_OptionText(1, "lux:%f",lux);
//			 SOLGUI_Widget_Spin(1, "ModeSelect:", INT8, 0, 1, &ControlMode_Flag);
//			 SOLGUI_Widget_GotoPage(2, &DATA_EDIT);
//			 SOLGUI_Widget_GotoPage(3, &CAN_IO);
//			 SOLGUI_Widget_OptionText(4, "Voltage = %fV", BusVoltage / 1000.0);
//			 SOLGUI_Widget_OptionText(5, "Current = %fA", BusCurrent / 1000000.0);
//			 SOLGUI_Widget_OptionText(6, "Power   = %fW", BusPower / 1000.0);
//			 SOLGUI_Widget_OptionText(7, "CalS=%fr/min ", CalSpeed);
//			 SOLGUI_Widget_OptionText(8, "Tempture= %d C", Tempture);
			 //SOLGUI_Widget_OptionText(5, "Tempture= %d C", Tempture);
		 });
void FlashSaveData(void)
{
//	Parameter_Buffer[0] = speed.kp;
//	Parameter_Buffer[1] = speed.ki;
//	Parameter_Buffer[2] = SetSpeed;
//	Parameter_Buffer[3] = current.kp;
//	Parameter_Buffer[4] = current.ki;
//	Parameter_Buffer[5] = SetCurrent;
//	Parameter_Buffer[6] = EncoderPulse;
//	//STMFLASH_Write(FLASH_SAVE_ADDR, (u16 *)Parameter_Buffer, SIZE);
//	FlashSaveFlag = 1;
}

void FlashInitData(void)
{
//	//STMFLASH_Read(FLASH_SAVE_ADDR, (u16 *)Datatemp, SIZE);
//	speed.kp = Datatemp[0];
//	speed.ki = Datatemp[1];
//	SetSpeed = Datatemp[2];
//	current.kp = Datatemp[3];
//	current.ki = Datatemp[4];
//	SetCurrent = Datatemp[5];
//	EncoderPulse = Datatemp[6];
}