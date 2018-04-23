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
	/*画标题栏*/
	GUI_SetColor(GUI_COL_TITLE1);
	GUI_FillRect(0, 0, 800, 36 );
	GUI_SetPenSize(1);
	GUI_SetColor(GUI_WHITE);
	GUI_DrawLine(170,8,170,34);
	GUI_DrawLine(0,37,800,37);
	GUI_SetColor(GUI_BLACK);
	GUI_DrawLine(0,38,800,38);
	/*画正文框*/
	GUI_SetColor(GUI_WHITE);
	GUI_FillRoundedRect(20,55,740,440,8);
	/*画装饰的线条*/
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
APP01-串口设置
*****************************************************/

static void REC_Handler_APP14(WM_MESSAGE * pMsg) 			
{
	uint16_t  mask=0x102a; 			 //2.->>CAN_BUF报文类型
	//char data[128]={"2017-03-21#V1.00  校验码8286#V1.00  校验码345F#V1.00  校验码345F#"};
	char buf[40]={0};
	char *p[5],i,k;
	
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//收到的报文是对应槽，对应网口，对应类型的报文
	{
		//myprintf("CAN_REC :版本信息/n");
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
	//myprintf("CAN_REQ :版本信息/n");
	CAN1_RX1_STA=0;
	can_err=CAN1_Send_REQ(send_id);
	CAN_SHOW_ERR(can_err);
}


extern char BDTIMEBUF[30];

void _cbCallback14(WM_MESSAGE * pMsg) 
{
	/*IP设置中使用了静态变量来实现光标位置的变化*/
	static uint8_t progress = 0;
	static uint32_t readbuff[4] ;
	static uint8_t xsel =0;
  WM_HWIN hItem;
  int     i;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			/*画标题栏和窗口*/
		  PAINTTITL_INFOAPP(7);
			
		   GUI_DrawLine(XPOS0-10,YPOS0-45,XPOS0-10,YPOS0+YGAPS*5+35);
			/*填写-号*/ 
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
			 
			//布置标题栏
       PAINTWIN_SETAPP(pMsg);
		  //布置下拉框
			
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04);  
			TEXT_SetFont(hItem, &GUI_FontHZ16);
			if( DevSys.USER_Right == 1 ){
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP01);  
				TEXT_SetText(hItem,"装置界面软件日期" );
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP02);  
				TEXT_SetText(hItem,BDTIMEBUF );
			}
		
       //布置EDIT框
      FLASH_Read(FLASH_SAVE_ADDR1,readbuff,3);//读出信息到readflash中
		   if(readbuff[0] == FLASH_SIGN+2)
					
			 /*  =======日期已经设置好，直接显示=======     */
			 {
					char tbuf[20] = {0};
					progress = 2;
					for(i=0;i<6;i++)
					{
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00 + i ));
					}
					
					sprintf((char*)tbuf,"%04d年%02d月%02d日",(readbuff[1]&0xffff0000)>>16,(readbuff[1]&0xff00)>>8,readbuff[1]&0xff);		
					//hItem = TEXT_CreateEx(	XPOS0,YPOS0+YGAPS*0, 200, RECTY,hWinSetapp, WM_CF_SHOW,TEXT_CF_VCENTER, ID_TEXT_APP11,0);
					hItem=WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP11);
				  TEXT_SetFont(hItem, &GUI_FontHZ24);
				  TEXT_SetText(hItem, (char*)tbuf);
				  memset(tbuf,0,20);
				  sprintf((char*)tbuf,"%04d年%02d月%02d日",(readbuff[2]&0xffff0000)>>16,(readbuff[2]&0xff00)>>8,readbuff[2]&0xff);		
          //hItem = TEXT_CreateEx(	XPOS0,YPOS0+YGAPS*1, 200, RECTY,hWinSetapp, WM_CF_SHOW,TEXT_CF_VCENTER, ID_TEXT_APP12,0);
					hItem=WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP12);
				  TEXT_SetFont(hItem, &GUI_FontHZ24);
				  TEXT_SetText(hItem, (char*)tbuf);		
					//TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04), "成都府河电力自动化成套设备有限责任公司制造\n地址：成都市高新区     服务电话：028-85120377 ");	
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04), "服务电话：028-85120377\n成都府河电力自动化成套设备有限责任公司制造");	
			 }
			 else if(readbuff[0] == FLASH_SIGN+1) //注意要删除EDIT 0-2
			
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
					sprintf((char*)tbuf,"%04d年%02d月%02d日",(readbuff[1]&0xffff0000)>>16,(readbuff[1]&0xff00)>>8,readbuff[1]&0xff);			
				  TEXT_SetText(hItem, (char*)tbuf);
				  
					
					hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP04 ); 
				  EDIT_SetDecMode(hItem, 01, 01, 12, 0, 0);
					EDIT_SetValue(hItem,DevHead.month);
					hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP05 ); 
					EDIT_SetDecMode(hItem, 01, 01, 31, 0, 0); 
					EDIT_SetValue(hItem,DevHead.day);	
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04), "尊敬的用户你好:\n    投运日期尚未完善，请在装置投运时设置正确日期,按<确认>键保存。");
					
					hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP03 );
					EDIT_SetDecMode(hItem, 0, 2010, 2200, 0, 0);
					EDIT_SetValue(hItem,DevHead.year);
					
					//REC_Handler_APP14(pMsg);
          WM_SetFocus (hItem); 
			 }		
			 else  //注意要删除EDIT 3-5
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
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04), "请设置出厂时间，按<确认>键保存");
				  
					hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_APP00 );
					EDIT_SetDecMode(hItem, 0, 2010, 2200, 0, 0);	
					EDIT_SetValue(hItem,DevHead.year);
				  WM_SetFocus (hItem); 
			 }	 
			CAN_SEND_APP14(); 
      break;
		/************************************************************
			按键消息特殊处理
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
			按键消息常规处理
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
				for(i=0;i<1000;i++){}//等待烧写结束
				if(WM_IsWindow(hWinSetapp))
				{	
					 GUI_EndDialog(pMsg->hWin, 0);Menuapp();//返回到主菜单窗口
				}
			}
			break;
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinSetapp))
			{	
				 GUI_EndDialog(pMsg->hWin, 0);Menuapp();//返回到主菜单窗口
			}
			break;	
		/************************************************************
			设置功能的实现
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
