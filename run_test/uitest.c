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
#include <ui.h>
#include <font_manager.h>


int main(int argc, char **argv)
{
	Button tButton;
	Region tRegion;
    pDispBuff ptBuff;
	int error;
	if(argc < 2){
		printf("Usage: %s font_path\n", argv[0]);
		return -1;
	}
	DisplaySystemRegister();
	SelectDefaultDisplay("fb");
	InitDefaultDisplay();

	ptBuff = GetDisplayBuffer();

	FontSystemRegister();
	error = SelectAndInitFont("freetype",argv[1]);
	if(error){
		printf("SelectAndInitFont failed\n");
		return -1;
	}

	tRegion.iLeftUpy = 100;
	tRegion.iLeftUpx = 100;
	tRegion.iHeight = 400;
	tRegion.iWidth = 400;
	InitButton("what can i say", &tButton,&tRegion, NULL, NULL);
	tButton.OnDraw(&tButton, ptBuff);
	while(1){
		tButton.OnTouch(&tButton, ptBuff,NULL);
		usleep(800000);
	}
	return 0;	
}

