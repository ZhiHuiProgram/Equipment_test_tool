#include <page_manager.h>
#include <stdio.h>
#include <config.h>
#include <ui.h>
#include <math.h>
#include <disp_manager.h>
#include <string.h>
#include <common.h>
#include <stdlib.h>
#include <unistd.h>
#include <font_manager.h>
#define X_GAP 5
#define Y_GAP 5
static Button g_tButton[MAX_ITEM_CFG_COUNT];
static int g_tButtonCnt;

static int MainPageOnPressed(Button *ptButton, pDispBuff ptDispBuff, pInputEvent ptInputEvent){
    unsigned int color = BUTTON_DEFAULT_COLOR;
    char name[100];
    char status[100];
    char command[1000];
    char *strButton;
    char *command_status[3] = {"err","ok","percent"};
    int command_status_index = 0;
    pItemCfg ptItemCfg; 

    strButton = ptButton->name;

    if(ptInputEvent->iType == INPUT_TYPE_TOUCH){
        if(GetItemCfgByName(ptButton->name)->bCanBeTouch==0){
            return -1;
        }
        if(ptInputEvent->iPressure == 0){
        ptButton->status = !ptButton->status;
        }
        if(ptButton->status){
            color = BUTTON_PRESSED_COLOR;
            command_status_index = 1;
        }
        strButton = ptButton->name;
    }else if(ptInputEvent->iType == INPUT_TYPE_NET){

        sscanf(ptInputEvent->str, "%s %s", name, status);
        if(strcmp(status,"ok")==0){
            command_status_index = 1;
            color = BUTTON_PRESSED_COLOR;
        }else if(strcmp(status,"err")==0){
            command_status_index = 0;
            color = BUTTON_DEFAULT_COLOR;
        }else if(status[0] >= '0'&& status[0] <= '9'){
            command_status_index = 2;
            color = BUTTON_PERCENT_COLOR;
            strButton = status;
        }
    }else{
        return -1;
    }
    DrawRegion(&ptButton->tRegion,color);

    DrawTextInRegionCenter(strButton,&ptButton->tRegion,BUTTON_TEXT_COLOR);

    FlushDisplayRegion(&ptButton->tRegion, ptDispBuff);

    ptItemCfg = GetItemCfgByName(ptButton->name);
    if(ptItemCfg->command[0] != '\0'){
        sprintf(command,"%s %s",ptItemCfg->command, command_status[command_status_index]);
        system(command);
    }
    return 0;
}
static int GetFontSizeForAllButton(){
    int i;
    int max_len = -1;
    int max_index = -1;
    int len;
    float k,kx,ky;
    RegionCartesian tRegionCar;
    for (i=0; i<g_tButtonCnt; i++){
        len = strlen(g_tButton[i].name);
        if(len > max_len){
            max_len = len;
            max_index = i;
        }
        
    }

    SetFontSize(100);
    GetStringRegionCar(g_tButton[max_index].name, &tRegionCar);

    kx = (float)g_tButton[max_index].tRegion.iWidth / tRegionCar.iWidth;
    ky = (float)g_tButton[max_index].tRegion.iHeight / tRegionCar.iHeight;
    k = kx < ky ? kx : ky;

    return k*80;
}
static void GenerateButtons(void){
    int width,height;
    int n_per_line;
    int n;
    int row,col;
    int rows;
    int xres,yres;
    int start_x,start_y;
    int pre_start_x,pre_start_y;
    int i=0;
    int iFontSize;
    g_tButtonCnt = n = GetItemCfgCout();
    pDispBuff ptDispBuff;
    pButton ptButton;
    ptDispBuff = GetDisplayBuffer();
    xres = ptDispBuff->iXres;
    yres = ptDispBuff->iYres;

    width = sqrt(1.0/0.618*xres*yres/n);
    n_per_line = xres/width+1;
    width = xres/ n_per_line;
    height = width*0.618;
    start_x = (xres - width*n_per_line)/2;
    rows = n/n_per_line;
    if(rows*n_per_line < n){
        rows++;
    }
    start_y = (yres - height*rows)/2;
    for(row=0; row<rows && i<n; row++){
        pre_start_y = start_y + height*row;
        pre_start_x = start_x - width;
        for(col=0; col<n_per_line && i<n; col++){
            if(i==n){
                break;
            }
            ptButton = &g_tButton[i];
            ptButton->tRegion.iLeftUpx = pre_start_x + width;
            ptButton->tRegion.iLeftUpy = pre_start_y ;
            ptButton->tRegion.iWidth = width - X_GAP;
            ptButton->tRegion.iHeight = height - Y_GAP;
            pre_start_x = ptButton->tRegion.iLeftUpx;
            InitButton(GetItemCfgByIndex(i)->name,ptButton,NULL,NULL,MainPageOnPressed);
            i++;
        }
        
    }
    iFontSize = GetFontSizeForAllButton();
//    SetFontSize(iFontSize);
    for (i=0; i<n; i++){
            g_tButton[i].iFontSize = iFontSize;
            g_tButton[i].OnDraw(&g_tButton[i],ptDispBuff);
            
            printf("nihao2\n");
    }
}
static int  isTouchPointInRegion(int iX, int iY, pRegion ptRegion){
    if(iX<ptRegion->iLeftUpx || iX>ptRegion->iLeftUpx+ptRegion->iWidth || iY<ptRegion->iLeftUpy || iY>ptRegion->iLeftUpy+ptRegion->iHeight){
        return 0;
    }
    return 1;
}

static pButton GetButtonByName(char *name){
    int i;
    for(i=0; i<g_tButtonCnt; i++){
        if(strcmp(name,g_tButton[i].name)==0)
            return &g_tButton[i];
    }
    return NULL;
}
static pButton GetButtonInputEvent(InputEvent *ptInputEvent){
    int i;
    char name[100];
    if(ptInputEvent->iType == INPUT_TYPE_NET){
        sscanf(ptInputEvent->str, "%s", name);
        return GetButtonByName(name);
    }else if(ptInputEvent->iType == INPUT_TYPE_TOUCH){
        for(i=0; i<g_tButtonCnt; i++){
            if(isTouchPointInRegion(ptInputEvent->iX,ptInputEvent->iY,&g_tButton[i].tRegion))
                return &g_tButton[i];
        }
    }else{
        return NULL;
    }
    return NULL;
}

static void MainPageRun(void *pParams)
{   
    int error;
    InputEvent tInputEvent;
    pButton ptButton;
    pDispBuff ptDisBuff = GetDisplayBuffer();
    error = ParseConfigFile();
    if(error){
        printf("Parse config file failed\n");
        return;
    }
    GenerateButtons();
    while(1){
        error = GetInputEvent(&tInputEvent);
        if(error)
            continue;
        ptButton = GetButtonInputEvent(&tInputEvent);
        if(!ptButton)
            continue;
        ptButton->OnTouch(ptButton,ptDisBuff, &tInputEvent);
    }
}
static PageManager g_tMainPage = {

   .name = "main",
   .Run  = MainPageRun,
};

void MainPageRegister(void)
{
    PageRegister(&g_tMainPage);
}