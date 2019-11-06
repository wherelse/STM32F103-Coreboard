#include "MAX44009.h"
#include "myiic.h"
#include "delay.h"

void MAX44009_Init(void)
{

	IIC_Init(); //IIC初始化
	//	MAX44009_WriteOneByte(0x05,0x55);
//	 MAX44009_WriteOneByte(0x05, 0x55);
	MAX44009_WriteOneByte(MAX44009_REG_CFG, 0x03);					  // Default mode: measurements performed every 800ms
																  // Auto range: CDR and integration time are automatically determined by
																  //             the internal autoranging circuitry of the IC
	MAX44009_WriteOneByte(MAX44009_REG_INTE, MAX44009_INT_DISABLE);//关闭中断
	MAX44009_WriteOneByte(MAX44009_REG_THT, MAX44009_IT_800ms);//积分时间800ms
	MAX44009_WriteOneByte(MAX44009_REG_THU, 0xff);
	MAX44009_WriteOneByte(MAX44009_REG_THL, 0x00);
   	//	MAX44009_WriteOneByte(0x06,0x66);
	//	MAX44009_WriteOneByte(0x07,0x77);
	MAX44009_WriteOneByte(0x02,0x80);//连续转换、手动模式、积分时间6.25MS
}

//在MAX44009指定地址读出一个数据
//ReadAddr:开始读数的地址
//返回值  :读到的数据
u8 MAX44009_ReadOneByte(u8 ReadAddr)
{
	u8 temp = 0;
	IIC_Start();

	IIC_Send_Byte(0x94);
	IIC_Wait_Ack();

	IIC_Send_Byte(ReadAddr);
	IIC_Wait_Ack();

	IIC_Start();
	IIC_Send_Byte(0X95); //进入接收模式
	IIC_Wait_Ack();

	temp = IIC_Read_Byte(0);

	IIC_Stop(); //产生一个停止条件
	return temp;
}
//在MAX44009指定地址写入一个数据
//WriteAddr  :写入数据的目的地址
//DataToWrite:要写入的数据
void MAX44009_WriteOneByte(u8 WriteAddr, u8 DataToWrite)
{
	IIC_Start();
	IIC_Send_Byte(0x94);
	IIC_Wait_Ack();

	IIC_Send_Byte(WriteAddr);
	IIC_Wait_Ack();

	IIC_Send_Byte(DataToWrite); //发送字节
	IIC_Wait_Ack();

	IIC_Stop(); //产生一个停止条件
		//delay_ms(10);
}

//在MAX44009里面读取流明值

float MAX44009_ReadLux(void)
{
	u8 exponent=0, mantissa=0;
	u8 High_Lux = 0, Low_Lux = 0;
	float result=0;
	High_Lux=MAX44009_ReadOneByte(0x03);
	Low_Lux=MAX44009_ReadOneByte(0x04);
	
	exponent = (High_Lux & 0xF0) >> 4;
	mantissa = (High_Lux & 0x0F) << 4;
	mantissa += Low_Lux & 0x0F;
//	result = mantissa * (1 << exponent) * 0.045;
	result=(float)(((0x00000001 << exponent) * (float)mantissa) * 0.045);
	return result;
}
//在MAX44009里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void MAX44009_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len)
{
	u8 t;
	for (t = 0; t < Len; t++)
	{
		MAX44009_WriteOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
	}
}

//在MAX44009里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址
//返回值     :数据
//Len        :要读出数据的长度2,4
u32 MAX44009_ReadLenByte(u16 ReadAddr, u8 Len)
{
	u8 t;
	u32 temp = 0;
	for (t = 0; t < Len; t++)
	{
		temp <<= 8;
		temp += MAX44009_ReadOneByte(ReadAddr + Len - t - 1);
	}
	return temp;
}

//在MAX44009里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void MAX44009_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
	while (NumToRead)
	{
		*pBuffer++ = MAX44009_ReadOneByte(ReadAddr++);
		NumToRead--;
	}
}
//在MAX44009里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void MAX44009_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
	while (NumToWrite--)
	{
		MAX44009_WriteOneByte(WriteAddr, *pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
