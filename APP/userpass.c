#include "mainmenu.h"
#include "sys.h"
#define ID_WINDOW_0   (GUI_ID_USER + 0x4f)
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x50)
#define ID_EDIT_0     (GUI_ID_USER + 0x51)
#define ID_TEXT_0     (GUI_ID_USER + 0x52)
#define ID_TEXT_1     (GUI_ID_USER + 0x53)

extern void CAN_SEND_APP12(void);


	static const GUI_WIDGET_CREATE_INFO _aDialog1[] = {
{ WINDOW_CreateIndirect, "", ID_WINDOW_0, 0,  0, 300, 150, 0, 0x0, 0 },
{ EDIT_CreateIndirect,     NULL,     ID_EDIT_0,   140, 30, 80,30, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 4 },
{ TEXT_CreateIndirect,     "确认密码", ID_TEXT_0,    40, 30, 100, 30, TEXT_CF_HCENTER|TEXT_CF_LEFT },
{ TEXT_CreateIndirect,     "请输入参数配置的确认密码",   ID_TEXT_1,    25, 90, 250, 30, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
};

static void _cbDialog1(WM_MESSAGE *pMsg) 
{
	WM_HWIN hItem;
	int Val;
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			break;
		case WM_INIT_DIALOG:
			 hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0 );
			 EDIT_SetDecMode(hItem, 0,   0, 9999, 0, 0);
		   EDIT_SetCursorAtChar(hItem,3);
		   hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1 );
		   TEXT_SetFont(hItem, &GUI_FontHZ16);
		   break;
    case MSG_SetUP:
		case MSG_SetInc:
			GUI_SendKeyMsg(GUI_KEY_UP, 1);
			break;
		case MSG_SetDOWM:
		case MSG_SetDec:	
			GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
			break;	
		case MSG_SetLEFT:
			GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
			break;
		case MSG_SetRIGHT:
			GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
			break;	
		
		case MSG_SetENTER:
		 hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0 );
		 Val = EDIT_GetValue(hItem);
		 if(Val == USERPASS)
		 {
			 if(DevSys.Login_cnt>0){
					CAN_SEND_APP12();
				  DevSys.Login_cnt=0;
			 }
			 GUI_EndDialog(pMsg->hWin, 1);hWin_msgto   = hWinSetapp;//退回到设置界面
			 WM_SendMessageNoPara(hWinSetapp, MSG_VerifyOK);
			 WM_SendMessageNoPara(hWinSetapp, MSG_FOUCE);
		 }
		 else
		 {
			 EDIT_SetValue(hItem,0);
			 hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1 );
			 TEXT_SetText(hItem, "密码错误，请重试或退出");
		 }
			break;
		case MSG_SetQIUT:
		  GUI_EndDialog(pMsg->hWin, 0); hWin_msgto   = hWinSetapp;//退回到设置界面
		  WM_SendMessageNoPara(hWinSetapp, MSG_FOUCE);
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

void Password_Verify(void)
{
	hWinPassv = GUI_CreateDialogBox(_aDialog1, GUI_COUNTOF(_aDialog1), _cbDialog1, WM_HBKWIN, 250, 120);
	hWin_msgto = hWinPassv;  
}
