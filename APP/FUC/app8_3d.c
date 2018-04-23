
#include "mainmenu.h"
#include "sys.h"

static void REC_Handler_APP08(WM_MESSAGE * pMsg) 			
{
	WM_HWIN hItem ;
	uint16_t mask=0x1012; 			 //2.->>CAN_BUF报文类型
	uint32_t data=0;
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//收到的报文是对应槽，对应网口，对应类型的报文
	{
		myprintf("CAN_REC :定位设置/n");
		data  += ((CAN1_RX1_BUF[SETBD3D_OFFSET]>>4)%10)*1000;
		data  += ((CAN1_RX1_BUF[SETBD3D_OFFSET]&0xf)%10)*100;
		data  += ((CAN1_RX1_BUF[SETBD3D_OFFSET+1]>>4)%10)*17;
		data  += (((CAN1_RX1_BUF[SETBD3D_OFFSET+1]&0xf)%10)*17)/10;
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00);
		EDIT_SetValue(hItem,data) ;
		data=0;
		data  += ((CAN1_RX1_BUF[SETBD3D_OFFSET+8]>>4)%10)*10000;
		data  += ((CAN1_RX1_BUF[SETBD3D_OFFSET+8]&0xf)%10)*1000;
		data  += ((CAN1_RX1_BUF[SETBD3D_OFFSET+9]>>4)%10)*100;
		data  += ((CAN1_RX1_BUF[SETBD3D_OFFSET+9]&0xf)%10)*17;
		data  += (((CAN1_RX1_BUF[SETBD3D_OFFSET+10]>>4)%10)*17)/10;
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP01);
		EDIT_SetValue(hItem,data) ;
		
		data=0;
		data  += ((CAN1_RX1_BUF[SETBD3D_OFFSET+16]>>4)%10)*1000;
		data  += ((CAN1_RX1_BUF[SETBD3D_OFFSET+16]&0xf)%10)*100;
		data  += ((CAN1_RX1_BUF[SETBD3D_OFFSET+17]>>4)%10)*10;
		data  += ((CAN1_RX1_BUF[SETBD3D_OFFSET+17]&0xf)%10)*1;
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP02);
		EDIT_SetValue(hItem,data) ;
		
	}
	CAN1_RX1_STA=0;
}

void CAN_SEND_APP08(WM_MESSAGE * pMsg,uint8_t rtr) 			
{
	WM_HWIN hItem ;
	uint32_t send_id = 0x03101220,data0,data1,data2,data3;			//CANID
	uint8_t  msg[8]  = {0}, senddata[7]; 
	uint8_t  can_err;
	if(rtr)
	{
		myprintf("CAN_REQ :定位设置/n");
		can_err=CAN1_Send_REQ(send_id);
	}
	else
	{
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00);
		data0= EDIT_GetValue(hItem);
		data1=data0/1000;
		data2=(data0/100)%10;
		senddata[0]=(data1<<4)|data2;
		data0=((data0%100)*60)/100;
		data1=data0/10;
		data2=data0%10;
		senddata[1]=(data1<<4)|data2;
		
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP01);
		data0= EDIT_GetValue(hItem);
		data1=data0/10000;
		data2=(data0/1000)%10;
		data3=(data0/100)%10;//个位度
		senddata[2]=(data1<<4)|data2;
		
		data0=((data0%100)*60)/100;
		data1=data0/10;
		data2=data0%10;
		senddata[3]=(data3<<4)|data1;
		senddata[4]=(data2<<4);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP02);
		data0= EDIT_GetValue(hItem);
		data1=data0/1000;
		data2=(data0/100)%10;
		data3=(data0/10)%10;
		data0=data0%10;
		senddata[5]=(data1<<4)|data2;
		senddata[6]=(data3<<4)|data0;
		msg[0]=senddata[0];
		msg[1]=senddata[1];
		can_err=CAN1_Send_Msg(msg,send_id,8);
		
		send_id++;
		msg[0]=senddata[2];
		msg[1]=senddata[3];
		msg[2]=senddata[4];
		can_err=CAN1_Send_Msg(msg,send_id,8);
		send_id++;
		msg[0]=senddata[5];
		msg[1]=senddata[6];
		msg[2]=0;
		can_err=CAN1_Send_Msg(msg,send_id,8);
		
		myprintf("CAN_SEND:定位设置/n");
	}
	CAN_SHOW_ERR(can_err);
}




void _cbCallback08(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
	int i;
  hDlg = pMsg->hWin;
	
  switch (pMsg->MsgId) {
    case WM_PAINT:
			PAINTTITL_SETAPP(3);/*画标题栏和窗口*/
				/*画标题栏*/
		  GUI_DrawLine(XPOS0+30,YPOS0-5,XPOS0+30,YPOS0+YGAPS*3);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);//布置标题栏
		  //布置下拉框
			i=0;
      hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + (i++));
      EDIT_SetDecMode(hItem, 0, 0, 9000, 2, 0);

      hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + (i++));
      EDIT_SetDecMode(hItem, 0, 0, 18000, 2, 0);
		
			hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + (i++));
      EDIT_SetDecMode(hItem, 0, 0, 9999, 0, 0);
		
		  CAN_SEND_APP08(pMsg,1); //请求配置
			hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00);
		  WM_SetFocus (hItem);	
		  
      break;
 		/************************************************************
			按键特殊处理
			*/	
		case MSG_FOUCE:
			hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00);
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
				CAN_SEND_APP08(pMsg,0);
			}
			break;
		case WM_TIMER:
			{
				REC_Handler_APP08(pMsg);
			}
			WM_RestartTimer(pMsg->Data.v, 20);	
			break;	
		
    case WM_NOTIFY_PARENT:
      break;
    default:
      WM_DefaultProc(pMsg);
  }
	
}



