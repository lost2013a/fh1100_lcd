
#include "mainmenu.h"
#include "sys.h"

const uint32_t template[4]={0};
static char Notbin=1;

static void REC_Handler_APP15(WM_MESSAGE * pMsg) 			
{
	uint16_t mask=0x103a; 			 //2.->>CAN_BUF报文类型
	uint32_t data[4],i,equal=0;
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//收到的报文是对应槽，对应网口，对应类型的报文
	{
		myprintf("CAN_REC :状态控制台\n");
		memcpy((char*)data,(char *)&CAN1_RX1_BUF[CONSOLE_OFFSET],16);
		for(i=0;i<4;i++){
			myprintf("data[%d]=%08X\n",i,data[i]);
			EDIT_SetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00+i),data[i]) ;
			if(template[i]!=data[i])
				equal+=1;
		}
		if(equal){
			DROPDOWN_SetSel(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0),0);
		}	
		else{
			DROPDOWN_SetSel(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0),1);
		}
	}
	CAN1_RX1_STA=0;
}

void CAN_SEND_APP15(WM_MESSAGE * pMsg,uint8_t rtr) 			
{
	uint32_t send_id = 0x03103a00;			//CANID
	//uint8_t  msg[8]  = {0,}; 
	uint32_t data[4];
	uint8_t  can_err,i;
	if(rtr)
	{
		myprintf("CAN_REQ :状态控制台\n");
		can_err=CAN1_Send_REQ(send_id);
	}
	else
	{
		for(i=0;i<4;i++){
			data[i]= EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00+i));
		}
		myprintf("CAN_SEND:模拟输出/n");
		can_err=CAN1_Send_Msg((u8 *)&data[0],send_id,8);
		can_err=CAN1_Send_Msg((u8 *)&data[2],send_id+1,8);
	}
	CAN_SHOW_ERR(can_err);
}




void _cbCallback15(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
	int i;
	uint32_t temp;
  hDlg = pMsg->hWin;
	
  switch (pMsg->MsgId) {
    case WM_PAINT:
			PAINTTITL_SETAPP(6);/*画标题栏和窗口*/
				/*画标题栏*/
		  GUI_DrawLine(XPOS0+30,YPOS0-5,XPOS0+30,YPOS0+YGAPS*6);
			if(Notbin){
				GUI_SetTextMode(GUI_TM_TRANS);
				GUI_SetColor(GUI_BLACK);
				GUI_DispStringAt("08",XPOS0+100,YPOS0+YGAPS*6);
				GUI_DispStringAt("16",XPOS0+200,YPOS0+YGAPS*6);
				GUI_DispStringAt("24",XPOS0+300,YPOS0+YGAPS*6);
				GUI_DispStringAt("32",XPOS0+400,YPOS0+YGAPS*6);
			}
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);//布置标题栏
		  //布置下拉框
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			DROPDOWN_AddString(hItem, "标准(默认)");  
			DROPDOWN_AddString(hItem, "自定义");
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);  
			DROPDOWN_AddString(hItem, "16进制显示");  
			DROPDOWN_AddString(hItem, "2进制显示");
			Notbin=1;//静态变量
			for(i=0;i<4;i++){
				hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + i);
				EDIT_SetHexMode(hItem, 0, 0, 0xffffffff);
			}
		  CAN_SEND_APP15(pMsg,1); //请求配置
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
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);
			if(hItem==WM_GetFocussedWindow()){
				Notbin=0;//静态变量
				for(i=0;i<4;i++){
					temp=EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00+i));
					EDIT_SetBinMode(hItem, temp, 0, 0xffffffff);
				}
			}
			GUI_SendKeyMsg(GUI_KEY_UP, 1);
	 		break;
	  case MSG_SetDec:		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);
			if(hItem==WM_GetFocussedWindow()){
				Notbin=1;//静态变量
				for(i=0;i<4;i++){
					temp=EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00+i));
					EDIT_SetHexMode(hItem, temp, 0, 0xffffffff);
				}
			}	
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
				CAN_SEND_APP15(pMsg,0);
			}
			break;
		case WM_TIMER:
			{
				REC_Handler_APP15(pMsg);
			}
			WM_RestartTimer(pMsg->Data.v, 20);	
			break;	
		
    case WM_NOTIFY_PARENT:
      break;
    default:
      WM_DefaultProc(pMsg);
  }
	
}



