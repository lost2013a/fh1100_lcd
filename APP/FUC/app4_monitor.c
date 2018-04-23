
#include "mainmenu.h"
#include "sys.h"

static void REC_Handler_APP04(WM_MESSAGE * pMsg) 			
{
	WM_HWIN hItem ;
	uint16_t mask=0xA040; 			 //2.->>CAN_BUF报文类型
	uint8_t data;
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//收到的报文是对应槽，对应网口，对应类型的报文
	{
		myprintf("CAN_REC :监测管理/n");
		data  = CAN1_RX1_BUF[MONIT_OFFSET];
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
		DROPDOWN_SetSel(hItem,data) ;
		DevSys.CAN1_Rec1st=1;//CAN1接收完成
	}
	CAN1_RX1_STA=0;
}

void CAN_SEND_APP04(WM_MESSAGE * pMsg,uint8_t rtr) 			
{
	WM_HWIN hItem ;
	uint32_t send_id = 0x03A04000;			//CANID
	uint8_t  msg[8]  = {0,}; 
	uint8_t  can_err;
	if(rtr)
	{
		myprintf("CAN_REQ :监测管理/n");
		if(DevSys.CAN1_Reqcnt<NUM_CAN1_RETRY+1)
			DevSys.CAN1_Reqcnt++;//CAN1请求++
		can_err=CAN1_Send_REQ(send_id);
	}
	else
	{
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
		msg[0]= DROPDOWN_GetSel(hItem);
		can_err=CAN1_Send_Msg(msg,send_id,8);
		myprintf("CAN_SEND:监测管理/n");	
	}
	CAN_SHOW_ERR(can_err);
}




void _cbCallback04(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
  hDlg = pMsg->hWin;
	
  switch (pMsg->MsgId) {
    case WM_PAINT:
			PAINTTITL_SETAPP(1);/*画标题栏和窗口*/
				/*画标题栏*/
		  GUI_DrawLine(XPOS0+30,YPOS0-5,XPOS0+30,YPOS0+YGAPS*1);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);//布置标题栏
		  //布置下拉框
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			DROPDOWN_AddString(hItem, "关闭");  
			DROPDOWN_AddString(hItem, "开启");
		  
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			WM_SetFocus (hItem);
		  Clear_CAN1();//初始化，清CAN1状态
		  CAN_SEND_APP04(pMsg,1); //请求配置
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
				 GUI_EndDialog(hDlg, 0);MenuSet();//返回到设置菜单窗口
			}
			break;	
		/************************************************************
			设置功能的实现
			*/	
		case MSG_VerifyOK:
			{
				CAN_SEND_APP04(pMsg,0);
			}
			break;
		case WM_TIMER:
			REC_Handler_APP04(pMsg);
			//针对初始化，如果没有收到恢复，多次尝试(3次)
			if((DevSys.CAN1_Rec1st==0)&&(DevSys.CAN1_Reqcnt<NUM_CAN1_RETRY))
				{
						CAN_SEND_APP04(pMsg,1);
						#ifdef SHOW_DEBUG		//调试信息
						if(DevSys.CAN1_Reqcnt==NUM_CAN1_RETRY-1)
							TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP01), GUI_ORANGE);
						#endif
				}
			WM_RestartTimer(pMsg->Data.v, 20);	
			break;	
    case WM_NOTIFY_PARENT:
      break;
    default:
      WM_DefaultProc(pMsg);
  }
	
}



