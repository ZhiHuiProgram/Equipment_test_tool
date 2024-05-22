#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(){ 
	int n = -1; 
	FILE *strm; 
	char buf[128]={0}; 
	sprintf(buf,"ps a | grep %s|grep -v grep", "main"); 

	if((strm = popen(buf, "r")) != NULL) 
	{ 
		if(fgets(buf, sizeof(buf), strm) != NULL) 
		{
			printf("buf=%s\n",buf);
			n = atoi(buf); 
			printf("n=%d\n",n);
		}
	}
	else
	{
		return -1;
	} 
	char buffer[20];
	sprintf(buffer,"kill -9 %d",n);
	system(buffer);
	system("/mnt/main simsun.ttc");
	pclose(strm); 
	return n; 
}
