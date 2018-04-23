#ifndef __FLASH_IF_H
#define __FLASH_IF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#define FLASH_START_ADDR 0x08000000//flash起始地址
#define FLASH_END_ADDR	 0x081E0000//flash结束地址
#define FLASH_SAVE_ADDR  0x08140000//我们要写入数据的地址扇区18
#define	FLASH_SAVE_ADDR1 0x081E0000
//#define FLASH_SAVE_ADDR2 0x08120000


//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//扇区0起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//扇区1起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//扇区2起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//扇区4起始地址, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//扇区5起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//扇区6起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//扇区7起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//扇区8起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//扇区10起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//扇区11起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_12    ((u32)0x08100000) 	//扇区12起始地址,16 Kbytes  
#define ADDR_FLASH_SECTOR_13    ((u32)0x08104000) 	//扇区13起始地址,16 Kbytes
#define ADDR_FLASH_SECTOR_14    ((u32)0x08108000) 	//扇区14起始地址,16 Kbytes 
#define ADDR_FLASH_SECTOR_15    ((u32)0x0810C000) 	//扇区15起始地址,16 Kbytes 
#define ADDR_FLASH_SECTOR_16    ((u32)0x08110000) 	//扇区16起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_17    ((u32)0x08120000) 	//扇区17起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_18    ((u32)0x08140000) 	//扇区18起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_23    ((u32)0x081E0000) 	//扇区23起始地址,128 Kbytes  


//#define FLASH_Sector_0     ((uint16_t)0x0000) /*!< Sector Number 0 */
//#define FLASH_Sector_1     ((uint16_t)0x0008) /*!< Sector Number 1 */
//#define FLASH_Sector_2     ((uint16_t)0x0010) /*!< Sector Number 2 */
//#define FLASH_Sector_3     ((uint16_t)0x0018) /*!< Sector Number 3 */
//#define FLASH_Sector_4     ((uint16_t)0x0020) /*!< Sector Number 4 */
//#define FLASH_Sector_5     ((uint16_t)0x0028) /*!< Sector Number 5 */
//#define FLASH_Sector_6     ((uint16_t)0x0030) /*!< Sector Number 6 */

void FLASH_Init(void);//解锁flash并清除一些标记
uint16_t GetSector(uint32_t Address);//通过起始地址获取所在扇区编号
void FLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead);//到指定的flash读取数据
void FLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite);//写入数据到指定的地方
void EraseSector(uint32_t EraseAddr,uint32_t NumToErase);

#endif 
