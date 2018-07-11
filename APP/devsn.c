#include "mainmenu.h"
#include "sys.h"
#include "flash_conf.h"
extern void PAINTWIN_SETAPP(WM_MESSAGE * pMsg);
extern GUI_CONST_STORAGE GUI_BITMAP bmfuhe1 ;
void PAINTTITL_SETAPP(uint8_t line);;
#define FLASH_SIGN  0x0a

void PAINTTITL_INFOAPP(uint8_t line)
{
		int i;
	/*��������*/
	GUI_SetColor(GUI_COL_TITLE1);
	GUI_FillRect(0, 0, 800, 36 );
	GUI_SetPenSize(1);
	GUI_SetColor(GUI_WHITE);
	GUI_DrawLine(170,8,170,34);
	GUI_DrawLine(0,37,800,37);
	GUI_SetColor(GUI_BLACK);
	GUI_DrawLine(0,38,800,38);
	/*�����Ŀ�*/
	GUI_SetColor(GUI_WHITE);
	GUI_FillRoundedRect(20,55,740,440,8);
	/*��װ�ε�����*/
	GUI_SetColor(GUI_COL_WIN1);
	GUI_SetPenSize(3);
	GUI_DrawLine(80,72,700,72);
	GUI_SetPenSize(1);
	for (i = 1; i < 4; i++) 
	 {
		GUI_DrawLine(80,72+YGAPS*i,400,72+YGAPS*i);
	 }
	for (i = 4; i < line+1; i++) 
	 {
		GUI_DrawLine(80,72+YGAPS*i,200+i*50,72+YGAPS*i);
	 }
}

/*****************************************************
APP01-��������
*****************************************************/

static void REC_Handler_APP14(WM_MESSAGE * pMsg) 			
{
	uint16_t  mask=0x102a; 			 //2.->>CAN_BUF��������
	//char data[128]={"2017-03-21#V1.00  У����8286#V1.00  У����345F#V1.00  У����345F#"};
	char buf[40]={0};
	char *p[5],i,k;
	
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//�յ��ı����Ƕ�Ӧ�ۣ���Ӧ���ڣ���Ӧ���͵ı���
	{
		//myprintf("CAN_REC :�汾��Ϣ/n");
		p[0]=(char*)&CAN1_RX1_BUF[SN_OFFSET];
		for(i=1;i<5;i++)
			{
				p[i]=strchr(p[i-1]+1,'#'); 
				if(p[i]==NULL)
					{
						break;
					}
				else
					{
						p[i]=p[i]+1;
					}
			}
		k=i;
		for(i=0;i<k-1;i++)
			{
					memset(buf,0,40);
					memcpy(buf,p[i],p[i+1]-p[i]-1);
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP05+i), buf);	
					//printf("[%d][%s]\n",i,buf);
			}
	  
	}
	CAN1_RX1_STA=0;
}



void CAN_SEND_APP14(void) 			
{
	uint32_t send_id = 0x03102af0;			//CANID
	uint8_t  can_err;
	//myprintf("CAN_REQ :�汾��Ϣ/n");
	CAN1_RX1_STA=0;
	can_err=CAN1_Send_REQ(send_id);
	CAN_SHOW_ERR(can_err);
}


extern char BDTIMEBUF[30];

void _cbCallback14(WM_MESSAGE * pMsg) 
{
	/*IP������ʹ���˾�̬������ʵ�ֹ��λ�õı仯*/
	static uint8_t progress = 0;
	static uint32_t readbuff[4] ;
	static uint8_t xsel =0;
  WM_HWIN hItem;
  int     i;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			/*���������ʹ���*/
		  PAINTTITL_INFOAPP(7);
			
		   GUI_DrawLine(XPOS0-10,YPOS0-45,XPOS0-10,YPOS0+YGAPS*5+35);
			/*��д-��*/ 
		  GUI_SetTextMode(GUI_TM_TRANS);
		  GUI_SetColor(GUI_BLACK);
		
      GUI_DrawBitmap(&bmfuhe1,500,YPOS0-10);
    

			if( progress<2 )
				{
					GUI_DispCharAt('-',XPOS0+XGAPS*1,  YPOS0+YGAPS*(0+progress)+10);
					GUI_DispCharAt('-',XPOS0+XGAPS*2-8,YPOS0+YGAPS*(0+progress)+10);
				}
        break;
    case WM_INIT_DIALOG:
			
			//���ñ�����
       PAINTWIN_SETAPP(pMsg);
		  //����������
			
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04);  
			TEXT_SetFont(hItem, &GUI_FontHZ16);
			if( DevSys.USER_Right == 1 ){
				u16 year;
				u8 mon,day,type,buf[2];
				char tbuf[20] = {0};
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP01);  
				TEXT_SetText(hItem,"װ�ý����������" );
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP02);  
				TEXT_SetText(hItem,BDTIMEBUF );
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP09);  
				TEXT_SetText(hItem,"FPGA����汾" );
				
				
				buf[0]  =   CAN1_RX0_BUF[T1_OFFSET+6];		
				buf[1]  = 	CAN1_RX0_BUF[T1_OFFSET+7];
				year  = (buf[0]>>2) +2000;
				mon   = ((buf[0]&0x3)<<2)|buf[1]>>6;
				day   = (buf[1]&0x3e)>>1;
				type	= buf[1]&0x1;
				
				if(year>=2018){
					 if(type)
							sprintf((char*)tbuf,"%04d��%02d��%02d��/����",year,mon,day);	
						else
							sprintf((char*)tbuf,"%04d��%02d��%02d��/����",year,mon,day);	
			
				}
				else{
					sprintf((char*)tbuf,"FPGA����汾̫�ͣ�������");	
				}
					hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10);  
				TEXT_SetText(hItem,(char*)tbuf );
			}
			
		
       //����EDIT��
      FLASH_Read(FLASH_SAVE_ADDR1,readbuff,3);//������Ϣ��readflash��
		   if(readbuff[0] == FLASH_SIGN+2)
					
			 /*  =======�����Ѿ����úã�ֱ����ʾ=======     */
			 {
					char tbuf[20] = {0};
					progress = 2;
					for(i=0;i<6;i++)
					{
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00 + i ));
					}
					
					sprintf((char*)tbuf,"%04d��%02d��%02d��",(readbuff[1]&0xffff0000)>>16,(readbuff[1]&0xff00)>>8,readbuff[1]&0xff);		
					//hItem = TEXT_CreateEx(	XPOS0,YPOS0+YGAPS*0, 200, RECTY,hWinSetapp, WM_CF_SHOW,TEXT_CF_VCENTER, ID_TEXT_APP11,0);
					hItem=WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP11);
				  TEXT_SetFont(hItem, &GUI_FontHZ24);
				  TEXT_SetText(hItem, (char*)tbuf);
				  memset(tbuf,0,20);
				  sprintf((char*)tbuf,"%04d��%02d��%02d��",(readbuff[2]&0xffff0000)>>16,(readbuff[2]&0xff00)>>8,readbuff[2]&0xff);		
          //hItem = TEXT_CreateEx(	XPOS0,YPOS0+YGAPS*1, 200, RECTY,hWinSetapp, WM_CF_SHOW,TEXT_CF_VCENTER, ID_TEXT_APP12,0);
					hItem=WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP12);
				  TEXT_SetFont(hItem, &GUI_FontHZ24);
				  TEXT_SetText(hItem, (char*)tbuf);		
					//TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04), "�ɶ����ӵ����Զ��������豸�������ι�˾����\n��ַ���ɶ��и�����     ����绰��028-85120377 ");	
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04), "����绰��028-85120377\n�ɶ����ӵ����Զ��������豸�������ι�˾����");	
			 }
			 else if(readbuff[0] == FLASH_SIGN+1) //ע��Ҫɾ��EDIT 0-2
			
			 {
					char tbuf[20] = {0};
					progress = 1;
					xsel =0;
					
					for(i=0;i<3;i++)
					{
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00 + i ));
					}
					//hItem = TEXT_CreateEx(	XPOS0,YPOS0+YGAPS*0, 200, RECTY,hWinSetapp, WM_CF_SHOW,TEXT_CF_VCENTER, ID_TEXT_APP11,0);	
					hItem=WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP11);
					TEXT_SetFont(hItem, &GUI_FontHZ24);
					sprintf((char*)tbuf,"%04d��%02d��%02d��",(readbuff[1]&0xffff0000)>>16,(readbuff[1]&0xff00)>>8,readbuff[1]&0xff);			
				  TEXT_SetText(hItem, (char*)tbuf);
				  
					
					hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP04 ); 
				  EDIT_SetDecMode(hItem, 01, 01, 12, 0, 0);
					EDIT_SetValue(hItem,DevHead.month);
					hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP05 ); 
					EDIT_SetDecMode(hItem, 01, 01, 31, 0, 0); 
					EDIT_SetValue(hItem,DevHead.day);	
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04), "�𾴵��û����:\n    Ͷ��������δ���ƣ�����װ��Ͷ��ʱ������ȷ����,��<ȷ��>�����档");
					
					hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP03 );
					EDIT_SetDecMode(hItem, 0, 2010, 2200, 0, 0);
					EDIT_SetValue(hItem,DevHead.year);
					
					//REC_Handler_APP14(pMsg);
          WM_SetFocus (hItem); 
			 }		
			 else  //ע��Ҫɾ��EDIT 3-5
			 {
					progress = 0;
					xsel = 0;
				  for(i=0;i<3;i++)
					{
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP03 + i ));
					}
				
					hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP01 );
				  EDIT_SetDecMode(hItem, 01, 01, 12, 0, 0);
					EDIT_SetValue(hItem,DevHead.month);
					hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP02 );  
					EDIT_SetDecMode(hItem, 01, 01, 31, 0, 0); 
					EDIT_SetValue(hItem,DevHead.day);
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04), "�����ó���ʱ�䣬��<ȷ��>������");
				  
					hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00 );
					EDIT_SetDecMode(hItem, 0, 2010, 2200, 0, 0);	
					EDIT_SetValue(hItem,DevHead.year);
				  WM_SetFocus (hItem); 
			 }	 
			CAN_SEND_APP14(); 
      break;
		/************************************************************
			������Ϣ���⴦��
			*/	
		case MSG_SetLEFT:
			 if(progress < 2)
			 {
				xsel = (xsel+7)%8;
		    if(xsel<4)
					{
						hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00 +progress*3);
						WM_SetFocus (hItem);	
						EDIT_SetCursorAtChar(hItem,xsel);
					}
				else
					{
						hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00 + 1 + (xsel-4)/2+progress*3 );
						WM_SetFocus (hItem);	
						EDIT_SetCursorAtChar(hItem,(xsel-4)%2);
					}
			 }
			break;
		case MSG_SetRIGHT:
			if(progress < 2)
			 {
				xsel = (xsel+1)%8;
		    if(xsel<4)
					{
						hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00 +progress*3 );
						WM_SetFocus (hItem);	
						EDIT_SetCursorAtChar(hItem,xsel);
					}
				else
					{
						hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00 + 1 + (xsel-4)/2 +progress*3 );
						WM_SetFocus (hItem);	
						EDIT_SetCursorAtChar(hItem,(xsel-4)%2);
					}
			 }
			break;
 		/************************************************************
			������Ϣ���洦��
			*/	
	  case MSG_SetInc:		
			GUI_SendKeyMsg(GUI_KEY_UP, 1);
			
	 		break;
	  case MSG_SetDec:			
			GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
			
			break;
		case MSG_SetENTER:
			if(progress < 2)
			 {
					Password_Verify();
			 }
		  break;
		case MSG_VerifyOK:
			if(progress < 2)
			{
				int  i;
			  static uint32_t writeflash[3];
				writeflash[1] = readbuff[1];
				writeflash[0] 			 = progress +1 + 0xa;
			
				writeflash[1+progress] = (EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00+progress*3))<<16)|(EDIT_GetValue(WM_GetDialogItem(pMsg->hWin,
				ID_EDIT_APP01+progress*3))<<8)|EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP02+progress*3))  ;
				
			  FLASH_Write(FLASH_SAVE_ADDR1,writeflash,3);
				for(i=0;i<1000;i++){}//�ȴ���д����
				if(WM_IsWindow(hWinSetapp))
				{	
					 GUI_EndDialog(pMsg->hWin, 0);Menuapp();//���ص����˵�����
				}
			}
			break;
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinSetapp))
			{	
				 GUI_EndDialog(pMsg->hWin, 0);Menuapp();//���ص����˵�����
			}
			break;	
		/************************************************************
			���ù��ܵ�ʵ��
			*/	
		case MSG_FOUCE:
      if(progress < 2)
			 {
				 hItem =WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00+progress*3 );
				 WM_SetFocus (hItem);
			 }
			 break;	
		case WM_TIMER:
			{
			 REC_Handler_APP14(pMsg);
			}
			WM_RestartTimer(pMsg->Data.v, 20);	
			break;		 
    default:
      WM_DefaultProc(pMsg);
  }
}


//xitems[0] = WM_GetDialogItem(hDlg, ID_DROPDOWN_APP0 );
/*************************** End of file ****************************/
