/* Includes ------------------------------------------------------------------*/
#include "flash_conf.h"


//读取指定地址的半字(16位数据) 
//faddr:读地址 
//返回值:对应数据.
u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
} 


void FLASH_Init(void)
{
	//读取flash不需要解锁
	FLASH_Unlock();//解锁从而可对flash模块寄存器写
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
								FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR |FLASH_FLAG_PGSERR);
	//操作结束,操作错误,写保护错误,编程对其错误,编程并行位数错误,编程顺序错误
}
void EraseSector(uint32_t EraseAddr,uint32_t NumToErase)
{
	int i;
	uint32_t endaddr=0;
	uint16_t StartSector,EndSector;
	endaddr = EraseAddr+NumToErase;
	
	FLASH_Init();
	
	StartSector = GetSector(EraseAddr);//获取写入起始地址所在扇区编号
	EndSector = GetSector(endaddr);//获取结束地址所在扇区的编号
	
	FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存!!!
	for(i=StartSector;i<=EndSector;i += 8)//扇区编号每次增加8
	{
		//擦除扇区i号
		if(FLASH_EraseSector(i,VoltageRange_3) != FLASH_COMPLETE)
		{
			while(1);//擦除失败
		}
	}
	FLASH_DataCacheCmd(ENABLE);//FLASH擦除结束,开启数据fetch
}

void FLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)
{
	FLASH_Status status = FLASH_COMPLETE;
	uint32_t addrx=0;
	uint32_t endaddr=0;	
	if(WriteAddr<0x08000000||WriteAddr%4)return;	//非法地址
	FLASH_Init();	//解锁 
	FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存!!!
	addrx 	= WriteAddr;	//写入的起始地址
	endaddr = WriteAddr+(NumToWrite*4);	//写入的结束地址
	

	if(addrx<0X1FFF0000)			//只有主存储区,才需要执行擦除操作!!
		{
			while(addrx<endaddr)		
				{
					if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
						{   
							status=FLASH_EraseSector(GetSector(addrx),VoltageRange_3);//VCC=2.7~3.6V之间!!
							if(status!=FLASH_COMPLETE)break;	//发生错误了
						}else addrx+=4;
				}	
		}
	if(status==FLASH_COMPLETE)
		{
			while(WriteAddr<endaddr)//写数据
			{
				if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)//写入数据
				{ 
					break;	//写入异常
				}
				WriteAddr+=4;
				pBuffer++;
			} 
		}
	FLASH_DataCacheCmd(ENABLE);//FLASH擦除结束,开启数据fetch
	FLASH_Lock();//上锁
}


void FLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]= *(__IO uint32_t*)ReadAddr;//读取4个字节.
		ReadAddr+=4;//偏移4个字节.	
	}
}

//将我们要存取的flash地址转换成扇区的编号
uint16_t GetSector(uint32_t Address)
{
  uint16_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11))
  {
    sector = FLASH_Sector_11;  
  }
	else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12))
  {
    sector = FLASH_Sector_12;  
  }
	else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13))
  {
    sector = FLASH_Sector_13;  
  }
	else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14))
  {
    sector = FLASH_Sector_14;  
  }
	else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15))
  {
    sector = FLASH_Sector_15;  
  }
	else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16))
  {
    sector = FLASH_Sector_16;  
  }
	else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17))
  {
    sector = FLASH_Sector_17;  
  }
	else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_18))
  {
    sector = FLASH_Sector_18;  
  }
	else /*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23)*/
	{
		sector = FLASH_Sector_23;
	}
  return sector;
}
