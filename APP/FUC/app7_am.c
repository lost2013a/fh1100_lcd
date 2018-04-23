
#include "mainmenu.h"
#include "sys.h"

static void REC_Handler_APP07(WM_MESSAGE * pMsg) 			
{
	WM_HWIN hItem ;
	uint16_t mask=0x1010; 			 //2.->>CAN_BUF报文类型
	uint8_t data;
	uint8_t am;
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//收到的报文是对应槽，对应网口，对应类型的报文
	{
		myprintf("CAN_REC :	信号设置/n");
		data  = CAN1_RX1_BUF[SETAM_OFFSET];
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
		DROPDOWN_SetSel(hItem,data) ;
		data  = CAN1_RX1_BUF[SETAM_OFFSET+1];
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);
		switch(data)
			{
				case 0:
					 am = 0;
					 break;				
				case 4:
					 am = 2;
					 break;			
				default:
					 am = 1;
					 break;				
			}
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);
	  DROPDOWN_SetSel(hItem,am) ;	
	}
	CAN1_RX1_STA=0;
}

void CAN_SEND_APP07(WM_MESSAGE * pMsg,uint8_t rtr) 			
{
	WM_HWIN hItem ;
	uint32_t send_id = 0x03101000;			//CANID
	uint8_t  msg[8]  = {0,} ,data; 
	uint8_t  can_err;
	if(rtr)
	{
		myprintf("CAN_REQ :信号设置/n");
		can_err=CAN1_Send_REQ(send_id);
	}
	else
	{
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
		msg[0]= DROPDOWN_GetSel(hItem);
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);
		data= DROPDOWN_GetSel(hItem);
		switch(data)
			{
				case 0:
					 msg[1] = 0;
					 break;				
				case 2:
					 msg[1] = 4;
					 break;			
				default:
					 msg[1] = 1;
					 break;				
			}
		can_err=CAN1_Send_Msg(msg,send_id,8);
		myprintf("CAN_SEND:信号设置/n");
	}
	CAN_SHOW_ERR(can_err);
}




void _cbCallback07(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			PAINTTITL_SETAPP(2);/*画标题栏和窗口*/
		  GUI_DrawLine(XPOS0-10,YPOS0-5,XPOS0-10,YPOS0+YGAPS*2);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);//布置标题栏
		  //布置下拉框
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
		  DROPDOWN_AddString(hItem, "1:2");  
			DROPDOWN_AddString(hItem, "1:3(默认)"); 
		  DROPDOWN_AddString(hItem, "1:6");
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);  
		  DROPDOWN_AddString(hItem, "PPS");  
			DROPDOWN_AddString(hItem, "PPX_1"); 
		  DROPDOWN_AddString(hItem, "PPX_2");
		  DROPDOWN_AddString(hItem, "PPX_3");
		  CAN_SEND_APP07(pMsg,1); //请求配置
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
	  case MSG_SetInc:		
			GUI_SendKeyMsg(GUI_KEY_UP, 1);
	 		break;
	  case MSG_SetDec:			
			GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
			break;
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinSetapp))
			{	
				 GUI_EndDialog(hDlg, 0);MenuSet();//返回到设置菜单窗口
			}
			break;	
		/************************************************************
			设置功能的实现
			*/	
		case MSG_SetENTER:
			Password_Verify();
		  break;	
		case MSG_VerifyOK:
			{
				CAN_SEND_APP07(pMsg,0);
			}
			break;
		case WM_TIMER:
			{
				REC_Handler_APP07(pMsg);
			}
			WM_RestartTimer(pMsg->Data.v, 20);	
			break;	
		
    case WM_NOTIFY_PARENT:
      break;
    default:
      WM_DefaultProc(pMsg);
  }
	
}



