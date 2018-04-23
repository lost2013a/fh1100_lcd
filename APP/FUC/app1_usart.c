
#include "mainmenu.h"
#include "sys.h"




/*****************************************************
APP01-串口设置
*****************************************************/

static void REC_Handler_APP01(WM_MESSAGE * pMsg) 			
{
	WM_HWIN hItem ;
	uint16_t  mask=0x1002; 			 //2.->>CAN_BUF报文类型
	uint8_t data[5];
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//收到的报文是对应槽，对应网口，对应类型的报文
	{
		//myprintf("CAN_REC :串口设置/n");
		data[0]	=	CAN1_RX1_BUF[S2_OFFSET+3]&0xf;
		data[1]	=	CAN1_RX1_BUF[S2_OFFSET+2]&0xf;
		data[2]	=	(CAN1_RX1_BUF[S2_OFFSET+2]&0xf0)>>4;
		data[3]	=	CAN1_RX1_BUF[S2_OFFSET+4]&0xf;  //IRIG-B2
		data[4]	=	CAN1_RX1_BUF[S2_OFFSET+5]&0xf;  //IRIG-B2
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
		DROPDOWN_SetSel(hItem,data[0]) ;
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);
		DROPDOWN_SetSel(hItem,data[1]) ;
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP2);
		DROPDOWN_SetSel(hItem,data[2]) ;
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP3);
		DROPDOWN_SetSel(hItem,data[3]) ;
		
		if( DevSys.USER_Right == 1 )//高级用户
		{
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP4);
			DROPDOWN_SetSel(hItem,data[4]) ;
		}
		
	  DevSys.CAN1_Rec1st=1;//CAN1接收完成
	}
	CAN1_RX1_STA=0;
}

void CAN_SEND_APP01(WM_MESSAGE * pMsg,uint8_t rtr) 			
{
	WM_HWIN hItem ;
	uint32_t send_id = 0x03100200;			//CANID
	uint8_t  msg[8]  = {0,}; 
	uint8_t  can_err;
	if(rtr)
	{
		//myprintf("CAN_REQ :串口设置/n");
		can_err=CAN1_Send_REQ(send_id);
		if(DevSys.CAN1_Reqcnt<NUM_CAN1_RETRY+1)
			DevSys.CAN1_Reqcnt++;//CAN1请求++		
	}
	else
	{
		uint8_t  p[4];
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);//类型
		p[0]= DROPDOWN_GetSel(hItem);
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);//波特率
		p[1]= DROPDOWN_GetSel(hItem);
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP2);//校验方式
		p[2]= DROPDOWN_GetSel(hItem);
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP3);//B码校验方式
		p[3]= DROPDOWN_GetSel(hItem);
		
		msg[0]= 0x22;
		msg[1]= 0x0;
		msg[2]= (p[2]<<4) | p[1];
		msg[3]= p[0];
		msg[4]= p[3];
		
		if( DevSys.USER_Right == 1 )//高级用户
		{
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP4);//B码质量位
			msg[5]= DROPDOWN_GetSel(hItem);
		}
		else{
			msg[5]=CAN1_RX1_BUF[S2_OFFSET+5]&0xf;
		}
		
		can_err=CAN1_Send_Msg(msg,send_id,8);
		//myprintf("CAN_SEND:串口设置/n");
	}
	CAN_SHOW_ERR(can_err);
}



 void _cbCallback01(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			PAINTTITL_SETAPP(4);/*画标题栏和窗口*/
		  GUI_DrawLine(XPOS0+30,YPOS0-5,XPOS0+30,YPOS0+YGAPS*5);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);//布置标题栏
		  //布置下拉框
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			DROPDOWN_AddString(hItem, "国标");  
			DROPDOWN_AddString(hItem, "南网");
		  DROPDOWN_AddString(hItem, "海南");
			DROPDOWN_AddString(hItem, "GPRMC");
		  DROPDOWN_AddString(hItem, "国标(含频率描述)");
		  DROPDOWN_AddString(hItem, "BJT");
		  DROPDOWN_AddString(hItem, "BJT(含频率描述)");
		  DROPDOWN_AddString(hItem, "南瑞");

		  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1); 
      DROPDOWN_AddString(hItem, "1200");		
		  DROPDOWN_AddString(hItem, "2400");  
			DROPDOWN_AddString(hItem, "4800");
		  DROPDOWN_AddString(hItem, "9600");
		  DROPDOWN_AddString(hItem, "19200");
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP2);  
		  DROPDOWN_AddString(hItem, "无校验");
		  DROPDOWN_AddString(hItem, "奇校验");  
			DROPDOWN_AddString(hItem, "偶校验");
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP3);   
		  DROPDOWN_AddString(hItem, "无校验");
		  DROPDOWN_AddString(hItem, "奇校验");  
			DROPDOWN_AddString(hItem, "偶校验");
			
			if( DevSys.USER_Right == 1 )//高级用户
				{
					
					hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP4);
					DROPDOWN_AddString(hItem, "本地质量位");  
					DROPDOWN_AddString(hItem, "保持0");
				}
			else{
					hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP05);
					WM_DeleteWindow(hItem);
					hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP4);
					WM_DeleteWindow(hItem);
			}
			
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04);  
			TEXT_SetFont(hItem, &GUI_FontHZ16);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			WM_SetFocus (hItem);
			Clear_CAN1();//初始化，清CAN1状态
		  CAN_SEND_APP01(pMsg,1); //请求配置
      break;
 		/************************************************************
			按键特殊处理
			*/	
		case MSG_FOUCE:
		//case WM_PRE_PAINT:	
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
				CAN_SEND_APP01(pMsg,0);
			}
			break;
		case WM_TIMER:
			REC_Handler_APP01(pMsg);	
		  //针对初始化，如果没有收到恢复，多次尝试(3次)
			if((DevSys.CAN1_Rec1st==0)&&(DevSys.CAN1_Reqcnt<NUM_CAN1_RETRY))
				{
						CAN_SEND_APP01(pMsg,1);
						#ifdef SHOW_DEBUG		//调试信息
						if(DevSys.CAN1_Reqcnt==NUM_CAN1_RETRY-1)
							TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP01), GUI_ORANGE);
						#endif
				}
			WM_RestartTimer(pMsg->Data.v, 20);	
			break;	
		
    default:
      WM_DefaultProc(pMsg);
  }
	
}



