
#include "mainmenu.h"
#include "sys.h"






/*****************************************************
APP02-网络设置
*****************************************************/
static void REC_Handler_APP06(WM_MESSAGE * pMsg) 			
{
	WM_HWIN hItem ;
	int gaps;
	char tbuf[20]={0};
	uint16_t  mask=0x1004; 			 //2.->>CAN_BUF报文类型
	uint32_t  temp1;
	uint8_t  slot,port;
	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);//插槽
	slot= DROPDOWN_GetSel(hItem);
	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);//端口
	port= DROPDOWN_GetSel(hItem);
	
	/*确定CAN_ID号*/
	mask=((slot+1)<<12)|port<<8|0x09;
	
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//收到的报文是对应槽，对应网口，对应类型的报文
	{
		//myprintf("CAN_REQ 网口设置:%d.%d\n",slot,port);
		DROPDOWN_SetSel(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP2),CAN1_RX1_BUF[S5_OFFSET+20]);

		EDIT_SetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00),CAN1_RX1_BUF[S5_OFFSET+22]) ;
		EDIT_SetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP01),CAN1_RX1_BUF[S5_OFFSET+31]) ;
		EDIT_SetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP02),CAN1_RX1_BUF[S5_OFFSET+21]) ;
		EDIT_SetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP03),CAN1_RX1_BUF[S5_OFFSET+37]) ;
		EDIT_SetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP04),CAN1_RX1_BUF[S5_OFFSET+38]) ;
		
		
		sprintf( tbuf , "(程序版本V%d.%d)",CAN1_RX1_BUF[S5_OFFSET+23]&0xf,CAN1_RX1_BUF[S5_OFFSET+23]>>4); 
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP08),tbuf );
		
		gaps=CAN1_RX1_BUF[S5_OFFSET+32];
		if(gaps>0x40)
			gaps=gaps-0x100;
		EDIT_SetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP05),gaps) ;
			gaps=CAN1_RX1_BUF[S5_OFFSET+34];
		if(gaps>0x40)
			gaps=gaps-0x100;
		EDIT_SetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP06),gaps) ;

		temp1 = (CAN1_RX1_BUF[S5_OFFSET+24]<<16)|(CAN1_RX1_BUF[S5_OFFSET+25]<<8)|CAN1_RX1_BUF[S5_OFFSET+26];
		EDIT_SetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP07),temp1) ;
		temp1 = (CAN1_RX1_BUF[S5_OFFSET+27]<<16)|(CAN1_RX1_BUF[S5_OFFSET+28]<<8)|CAN1_RX1_BUF[S5_OFFSET+29];
		EDIT_SetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP08),temp1) ;
		DevSys.CAN1_Rec1st=1;//CAN1接收完成	
	}
	CAN1_RX1_STA=0;
}

void CAN_SEND_APP06(WM_MESSAGE * pMsg,uint8_t rtr) 			
{
	WM_HWIN hItem ;
	int      i;
	uint32_t send_id ,mac1;			//CANID
	uint8_t  msg[32]  = {0}; 
	uint8_t  can_err;
	uint8_t  slot,port;
	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);//插槽
	slot= DROPDOWN_GetSel(hItem);
	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);//端口
	port= DROPDOWN_GetSel(hItem);
	/*确定CAN_ID号*/
	if((slot==0)||(slot==10))//cpu板和检控板
		send_id=0x03<<24|((slot+1)<<20)|port<<16|0x0940;
	else
		send_id=0x06<<24|((slot+1)<<20)|port<<16|0x0940;

	if(rtr)
		{
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP2);//清0
			DROPDOWN_SetSel(hItem,0);
			for(i=0;i<9;i++)
			{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00+i);//清0
				EDIT_SetValue(hItem,0) ;
			}	
			//myprintf("CAN_REQ 网口设置:%d.%d\n",slot,port);
			if(DevSys.CAN1_Reqcnt<NUM_CAN1_RETRY+1)
				DevSys.CAN1_Reqcnt++;//CAN1请求++
			can_err=CAN1_Send_REQ(send_id);
			
			
		}
	else
		{
			//第3帧
			msg[4]= DROPDOWN_GetSel(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP2));	
			msg[5]= EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP02));	
			msg[6]= EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00));	
			//第4帧
			mac1  = EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP07)) ;
			msg[8]=(mac1&0xff0000)>>16;
			msg[9]=(mac1&0xff00)>>8;
			msg[10]=mac1&0xff;
			mac1  = EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP08)) ;
			msg[11]=(mac1&0xff0000)>>16;
			msg[12]=(mac1&0xff00)>>8;
			msg[13]=mac1&0xff;
			msg[15]  = EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP01)) ;
			//第5帧
			msg[16]=EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP05)) ;
			msg[17]=6;
			msg[18]=EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP06)) ;
			msg[19]=1;
			msg[20]=0;
			msg[21]=EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP03)) ;
			msg[22]=EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP04)) ;
			msg[23]= (send_id&0xff0000)>>16;
			
			if((slot==0)||(slot==10))//cpu板和检控板
				{
					CAN1_Send_Msg(&CAN1_RX1_BUF[S5_OFFSET],send_id,8);
					CAN1_Send_Msg(&CAN1_RX1_BUF[S5_OFFSET+8],send_id+1,8);
				}
			else//net6板
				{
					CAN1_Send_Msg(&msg[24],send_id,8);
					CAN1_Send_Msg(&msg[24],send_id+1,8);
				}
			CAN1_Send_Msg(&msg[0],send_id+2,8);
			CAN1_Send_Msg(&msg[8],send_id+3,8);
			can_err=CAN1_Send_Msg(&msg[16],send_id+4,8);
			//myprintf("CAN_SEND 网口设置:%d.%d\n",slot,port);
		}
	CAN_SHOW_ERR(can_err);
}


 void _cbCallback06(WM_MESSAGE * pMsg) 
{
	/*IP设置中使用了静态变量来实现光标位置的变化*/
	static WM_HWIN xitems[9];      //控件划分成6行，记录每行第一个控件的句柄。
	static uint8_t xsel  , ysel;   //光标的位置
  WM_HWIN hDlg;
  WM_HWIN hItem;
  int     i;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			/*画标题栏和窗口*/
		  PAINTTITL_MSTAAPP(8);/*画标题栏和窗口*/
		  GUI_DrawLine(XPOS0-10,YPOS0-5-YGAPS,XPOS0-10,YPOS0+YGAPS*8);
		  GUI_SetTextMode(GUI_TM_TRANS);
		  GUI_SetColor(GUI_BLACK);
		  GUI_DispCharAt('-',XPOS0+92,YPOS0+YGAPS*7+10);	
      break;
    case WM_INIT_DIALOG:
			//布置标题栏
       PAINTWIN_SETAPP(pMsg);
		  //布置下拉框
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			DROPDOWN_AddString(hItem, "CPU板");  
			DROPDOWN_AddString(hItem, "扩展板3");
			DROPDOWN_AddString(hItem, "扩展板4");
			DROPDOWN_AddString(hItem, "扩展板5");
			DROPDOWN_AddString(hItem, "扩展板6");
			DROPDOWN_AddString(hItem, "扩展板7");
			DROPDOWN_AddString(hItem, "扩展板8");
			DROPDOWN_AddString(hItem, "扩展板9");
			DROPDOWN_AddString(hItem, "扩展板10");
		  DROPDOWN_AddString(hItem, "扩展板11");
		  DROPDOWN_AddString(hItem, "监测板");
		  //DROPDOWN_AddString(hItem, "扩展板13");
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);  
			DROPDOWN_AddString(hItem, "网口1");
			DROPDOWN_AddString(hItem, "网口2");
			DROPDOWN_AddString(hItem, "网口3");
			DROPDOWN_AddString(hItem, "网口4");
			DROPDOWN_AddString(hItem, "网口5");
			DROPDOWN_AddString(hItem, "网口6");
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP2);  
			DROPDOWN_AddString(hItem, "ETH-E2E(主)");
			DROPDOWN_AddString(hItem, "ETH-P2P(主)");
			DROPDOWN_AddString(hItem, "UDP-E2E(主)");
			DROPDOWN_AddString(hItem, "UDP-P2P(主)");
			DROPDOWN_AddString(hItem, "ETH-E2E(从)");
			DROPDOWN_AddString(hItem, "ETH-P2P(从)");
			DROPDOWN_AddString(hItem, "UDP-E2E(从)");
			DROPDOWN_AddString(hItem, "UDP-P2P(从)");
			DROPDOWN_AddString(hItem, "NTP（默认）");
			for (i = 0; i < 5; i++) 
	    {
        hItem = WM_GetDialogItem(hDlg, ID_TEXT_APP04 + i);
        TEXT_SetFont(hItem, &GUI_FontHZ16);
      }
      //布置EDIT框
      for (i = 0; i < 3; i++) 
	    {
        hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + i);
        EDIT_SetDecMode(hItem, 0, 0, 255, 0, 0);
      }
			for (i = 0; i < 2; i++) 
	    {
        hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP03 + i);
        EDIT_SetDecMode(hItem, 0, 1, 254, 0, 0);
      }
			hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP05);	
      EDIT_SetDecMode(hItem, 0, -3, 4, 0, 0);
			hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP06);	
      EDIT_SetDecMode(hItem, 0, -3, 4, 0, 0);
			for (i = 0; i < 2; i++) 
	    {
				hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP07+i);	
				EDIT_SetHexMode(hItem, 0, 0, 0xffffff);  
			}
      //控件划分成6行，记录每行第一个控件的句柄
			xitems[0] = WM_GetDialogItem(hDlg, ID_DROPDOWN_APP0 );
			xitems[1] = WM_GetDialogItem(hDlg, ID_DROPDOWN_APP1 );
			xitems[2] = WM_GetDialogItem(hDlg, ID_DROPDOWN_APP2 );
		  xitems[3] = WM_GetDialogItem(hDlg, ID_EDIT_APP00 );
			xitems[4] = WM_GetDialogItem(hDlg, ID_EDIT_APP01 );
			xitems[5] = WM_GetDialogItem(hDlg, ID_EDIT_APP02 );
			xitems[6] = WM_GetDialogItem(hDlg, ID_EDIT_APP03 );
			xitems[7] = WM_GetDialogItem(hDlg, ID_EDIT_APP05 );
			xitems[8] = WM_GetDialogItem(hDlg, ID_EDIT_APP07 );
			//初始化焦点
			xsel = ysel =0;
			WM_SetFocus (xitems[0]);
			Clear_CAN1();//初始化，清CAN1状态
			CAN_SEND_APP06(pMsg,1);
      break;
		/************************************************************
			按键消息特殊处理
			*/	
		case MSG_SetUP:
			ysel = (ysel+8)%9;
		  WM_SetFocus (xitems[ysel]);
		  if(ysel>2)EDIT_SetCursorAtChar(xitems[ysel],0);
		  xsel = 0;
			break;
		case MSG_SetDOWM:
			ysel = (ysel+1)%9;
		  WM_SetFocus (xitems[ysel]);
		  if(ysel>2) EDIT_SetCursorAtChar(xitems[ysel],0);
		  xsel = 0;
			break;	
		case MSG_SetLEFT:
				if(ysel==6)
					{
						xsel = (xsel+5)%6;
						hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP03  + xsel/3);
						WM_SetFocus (hItem);	
						EDIT_SetCursorAtChar(hItem,xsel%3);
					}
				else if(ysel==7)	
					{
						xsel = (xsel+3)%4;
						hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP05  + xsel/2);
						WM_SetFocus (hItem);	
						EDIT_SetCursorAtChar(hItem,xsel%2);
					}
				else if(ysel==8)	
					{
						xsel = (xsel+11)%12;
						hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP07 + xsel/6);
						WM_SetFocus (hItem);	
						EDIT_SetCursorAtChar(hItem,xsel%6);
					}	
				else if(ysel>2)	
					{
						xsel = (xsel+2)%3;
						hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + ysel-3);
						WM_SetFocus (hItem);	
						EDIT_SetCursorAtChar(hItem,xsel);
					}		
				break;
		case MSG_SetRIGHT:
				if(ysel==6)
					{
						xsel = (xsel+1)%6;
						hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP03  + xsel/3);
						WM_SetFocus (hItem);	
						EDIT_SetCursorAtChar(hItem,xsel%3);
					}
				else if(ysel==7)	
					{
						xsel = (xsel+1)%4;
						hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP05  + xsel/2);
						WM_SetFocus (hItem);	
						EDIT_SetCursorAtChar(hItem,xsel%2);
					}
				else if(ysel==8)	
					{
						xsel = (xsel+1)%12;
						hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP07 + xsel/6);
						WM_SetFocus (hItem);	
						EDIT_SetCursorAtChar(hItem,xsel%6);
					}	
				else if(ysel>2)	
					{
						xsel = (xsel+1)%3;
						hItem = WM_GetDialogItem(hDlg, ID_EDIT_APP00 + ysel-3);
						WM_SetFocus (hItem);	
						EDIT_SetCursorAtChar(hItem,xsel);
					}			
				break;
 		/************************************************************
			按键消息常规处理
			*/	
	  case MSG_SetInc:		
			GUI_SendKeyMsg(GUI_KEY_UP, 1);
		  if(ysel<2){
				Clear_CAN1();//初始化，清CAN1状态
				CAN_SEND_APP06(pMsg,1);
			}
	 		break;
	  case MSG_SetDec:			
			GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
		  if(ysel<2){
				Clear_CAN1();//初始化，清CAN1状态
				CAN_SEND_APP06(pMsg,1);
			}
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
			  CAN_SEND_APP06(pMsg,0);
			}
			break;
		case WM_TIMER:
			REC_Handler_APP06(pMsg);
			//针对初始化，如果没有收到恢复，多次尝试(3次)
			if((DevSys.CAN1_Rec1st==0)&&(DevSys.CAN1_Reqcnt<NUM_CAN1_RETRY))
				{
						CAN_SEND_APP06(pMsg,1);
						#ifdef SHOW_DEBUG		//调试信息
						if(DevSys.CAN1_Reqcnt==NUM_CAN1_RETRY-1)
							TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP01), GUI_ORANGE);
						#endif
				}
			WM_RestartTimer(pMsg->Data.v, 20);	
			break;		
		case MSG_FOUCE:
			xsel=ysel=0;
		  WM_SetFocus (xitems[0]);	
    default:
      WM_DefaultProc(pMsg);
  }
}





