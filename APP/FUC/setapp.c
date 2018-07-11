#include "mainmenu.h"
#include "sys.h"



/*
*********************************************************************************************************
*                                  Ӧ�ó�����ں���
*********************************************************************************************************
*/ 


typedef struct 
{
	const GUI_WIDGET_CREATE_INFO * paWidget;
	const int                    NumWidgets;
} APP_CREATE_ITEM;

extern  void *_APP_CB_LIST[APPITEMS];

static const GUI_WIDGET_CREATE_INFO _aDialogCreate00[] = {
 
  { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "��������",             	ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
  { TEXT_CreateIndirect,     "ʱ����������:" ,   ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*0,  150,  30, TEXT_CF_LEFT },
	
	
  { DROPDOWN_CreateIndirect,  NULL,     				ID_DROPDOWN_APP0,XPOS0+XGAPS*0,YPOS0+YGAPS*0-2, 160, 100, 0, 0x0, 0 },
	
	//
	{ TEXT_CreateIndirect,     "ʱԴѡ�����:" ,  	ID_TEXT_APP04,  XPOS0-XPOS_T6,  YPOS0+YGAPS*1,  150,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "ģ��1����:" ,    	ID_TEXT_APP05,  XPOS0-XPOS_T6,  YPOS0+YGAPS*2,  150,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "ģ��2����:" ,    	ID_TEXT_APP06,  XPOS0-XPOS_T6,  YPOS0+YGAPS*3,  150,  30, TEXT_CF_LEFT },
	{ DROPDOWN_CreateIndirect,  NULL,     				ID_DROPDOWN_APP1,XPOS0+XGAPS*0,YPOS0+YGAPS*1-2, 160, 100, 0, 0x0, 0 },
	{ DROPDOWN_CreateIndirect,  NULL,     				ID_DROPDOWN_APP2,XPOS0+XGAPS*0,YPOS0+YGAPS*2-2, 160, 100, 0, 0x0, 0 },
	{ DROPDOWN_CreateIndirect,  NULL,     				ID_DROPDOWN_APP3,XPOS0+XGAPS*0,YPOS0+YGAPS*3-2, 160, 100, 0, 0x0, 0 },
	//


};
static const GUI_WIDGET_CREATE_INFO _aDialogCreate01[] = {
  { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "��������",             	ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
  { TEXT_CreateIndirect,     "���ڱ�������:" ,    	ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*0,  200,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "���ڲ�����:",      ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*1,  200,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "����У�鷽ʽ:" ,    ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*2,  200,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "IRIG-B��У�鷽ʽ:",   ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*3,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "IRIG-B������λ:",     ID_TEXT_APP05,  XPOS0-XPOS_T6,  YPOS0+YGAPS*4,  200,  30, TEXT_CF_LEFT },
	
 
  { DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP0,   XPOS0+XGAPS*1,YPOS0+YGAPS*0, 225, 100, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP1,   XPOS0+XGAPS*1,YPOS0+YGAPS*1, 160, 100, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP2,   XPOS0+XGAPS*1,YPOS0+YGAPS*2, 160, 100, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP3,   XPOS0+XGAPS*1,YPOS0+YGAPS*3, 160, 100, 0, 0x0, 0 },
	//
	{ TEXT_CreateIndirect,     "���ں�IRIG-B������������ڴ���COM2��IRIG-B2",   ID_TEXT_APP04,  XPOS0-XPOS_T6,  YPOS0+YGAPS*5,  400,  30, TEXT_CF_LEFT },
	{ DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP4,   XPOS0+XGAPS*1,YPOS0+YGAPS*4, 160, 100, 0, 0x0, 0 },
	//
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate02[] = {
  { WINDOW_CreateIndirect,  "",               ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "IP����", ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",  ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
  { TEXT_CreateIndirect,     "������ۺ�:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T5,  YPOS0+YGAPS*0,  150,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "�����˿ں�:",   ID_TEXT_APP03,  XPOS0-XPOS_T5,  YPOS0+YGAPS*1,  150,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "����IP��ַ:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T5,  YPOS0+YGAPS*2,  150,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "��������:",     ID_TEXT_APP03,  XPOS0-XPOS_T5,  YPOS0+YGAPS*3,  150,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Ĭ������:",     ID_TEXT_APP03,  XPOS0-XPOS_T5,  YPOS0+YGAPS*4,  150,  30, TEXT_CF_LEFT },
 // { TEXT_CreateIndirect,     "NTP�Զ�IP:",    ID_TEXT_APP03,  XPOS0-XPOS_T5,  YPOS0+YGAPS*5,  150,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "ѡ������ڲ�����ʱ��IP��ַ��ȫΪ0��",   ID_TEXT_APP04,  XPOS0-XPOS_T5,  YPOS0+YGAPS*6,  550,  30, TEXT_CF_LEFT },
	
 
  { DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP0,   XPOS0+XGAPS*0,YPOS0+YGAPS*0-2, 135, 100, 0, 0x0, 0  },
  { DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP1,   XPOS0+XGAPS*0,YPOS0+YGAPS*1-2, 135, 100, 0, 0x0, 0 },

  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP00,   XPOS0+XGAPS*0,YPOS0+YGAPS*2, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP01,   XPOS0+XGAPS*1,YPOS0+YGAPS*2, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP02,   XPOS0+XGAPS*2,YPOS0+YGAPS*2, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP03,   XPOS0+XGAPS*3,YPOS0+YGAPS*2, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },

  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP04,   XPOS0+XGAPS*0,YPOS0+YGAPS*3, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP05,   XPOS0+XGAPS*1,YPOS0+YGAPS*3, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP06,   XPOS0+XGAPS*2,YPOS0+YGAPS*3, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP07,   XPOS0+XGAPS*3,YPOS0+YGAPS*3, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },

  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP08,   XPOS0+XGAPS*0,YPOS0+YGAPS*4, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP09,   XPOS0+XGAPS*1,YPOS0+YGAPS*4, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP10,   XPOS0+XGAPS*2,YPOS0+YGAPS*4, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP11,   XPOS0+XGAPS*3,YPOS0+YGAPS*4, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },

//  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP12,   XPOS0+XGAPS*0,YPOS0+YGAPS*5, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
//  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP13,   XPOS0+XGAPS*1,YPOS0+YGAPS*5, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
//  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP14,   XPOS0+XGAPS*2,YPOS0+YGAPS*5, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
//  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP15,   XPOS0+XGAPS*3,YPOS0+YGAPS*5, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },

};



static const GUI_WIDGET_CREATE_INFO _aDialogCreate03[] = {
  { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "�ӳ�����",             	ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
  { TEXT_CreateIndirect,     "���������ӳ�:" ,    	ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*0,  200,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "GPS�����ӳ�:",      ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*1,  200,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "IRIG-B1�����ӳ�:" ,    ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*2,  200,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "IRIG-B2�����ӳ�:",   ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*3,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "PTP1�����ӳ�:" ,    	 ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*4,  200,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "PTP2�����ӳ�:",  	  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*5,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "�ź�����ӳ�:",      ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*6,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "����",            ID_TEXT_APP03,  420,  YPOS0+YGAPS*0,  50,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "����",            ID_TEXT_APP03,  420,  YPOS0+YGAPS*1,  50,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "����",            ID_TEXT_APP03,  420,  YPOS0+YGAPS*2,  50,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "����",            ID_TEXT_APP03,  420,  YPOS0+YGAPS*3,  50,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "����",            ID_TEXT_APP03,  420,  YPOS0+YGAPS*4,  50,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "����",            ID_TEXT_APP03,  420,  YPOS0+YGAPS*5,  50,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "����",            ID_TEXT_APP03,  420,  YPOS0+YGAPS*6,  50,  30, TEXT_CF_LEFT },
	
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP00,   XPOS0+XGAPS*1,YPOS0+YGAPS*0, 110, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP01,   XPOS0+XGAPS*1,YPOS0+YGAPS*1, 110, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP02,   XPOS0+XGAPS*1,YPOS0+YGAPS*2, 110, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP03,   XPOS0+XGAPS*1,YPOS0+YGAPS*3, 110, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP04,   XPOS0+XGAPS*1,YPOS0+YGAPS*4, 110, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP05,   XPOS0+XGAPS*1,YPOS0+YGAPS*5, 110, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP06,   XPOS0+XGAPS*1,YPOS0+YGAPS*6, 110, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
};


static const GUI_WIDGET_CREATE_INFO _aDialogCreate04[] = {
  { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "������",             	ID_TEXT_APP00, 12,	2, 180,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
  { TEXT_CreateIndirect,     "������:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*0,  200,  30, TEXT_CF_LEFT },
  { DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP0,   XPOS0+65,YPOS0+YGAPS*0-2, 180, 100, 0, 0x0, 0  },
};






static const GUI_WIDGET_CREATE_INFO _aDialogCreate05[] = {
  { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Դ���ȼ�",             	ID_TEXT_APP00, 12,	2, 180,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
  { TEXT_CreateIndirect,     "���ȼ�����:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*0,  150,  30, TEXT_CF_LEFT },
  { DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP0,   XPOS0+XGAPS*0,YPOS0+YGAPS*0-2, 210, 100, 0, 0x0, 0  },

};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate06[] = {
  { WINDOW_CreateIndirect,  "",               ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "PTP����", ID_TEXT_APP00, 20,	4, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",  ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect,     "������ۺ�:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T5,  YPOS0-YGAPS*1,  150,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "�����˿ں�:",   ID_TEXT_APP03,  XPOS0-XPOS_T5,  YPOS0+YGAPS*0,  150,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "PTPģʽ:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T5,  YPOS0+YGAPS*1,  150,  30, TEXT_CF_LEFT },
	
	{ TEXT_CreateIndirect,     "",    ID_TEXT_APP08,  425,  YPOS0+YGAPS*1,  200 , 30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	
  { TEXT_CreateIndirect,     "UTC_Offset:",   ID_TEXT_APP03,  XPOS0-XPOS_T5,  YPOS0+YGAPS*2,  150,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "ʱ����:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T5,  YPOS0+YGAPS*3,  150,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "ʱ�Ӽ���:",     ID_TEXT_APP03,  XPOS0-XPOS_T5,  YPOS0+YGAPS*4,  150,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "���ȼ�:",     ID_TEXT_APP03,  XPOS0-XPOS_T5,  YPOS0+YGAPS*5,  150,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "��������:",    ID_TEXT_APP03,  XPOS0-XPOS_T5,  YPOS0+YGAPS*6,  150,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "MAC��ַ:",    ID_TEXT_APP03,  XPOS0-XPOS_T5,  YPOS0+YGAPS*7,  150,  30, TEXT_CF_LEFT },

	{ TEXT_CreateIndirect,     "(���ȼ�1)",    ID_TEXT_APP04,  XPOS0+RECTX,  YPOS0+YGAPS*5,  80,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "(���ȼ�2)",    ID_TEXT_APP05,  XPOS0+RECTX+145,  YPOS0+YGAPS*5,  80 , 30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "(Announce)",    ID_TEXT_APP06,  XPOS0+RECTX,  YPOS0+YGAPS*6,  80,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
 	{ TEXT_CreateIndirect,     "(Sync)",    ID_TEXT_APP07,  XPOS0+RECTX+145,  YPOS0+YGAPS*6,  80,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },

	{ DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP0,   XPOS0+XGAPS*0,YPOS0-YGAPS*1-2, 165, 100, 0, 0x0, 0  },
  { DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP1,   XPOS0+XGAPS*0,YPOS0+YGAPS*0-2, 165, 100, 0, 0x0, 0 },
	
  { DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP2,   XPOS0+XGAPS*0,YPOS0+YGAPS*1-2, 165, 100, 0, 0x0, 0  },
 

  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP00,   XPOS0,YPOS0+YGAPS*2, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP01,   XPOS0,YPOS0+YGAPS*3, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP02,   XPOS0,YPOS0+YGAPS*4, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP03,   XPOS0,YPOS0+YGAPS*5, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP04,   XPOS0+145,YPOS0+YGAPS*5, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP05,   XPOS0,YPOS0+YGAPS*6, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP06,   XPOS0+145,YPOS0+YGAPS*6, RECTX, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP07,   XPOS0,YPOS0+YGAPS*7, RECTX*2, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 6 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP08,   XPOS0+100,YPOS0+YGAPS*7, RECTX*2, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 6 },

};


static const GUI_WIDGET_CREATE_INFO _aDialogCreate07[] = {
  { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "�ź�����",             	ID_TEXT_APP00, 12,	2, 180,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
  { TEXT_CreateIndirect,     "AC����Ʊ�:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*0,  150,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "PPX�ź�:" , 	  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*1,  150,  30, TEXT_CF_LEFT },
  { DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP0,   XPOS0+XGAPS*0,YPOS0+YGAPS*0-2, 150, 100, 0, 0x0, 0  },
	{ DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP1,   XPOS0+XGAPS*0,YPOS0+YGAPS*1-2, 150, 100, 0, 0x0, 0  },

};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate08[] = {
  { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "��λ����",             	ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect,     "�Զ���γ��(N):" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*0,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "�Զ��徭��(E):" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*1,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "�Զ���߶�:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*2,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "��" ,  ID_TEXT_APP03,  XPOS0+160,  YPOS0+YGAPS*0,  30,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "��" ,  ID_TEXT_APP03,  XPOS0+160,  YPOS0+YGAPS*1,  30,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "��" ,  ID_TEXT_APP03,  XPOS0+160,  YPOS0+YGAPS*2,  30,  30, TEXT_CF_LEFT },
 
	
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP00,   XPOS0+65,YPOS0+YGAPS*0, 90, RECTY, TEXT_CF_LEFT, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP01,   XPOS0+65,YPOS0+YGAPS*1, 90, RECTY, TEXT_CF_LEFT, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP02,   XPOS0+65,YPOS0+YGAPS*2, 90, RECTY, TEXT_CF_LEFT, 3 },
  
};
static const GUI_WIDGET_CREATE_INFO _aDialogCreate09[] = {
  { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "ģ�����",             	ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
	
	
	{ TEXT_CreateIndirect,     "ģ�����ʹ��:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*0,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "�Զ���ʱ��(��):" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*1,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "�Զ���ʱ��(��):" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*2,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "�Զ���ʱ��(��):" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*3,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "�Զ���ʱ��(ʱ):" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*4,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "�Զ���ʱ��(��):" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*5,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "�Զ���ʱ��(��):" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*6,  200,  30, TEXT_CF_LEFT },
 
	{ DROPDOWN_CreateIndirect,  NULL,    ID_DROPDOWN_APP0, XPOS0+65,YPOS0+YGAPS*0-2, 160, 100, 0, 0x0, 0  },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP00,   XPOS0+65,YPOS0+YGAPS*1, 55, RECTY, TEXT_CF_LEFT, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP01,   XPOS0+65,YPOS0+YGAPS*2, 55, RECTY, TEXT_CF_LEFT, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP02,   XPOS0+65,YPOS0+YGAPS*3, 55, RECTY, TEXT_CF_LEFT, 3 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP03,   XPOS0+65,YPOS0+YGAPS*4, 55, RECTY, TEXT_CF_LEFT, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP04,   XPOS0+65,YPOS0+YGAPS*5, 55, RECTY, TEXT_CF_LEFT, 3 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP05,   XPOS0+65,YPOS0+YGAPS*6, 55, RECTY, TEXT_CF_LEFT, 3 },
  
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate10[] = {
  { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "ʱ������",             	ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
  { TEXT_CreateIndirect,     "ʱ������:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*0,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "����ʱʱ��ƫ��:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*1,  200,  30, TEXT_CF_LEFT },

  { DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP0,   XPOS0+65,YPOS0+YGAPS*0-2, 180, 100, 0, 0x0, 0  },
	{ DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP1,   XPOS0+65,YPOS0+YGAPS*1-2, 180, 100, 0, 0x0, 0  },
	
};



static const GUI_WIDGET_CREATE_INFO _aDialogCreate15[] = {
 
  { WINDOW_CreateIndirect,  "",               ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "��������",          	ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",   ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect,     "��Ļ��������:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*0,  150,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "��¼��������:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*1,  150,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "" ,  						 ID_TEXT_APP04,  XPOS0-XPOS_T6,  YPOS0+YGAPS*3,  150,  30, TEXT_CF_LEFT },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP00,   XPOS0,YPOS0+YGAPS*1, 80, RECTY, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
	
	 { DROPDOWN_CreateIndirect,  NULL,     ID_DROPDOWN_APP0,   XPOS0+XGAPS*0,YPOS0+YGAPS*0-2, 160, 100, 0, 0x0, 0  },
	
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate12[] = {
	{ WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	//{ TEXT_CreateIndirect, "�û�����:",         	ID_TEXT_APP00, 280,	140, 160,	30, TEXT_CF_VCENTER},
	{ TEXT_CreateIndirect, "�������û�����",         	ID_TEXT_APP01, 200,	80, 400,	30, TEXT_CF_HCENTER|TEXT_CF_VCENTER},
	{ TEXT_CreateIndirect, "�ɶ����ӵ����Զ��������豸�������ι�˾",      	ID_TEXT_APP02, 420,	442, 400,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "CHENGDU FUHE ELECTRIC POWER AUTOMATION EQUIPMENT SETS CO .LTD,.",  
													ID_TEXT_APP03, 420,	462, 400,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "", 	ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP00,   350,145, 100, 40, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 3 },
};



static const GUI_WIDGET_CREATE_INFO _aDialogCreate13[] = {
 
  { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "��־��ѯ",             ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "��־��ȡ��...",    		  ID_TEXT_APP03, 550,	20, 250,	16, TEXT_CF_LEFT|TEXT_CF_VCENTER, 0x64, 0},
	{ PROGBAR_CreateIndirect,  NULL,     				  ID_PROGBAR_APP0,655,20 ,130, 16, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect,  NULL,     				ID_LISTVIEW_APP0,1,36 ,800-1, 443, 0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate14[] = {
 
  { WINDOW_CreateIndirect,  "",               ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "������Ϣ", ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",  ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
  { TEXT_CreateIndirect,     "װ���ͺ�:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*(-1),  150,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "��������:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*0,  150,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Ͷ������:",   ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*1,  150,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "������������:",   ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*2,  150,  30, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "����汾:",       ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*3,  150,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "ICDģ�Ͱ汾:",    ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*4,  150,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "CIDģ�Ͱ汾:",    ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*5,  150,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "",    ID_TEXT_APP09,  XPOS0-XPOS_T6,  YPOS0+YGAPS*6,  150,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "FH-1100-G-JPQ" ,  			ID_TEXT_APP03,  XPOS0,        	YPOS0+YGAPS*(-1),  180,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "2017-01-21" ,         ID_TEXT_APP05,  XPOS0,        	YPOS0+YGAPS*2,  150,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "Ver1.0  У����8286" , ID_TEXT_APP06,  XPOS0,        	YPOS0+YGAPS*3,  270,  30, TEXT_CF_LEFT }, 
	{ TEXT_CreateIndirect,     "Ver1.0  У����345f" , ID_TEXT_APP07,  XPOS0,        	YPOS0+YGAPS*4,  270,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "Ver1.0  У����345f" , ID_TEXT_APP08,  XPOS0,        	YPOS0+YGAPS*5,  270,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "" , ID_TEXT_APP10,  XPOS0,        	YPOS0+YGAPS*6,  350,  30, TEXT_CF_LEFT },
	
	{ TEXT_CreateIndirect,     "" , ID_TEXT_APP11,  XPOS0,YPOS0+YGAPS*0, 200, RECTY, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "" , ID_TEXT_APP12,  XPOS0,YPOS0+YGAPS*1, 200, RECTY, TEXT_CF_LEFT },
	
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP00,  XPOS0+XGAPS*0,YPOS0+YGAPS*0,    50, RECTY,  TEXT_CF_VCENTER, 4 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP01,  XPOS0+XGAPS*1+10,YPOS0+YGAPS*0, 28, RECTY,  TEXT_CF_VCENTER, 2 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP02,  XPOS0+XGAPS*2,YPOS0+YGAPS*0,    28, RECTY,  TEXT_CF_VCENTER, 2 },
	
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP03,  XPOS0+XGAPS*0,YPOS0+YGAPS*1,    50, RECTY,  TEXT_CF_VCENTER, 4 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP04,  XPOS0+XGAPS*1+10,YPOS0+YGAPS*1, 28, RECTY,  TEXT_CF_VCENTER, 2 },
  { EDIT_CreateIndirect,     NULL,     ID_EDIT_APP05,  XPOS0+XGAPS*2,YPOS0+YGAPS*1,    28, RECTY,  TEXT_CF_VCENTER, 2 },
	
	
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP04,  XPOS0-XPOS_T6,  YPOS0+YGAPS*7-2,  600,  36, TEXT_CF_LEFT },
	
};


static const GUI_WIDGET_CREATE_INFO _aDialogCreate11[] = {
  { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "״̬����",             ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
	
	
	{ TEXT_CreateIndirect,     "ʹ��ģ��:" , 	   ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*(-1),  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "�ֶ�01-02:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*0,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "�ֶ�03-04:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*1,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "�ֶ�05-06:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*2,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "�ֶ�07-08:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*3,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,   	 "�ֶ�09-10:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*4,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "�ֶ�11-12:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*5,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "�ֶ�13-14:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*6,  200,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "�ֶ�15-16:" ,  ID_TEXT_APP03,  XPOS0-XPOS_T6,  YPOS0+YGAPS*7,  200,  30, TEXT_CF_LEFT },
	
	{ TEXT_CreateIndirect,     "" ,  ID_TEXT_APP10,  350,  YPOS0+YGAPS*0+7,  300,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "" ,  ID_TEXT_APP11,  350,  YPOS0+YGAPS*1+7,  300,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "" ,  ID_TEXT_APP12,  350,  YPOS0+YGAPS*2+7,  300,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "" ,  ID_TEXT_APP13,  350,  YPOS0+YGAPS*3+7,  300,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "" ,  ID_TEXT_APP14,  350,  YPOS0+YGAPS*4+7,  300,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "" ,  ID_TEXT_APP15,  350,  YPOS0+YGAPS*5+7,  300,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "" ,  ID_TEXT_APP16,  350,  YPOS0+YGAPS*6+7,  300,  30, TEXT_CF_LEFT },
	{ TEXT_CreateIndirect,     "" ,  ID_TEXT_APP17,  350,  YPOS0+YGAPS*7+7,  300,  30, TEXT_CF_LEFT },
	
 
	{ DROPDOWN_CreateIndirect, NULL,    ID_DROPDOWN_APP0, XPOS0,YPOS0+YGAPS*(-1)-2, 160, 100, 0, 0x0, 0  },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP00,   XPOS0,YPOS0+YGAPS*0, 60, RECTY, GUI_TA_HCENTER, 4 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP01,   XPOS0,YPOS0+YGAPS*1, 60, RECTY, GUI_TA_HCENTER, 4 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP02,   XPOS0,YPOS0+YGAPS*2, 60, RECTY, GUI_TA_HCENTER, 4 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP03,   XPOS0,YPOS0+YGAPS*3, 60, RECTY, GUI_TA_HCENTER, 4 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP04,   XPOS0,YPOS0+YGAPS*4, 60, RECTY, GUI_TA_HCENTER, 4 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP05,   XPOS0,YPOS0+YGAPS*5, 60, RECTY, GUI_TA_HCENTER, 4 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP06,   XPOS0,YPOS0+YGAPS*6, 60, RECTY, GUI_TA_HCENTER, 4 },
	{ EDIT_CreateIndirect,     NULL,     ID_EDIT_APP07,   XPOS0,YPOS0+YGAPS*7, 60, RECTY, GUI_TA_HCENTER, 4 },
	//{ TEXT_CreateIndirect,    "   -  08    -  16     -  24    -  32" ,  ID_TEXT_APP04,  360,  YPOS0+YGAPS*5,  400,  30, TEXT_CF_LEFT },
  
};





static const GUI_WIDGET_CREATE_INFO _aDialogCreate20[] = {
 
  { WINDOW_CreateIndirect,  "",               ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "��Դ״̬", ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",  ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
	//{ TEXT_CreateIndirect,     "**��Դ״̬**",   			  ID_TEXT_APP02, 250,  420,  300,  48, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
	
	{ TEXT_CreateIndirect,     "��Դģ��1״̬:" ,  ID_TEXT_APP03,  LXPOS0-LXPOS_T7-16,  YPOS0+LYGAPS*0,  LXPOS_T7,  LYGAPS, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "��Դģ��2״̬:" ,  ID_TEXT_APP04,   LXPOS0-LXPOS_T7-16,  YPOS0+LYGAPS*1,  LXPOS_T7,  LYGAPS, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "����Ƶ��1:" ,  ID_TEXT_APP05,  LXPOS0-LXPOS_T7-16,  YPOS0+LYGAPS*2,  LXPOS_T7,  LYGAPS, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "����Ƶ��2:" ,  ID_TEXT_APP06,   LXPOS0-LXPOS_T7-16,  YPOS0+LYGAPS*3,  LXPOS_T7,  LYGAPS, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP10, LXPOS0,  YPOS0+LYGAPS*0,  LXPOS_T7,  LYGAPS, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP11, LXPOS0,  YPOS0+LYGAPS*1,  LXPOS_T7,  LYGAPS, TEXT_CF_LEFT|TEXT_CF_VCENTER},
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP12, LXPOS0,  YPOS0+LYGAPS*2,  LXPOS_T7,  LYGAPS, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP13, LXPOS0,  YPOS0+LYGAPS*3,  LXPOS_T7,  LYGAPS, TEXT_CF_LEFT|TEXT_CF_VCENTER},
};


static const GUI_WIDGET_CREATE_INFO _aDialogCreate21[] = {
 
  { WINDOW_CreateIndirect,  "",               ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Ƶ��Դѱ��״̬", ID_TEXT_APP00, 12,	2, 230,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", ID_TEXT_APP01, 260,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",  ID_TEXT_APP02, 260,	23, 200,	16, 0, 0x64, 0},
	//{ TEXT_CreateIndirect,     "**��Դ״̬**",   			  ID_TEXT_APP02, 250,  420,  300,  48, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
	
	{ TEXT_CreateIndirect,     "Ƶ��Դѱ��״̬:" ,  ID_TEXT_APP03,  LXPOS0-LXPOS_T8,  YPOS0+LYGAPS*0,  LXPOS_T8,  LYGAPS, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "" ,  ID_TEXT_APP04,  LXPOS0-LXPOS_T8,  					YPOS0+LYGAPS*1,  LXPOS_T8,  LYGAPS, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "" ,  ID_TEXT_APP05,  LXPOS0-LXPOS_T8, 			 YPOS0+LYGAPS*2,  LXPOS_T8,  LYGAPS, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "" ,  ID_TEXT_APP06,  LXPOS0-LXPOS_T8,  		YPOS0+LYGAPS*3,  LXPOS_T8,  LYGAPS, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP10, LXPOS0,  YPOS0+LYGAPS*0,  LXPOS_T8,  LYGAPS, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP11, LXPOS0,  YPOS0+LYGAPS*1,  LXPOS_T8,  LYGAPS, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP12, LXPOS0,  YPOS0+LYGAPS*2,  LXPOS_T8,  LYGAPS, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP13, LXPOS0,  YPOS0+LYGAPS*3,  LXPOS_T8,  LYGAPS, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	
	
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate22[] = {
 
  { WINDOW_CreateIndirect,  "",               ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "�澯״̬", ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",  ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "",    		  ID_TEXT_APP03, 550,	20, 250,	16, TEXT_CF_LEFT|TEXT_CF_VCENTER, 0x64, 0},
	{ TEXT_CreateIndirect, "��<ȷ��>���¸澯״̬",    	ID_TEXT_APP04, 300,	480-32, 200,	24, TEXT_CF_HCENTER|TEXT_CF_VCENTER, 0x64, 0},
  { LISTVIEW_CreateIndirect,  NULL,     				ID_LISTVIEW_APP0,1,36 ,800-1, 443-46, 0, 0x0, 0 },
	

};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate23[] = {
  { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "����״̬",             	ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
  { TEXT_CreateIndirect,     "����ͬ��״̬:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*(-1),  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "��������״̬:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*0,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "����ģ��״̬:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*1,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "�������ǿ���:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*2,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "����ͨ��ƫ��:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*3,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "�ź�״̬:" ,      ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*4,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "����״̬:" ,      ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*5,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "ʱ��������״̬:" , ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*6,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "" , 		 ID_TEXT_APP04,  XPOS1-XPOS_T8,  YPOS0+YGAPS*7,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP10, XPOS1,  YPOS0+YGAPS*(-1),  XPOS0,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP11, XPOS1,  YPOS0+YGAPS*0,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP12, XPOS1,  YPOS0+YGAPS*1,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP13, XPOS1,  YPOS0+YGAPS*2,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP14, XPOS1,  YPOS0+YGAPS*3,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP15, XPOS1,  YPOS0+YGAPS*4,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP16, XPOS1,  YPOS0+YGAPS*5,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP17, XPOS1,  YPOS0+YGAPS*6,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP18, XPOS1,  YPOS0+YGAPS*7,  600,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	
};


static const GUI_WIDGET_CREATE_INFO _aDialogCreate24[] = {
    { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "GPS״̬",             	ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
  { TEXT_CreateIndirect,     "GPSͬ��״̬:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*(-1),  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "GPS����״̬:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*0,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "GPSģ��״̬:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*1,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "GPS���ǿ���:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*2,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "GPSͨ��ƫ��:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*3,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "�ź�״̬:" ,      ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*4,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "����״̬:" ,      ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*5,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "ʱ��������״̬:" , ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*6,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "" , 		 ID_TEXT_APP04,  XPOS1-XPOS_T8,  YPOS0+YGAPS*7,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP10, XPOS1,  YPOS0+YGAPS*(-1),  XPOS0,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP11, XPOS1,  YPOS0+YGAPS*0,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP12, XPOS1,  YPOS0+YGAPS*1,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP13, XPOS1,  YPOS0+YGAPS*2,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP14, XPOS1,  YPOS0+YGAPS*3,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP15, XPOS1,  YPOS0+YGAPS*4,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP16, XPOS1,  YPOS0+YGAPS*5,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP17, XPOS1,  YPOS0+YGAPS*6,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP18, XPOS1,  YPOS0+YGAPS*7,  600,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate25[] = {
  { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "IRIG-B1״̬",             	ID_TEXT_APP00, 12,	2, 180,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 210,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 210,	23, 200,	16, 0, 0x64, 0},
  { TEXT_CreateIndirect,     "IRIG-B1ͬ��״̬:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8-12,  YPOS0+YGAPS*0,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "IRIG-B1����λ:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8-12,  YPOS0+YGAPS*1,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "IRIG-B1ͨ��ƫ��:" ,      ID_TEXT_APP03,  XPOS1-XPOS_T8-12,  YPOS0+YGAPS*2,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "�ź�״̬:" ,      ID_TEXT_APP03,  XPOS1-XPOS_T8-12,  YPOS0+YGAPS*3,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "����״̬:" ,      ID_TEXT_APP03,  XPOS1-XPOS_T8-12,  YPOS0+YGAPS*4,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "ʱ��������״̬:" , ID_TEXT_APP03,  XPOS1-XPOS_T8-12,  YPOS0+YGAPS*5,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP10, XPOS1,  YPOS0+YGAPS*0,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP11, XPOS1,  YPOS0+YGAPS*1,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP12, XPOS1,  YPOS0+YGAPS*2,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP13, XPOS1,  YPOS0+YGAPS*3,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP14, XPOS1,  YPOS0+YGAPS*4,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP15, XPOS1,  YPOS0+YGAPS*5,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate26[] = {
  { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "IRIG-B2״̬",             	ID_TEXT_APP00, 12,	2, 180,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 210,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 210,	23, 200,	16, 0, 0x64, 0},
  { TEXT_CreateIndirect,     "IRIG-B2ͬ��״̬:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8-12,  YPOS0+YGAPS*0,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "IRIG-B2����λ:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8-12,  YPOS0+YGAPS*1,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "IRIG-B2ͨ��ƫ��:" ,      ID_TEXT_APP03,  XPOS1-XPOS_T8-12,  YPOS0+YGAPS*2,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "�ź�״̬:" ,      ID_TEXT_APP03,  XPOS1-XPOS_T8-12,  YPOS0+YGAPS*3,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "����״̬:" ,      ID_TEXT_APP03,  XPOS1-XPOS_T8-12,  YPOS0+YGAPS*4,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "ʱ��������״̬:" , ID_TEXT_APP03,  XPOS1-XPOS_T8-12,  YPOS0+YGAPS*5,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP10, XPOS1,  YPOS0+YGAPS*0,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP11, XPOS1,  YPOS0+YGAPS*1,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP12, XPOS1,  YPOS0+YGAPS*2,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP13, XPOS1,  YPOS0+YGAPS*3,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP14, XPOS1,  YPOS0+YGAPS*4,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP15, XPOS1,  YPOS0+YGAPS*5,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	
};


static const GUI_WIDGET_CREATE_INFO _aDialogCreate27[] = {
  { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "PTP1״̬",             	ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
  { TEXT_CreateIndirect,     "PTP1ͬ��״̬:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*(-1),  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "PTP1����ģʽ:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*0,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "PTP1����״̬:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*1,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "PTP1��·�ӳ�:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*2,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "PTP1ͨ��ƫ��:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*3,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "�ź�״̬:" ,      ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*4,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "����״̬:" ,      ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*5,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "ʱ��������״̬:" , ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*6,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "�����ʱ��MAC:" , ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*7,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP10, XPOS1,  YPOS0+YGAPS*(-1),  XPOS0,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP11, XPOS1,  YPOS0+YGAPS*0,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP12, XPOS1,  YPOS0+YGAPS*1,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP13, XPOS1,  YPOS0+YGAPS*2,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP14, XPOS1,  YPOS0+YGAPS*3,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP15, XPOS1,  YPOS0+YGAPS*4,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP16, XPOS1,  YPOS0+YGAPS*5,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP17, XPOS1,  YPOS0+YGAPS*6,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP18, XPOS1,  YPOS0+YGAPS*7,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate28[] = {
  { WINDOW_CreateIndirect,  "",                 ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "PTP2״̬",             	ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", 	ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",    	ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
  { TEXT_CreateIndirect,     "PTP2ͬ��״̬:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*(-1),  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "PTP2����ģʽ:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*0,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "PTP2����״̬:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*1,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "PTP2��·�ӳ�:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*2,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "PTP2ͨ��ƫ��:" ,  ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*3,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "�ź�״̬:" ,      ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*4,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "����״̬:" ,      ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*5,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "ʱ��������״̬:" , ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*6,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "�����ʱ��MAC:" , ID_TEXT_APP03,  XPOS1-XPOS_T8,  YPOS0+YGAPS*7,  XPOS_T8,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP10, XPOS1,  YPOS0+YGAPS*(-1),  XPOS0,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP11, XPOS1,  YPOS0+YGAPS*0,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP12, XPOS1,  YPOS0+YGAPS*1,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP13, XPOS1,  YPOS0+YGAPS*2,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP14, XPOS1,  YPOS0+YGAPS*3,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
  { TEXT_CreateIndirect,     "",   			ID_TEXT_APP15, XPOS1,  YPOS0+YGAPS*4,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP16, XPOS1,  YPOS0+YGAPS*5,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP17, XPOS1,  YPOS0+YGAPS*6,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,     "",   			ID_TEXT_APP18, XPOS1,  YPOS0+YGAPS*7,  XPOS0+100,  30, TEXT_CF_LEFT|TEXT_CF_VCENTER },
	
};


static const GUI_WIDGET_CREATE_INFO _aDialogCreate29[] = {
  { WINDOW_CreateIndirect,  "",               ID_WINDOW_APP0, 0,  0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "���״̬", ID_TEXT_APP00, 12,	2, 150,	32, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "FH1100ʱ��ͬ��װ��", ID_TEXT_APP01, 180,	9, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "www.fuhe.com.cn",  ID_TEXT_APP02, 180,	23, 200,	16, 0, 0x64, 0},
	{ TEXT_CreateIndirect, "",    		  ID_TEXT_APP03, 550,	20, 250,	16, TEXT_CF_LEFT|TEXT_CF_VCENTER, 0x64, 0},
  { LISTVIEW_CreateIndirect,  NULL,     				ID_LISTVIEW_APP0,1,36 ,800-1, 443-46, 0, 0x0, 0 },
	
};




static const APP_CREATE_ITEM _APP_CREATE_LIST[] ={
	
 {_aDialogCreate00,GUI_COUNTOF(_aDialogCreate00)},
 {_aDialogCreate01,GUI_COUNTOF(_aDialogCreate01)},
 {_aDialogCreate02,GUI_COUNTOF(_aDialogCreate02)},
 {_aDialogCreate03,GUI_COUNTOF(_aDialogCreate03)},
 {_aDialogCreate04,GUI_COUNTOF(_aDialogCreate04)},
 {_aDialogCreate05,GUI_COUNTOF(_aDialogCreate05)},
 {_aDialogCreate06,GUI_COUNTOF(_aDialogCreate06)},
 {_aDialogCreate07,GUI_COUNTOF(_aDialogCreate07)},
 {_aDialogCreate08,GUI_COUNTOF(_aDialogCreate08)},
 {_aDialogCreate09,GUI_COUNTOF(_aDialogCreate09)},
 {_aDialogCreate10,GUI_COUNTOF(_aDialogCreate10)},
 {_aDialogCreate11,GUI_COUNTOF(_aDialogCreate11)},
 {_aDialogCreate12,GUI_COUNTOF(_aDialogCreate12)},
 {_aDialogCreate13,GUI_COUNTOF(_aDialogCreate13)},
 {_aDialogCreate14,GUI_COUNTOF(_aDialogCreate14)},
 {_aDialogCreate15,GUI_COUNTOF(_aDialogCreate15)},
 {0,0},//16
 {0,0},//17
 {0,0},//18
 {0,0},//19
 {_aDialogCreate20,GUI_COUNTOF(_aDialogCreate20)},
 {_aDialogCreate21,GUI_COUNTOF(_aDialogCreate21)},
 {_aDialogCreate22,GUI_COUNTOF(_aDialogCreate22)},
 {_aDialogCreate23,GUI_COUNTOF(_aDialogCreate23)},
 {_aDialogCreate24,GUI_COUNTOF(_aDialogCreate24)},
 {_aDialogCreate25,GUI_COUNTOF(_aDialogCreate25)},
 {_aDialogCreate26,GUI_COUNTOF(_aDialogCreate26)},
 {_aDialogCreate27,GUI_COUNTOF(_aDialogCreate27)},
 {_aDialogCreate28,GUI_COUNTOF(_aDialogCreate28)},
 {_aDialogCreate29,GUI_COUNTOF(_aDialogCreate29)},
};




void APP_Setup(uint8_t app_num) 
{

	hWinSetapp = GUI_CreateDialogBox(_APP_CREATE_LIST[app_num].paWidget,_APP_CREATE_LIST[app_num].NumWidgets, (WM_CALLBACK *)_APP_CB_LIST[app_num], WM_HBKWIN, 0, 0);
	hWin_msgto   = hWinSetapp;
	WM_CreateTimer(WM_GetClientWindow(hWinSetapp), 
				   ID_TimerTime1, 	            
				   20,                           
				   0);
}


