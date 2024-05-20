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
#include <page_manager.h>
#include <ui.h>
#include <font_manager.h>


int main(int argc, char **argv)
{
	PagesRegister();
	Page("main")->Run(NULL);
	return 0;	
}

