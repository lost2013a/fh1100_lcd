//////////////////////////////////////////////////////////////////////////////////	 
//创建日期:2016/5/25
//版本：V1.0
//ZX						  
////////////////////////////////////////////////////////////////////////////////// 
#include "tftlcd.h"
#include "delay.h" 
#include "fmc.h"
//写寄存器函数
//regval:寄存器值
 	
//extern  struct _lcd_dev lcddev;	
	
#if 1
void LCD_CmdWrite(vu8 regval)
{   
	regval=regval;		//使用-O2优化的时候,必须插入的延时
	TFTLCD->LCD_REG=(regval);
	//delay_us(1);
}



//写LCD数据
//data:要写入的值
void LCD_DataWrite(vu16 data)
{	  
	data=data;			//使用-O2优化的时候,必须插入的延时
	TFTLCD->LCD_RAM=(data>>8);	
	TFTLCD->LCD_RAM=(data&0xff);	
}

//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WriteReg(u8 LCD_Reg,u8 LCD_RegValue)
{	
	TFTLCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号	
	TFTLCD->LCD_RAM = LCD_RegValue;//写入数据	    		 
}	 



u8 LCD_DataRead(void)
{
	vu8 ram;			//防止被优化
	ram=TFTLCD->LCD_RAM;	
	return ram;	 
}					   
  
u8 LCD_StatusRead(void)
{
	vu8 ram;			//防止被优化
	ram=TFTLCD->LCD_REG;	
	return ram;	 
}	


//读寄存器
//LCD_Reg:寄存器地址
//返回值:读到的数据
u8 LCD_ReadReg(u8 LCD_Reg)
{										   
	LCD_CmdWrite(LCD_Reg);		//写入要读的寄存器序号
	//delay_us(1);		  
	return LCD_DataRead();		//返回读到的值
}   

//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
u16 LCD_ReadPoint(u16 Xpos,u16 Ypos)
{
 	vu16 color=0,temp;
	if(Xpos>=lcddev.width||Ypos>=lcddev.height)return 0;	//超过了范围,直接返回		    
	LCD_CmdWrite(0x4a);
	LCD_DataWrite(Xpos);  
  LCD_CmdWrite(0x4b);	   
	LCD_DataWrite(Xpos>>8);
  LCD_CmdWrite(0x4c);
	LCD_DataWrite(Ypos);
  LCD_CmdWrite(0x4d);	   
	LCD_DataWrite(Ypos>>8);
	
	
	
	  
 	temp=(LCD_DataRead()<<8);  		  						//实际坐标颜色
	delay_us(2);	  
	color=temp|LCD_DataRead(); 
	
	return color;						//其他IC
}		


#endif

 /**
  * @brief  FMC初始化
  * @param  无
  * @retval 无
  */
void FMC_Init(void)
{ 		
  GPIO_InitTypeDef  GPIO_InitStructure;
	FMC_NORSRAMInitTypeDef  FMC_NORSRAMInitStructure;
  FMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FMC_NORSRAMTimingInitTypeDef  timingRead; 
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);//使能PA,PC,PD,PE时钟  
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC,ENABLE);//使能FMC时钟  
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//PA12 推挽输出,控制复位
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PC11 输入LDC_BUSY
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//  输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//无上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	 /* 
  PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10   ------> FMC_D7
  PD11   ------> FMC_A16
  PD14   ------> FMC_D0
  PD15   ------> FMC_D1
  PD0   ------> FMC_D2
  PD1   ------> FMC_D3
  PD4   ------> FMC_NOE
  PD5   ------> FMC_NWE
  PD7   ------> FMC_NE1
  */
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = 0xF<<7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
	

  GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FMC); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource7,GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource11,GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FMC);
	
  readWriteTiming.FMC_AddressSetupTime = 4;	 //地址建立时间（ADDSET）为12个HCLK =72ns 
  readWriteTiming.FMC_AddressHoldTime = 0;	 //地址保持时间（ADDHLD）模式A未用到	
  readWriteTiming.FMC_DataSetupTime = 6;		 //数据保存时间为60个HCLK	=6*75=480ns
  readWriteTiming.FMC_BusTurnAroundDuration = 1;
  readWriteTiming.FMC_CLKDivision = 0x00;
  readWriteTiming.FMC_DataLatency = 0x00;
  readWriteTiming.FMC_AccessMode = FMC_AccessMode_A;	 //模式A 
	
	timingRead.FMC_AddressSetupTime = 4;
	timingRead.FMC_AddressHoldTime = 0;
	timingRead.FMC_DataSetupTime = 8;
	timingRead.FMC_BusTurnAroundDuration = 1;
	timingRead.FMC_CLKDivision = 0;
	timingRead.FMC_DataLatency = 0;
	timingRead.FMC_AccessMode = FMC_AccessMode_A;
	
 
  FMC_NORSRAMInitStructure.FMC_Bank = FMC_Bank1_NORSRAM1;//  使用NE1
  FMC_NORSRAMInitStructure.FMC_DataAddressMux = FMC_DataAddressMux_Disable; // 不复用数据地址
  FMC_NORSRAMInitStructure.FMC_MemoryType =FMC_MemoryType_SRAM;// FMC_MemoryType_SRAM; 
  FMC_NORSRAMInitStructure.FMC_MemoryDataWidth = FMC_NORSRAM_MemoryDataWidth_8b;//存储器数据宽度为8bit   
  FMC_NORSRAMInitStructure.FMC_BurstAccessMode =FMC_BurstAccessMode_Disable;// FMC_BurstAccessMode_Disable; 
  FMC_NORSRAMInitStructure.FMC_WaitSignalPolarity = FMC_WaitSignalPolarity_Low;
	FMC_NORSRAMInitStructure.FMC_AsynchronousWait=FMC_AsynchronousWait_Disable; 
  FMC_NORSRAMInitStructure.FMC_WrapMode = FMC_WrapMode_Disable;   
  FMC_NORSRAMInitStructure.FMC_WaitSignalActive = FMC_WaitSignalActive_BeforeWaitState;  
  FMC_NORSRAMInitStructure.FMC_WriteOperation = FMC_WriteOperation_Enable;	//  存储器写使能
  FMC_NORSRAMInitStructure.FMC_WaitSignal = FMC_WaitSignal_Disable;   
  FMC_NORSRAMInitStructure.FMC_ExtendedMode = FMC_ExtendedMode_Disable; // 读写使用相同的时序
  FMC_NORSRAMInitStructure.FMC_WriteBurst = FMC_WriteBurst_Disable; 
  FMC_NORSRAMInitStructure.FMC_ReadWriteTimingStruct = &timingRead; //读写时序
  FMC_NORSRAMInitStructure.FMC_WriteTimingStruct = &readWriteTiming;  //写时序
  FMC_NORSRAMInit(&FMC_NORSRAMInitStructure);  // 初始化FMC配置
  FMC_NORSRAMCmd(FMC_Bank1_NORSRAM1, ENABLE);  // 使能BANK1,NE1	
}	



/*******************************************************************************
* Function Name  : LCD_SetTextColor
* Description    : Sets the Text color.
* Input          : - Color: specifies the Text color code RGB(5-6-5).
* Output         : - TextColor: Text color global variable used by LCD_DrawChar
*                  and LCD_DrawPicture functions.
* Return         : None
*******************************************************************************/
void LCD_SetTextColor(u16 Color)
{
  TextColor = Color;
  LCD_CmdWrite(0x63);
	LCD_DataWrite((Color&0xf800)>>11);
	
  LCD_CmdWrite(0x64);
	LCD_DataWrite((Color&0x07e0)>>5);
	
	LCD_CmdWrite(0x65);
	LCD_DataWrite((Color&0x001f));
}

/*******************************************************************************
* Function Name  : LCD_SetBackColor
* Description    : Sets the Background color.
* Input          : - Color: specifies the Background color code RGB(5-6-5).
* Output         : - BackColor: Background color global variable used by 
*                  LCD_DrawChar and LCD_DrawPicture functions.
* Return         : None
*******************************************************************************/
void LCD_SetBackColor(u16 Color)
{
  BACK_COLOR = Color;

  LCD_CmdWrite(0x60);
	LCD_DataWrite((Color&0xf800)>>11);
  LCD_CmdWrite(0x61);
	LCD_DataWrite((Color&0x07e0)>>5);
	LCD_CmdWrite(0x62);
	LCD_DataWrite((Color&0x001f));
}










/**
  * @brief  LCD初始化化
  * @param  无
  * @retval 无
  */
void TFTLCD_Init(void) 
{  
	uint16_t id;
	FMC_Init();
	LCD_RST = 0;
	//GPIO_ResetBits(GPIOA, GPIO_Pin_12);//复位LCD
	delay_ms(5);	
  LCD_RST = 1;	
	//GPIO_SetBits(GPIOA, GPIO_Pin_12 );		 	 
	delay_ms(20);	
	
	
	TFTLCD->LCD_REG=0x00;
	//delay_us(3);
	id=TFTLCD->LCD_RAM;
	myprintf("读取液晶模块ID:%x",id);
	MYLCD_DEV_INIT();
		
	
	LCD_WriteReg(0x88,12);
	delay_ms(1);    
	LCD_WriteReg(0x89,02);
	delay_ms(1);
	/*  SYS_CLK = FIN * ( PLLDIVN [4:0] +1 ) / ((PLLDIVM+1 ) * ( 2^PLLDIVK [2:0] ))
							= 20M * (14 + 1) / ((0 + 1) * (2 ^ 2)) = 75MHz
	*/
	//LCD_WriteReg(0x10,0x00);	//16Bit色彩+8bit总线
	LCD_WriteReg(0x10,0x0c);	//16Bit色彩+8bit总线
	/*bit3:2 色彩深度设定 (Color Depth Setting)
		00b : 8-bpp 的通用TFT 接口， i.e. 256 色。
		1xb : 16-bpp 的通用TFT 接口， i.e. 65K 色。	 【选这个】

	bit1:0 MCUIF 选择
		00b : 8-位MCU 接口。
		1xb : 16-位MCU 接口。 【选这个】
	*/
	delay_us(10); 
	LCD_WriteReg(0x04,0x81);//pclk=sys_clk/2
	delay_ms(1);
 

	LCD_WriteReg(0x14, 0x63);	//x=800
	LCD_WriteReg(0x15, 0x00);	                 
	LCD_WriteReg(0x16, 0x03); 
	LCD_WriteReg(0x17, 0x03);
	LCD_WriteReg(0x18, 0x0B);
	
	LCD_WriteReg(0x19, 0xDF);	//y=480
	LCD_WriteReg(0x1A, 0x01);
	LCD_WriteReg(0x1B, 0x1F);
	LCD_WriteReg(0x1C, 0x00);
	LCD_WriteReg(0x1D, 0x16);
	LCD_WriteReg(0x1E, 0x00);
	LCD_WriteReg(0x1F, 0x01);
	
	
	
	LCD_WriteReg(0x30,0x00); //x0=0
	LCD_WriteReg(0x31,0x00); 
	LCD_WriteReg(0x34,0x1F); //x1=800
	LCD_WriteReg(0x35,0x03); 
	
	LCD_WriteReg(0x32,0x00); //y0=0
	LCD_WriteReg(0x33,0x00); 
	LCD_WriteReg(0x36,0xdf); //y1=480
	LCD_WriteReg(0x37,0x01); 

	delay_ms(10); 
	
	LCD_WriteReg(0x8a,0x81); //有BUG？ 	
	
	LCD_BackLightPwm(102); // 半亮
	TFTLCD_Clear(0xffff);
	LCD_BackLightOn();
	LCD_DisplayOn();
	

}


//REG[8A]
void LCD_BackLightOn(void)
{
	u8 temp;
	LCD_CmdWrite(0x8a);  
	temp = LCD_DataRead();
	temp |= 0x80 ;
	LCD_WriteReg(0x8a, temp);
 }

void LCD_BackLightOff(void)
{
	u8 temp;
	LCD_CmdWrite(0x8a);
	temp = LCD_DataRead();
	temp &= 0x7f ;
	LCD_WriteReg(0x8a, temp);  
 }


//pwm:背光等级,0~100.越大越亮.
void LCD_BackLightPwm(u8 pwm)
{	
	uint16_t outpwm=pwm*2.55;
	if(outpwm>0xff)
		{
			outpwm=0xff;
		}
	LCD_WriteReg(0x8b, outpwm);	//亮度0-100
}



void LCD_DisplayOn(void)
{
	LCD_WriteReg(0x01, 0x80);
}
	
void LCD_DisplayOff(void)
{
	LCD_WriteReg(0x01, 0x00);
}








void LCD_WriteRAM_Prepare(void)
{
  LCD_CmdWrite(0x02); //
}


void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
  LCD_CmdWrite(0x46);
	LCD_DataWrite(Xpos);  
  LCD_CmdWrite(0x47);	   
	LCD_DataWrite(Xpos>>8);
  LCD_CmdWrite(0x48);
	LCD_DataWrite(Ypos);
  LCD_CmdWrite(0x49);	   
	LCD_DataWrite(Ypos>>8);
}	







