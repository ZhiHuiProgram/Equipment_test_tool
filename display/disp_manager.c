#include "disp_manager.h"
#include <stdio.h>
#include <string.h>
#include <font_manager.h>
static pDispOpr g_DispDevs = NULL;
static pDispOpr g_DispDefault = NULL;
static DispBuff g_tDispBuff;
static int line_width, pixel_width;

void DrawTextInRegionCenter(char *name,pRegion ptRegion, unsigned int color){
	int n = strlen(name);
	int i=0;
	int error;
	FontBitMap tFontBitMap;
	RegionCartesian tRegionCartesian;
	int iOriginx, iOriginy;

	GetStringRegionCar(name, &tRegionCartesian);

	iOriginx = ptRegion->iLeftUpx + (ptRegion->iWidth - tRegionCartesian.iWidth)/2 - tRegionCartesian.iLeftUpx;
	iOriginy = ptRegion->iLeftUpy + (ptRegion->iHeight - tRegionCartesian.iHeight)/2 + tRegionCartesian.iLeftUpy;

	while(name[i]){
		tFontBitMap.iCurOriginX = iOriginx;
		tFontBitMap.iCurOriginY = iOriginy;
		error = GetFontBitMap(name[i], &tFontBitMap);
		if(error){
			printf("GetFontBitMap failed\n");
			return ;
		}

		DrawFontBitMap(&tFontBitMap,color);
		
		iOriginx = tFontBitMap.iNextOriginX;
		iOriginy = tFontBitMap.iNextOriginY;
		i++;
	}
} 
void DrawFontBitMap(pFontBitMap ptFontBitMap, unsigned int color){
	int  i, j, p, q;
	int x = ptFontBitMap->tregion.iLeftUpx;
	int y = ptFontBitMap->tregion.iLeftUpy;
    int  x_max = x + ptFontBitMap->tregion.iWidth;
    int  y_max = y + ptFontBitMap->tregion.iHeight;
	int width = ptFontBitMap->tregion.iWidth;
	unsigned char *buffer = ptFontBitMap->pucBuffer;
    //printf("x = %d, y = %d\n", x, y);

    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
        for ( i = x, p = 0; i < x_max; i++, p++ )
        {
            if ( i < 0 || j < 0 || i >= g_tDispBuff.iXres || j >= g_tDispBuff.iYres)
            continue;

			if(buffer[q * width + p]){
				PutPixel(i, j, color);
			}else{
				PutPixel(i, j, 0xffe52d);
			}
		}
	}
}
void DrawRegion(pRegion ptRegion, unsigned int color){
	int x = ptRegion->iLeftUpx;
	int y = ptRegion->iLeftUpy;
	int width = ptRegion->iWidth;
	int height = ptRegion->iHeight;
	int i, j;
	for(j = y; j < y + height; j++){
		for(i = x; i < x + width; i++){
			PutPixel(i, j, color);
		}
	}
}
void PutPixel(int x, int y, unsigned int color)
{
	unsigned char *pen_8 = (unsigned char *)(g_tDispBuff.buff+y*line_width+x*pixel_width);
	unsigned short *pen_16;	
	unsigned int *pen_32;	

	unsigned int red, green, blue;	

	pen_16 = (unsigned short *)pen_8;
	pen_32 = (unsigned int *)pen_8;

	switch (g_tDispBuff.iBpp)
	{
		case 8:
		{
			*pen_8 = color;
			break;
		}
		case 16:
		{
			/* 565 */
			red   = (color >> 16) & 0xff;
			green = (color >> 8) & 0xff;
			blue  = (color >> 0) & 0xff;
			color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pen_16 = color;
			break;
		}
		case 32:
		{
			*pen_32 = color;
			break;
		}
		default:
		{
			printf("can't surport %dbpp\n", g_tDispBuff.iBpp);
			break;
		}
	}
}

void RegisterDisplay(pDispOpr ptDispOpr){
    ptDispOpr->ptNext = g_DispDevs;
    g_DispDevs = ptDispOpr;
}

int SelectDefaultDisplay(char *name){
    pDispOpr pTmp = g_DispDevs;
    while(pTmp){
        if(strcmp(pTmp->name, name) == 0){
            g_DispDefault = pTmp;
            return 0;
        }
        pTmp = pTmp->ptNext;
    }
    return -1;
}

int InitDefaultDisplay(){
    int ret;

    ret = g_DispDefault->DeviceInit();
    if(ret){
        printf("InitDefaultDisplay failed\n");
        return -1;
    }
    ret = g_DispDefault->GetBuffer(&g_tDispBuff);
    if(ret){
        printf("GetBuffer failed\n");
        return -1;
    }
    line_width = g_tDispBuff.iXres * g_tDispBuff.iBpp / 8;
    pixel_width = g_tDispBuff.iBpp / 8;
    return 0;
}
pDispBuff GetDisplayBuffer(){
	return &g_tDispBuff;
}

int FlushDisplayRegion(pRegion ptRegion, pDispBuff ptDispBuff){
    return g_DispDefault->FlushRegion(ptRegion, ptDispBuff);

}
void DisplaySystemRegister(){
	extern void FramebufferRegister();
    FramebufferRegister();
}