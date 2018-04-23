
#include "mainmenu.h"
#include "sys.h"
#include "flash_conf.h"



void _cbCallback15(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
	char tbuf[20]={0};
  hDlg = pMsg->hWin;
	
	//static uint32_t readbuff[2] ;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			PAINTTITL_SETAPP(2);/*画标题栏和窗口*/
		  GUI_DrawLine(XPOS0-10,YPOS0-5,XPOS0-10,YPOS0+YGAPS*2);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);//布置标题栏
		  //布置下拉框
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
		  DROPDOWN_AddString(hItem, "开启");  
			DROPDOWN_AddString(hItem, "关闭(默认)");
	    DROPDOWN_SetSel(hItem,DevSys.LCD_Protect_ON) ;
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00 ); 
		  EDIT_SetDecMode(hItem, DevSys.USER_PASS, 0, 9999, 0, 0);
		
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04);  
			TEXT_SetFont(hItem, &GUI_FontHZ16);
		  TEXT_SetTextColor(hItem, GUI_RED);
		
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			WM_SetFocus (hItem);
      break;
 		/************************************************************
			按键特殊处理
			*/	
		case MSG_FOUCE:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			WM_SetFocus (hItem);
			break;
		/************************************************************
			按键消息常规处理
			*/	

		case MSG_SetUP:		
			GUI_SendKeyMsg(GUI_KEY_BACKTAB, 1);
	 		break;
	  case MSG_SetDOWM:			
			GUI_SendKeyMsg(GUI_KEY_TAB, 1);
			break;
		case MSG_SetLEFT:		
			GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
	 		break;
	  case MSG_SetRIGHT:			
			GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
			break;
	  case MSG_SetInc:		
			GUI_SendKeyMsg(GUI_KEY_UP, 1);
	 		break;
	  case MSG_SetDec:			
			GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
			break;
		case MSG_SetENTER:
			Password_Verify();
		  break;
		case MSG_PPSTICK:
		  if(DevSys.quality<3)
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04),"" );
			else
				{
					sprintf( (char*)(tbuf),"CAN0异常(%03d)",DevSys.quality);
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04),tbuf );
				}
			break;
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinSetapp))
			{	
				 GUI_EndDialog(hDlg, 0);Menuapp();//返回到设置菜单窗口
			}
			break;	
		/************************************************************
			设置功能的实现
			*/	
		case MSG_VerifyOK:
			{
				int i;
				static uint32_t writeflash[3];
				writeflash[0] = 0x55;
				writeflash[1] = DROPDOWN_GetSel(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0));
				writeflash[2] = EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00));
				myprintf("writeflash[3]={h%x,%d,,%d}",writeflash[0],writeflash[1],writeflash[2]);
				DevSys.LCD_Protect_ON =  writeflash[1];
				DevSys.USER_PASS   =  writeflash[2];;
			  FLASH_Write(FLASH_SAVE_ADDR,writeflash,3);
				for(i=0;i<1000;i++){}//等待烧写结束
				if(WM_IsWindow(hWinSetapp))
				{	
					
					 GUI_EndDialog(pMsg->hWin, 0);Menuapp();//返回到主菜单窗口
				}
			}
			break;
		
    case WM_NOTIFY_PARENT:
      break;
    default:
      WM_DefaultProc(pMsg);
  }
	
}



