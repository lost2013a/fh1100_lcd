
#include "mainmenu.h"
#include "sys.h"


typedef struct 
{
	WM_CALLBACK * cb;
} APP_LIST_ITEM;

static void PAINTTITL_SETAPP(uint8_t line)
{
	int i;
	/*画标题栏*/
	GUI_SetColor(GUI_COL_TITLE1);
	GUI_FillRect(0, 0, 800, 36 );
	GUI_SetPenSize(1);
	GUI_SetColor(GUI_WHITE);
	GUI_DrawLine(170,8,170,34);
	GUI_DrawLine(0,37,800,37);
	GUI_SetColor(GUI_BLACK);
	GUI_DrawLine(0,38,800,38);
	/*画正文框*/
	GUI_SetColor(GUI_WHITE);
	GUI_FillRoundedRect(20,70,700,440,8);
	/*画装饰的线条*/
	GUI_SetColor(GUI_COL_WIN1);
	GUI_SetPenSize(3);
	GUI_DrawLine(80,112,600,112);
	GUI_SetPenSize(1);
	GUI_DrawLine(XPOS0-10,YPOS0-5,XPOS0-10,YPOS0+YGAPS*line);
	for (i = 1; i < line+1; i++) 
	 {
		GUI_DrawLine(80,112+YGAPS*i,600,112+YGAPS*i);
	 }
}

static void PAINTWIN_SETAPP(WM_MESSAGE * pMsg)
{

  WM_HWIN hItem = pMsg->hWin;
	WINDOW_SetBkColor(pMsg->hWin, GUI_COL_WIN1); //布置背景色
	//布置标题栏
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP00);
	TEXT_SetTextColor(hItem, GUI_WHITE);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetFont(hItem, &GUI_FontHZ24);
		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP01);
	TEXT_SetTextColor(hItem, GUI_GRAY_11);
	TEXT_SetTextAlign(hItem,   GUI_TA_VCENTER);
	TEXT_SetFont(hItem, &GUI_FontHZ16);
		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP02);
	TEXT_SetTextColor(hItem, GUI_GRAY_11);
	TEXT_SetTextAlign(hItem,   GUI_TA_VCENTER);
	TEXT_SetFont(hItem, &GUI_Font13B_1);
}


static void _cbCallback00(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			PAINTTITL_SETAPP(1);/*画标题栏和窗口*/
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);//布置标题栏
		  //布置下拉框
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			DROPDOWN_SetFont(hItem, &GUI_FontHZ24); 
			DROPDOWN_AddString(hItem, "主时钟");  
			DROPDOWN_AddString(hItem, "从时钟");
      break;
 		/************************************************************
			按键特殊处理
			*/	
		
		case MSG_SetUP:
		case MSG_SetDOWM:
		case MSG_SetLEFT:
		case MSG_SetRIGHT:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			WM_SetFocus (hItem);
			break;
		/************************************************************
			按键消息常规处理
			*/	

	  case MSG_SetInc:		
			GUI_SendKeyMsg(GUI_KEY_UP, 1);
	 		break;
	  case MSG_SetDec:			
			GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
			break;
		case MSG_SetENTER:
			Password_Verify();
		  break;
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinSetapp))
			{	
				 GUI_EndDialog(hDlg, 0);hWin_msgto=hWinMenuSet;MenuSet();//返回到设置菜单窗口
			}
			break;	
		/************************************************************
			设置功能的实现
			*/	
		case MSG_VerifyOK:
			{
				myprintf("set ok ");
			}
			break;
		case WM_TIMER:
			{
			
			}
			WM_RestartTimer(pMsg->Data.v, Timer2value);	
			break;	
    case WM_NOTIFY_PARENT:
      break;
    default:
      WM_DefaultProc(pMsg);
  }
	
}


static void _cbCallback02(WM_MESSAGE * pMsg) 
{
	/*IP设置中使用了静态变量来实现光标位置的变化*/
	static WM_HWIN xitems[6];      //控件划分成6行，记录每行第一个控件的句柄。
	static uint8_t xsel  , ysel;   //光标的位置
	static uint8_t ITEMS = 16;     //编辑框的数目
	const uint8_t _aDefaultSep[16] = {192,168, };  //编辑框的初始值
  WM_HWIN hDlg;
  WM_HWIN hItem;
  int     i;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			/*画标题栏和窗口*/
		  PAINTTITL_SETAPP(6);
			/*填写网络地址的.号*/ 
		  GUI_SetTextMode(GUI_TM_TRANS);
		  GUI_SetColor(GUI_BLACK);
		  for (i = 2; i < 6; i++) 
		   {
		    GUI_DispCharAt('.',XPOS0+XGAPS*1-8,YPOS0+YGAPS*i+14);
		    GUI_DispCharAt('.',XPOS0+XGAPS*2-8,YPOS0+YGAPS*i+14);
			  GUI_DispCharAt('.',XPOS0+XGAPS*3-8,YPOS0+YGAPS*i+14);
		   }
       break;
    case WM_INIT_DIALOG:
			//布置标题栏
       PAINTWIN_SETAPP(pMsg);
		  //布置下拉框
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			DROPDOWN_SetFont(hItem, &GUI_FontHZ16); 
			DROPDOWN_AddString(hItem, "CPU板");  
			DROPDOWN_AddString(hItem, "扩展板1");
			DROPDOWN_AddString(hItem, "扩展板2");
			DROPDOWN_AddString(hItem, "扩展板3");
			DROPDOWN_AddString(hItem, "扩展板4");
			DROPDOWN_AddString(hItem, "扩展板5");
			DROPDOWN_AddString(hItem, "扩展板6");
			DROPDOWN_AddString(hItem, "扩展板7");
			DROPDOWN_AddString(hItem, "扩展板8");
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);  
			DROPDOWN_SetFont(hItem, &GUI_FontHZ16); 
			DROPDOWN_AddString(hItem, "网口1");
			DROPDOWN_AddString(hItem, "网口2");
			DROPDOWN_AddString(hItem, "网口3");
			DROPDOWN_AddString(hItem, "网口4");
			DROPDOWN_AddString(hItem, "网口5");
			DROPDOWN_AddString(hItem, "网口6");
      
       //布置EDIT框
      for (i = 0; i < ITEMS; i++) 
	    {
        hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + i);
        EDIT_SetDecMode(hItem, _aDefaultSep[i], 0, 255, 0, 0);
      }
      //控件划分成6行，记录每行第一个控件的句柄
			xitems[0] = WM_GetDialogItem(hDlg, ID_DROPDOWN_APP0 );
			xitems[1] = WM_GetDialogItem(hDlg, ID_DROPDOWN_APP1 );
			xitems[2] = WM_GetDialogItem(hDlg, ID_EDIT_APP00 );
		  xitems[3] = WM_GetDialogItem(hDlg, ID_EDIT_APP04 );
			xitems[4] = WM_GetDialogItem(hDlg, ID_EDIT_APP08 );
			xitems[5] = WM_GetDialogItem(hDlg, ID_EDIT_APP12 );
      break;
		/************************************************************
			按键消息特殊处理
			*/	
		case MSG_SetUP:
			ysel = (ysel+5)%6;
		  WM_SetFocus (xitems[ysel]);
		  if(ysel>1)EDIT_SetCursorAtChar(xitems[ysel],0);
		  xsel = 0;
		  myprintf("MSG_SetUP");
			break;
		case MSG_SetDOWM:
			ysel = (ysel+1)%6;
		  WM_SetFocus (xitems[ysel]);
		  if(ysel>1) EDIT_SetCursorAtChar(xitems[ysel],0);
		  xsel = 0;
		 myprintf("MSG_SetDOWM");
			break;	
		case MSG_SetLEFT:
			if(ysel>1)
			{
				xsel = (xsel+11)%12;
				hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + (ysel-2)*4 + xsel/3);
				WM_SetFocus (hItem);	
				EDIT_SetCursorAtChar(hItem,xsel%3);
				myprintf("MSG_SetLEFT");
			}
			break;
		case MSG_SetRIGHT:
			if(ysel>1)
			{
				xsel = (xsel+1)%12;
				hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + (ysel-2)*4 + xsel/3);
				WM_SetFocus (hItem);	
				EDIT_SetCursorAtChar(hItem,xsel%3);
				myprintf("MSG_SetRIGHT");
			}
			break;
 		/************************************************************
			按键消息常规处理
			*/	
	  case MSG_SetInc:		
			GUI_SendKeyMsg(GUI_KEY_UP, 1);
	 		break;
	  case MSG_SetDec:			
			GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
			break;
		case MSG_SetENTER:
			Password_Verify();
		  break;
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinSetapp))
			{	
				 GUI_EndDialog(hDlg, 0);hWin_msgto=hWinMenuSet;MenuSet();//返回到设置菜单窗口
			}
			break;	
		/************************************************************
			设置功能的实现
			*/	
		case MSG_VerifyOK:
			{
				myprintf("set ok ");
			}
			break;
    case WM_NOTIFY_PARENT:
      break;
		case MSG_CAN1REV:
      break;
    default:
      WM_DefaultProc(pMsg);
  }
}




const APP_LIST_ITEM _APP_CB_LIST[] ={
	_cbCallback00,
	_cbCallback02,
	_cbCallback02,
  _cbCallback02,
};



/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/


