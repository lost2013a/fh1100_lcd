
#include "mainmenu.h"
#include "sys.h"

#define xpos0  250
#define ypos0  50
#define ra     150
#define rap    150
static int aPoints[2] ;
GUI_RECT Rect = {xpos0, ypos0, xpos0+2*ra,ypos0+2*ra};

static void _DrawIt( void) 
{
	static int col = 0;
  static int i = 0;
	static int x;
	i++;
  if(i<20)
	{
		x=2;
	}
	else if(i<40)
  {
		x=6;
	}
	else if(i<60)
  {
		x=8;
	}
  else if(i<80)
  {
		x=1;
	}
	else if(i<100)
  {
		x=4; 
	}
  else if(i<120)
  {
		x=9;
	}
	else
	{ 
	 i=0;
	 x=5;
	}
	if(i%60==30)
		{
			col = (col&0xff00)+0x1000;	
		}
	GUI_SetColor(GUI_BLUE);
	GUI_SetPenSize( 20);
	GUI_DrawArc( xpos0+ra,ypos0+ra,ra-9, ra,0-9, 360 );
//	if(aPoints[0] > 2000)
//	{
//		i=0;
//		aPoints[0] =0;
//		aPoints[1] =100;
//	}
	aPoints[0]=x+aPoints[0];
  aPoints[1]=10-x+aPoints[1];
	GUI_SetPenSize( 10 );
	GUI_SetColor(col);
  GUI_AA_DrawArc( xpos0+ra,ypos0+ra,ra-8, ra-8,aPoints[0], aPoints[1] );
}



void Openapp(void)
{
	int timeout = 0;
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_FontHZ32);
	GUI_SetColor(GUI_WHITE);
	
	GUI_DispStringAt("系统初始化中", 320, 140);
	GUI_DispStringAt("请稍后...", 320, 180);
	GUI_SetColor(GUI_GREEN);
	aPoints[0] =0;
	aPoints[1] =100;
	while((DevSys.first_recv_Can0==0)&&(timeout<500))
		{
			timeout++;
			GUI_MEMDEV_Draw(&Rect,(GUI_CALLBACK_VOID_P*)&_DrawIt,0,0,0);//使用分段存储绘制
			GUI_Delay(10);
		}
	if(timeout>500-1)//超时了
	{	
		GUI_Clear();
		GUI_SetColor(GUI_BLACK);
		GUI_DispStringAt("系统初始化超时", 320, 140);
		GUI_DispStringAt("请检查!", 320, 180);
		timeout = 0;
		while(DevSys.first_recv_Can0==0)//继续等待DevSys.first_recv_Can0=1
		{
			if(KEY_Scan() == KEY5_E)
			{
				return;
			}
			GUI_Delay(10);
		}
	}
}
/*************************** End of file ****************************/

