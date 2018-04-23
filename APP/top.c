#include "sys.h"
#include "Mainmenu.h"
extern GUI_CONST_STORAGE GUI_BITMAP bmpic1;
extern GUI_CONST_STORAGE GUI_BITMAP bmpic2;
extern GUI_CONST_STORAGE GUI_BITMAP bmpic3;
extern GUI_CONST_STORAGE GUI_BITMAP bmpic4;
extern GUI_CONST_STORAGE GUI_BITMAP bmpic5;
extern GUI_CONST_STORAGE GUI_BITMAP bmpic6;
extern uint8_t	s_ucSelIconIndex ; 
extern uint8_t	s_ucSelIconIndex1 ; 
extern uint8_t	s_ucSelIconIndex2 ; 
extern void  Openapp(void);
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
  { WINDOW_CreateIndirect, "", ID_WINDOW_TOP,0,     0,   800, 480, 0, 0x0,  0 },
  { TEXT_CreateIndirect,   "", ID_TEXT_SYS,  25,    20,  100, 40,  GUI_TA_HCENTER|GUI_TA_VCENTER, 0x64, 0 },
  { TEXT_CreateIndirect,   "", ID_TEXT_SOCUE,550,   20,  240, 40,  GUI_TA_HCENTER|GUI_TA_VCENTER, 0x64, 0 },
  { TEXT_CreateIndirect,   "", ID_TEXT_DATA, 250,   225, 300, 50,  GUI_TA_HCENTER|GUI_TA_VCENTER, 0x64, 0 },
  { TEXT_CreateIndirect,   "", ID_TEXT_TIME, 200,   110, 400, 100, GUI_TA_HCENTER|GUI_TA_VCENTER, 0x64, 0 },
	{ TEXT_CreateIndirect,   "", ID_TEXT_BD,   10,    456, 96,  24,  GUI_TA_LEFT, 0x64, 0 },
	{ TEXT_CreateIndirect,   "", ID_TEXT_GPS,  120,   456, 96,  24,  GUI_TA_LEFT, 0x64, 0 },
	{ TEXT_CreateIndirect,   "", ID_TEXT_B1,   230,   456, 96,  24,  GUI_TA_LEFT, 0x64, 0 },
	{ TEXT_CreateIndirect,   "", ID_TEXT_B2,   340,   456, 96,  24,  GUI_TA_LEFT, 0x64, 0 },
	{ TEXT_CreateIndirect,   "", ID_TEXT_PTP1,   450+10,   456, 96,  24,  GUI_TA_LEFT, 0x64, 0 },	//PTP1
	{ TEXT_CreateIndirect,   "", ID_TEXT_PTP2,   560+10,   456, 96,  24,  GUI_TA_LEFT, 0x64, 0 },	//PTP2
};

static void show_toptime(WM_MESSAGE * pMsg)
{ 
	static u8 TBUFF_LENTH = 20;
	char tbuf[20]={0};
	/*
	*********************************************************************************************************
	*                                      大时间
	*********************************************************************************************************
	*/	
	sprintf((char*)tbuf,"%02d:%02d:%02d",DevHead.h,DevHead.m,DevHead.s);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_TIME),(const char *)tbuf);
	memset(tbuf,0,TBUFF_LENTH);
	sprintf((char*)tbuf,"%04d年%02d月%02d日",DevHead.year,DevHead.month,DevHead.day);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_DATA),(const char *)tbuf);
	/*
	*********************************************************************************************************
	*                                      当前基准
	*********************************************************************************************************
	*/	
	memset(tbuf,0,TBUFF_LENTH); 
	switch(DevHead.selsouce)
		{
			case 0:	
				sprintf((char*)tbuf,"当前基准:北斗 %02d",DevHead.bds_num_capture);
				break;
			case 1:
				sprintf((char*)tbuf,"当前基准:GPS %02d",DevHead.gps_num_capture);
				break;
			case 2: case 5:
				sprintf((char*)tbuf,"当前基准:IRIG-B1 %1X",DevHead.b1_qual);
				break;
			case 3: case 6:
				sprintf((char*)tbuf,"当前基准:IRIG-B2 %1X",DevHead.b2_qual);
				break;
			case 7: 
				sprintf((char*)tbuf,"当前基准:PTP1");
				break;
			case 8: 
				sprintf((char*)tbuf,"当前基准:PTP2");
				break;
			case 4:
				if(DevHead.synflag){
					sprintf((char*)tbuf,"自守时");
				}	
				else{
					sprintf((char*)tbuf,"自守时:%01X",DevHead.self_qual);
				}
				break;
		}	
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_SOCUE),(const char *)tbuf);	
			
	/*
	*********************************************************************************************************
	*                                     主从状态
	*********************************************************************************************************
	*/				
	memset(tbuf,0,TBUFF_LENTH);
	if(DevHead.is_master==0)
		sprintf((char*)tbuf,"主时钟");			
	else									
		sprintf((char*)tbuf,"从时钟");
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_SYS),(const char *)tbuf);	
	/*
	*********************************************************************************************************
	*                                     源状态
	*********************************************************************************************************
	*/			
	if(DevHead.bds==0)//有源信号才显示状态
		{
			memset(tbuf,0,TBUFF_LENTH);
			sprintf((char*)tbuf,"北斗:%02d",DevHead.bds_num_capture);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_BD),(const char *)tbuf);
			if(DevHead.bds_sta_synch==0) 
				TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_BD), GUI_WHITE);
			else
				TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_BD), GUI_GRAY_10);	
		}
	else
		{
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_BD),"");
		}
	if(DevHead.gps==0)
		{
			memset(tbuf,0,TBUFF_LENTH);
			sprintf((char*)tbuf,"GPS:%02d",DevHead.gps_num_capture);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_GPS),(const char *)tbuf);
			if(DevHead.gps_sta_synch==0) 
				TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_GPS), GUI_WHITE);
			else
				TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_GPS), GUI_GRAY_10);
		}
	else 	
		{
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_GPS),"");
		}
	if(DevHead.b1==0)
		{
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_B1),(const char *)"IRIG-B1");
			if(DevHead.b1_sta_synch==0) 
				TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_B1), GUI_WHITE);
			else
				TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_B1), GUI_GRAY_10);	
		}	
	else 	
		{
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_B1),"");
		}	
	if(DevHead.b2==0)
		{
		  TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_B2),(const char *)"IRIG-B2");
			if(DevHead.b2_sta_synch==0) 
				TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_B2), GUI_WHITE);
			else
				TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_B2), GUI_GRAY_10);
		}
	else 	
		{
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_B2),"");
		}	
	if(DevHead.ptp1==0)
		{
		  TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_PTP1),(const char *)"PTP1");
			if(DevHead.ptp1_sta_synch==0) 
				TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_PTP1), GUI_WHITE);
			else
				TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_PTP1), GUI_GRAY_10);
		}
	else 	
		{
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_PTP1),"");
		}		
	if(DevHead.ptp2==0)
		{
		  TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_PTP2),(const char *)"PTP2");
			if(DevHead.ptp2_sta_synch==0) 
				TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_PTP2), GUI_WHITE);
			else
				TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_PTP2), GUI_GRAY_10);
		}
	else 	
		{
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_PTP2),"");
		}			
}


static void show_Baricon(WM_MESSAGE * pMsg)
{
	if((DevHead.bds==0)&&(DevHead.bds_sta_synch==0))
		{
				GUI_DrawBitmap(&bmpic2,25,455-29);
		}
	if((DevHead.gps==0)&&(DevHead.gps_sta_synch==0))
		{
			GUI_DrawBitmap(&bmpic1,135,455-32);
		}		
	if((DevHead.b1==0)&&(DevHead.b1_sta_synch==0))
		{
			GUI_DrawBitmap(&bmpic3,245,455-25);
		}	
	if((DevHead.b2==0)&&(DevHead.b2_sta_synch==0))
		{
			GUI_DrawBitmap(&bmpic4,355,455-25);
		}		
	if((DevHead.ptp1==0)&&(DevHead.ptp1_sta_synch==0))
		{
			GUI_DrawBitmap(&bmpic5,465,455-32);
		}		
	if((DevHead.ptp2==0)&&(DevHead.ptp2_sta_synch==0))
		{
			GUI_DrawBitmap(&bmpic6,575,455-32);
		}				
}


/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			WINDOW_SetBkColor(pMsg->hWin, GUI_COL_TITLE1);//布置背景色
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SYS);
			TEXT_SetTextColor(hItem, GUI_WHITE);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SOCUE);
			TEXT_SetTextColor(hItem, GUI_WHITE);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_DATA);
			TEXT_SetTextColor(hItem, GUI_WHITE);
			TEXT_SetFont(hItem, &GUI_FontHZ32);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TIME);
			TEXT_SetTextColor(hItem, GUI_WHITE);
			TEXT_SetFont(hItem, GUI_FONT_D48X64);
		 	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_BD);
			TEXT_SetTextColor(hItem, GUI_WHITE);
			TEXT_SetFont(hItem, &GUI_FontHZ24);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_GPS);
			TEXT_SetTextColor(hItem, GUI_WHITE);
			TEXT_SetFont(hItem, &GUI_FontHZ24);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_B1);
			TEXT_SetTextColor(hItem, GUI_WHITE);
			TEXT_SetFont(hItem, &GUI_FontHZ24);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_B2);
			TEXT_SetTextColor(hItem, GUI_WHITE);
			TEXT_SetFont(hItem, &GUI_FontHZ24);	
			show_toptime( pMsg);  //显示时间
			break;
		case WM_PAINT:
			//画背景
		  GUI_SetColor(GUI_BLUE);
		  GUI_FillRect(0, 0, 800, 480);
	    show_Baricon( pMsg);
		  break;	
		case MSG_PPSTICK:
			myprintf("MSG_PPSTICK\n");
		  WM_SelectWindow(hWinTop);
	    show_toptime( pMsg);
		  show_Baricon( pMsg);
			break;
		case MSG_PPSTICK_300:
			WM_SelectWindow(hWinTop);
			GUI_SetColor(GUI_BLUE);
			GUI_FillRect(0,455-32,650,455);
			break;
		case MSG_SetENTER:
			if(WM_IsWindow(hWinTop))
				{
					GUI_EndDialog(hWinTop, 0); APP_Setup(12);//login app进入到用户登陆界面
				}
			break;	
		default:
			WM_DefaultProc(pMsg);
			break;
		 }
}
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

void Topapp(void) 
{
   hWinTop = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	 hWin_msgto = hWinTop;  
   s_ucSelIconIndex  = 0 ;
   s_ucSelIconIndex1 = 0 ;
	 s_ucSelIconIndex2 = 0 ;
   DevSys.USER_Right = 0 ;	
}

void MainTask(void) 
{
	WM_HWIN hWin ; 
	uint8_t  ucKeyCode;
	GUI_Init();
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	WM_SetCreateFlags(WM_CF_MEMDEV);
  TEXT_SetDefaultFont( &GUI_FontHZ24);
	TEXT_SetDefaultTextColor(GUI_BLACK);
	DROPDOWN_SetDefaultFont(&GUI_FontHZ24);
	EDIT_SetDefaultFont(&GUI_Font20_1) ;
	LISTVIEW_SetDefaultGridColor(GUI_COL_WIN1);
	LISTVIEW_SetDefaultFont(&GUI_FontHZ24);
	SCROLLBAR_SetDefaultWidth(12);
	CAN1_RX0_STA = 0;//清0
	CAN1_RX1_STA = 0;//清0
	Openapp();
  Topapp();
	while (1) 
	{ 
		ucKeyCode = KEY_Scan();
		hWin = hWin_msgto;
		if(ucKeyCode != KEY_NONE)
			{	
				//if(DevSys.idle_out_flag == STA_OK)
					{
						switch (ucKeyCode)
							{
								case KEY5_E:			
									WM_SendMessageNoPara(hWin, MSG_SetENTER);
									break;
								case KEY6_Q:			
									WM_SendMessageNoPara(hWin, MSG_SetQIUT);
									break;
								case KEY7_INC:			
									WM_SendMessageNoPara(hWin, MSG_SetInc);
									break;
								case KEY8_DEC:			
									WM_SendMessageNoPara(hWin, MSG_SetDec);
									break;
								case KEY1_U:			
									WM_SendMessageNoPara(hWin, MSG_SetUP);
									break;
								case KEY2_D:			
									WM_SendMessageNoPara(hWin, MSG_SetDOWM);
									break;
								case KEY3_L:			
									WM_SendMessageNoPara(hWin, MSG_SetLEFT);
									break;
								case KEY4_R:		
									WM_SendMessageNoPara(hWin, MSG_SetRIGHT);
									break;
								default:     
									break;
							}
					}
				 DevSys.idle_out	 = 0;
			}
		if(DevSys.PPS_Tick==1)
			{
				WM_SendMessageNoPara(hWin, MSG_PPSTICK);
				DevSys.PPS_Tick=0;
			}
		if(DevSys.PPS_Tick_300==1)
			{
				WM_SendMessageNoPara(hWin, MSG_PPSTICK_300);
				DevSys.PPS_Tick_300=0;
			}		
		GUI_Delay(3);
	}
}
/*************************** End of file ****************************/
