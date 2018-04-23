#include "includes.h"
#include "Mainmenu.h"
#include "sys.h"
#include "WM.h"
#include "DIALOG.h"

/*
*********************************************************************************************************
*			优先级安排
*********************************************************************************************************
*/
/*
APP_CFG_TASK_START_PRIO            2u		扫描按键
APP_CFG_TASK_MsgCAN0_PRIO          3u		收CAN0报文		
APP_CFG_TASK_PPSTICK_PRIO          4u		每秒更新状态(每秒的CAN0触发，CAN没收到则使用硬件RTC秒唤醒)
APP_CFG_TASK_COM_PRIO              5u		保留
APP_CFG_TASK_USER_IF_PRIO          6u		屏幕保护等其他功能
APP_CFG_TASK_GUI_PRIO              (OS_CFG_PRIO_MAX - 4u)	界面显示，最低优先级
*/

/*
*********************************************************************************************************
*            进程控制块
*********************************************************************************************************
*/                                                         
static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

static  OS_TCB   AppTaskMsgCAN0TCB;
static  CPU_STK  AppTaskMsgCAN0Stk[APP_CFG_TASK_MsgCAN0_STK_SIZE];

static  OS_TCB   AppTaskCOMTCB;
static  CPU_STK  AppTaskCOMStk[APP_CFG_TASK_COM_STK_SIZE];

static  OS_TCB   AppTaskPPSTICKTCB;
static  CPU_STK  AppTaskPPSTICKStk[APP_CFG_TASK_PPSTICK_STK_SIZE];

static  OS_TCB   AppTaskUserIFTCB;
static  CPU_STK  AppTaskUserIFStk[APP_CFG_TASK_USER_IF_STK_SIZE];

static  OS_TCB   AppTaskGUITCB;
static  CPU_STK  AppTaskGUIStk[APP_CFG_TASK_GUI_STK_SIZE];

/*
*********************************************************************************************************
*            进程函数
*********************************************************************************************************
*/
static  void  AppTaskStart          (void     *p_arg);
static  void  AppTaskMsgCAN0         (void     *p_arg);
static  void  AppTaskPPSTICK         (void     *p_arg);
static  void  AppTaskCOM			(void 	  *p_arg);
static  void  AppTaskCreate         (void);
static  void  AppTaskGUI(void *p_arg);
static  void  AppObjCreate          (void);
void    App_Printf (CPU_CHAR *format, ...);

/*
*******************************************************************************************************
*             信号量变量
*******************************************************************************************************
*/
static  OS_SEM       AppPrintfSemp;	/* 用于printf互斥 */
static	OS_SEM       SEM_SYNCH;	    /* 用于CAN同步 */
static	OS_SEM       SEM_PPSTICK;	  /* 用于秒脉冲响应 */
char BDTIMEBUF[30]= {0};


int main(void)
{
	OS_ERR  err;  
	bsp_init(); 
	sprintf(BDTIMEBUF,"%c%c%c%c -%c%c-%c%c "__TIME__"",BUILD_Y0,BUILD_Y1,BUILD_Y2,BUILD_Y3,
			BUILD_M0,BUILD_M1,BUILD_D0,BUILD_D1);
	OSInit(&err);  
	/* 创建一个启动任务（也就是主任务）。启动任务会创建所有的应用程序任务 */
	OSTaskCreate((OS_TCB       *)&AppTaskStartTCB,  /* 任务控制块地址 */           
                 (CPU_CHAR     *)"App Task Start",  /* 任务名 */
                 (OS_TASK_PTR   )AppTaskStart,      /* 启动任务函数地址 */
                 (void         *)0,                 /* 传递给任务的参数 */
                 (OS_PRIO       )APP_CFG_TASK_START_PRIO, /* 任务优先级 */
                 (CPU_STK      *)&AppTaskStartStk[0],     /* 堆栈基地址 */
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE / 10, /* 堆栈监测区，这里表示后10%作为监测区 */
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,      /* 堆栈空间大小 */
                 (OS_MSG_QTY    )0,  /* 本任务支持接受的最大消息数 */
                 (OS_TICK       )0,  /* 设置时间片 */
                 (void         *)0,  /* 堆栈空间大小 */  
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
				 (OS_ERR       *)&err);	
				 /*  定义如下：
					OS_OPT_TASK_STK_CHK      使能检测任务栈，统计任务栈已用的和未用的
					OS_OPT_TASK_STK_CLR      在创建任务时，清零任务栈
					OS_OPT_TASK_SAVE_FP      如果CPU有浮点寄存器，则在任务切换时保存浮点寄存器的内容
				 */   
    OSStart(&err);                                               
    (void)&err;
    return (0);
}


static  void  AppTaskStart (void *p_arg)
{
	OS_ERR      err;
    (void)p_arg;  
#if OS_CFG_STAT_TASK_EN > 0u
     OSStatTaskCPUUsageInit(&err);   
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif	
	/* 创建应用程序的任务 */
	AppTaskCreate();
	/* 创建任务通信 */
	AppObjCreate();
	while(1)
	{
		bsp_KeyScan();		//保持对按键的扫描
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err); //延时5ms	
	}     
}


static void AppTaskMsgCAN0(void *p_arg)
{
	OS_ERR      err;
	(void)p_arg;
	while(1)
	{	
		OSSemPend(&SEM_SYNCH,0,OS_OPT_PEND_BLOCKING,0,&err); //等待来自CPU的CAN信号，和RTC的秒唤醒共同去(竞争)驱动定时器3
		if(err == OS_ERR_NONE)
		{	
			CAN_Head_Handler();	
		}			
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //延时50ms	
	}   
}


static void AppTaskPPSTICK(void *p_arg)
{
	OS_ERR      err;
	(void)p_arg;	              /* 避免编译器报警 */
	while (1) 
	{   
		OSSemPend(&SEM_PPSTICK,0,OS_OPT_PEND_BLOCKING,0,&err); //请求PPSTICK信号量，由定时器3产生
		DEV_PPS_RECV();        			//每秒更新装置的界面的显示(主界面和灯)
		#if defined PPSTICK__RTC
			DEV_Time_Handler(1);		//报文质量位如果小于2，则使用报文的时间，否则读RTC的时间来显示
		#endif
		DevSys.PPS_Tick =1;				//用来更新装置界面的状态更新的一个信号
		OSTimeDlyHMSM(0,0,0,350,OS_OPT_TIME_HMSM_STRICT,&err); //延时350ms	
		DevSys.PPS_Tick_300 = 1;		//用来更新装置界面的状态更新的另一个信号,用来做闪烁效果
	}
}



static void AppTaskCOM(void *p_arg)
{
	
	OS_ERR  err;  	
	(void)p_arg;
	while(1)
	{	
		OSTimeDlyHMSM(0,10,0,0,OS_OPT_TIME_HMSM_STRICT,&err); //保留，可用来获取调试信息的
	} 						  	 	       											   
}


static void AppTaskUserIF(void *p_arg)
{
	
	static uint8_t laststate = 0;
	OS_ERR      err;
	(void)p_arg;	           /* 避免编译器报警 */
	while (1) 
	{ 
		//喂狗
   	#if defined WATCHDOG_USE
			IWDG_Feed();
		#endif	
		if(DevSys.LCD_Protect_ON == 0)//启用屏幕保护
			{
				if(DevSys.idle_out_flag!=laststate)  //有变位调整
					{
						if(DevSys.idle_out_flag == 0)
						 LCD_BackLightOn();
						else
						 LCD_BackLightOff();
					}
				laststate = DevSys.idle_out_flag;
			}
		//屏幕保护
		if(DevSys.idle_out>=IDLE_TIMEOUT)			//保护
			{
				DevSys.idle_out_flag = STA_ERR;
			}
		else										//累加
			{
				DevSys.idle_out_flag = STA_OK;
				DevSys.idle_out++;
			}
		//不通迅的重启机制
		if(DevHead.devfalt&0x08){
			if(DevSys.can0_err<CPU_REBOOT_TIME)
				DevSys.can0_err++;
			else if(DevSys.can0_err==CPU_REBOOT_TIME)
				NVIC_SystemReset();
		}	
		else
			DevSys.can0_err=0;
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
	}
}


static void AppTaskGUI(void *p_arg)
{
	MainTask();
}


static  void  AppTaskCreate (void)
{
	OS_ERR      err;
	
	/**************创建MsgCAN0任务*********************/
	OSTaskCreate((OS_TCB       *)&AppTaskMsgCAN0TCB,             
                 (CPU_CHAR     *)"App Task MspCAN0",
                 (OS_TASK_PTR   )AppTaskMsgCAN0, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_MsgCAN0_PRIO,
                 (CPU_STK      *)&AppTaskMsgCAN0Stk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_MsgCAN0_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_MsgCAN0_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
				 
	/**************创建PPS_TICK任务*********************/
	OSTaskCreate((OS_TCB       *)&AppTaskPPSTICKTCB,             
                 (CPU_CHAR     *)"App Task PPSTICK",
                 (OS_TASK_PTR   )AppTaskPPSTICK, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_PPSTICK_PRIO,
                 (CPU_STK      *)&AppTaskPPSTICKStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_PPSTICK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_PPSTICK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )( OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);

	/**************创建COM任务*********************/
	OSTaskCreate((OS_TCB       *)&AppTaskCOMTCB,            
                 (CPU_CHAR     *)"App Task COM",
                 (OS_TASK_PTR   )AppTaskCOM, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_COM_PRIO,
                 (CPU_STK      *)&AppTaskCOMStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_COM_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_COM_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);	
/**************创建USER IF任务*********************/
	OSTaskCreate((OS_TCB       *)&AppTaskUserIFTCB,             
                 (CPU_CHAR     *)"App Task UserIF",
                 (OS_TASK_PTR   )AppTaskUserIF, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_USER_IF_PRIO,
                 (CPU_STK      *)&AppTaskUserIFStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_USER_IF_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_USER_IF_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_SAVE_FP | OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
								 
	/**************创建GUI任务*********************/			 
	OSTaskCreate((OS_TCB       *)&AppTaskGUITCB,              
                 (CPU_CHAR     *)"App Task GUI",
                 (OS_TASK_PTR   )AppTaskGUI, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_GUI_PRIO,
                 (CPU_STK      *)&AppTaskGUIStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),//STEMWIN需要存储浮点
                 (OS_ERR       *)&err);


 
								 
}

/*
*********************************************************************************************************
*	函 数 名: AppObjCreate
*	功能说明: 创建任务通讯
*	形    参: p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
static  void  AppObjCreate (void)
{
	OS_ERR      err;
	
	/* 
	   创建信号量数值为1的时候可以实现互斥功能，也就是只有一个资源可以使用 
	   本例程是将串口1的打印函数作为保护的资源。防止串口打印的时候被其它任务抢占
	   造成串口打印错乱。
	*/
	OSSemCreate((OS_SEM    *)&AppPrintfSemp,
				(CPU_CHAR  *)"AppPrintfSemp",
				(OS_SEM_CTR )0,//初始值
				(OS_ERR    *)&err);
	
	/* 创建计数值为0，用于实现任务同步功能 */
	OSSemCreate((OS_SEM    *)&SEM_SYNCH,
				(CPU_CHAR  *)"SEM_SYNCH",
				(OS_SEM_CTR )0,
				(OS_ERR    *)&err);
	
	/* 创建计数值为0，用于实现任务PPS功能 */
	OSSemCreate((OS_SEM    *)&SEM_PPSTICK,
				(CPU_CHAR  *)"SEM_PPSTICK",
				(OS_SEM_CTR )0,
				(OS_ERR    *)&err);
}

/*
*********************************************************************************************************
*	函 数 名: App_Printf
*	功能说明: 线程安全的printf方式		  			  
*	形    参: 同printf的参数。
*             在C中，当无法列出传递函数的所有实参的类型和数目时,可以用省略号指定参数表
*	返 回 值: 无
*********************************************************************************************************
*/
void  App_Printf(CPU_CHAR *format, ...)
{
    CPU_CHAR  buf_str[80 + 1];
    va_list   v_args;
    OS_ERR    os_err;


    va_start(v_args, format);
   (void)vsnprintf((char       *)&buf_str[0],
                   (size_t      ) sizeof(buf_str),
                   (char const *) format,
                                  v_args);
    va_end(v_args);

	/* 互斥操作 */
    OSSemPend((OS_SEM  *)&AppPrintfSemp,
              (OS_TICK  )0u,
              (OS_OPT   )OS_OPT_PEND_BLOCKING,
              (CPU_TS  *)0,
              (OS_ERR  *)&os_err);

    printf("%s", buf_str);

   (void)OSSemPost((OS_SEM  *)&AppPrintfSemp,
                   (OS_OPT   )OS_OPT_POST_1,
                   (OS_ERR  *)&os_err);

}


//RTC WAKE UP中断服务函数
void RTC_WKUP_IRQHandler(void)
{ 
#if SYSTEM_SUPPORT_OS  //使用UCOS操作系统
	OSIntEnter();    
#endif	
if(RTC_GetFlagStatus(RTC_FLAG_WUTF)==SET)//WK_UP中断?
	{ 
		RTC_ClearFlag(RTC_FLAG_WUTF);	//清除中断标志
		TIM_Cmd(TIM3, DISABLE);
		TIM_Cmd(TIM3, ENABLE);
	}   
	EXTI_ClearITPendingBit(EXTI_Line22);//清除中断线22的中断标志 			
#if SYSTEM_SUPPORT_OS  
	OSIntExit();    	//退出中断，触发中断级调度
#endif		
}
 


 /**
  * @brief  CAN1_RX0消息挂号中断服务函数，接受POWERPC每秒动态信息
  * @param  无
  * @retval 无
  */		 
void CAN1_RX0_IRQHandler(void)
{
	OS_ERR    os_err;
	CanRxMsg RxMessage;
	u32 i,bufoff;
	RxMessage.ExtId=0;
#if SYSTEM_SUPPORT_OS  //使用UCOS操作系统
	OSIntEnter();    
#endif
	
	if(RxMessage.ExtId==0x01100130)		//收到报文包头，从新开始接受
			CAN1_RX0_STA=0;
	if((CAN1_RX0_STA&0x10000)>>16)//接收完成	
		CAN1->RF0R |= CAN_RF0R_RFOM0; //清FIFO_0挂号消息
	else
	{
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		bufoff=(((RxMessage.ExtId&0xff00)>>8)+(RxMessage.ExtId&0xf))<<3;			//计算BUF存放的地址偏移量
		if(RxMessage.ExtId==0x01102311)					//收到最后一帧，置接受完成标志位
			{
				CAN1_RX0_STA|=0x10000;	
 /*
*********************************************************************************************************
			发送信号量
*********************************************************************************************************
*/			
				OSSemPost(&SEM_SYNCH,OS_OPT_POST_1,&os_err);//发送信号量			
			}
		
		for(i=0;i<RxMessage.DLC;i++)												//数据帧固定8个字节
		{
			CAN1_RX0_BUF[i+bufoff]=RxMessage.Data[i];
			CAN1_RX0_STA++;
			if((CAN1_RX0_STA&0xffff)>(CAN1_RX0_LEN-1))
				CAN1_RX0_STA=0;//接收数据错误,重新开始接收	 		
		}
	}
#if DEBUG_CANRECIVE_
	//ShowxNum(0,0,bufoff,6,0);
#endif
 #if SYSTEM_SUPPORT_OS  
	OSIntExit();    	//退出中断，触发中断级调度
#endif		
}

 /**
  * @brief  CAN1_RX1消息挂号中断服务函数，接收配置状态信息
  * @param  无
  * @retval 无
  */
void CAN1_RX1_IRQHandler(void)
{
	CanRxMsg RxMessage;
	u32 i, FRAME_LEN,FRAME_NUMS,bufoff,FRAME_REV; 
	#if SYSTEM_SUPPORT_OS  //使用UCOS操作系统
		OSIntEnter();    
	#endif
	if((CAN1_RX1_STA&0xffff0000)!=0)		//接收完成标志	2016.06.21修改接受完成条件为CAN1_RX1_STA&0xffff0000
		CAN1->RF1R |= CAN_RF1R_RFOM1; 		//清FIFO_1挂号消息
	else
	{
		CAN_Receive(CAN1, CAN_FIFO1, &RxMessage);
		bufoff=(((RxMessage.ExtId&0xff00)>>8)+(RxMessage.ExtId&0xf))<<3;			//计算BUF存放的地址偏移量
		FRAME_NUMS=RxMessage.ExtId&0x0f;     	//帧序号
		FRAME_LEN=(RxMessage.ExtId&0xf0)>>4; 	//报文包帧数量
		FRAME_REV=(CAN1_RX1_STA&0xffff)>>3;		//收到帧数=(字节数/8)
		if(FRAME_NUMS==FRAME_LEN)	
		{
			if((FRAME_REV==FRAME_LEN)||(FRAME_LEN==0))		//2016.08.15修改：1.报文包帧数量=帧序号=收到帧数2.报文包帧数量=帧序号=0接受完成	
				CAN1_RX1_STA|=((RxMessage.ExtId&0x00ffff00)<<8);	//置标志位(标志包含插槽位，网口号，报文类型共16BIT)
		}	
		else if(FRAME_NUMS==0)							//如果收到报文包头，重新开始接受
			CAN1_RX1_STA=0;
		for(i=0;i<RxMessage.DLC;i++)				
		{
			CAN1_RX1_BUF[i+(bufoff)]=RxMessage.Data[i];
			CAN1_RX1_STA++;
			if((CAN1_RX1_STA&0xffff)>(CAN1_RX1_LEN-1))
				CAN1_RX1_STA=0;//接收数据错误,重新开始接收	
		}	
	}		
#if SYSTEM_SUPPORT_OS  
	OSIntExit();    	//退出中断，触发中断级调度
#endif	
	//LED10=!LED10;
}

//定时器3中断服务函数,由PPS唤醒和CAN_PPS竞争触发
void TIM3_IRQHandler(void)
{
	OS_ERR    os_err;
#if SYSTEM_SUPPORT_OS  //使用UCOS操作系统
	OSIntEnter();    
#endif
	if(TIM_GetITStatus(TIM3,TIM_IT_CC1)==SET) //溢出中断	
	{
		
		 /*
*********************************************************************************************************
			发送信号量
*********************************************************************************************************
*/			
		OSSemPost(&SEM_PPSTICK,OS_OPT_POST_1,&os_err);//发送信号量		
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_CC1);  //清除中断标志位
	
#if SYSTEM_SUPPORT_OS  
	OSIntExit();    	//退出中断，触发中断级调度
#endif		
}







