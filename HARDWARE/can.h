//////////////////////////////////////////////////////////////////////////////////	 
//创建日期:2016/5/26
//版本：V1.0
//ZX						  
////////////////////////////////////////////////////////////////////////////////// 
#ifndef __CAN_H
#define __CAN_H	 
#include "stm32f4xx.h"    
#define CAN_DEBUG 1
/***CAN1_BUF数组大小***/
#define CAN1_RX0_LEN				400		//
#define CAN1_RX1_LEN				600		//
#define DEBUG_CANRECIVE_ 0
/***CAN1_R0_BUF数据地址偏移量***/

#define T0_OFFSET	  8 	
#define T1_OFFSET	  40 	
#define T2_OFFSET	  80 	
#define T3_OFFSET	  160 	
#define T4_OFFSET	  184	
#define T5_OFFSET  	248
#define T6_OFFSET  	280

#define TIME_STA_OFFSET	T0_OFFSET	  		//8
#define BDS_STA_OFFSET	T1_OFFSET	 			//40
#define GPS_STA_OFFSET	T2_OFFSET	  		//80
#define B1B2_STA_OFFSET	T3_OFFSET	 			//160
#define P1_STA_OFFSET		T4_OFFSET	  		//184
#define P2_STA_OFFSET		216	  //216
#define DWFQ_STA_OFFSET	T5_OFFSET




/***CAN1_R1_BUF数据地址偏移量***/
#define R0_OFFSET	  0 
#define SETSYSMODE_OFFSET	  8 	
#define S2_OFFSET	  16 	
#define S3_OFFSET	  24 	
#define SETDlY_OFFSET	  32 	
#define S5_OFFSET	  72	
#define S6_OFFSET  	112
#define SETPPX_OFFSET  	120
#define SETAM_OFFSET	  128 	
#define SETENOUT_OFFSET	  136	
#define SETBD3D_OFFSET  144
#define SETSLOT_OFFSET  168
#define S12_OFFSET  184
#define LOG_OFFSET  200
#define SN_OFFSET  336
#define CONSOLE_OFFSET  464
#define MONIT_OFFSET  512
extern u8  CAN1_RX0_BUF[CAN1_RX0_LEN]; 	//接收缓冲 
extern u32 CAN1_RX0_STA;         				//接收状态标记	
extern u8  CAN1_RX1_BUF[CAN1_RX1_LEN]; 	//接收缓冲 
extern u32 CAN1_RX1_STA;         				//接收状态标记	

u8 CAN1_Mode_Init(void);
u8 CAN1_Send_Msg(u8* msg,u32 ext_id,u8 len);	
u8 CAN1_Send_REQ(u32 ext_id);
void CAN_SHOW_ERR(u8 can_err);
//u8 CAN1_RX1_Receive_Msg(u8 *buf);							
#endif


















