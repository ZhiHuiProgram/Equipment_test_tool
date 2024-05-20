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

int main(int argc, char **argv){
	int error;
//初始化显示系统
	DisplaySystemRegister();
	SelectDefaultDisplay("fb");
	InitDefaultDisplay();
//初始化字库
//初始化输入系统
	InputSystemRegister();
	InputDeviceInit();
	FontSystemRegister();
	error = SelectAndInitFont("freetype",argv[1]);
	if(error){
		printf("SelectAndInitFont failed\n");
		return -1;
	}
	PageSystemRegister();
//运行业务系统
	Page("main")->Run(NULL);

	
	return 0;	
}

