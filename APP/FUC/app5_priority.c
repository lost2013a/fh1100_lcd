
#include "mainmenu.h"
#include "sys.h"

static void REC_Handler_APP05(WM_MESSAGE * pMsg) 			
{
	WM_HWIN hItem ;
	uint16_t mask=0x1003; 			 //2.->>CAN_BUF报文类型
	uint32_t data ,pri = 0;
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//收到的报文是对应槽，对应网口，对应类型的报文
	{
		myprintf("CAN_REC :源优先级/n");
		data = (CAN1_RX1_BUF[S3_OFFSET]<<16)|(CAN1_RX1_BUF[S3_OFFSET+1]<<8)|(CAN1_RX1_BUF[S3_OFFSET+2]);	
		switch(data)
			{
				case 0x123456:
					 pri = 0;
					 break;				
				case 0x213456:
					 pri = 1;
					 break;	
				case 0x231456:
					 pri = 2;
					 break;		
				case 0x234156:
					 pri = 3;
					 break;		
				case 0x234516:
					 pri = 4;
					 break;		
				case 0x234561:
					 pri = 5;
					 break;		
				default:
					 break;
			}
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
	  DROPDOWN_SetSel(hItem,pri) ;
	}
	CAN1_RX1_STA=0;
}

void CAN_SEND_APP05(WM_MESSAGE * pMsg,uint8_t rtr) 			
{
	WM_HWIN hItem ;
	uint32_t send_id = 0x03100300;			//CANID
	uint8_t  msg[8]  = {0x12,0x34,0x56} ,data; 
	uint8_t  can_err;
	if(rtr)
	{
		myprintf("CAN_REQ :源优先级/n");
		can_err=CAN1_Send_REQ(send_id);
	}
	else
	{
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
		data= DROPDOWN_GetSel(hItem);
		switch(data)
				{
					case 1:
						 msg[0] = 0x21;
						 break;				
					case 2:
						 msg[0] = 0x23;
						 msg[1] = 0x14;
						 break;	
					case 3:
						 msg[0] = 0x23;
						 msg[1] = 0x41;
						 break;	
					case 4:
						 msg[0] = 0x23;
						 msg[1] = 0x45;
						 msg[2] = 0x16;
						 break;	
					case 5:
						 msg[0] = 0x23;
						 msg[1] = 0x45;
						 msg[2] = 0x61;
					   break;	
					default:
						 break;
				}
		can_err=CAN1_Send_Msg(msg,send_id,8);
		myprintf("CAN_SEND:源优先级/n");
	}
	CAN_SHOW_ERR(can_err);
}




void _cbCallback05(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			PAINTTITL_SETAPP(1);/*画标题栏和窗口*/
		  GUI_DrawLine(XPOS0-10,YPOS0-5,XPOS0-10,YPOS0+YGAPS*1);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);//布置标题栏
		  //布置下拉框
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			DROPDOWN_AddString(hItem, "北斗优先(默认)");  
			DROPDOWN_AddString(hItem, "GPS优先"); 
      DROPDOWN_AddString(hItem, "IRIG-B1优先");  
			DROPDOWN_AddString(hItem, "IRIG-B2优先"); 
			DROPDOWN_AddString(hItem, "PTP1优先");  
			DROPDOWN_AddString(hItem, "PTP2优先");
		 
		  //hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			WM_SetFocus (hItem);
		  CAN_SEND_APP05(pMsg,1); //请求配置
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
				CAN_SEND_APP05(pMsg,0);
			}
			break;
		case WM_TIMER:
			{
				REC_Handler_APP05(pMsg);
			}
			WM_RestartTimer(pMsg->Data.v, 20);	
			break;	
		
    case WM_NOTIFY_PARENT:
      break;
    default:
      WM_DefaultProc(pMsg);
  }
	
}



