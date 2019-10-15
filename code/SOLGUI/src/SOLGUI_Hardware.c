#include "SOLGUI_Type.h"
#include "SOLGUI_Config.h"
#include "SOLGUI_Hardware.h"

//*******************����Ļ����ͷ�ļ���*******************
#include "oled.h"

//##########################����Ҫ�û�ʵ�ֵĲ��֡�############################

void SOLGUI_Hardware_Init(void)
{
//=========���û�Ӳ����ʼ����ֲ����=======
	OLED_Init();
	OLED_Clear();

//========================================
}

void SOLGUI_Hardware_FillScreen(u8 GRAM[SCREEN_X_WIDTH][SCREEN_Y_PAGE])
{
//=========���û�Ӳ����Ļ��亯����ֲ����=======
		OLED_FillScreen(GRAM);
//========================================
}

void SOLGUI_Hardware_ClearScreen(void)
{
//=========���û�Ӳ������������ֲ����=======
	//pixelC_HW_OLED_CLS();		
OLED_Clear();
//========================================	
}



