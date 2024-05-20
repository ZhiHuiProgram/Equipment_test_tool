#include <ui.h>
#include <disp_manager.h>
#include <input_manager.h>
#include <font_manager.h>
#include <stdio.h>

static int DefaultDrawButton(pButton ptButton,pDispBuff ptDispBuff){
    DrawRegion(&(ptButton->tRegion),BUTTON_DEFAULT_COLOR);
    SetFontSize(ptButton->iFontSize);
    DrawTextInRegionCenter(ptButton->name,&ptButton->tRegion,BUTTON_TEXT_COLOR);
    FlushDisplayRegion(&ptButton->tRegion, ptDispBuff);
    return 0;
}

static int DefaultTouchButton(Button *ptButton, pDispBuff ptDispBuff, pInputEvent ptInputEvent){
    unsigned int color = BUTTON_DEFAULT_COLOR;
    ptButton->status = !ptButton->status;
    if(ptButton->status)
        color = BUTTON_PRESSED_COLOR;
    DrawRegion(&ptButton->tRegion,color);

    DrawTextInRegionCenter(ptButton->name,&ptButton->tRegion,BUTTON_TEXT_COLOR);

    FlushDisplayRegion(&ptButton->tRegion, ptDispBuff);
    return 0;
}


void InitButton(char *name,pButton ptButton, pRegion ptRegion,ONDRAW_FUNC onDraw, ONTOUCH_FUNC onTouch){
    ptButton->status = 0;
    ptButton->name = name;
    if(ptRegion){
        ptButton->tRegion = *ptRegion;
    }
    ptButton->OnDraw = onDraw? onDraw : DefaultDrawButton;
    ptButton->OnTouch = onTouch? onTouch : DefaultTouchButton;
}