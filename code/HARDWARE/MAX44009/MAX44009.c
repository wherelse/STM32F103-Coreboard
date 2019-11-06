#include "MAX44009.h"
#include "myiic.h"
#include "delay.h"

void MAX44009_Init(void)
{

	IIC_Init(); //IIC��ʼ��
	//	MAX44009_WriteOneByte(0x05,0x55);
//	 MAX44009_WriteOneByte(0x05, 0x55);
	MAX44009_WriteOneByte(MAX44009_REG_CFG, 0x03);					  // Default mode: measurements performed every 800ms
																  // Auto range: CDR and integration time are automatically determined by
																  //             the internal autoranging circuitry of the IC
	MAX44009_WriteOneByte(MAX44009_REG_INTE, MAX44009_INT_DISABLE);//�ر��ж�
	MAX44009_WriteOneByte(MAX44009_REG_THT, MAX44009_IT_800ms);//����ʱ��800ms
	MAX44009_WriteOneByte(MAX44009_REG_THU, 0xff);
	MAX44009_WriteOneByte(MAX44009_REG_THL, 0x00);
   	//	MAX44009_WriteOneByte(0x06,0x66);
	//	MAX44009_WriteOneByte(0x07,0x77);
	MAX44009_WriteOneByte(0x02,0x80);//����ת�����ֶ�ģʽ������ʱ��6.25MS
}

//��MAX44009ָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ
//����ֵ  :����������
u8 MAX44009_ReadOneByte(u8 ReadAddr)
{
	u8 temp = 0;
	IIC_Start();

	IIC_Send_Byte(0x94);
	IIC_Wait_Ack();

	IIC_Send_Byte(ReadAddr);
	IIC_Wait_Ack();

	IIC_Start();
	IIC_Send_Byte(0X95); //�������ģʽ
	IIC_Wait_Ack();

	temp = IIC_Read_Byte(0);

	IIC_Stop(); //����һ��ֹͣ����
	return temp;
}
//��MAX44009ָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
//DataToWrite:Ҫд�������
void MAX44009_WriteOneByte(u8 WriteAddr, u8 DataToWrite)
{
	IIC_Start();
	IIC_Send_Byte(0x94);
	IIC_Wait_Ack();

	IIC_Send_Byte(WriteAddr);
	IIC_Wait_Ack();

	IIC_Send_Byte(DataToWrite); //�����ֽ�
	IIC_Wait_Ack();

	IIC_Stop(); //����һ��ֹͣ����
		//delay_ms(10);
}

//��MAX44009�����ȡ����ֵ

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
//��MAX44009�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void MAX44009_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len)
{
	u8 t;
	for (t = 0; t < Len; t++)
	{
		MAX44009_WriteOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
	}
}

//��MAX44009�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
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

//��MAX44009�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void MAX44009_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
	while (NumToRead)
	{
		*pBuffer++ = MAX44009_ReadOneByte(ReadAddr++);
		NumToRead--;
	}
}
//��MAX44009�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void MAX44009_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
	while (NumToWrite--)
	{
		MAX44009_WriteOneByte(WriteAddr, *pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
