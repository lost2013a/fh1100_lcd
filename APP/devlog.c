#include "mainmenu.h"
#include "sys.h"


#define WIAT_OUT   30

extern u8 LOG_POINTRES[100][40];
extern u8 LOG_POINT[100][40];



extern GUI_CONST_STORAGE GUI_BITMAP bmpic1;

extern void PAINTWIN_SETAPP(WM_MESSAGE * pMsg);

static uint8_t logbuff[3200] ;
	
void CAN_SEND_APP13(uint8_t req) 			
{
	uint32_t send_id=0x03101900;			//CANID
	uint8_t  can_err;
	send_id|=(req);
	can_err=CAN1_Send_REQ(send_id);
	CAN_SHOW_ERR(can_err);
}
	
static uint8_t REC_Handler_APP13(uint8_t reqindex) 			
{
	
	uint16_t mask=0x1019; 			 //2.->>CAN_BUF报文类型
	uint16_t FRAME_REV;
	int i ,t=0;
	CAN_SEND_APP13(reqindex+1);
	myprintf("日志请求:%d\n",reqindex);
	while(t<WIAT_OUT)
	{
		if ((CAN1_RX1_STA&0xffff0000)>0)  							//CAN1_R1状态标志
			{
				if(mask==((CAN1_RX1_STA&0xffff0000)>>16))	//收到的报文是对应槽，对应网口，对应类型的报文
					{	
						FRAME_REV=(CAN1_RX1_STA&0xffff)>>4;		//收到日志条数=(字节数/16)
						for(i=0;i<FRAME_REV;i++)
						{
							memcpy(&logbuff[(reqindex+(FRAME_REV-1-i))*10],&CAN1_RX1_BUF[LOG_OFFSET+i*16],10);
						}
						//memcpy(void *dest, const void *src, size_t n);
						//myprintf("CAN_REC 收到日志:%d条\n",FRAME_REV);
						
						CAN1_RX1_STA=0;
						myprintf("t:%d\n",t);
						return FRAME_REV;
					}	
				CAN1_RX1_STA=0;
			}
		GUI_Delay(1);
		t++;
  }
	myprintf("日志请求:%d请求超时\n",reqindex);
	return 0;
}





 void _cbCallback13(WM_MESSAGE * pMsg) 
{
  WM_HWIN hDlg;
  WM_HWIN hItem;
	HEADER_Handle hHeader;
	int i;
	char text3buf[30]={0};
  static uint16_t  logitems;
  static uint16_t reqindex,remain,fold,reced,temp,value;
	static uint16_t wrong_timeout,timecnt = 0;
	static char tbuf1[4] ;//要包含字符串结束符号
  static char tbuf2[20] ;
	static char tbuf3[3] ;
	static char tbuf4[40] ;
	static uint16_t TBUFF_LENTH = 40;
 
  
  
	
  hDlg = pMsg->hWin;

   
		  
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
      break;
  case WM_INIT_DIALOG:
			PAINTWIN_SETAPP(pMsg);//布置标题栏
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP03);
			TEXT_SetTextColor(hItem, GUI_WHITE);
			//TEXT_SetFont(hItem, &GUI_FontHZ32);
			//TEXT_SetTextAlign(hItem,   GUI_TA_VCENTER);
			TEXT_SetFont(hItem, &GUI_FontHZ16);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_APP0);  
			hHeader = LISTVIEW_GetHeader(hItem);
			HEADER_SetFont(hHeader, &GUI_FontHZ16);
			logitems = CAN1_RX0_BUF[T0_OFFSET+6];
			if(DevSys.quality>5)
				logitems=0;
			if(logitems>200){
				logitems=200;
			}		
			reced = 0;
			wrong_timeout = 0;
			timecnt = 0x55;
			myprintf("日志条数:%d\n",logitems);
			//LISTVIEW_AddColumn(hItem, 1, "", GUI_TA_CENTER|TEXT_CF_VCENTER);//解决不了初始化问题，加的一个空数据列
			LISTVIEW_AddColumn(hItem, 70, "序号", GUI_TA_CENTER|TEXT_CF_VCENTER);
			LISTVIEW_AddColumn(hItem, 170, "时间", GUI_TA_CENTER|TEXT_CF_VCENTER);
			LISTVIEW_AddColumn(hItem, 60, "类型", GUI_TA_CENTER|TEXT_CF_VCENTER);
			LISTVIEW_AddColumn(hItem, 500,  "        日志内容" , TEXT_CF_LEFT|TEXT_CF_VCENTER);
			LISTVIEW_SetRowHeight(hItem, 55);
			LISTVIEW_SetGridVis(hItem,1);
			SCROLLBAR_CreateAttached(hItem, SCROLLBAR_CF_VERTICAL);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_APP0);		
			PROGBAR_SetMinMax(hItem,0,200);		
			PROGBAR_SetFont(hItem, &GUI_Font8x16);	
			PROGBAR_SetBarColor(hItem, 0, GUI_GREEN);
			// PROGBAR_SetBarColor(hItem, 1, GUI_RED);
			//reqindex = 0;
      break;
		case WM_PRE_PAINT:	
			hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_APP0);  
			WM_SetFocus (hItem);
		  //WM_RestartTimer(pMsg->Data.v, 10);	//重启定时器，注意！
			break;
		 /************************************************************
				按键消息常规处理
				*/	
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
		case MSG_SetQIUT:
			if(WM_IsWindow(hWinSetapp)){	
				 timecnt = 0;GUI_EndDialog(hDlg, 0);Menuapp();//返回到主菜单窗口
			}
			break;	
		/************************************************************
			设置功能的实现
			*/	
		case WM_TIMER:
		   /*  ===================================
					 =========0日志的情况============================*/
		
		    if(timecnt!=0x55)//定时器启动早于初始化
				{
					myprintf("定时器没有初始化完\n");
					WM_RestartTimer(pMsg->Data.v, 50);	//重启定时器，注意！
					break; 
				}
				if(logitems<1){
						hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP03);
            TEXT_SetTextAlign(hItem,TEXT_CF_RIGHT);						
						TEXT_SetText(hItem,"暂无日志" );
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_APP0  ));
						myprintf("暂时没有日志\n");
				}
				else{
					  WM_HWIN PRODlg;
				/*  ===================================
					 =========第一次查询，和查询丢失的情况============================*/
					  myprintf("日志查询(%d/%d)\n",reced,logitems);
					  //TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP03),"读取日志中..." );
						if((reced<logitems)&&(wrong_timeout<1))
						{
							/*  
					      ==第一次查询，有丢失，重启定时器，再次查询==*/
						
							
							fold   =logitems/8; //每次8条，次数
							remain =logitems%8; //每次8条，余数
							myprintf("logitems = %d,fold = %d,remain = %d\n",logitems,fold,remain);
								//请求日志
							reced    = 0;//收到的条数
							reqindex = 0;//查询的指针
							CAN1_RX1_STA=0;
							PRODlg = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_APP0  );
							for (i = 0; i <fold; i++) 
								{
									reced+=REC_Handler_APP13(reqindex);
									reqindex+=8;
									PROGBAR_SetValue(PRODlg, reced);
									myprintf("日志收到:%d\n",reced);
									if(reced==0)//异常，放弃查询任务
										i=fold;
								}	
							if(remain>0)//再查询余下的条
								{
									reced+=REC_Handler_APP13(reqindex);
									PROGBAR_SetValue(PRODlg, reced);
									myprintf("reced = %d\n",reced);
								}
							myprintf("日志接收完成，共收到:%d/%d条\n",reced,logitems);
								if(reced<logitems)
								{
							    wrong_timeout++;
									sprintf((char*)text3buf,"读取失败:%d/%d,第%d次重试",reced,logitems,wrong_timeout);
									TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP03),text3buf );
									WM_RestartTimer(pMsg->Data.v, 10);	//重启定时器，注意！
								}
								else
								{
									wrong_timeout = 0;
									WM_RestartTimer(pMsg->Data.v, 10);	//重启定时器，注意！
								}
						 }
						else
						{
							hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_APP0);  
							timecnt = 0;
								/*  
					      ==接收正常，开始加载日志==*/
							memset(text3buf,0,30);
							if(wrong_timeout<1)
								{
									sprintf((char*)text3buf,"日志读取成功,共%d条。",logitems);
									TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP03),text3buf );
									WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_APP0  ));
								}
							else
								{
									TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP03),"日志读取失败:" );
								}
							for (i = 0; i <reced; i++) 
									{
										
										LISTVIEW_AddRow(hItem, 0);				
									}		
								//加载日志
							for (i = 0; i <reced; i++) 
								{  
								 
									int k;
									k = reced-1-i;
								 sprintf((char*)tbuf1,"%03d",logbuff[k*10]);
								 sprintf((char*)tbuf2,"%04d-%02d-%02d\n%02d:%02d:%02d",(logbuff[k*10+1]<<8)+logbuff[k*10+2],logbuff[k*10+3],logbuff[k*10+4],
								 logbuff[k*10+5],logbuff[k*10+6],logbuff[k*10+7]);
								 sprintf((char*)tbuf3,"%d",logbuff[k*10+8]);
								 temp  = logbuff[k*10+8];
								 value = logbuff[k*10+9]; 
						      switch(temp){
										case 7:
										case 8:	
											memcpy((char*)tbuf4,(char *)&(LOG_POINTRES[temp][0]),TBUFF_LENTH);//7,8北斗(GPS)天线状态:
												if(value == 1)
													strcat((char*)tbuf4,"异常");
												else if(value == 3)
													strcat((char*)tbuf4,"短路");
												else if(value == 4)
													strcat((char*)tbuf4,"开路");
												else
													strcat((char*)tbuf4,"恢复");
											break;
										case 38:
											memcpy((char*)tbuf4,(char *)&(LOG_POINTRES[temp][0]),TBUFF_LENTH);//38,时间源选择结果:
												switch(value){
													case 1:
														strcat((char*)tbuf4,"GPS");
														break;
													case 2: case 5:
														strcat((char*)tbuf4,"IRIG-B1");
														break;
													case 3: case 6:
														strcat((char*)tbuf4,"IRIG-B2");
														break;
													case 4:
														strcat((char*)tbuf4,"自守时");
														break;
													case 7:
														strcat((char*)tbuf4,"PTP1");
														break;
													case 8:
														strcat((char*)tbuf4,"PTP2");
														break;
													default:
														strcat((char*)tbuf4,"北斗");
														break;
												}
												break;
										case 39: case 40:
										case 43: case 44:
										case 45: case 46:	
											memcpy((char*)tbuf4,(char *)&(LOG_POINTRES[temp][0]),TBUFF_LENTH);//39-46,源优先级:		
											switch(value){
													case 1:
														strcat((char*)tbuf4,"1");
														break;
													case 2: 
														strcat((char*)tbuf4,"2");
														break;
													case 3: 
														strcat((char*)tbuf4,"3");
														break;
													case 4:
														strcat((char*)tbuf4,"4");
														break;
													case 5:
														strcat((char*)tbuf4,"5");
														break;
													case 6:
														strcat((char*)tbuf4,"6");
														break;
													default:
														strcat((char*)tbuf4,"默认");
														break;
											}
											break;
										default://不需要特殊处理的
											if(value == 0)
												memcpy((char*)tbuf4,(char *)&(LOG_POINTRES[temp][0]),TBUFF_LENTH);
											else if(value == 1)
												memcpy((char*)tbuf4,(char *)&(LOG_POINT[temp][0]),TBUFF_LENTH);
											break;							 
								 }
								 
								 
								 
								 LISTVIEW_SetItemText(hItem,0,i,tbuf1);
								 LISTVIEW_SetItemText(hItem,1,i,tbuf2);
								 LISTVIEW_SetItemText(hItem,2,i,tbuf3);
								 LISTVIEW_SetItemText(hItem,3,i,tbuf4);
								}		//end LISTVIEW初始化
							LISTVIEW_SetSel(hItem,0);
						}			//end 日志加载内容完成			
		    }	//end 日志记录初始化完成			
			break;		
    case WM_NOTIFY_PARENT:
      break;
    default:
      WM_DefaultProc(pMsg);
  }
	
}




/*************************** End of file ****************************/
