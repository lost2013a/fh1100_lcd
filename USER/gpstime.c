#include "gpstime.h"
#include "includes.h"
#include "sys.h"

__DEV_HEAD	DevHead;
__RUNSYS    DevSys;


/**************
在线程AppTaskMsgCAN0中的任务
1.CAN_Head_Handler
***************/

uint8_t CAN_Head_Handler(void)
{
	static u16 i=0;
	u8 recquality;
	__DEV_HEAD	RecHead;

	recquality	=0;
	RecHead.devfalt =		CAN1_RX0_BUF[T0_OFFSET+1];
	RecHead.devalar =		CAN1_RX0_BUF[T0_OFFSET+2];
	RecHead.tb   	 =		(DEV_STA)((CAN1_RX0_BUF[T0_OFFSET]&0x08)>>3);					//同步状态	
	RecHead.synflag =		(DEV_STA)((CAN1_RX0_BUF[T0_OFFSET]&0x10)>>4);					//初始化状态	
	RecHead.bds		 =		(DEV_STA)(CAN1_RX0_BUF[T1_OFFSET+4]&0x01);				//北斗源信号状态 
	RecHead.gps		 =		(DEV_STA)(CAN1_RX0_BUF[T2_OFFSET+4]&0x01);				//GPS源信号状态
	RecHead.b1			 =	(DEV_STA)(CAN1_RX0_BUF[B1B2_STA_OFFSET+2]&0x01);	//IRIG-B1源信号状态	
	RecHead.b2			 =	(DEV_STA)(CAN1_RX0_BUF[B1B2_STA_OFFSET+5]&0x01);	//IRIG-B2源信号状态
	//	RecHead.monitor1			 =	(DEV_STA)(~((CAN1_RX0_BUF[T0_OFFSET+4]&0x10)>>5));	//mon1状态	
	//	RecHead.monitor2			 =	(DEV_STA)(~((CAN1_RX0_BUF[T0_OFFSET+4]&0x20)>>4));	//mon2源信号状态
	//南网
	RecHead.monitor1			 =	(DEV_STA)(1);	//
	RecHead.monitor2			 =	(DEV_STA)(0);	//电源绿灯
	RecHead.ptp1		       =	(DEV_STA) (CAN1_RX0_BUF[T4_OFFSET+2]&0x01);						//信号状态	
	RecHead.ptp2		       =	(DEV_STA) (CAN1_RX0_BUF[T4_OFFSET+32+2]&0x01);						//信号状态	

	RecHead.ptp1_sta_synch =	(DEV_STA)((CAN1_RX0_BUF[T4_OFFSET+2]&0x02)>>1);				//同步状态	
	RecHead.ptp2_sta_synch =	(DEV_STA)((CAN1_RX0_BUF[T4_OFFSET+32+2]&0x02)>>1);				//同步状态	

//			RecHead.quality					=		0;																					//收到的报文质量
	RecHead.is_master 			= 	CAN1_RX0_BUF[T0_OFFSET+7];									//主，从
	RecHead.selsouce				= 	CAN1_RX0_BUF[T0_OFFSET+4]&0x0f; 						//时源选择结果	  
	RecHead.bds_sta_synch		=		(CAN1_RX0_BUF[T1_OFFSET+4]&0x02)>>1;				//北斗源同步状态
	RecHead.bds_num_capture	=		(CAN1_RX0_BUF[T1_OFFSET+2]&0xf0)>>4;				//北斗源卫星捕获数量  
	RecHead.gps_sta_synch		=		(CAN1_RX0_BUF[T2_OFFSET+4]&0x02)>>1;				//GPS源同步状态
	RecHead.gps_num_capture	=		(CAN1_RX0_BUF[T2_OFFSET+2]&0xf0)>>4;				//GPS源卫星捕获数量 
	RecHead.b1_sta_synch		=		(CAN1_RX0_BUF[T3_OFFSET+2]&0x02)>>1;				//IRIG-B1源同步状态
	RecHead.b1_qual					=		CAN1_RX0_BUF[T3_OFFSET+1]&0x0f;							//IRIG-B1源质量位
	RecHead.b2_sta_synch		=		(CAN1_RX0_BUF[T3_OFFSET+5]&0x02)>>1;				//IRIG-B2源同步状态
	RecHead.b2_qual					=		CAN1_RX0_BUF[T3_OFFSET+4]&0x0f;							//IRIG-B1源质量位
	RecHead.self_qual				=		(CAN1_RX0_BUF[T0_OFFSET+3]&0x0f);
	//RecHead.self_qual				=   (RecHead.self_qual/10)*7+RecHead.self_qual+48;

	RecHead.year						=		(CAN1_RX0_BUF[T0_OFFSET+9]<<8)|CAN1_RX0_BUF[T0_OFFSET+9+1];  	//本地时间->年
	RecHead.month						=		CAN1_RX0_BUF[T0_OFFSET+9+2];								//本地时间->月
	RecHead.day							=		CAN1_RX0_BUF[T0_OFFSET+9+3];								//本地时间->日
	RecHead.h 							=		CAN1_RX0_BUF[T0_OFFSET+9+4];								//本地时间->时
	RecHead.m								=		CAN1_RX0_BUF[T0_OFFSET+9+5];								//本地时间->分
	RecHead.s								=		CAN1_RX0_BUF[T0_OFFSET+9+6];								//本地时间->秒
	
	if((RecHead.h>24)||(RecHead.m>60)||(RecHead.s>60))
	{
		recquality=1;//不正确
	}
	if(recquality==0)		//是正确的报文头
	{
		DevHead=RecHead;			//装置状态=收到的状态，此时装置时间质量最高（0）	
		DevSys.quality=0;
		DevSys.first_recv_Can0 =1;
		#ifdef PPSTICK__RTC		
		if((i>5)&&(DevHead.s%30!=15)){
			TIM_Cmd(TIM3, DISABLE);
			TIM_Cmd(TIM3, ENABLE);
		}
		else{			//初始化前，和稳定后15S和45S设置1次
			RTC_Set_Time(DevHead.h,DevHead.m,DevHead.s,RTC_H12_AM);			//设置RTC时间和日期
			RTC_Set_Date(DevHead.year%100,DevHead.month,DevHead.day,1);	
			TIM_Cmd(TIM3, DISABLE);
			TIM_Cmd(TIM3, ENABLE);
			if(i<=5)	
				i++;
		}
		#endif
		CAN1_RX0_STA = 0;
		return 1;	
	}
	CAN1_RX0_STA = 0;
	return 0;
}	


/**************
在线程AppTaskPPSTICK中的任务
1.DEV_PPS_RECV
2.DEV_Time_Handler
***************/
void Show_Dev_LEDs(void)
{
	if(DevHead.devfalt==0) 		LED2=1; else LED2=0;
	if(DevHead.devalar==0) 		LED3=1; else LED3=0;	
	if(DevHead.bds 	==STA_OK)	LED6=0; else LED6=1;											
	if(DevHead.gps 	==STA_OK)	LED7=0; else LED7=1;
	if(DevHead.b1  	==STA_OK) 	LED8=0; else LED8=1;
	if(DevHead.b2	==STA_OK)	LED9=0; else LED9=1;	
	if(DevHead.tb	==STA_OK)	LED4=0; else LED4=1;
 	if(DevHead.monitor1	 ==STA_OK) 	LED10=0; else LED10=1;	
	if(DevHead.monitor2	 ==STA_OK) 	LED11=0; else LED11=1;	
	if((DevHead.ptp1 ==STA_OK)||(DevHead.ptp2 ==STA_OK)) LED12=0; else LED12=1;	
	if(DevHead.tb	 ==STA_OK){		
		DevSys.PPS_Ready_Out = 1;	//标志PPS开始输出
		LED1=0;						//具备输出能力后，运行灯亮
	}
}	

void DEV_PPS_RECV(void)
{				

	if(DevSys.quality<CPU_FALT_TIME){
		DevSys.quality++;			//报文质量每秒+1
	}
	else{
		DevHead.devfalt|=0x08;		//CPU通讯超时了,报CPU核心板故障
	}
	Show_Dev_LEDs();				//状态灯的控制	
	if(DevSys.PPS_Ready_Out == 1){	//PPS的秒闪烁
		LED5;
	}
}

//处理装置的时间显示
void DEV_Time_Handler(uint8_t tick)
{
	if((DevSys.quality>tick)&&(DevHead.tb==STA_OK))			//报文质量>n(上n次没有收到CAN).并且已经同步
	{
		RTC_TimeTypeDef RTC_TimeStruct;
		RTC_DateTypeDef RTC_DateStruct;
		RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);		//读取RTC时间和日期
		RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
		DevHead.year		=	RTC_DateStruct.RTC_Year+2000;		
		DevHead.month	=	RTC_DateStruct.RTC_Month;
		DevHead.day		=	RTC_DateStruct.RTC_Date;
		DevHead.h			=	RTC_TimeStruct.RTC_Hours;
		DevHead.m			=	RTC_TimeStruct.RTC_Minutes;
		DevHead.s			=	RTC_TimeStruct.RTC_Seconds;
	}
}

void Clear_CAN1(void)
{
	CAN1_RX1_STA=0;					
	DevSys.CAN1_Reqcnt=0;	//调用Clear_CAN1()来清此参数
	DevSys.CAN1_Rec1st=0;	//调用Clear_CAN1()来清此参数
}



