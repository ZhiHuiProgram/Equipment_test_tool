#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <disp_manager.h>
#include <stdlib.h>
#include <input_manager.h>

int main(int argc, char **argv)
{
	int ret;
	InputEvent event;
//	pevent = (pInputEvent)malloc(sizeof(InputEvent));

    InputInit();
	InputDeviceInit();
	while(1){
		ret=GetInputEvent(&event);
//		printf("pevent:%X\n", event);
		if(ret!=0){
			printf("No input event\n");
			return -1;
		}else{
			if(event.iType == INPUT_TYPE_TOUCH){
				printf("main :type :%d, ix:%d, iy:%d, pressure:%d\n", 
				event.iType,event.iX, event.iY, event.iPressure);
			}
		}
	}
	return 0;	
}

