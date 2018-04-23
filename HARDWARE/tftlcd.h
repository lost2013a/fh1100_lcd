//////////////////////////////////////////////////////////////////////////////////	 
//创建日期:2016/5/25
//版本：V1.0
//ZX						  
////////////////////////////////////////////////////////////////////////////////// 
#ifndef __TFTLCD_H
#define __TFTLCD_H
#include "sys.h"
#include "fmc.h"
//LCD重要参数集
typedef struct  _lcd_dev
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u8  dir;				//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wramcmd;		//开始写gram指令
}lcd_dev; 

extern  struct _lcd_dev lcddev;	

extern u16 POINT_COLOR;	//画笔颜色
extern u16 BACK_COLOR;  //背景色 
extern u16 TextColor;

#define WHITE      0xFFFF		//白
#define BLACK      0x0000	  //黑	
#define BLUE       0x001F  	
#define BRED       0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED        0xF800
#define MAGENTA    0xF81F
#define GREEN      0x07E0
#define CYAN       0x7FFF
#define YELLOW     0xFFE0
#define BROWN 		 0XBC40 //棕色
#define BRRED 		 0XFC07 //棕红色
#define GRAY  		 0X8430 //灰色
//GUI颜色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY      0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)



void MYLCD_DEV_INIT(void);


void LCD_DrawPoint(u16 x,u16 y);
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);
u32 LCD_Pow(u8 m,u8 n);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,char *p);
       

void UI_DrawHColorLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, uint16_t *_pColor);




#endif
