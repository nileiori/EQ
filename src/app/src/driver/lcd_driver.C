/******************************************************************** 
//��Ȩ˵��	:Shenzhen E-EYE Co. Ltd., Copyright 2009- All Rights Reserved.
//�ļ�����	:Lcd_Driver.c		
//����		:Һ����ʾ����������
//�汾��	:
//������	:dxl
//����ʱ��	:2010.11
//�޸���	:
//�޸�ʱ��	:
//�޸ļ�Ҫ˵��	:
//��ע		://ʹ�õĿ���оƬ��ST7565
***********************************************************************/
#include "include.h"

/*********************************************************************
//��������	:Lcd_DisplayString(u8 Line, u8 StartColumn, u8 DisplayModule, u8 *pBuffer, u16 length)
//����		:��ʾһ���ַ��������ַ�����������Ӣ�Ļ��
//����		:Line����ʼ�У�ȡֵֻ��Ϊ1-4��StartColumm����ʼ�У�ȡֵΪ0-116��
//		:DisplayModule����ʾģʽ��0Ϊ����ģʽ��1Ϊ���ԣ�pBuffer��ָ���ַ��������ֽڣ�length���ַ�������
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
#ifndef NEW_LCD_DRIVER_GDI
void Lcd_DisplayString(u8 Line, u8 StartColumn, u8 DisplayModule, u8 *pBuffer, u16 length)	
{
	u8	*p;
	s8	LengthCount;
	u8	HighByte;
	u8	LowByte;
	u8	AsciiByte;
	u8	temp;
	u8	DispBuffer[33];
	s8	DispLine;
	u8	DispColumn;
	u8	num = 0;
	u32	Address;
	//u8	i;
	
	//��ʼ��Һ����ʾ��IO��
	Lcd_IoInitial();
	/*
	for(i=0; i<200; i++)
	{
	
	}
	*/
	//����в���
	if((Line > 4)||(0 == Line))
	{
		return ;//�д���,��ֻ��Ϊ1,2,3,4
	}
	//����в���
	if(StartColumn > (128-16))
	{
		return ;//�д���
	}
	//ָ���ַ���ͷ��,���ȼ�����0
	p = pBuffer;
	LengthCount = 0;
	DispLine = Line;
	DispColumn = StartColumn;
	
	//�����ʾֱ���ַ���ĩβ
	for(;LengthCount < length;)
	{
		temp = *p;
		if(temp > 0x80)	//�����ַ�
		{
			//��ȡ�����ַ�������
			HighByte = *p++;
			LowByte = *p++;
			if(0xFF == HighByte)//����û�г�ʼ����
			{
				HighByte = 0xA3;//��ʾΪ'@'
				LowByte = 0xC0;
			}
			//����ƫ�Ƶ�ַ
			Address = ((HighByte-0xa1)*94 + (LowByte-0xa1))*32;
			//��ȡ�ֿ�
			Lcd_ReadOneChineseCharacterFromFlash(Address, DispBuffer);
			//��ʾһ������
			Lcd_DisplayOneChineseCharacter(DispLine, DispColumn, DisplayModule, DispBuffer);
			//�м���
			DispColumn += 16;
			//�ж���һ�����Ǻ��ֻ���Ӣ��
			if( *p > 0x80)
			{
				num = 128 - 16;
			}
			else
			{
				num = 128 - 8;
			}
			//�м���
			if(DispColumn > num)//������һ��
			{
				if(112 == num)//��һ�ո�
				{
					//��ȡASCII�ַ�
					AsciiByte = ' ';
					//����ƫ�Ƶ�ַ
					Address = AsciiByte * 16;
					//��ȡ�ֿ�
					Lcd_ReadOneEnglishCharacterFromFlash(Address, DispBuffer);
					//��ʾһ��ASCII��
					Lcd_DisplayOneEnglishCharacter(DispLine, DispColumn, DisplayModule, DispBuffer);
				}
				DispColumn = 0;
				DispLine++;
				if(DispLine > 4)//�ص���һ��
				{
					return ;
					//DispLine = 1;
				}
				
			}
			//���ȼ�2
			LengthCount += 2;
		}
		else //ASCII�ַ�
		{
			//��ȡASCII�ַ�
			AsciiByte = *p++;
			//����ƫ�Ƶ�ַ
			Address = AsciiByte * 16;
			//��ȡ�ֿ�
			Lcd_ReadOneEnglishCharacterFromFlash(Address, DispBuffer);
			//��ʾһ��ASCII��
			Lcd_DisplayOneEnglishCharacter(DispLine, DispColumn, DisplayModule, DispBuffer);
			//�м���
			DispColumn += 8;
			//�ж���һ�����Ǻ��ֻ���Ӣ��
			if( *p > 0x80)
			{
				num = 128 - 16;
			}
			else
			{
				num = 128 - 8;
			}
			//�м���
			if(DispColumn > num)//������һ��
			{
				if(112 == num)//��һ�ո�
				{
					//��ȡASCII�ַ�
					AsciiByte = ' ';
					//����ƫ�Ƶ�ַ
					Address = AsciiByte * 16;
					//��ȡ�ֿ�
					Lcd_ReadOneEnglishCharacterFromFlash(Address, DispBuffer);
					//��ʾһ��ASCII��
					Lcd_DisplayOneEnglishCharacter(DispLine, DispColumn, DisplayModule, DispBuffer);
				}
				DispColumn = 0;
				DispLine++;
				if(DispLine > 4)//�ص���һ��
				{
					return ;
					//DispLine = 1;
				}
			}
			//���ȼ�1
			LengthCount++;
		}
	}		
}
#endif
/*********************************************************************
//��������	:Lcd_ReadOneEnglishCharacterFromFlash(u32 Address, u8 *pBuffer)
//����		:��FLASH�ж�ȡһ��Ӣ���ַ��ı���
//����		:Address��ƫ�Ƶ�ַ
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:��ȡ�ĳ���
//��ע		:
*********************************************************************/
#ifdef	HZK16_IN_CHIP
u8 Lcd_ReadOneEnglishCharacterFromFlash(u32 Address, u8 *pBuffer)
{
	u8	i;
	u8	*p;
	p = pBuffer;
	for(i=0; i<16; i++)
	{
		*p++ = READ_ASCII_BYTE(Address+i);
	}
	return 16;
}
#else
u8 Lcd_ReadOneEnglishCharacterFromFlash(u32 Address, u8 *pBuffer)
{
	u32	Address1;
	Address1 = Address + 0x40000;
	sFLASH_ReadBuffer(pBuffer, Address1, 16);
	return 16;
}
#endif
/*********************************************************************
//��������	:Lcd_DisplayOneEnglishCharacter(u8 Line, u8 StartColumn, u8 DisplayModule, u8 *pBuffer)
//����		:��ʾһ��Ӣ���ַ�
//����		:Line����ʼ�У�ȡֵֻ��Ϊ1-4��StartColumm����ʼ�У�ȡֵΪ0-124��
//		:DisplayModule����ʾģʽ��0Ϊ����ģʽ��1Ϊ���ԣ�pBuffer��ָ��Ӣ�ı���8�ֽڵ����ֽ�
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:0����������0���쳣
//��ע		:
*********************************************************************/
u8 Lcd_DisplayOneEnglishCharacter(u8 Line, u8 StartColumn, u8 DisplayModule, u8 *pBuffer)
{

    #ifdef NEW_LCD_DRIVER_GDI
    Lcd_DisplayString(Line,StartColumn,DisplayModule,pBuffer,1);
    #else
	u8	HighHalfByte;
	u8	LowHalfByte;
	u8	i;
	u8	StartPage;
	u8	tempchar;
	u8	*p;
	
	//����в���
	if((Line > 4)||(0 == Line))
	{
		return 1;//�д���,��ֻ��Ϊ1,2,3,4
	}
	//����в���
	if(StartColumn > (132-8))
	{
		return 2;//�д���
	}
	//ָ���׵�ַ
	p = pBuffer;
	//����ҳ��
	if(1 == Line)
	{
		StartPage = 3;
	}
	else if(2 == Line)
	{
		StartPage = 1;
	}
	else if(3 == Line)
	{
		StartPage = 7;
	}
	else if(4 == Line)
	{
		StartPage = 5;
	}
	else
	{
		return 1;//
	}
	//��ȡ�еĸ���λ
	HighHalfByte = (StartColumn >> 4) & 0x0f;
	//��ȡ�еĵ���λ
	LowHalfByte = StartColumn & 0x0f;
	//������ʼ��
	Lcd_WriteCom(LowHalfByte);
        Lcd_WriteCom(0x10 | HighHalfByte);
	//������ʼҳ
        Lcd_WriteCom(0xb0 | StartPage);
	//�����ϰ벿
	for(i=0; i<8; i++)
	{
		tempchar = *p++;
		if(0 != DisplayModule)
		{
			tempchar = tempchar ^ 0xff;
		}
		Lcd_WriteData(tempchar);
	}
	//������ʼ��
	Lcd_WriteCom(LowHalfByte);
        Lcd_WriteCom(0x10 | HighHalfByte);
	//������ʼҳ
        Lcd_WriteCom(0xb0 | (StartPage-1));
	//�����°벿
	for(i=0; i<8; i++)
	{
		tempchar = *p++;
		if(0 != DisplayModule)
		{
			tempchar = tempchar ^ 0xff;
		}
		Lcd_WriteData(tempchar);
	}
	#endif
	
	return 0;
}

/*********************************************************************
//��������	:ReadOneChineseCharacterFromFlash(u32	Address, u8 *pBuffer)
//����		:��FLASH�ж�ȡһ�����ֵı���
//����		:Address��ƫ�Ƶ�ַ
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:��ȡ�ĳ���
//��ע		:
*********************************************************************/
#ifdef HZK16_IN_CHIP
u8 Lcd_ReadOneChineseCharacterFromFlash(u32	Address, u8 *pBuffer)
{
	u8	i;
	u8	*p;
	p = pBuffer;
	for(i=0; i<32; i++)
	{
		*p++ = READ_HZK16_BYTE(Address+i);
	}
	return 32;
}
#else
u8 Lcd_ReadOneChineseCharacterFromFlash(u32 Address, u8 *pBuffer)
{
	u32	Address1;
	Address1 = Address;
	sFLASH_ReadBuffer(pBuffer, Address1, 32);
	return 32;
}
#endif
/*********************************************************************
//��������	:Lcd_DisplayOneChineseCharacter(u8 Line, u8 StartColumn, u8 DisplayModule, u8 *pBuffer)
//����		:��ʾһ������
//����		:Line����ʼ�У�ȡֵֻ��Ϊ1-4��StartColumm����ʼ�У�ȡֵΪ0-116��
//		:DisplayModule����ʾģʽ��0Ϊ����ģʽ��1Ϊ���ԣ�pBuffer��ָ���ֱ���16�ֽڵ����ֽ�
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:0����������0���쳣
//��ע		:
*********************************************************************/
#ifndef NEW_LCD_DRIVER_GDI
u8 Lcd_DisplayOneChineseCharacter(u8 Line, u8 StartColumn, u8 DisplayModule, u8 *pBuffer)
{
	u8	HighHalfByte;
	u8	LowHalfByte;
	u8	i;
	u8	StartPage;
	u8	tempchar;
	u8	*p = NULL;
	
	//����в���
	if((Line > 4)||(0 == Line))
	{
		return 1;//�д���,��ֻ��Ϊ1,2,3,4
	}
	//����в���
	if(StartColumn > (132-16))
	{
		return 2;//�д���
	}
	//ָ���׵�ַ
	p = pBuffer;
	//����ҳ��
	if(1 == Line)
	{
		StartPage = 3;
	}
	else if(2 == Line)
	{
		StartPage = 1;
	}
	else if(3 == Line)
	{
		StartPage = 7;
	}
	else if(4 == Line)
	{
		StartPage = 5;
	}
	else
	{
		return 1;//�д���
	}
	//��ȡ�еĸ���λ
	HighHalfByte = (StartColumn >> 4) & 0x0f;
	//��ȡ�еĵ���λ
	LowHalfByte = StartColumn & 0x0f;
	//������ʼ��
	Lcd_WriteCom(LowHalfByte);
        Lcd_WriteCom(0x10 | HighHalfByte);
	//������ʼҳ
        Lcd_WriteCom(0xb0 | StartPage);
	//�����ϰ벿
	for(i=0; i<16; i++)
	{
		tempchar = *p++;
		if(0 != DisplayModule)
		{
			tempchar = tempchar ^ 0xff;
		}
		Lcd_WriteData(tempchar);
	}
	//������ʼ��
	Lcd_WriteCom(LowHalfByte);
        Lcd_WriteCom(0x10 | HighHalfByte);
	//������ʼҳ
        Lcd_WriteCom(0xb0 | (StartPage-1));
	//�����°벿
	for(i=0; i<16; i++)
	{
		tempchar = *p++;
		if(0 != DisplayModule)
		{
			tempchar = tempchar ^ 0xff;
		}
		Lcd_WriteData(tempchar);
	}	
	return 0;
}
#endif
/*********************************************************************
//��������	:Lcd_InitialST7567(void)
//����		:��ʼ��Һ����ʾ��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void Lcd_InitialST7567(void)
{   
   	Lcd_WriteCom(0xe2);  //0xE2=B1110 0010,������λ
	LCD_Delay();

    	Lcd_WriteCom(0xa2);  //0xA2=B1010 0010,����LCDƫѹ��,0-1/9
	LCD_Delay();
	
	Lcd_WriteCom(0xa0);  //0xA0=B1010 0000,��ʾ��ַ����,0-����,������
	LCD_Delay();
	
	Lcd_WriteCom(0xc0);  //0xC0=B11000000,��ɨ��˳��ѡ��,0-��ͨ˳��
	LCD_Delay();
        
	Lcd_WriteCom(0x27);  //0x27=B0010 0111,�ڲ���ѹֵ��������,(20h-27h) 27h ,�Ľ���һ�������,����ʹ�õ�
	
	
	LCD_Delay();
        
	Lcd_WriteCom(0x81);  //0x81=B1000 0001,�����ڲ�����΢��,�����ڲ�����΢����������Һ����ѹ��������ָ���������ʹ��
	LCD_Delay();
        
	Lcd_WriteCom(0x32);  //0x32=B0011 0010,���õ�ѹֵ(0~63��),�Ľ���һ�������,����ʹ�õ�
	
	LCD_Delay();
	
	Lcd_WriteCom(0x2f);  //0x2F=B0010 1111,��Դ���Ƽ�,ѡ���ڲ���ѹ��Ӧ����ģʽ��
	LCD_Delay();
	
    	Lcd_WriteCom(0x40);  //0x40=B0100 0000,��ʾ��ʼ������
	LCD_Delay();
	Lcd_WriteCom(0xb0);  //0xB0=B1011 0000,��ʾҳ��ַ����4 λ,ҳ0
	LCD_Delay();
	Lcd_WriteCom(0xb7);  //0xB7=B10110111,��ʾҳ��ַ����4 λ,ҳ7,��ҳ����0~7����ҳ
	LCD_Delay();
	
	Lcd_WriteCom(0x01);  //0x01=B0000 0001 �����е�ַ��4λ,-1
	LCD_Delay();
	Lcd_WriteCom(0x10);  //0x01=B0001 0000 �����е�ַ��4λ,-0,����B0000 0001 =1��
	LCD_Delay();
	
	Lcd_WriteCom(0x00);  //0x00=B0000 0000 �����е�ַ��4λ,-0
	LCD_Delay();
	Lcd_WriteCom(0x18);  //0x01=B0001 1000 �����е�ַ��4λ,-1000,����B1000 0000 = 128��
	LCD_Delay();
	Lcd_WriteCom(0xaf);  //0xAF=B1010 1111, ��ʾ��
	LCD_Delay();
}
/*********************************************************************
//��������	:Lcd_BeepIoInit(void)
//����		:������IO��ʼ��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void Lcd_BeepIoInit(void)
{
	GpioOutInit(BUZZER);
}
/*********************************************************************
//��������	:Lcd_IoInitial(void)
//����		:Һ��ģ��IO�ڳ�ʼ��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void Lcd_IoInitial(void)
{
	GpioOutInit(LCD_ON);
        GpioOutInit(LCD_CS);
        GpioOutInit(LCD_AO);
        GpioOutInit(LCD_SPI_SCK);
        GpioOutInit(LCD_SPI_MISO);
        GpioOutInit(LCD_SPI_MOSI);
        GpioOutOn(LCD_CS);              //ȡ��Ƭѡ
        GpioOutOn(LCD_SPI_MOSI);//dxl,2014.1.14����
        GpioOutOn(LCD_SPI_SCK);//dxl,2014.1.14����
        GpioOutOn(LCD_AO);//dxl,2014.1.14����
}

/*********************************************************************
//��������	:Lcd_WriteCom(u8 com)
//����		:дһ���ֽڵ�����
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
/*dxl,2014.1.9����,ԭ������������
void Lcd_WriteCom(u8 com)
{
    u8 i;

	LCD_CS_LOW();
	LCD_A0_LOW();
    for(i=0;i<8;i++)
		{
		  if(0x80 == (0x80&com))
		  {
		  	LCD_DA_HIGH();
		  }
		  else
		  {
		  	LCD_DA_LOW();
		  }
		  
          	LCD_SCK_LOW();
		LCD_SCK_HIGH();
		//LCD_SCK_HIGH();
		LCD_SCK_LOW();
		  com=com<<1;
		}
	LCD_A0_HIGH();
	LCD_CS_HIGH();
}
*/
void Lcd_WriteCom(u8 com)
{
    u8 i;

	LCD_CS_LOW();
        LCD_SCK_HIGH();

        for(i=0;i<8;i++)
	{
		if(0x80 == (0x80&com))
		{
		  	LCD_DA_HIGH();
		}
		else
		{
		  	LCD_DA_LOW();
		}
                if(4 == i)
                {
                        LCD_A0_LOW();
                }
                
          	LCD_SCK_LOW();
		LCD_SCK_HIGH();
		com=com<<1;
	}
	LCD_CS_HIGH();
}
/*********************************************************************
//��������	:Lcd_WriteData(u8 dat)
//����		:дһ���ֽڵ�����
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
/*dxl,2014.1.9����,ԭ������������
void Lcd_WriteData(u8 dat)
{
    	u8 i;
		
	LCD_CS_LOW();
	LCD_A0_HIGH();
    	for(i=0;i<8;i++)
	{
		if(0x01 == (0x01&dat))
		{
		  	LCD_DA_HIGH();
		}
		else
		{
			LCD_DA_LOW();
		}
          	LCD_SCK_LOW();
		//LCD_SCK_HIGH();
		LCD_SCK_HIGH();
		LCD_SCK_LOW();
		dat=dat >> 1;
	}
	LCD_A0_LOW();
	LCD_CS_HIGH();
}
*/
void Lcd_WriteData(u8 dat)
{
    	u8 i;
		
	LCD_CS_LOW();
        LCD_SCK_HIGH();
    	for(i=0;i<8;i++)
	{
		if(0x01 == (0x01&dat))
		{
		  	LCD_DA_HIGH();
		}
		else
		{
			LCD_DA_LOW();
		}
                if(4 == i)
                {
                        LCD_A0_HIGH(); 
                }
          	LCD_SCK_LOW();
		LCD_SCK_HIGH();
		dat=dat >> 1;
	}
	LCD_CS_HIGH();
}
/*********************************************************************
//��������	:Lcd_DisplayClr(void)
//����		:���������Ļ
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void Lcd_DisplayClr(void)
{
    #ifdef NEW_LCD_DRIVER_GDI
    LcdClearScreen();
    #else
	u8 i,j;

	for(i=0;i<8;i++)
       	{
         	Lcd_WriteCom(0x01);
        	Lcd_WriteCom(0x10);
         	Lcd_WriteCom(0xb0|i);

         	for(j=0;j<132;j++)
            	{
              		Lcd_WriteData(0x00);
            	}
       }
    #endif
}
/*********************************************************************
//��������	:Lcd_Initial(void)
//����		:lcdģ���ʼ��
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void Lcd_Initial(void)
{
	Lcd_BeepIoInit();
	Lcd_IoInitial();	
    
	Lcd_InitialST7567();
	Lcd_DisplayClr();	
    
}
/*********************************************************************
//��������	:ChangeGraphFormatToChineseFormat(u8 *pGraphBuffer, u8 *pChineseBuffer)
//����		:��������֮ǰͼ�θ�ʽ�ı���ת���ɺ��ֱ���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:����Buffer�ĳ��Ⱦ�Ϊ32�ֽ�
*********************************************************************/
void ChangeGraphFormatToChineseFormat(u8 *pChineseBuffer, u8 *pGraphBuffer)
{
	u8	i;
	u8	j;
	u8	temp;
	u8	NewChar;
	u8	MoveBit = 0x01;
	u8	*pC;
	u8	*pG;
	
	pC = pChineseBuffer;
	
	//ת�����ϲ�
	for(i=0;i<8; i++)
	{
		NewChar = 0;
		pG = pGraphBuffer;
		for(j=0; j<8; j++)
		{
			//ȡ8��ͼ���ֽڵĵ�(7-i)λ
			temp = *pG++;
			temp &= (MoveBit << (7-i));
			temp = temp >> (7-i);
			NewChar |= (temp << j);
		}
		*pC++ = NewChar;
	}
		
	//ת�����ϲ�
	for(i=0;i<8; i++)
	{
		NewChar = 0;
		pG = pGraphBuffer+16;
		for(j=0; j<8; j++)
		{
			//ȡ8��ͼ���ֽڵĵ�(7-i)λ
			temp = *pG++;
			temp &= (MoveBit << (7-i));
			temp = temp >> (7-i);
			NewChar |= (temp << j);
		}
		*pC++ = NewChar;
	}
		
	//ת�����²�
	for(i=0;i<8; i++)
	{
		NewChar = 0;
		pG = pGraphBuffer+8;
		for(j=0; j<8; j++)
		{
			//ȡ8��ͼ���ֽڵĵ�(7-i)λ
			temp = *pG++;
			temp &= (MoveBit << (7-i));
			temp = temp >> (7-i);
			NewChar |= (temp << j);
		}
		*pC++ = NewChar;
	}
		
	//ת�����²�
	for(i=0;i<8; i++)
	{
		NewChar = 0;
		pG = pGraphBuffer+24;
		for(j=0; j<8; j++)
		{
			//ȡ8��ͼ���ֽڵĵ�(7-i)λ
			temp = *pG++;
			temp &= (MoveBit << (7-i));
			temp = temp >> (7-i);
			NewChar |= (temp << j);
		}
		*pC++ = NewChar;
	}
}
/*********************************************************************
//��������	:displayfull
//����		:������ʾ,ר���ڲ���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void displayfull(void)
{
    u8 i,j;

    for(i=0;i<8;i++)
    {
         Lcd_WriteCom(0x01);
         Lcd_WriteCom(0x10);
         Lcd_WriteCom(0xb0|i);

         for(j=0;j<128;j++)
         {
            Lcd_WriteData(0xff);
         }
     }
}
/*********************************************************************
//��������	:LCD_Delay
//����		:�ӳٺ���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:
*********************************************************************/
void LCD_Delay(void)
{
    __IO uint32_t i = 0;
    for(i = 0xFF; i != 0; i--)
    {
    }
}
/*********************************************************************
//��������	:lcd_delay(u32 n)
//����		:�ӳٺ���
//����		:
//���		:
//ʹ����Դ	:
//ȫ�ֱ���	:   
//���ú���	:
//�ж���Դ	:  
//����		:
//��ע		:��ʱn��for���ʱ��
*********************************************************************/

static void lcd_delay(u32 n)
{
	for(;n>0;n--);
}
