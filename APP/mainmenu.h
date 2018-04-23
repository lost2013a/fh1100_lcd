#ifndef __Mainmenu_H
#define __Mainmenu_H

#include "GUI.h"
#include "DIALOG.h"
#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"
#include "GRAPH.h"
#include "MENU.h"
#include "MULTIPAGE.h"
#include "ICONVIEW.h"
#include "TREEVIEW.h"

/*
************************************************************************
*						数据定义
************************************************************************
*/
typedef unsigned          char uint8_t;
typedef unsigned short    int  uint16_t;
typedef unsigned          int  uint32_t;

/*
************************************************************************
*						供外部文件调用
************************************************************************
*/
extern WM_HWIN  hWin_msgto ;   //指定窗口句柄用的句柄指针，主要用于传递按键消息和指定聚焦
extern WM_HWIN  hWinTop;       //存放顶层窗口的句柄
extern WM_HWIN  hWinLogin;     //用户登录
extern uint8_t  LoginRight;    //用户权限
extern WM_HWIN  hWinMainMenu;  //主菜单
extern WM_HWIN  hWinIconMenu;  //主菜单的图标
//一级菜单
extern WM_HWIN  hWinDevsta;    //装置状态
extern WM_HWIN  hWinMenuSet;   //参数设置
extern WM_HWIN  hWinIconSet;   //参数设置的图标
extern WM_HWIN  hWinMenuSta;   //状态目录
extern WM_HWIN  hWinIconSta;   //装置状态的图标
extern WM_HWIN  hWinLog;       //日志查询
extern WM_HWIN  hWinDevinfo;   //出厂信息
extern WM_HWIN  hWinOthers;    //其他设置
//二级菜单
extern WM_HWIN  hWinSetapp;    //参数设置应用的句柄
extern WM_HWIN  hWinOthersapp; //其他设置应用的句柄
extern WM_HWIN  hWinPassv;     //密码确认界面的句柄
extern uint32_t USERPASS;      //用户的密码




/*
*********************************************************************************************************
*                                       函数和变量
*********************************************************************************************************
*/

//void MainTask(void);
void Topapp(void); 
void Menuapp(void);
void MenuSet(void);
void APP_Setup(uint8_t app_num);
void Password_Verify(void);
void MenuSta(void); 
void PAINTTITL_SETAPP(uint8_t line);
void PAINTWIN_SETAPP(WM_MESSAGE * pMsg);
void PAINTTITL_STAAPP(uint8_t line);
void PAINTTITL_MSTAAPP(uint8_t line);
/*
************************************************************************
*						数据结构
************************************************************************
*/


typedef struct {
  const GUI_BITMAP * pBitmap;
  const char       * pText;
} BITMAP_ITEM;

enum	Ant_DEV_STA					//状态正常异常枚举
{ 
	Ant_OK			 	=	0x02,		//天线状态：正常
	Ant_SHORT			=	0x03,		//天线状态：短路
	Ant_OPEN 			=	0x04		//天线状态：开路
}	;

#define OCX_PERIOD  1  //晶振周期,界面不做计算取值1。

/*
************************************************************************
*						宏定义
************************************************************************
*/
#define APPITEMS 12  //设置APP的数
#define Devsta_num  9  //10个

#define XPOS_T5 150
#define XPOS_T6 170
#define XPOS_T8 210
#define XPOS0  250
#define XPOS1  300
#define XGAPS  55
#define YPOS0  120
#define YGAPS  40
#define RECTX  45
#define RECTY  26

#define LXPOS_T5 (32*5+8)
#define LXPOS_T6 (32*6+8)
#define LXPOS_T7 (32*7+8)
#define LXPOS_T8 (32*8+8)
#define LXPOS0  350
#define LXGAPS  55
#define LYPOS0  120
#define LYGAPS  55
#define LRECTX  45
#define LRECTY  26



#define GUEST_RIGHT 	0
#define ADMIN_RIGHT 	1

#define ID_WINDOW_APP0    (GUI_ID_USER + 0x20)
#define ID_WINDOW_APP1    (GUI_ID_USER + 0x21)
#define ID_WINDOW_APP2    (GUI_ID_USER + 0x22)
#define ID_WINDOW_APP3    (GUI_ID_USER + 0x23)

#define ID_TEXT_APP00      (GUI_ID_USER + 0x24)
#define ID_TEXT_APP01      (GUI_ID_USER + 0x25)
#define ID_TEXT_APP02      (GUI_ID_USER + 0x26)
#define ID_TEXT_APP03      (GUI_ID_USER + 0x27)
#define ID_TEXT_APP04      (GUI_ID_USER + 0x28)
#define ID_TEXT_APP05      (GUI_ID_USER + 0x29)
#define ID_TEXT_APP06      (GUI_ID_USER + 0x2a)
#define ID_TEXT_APP07      (GUI_ID_USER + 0x2b)
#define ID_TEXT_APP08      (GUI_ID_USER + 0x2c)
#define ID_TEXT_APP09      (GUI_ID_USER + 0x2d)


#define ID_EDIT_APP00      (GUI_ID_USER + 0x30)
#define ID_EDIT_APP01      (GUI_ID_USER + 0x31)
#define ID_EDIT_APP02      (GUI_ID_USER + 0x32)
#define ID_EDIT_APP03      (GUI_ID_USER + 0x33)
#define ID_EDIT_APP04      (GUI_ID_USER + 0x34)
#define ID_EDIT_APP05      (GUI_ID_USER + 0x35)
#define ID_EDIT_APP06      (GUI_ID_USER + 0x36)
#define ID_EDIT_APP07      (GUI_ID_USER + 0x37)
#define ID_EDIT_APP08      (GUI_ID_USER + 0x38)
#define ID_EDIT_APP09      (GUI_ID_USER + 0x39)
#define ID_EDIT_APP10      (GUI_ID_USER + 0x3a)
#define ID_EDIT_APP11      (GUI_ID_USER + 0x3b)
#define ID_EDIT_APP12      (GUI_ID_USER + 0x3c)
#define ID_EDIT_APP13      (GUI_ID_USER + 0x3d)
#define ID_EDIT_APP14      (GUI_ID_USER + 0x3e)
#define ID_EDIT_APP15      (GUI_ID_USER + 0x3f)

#define ID_DROPDOWN_APP0   (GUI_ID_USER + 0x40)
#define ID_DROPDOWN_APP1   (GUI_ID_USER + 0x41)
#define ID_DROPDOWN_APP2   (GUI_ID_USER + 0x42)
#define ID_DROPDOWN_APP3   (GUI_ID_USER + 0x43)
#define ID_DROPDOWN_APP4   (GUI_ID_USER + 0x44)
#define ID_DROPDOWN_APP5   (GUI_ID_USER + 0x45)

#define ID_LISTVIEW_APP0   (GUI_ID_USER + 0x48)

#define ID_BUTTON_APP0     (GUI_ID_USER + 0x49)
#define ID_BUTTON_APP1     (GUI_ID_USER + 0x4a)
#define ID_BUTTON_APP2     (GUI_ID_USER + 0x4b)
#define ID_BUTTON_APP3     (GUI_ID_USER + 0x4c)

#define ID_PROGBAR_APP0   (GUI_ID_USER + 0x4d)


#define ID_TEXT_APP10      (GUI_ID_USER + 0x50)
#define ID_TEXT_APP11      (GUI_ID_USER + 0x51)
#define ID_TEXT_APP12      (GUI_ID_USER + 0x52)
#define ID_TEXT_APP13      (GUI_ID_USER + 0x53)
#define ID_TEXT_APP14      (GUI_ID_USER + 0x54)
#define ID_TEXT_APP15      (GUI_ID_USER + 0x55)
#define ID_TEXT_APP16      (GUI_ID_USER + 0x56)
#define ID_TEXT_APP17      (GUI_ID_USER + 0x57)
#define ID_TEXT_APP18      (GUI_ID_USER + 0x58)
#define ID_TEXT_APP19      (GUI_ID_USER + 0x59)
#define ID_TEXT_APP20      (GUI_ID_USER + 0x5a)
#define ID_TEXT_APP21      (GUI_ID_USER + 0x5b)


#define ID_WINDOW_TOP (GUI_ID_USER + 0x70)
#define ID_TEXT_SYS   (GUI_ID_USER + 0x71)
#define ID_TEXT_SOCUE (GUI_ID_USER + 0x72)
#define ID_TEXT_DATA  (GUI_ID_USER + 0x73)
#define ID_TEXT_TIME  (GUI_ID_USER + 0x74)
#define ID_TEXT_BD    (GUI_ID_USER + 0x75)
#define ID_TEXT_GPS   (GUI_ID_USER + 0x76)
#define ID_TEXT_B1    (GUI_ID_USER + 0x77)
#define ID_TEXT_B2    (GUI_ID_USER + 0x78)
#define ID_TEXT_PTP1    (GUI_ID_USER + 0x79)
#define ID_TEXT_PTP2    (GUI_ID_USER + 0x80)

#define MSG_SetENTER      (GUI_ID_USER + 0x80)
#define MSG_SetQIUT       (GUI_ID_USER + 0x81)
#define MSG_SetInc        (GUI_ID_USER + 0x82)
#define MSG_SetDec        (GUI_ID_USER + 0x83)

#define MSG_SetUP         (GUI_ID_USER + 0x84)
#define MSG_SetDOWM       (GUI_ID_USER + 0x85)
#define MSG_SetLEFT       (GUI_ID_USER + 0x86)
#define MSG_SetRIGHT      (GUI_ID_USER + 0x87)

#define MSG_SetICONFocus  (GUI_ID_USER + 0x88)
#define MSG_PPSTICK       (GUI_ID_USER + 0x89)
#define MSG_PPSTICK_300   (GUI_ID_USER + 0x8a)
#define MSG_VerifyOK      (GUI_ID_USER + 0x8b)
#define MSG_FOUCE         (GUI_ID_USER + 0x8c)
#define MSG_CAN1REV       (GUI_ID_USER + 0x8d)

#define ID_TimerTime1    (GUI_ID_USER + 0x90)
#define ID_TimerTime2    (GUI_ID_USER + 0x91)
#define ID_TimerTime3    (GUI_ID_USER + 0x92)

//#define ID_TimerTime1    1
//#define ID_TimerTime2    2
//#define ID_TimerTime3    3



/*
*********************************************************************************************************
*                                    颜色字体
*********************************************************************************************************
*/

#define GUI_COL_TITLE1         0x007F4816
#define GUI_COL_TITLE2         0x00FFFFFF
#define GUI_COL_TITLE3         0x00000000
#define GUI_COL_TITLE4         0x00E7E7E7

//#define GUI_COL_WIN1           0x00F6F6F6
#define GUI_COL_WIN1           0x00E6E6E6
#define GUI_COL_WIN2           0x00F0F0F0

extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ32;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ24;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ16;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ12;


#endif

/*****************************(END OF FILE) *********************************/
