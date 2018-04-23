
#include "mainmenu.h"
#include "sys.h"
extern GUI_CONST_STORAGE GUI_BITMAP bmleft;
extern GUI_CONST_STORAGE GUI_BITMAP bmright;
extern uint8_t	s_ucSelIconIndex2;

static uint8_t data[16]={0};
//static uint8_t data[16]={0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x10};
u8 SLOT_P[16][16]=			//布尔量为1的点表
{
{"(空)"}	,	  //0x10
{"CPU板"},	
{"SIG板"},	
{"RS485板"},	
{"多模光纤板"},	
{"单模光纤板"},	
{"空接点板"},	
{"TTL板"},	
{"RS232板"},	
{"交流IRIG-B码板"},	
{"NET6板"},	
{"TOS板"},	
{"频率采集板"},	
{"电源模块1"},	
{"电源模块2"},
{"(空)"}	 
};

/*****************************************************
APP00-主从设置
*****************************************************/
static void REC_Handler_APP29(WM_MESSAGE * pMsg) 			
{
	uint16_t mask=0x1015; 			 //2.->>CAN_BUF报文类型
	uint8_t i;
	if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//收到的报文是对应槽，对应网口，对应类型的报文
	{
		//myprintf("CAN_REC :主从设置/n");
		for(i=0;i<16;i++)
		{
			data[i]=CAN1_RX1_BUF[SETSLOT_OFFSET+i];
		}
	}
	CAN1_RX1_STA=0;
}

void CAN_SEND_APP29(WM_MESSAGE * pMsg,uint8_t rtr) 			
{
	uint32_t send_id = 0x03101500;			//CANID
	uint8_t  can_err;
	if(rtr)
	{
		can_err=CAN1_Send_REQ(send_id);
	}
	CAN_SHOW_ERR(can_err);
}





void _cbCallback29(WM_MESSAGE * pMsg) 
{

	HEADER_Handle hHeader;
  WM_HWIN hItem;
	static int i =0,temp;
  static char tbuf1[10];
	static char tbuf2[16];
	static uint16_t timecnt = 0;
  switch (pMsg->MsgId) {
    case WM_PAINT:
		/*画标题栏*/
		GUI_SetColor(GUI_COL_TITLE1);
		GUI_FillRect(0, 0, 800, 36 );
		GUI_SetPenSize(1);
		GUI_SetColor(GUI_WHITE);
		GUI_DrawLine(170,8,170,34);
		GUI_DrawLine(0,37,800,37);
		GUI_SetColor(GUI_BLACK);
		GUI_DrawLine(0,38,800,38);
		GUI_DrawBitmap(&bmleft,30,480-46);
		// GUI_DrawBitmap(&bmright,680-48,480-46);
		break;
    case WM_INIT_DIALOG:
		PAINTWIN_SETAPP(pMsg);
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_APP0);  
		hHeader = LISTVIEW_GetHeader(hItem);
		HEADER_SetFont(hHeader, &GUI_FontHZ16);
		LISTVIEW_AddColumn(hItem, 85, "插槽", GUI_TA_CENTER|TEXT_CF_VCENTER);
		LISTVIEW_AddColumn(hItem, 300,  "     板卡类型" , TEXT_CF_LEFT|TEXT_CF_VCENTER);
		LISTVIEW_AddColumn(hItem, 85, "插槽", GUI_TA_CENTER|TEXT_CF_VCENTER);
		LISTVIEW_AddColumn(hItem, 300,  "     板卡类型" , TEXT_CF_LEFT|TEXT_CF_VCENTER);
		LISTVIEW_SetRowHeight(hItem, 55);
		LISTVIEW_SetGridVis(hItem,1);
		SCROLLBAR_CreateAttached(hItem, SCROLLBAR_CF_VERTICAL);
		data[0]=0;
		CAN_SEND_APP29(pMsg,1);
		hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_APP0);  
		WM_SetFocus (hItem);			
		timecnt = 0x55;
		break;
	case MSG_SetUP:
		GUI_SendKeyMsg(GUI_KEY_UP, 1);
		break;
	case MSG_SetDOWM:
		GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
		 break;
	case MSG_SetInc:	
		GUI_SendKeyMsg(GUI_KEY_PGUP, 1);
 		break;
	case MSG_SetDec:	
      	GUI_SendKeyMsg(GUI_KEY_PGDOWN, 1);			
		break;	 
	case MSG_SetLEFT:		
		if(WM_IsWindow(hWinSetapp))
		{	
			if(s_ucSelIconIndex2>0)s_ucSelIconIndex2--;GUI_EndDialog(pMsg->hWin, 0);APP_Setup(s_ucSelIconIndex2+20);//进入到设置APP窗口
		}	
 		break;
//	  case MSG_SetRIGHT:			
//			if(WM_IsWindow(hWinSetapp))
//			{	
//				if(s_ucSelIconIndex2<Devsta_num)s_ucSelIconIndex2++;GUI_EndDialog(pMsg->hWin, 0);APP_Setup(s_ucSelIconIndex2+20);//进入到设置APP窗口
//			}	
//			break;
	case WM_TIMER:
		if(timecnt!=0x55)//定时器启动早于初始化
			{
				WM_RestartTimer(pMsg->Data.v, 20);	//定时器没有初始化完
				break; 
			}	
		if(data[0]==0)
			{
				REC_Handler_APP29(pMsg); 	
				WM_RestartTimer(pMsg->Data.v, 10);	
			}
		else
			{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_APP0);  
				for (i = 0; i <10; i++) 
					{
						
						LISTVIEW_AddRow(hItem, 0);				
					}		
		
				for(i=0;i<15;i++)
					{	
						if((data[i]&0x10)==0x10)//是普通型装置板卡
							temp=data[i]&0x0f;		
						else
							temp=0x0;			//是A型(监测板卡)，不支持
						sprintf( (char*)(tbuf1 ),"插槽%02d",i+1);
						tbuf1[9]=0;				//要截断，避免溢出
						memcpy((char*)tbuf2,(char *)&(SLOT_P[temp][0]),16);
						temp=i/2;
						if(i%2==0)
							{
								LISTVIEW_SetItemText(hItem,0,temp,tbuf1);
								LISTVIEW_SetItemText(hItem,1,temp,tbuf2);	
							}
						else
							{
								LISTVIEW_SetItemText(hItem,2,temp,tbuf1);
								LISTVIEW_SetItemText(hItem,3,temp,tbuf2);	
							}
										
					}
				}		
			break;			
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinSetapp))
			{	
				 GUI_EndDialog(pMsg->hWin, 0);MenuSta();//返回到状态菜单窗口
			}
			break;	
    default:
      WM_DefaultProc(pMsg);
	
	}

}

