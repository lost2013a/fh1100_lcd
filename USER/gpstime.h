#ifndef __GPSTIME_H
#define __GPSTIME_H 

#include <sys.h>	  

extern struct __DEV 			DevHead;
extern struct _RUNSYS     DevSys;
#define ADMIN_PASS   1100

typedef enum								//状态正常异常枚举
{ 
	STA_OK  		= 0x00, 	//状态正常	
	STA_ERR 		= 0x01		//状态异常	
}DEV_STA;



typedef struct __DEV
{
	DEV_STA	 tb;				//同步标志
	DEV_STA	 synflag;			//初始化标志
	DEV_STA  bds;				//BDS标志
	DEV_STA  gps;				//GPS标志
	DEV_STA  b1;				//B1标志
	DEV_STA  b2;				//B2标志
	DEV_STA  monitor1;			//monitor1标志	
	DEV_STA  monitor2;			//monitor2标志
	DEV_STA  ptp1;				//PTP1标志	
	DEV_STA  ptp2;				//PTP1标志	
	
	uint8_t  devfalt;			//装置故障
	uint8_t  devalar;			//装置告警
	//uint8_t	 cpufalt;		//CPU核心板异常，已删除，避免拷贝赋值时候值不确定
	
	uint8_t  is_master;	 		//主，从
	uint8_t  selsouce;			//当前基准源	
	uint8_t  bds_sta_synch;		//北斗源同步状态
	uint8_t  bds_num_capture;	//北斗源卫星捕获数量  
	uint8_t  gps_sta_synch;		//GPS源同步状态
	uint8_t  gps_num_capture;	//GPS源卫星捕获数量
	uint8_t  b1_sta_synch;		//IRIG-B1源同步状态
	uint8_t  b1_qual;			//IRIG-B1源质量位
	uint8_t  b2_sta_synch;		//IRIG-B2源同步状态
	uint8_t  b2_qual;			//IRIG-B2源质量位
	uint8_t  ptp1_sta_synch;	//IRIG-B1源同步状态
	uint8_t  ptp2_sta_synch;	//IRIG-B1源同步状态
	uint8_t  self_qual;			//自守时质量位
	uint16_t year;				//本地时间->年
	uint8_t	 month;				//本地时间->月
	uint8_t	 day;				//本地时间->日
	uint8_t	 h; 				//本地时间->时
	uint8_t	 m;					//本地时间->分
	uint8_t	 s;					//本地时间->秒
}__DEV_HEAD;


typedef struct _RUNSYS
{
	uint8_t   PPS_Ready_Out;		//PPS开始输出的标志，由自己判断
	uint8_t	  PPS_Tick;					//秒脉冲上升沿标志(来着RTC时钟每秒唤醒)
	uint8_t	  PPS_Tick_300;	
	uint16_t  quality;					//接受报文时间质量
	DEV_STA   idle_out_flag;		//智能背光
	uint16_t  idle_out;					//闲置没有操作的时间(智能背光操作)
	uint16_t  wrongpass_cnt;		//密码错误的计数
	uint16_t  wrongpass_waiting;//密码错误后的等待时间
	uint16_t  can0_err;         //CAN0错误次数,超过一定值后重启系统
	uint8_t   can1_err;         //CAN1错误状态,用于显示CAN1错误
	uint8_t   first_recv_Can0;  //完成初始化界面Open
	uint8_t   USER_Right;       //用户登陆后的权限
	uint32_t  USER_PASS;				//用户登陆密码
	uint8_t   Login_cnt;        //用户登陆生产日志的一个计数，（对大1）
	uint8_t   LCD_Protect_ON;		//屏幕保护开关
	uint8_t   CAN1_Rec1st;		  //配置参数收到回复
	uint8_t   CAN1_Reqcnt;		  //配置参数请求次数
	
}__RUNSYS;


void Dev_Period_Handler(void);
void DEV_PPS_RECV(void);

uint8_t CAN_Head_Handler(void);
void DEV_Time_Handler(uint8_t tick);

void Clear_CAN1(void);

#endif



















