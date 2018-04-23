//////////////////////////////////////////////////////////////////////////////////	 
//创建日期:2016/4/13
//版本：V1.0
//zx							  
////////////////////////////////////////////////////////////////////////////////// 	

#ifndef __LED_H
#define __LED_H
#include "sys.h"
//LED端口定义
#define LED1  PCout(15)		// 	1运行
#define LED2  PCout(14)		// 	2故障
#define LED3  PCout(13)		// 	3告警
#define LED4  PEout(6)		// 	4同步
#define LED5  TIM_Cmd(TIM9, ENABLE)
#define LED6  PEout(4)		// 	6北斗
#define LED7  PEout(3)		// 	7GPS
#define LED8  PEout(2)		// 	8IRIG-B1
#define LED9  PBout(5)		// 	9IRIG-B2
#define LED10 PEout(1)		// 10_1
#define LED11 PEout(0)		// 10_2
#define LED12 PBout(9)		// 11PTP
void LED_Init(void);//初始化		 			
#endif
