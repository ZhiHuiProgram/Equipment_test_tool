#include <config.h>
#include <stdio.h>
#include <string.h>
static ItemCfg g_tItemCfg[MAX_ITEM_CFG_COUNT];
static int g_iItemCfgCount = 0;
int ParseConfigFile(void){
    char line[100];
    char *p = line;
    FILE *fp = fopen(CFG_FILE, "r");
    if(fp == NULL){
        printf("Open file %s failed.\n", CFG_FILE);
        return -1;
    }
    
    while(fgets(line, sizeof(line), fp)!= NULL){
        line[99] = '\0';
        p = line;
        while(*p ==' ' || *p == '\t') 
            p++;
        if(*p == '#') 
            continue;
        g_tItemCfg[g_iItemCfgCount].command[0]='\0';
        g_tItemCfg[g_iItemCfgCount].index = g_iItemCfgCount;
        sscanf(p, "%s %d %s",g_tItemCfg[g_iItemCfgCount].name,\
         &g_tItemCfg[g_iItemCfgCount].bCanBeTouch, g_tItemCfg[g_iItemCfgCount].command);
        
        g_iItemCfgCount++;
    }
    fclose(fp);
    return 0;
}

int GetItemCfgCout(void){
    return g_iItemCfgCount;
}

pItemCfg GetItemCfgByIndex(int index){
    if (index < g_iItemCfgCount){
        return &g_tItemCfg[index];
    }else{
        return NULL;
    }
}

pItemCfg GetItemCfgByName(char* name){
    int i;
    for(i=0; i<g_iItemCfgCount; i++){
        if(strcmp(g_tItemCfg[i].name, name) == 0){
            return &g_tItemCfg[i];
        }
    }
    return NULL;
}