//////////////////////////////////////////////////////////////////////////////////	 
//创建日期:2016/5/25
//版本：V1.0
//ZX						  
////////////////////////////////////////////////////////////////////////////////// 
#include "tftlcd.h"
#include "delay.h" 
#include "sys.h" 
#include "fmc.h"
u16 POINT_COLOR	=0x0000;	//画笔颜色
u16 BACK_COLOR	=0xFFE0;  //背景色 
u16 TextColor		;
struct _lcd_dev lcddev;	




void MYLCD_DEV_INIT(void)
{
  lcddev.width	=	800;
	lcddev.height	=	480;
	lcddev.dir		= 1;				//横屏还是竖屏控制：0，竖屏；1，横屏。	
	lcddev.wramcmd=	0x02;	
}




void TFTLCD_Clear(u16 Color)
{
  u32 index = 0;
  
  LCD_SetCursor(0,0); 

  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */

  for(index = 0; index < 384000; index++)
  {
    LCD_DataWrite(Color);
  }
 // while(1);  
}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position. 
* Output         : None
* Return         : None
*******************************************************************************/

		 




//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_DataWrite(POINT_COLOR);
}
//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_DataWrite(color); 
}	 




//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;	 
	for(i=sy;i<=ey;i++)
	{
		LCD_SetCursor(sx,i);      				//设置光标位置 
		LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
		for(j=0;j<xlen;j++)LCD_DataWrite(color);	//显示颜色 	    
	}	 
}  
//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//设置光标位置 
		LCD_WriteRAM_Prepare();     //开始写入GRAM
		for(j=0;j<width;j++)LCD_DataWrite(color[i*width+j]);//写入数据 
	}		  
}  
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    	   	 	  
}   
//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,char *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}


///**
// * @brief  缩放字模，缩放后的字模由1个像素点由8个数据位来表示
//										0x01表示笔迹，0x00表示空白区
// * @param  in_width ：原始字符宽度
// * @param  in_heig ：原始字符高度
// * @param  out_width ：缩放后的字符宽度
// * @param  out_heig：缩放后的字符高度
// * @param  in_ptr ：字库输入指针	注意：1pixel 1bit
// * @param  out_ptr ：缩放后的字符输出指针 注意: 1pixel 8bit
// *		out_ptr实际上没有正常输出，改成了直接输出到全局指针zoomBuff中
// * @param  en_cn ：0为英文，1为中文
// * @retval 无
// */
//void LCD_zoomChar(uint16_t in_width,	//原始字符宽度
//									uint16_t in_heig,		//原始字符高度
//									uint16_t out_width,	//缩放后的字符宽度
//									uint16_t out_heig,	//缩放后的字符高度
//									uint8_t *in_ptr,	//字库输入指针	注意：1pixel 1bit
//									uint8_t *out_ptr, //缩放后的字符输出指针 注意: 1pixel 8bit
//									uint8_t en_cn)		//0为英文，1为中文	
//{
//	uint8_t *pts,*ots;
//	//根据源字模及目标字模大小，设定运算比例因子，左移16是为了把浮点运算转成定点运算
//	unsigned int xrIntFloat_16=(in_width<<16)/out_width+1; 
//  unsigned int yrIntFloat_16=(in_heig<<16)/out_heig+1;
//	
//	unsigned int srcy_16=0;
//	unsigned int y,x;
//	uint8_t *pSrcLine;
//	uint8_t tempBuff[1024] = {0};
//	u32			uChar;
//	u16			charBit = in_width / 8;
//	u16			Bitdiff = 32 - in_width;
//	
//	//检查参数是否合法
//	if(in_width >= 32) return;												//字库不允许超过32像素
//	if(in_width * in_heig == 0) return;	
//	if(in_width * in_heig >= 1024 ) return; 					//限制输入最大 32*32
//	
//	if(out_width * out_heig == 0) return;	
//	if(out_width * out_heig >= ZOOMMAXBUFF ) return; //限制最大缩放 128*128
//	pts = (uint8_t*)&tempBuff;
//	
//	//为方便运算，字库的数据由1 pixel 1bit 映射到1pixel 8bit
//	//0x01表示笔迹，0x00表示空白区
//	if(en_cn == 0x00)//英文
//	{
//		//这里以16 * 24字库作为测试，其他大小的字库自行根据下列代码做下映射就可以
//		//英文和中文字库上下边界不对，可在此次调整。需要注意tempBuff防止溢出
//			pts+=in_width*4;
//			for(y=0;y<in_heig;y++)	
//			{
//				uChar = *(u32 *)(in_ptr + y * charBit) >> Bitdiff;
//				for(x=0;x<in_width;x++)
//					{
//						*pts++ = (uChar >> x) & 0x01;
//					}
//			}		
//	}
//	else //中文
//	{
//			for(y=0;y<in_heig;y++)	
//			{
//				/*源字模数据*/
//				uChar = in_ptr [ y * 3 ];
//				uChar = ( uChar << 8 );
//				uChar |= in_ptr [ y * 3 + 1 ];
//				uChar = ( uChar << 8 );
//				uChar |= in_ptr [ y * 3 + 2];
//				/*映射*/
//				for(x=0;x<in_width;x++)
//					{
//						if(((uChar << x) & 0x800000) == 0x800000)
//							*pts++ = 0x01;
//						else
//							*pts++ = 0x00;
//					}
//			}		
//	}

//	//zoom过程
//	pts = (uint8_t*)&tempBuff;	//映射后的源数据指针
//	ots = (uint8_t*)&zoomBuff;	//输出数据的指针
//	for (y=0;y<out_heig;y++)	/*行遍历*/
//    {
//				unsigned int srcx_16=0;
//        pSrcLine=pts+in_width*(srcy_16>>16);				
//        for (x=0;x<out_width;x++) /*行内像素遍历*/
//        {
//            ots[x]=pSrcLine[srcx_16>>16]; //把源字模数据复制到目标指针中
//            srcx_16+=xrIntFloat_16;			//按比例偏移源像素点
//        }
//        srcy_16+=yrIntFloat_16;				  //按比例偏移源像素点
//        ots+=out_width;						
//    }
//	/*！！！缩放后的字模数据直接存储到全局指针zoomBuff里了*/
//	out_ptr = (uint8_t*)&zoomBuff;	//out_ptr没有正确传出，后面调用直接改成了全局变量指针！
//	
//	/*实际中如果使用out_ptr不需要下面这一句！！！
//		只是因为out_ptr没有使用，会导致warning。强迫症*/
//	out_ptr++; 
//}			


///**
// * @brief  利用缩放后的字模显示字符
// * @param  Xpos ：字符显示位置x
// * @param  Ypos ：字符显示位置y
// * @param  Font_width ：字符宽度
// * @param  Font_Heig：字符高度
// * @param  c ：要显示的字模数据
// * @param  DrawModel ：是否反色显示 
// * @retval 无
// */
//void LCD_DrawChar_Ex(uint16_t Xpos, //字符显示位置x
//												uint16_t Ypos, //字符显示位置y
//												uint16_t Font_width, //字符宽度
//												uint16_t Font_Heig,  //字符高度 
//												uint8_t *c,						//字模数据
//												uint16_t DrawModel)		//是否反色显示
//{
//  uint32_t index = 0, counter = 0, xpos =0;
//  uint32_t  Xaddress = 0;
//  
//  xpos = Xpos*lcddev.width*3;
//  Xaddress += Ypos;
//  
//  for(index = 0; index < Font_Heig; index++)
//  {
//    
//    for(counter = 0; counter < Font_width; counter++)
//    {
//      if(*c++ == DrawModel)	//根据字模及反色设置决定显示哪种颜色
//      {
//        *(__IO uint16_t*)(CurrentFrameBuffer + (3*Xaddress) + xpos) = (0x00FFFF & BACK_COLOR);        //GB
//        *(__IO uint8_t*)(CurrentFrameBuffer + (3*Xaddress) + xpos+2) = (0xFF0000 & BACK_COLOR) >> 16; //R
//      }
//      else
//      {
//        *(__IO uint16_t*)(CurrentFrameBuffer + (3*Xaddress) + xpos) = (0x00FFFF & POINT_COLOR);        //GB
//        *(__IO uint8_t*)(CurrentFrameBuffer + (3*Xaddress) + xpos+2) = (0xFF0000 & POINT_COLOR) >> 16; //R
//      }
//      Xaddress++;
//    }
//      Xaddress += (LCD_PIXEL_WIDTH - Font_width);
//  }
//}




//void LCD_DisplayStringLineEx(uint16_t x, 		//字符显示位置x
//														 uint16_t y, 				//字符显示位置y
//														 uint16_t Font_width,	//要显示的字体宽度，英文字符在此基础上/2。注意为偶数
//														 uint16_t Font_Heig,	//要显示的字体高度，注意为偶数
//														 uint8_t size,	
//														 uint8_t *ptr,					//显示的字符内容
//														 uint16_t DrawModel)  //是否反色显示
//{
//	uint16_t refcolumn = x; //x坐标
//	uint16_t Charwidth;
//	uint8_t *psr;
//	uint8_t Ascii;	//英文
//	uint16_t usCh;  //中文
//	uint8_t ucBuffer [ 24*24/8 ];	
//	
//	//while ((refcolumn < lcddev.width) && ((*ptr != 0) & (((refcolumn + size) & 0xFFFF) >= size)))
//	while ((refcolumn < lcddev.width) && ((*ptr != 0) ))
//	{
//		if(*ptr > 0x80) //如果是中文
//		{
//			Charwidth = Font_width;
//			usCh = * ( uint16_t * ) ptr;				
//			usCh = ( usCh << 8 ) + ( usCh >> 8 );
//			macGetGBKCode ( ucBuffer, usCh );	//取字模数据
//			//缩放字模数据
//			LCD_zoomChar(24,24,Charwidth,Font_Heig,(uint8_t *)&ucBuffer,psr,1); 
//			//显示单个字符
//			LCD_DrawChar_Ex(y,refcolumn,Charwidth,Font_Heig,(uint8_t*)&zoomBuff,DrawModel);
//			refcolumn+=Charwidth;
//			ptr+=2;
//		}
//		else
//		{
//				Charwidth = Font_width / 2;
//				Ascii = *ptr - 32;
//				//缩放字模数据
//				LCD_zoomChar(16,24,Charwidth,Font_Heig,(uint8_t *)&LCD_Currentfonts->table[Ascii * LCD_Currentfonts->Height],psr,0);
//			  //显示单个字符
//				LCD_DrawChar_Ex(y,refcolumn,Charwidth,Font_Heig,(uint8_t*)&zoomBuff,DrawModel);
//				refcolumn+=Charwidth;
//				ptr++;
//		}
//	}
//}

void UI_DrawHColorLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, uint16_t *_pColor)
{
	uint16_t i;

	 LCD_SetCursor(_usX1, _usY1);
	
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	
	
	for (i = 0; i < _usWidth; i++)
	{
		LCD_DataWrite(*_pColor++);
	
		//RA8875_WriteData16(*_pColor++);
	}
	
}




