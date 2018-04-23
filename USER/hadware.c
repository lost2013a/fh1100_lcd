#include "includes.h"
#include "hadware.h"
#include "mainmenu.h"
#include "flash_conf.h"
void bsp_init(void)
{
	int i;
	uint32_t readbuff[3];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	delay_init(168);  	//时钟初始化
	uart_init(115200);    //串口波特率设置
	LED_Init();           //LED初始化
	//南网，开机电源灯亮
	LED11=0;
	TFTLCD_Init();
	W25QXX_Init();				//初始化W25Q128
	bsp_InitKey(); 	
  CAN1_Mode_Init();     //波特率1M CAN初始化	
	USERPASS = 0001;
	#if defined PPSTICK__RTC
		My_RTC_Init();		 		//初始化RTC
		RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);		//配置WAKE UP中断,1秒钟中断一次
		RTC_WakeUpCmd( ENABLE);//开启WAKE UP 定时器
	#endif
	#if defined WATCHDOG_USE
		IWDG_Init();
	#endif
	my_mem_init(SRAMIN);		//初始化内部内存池
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);//开启CRC时钟
	/*
	 装置的初始化状态
	*/
	memset(&DevHead,0,sizeof(__DEV_HEAD)); 		//结构体数据清0
	memset(&DevSys,0,sizeof(__RUNSYS)); 		//结构体数据清0
	DevHead.tb       = STA_ERR;
	DevHead.synflag  = STA_ERR;
	DevHead.bds      = STA_ERR;
	DevHead.gps      = STA_ERR;
	DevHead.b1       = STA_ERR;
	DevHead.b2 		   = STA_ERR;
	DevHead.monitor1 = STA_ERR;
	//南网，开机电源灯亮
	//DevHead.monitor2 = STA_ERR;	
	DevHead.ptp1     = STA_ERR;
	DevHead.ptp2     = STA_ERR;
  FLASH_Read(FLASH_SAVE_ADDR,readbuff,3);//读出信息到readflash中
	if( readbuff[0] != 0x55 )//特殊值
	 {
		 readbuff[0] = 0x55;
		 readbuff[1] = 1;
		 readbuff[2] = 0;
		 DevSys.LCD_Protect_ON =  readbuff[1];
		 DevSys.USER_PASS   =  readbuff[2];
		 FLASH_Write(FLASH_SAVE_ADDR,readbuff,3);//写用户
		 for(i=0;i<1000;i++){}//等待烧写结束		 
	 }
	else
	 {
		 DevSys.LCD_Protect_ON =  readbuff[1];
		 DevSys.USER_PASS   =  readbuff[2];
		 myprintf("readbuff[3]={h%x,%d,,%d}",readbuff[0],readbuff[1],readbuff[2]);
	 }
}


