/*
*********************************************************************************************************
*
*	模块名称 : 按键驱动模块
*	文件名称 : bsp_key.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_KEY_H
#define __BSP_KEY_H
#include "sys.h"

///* 根据应用程序的功能重命名按键宏 */
#define KEY1_U 		KEY_1_DOWN
#define KEY2_D 		KEY_2_DOWN
#define KEY3_L		KEY_3_DOWN
#define KEY4_R 		KEY_4_DOWN
#define KEY5_E 		KEY_5_DOWN
#define KEY6_Q 		KEY_6_DOWN
#define KEY7_INC 	KEY_7_DOWN
#define KEY8_DEC 	KEY_8_DOWN

typedef enum
{
	KEY_NONE = 0,			/* 0 表示按键事件 */

	KEY_1_DOWN,				/* 1键按下 */
	KEY_1_UP,				  /* 1键弹起 */
	KEY_1_LONG,				/* 1键长按 */

	KEY_2_DOWN,				/* 2键按下 */
	KEY_2_UP,				  /* 2键弹起 */
	KEY_2_LONG,				/* 2键长按 */

	KEY_3_DOWN,				/* 3键按下 */
	KEY_3_UP,				  /* 3键弹起 */
	KEY_3_LONG,				/* 3键长按 */

	KEY_4_DOWN,				/* 4键按下 */
	KEY_4_UP,				  /* 4键弹起 */
	KEY_4_LONG,				/* 4键长按 */

	KEY_5_DOWN,				/* 5键按下 */
	KEY_5_UP,			  	/* 5键弹起 */
	KEY_5_LONG,				/* 5键长按 */

	KEY_6_DOWN,				/* 6键按下 */
	KEY_6_UP,				  /* 6键弹起 */
	KEY_6_LONG,				/* 6键长按 */

	KEY_7_DOWN,				/* 7键按下 */
	KEY_7_UP,				  /* 7键弹起 */
	KEY_7_LONG,			 	/* 7键长按 */

	KEY_8_DOWN,				/* 8键按下 */
	KEY_8_UP,				  /* 8键弹起 */
	KEY_8_LONG,				/* 8键长按 */

}KEY_ENUM;
/* 供外部调用的函数声明 */
void bsp_InitKey(void);
void bsp_KeyScan(void);
uint8_t KEY_Scan(void);
//void bsp_PutKey(uint8_t _KeyCode);
//uint8_t bsp_GetKey2(void);
//uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
//void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
//void bsp_ClearKey(void);



#endif

