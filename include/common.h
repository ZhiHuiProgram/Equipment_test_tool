#ifndef COMMON_H
#define COMMON_H
#ifndef NULL
#define NULL (void*)0
#endif
typedef struct Region{
	int iLeftUpx;
	int iLeftUpy;
	int iWidth;
	int iHeight;
}Region,*pRegion;

typedef struct RegionCartesian{
	int iLeftUpx;
	int iLeftUpy;
	int iWidth;
	int iHeight;
}RegionCartesian,*pRegionCartesian;
#endif