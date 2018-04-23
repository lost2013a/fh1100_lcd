
#include "mainmenu.h"
#include "sys.h"
extern GUI_CONST_STORAGE GUI_BITMAP bmleft;
extern GUI_CONST_STORAGE GUI_BITMAP bmright;
extern uint8_t	s_ucSelIconIndex2;
static  uint8_t	showline;

static void Showsta(WM_MESSAGE * pMsg)
{
	int i =0;
	char tbuf[20]={0};
	DEV_STA 	osc_tame_sta,ox_synch_sta;
	u16 	offset1=T0_OFFSET+16;   

	u32 	osc_frequency		=		(CAN1_RX0_BUF[offset1+1]<<24)+(CAN1_RX0_BUF[offset1+2]<<16)+(CAN1_RX0_BUF[offset1+3]<<8)
													+(CAN1_RX0_BUF[offset1+4]);					//频率值
	u32 	osc_adjust			=		(CAN1_RX0_BUF[offset1+5]<<16)+(CAN1_RX0_BUF[offset1+6]<<8)
													+(CAN1_RX0_BUF[offset1+7]);					//频率估算
	u16 	offset2=T0_OFFSET+16; 
	u8  	ox_synch_on   	=   (CAN1_RX0_BUF[offset2]&0x01);				//原子钟锁定状态

	osc_tame_sta				=		(DEV_STA)	((CAN1_RX0_BUF[T0_OFFSET]&0x20)>>5);				//频率源驯服状态	
	ox_synch_sta				=		(DEV_STA)(CAN1_RX0_BUF[offset2]&0x02);				//原子钟锁定状态
	if(osc_frequency>99999999)   	//超出范围
		osc_frequency=99999999;
	if(osc_frequency>99999999)			//超出范围
		osc_adjust=99999999;

	if(osc_tame_sta==STA_OK)
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"驯服" );
	else
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"未驯服" ); 
	if( showline==4)
	{
		sprintf( tbuf , "%04dHz\n",osc_frequency); 
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf );//频率
		memset(tbuf,0,20); 
		sprintf( tbuf,"%04dHz\n",osc_adjust);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf );//频率估算
		if((ox_synch_on==0)&&(ox_synch_sta==STA_OK))
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"已锁定" );
		else
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"未锁定" );	
	}
}





void _cbCallback21(WM_MESSAGE * pMsg) 
{

  WM_HWIN hItem;
	int i ;
  switch (pMsg->MsgId) {
    case WM_PAINT:
			//PAINTTITL_STAAPP(4);/*画标题栏和窗口*/
		

		
		/*画标题栏*/
	GUI_SetColor(GUI_COL_TITLE1);
	GUI_FillRect(0, 0, 800, 36 );
	GUI_SetPenSize(1);
	GUI_SetColor(GUI_WHITE);
	GUI_DrawLine(250,8,250,34);
	GUI_DrawLine(0,37,800,37);
	GUI_SetColor(GUI_BLACK);
	GUI_DrawLine(0,38,800,38);
	/*画正文框*/
	GUI_SetColor(GUI_WHITE);
	GUI_FillRoundedRect(20,70,700,440,8);
	/*画装饰的线条*/
	GUI_SetColor(GUI_COL_WIN1);
	GUI_SetPenSize(3);
	GUI_DrawLine(80,116,600,116);
	GUI_SetPenSize(1);
	for (i = 1; i < showline+1; i++) 
	 {
		GUI_DrawLine(80,116+LYGAPS*i,600,116+LYGAPS*i);
	 }
		  GUI_DrawLine(LXPOS0-20,YPOS0-5,LXPOS0-20,YPOS0+LYGAPS*showline);
		  GUI_DrawBitmap(&bmleft,30,480-46);
		  GUI_DrawBitmap(&bmright,680-48,480-46);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);
		  if( DevSys.USER_Right == 1 )
				showline =4;
			else
				showline =1;
      for(i=0;i<showline;i++)
			 {
			     hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP03 + i );
				   TEXT_SetFont(hItem, &GUI_FontHZ32);
				   hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10 + i );
				   TEXT_SetFont(hItem, &GUI_FontHZ32);
			 }
			 
			 if(showline==4)
			 {
				 TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04),"频率值:" );
				 TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP05), "频率估算:" );
				 TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP06), "原子钟状态:" );
			 }
			Showsta(pMsg); 
      break;
			 
			case MSG_SetLEFT:		
			if(WM_IsWindow(hWinSetapp))
			{	
				if(s_ucSelIconIndex2>0)s_ucSelIconIndex2--;GUI_EndDialog(pMsg->hWin, 0);APP_Setup(s_ucSelIconIndex2+20);//进入到设置APP窗口
			}	
	 		break;
	  case MSG_SetRIGHT:			
			if(WM_IsWindow(hWinSetapp))
			{	
				if(s_ucSelIconIndex2<Devsta_num)s_ucSelIconIndex2++;GUI_EndDialog(pMsg->hWin, 0);APP_Setup(s_ucSelIconIndex2+20);//进入到设置APP窗口
			}	
			break;
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinSetapp))
			{	
				 GUI_EndDialog(pMsg->hWin, 0);MenuSta();//返回到状态菜单窗口
			}
			break;	
		case MSG_PPSTICK:
			Showsta(pMsg);
		 break;
    default:
      WM_DefaultProc(pMsg);
  }
	
}



