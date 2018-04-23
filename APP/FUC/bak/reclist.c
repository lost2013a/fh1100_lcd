
#include "mainmenu.h"
#include "sys.h"



static void Fuc_SYSMODE_Handler(WM_MESSAGE * pMsg) 			
{
	WM_HWIN hItem ;
	uint16_t SYSMODEFLAG=0x1001; 			 //2.->>CAN_BUF报文类型
	uint8_t data;
	if(SYSMODEFLAG==((CAN1_RX1_STA&0xffff0000)>>16))	//收到的报文是对应槽，对应网口，对应类型的报文
	{
		data  = CAN1_RX1_BUF[SETSYSMODE_OFFSET];
		if(data>1)  //处理溢出
			data =0;
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
		DROPDOWN_SetSel(hItem,data) ;
	}
	CAN1_RX1_STA=0;
}

void Send_SetSYSMODE(WM_MESSAGE * pMsg,uint8_t rtr) 			
{
	WM_HWIN hItem ;
	uint32_t send_id = 0x03100100;			//CANID
	uint8_t  msg[8]  = {0,}; 
	uint8_t  can_err;
	if(rtr)
	{
		can_err=CAN1_Send_REQ(send_id);
	}
	else
	{
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_APP0);
		msg[0]= DROPDOWN_GetSel(hItem);
		if(msg[0]>1)  //处理溢出
			msg[0] =0;
		can_err=CAN1_Send_Msg(msg,send_id,8);
	}
	CAN_SHOW_ERR(can_err);
}

const REC_LIST_ITEM _REC_CB_LIST[] ={
	
{Fuc_SYSMODE_Handler,Send_SetSYSMODE},
	
};



/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/


