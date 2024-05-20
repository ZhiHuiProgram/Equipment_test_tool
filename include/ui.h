#ifndef UI_H
#define UI_H
#include <common.h>
#include <disp_manager.h>
#include <input_manager.h>

#define BUTTON_DEFAULT_COLOR 0xff0000
#define BUTTON_PRESSED_COLOR 0x00FF00
#define BUTTON_PERCENT_COLOR 0x0000FF
#define BUTTON_TEXT_COLOR 0x000000

struct Button;
typedef int (*ONDRAW_FUNC)(struct Button *pbutton, pDispBuff ptDispBuff);
typedef int (*ONTOUCH_FUNC)(struct Button *pbutton, pDispBuff ptDispBuff, pInputEvent ptInputEvent);
typedef struct Button{
    char *name;
    int status;
    int iFontSize;
    Region tRegion;
    ONDRAW_FUNC OnDraw;
    ONTOUCH_FUNC OnTouch;
}Button,*pButton;
void InitButton(char *name,pButton ptButton, pRegion ptRegion,ONDRAW_FUNC onDraw, ONTOUCH_FUNC onTouch);

#endif