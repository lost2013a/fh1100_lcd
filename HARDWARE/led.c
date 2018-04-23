//////////////////////////////////////////////////////////////////////////////////	 
//创建日期:2016/5/15
//版本：V1.0
//zx							  
////////////////////////////////////////////////////////////////////////////////// 	
#include "led.h" 
 
    
 //LED IO初始化，设置默认状态
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  	//TIM9时钟使能  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOB,C,E时钟
  //初始化GPIOB5
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
#ifdef 	Hadware_version2		//-----------硬件版本V1.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
#endif
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	//初始化GPIOE0-6
	GPIO_InitStructure.GPIO_Pin = (0X7F<<0);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	//设置默认输出电平1
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
  GPIO_SetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	GPIO_SetBits(GPIOE,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);
	//TIM9_OPM_Init

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;           //GPIOE5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化PE5
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9); //GPIOE5复用为定时器9
	//168M/16800=10Khz的计数频率,每次计数100us，PPS灯亮时间=100us*3500=350ms. 
	TIM_TimeBaseStructure.TIM_Prescaler=16800-1;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=3500-1;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //采样时钟分频，这里不需要用到
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure);//初始化定时器9	
	//初始化TIM9 Channel1 PWM2模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CNT<CCR，通道 CH 为无效，否则为有效
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_Pulse =1;//快速比较模式下无效
	TIM_OC1Init(TIM9, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM9 OC1
	TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);  //使能TIM9在CCR1上的预装载寄存器
  TIM_ARRPreloadConfig(TIM9,ENABLE);//ARPE使能 
	TIM_SelectOnePulseMode(TIM9,TIM_CR1_OPM);//单脉冲模式
	TIM_OC1FastConfig(TIM9,TIM_CCMR1_OC1FE);//快速比较模式		
}
