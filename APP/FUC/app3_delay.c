
#include "mainmenu.h"
#include "sys.h"





/*****************************************************
APP03-延迟设置
*****************************************************/
static void REC_Handler_APP03(WM_MESSAGE * pMsg) 			
{
	WM_HWIN hItem ;
	uint16_t  mask=0x1004; 			 //2.->>CAN_BUF报文类型
	uint16_t i;
	int data[7] , temp ,datasign;
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//收到的报文是对应槽，对应网口，对应类型的报文
	{
		myprintf("CAN_REC :延迟设置/n");
		for(i=0;i<7;i++)
		{
			temp 	= ((CAN1_RX1_BUF[SETDlY_OFFSET+(i<<2)]&0x7f)<<24)|(CAN1_RX1_BUF[SETDlY_OFFSET+1+(i<<2)]<<16)|(CAN1_RX1_BUF[SETDlY_OFFSET+2+(i<<2)]<<8)|CAN1_RX1_BUF[SETDlY_OFFSET+3+(i<<2)];
			temp	=	temp*OCX_PERIOD;
			if(temp>99999999)
				temp=99999999;
			datasign 	=	(CAN1_RX1_BUF[SETDlY_OFFSET+(i<<2)]<<24)&0x80000000;
			if(datasign)
				{
					data[i] =	0-temp;
				}
			else
				{
					data[i] =	temp;
				}
		}
		for(i=0;i<7;i++)
		{
			hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00+i);
			EDIT_SetValue(hItem,data[i]) ;
		}	
	 DevSys.CAN1_Rec1st=1;//CAN1接收完成	
	}
	CAN1_RX1_STA=0;
}

void CAN_SEND_APP03(WM_MESSAGE * pMsg,uint8_t rtr) 			
{
	WM_HWIN hItem ;
	uint32_t send_id = 0x03100440;			//CANID
	uint8_t  msg[8]  = {0,}; 
	uint8_t  can_err;
	if(rtr)
	{
		myprintf("CAN_REQ :延迟设置/n");
		if(DevSys.CAN1_Reqcnt<NUM_CAN1_RETRY+1)
			DevSys.CAN1_Reqcnt++;//CAN1请求++
		can_err=CAN1_Send_REQ(send_id);
	}
	else
	{ 
		int      i;
		int      readdata[7]={0};
		uint32_t senddata[7]={0};
		for(i=0;i<7;i++)
		{
			hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00+i);
		  readdata[i]= EDIT_GetValue(hItem) ;
			senddata[i]= (abs(readdata[i]))/OCX_PERIOD;
			if(readdata[i]<0)
				{
					senddata[i] = senddata[i]|0x80000000;//负数
				}
			  senddata[i] = htonl(senddata[i]);
		}	
		i = 0;
		memcpy((uint8_t*)msg,(uint32_t*)&senddata[0],8);
		can_err=CAN1_Send_Msg(msg,send_id+i,8); 
   	i++;	
		memcpy((uint8_t*)msg,(uint32_t*)&senddata[2],8);
		can_err=CAN1_Send_Msg(msg,send_id+i,8); 
		i++;	
		memcpy((uint8_t*)msg,(uint32_t*)&senddata[4],8);
		can_err=CAN1_Send_Msg(msg,send_id+i,8); 
		i++;	
		memcpy((uint8_t*)msg,(uint32_t*)&senddata[6],4);
		can_err=CAN1_Send_Msg(msg,send_id+i,8); 
		i++;	
		memset(msg,0,8);
		can_err=CAN1_Send_Msg(msg,send_id+i,8); //只判断了最后一帧发送正常
		
	}
	CAN_SHOW_ERR(can_err);
}





 void _cbCallback03(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
	int i;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			PAINTTITL_SETAPP(7);/*画标题栏和窗口*/
		  GUI_DrawLine(XPOS0+30,YPOS0-5,XPOS0+30,YPOS0+YGAPS*7);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);//布置标题栏
			 //布置EDIT框
      for (i = 0; i < 7; i++) 
	    {
        hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + i);
        EDIT_SetDecMode(hItem, 0, -99999999, 99999999, 0, GUI_EDIT_SIGNED);
      }
		 
			hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00);  
			WM_SetFocus (hItem);
			Clear_CAN1();//初始化，清CAN1状态
			CAN_SEND_APP03(pMsg,1); //请求配置
      break;
 		/************************************************************
			按键特殊处理
			*/	
		case MSG_FOUCE:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00);  
			WM_SetFocus (hItem);
			break;
		/************************************************************
			按键消息常规处理
			*/	
		case MSG_SetLEFT:
			GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
		  break;
		case MSG_SetRIGHT:
			GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
		  break;
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
				 GUI_EndDialog(hDlg, 0);hWin_msgto=hWinMenuSet;MenuSet();//返回到设置菜单窗口
			}
			break;	
		/************************************************************
			设置功能的实现
			*/	
		case MSG_VerifyOK:
			{
				CAN_SEND_APP03(pMsg,0);
			}
			break;
		case WM_TIMER:
			 REC_Handler_APP03(pMsg);
			//针对初始化，如果没有收到恢复，多次尝试(3次)
			if((DevSys.CAN1_Rec1st==0)&&(DevSys.CAN1_Reqcnt<NUM_CAN1_RETRY))
				{
						CAN_SEND_APP03(pMsg,1);
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








