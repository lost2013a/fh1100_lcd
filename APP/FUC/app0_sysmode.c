
#include "mainmenu.h"
#include "sys.h"


/*****************************************************
APP00-��������
*****************************************************/

static void REC_Handler_APP00(WM_MESSAGE * pMsg) 			
{
	WM_HWIN hItem ;
	uint16_t mask=0x1001; 			 //2.->>CAN_BUF��������
	uint8_t data[4];
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//�յ��ı����Ƕ�Ӧ�ۣ���Ӧ���ڣ���Ӧ���͵ı���
	{
		//myprintf("CAN_REC :��������/n");
		data[0]  = CAN1_RX1_BUF[SETSYSMODE_OFFSET];
		data[1]  = CAN1_RX1_BUF[SETSYSMODE_OFFSET+1];
		//͵����ֱ��MOD��һ������
		data[2] = data[1];
		data[1] = data[0]&0xf;
		data[0] = data[0]>>4;
		data[3] = data[2]&0xf;
		data[2] = data[2]>>4;
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
		DROPDOWN_SetSel(hItem,data[0]);
		if( DevSys.USER_Right == 1)//�߼��û�
			{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);
				DROPDOWN_SetSel(hItem,data[1]);
				hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP2);
				DROPDOWN_SetSel(hItem,data[2]);
				hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP3);
				DROPDOWN_SetSel(hItem,data[3]);
			}
		DevSys.CAN1_Rec1st=1;//CAN1�������	
	}
	CAN1_RX1_STA=0;
}

void CAN_SEND_APP00(WM_MESSAGE * pMsg,uint8_t rtr) 			
{
	WM_HWIN hItem ;
	uint32_t send_id = 0x03100100;			//CANID
	uint8_t  msg[8]  = {0}; 
	uint8_t  can_err;
	if(rtr)
	{
		//myprintf("CAN_REQ :��������/n");
		if(DevSys.CAN1_Reqcnt<NUM_CAN1_RETRY+1)
			DevSys.CAN1_Reqcnt++;//CAN1����++
		can_err=CAN1_Send_REQ(send_id);
	}
	else
	{
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
		msg[0]= DROPDOWN_GetSel(hItem);	
		if( DevSys.USER_Right == 1)//�߼��û�
			{		
				hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);
				msg[1]= DROPDOWN_GetSel(hItem);
				if(1){
					hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP2);
					msg[2]= DROPDOWN_GetSel(hItem);
					hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP3);
					msg[3]= DROPDOWN_GetSel(hItem);
				}
			}
		else
		{
			msg[1]=CAN1_RX1_BUF[SETSYSMODE_OFFSET+1];
			msg[2]=CAN1_RX1_BUF[SETSYSMODE_OFFSET+2];
			msg[3]=CAN1_RX1_BUF[SETSYSMODE_OFFSET+3];
		}
		
		//͵����ֱ��MOD��һ������
		msg[0] = ((msg[0]&0xf)<<4) | (msg[1]&0xf);
		msg[1] = ((msg[2]&0xf)<<4) | (msg[3]&0xf);
		can_err=CAN1_Send_Msg(msg,send_id,8);
		//myprintf("CAN_SEND:��������/n");
	}
	CAN_SHOW_ERR(can_err);
}


 void _cbCallback00(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
	int i;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			if( DevSys.USER_Right == 1 ){
				PAINTTITL_SETAPP(4);/*���������ʹ���*/
				GUI_DrawLine(XPOS0-10,YPOS0-5,XPOS0-10,YPOS0+YGAPS*4);
			}
			else{
				PAINTTITL_SETAPP(1);/*���������ʹ���*/
				GUI_DrawLine(XPOS0-10,YPOS0-5,XPOS0-10,YPOS0+YGAPS*1);
			}
			//PAINTTITL_SETAPP(1);/*���������ʹ���*/
		  //GUI_DrawLine(XPOS0-10,YPOS0-5,XPOS0-10,YPOS0+YGAPS*1);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);//���ñ�����
		  //����������
			if( DevSys.USER_Right == 1 )//�߼��û�
				{
					
					hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1);
					DROPDOWN_AddString(hItem, "��Դ(��׼)");  
					DROPDOWN_AddString(hItem, "��Դ(��׼)");
					DROPDOWN_AddString(hItem, "����");  
					DROPDOWN_AddString(hItem, "����");  
					for(i=0;i<2;i++){
						hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP2+i);
						DROPDOWN_AddString(hItem, "��Ч");  
						DROPDOWN_AddString(hItem, "UBLOX");  
						DROPDOWN_AddString(hItem, "̩��");
						DROPDOWN_AddString(hItem, "��������");
						DROPDOWN_AddString(hItem, "����ķ");
						DROPDOWN_AddString(hItem, "����1");
						DROPDOWN_AddString(hItem, "����2");
					}
				}
			else
				for(i=0;i<3;i++){
					hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04+i);
					WM_DeleteWindow(hItem);
					hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP1+i);
					WM_DeleteWindow(hItem);
			}
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);  
			DROPDOWN_AddString(hItem, "��ʱ��");  
			DROPDOWN_AddString(hItem, "��ʱ��");	
			WM_SetFocus (hItem);
			Clear_CAN1();//��ʼ������CAN1״̬	
			CAN_SEND_APP00(pMsg,1); //��������
      break;
 		/************************************************************
			�������⴦��
			*/	
		case MSG_FOUCE:
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
			if(WM_IsWindow(hWinSetapp)){	
				 GUI_EndDialog(hDlg, 0);MenuSet();//���ص����ò˵�����
			}
			break;	
		/************************************************************
			���ù��ܵ�ʵ��
			*/	
		case MSG_VerifyOK:
			{
				CAN_SEND_APP00(pMsg,0);
			}
			break;
		case WM_TIMER:
			REC_Handler_APP00(pMsg);
			//��Գ�ʼ�������û���յ��ָ�����γ���(3��)
			if((DevSys.CAN1_Rec1st==0)&&(DevSys.CAN1_Reqcnt<NUM_CAN1_RETRY))
				{
						CAN_SEND_APP00(pMsg,1);
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



