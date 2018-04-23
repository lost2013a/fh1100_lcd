#include "Mainmenu.h"
#include "sys.h"




#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_TEXT_0      (GUI_ID_USER + 0x01)
#define ID_TEXT_1      (GUI_ID_USER + 0x02)
#define ID_TEXT_2      (GUI_ID_USER + 0x03)
extern GUI_CONST_STORAGE GUI_BITMAP bmsta1;
extern GUI_CONST_STORAGE GUI_BITMAP bmsta2;
extern GUI_CONST_STORAGE GUI_BITMAP bmsta3;
extern GUI_CONST_STORAGE GUI_BITMAP bmsta4;
extern GUI_CONST_STORAGE GUI_BITMAP bmsta5;
extern GUI_CONST_STORAGE GUI_BITMAP bmsta6;
extern GUI_CONST_STORAGE GUI_BITMAP bmsta7;
extern GUI_CONST_STORAGE GUI_BITMAP bmsta8;
extern GUI_CONST_STORAGE GUI_BITMAP bmset7;
extern GUI_CONST_STORAGE GUI_BITMAP bmptp2;
uint8_t	s_ucSelIconIndex2 = 0;
static const BITMAP_ITEM _aBitmapItem2[] = 
{
	{&bmsta1,    "电源状态"},
	{&bmsta2,    "频率源驯\n服状态"},
	{&bmsta3,    "告警状态"},
	{&bmsta4,    "北斗状态"},
	{&bmsta5,    "GPS状态"},
	{&bmsta6,    "IRIG-B1\n状态"},
	{&bmsta7,    "IRIG-B2\n状态"},
	{&bmset7,    "PTP1状态"},
	{&bmptp2,    "PTP2状态"},
	{&bmsta8,    "插槽状态"},
};


static const GUI_WIDGET_CREATE_INFO _aDialogCreate0[] = 
{
	{ WINDOW_CreateIndirect,     "",ID_WINDOW_0, 0, 0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "装置状态",            	ID_TEXT_0, 12,	2,  150,	32, GUI_TA_HCENTER|GUI_TA_VCENTER, 0x64, 0},
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
			pMsgInfo.hWinSrc = hWinIconSta;
			pMsgInfo.Data.v = WM_NOTIFICATION_RELEASED;
			WM_SendMessage(pMsg->hWin, &pMsgInfo);	
			break;
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinMenuSta))
			{	
				GUI_EndDialog(hWinMenuSta, 0);Menuapp();//返回到主菜单窗口
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
							s_ucSelIconIndex2  = ICONVIEW_GetSel(pMsg->hWinSrc);
							if(WM_IsWindow(hWinMenuSta))
									{	
										GUI_EndDialog(hWinMenuSta, 0);APP_Setup(s_ucSelIconIndex2+20);//进入到设置APP窗口
									}	
						  break;
					}
				break;//end GUI_ID_ICONVIEW1:
			}
			break;//end WM_NOTIFY_PARENT:
		case WM_PRE_PAINT:		
			WM_SetFocus(hWinIconSta);
			break;	
		default:
			WM_DefaultProc(pMsg);
		  break;	
	}
}



void MenuSta(void) 
{
	uint8_t i;
  uint8_t j;
	hWinMenuSta = GUI_CreateDialogBox(_aDialogCreate0, GUI_COUNTOF(_aDialogCreate0), _cbDialog, WM_HBKWIN, 0, 0);
	hWin_msgto=hWinMenuSta;//进入到主菜单目录
	hWinIconSta = ICONVIEW_CreateEx(
	             85, 					   	
						   80, 						
							 700,    				
							 400, 			
	             hWinMenuSta, 				  
							 WM_CF_SHOW | WM_CF_HASTRANS,     
	             0,                
							 GUI_ID_ICONVIEW1, 	
							100, 				    
							108);		
	ICONVIEW_SetFont(hWinIconSta, &GUI_FontHZ24);

	j= GUI_COUNTOF(_aBitmapItem2);

	for (i = 0; i < j; i++) 
	{	
		ICONVIEW_AddBitmapItem(hWinIconSta, _aBitmapItem2[i].pBitmap, _aBitmapItem2[i].pText);
	}
	for (i = 0; i < j; i++) 
	{	
		ICONVIEW_SetTextColor(hWinIconSta, i ,GUI_BLACK);
	}
	
	ICONVIEW_SetBkColor(hWinIconSta, ICONVIEW_CI_SEL,GUI_BLUE| 0xb0000000);
	ICONVIEW_SetSpace(hWinIconSta, GUI_COORD_Y, 32);
	ICONVIEW_SetSpace(hWinIconSta, GUI_COORD_X, 32);
	ICONVIEW_SetIconAlign(hWinIconSta, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
	//ICONVIEW_SetTextAlign(hWinIconSta, ICONVIEW_IA_HCENTER|GUI_TA_BOTTOM);
	ICONVIEW_SetSel(hWinIconSta,s_ucSelIconIndex2);                 

}





