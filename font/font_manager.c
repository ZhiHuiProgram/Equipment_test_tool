#include <font_manager.h>
#include <stdio.h>
#include <string.h>
static pFontOpr g_ptFonts = NULL;
static pFontOpr g_ptDefaultFontOpr = NULL;
void RegisterFont(pFontOpr ptFontOpr){
    ptFontOpr->pNext = g_ptFonts;
    g_ptFonts = ptFontOpr;
}
void FontSystemRegister(){
    extern void FreetypeRegister();
    FreetypeRegister();
}
int SelectAndInitFont(char *aFontOprName, char *aFontFileName){
    pFontOpr ptTmp = g_ptFonts;
    while(ptTmp!= NULL){
        if(strcmp(ptTmp->name, aFontOprName) == 0){
            break;
        }
        ptTmp = ptTmp->pNext;
    }
    if(!ptTmp){
        return -1;
    }
    g_ptDefaultFontOpr = ptTmp;
    return ptTmp->FontInit(aFontFileName);
}
int SetFontSize(int iFontSize){
    return g_ptDefaultFontOpr->SetFontSize(iFontSize);
}
int GetFontBitMap(unsigned int dwCode, pFontBitMap ptFontBitMap){
    return g_ptDefaultFontOpr->GetFontBitMap(dwCode, ptFontBitMap);
}
int GetStringRegionCar(char *str, pRegionCartesian ptRegionCartesian){
    return g_ptDefaultFontOpr->GetStringRegionCar(str, ptRegionCartesian);
}