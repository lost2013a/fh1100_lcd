#include "Mainmenu.h"
#include "sys.h"
#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_TEXT_0      (GUI_ID_USER + 0x01)
#define ID_TEXT_1      (GUI_ID_USER + 0x02)
#define ID_TEXT_2      (GUI_ID_USER + 0x03)
extern GUI_CONST_STORAGE GUI_BITMAP bmset1;
extern GUI_CONST_STORAGE GUI_BITMAP bmset2;
extern GUI_CONST_STORAGE GUI_BITMAP bmset3;
extern GUI_CONST_STORAGE GUI_BITMAP bmset4;
extern GUI_CONST_STORAGE GUI_BITMAP bmset5;
extern GUI_CONST_STORAGE GUI_BITMAP bmset6;
extern GUI_CONST_STORAGE GUI_BITMAP bmset7;
extern GUI_CONST_STORAGE GUI_BITMAP bmset8;
extern GUI_CONST_STORAGE GUI_BITMAP bmset9;
extern GUI_CONST_STORAGE GUI_BITMAP bmset10;
extern GUI_CONST_STORAGE GUI_BITMAP bmset11;
extern GUI_CONST_STORAGE GUI_BITMAP bmset12;
uint8_t	s_ucSelIconIndex1 = 0;
static const BITMAP_ITEM _aBitmapItem1[] = 
{
	{&bmset1,    "主从设置"},
	{&bmset2,    "串口设置"},
	{&bmset3,    "IP设置"},
	{&bmset4,    "延迟设置"},
	{&bmset11,   "监测管理"},
	{&bmset6,    "源优先级"},
	{&bmset7,    "PTP设置"},
	{&bmset8,    "信号设置"},
	{&bmset9,    "定位设置"},
	{&bmset10,   "模拟输出"},
	{&bmset5,    "时区设置"},
	{&bmset12,   "状态控制"},
	
};


static const GUI_WIDGET_CREATE_INFO _aDialogCreate0[] = 
{
	{ WINDOW_CreateIndirect,     "",ID_WINDOW_0, 0, 0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "参数设置",            	ID_TEXT_0, 12,	2,  150,	32, GUI_TA_HCENTER|GUI_TA_VCENTER, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100时间同步装置", ID_TEXT_1, 180,	9,  200,	16, GUI_TA_VCENTER, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",  	ID_TEXT_2, 180,	23, 200,	16, GUI_TA_VCENTER, 0x64, 0},
};
 
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	WM_MESSAGE pMsgInfo;
	int NCode, Id;
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
				GUI_SetColor(GUI_COL_WIN1);
		  GUI_FillRect(0, 39, 800, 480);
			//GUI_DrawGradientV(0, 39, 800, 480, GUI_LIGHTBLUE, GUI_BLUE);
		  GUI_SetPenSize(1);
		  GUI_SetColor(GUI_COL_TITLE2);
		  GUI_DrawLine(170,8,170,34);
		  GUI_DrawLine(0,37,800,37);
		  GUI_SetColor(GUI_COL_TITLE3);
		  GUI_DrawLine(0,38,800,38);
			break;
		case WM_INIT_DIALOG:
			{
				WINDOW_SetBkColor(pMsg->hWin, GUI_COL_TITLE1);
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
				TEXT_SetTextColor(hItem, GUI_WHITE);	
				TEXT_SetFont(hItem, &GUI_FontHZ32);
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
				TEXT_SetTextColor(hItem, GUI_GRAY_11);
				TEXT_SetFont(hItem, &GUI_FontHZ16);
				
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
				TEXT_SetTextColor(hItem, GUI_GRAY_11);
				TEXT_SetFont(hItem, &GUI_Font13B_1);
			}
			break;
//		case WM_TIMER:
//			{
//				WM_SetFocus(hWinIconSet);
//				WM_RestartTimer(pMsg->Data.v, 50);	
//			}
//			break;
		case MSG_SetUP:
			GUI_SendKeyMsg(GUI_KEY_UP, 1);
			break;
		case MSG_SetDOWM:
			GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
			break;	
		case MSG_SetLEFT:
			GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
			break;
		case MSG_SetRIGHT:
			GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
			break;	
		case MSG_SetENTER:
			pMsgInfo.MsgId = WM_NOTIFY_PARENT;
			pMsgInfo.hWinSrc = hWinIconSet;
			pMsgInfo.Data.v = WM_NOTIFICATION_RELEASED;
			WM_SendMessage(pMsg->hWin, &pMsgInfo);	
			break;
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinMenuSet))
			{	
				GUI_EndDialog(hWinMenuSet, 0);Menuapp();//返回到主菜单窗口
			}
			break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);     
			NCode = pMsg->Data.v;  
			switch (Id) 
			{
				case GUI_ID_ICONVIEW1:
					switch (NCode) 
					{
						case  WM_NOTIFICATION_RELEASED:
							s_ucSelIconIndex1  = ICONVIEW_GetSel(pMsg->hWinSrc);
							if(WM_IsWindow(hWinMenuSet))
									{	
										GUI_EndDialog(hWinMenuSet, 0);APP_Setup(s_ucSelIconIndex1);//进入到设置APP窗口
									}	
						  break;
					}
				break;//end GUI_ID_ICONVIEW1:
			}
			break;//end WM_NOTIFY_PARENT:
		case WM_PRE_PAINT:		
			WM_SetFocus(hWinIconSet);
			break;	
		default:
			WM_DefaultProc(pMsg);
		  break;	
	}
}



void MenuSet(void) 
{
	uint8_t i;
  uint8_t j;
	hWinMenuSet = GUI_CreateDialogBox(_aDialogCreate0, GUI_COUNTOF(_aDialogCreate0), _cbDialog, WM_HBKWIN, 0, 0);
	hWin_msgto=hWinMenuSet;//进入到主菜单目录
	hWinIconSet = ICONVIEW_CreateEx(
	             85, 					   	
						   80, 						
							 700,    				
							 400, 			
	             hWinMenuSet, 				  
							 WM_CF_SHOW | WM_CF_HASTRANS,     
	             0,                
							 GUI_ID_ICONVIEW1, 	
							 100, 				    
							 108);		
	ICONVIEW_SetFont(hWinIconSet, &GUI_FontHZ24);
	if( DevSys.USER_Right == 1 )
		j= GUI_COUNTOF(_aBitmapItem1);
	else
		j = 5;
	for (i = 0; i < j; i++) 
	{	
		ICONVIEW_AddBitmapItem(hWinIconSet, _aBitmapItem1[i].pBitmap, _aBitmapItem1[i].pText);
	}
	for (i = 0; i < j; i++) 
	{	
		ICONVIEW_SetTextColor(hWinIconSet, i ,GUI_BLACK);
	}
	ICONVIEW_SetBkColor(hWinIconSet, ICONVIEW_CI_SEL, GUI_BLUE| 0xb0000000);
	ICONVIEW_SetSpace(hWinIconSet, GUI_COORD_Y, 32);
	ICONVIEW_SetSpace(hWinIconSet, GUI_COORD_X, 32);
	ICONVIEW_SetIconAlign(hWinIconSet, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
	ICONVIEW_SetSel(hWinIconSet,s_ucSelIconIndex1);
//	WM_CreateTimer(WM_GetClientWindow(hWinMenuSet), 
//				   ID_TimerTime1, 	            
//				   20,                           
//				   0);                      

}





