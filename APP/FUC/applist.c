
#include "mainmenu.h"
#include "sys.h"

extern void _cbCallback00(WM_MESSAGE * pMsg) ;
extern void _cbCallback01(WM_MESSAGE * pMsg) ;
extern void _cbCallback02(WM_MESSAGE * pMsg) ;
extern void _cbCallback03(WM_MESSAGE * pMsg) ;
extern void _cbCallback04(WM_MESSAGE * pMsg) ;
extern void _cbCallback05(WM_MESSAGE * pMsg) ;
extern void _cbCallback06(WM_MESSAGE * pMsg) ;
extern void _cbCallback07(WM_MESSAGE * pMsg) ;
extern void _cbCallback08(WM_MESSAGE * pMsg) ;
extern void _cbCallback09(WM_MESSAGE * pMsg) ;
extern void _cbCallback10(WM_MESSAGE * pMsg) ;
extern void _cbCallback11(WM_MESSAGE * pMsg) ;
extern void _cbCallback12(WM_MESSAGE * pMsg) ;
extern void _cbCallback13(WM_MESSAGE * pMsg) ;
extern void _cbCallback14(WM_MESSAGE * pMsg) ;
extern void _cbCallback15(WM_MESSAGE * pMsg) ;
extern void _cbCallback20(WM_MESSAGE * pMsg) ;
extern void _cbCallback21(WM_MESSAGE * pMsg) ;
extern void _cbCallback22(WM_MESSAGE * pMsg) ;
extern void _cbCallback23(WM_MESSAGE * pMsg) ;
extern void _cbCallback24(WM_MESSAGE * pMsg) ;
extern void _cbCallback25(WM_MESSAGE * pMsg) ;
extern void _cbCallback26(WM_MESSAGE * pMsg) ;
extern void _cbCallback27(WM_MESSAGE * pMsg) ;
extern void _cbCallback28(WM_MESSAGE * pMsg) ;
extern void _cbCallback29(WM_MESSAGE * pMsg) ;
//函数数据结构
typedef struct 
{
	WM_CALLBACK * cb;
} APP_LIST_ITEM;



void PAINTTITL_SETAPP(uint8_t line)
{
	int i;
	/*画标题栏*/
	GUI_SetColor(GUI_COL_TITLE1);
	GUI_FillRect(0, 0, 800, 36 );
	GUI_SetPenSize(1);
	GUI_SetColor(GUI_WHITE);
	GUI_DrawLine(170,8,170,34);
	GUI_DrawLine(0,37,800,37);
	GUI_SetColor(GUI_BLACK);
	GUI_DrawLine(0,38,800,38);
	/*画正文框*/
	GUI_SetColor(GUI_WHITE);
	GUI_FillRoundedRect(20,70,700,440,8);
	/*画装饰的线条*/
	GUI_SetColor(GUI_COL_WIN1);
	GUI_SetPenSize(3);
	GUI_DrawLine(80,112,600,112);
	GUI_SetPenSize(1);
	for (i = 1; i < line+1; i++) 
	 {
		GUI_DrawLine(80,112+YGAPS*i,600,112+YGAPS*i);
	 }
}



void PAINTTITL_STAAPP(uint8_t line)
{
	int i;
	/*画标题栏*/
	GUI_SetColor(GUI_COL_TITLE1);
	GUI_FillRect(0, 0, 800, 36 );
	GUI_SetPenSize(1);
	GUI_SetColor(GUI_WHITE);
	GUI_DrawLine(170,8,170,34);
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
	for (i = 1; i < line+1; i++) 
	 {
		GUI_DrawLine(80,116+LYGAPS*i,600,116+LYGAPS*i);
	 }
}

void PAINTTITL_MSTAAPP(uint8_t line)
{
	int i;
	/*画标题栏*/
	GUI_SetColor(GUI_COL_TITLE1);
	GUI_FillRect(0, 0, 800, 36 );
	GUI_SetPenSize(1);
	GUI_SetColor(GUI_WHITE);
	GUI_DrawLine(170,8,170,34);
	GUI_DrawLine(0,37,800,37);
	GUI_SetColor(GUI_BLACK);
	GUI_DrawLine(0,38,800,38);
	/*画正文框*/
	GUI_SetColor(GUI_WHITE);
	GUI_FillRoundedRect(20,55,700,440,8);
	/*画装饰的线条*/
	GUI_SetColor(GUI_COL_WIN1);
	GUI_SetPenSize(3);
	GUI_DrawLine(80,72,600,72);
	GUI_SetPenSize(1);
	for (i = 1; i < line+1; i++) 
	 {
		GUI_DrawLine(80,72+YGAPS*i,600,72+YGAPS*i);
	 }
}
void PAINTWIN_SETAPP(WM_MESSAGE * pMsg)
{

  WM_HWIN hItem = pMsg->hWin;
	WINDOW_SetBkColor(pMsg->hWin, GUI_COL_WIN1); //布置背景色
	//布置标题栏
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP00);
	TEXT_SetTextColor(hItem, GUI_WHITE);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP01);
	TEXT_SetTextColor(hItem, GUI_GRAY_11);
	TEXT_SetTextAlign(hItem,   GUI_TA_VCENTER);
	TEXT_SetFont(hItem, &GUI_FontHZ16);
		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_APP02);
	TEXT_SetTextColor(hItem, GUI_GRAY_11);
	TEXT_SetTextAlign(hItem,   GUI_TA_VCENTER);
	TEXT_SetFont(hItem, &GUI_Font13B_1);
}




const APP_LIST_ITEM _APP_CB_LIST[] ={
	_cbCallback00,
	_cbCallback01,
	_cbCallback02,
  _cbCallback03,
	_cbCallback04,
	_cbCallback05,
	_cbCallback06,
	_cbCallback07,
	_cbCallback08,
	_cbCallback09,
	_cbCallback10,  //监测管理
	_cbCallback11,  //状态控制台
	_cbCallback12,	//用户登陆
  _cbCallback13,	//日志查询
	_cbCallback14,	//出厂日期
	_cbCallback15,	//其他参数
	0,//16
	0,//17
	0,//18
	0,//19
	_cbCallback20,	//电源状态
	_cbCallback21,	//
	_cbCallback22,	//
	_cbCallback23,	//
	_cbCallback24,	//
	_cbCallback25,	//
	_cbCallback26,	//
	_cbCallback27,	//
	_cbCallback28,	//
	_cbCallback29,	//
};





