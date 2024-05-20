#ifndef __DISP_MANAGER_H
#define __DISP_MANAGER_H

#include "common.h"

typedef struct DispBuff{
	int iXres;
	int iYres;
	int iBpp;
	char *buff;
}DispBuff,*pDispBuff;

typedef struct DispOpr{
	char *name;
	int (*DeviceInit)(void);
	int (*DeviceExit)(void);
	int (*GetBuffer)(pDispBuff ptDispBuff);
	int (*FlushRegion)(pRegion ptRegion, pDispBuff ptDispBuff);
	struct DispOpr *ptNext;
}DispOpr, *pDispOpr;

void RegisterDisplay(pDispOpr ptDispOpr);
void PutPixel(int x, int y, unsigned int color);
int SelectDefaultDisplay(char *name);
void DrawRegion(pRegion ptRegion, unsigned int color);
int InitDefaultDisplay();
void DrawTextInRegionCenter(char *name,pRegion ptRegion, unsigned int color);
void DisplaySystemRegister();
int FlushDisplayRegion(pRegion ptRegion, pDispBuff ptDispBuff);
pDispBuff GetDisplayBuffer();
#endif
