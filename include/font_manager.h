#ifndef __FONT_MANAGER_H
#define __FONT_MANAGER_H
#include <disp_manager.h>

typedef struct FontBitMap{
	Region tregion;
	int iCurOriginX;
	int iCurOriginY;
	int iNextOriginX;
	int iNextOriginY;
	unsigned char *pucBuffer;
}FontBitMap,*pFontBitMap;

typedef struct FontOpr{
	char *name;
	int (*FontInit)(char *aFileName);
	int (*SetFontSize)(int iFontSize);
	int (*GetFontBitMap)(unsigned int dwCode, pFontBitMap pBitMap);
	int (*GetStringRegionCar)(char *str, pRegionCartesian ptRegionCartesian);
	struct FontOpr *pNext;
}FontOpr, *pFontOpr;
void RegisterFont(pFontOpr ptFontOpr);
void FontSystemRegister();
int SetFontSize(int iFontSize);
int GetFontBitMap(unsigned int dwCode, pFontBitMap ptFontBitMap);
void DrawFontBitMap(pFontBitMap ptFontBitMap, unsigned int color);
int SelectAndInitFont(char *aFontOprName, char *aFontFileName);
int GetStringRegionCar(char *str, pRegionCartesian ptRegionCartesian);
#endif
