
#include "mainmenu.h"
#include "sys.h"




/*****************************************************
APP01-��������
*****************************************************/

static void REC_Handler_APP01(WM_MESSAGE * pMsg) 			
{
	WM_HWIN hItem ;
	uint16_t  mask=0x1002; 			 //2.->>CAN_BUF��������
	uint8_t data[5];
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//�յ��ı����Ƕ�Ӧ�ۣ���Ӧ���ڣ���Ӧ���͵ı���
	{
		//myprintf("CAN_REC :��������/n");
		data[0]	=	CAN1_RX1_BUF[S2_OFFSET+3]&0xf;
		data[1]	=	CAN1_RX1_BUF[S2_OFFSET+2]&0xf;
		data[2]	=	(CAN1_RX1_BUF[S2_OFFSET+2]&0xf0)>>4;
		data[3]	=	CAN1_RX1_BUF[S2_OFFSET+4]&0xf;  //IRIG-B2
		//͵����ֱ��MOD��һ������
		data[4] = data[3]>>2;
		data[3] = data[3]&0x3;
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
		DROPDOWN_SetSel(hItem,data[0]) ;
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);
		DROPDOWN_SetSel(hItem,data[1]) ;
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP2);
		DROPDOWN_SetSel(hItem,data[2]) ;
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP3);
		DROPDOWN_SetSel(hItem,data[3]) ;
		
		if( DevSys.USER_Right == 1 )//�߼��û�
		{
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP4);
			DROPDOWN_SetSel(hItem,data[4]) ;
		}
		
	  DevSys.CAN1_Rec1st=1;//CAN1�������
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
		//myprintf("CAN_REQ :��������/n");
		can_err=CAN1_Send_REQ(send_id);
		if(DevSys.CAN1_Reqcnt<NUM_CAN1_RETRY+1)
			DevSys.CAN1_Reqcnt++;//CAN1����++		
	}
	else
	{
		uint8_t  p[4];
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);//����
		p[0]= DROPDOWN_GetSel(hItem);
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);//������
		p[1]= DROPDOWN_GetSel(hItem);
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP2);//У�鷽ʽ
		p[2]= DROPDOWN_GetSel(hItem);
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP3);//B��У�鷽ʽ
		p[3]= DROPDOWN_GetSel(hItem);
		
		msg[0]= 0x22;
		msg[1]= 0x0;
		msg[2]= (p[2]<<4) | p[1];
		msg[3]= p[0];
		msg[4]= p[3]&0x3;
		
		if( DevSys.USER_Right == 1 )//�߼��û�
		{
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP4);//B������λ
			msg[5]= DROPDOWN_GetSel(hItem);
		}
		else{
			msg[5]=CAN1_RX1_BUF[S2_OFFSET+5]&0xf;
		}
		//͵����ֱ��MOD��һ������
		msg[4] |= (msg[5]&0x1)<<2 ;
		msg[5] = 0;
		can_err=CAN1_Send_Msg(msg,send_id,8);
		//myprintf("CAN_SEND:��������/n");
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
			PAINTTITL_SETAPP(4);/*���������ʹ���*/
		  GUI_DrawLine(XPOS0+30,YPOS0-5,XPOS0+30,YPOS0+YGAPS*5);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);//���ñ�����
		  //����������
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			DROPDOWN_AddString(hItem, "����");  
			DROPDOWN_AddString(hItem, "����");
		  DROPDOWN_AddString(hItem, "����");
			DROPDOWN_AddString(hItem, "GPRMC");
		  DROPDOWN_AddString(hItem, "����(��Ƶ������)");
		  DROPDOWN_AddString(hItem, "BJT");
		  DROPDOWN_AddString(hItem, "BJT(��Ƶ������)");
		  DROPDOWN_AddString(hItem, "����");

		  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1); 
      DROPDOWN_AddString(hItem, "1200");		
		  DROPDOWN_AddString(hItem, "2400");  
			DROPDOWN_AddString(hItem, "4800");
		  DROPDOWN_AddString(hItem, "9600");
		  DROPDOWN_AddString(hItem, "19200");
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP2);  
		  DROPDOWN_AddString(hItem, "��У��");
		  DROPDOWN_AddString(hItem, "��У��");  
			DROPDOWN_AddString(hItem, "żУ��");
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP3);   
		  DROPDOWN_AddString(hItem, "��У��");
		  DROPDOWN_AddString(hItem, "��У��");  
			DROPDOWN_AddString(hItem, "żУ��");
			
			if( DevSys.USER_Right == 1 )//�߼��û�
				{
					
					hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP4);
					DROPDOWN_AddString(hItem, "��������λ");  
					DROPDOWN_AddString(hItem, "����0");
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
			Clear_CAN1();//��ʼ������CAN1״̬
		  CAN_SEND_APP01(pMsg,1); //��������
      break;
 		/************************************************************
			�������⴦��
			*/	
		case MSG_FOUCE:
		//case WM_PRE_PAINT:	
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			WM_SetFocus (hItem);
			break;
		/************************************************************
			������Ϣ���洦��
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
				 GUI_EndDialog(hDlg, 0);MenuSet();//���ص����ò˵�����
			}
			break;	
		/************************************************************
			���ù��ܵ�ʵ��
			*/	
		case MSG_VerifyOK:
			{
				CAN_SEND_APP01(pMsg,0);
			}
			break;
		case WM_TIMER:
			REC_Handler_APP01(pMsg);	
		  //��Գ�ʼ�������û���յ��ָ�����γ���(3��)
			if((DevSys.CAN1_Rec1st==0)&&(DevSys.CAN1_Reqcnt<NUM_CAN1_RETRY))
				{
						CAN_SEND_APP01(pMsg,1);
						#ifdef SHOW_DEBUG		//������Ϣ
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



