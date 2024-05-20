#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

typedef struct PageManager{
    char *name;
    void (*Run)(void * pParams);
    struct PageManager *next;
}PageManager,*pPageManager;
void PageRegister(pPageManager ptPageManager);
pPageManager Page(char *name);

void PageSystemRegister(void );
#endif