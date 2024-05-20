#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <disp_manager.h>
#include <font_manager.h>
#include <input_manager.h>
#include <page_manager.h>
#include <config.h>
#include <common.h>
#include <ui.h>
int main(int argc, char **argv)
{
	return 0;
}

int main(int argc, char **argv){
    Region region;
    pDispBuff ptBuff;
	int error;
	if(argc < 2){
		printf("Usage: %s font_file_path string\n", argv[0]);
	}
//初始化显示系统
	DisplayInit();
	SelectDefaultDisplay("fb");
	InitDefaultDisplay();
//初始化字库
	ptBuff = GetDisplayBuffer();

//初始化输入系统
	InputInit();
	InputDeviceInit();

	FontsRegister();

	error = SelectAndInitFont("freetype",argv[1]);
	if(error){
		printf("SelectAndInitFont failed\n");
		return -1;
	}
	PagesRegister();
//运行业务系统
	Page("main")->Run(NULL);
	
	return 0;	
}

