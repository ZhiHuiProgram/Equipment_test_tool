#ifndef CONFIG_H
#define CONFIG_H

#include <common.h>

#define MAX_ITEM_CFG_COUNT 30
#define CFG_FILE "/etc/test_gui/gui.conf"

typedef struct ItemCfg{
    int index;
    char name[100];
    int bCanBeTouch;
    char command[100];
}ItemCfg, *pItemCfg;

int ParseConfigFile(void);
int GetItemCfgCout(void);
pItemCfg GetItemCfgByIndex(int index);
pItemCfg GetItemCfgByName(char* name);

#endif