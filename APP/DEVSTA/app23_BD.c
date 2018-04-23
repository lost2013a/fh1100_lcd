
#include "mainmenu.h"
#include "sys.h"
extern GUI_CONST_STORAGE GUI_BITMAP bmleft;
extern GUI_CONST_STORAGE GUI_BITMAP bmright;
extern uint8_t	s_ucSelIconIndex2;
#define BUF_S_LEN 36

static void Showsta(WM_MESSAGE * pMsg)
{
	int i =0;
	char tbuf[BUF_S_LEN]={0};
	DEV_STA	 		bds_sta_synch,bds_sta_module,bds_dif_sign,bds_sta_signal,bds_LS_come,bds_LS_sign;
	u8    	bds_sta_ant;
	u8 			bds_num_capture,bds_time_valid;
	u32 		bds_dif_s,bds_dif_ns;
	
	u8 		  latitude[12],longitude[12],high[12],j,E3=0;		
	u32     I_Lat=0,I_Lon=0,I_Hig=0,D_Lat=0,D_Lon=0,D_Hig=0;
	
	

	bds_sta_signal				=		(DEV_STA)(CAN1_RX0_BUF[T1_OFFSET+4]&0x01);							//北斗源信号状态	
	bds_sta_synch					=		(DEV_STA)((CAN1_RX0_BUF[T1_OFFSET+4]&0x02)>>1);					//北斗源同步状态	
	bds_time_valid				=		(DEV_STA)((CAN1_RX0_BUF[T1_OFFSET+4]&0x04)>>2);					//北斗源时间连续状态 
	bds_sta_module				=		(DEV_STA)((CAN1_RX0_BUF[T1_OFFSET+3]&0x10)>>3);					//北斗源模块状态
	bds_sta_ant						=		CAN1_RX0_BUF[T1_OFFSET+3]&0xf;													//北斗源天线状态
	bds_num_capture				=		(CAN1_RX0_BUF[T1_OFFSET+2]&0xf0)>>4;										//北斗源卫星捕获数量
	//offset2=BDS_STA_OFFSET+32;	
	bds_dif_sign					=		(DEV_STA)((CAN1_RX0_BUF[BDS_STA_OFFSET+32]&0x80)>>7);	//北斗源PPS相对差符号
  bds_dif_s							=		((CAN1_RX0_BUF[BDS_STA_OFFSET+32]&0x7f)<<24)+(CAN1_RX0_BUF[BDS_STA_OFFSET+32+1]<<16)+(CAN1_RX0_BUF[BDS_STA_OFFSET+32+2]<<8)
														+(CAN1_RX0_BUF[BDS_STA_OFFSET+32+3]);									//北斗源PPS相对差值秒位
	bds_dif_ns						=		((CAN1_RX0_BUF[BDS_STA_OFFSET+32+4]<<24)+(CAN1_RX0_BUF[BDS_STA_OFFSET+32+5]<<16)+(CAN1_RX0_BUF[BDS_STA_OFFSET+32+6]<<8)
														+(CAN1_RX0_BUF[BDS_STA_OFFSET+32+7]))*OCX_PERIOD;									//北斗源PPS相对差值纳秒位
	bds_dif_ns =bds_dif_ns&0x7fffffff;													
	if(bds_dif_ns>999999999) bds_dif_ns=999999999;												//溢出值置999 999 999
	
	
	
	bds_LS_come						=		(DEV_STA)((CAN1_RX0_BUF[T1_OFFSET+1]&0x80)>>7);		//闰秒预告
	bds_LS_sign						=		(DEV_STA)((CAN1_RX0_BUF[T1_OFFSET+1]&0x40)>>6);		//闰秒标志
	
	
	
	for(j=0;j<6;j++)
		{
			latitude[2*j]    = (CAN1_RX0_BUF[BDS_STA_OFFSET+8+j]>>4)%10;		  
			latitude[2*j+1]  = (CAN1_RX0_BUF[BDS_STA_OFFSET+8+j]&0xf)%10;	 		
			longitude[2*j]   = (CAN1_RX0_BUF[BDS_STA_OFFSET+16+j]>>4)%10;	 
			longitude[2*j+1] = (CAN1_RX0_BUF[BDS_STA_OFFSET+16+j]&0xf)%10;		
			high[2*j]        = CAN1_RX0_BUF[BDS_STA_OFFSET+24+j]>>4;		if(high[2*j]  		 ==0xE)		E3=2*j;		else high[2*j]=high[2*j]%10;
			high[2*j+1]      = CAN1_RX0_BUF[BDS_STA_OFFSET+24+j]&0xf;		if(high[2*j+1]		 ==0xE)		E3=2*j+1;	else high[2*j+1]=high[2*j+1]%10;
		}
	
	  
	I_Lat+=latitude[0]*10;
	I_Lat+=latitude[1];
	D_Lat+=latitude[2]*100000;
	D_Lat+=latitude[3]*10000;
	D_Lat+=latitude[5]*1000;
	D_Lat+=latitude[6]*100;
	D_Lat+=latitude[7]*10;
	D_Lat+=latitude[8];

	I_Lon+=longitude[0]*100;
	I_Lon+=longitude[1]*10;
	I_Lon+=longitude[2];

	D_Lon+=longitude[3]*100000;
	D_Lon+=longitude[4]*10000;
	D_Lon+=longitude[6]*1000;
	D_Lon+=longitude[7]*100;
	D_Lon+=longitude[8]*10;
	D_Lon+=longitude[9];

	D_Lat=D_Lat/60;
	D_Lon=D_Lon/60;
	

	if((E3>0)&&(E3<6))	
		{
			for(j=0;j<E3;j++)
				I_Hig+=high[j]*LCD_Pow(10,E3-j-1);
		  D_Hig+=high[E3+1];	
		}
		

		
		
		/*-----------------------0同步状态-----------------------------------------------------------------------------*/
		if(bds_sta_synch==STA_OK)
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"同步");
		else 
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"失步");
				/*
		-------------------------1天线状态-----------------------------------------------------------------------------*/
		if(bds_sta_ant==Ant_OK)	
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"正常");
		else if(bds_sta_ant==Ant_SHORT)	
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"短路");
		else 
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"开路");
				/*
		-------------------------2模块状态-----------------------------------------------------------------------------*/
		if(bds_sta_module==STA_OK)	
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"正常");
		else 
		  TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"异常");
		
			/*
		-------------------------3卫星颗数-----------------------------------------------------------------------------*/
		//memset(tbuf,0,BUF_S_LEN);
		sprintf( (char*)(tbuf),"%02d颗",bds_num_capture);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf);
		
		/*
		-------------------------4通道差值-----------------------------------------------------------------------------*/
		memset(tbuf,0,BUF_S_LEN);
		if(bds_dif_sign==STA_OK)		
			sprintf( (char*)(tbuf),"+%d秒% 03d纳秒\n",bds_dif_s,bds_dif_ns);
		else
			sprintf( (char*)(tbuf),"-%d秒% 03d纳秒\n",bds_dif_s,bds_dif_ns);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf);
		/*
		-------------------------5信号状态-----------------------------------------------------------------------------*/
		if(bds_sta_signal==STA_OK)	
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"正常");
		else 
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"异常");
		
			/*
		-------------------------6闰秒状态-----------------------------------------------------------------------------*/
		
		if(bds_LS_come)
		{
			if(bds_LS_sign==0)
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"正闰秒");
			else
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"负闰秒");
		}
		else
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"无闰秒");
			/*
		-------------------------7时间连续性状态-----------------------------------------------------------------------------*/
		if(bds_time_valid==STA_OK)	
		 TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"正常");
		else
		 TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),"异常");
			/*
		-------------------------8经纬度-----------------------------------------------------------------------------*/
		
		 if( DevSys.USER_Right == 1 )
			{
				memset(tbuf,0,BUF_S_LEN);
				sprintf( (char*)(tbuf),"N%02d.%04d,E%03d.%04d,高%03d.%01d米",I_Lat,D_Lat,I_Lon,D_Lon,I_Hig,D_Hig);
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP04),"北斗模块定位:");	
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP10+(i++)),tbuf);
			}
}




void _cbCallback23(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) {
    case WM_PAINT:
			PAINTTITL_MSTAAPP(8);/*画标题栏和窗口*/
		  GUI_DrawLine(XPOS1-20,YPOS0-5-YGAPS,XPOS1-20,YPOS0+YGAPS*8);
		  GUI_DrawBitmap(&bmleft,30,480-46);
		  GUI_DrawBitmap(&bmright,680-48,480-46);
      break;
    case WM_INIT_DIALOG:
      PAINTWIN_SETAPP(pMsg);
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



