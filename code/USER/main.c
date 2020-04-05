#include "includeFile.h"

/*------->定义FLASH相关变量和参数<----------------*/
//要写入到STM32 FLASH的参数数组
float Parameter_Buffer[8] = {0};                  //缓存数组
#define SIZE sizeof(Parameter_Buffer)             //数组长度
#define FLASH_SAVE_ADDR 0X08000000 + (100 * 1024) //STM32F103CBT6 FLASH 空间为128K 将数据存储在100K-128K的区域，\
设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
float Datatemp[SIZE];
/*------------------------------------------------*/

float voltage = 0;
float lux;
float Apertute[] = {1, 1.2, 1.4, 1.6, 1.8, 2, 2.2, 2.5, 2.8, 3.2, 3.5, 4, 4.5, 5, 5.6, 6.3, 7.1, 8, 9, 10, 11, 13, 14, 16, 18, 20, 22, 32, 45, 64};
float iso[] = {6, 12, 25, 50, 100, 125, 160, 200, 250, 320, 400, 500, 640, 800, 1000, 1250, 1600, 2000, 2500, 3200, 4000, 5000, 6400, 8000, 10000, 12800, 16000, 20000, 25600, 51200};
float exposuretime;
int Apertute_index = 1, iso_index = 4;
float ev;
int tmin, tsec, tfr, tmode;
	
	
void ShutterSpeed_calculate(void);
		
int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
  delay_init();                                   //延时函数初始化
  uart_init(115200);                              //串口初始化为115200
  LED_Init();                                     //初始化LED
  Adc_Init();
  KEY_Init();                   //初始化按键
  BEEP_Init();                  //初始化蜂鸣器
  SOLGUI_Hardware_Init();       //OLED初始化
  SOLGUI_Init(&UI_MENU);        //菜单显示框架初始化
  SOLGUI_Refresh();             //OLED刷新
  TIM2_Int_Init(9999, 720 - 1); //100ms中断，屏幕更新，按键读取
  MAX44009_Init();
  USB_Port_Set(1); //USB连接
  Set_USBClock();
  USB_Interrupts_Config();

	USB_Init();
  //	Beeptimes(1);
  while (1)
  {
    voltage = Get_Adc_Average(0, 50) / 4096.0 * 3.45 * 2;
    lux = MAX44009_ReadLux();
    exposuretime = pow(Apertute[Apertute_index], 2) * 64 / (lux * iso[iso_index]);
    ev = log(pow(Apertute[Apertute_index], 2)) / log(2) + log(1 / exposuretime) / log(2);
    ShutterSpeed_calculate();

    usb_printf("lux=%f\n", lux);
    delay_ms(1000);
  }
}
void ShutterSpeed_calculate(void)
{
  if (exposuretime > 60)
  {
    tmin = exposuretime / 60;
    tmode = 2;
  }
  else if (exposuretime < 0.75)
  {
    if (exposuretime < 0.000125)
    {
      tmode = 0;
    }
    else
    {
      tmode = 2;
    }

    if ((exposuretime <= 0.000188) && (exposuretime > 0.000125))
    {
      tfr = 8000;
    }
    if ((exposuretime <= 0.000375) && (exposuretime > 0.000188))
    {
      tfr = 4000;
    }
    if ((exposuretime <= 0.00075) && (exposuretime > 0.000375))
    {
      tfr = 2000;
    }
    if ((exposuretime <= 0.0015) && (exposuretime > 0.00075))
    {
      tfr = 1000;
    }
    if ((exposuretime <= 0.003) && (exposuretime > 0.0015))
    {
      tfr = 500;
    }
    if ((exposuretime <= 0.006) && (exposuretime > 0.003))
    {
      tfr = 250;
    }
    if ((exposuretime <= 0.012333) && (exposuretime > 0.006))
    {
      tfr = 125;
    }
    if ((exposuretime <= 0.025) && (exposuretime > 0.012333))
    {
      tfr = 60;
    }
    if ((exposuretime <= 0.05) && (exposuretime > 0.025))
    {
      tfr = 30;
    }
    if ((exposuretime <= 0.095833) && (exposuretime > 0.05))
    {
      tfr = 15;
    }
    if ((exposuretime <= 0.1875) && (exposuretime > 0.095833))
    {
      tfr = 8;
    }
    if ((exposuretime <= 0.375) && (exposuretime > 0.1875))
    {
      tfr = 4;
    }
    if ((exposuretime <= 0.75) && (exposuretime > 0.375))
    {
      tfr = 2;
    }
  }
  else if ((exposuretime >= 0.75) && (exposuretime < 60))
  {
    tsec = exposuretime; //Exposure in seconds
    tmode = 1;
  }
  if (lux <= 0)
    tmode = 0;
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
__M_PAGE(UI_MENU, "Menu", PAGE_NULL,
         {
           SOLGUI_Cursor(6, 0, 9);
           SOLGUI_Widget_OptionText(0, "BAT:%fV", voltage);
           SOLGUI_Widget_OptionText(1, "luminance:");
           SOLGUI_Widget_OptionText(2, "%flux", lux);
           switch (tmode)
           {
           case 0:
             SOLGUI_Widget_OptionText(3, "overload or no light");
             break;
           case 1:
             SOLGUI_Widget_OptionText(3, "%ds", tsec);
             break;
           case 2:
             SOLGUI_Widget_OptionText(3, "time:1/%d", tfr);
             break;
           }
           SOLGUI_Widget_OptionText(4, "EV:%f", round(ev));
           SOLGUI_Widget_OptionText(5, "ISO:%f", iso[iso_index]);
           SOLGUI_Widget_OptionText(6, "F%f", Apertute[Apertute_index]);
           SOLGUI_Widget_Spin(7, "isonum:", INT8, 0, 30, &iso_index);
           SOLGUI_Widget_Spin(8, "Anum:", INT8, 0, 30, &Apertute_index);
           //			 SOLGUI_Widget_Spin(1, "ModeSelect:", INT8, 0, 1, &ControlMode_Flag);
           //			 SOLGUI_Widget_GotoPage(2, &DATA_EDIT);
           //			 SOLGUI_Widget_GotoPage(3, &CAN_IO);
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