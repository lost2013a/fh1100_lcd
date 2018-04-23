
#include "mainmenu.h"
#include "sys.h"

static void REC_Handler_APP09(WM_MESSAGE * pMsg) 			
{
	uint16_t mask=0x1011; 			 //2.->>CAN_BUF报文类型
	uint16_t data,i;
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//收到的报文是对应槽，对应网口，对应类型的报文
	{
		myprintf("CAN_REC :模拟输出/n");
		data  = CAN1_RX1_BUF[SETENOUT_OFFSET];
		DROPDOWN_SetSel(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0),data) ;
		data  = (CAN1_RX1_BUF[SETENOUT_OFFSET+1]<<8)+CAN1_RX1_BUF[SETENOUT_OFFSET+2];
		EDIT_SetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00),data) ;
		for(i=0;i<5;i++)
		{
			data  = CAN1_RX1_BUF[SETENOUT_OFFSET+3+i];
			EDIT_SetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP01+i),data) ;
		}
	}
	CAN1_RX1_STA=0;
}

void CAN_SEND_APP09(WM_MESSAGE * pMsg,uint8_t rtr) 			
{
	WM_HWIN hItem ;
	uint32_t send_id = 0x03101100;			//CANID
	uint8_t  msg[8]  = {0,}; 
	uint8_t  can_err;
	uint16_t data,i;
	if(rtr)
	{
		myprintf("CAN_REQ :模拟输出/n");
		can_err=CAN1_Send_REQ(send_id);
	}
	else
	{
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
		msg[0]= DROPDOWN_GetSel(hItem);
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00);
		data  = EDIT_GetValue(hItem);
		msg[1]=data>>8;
		msg[2]=data&0xff;
		
		for(i=0;i<5;i++)
		{
			msg[3+i]=EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP01+i));
		}
		can_err=CAN1_Send_Msg(msg,send_id,8);
		myprintf("CAN_SEND:模拟输出/n");
		for(i=0;i<8;i++)
		{
			myprintf("%d ",msg[i]);
		}
	}
	CAN_SHOW_ERR(can_err);
}




void _cbCallback09(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
	int i;
  hDlg = pMsg->hWin;
	
  switch (pMsg->MsgId) {
    case WM_PAINT:
			PAINTTITL_SETAPP(7);/*画标题栏和窗口*/
				/*画标题栏*/
		  GUI_DrawLine(XPOS0+30,YPOS0-5,XPOS0+30,YPOS0+YGAPS*7);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);//布置标题栏
		  //布置下拉框
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			DROPDOWN_AddString(hItem, "关闭(默认)");  
			DROPDOWN_AddString(hItem, "使能");
			i=0;
      hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + (i++));
      EDIT_SetDecMode(hItem, 2010, 2010, 2100, 0, 0);
      hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + (i++));
      EDIT_SetDecMode(hItem, 1, 1, 12, 0, 0);
			hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + (i++));
      EDIT_SetDecMode(hItem, 1, 1, 31, 0, 0);
		   hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + (i++));
      EDIT_SetDecMode(hItem, 0, 0, 23, 0, 0);
      hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + (i++));
      EDIT_SetDecMode(hItem, 0, 0, 59, 0, 0);
			hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + (i++));
      EDIT_SetDecMode(hItem, 0, 0, 59, 0, 0);
		  CAN_SEND_APP09(pMsg,1); //请求配置
		  
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
				CAN_SEND_APP09(pMsg,0);
			}
			break;
		case WM_TIMER:
			{
				REC_Handler_APP09(pMsg);
			}
			WM_RestartTimer(pMsg->Data.v, 20);	
			break;	
		
    case WM_NOTIFY_PARENT:
      break;
    default:
      WM_DefaultProc(pMsg);
  }
	
}



