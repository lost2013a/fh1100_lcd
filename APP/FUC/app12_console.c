
#include "mainmenu.h"
#include "sys.h"
extern void PAINTTITL_INFOAPP(uint8_t line);

static uint16_t template1[8]={
0xffff,	//1
0xffff,	//2
0xffff,	//3
0xffff,	//4
0xffff,	//5
0xffff,	//6
0xffff,	//7
0xffff,	//8
};

static uint16_t template2[8]={
0xfffc,	//1
0xffff,	//2
0xffff,	//3
0xffff,	//4
0xffff,	//5
0xffff,	//6
0xffff,	//7
0xffff,	//8
};


static void REC_Handler_APP11(WM_MESSAGE * pMsg,char mode) 			
{
	uint16_t data[8],i,equal=0;
	uint16_t *ptemplate;
	if(DevHead.is_master==0)
		ptemplate=template1;
	else
		ptemplate=template2;
	if(mode==0){	//正常的CAN报文
		uint16_t mask=0x103a; 			 //2.->>CAN_BUF报文类型
		if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//收到的报文是对应槽，对应网口，对应类型的报文
		{
			myprintf("CAN_REC :状态控制台\n");
			for(i=0;i<8;i++){
				data[i]=(CAN1_RX1_BUF[CONSOLE_OFFSET+2*i]<<8)+CAN1_RX1_BUF[CONSOLE_OFFSET+2*i+1];
			}
			for(i=0;i<8;i++){
				EDIT_SetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00+i),data[i]) ;
				if(ptemplate[i]!=data[i])
					equal+=1;
			}
			if(equal){			//不等于推荐配置，设置为自定义
				DROPDOWN_SetSel(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0),1);
			}	
			else{						//等于推荐配置
				DROPDOWN_SetSel(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0),0);
			}
		}
		CAN1_RX1_STA=0;
	}
	else{		//使用模版	,将模版值取反
		memcpy((uint16_t*)data,(uint16_t*)ptemplate,16);
		for(i=0;i<8;i++){
			EDIT_SetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00+i),data[i]) ;
		}
		i=DROPDOWN_GetSel(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0));
		i=!i;
		DROPDOWN_SetSel(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0),i);
	}
}

void CAN_SEND_APP11(WM_MESSAGE * pMsg,uint8_t rtr) 			
{
	uint32_t send_id = 0x03103a10;			//CANID
	//uint8_t  msg[8]  = {0,}; 
	uint16_t data[8],temp;
	uint8_t  can_err,i;
	if(rtr)
	{
		myprintf("CAN_REQ :状态控制台\n");
		can_err=CAN1_Send_REQ(send_id);
	}
	else
	{
		for(i=0;i<8;i++){
			temp= EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00+i));
			data[i]=(temp>>8)+((temp&0xff)<<8);
		}
		myprintf("CAN_SEND:模拟输出/n");
		can_err=CAN1_Send_Msg((u8 *)&data[0],send_id,8);
		can_err=CAN1_Send_Msg((u8 *)&data[4],send_id+1,8);
	}
	CAN_SHOW_ERR(can_err);
}

void binshow(uint16_t num,char* p)
{
	int i;
	for(i=0;i<16;i++){
		if(num&(1<<i))
			p[16-(i+i/8)]='1';
		else
			p[16-(i+i/8)]='0';
	}
	p[8]='-';
}




void _cbCallback11(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
	int i;
	char tbuf[18]={0};
	uint16_t temp;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			 PAINTTITL_INFOAPP(8);
		   for (i = 1; i < 8; i++) 
			 {
				GUI_DrawLine(80,72+YGAPS*i,600,72+YGAPS*i);
			 }
			GUI_DrawLine(XPOS0-30,YPOS0-YGAPS-5,XPOS0-30,YPOS0+YGAPS*8);
      break;
    case WM_INIT_DIALOG:
			PAINTWIN_SETAPP(pMsg);//布置标题栏
			//布置下拉框
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			DROPDOWN_AddString(hItem, "默认配置");  
			DROPDOWN_AddString(hItem, "自定义配置");

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04);
			TEXT_SetFont(hItem, &GUI_FontHZ16);
			TEXT_SetTextColor(hItem, GUI_BLUE);
			for(i=0;i<8;i++){
				hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + i);
				EDIT_SetHexMode(hItem, 0, 0, 0xffff);
				hItem = WM_GetDialogItem(hDlg, ID_TEXT_APP10 + i);
				TEXT_SetTextColor(hItem, GUI_GRAY_5);
				TEXT_SetFont(hItem, &GUI_Font20_1);
			}
		  CAN_SEND_APP11(pMsg,1); //请求配置
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
			hItem=WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
			if((1==DROPDOWN_GetSel(hItem))&&(hItem==WM_GetFocussedWindow())){
				REC_Handler_APP11(pMsg,1);
			}
			else{
				GUI_SendKeyMsg(GUI_KEY_UP, 1);
			}
	 		break;
	  case MSG_SetDec:
			hItem=WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
			if((1==DROPDOWN_GetSel(hItem))&&(hItem==WM_GetFocussedWindow())){
				REC_Handler_APP11(pMsg,1);
			}
			else{
				GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
			}	
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
				CAN_SEND_APP11(pMsg,0);
			}
			break;
		case WM_TIMER:
			
			REC_Handler_APP11(pMsg,0);
			for(i=0;i<8;i++){
				temp=EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00+i));
				binshow(temp,tbuf);
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+i),tbuf);
			}
			WM_RestartTimer(pMsg->Data.v, 20);	
			break;	
		
    case WM_NOTIFY_PARENT:
      break;
    default:
      WM_DefaultProc(pMsg);
  }
	
}



