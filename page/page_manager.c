#include <page_manager.h>
#include <common.h>
#include <string.h>


static pPageManager g_ptPages = NULL;

void PageRegister(pPageManager ptPageManager){
    ptPageManager->next = g_ptPages;
    g_ptPages = ptPageManager;
}

pPageManager Page(char *name){
    pPageManager ptTmp = g_ptPages;
    while(ptTmp){
        if(strcmp(ptTmp->name, name) == 0){
            return ptTmp;
        }
        ptTmp = ptTmp->next;
    }
    return NULL;
}

void PageSystemRegister(void ){
    void MainPageRegister(void);
    MainPageRegister();
}
