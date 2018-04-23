
#include "mainmenu.h"
#include "sys.h"
extern GUI_CONST_STORAGE GUI_BITMAP bmlogin1;


void CAN_SEND_APP12(void) 			
{
	uint32_t send_id = 0x03102900;			//CANID
	uint8_t  msg[8]  = {0}; 
	uint8_t  can_err;
	if( DevSys.USER_Right == 1)//高级用户
		{		
			msg[0]= 1;	
		}
	can_err=CAN1_Send_Msg(msg,send_id,8);
	CAN_SHOW_ERR(can_err);
}



 void _cbCallback12(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int Val;
  switch (pMsg->MsgId) {
    case WM_PAINT:
	  GUI_DrawGradientV(0, 0, 800, 480, GUI_LIGHTBLUE, GUI_BLUE);
	  //GUI_DrawBitmap(&bmlogin1,250,115);
	  //GUI_SetColor(GUI_COL_WIN1);
      break;
    case WM_INIT_DIALOG:
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00 );
			EDIT_SetDecMode(hItem, 0,   0, 9999, 0, 0);
		  EDIT_SetFont(hItem, &GUI_FontHZ32);
//		  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP00 );
//			TEXT_SetTextColor(hItem, GUI_WHITE);
//		  TEXT_SetFont(hItem, &GUI_FontHZ32);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP01 );
			TEXT_SetFont(hItem, &GUI_FontHZ32);
      TEXT_SetTextColor(hItem, GUI_WHITE);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP02 );
			TEXT_SetFont(hItem, &GUI_FontHZ16);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP03 );
			TEXT_SetFont(hItem, &GUI_Font13_1);
		  break;
		/************************************************************
			按键消息常规处理
			*/	

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
		 hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00 );
		 Val = EDIT_GetValue(hItem);
		 if(Val == ADMIN_PASS)
		 {
			 if(WM_IsWindow(hWinSetapp))
				{
					DevSys.USER_Right = 1;
					DevSys.Login_cnt  = 1;
					//CAN_SEND_APP12();
					GUI_EndDialog(hWinSetapp, 0); Menuapp();//进入到主菜单窗口
				}
		 }
		 else  if(Val == DevSys.USER_PASS)
		 {
				if(WM_IsWindow(hWinSetapp))
				{
					DevSys.Login_cnt  = 1;
					//CAN_SEND_APP12();
					GUI_EndDialog(hWinSetapp, 0); Menuapp();//进入到主菜单窗口
				}
		 }
		 else
		 {
			 EDIT_SetValue(hItem,0);
			 hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP01 );
			 TEXT_SetText(hItem, "登录失败，请重试或退出");
		 }
			break;
		case MSG_SetQIUT:
		  if(WM_IsWindow(hWinSetapp))
				{
					GUI_EndDialog(hWinSetapp, 0);
					Topapp();//返回到顶层窗口
				}
			break;
		default:
			WM_DefaultProc(pMsg);
	
	
  }
	
}


