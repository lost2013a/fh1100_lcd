/*
*********************************************************************************************************
*	                                  
*	模块名称 : 汉字显示接口
*	文件名称 : GUICharPEx.c
*	版    本 : V1.0
*	说    明 : 这种汉字显示方式适合使用等宽字体，非等宽的微软雅黑等字体显示效果较差。
*              支持点阵：
*                1. 支持12点阵，16点阵，24点阵以及32点阵汉字。
*                2. 6X12点阵的ASCII，8X16点阵的ASCII，12X24点阵的ASCII，16X32点阵的ASCII。
*              字库存储方式：
*                1. 字库存储到SPI FLASH里面。
*
*	修改记录 :
*		版本号    日期           作者         说明
*		V1.0    2015-04-24     Eric2013       首版
*
*      Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "Mainmenu.h"
#include "includes.h"


/* 点阵数据缓存, 必须大于等于单个字模需要的存储空间*/ 
#define BYTES_PER_FONT      512 
static U8 GUI_FontDataBuf[BYTES_PER_FONT];

/*
*********************************************************************************************************
*	函 数 名: GUI_GetDataFromMemory
*	功能说明: 读取点阵数据
*	形    参: pProp  GUI_FONT_PROP类型结构
*             c      字符
*	返 回 值: 无
*********************************************************************************************************
*/
static void GUI_GetDataFromMemory(const GUI_FONT_PROP GUI_UNI_PTR *pProp, U16P c) 
{ 
    U16 BytesPerFont; 
    U32 oft = 0, BaseAdd; 
	U8 code1,code2;
	
	char *font = (char *)pProp->paCharInfo->pData; 

	/* 每个字模的数据字节数 */
    BytesPerFont = GUI_pContext->pAFont->YSize * pProp->paCharInfo->BytesPerLine; 
    if (BytesPerFont > BYTES_PER_FONT)
	{
		BytesPerFont = BYTES_PER_FONT;
	}
	
	/* 英文字符地址偏移计算 */ 
    if (c < 0x80)                                                                
    { 
		if(strncmp("A12", font, 3) == 0)     /* 6*12 ASCII字符 */
		{
			BaseAdd = 0x1DBE00;
		}
		else if(strncmp("A16", font, 3) == 0) /* 8*16 ASCII字符 */
		{
			BaseAdd = 0x1DD780;
		}
		else if(strncmp("A24", font, 3) == 0) /* 12*24 ASCII字符 */
		{
			BaseAdd = 0x1DFF00;
		}
		else if(strncmp("A32", font, 3) == 0) /* 24*48 ASCII字符 */
		{
			BaseAdd = 0x1E5A50;
		}
		
		oft = (c-0x20) * BytesPerFont + BaseAdd; 
    } 
	/* 汉字和全角字符的偏移地址计算 */
    else                                                                           
    { 
		if(strncmp("H12", font, 3) == 0)      /* 12*12 字符 */
		{
			BaseAdd = 0x0;
		}
		else if(strncmp("H16", font, 3) == 0)  /* 16*16 字符 */
		{
			BaseAdd = 0x2C9D0;
		}
		else if(strncmp("H24", font, 3) == 0)  /* 24*24 字符 */
		{
			BaseAdd = 0x68190;
		}
		else if(strncmp("H32", font, 3) == 0)  /* 32*32 字符 */
		{
			BaseAdd = 0XEDF00;
		}
		
		/* 根据汉字内码的计算公式锁定起始地址 */
		code2 = c >> 8;
		code1 = c & 0xFF;
		if (code1 >=0xA1 && code1 <= 0xA9 && code2 >=0xA1)
		{
			oft = ((code1 - 0xA1) * 94 + (code2 - 0xA1)) * BytesPerFont + BaseAdd;
		}
		else if (code1 >=0xB0 && code1 <= 0xF7 && code2 >=0xA1)
		{
			oft = ((code1 - 0xB0) * 94 + (code2 - 0xA1) + 846) * BytesPerFont + BaseAdd;
		}

    }
	W25QXX_Read(GUI_FontDataBuf,oft,BytesPerFont);
	/* 读取点阵数据 */
	//sf_ReadBuffer(GUI_FontDataBuf, oft, BytesPerFont);
} 

/*
*********************************************************************************************************
*	函 数 名: GUIUSER_DispChar
*	功能说明: 显示字符
*	形    参: c 显示的字符
*	返 回 值: 无
*********************************************************************************************************
*/
void GUIUSER_DispChar(U16P c)  
{ 
    int BytesPerLine; 
    GUI_DRAWMODE DrawMode = GUI_pContext->TextMode; 
    const GUI_FONT_PROP GUI_UNI_PTR *pProp = GUI_pContext->pAFont->p.pProp; 
	
    /* 确定显示的字符是否在有效范围内 */  
    for (; pProp; pProp = pProp->pNext)                                          
    { 
        if ((c >= pProp->First) && (c <= pProp->Last))break; 
    } 
	
	/* 字符有效，进行显示 */
    if (pProp) 
    { 
        GUI_DRAWMODE OldDrawMode;
        const GUI_CHARINFO GUI_UNI_PTR * pCharInfo = pProp->paCharInfo;
        GUI_GetDataFromMemory(pProp, c);
        BytesPerLine = pCharInfo->BytesPerLine;                
        OldDrawMode  = LCD_SetDrawMode(DrawMode);
        LCD_DrawBitmap(GUI_pContext->DispPosX, GUI_pContext->DispPosY,
                       pCharInfo->XSize, GUI_pContext->pAFont->YSize,
                       GUI_pContext->pAFont->XMag, GUI_pContext->pAFont->YMag,
                       1,    
                       BytesPerLine,
                       &GUI_FontDataBuf[0],
                       &LCD_BKCOLORINDEX
                       );
		
        /* 填充背景 */
        if (GUI_pContext->pAFont->YDist > GUI_pContext->pAFont->YSize) 
        {
            int YMag = GUI_pContext->pAFont->YMag;
            int YDist = GUI_pContext->pAFont->YDist * YMag;
            int YSize = GUI_pContext->pAFont->YSize * YMag;
            if (DrawMode != LCD_DRAWMODE_TRANS) 
            {
                LCD_COLOR OldColor = GUI_GetColor();
                GUI_SetColor(GUI_GetBkColor());
                LCD_FillRect(GUI_pContext->DispPosX, GUI_pContext->DispPosY + YSize, 
                             GUI_pContext->DispPosX + pCharInfo->XSize, 
                             GUI_pContext->DispPosY + YDist);
                GUI_SetColor(OldColor);
            }
        }
		
        LCD_SetDrawMode(OldDrawMode);
//      if (!GUI_MoveRTL)
        GUI_pContext->DispPosX += pCharInfo->XDist * GUI_pContext->pAFont->XMag;
    }
} 

/*
*********************************************************************************************************
*	函 数 名: GUIUSER_X_GetCharDistX
*	功能说明: 获取字符的X轴间距
*	形    参: c  字符
*	返 回 值: 无
*********************************************************************************************************
*/
int GUIUSER_GetCharDistX(U16P c, int * pSizeX) 
{
    const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUI_pContext->pAFont->p.pProp;  
    for (; pProp; pProp = pProp->pNext)                                         
    {
        if ((c >= pProp->First) && (c <= pProp->Last))break;
    }
    return (pProp) ? (pProp->paCharInfo)->XSize * GUI_pContext->pAFont->XMag : 0;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
