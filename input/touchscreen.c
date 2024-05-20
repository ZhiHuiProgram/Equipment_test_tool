#include<input_manager.h>
#include <tslib.h>
#include <stdio.h>
#include <input_manager.h>
static struct tsdev *g_ts;
static int TouchscreenGetInputEvent(pInputEvent ptInputEvent){
    
    struct ts_sample samp;
	int ret;
READ:
	ret = ts_read(g_ts, &samp, 1);
    if(ret != 1){
        printf("按键过多\n");
        goto READ;
    }
    ptInputEvent->iType = INPUT_TYPE_TOUCH;
    ptInputEvent->iX = samp.x;
    ptInputEvent->iY = samp.y;
    ptInputEvent->iPressure = samp.pressure;
    ptInputEvent->tTime = samp.tv;
    return 0;
}
static int TouchscreenDeviceInit(void){
    g_ts = ts_setup(NULL, 0);  // 设置触摸设备
	if (!g_ts)  // 若触摸设备设置失败
	{
		printf("ts_setup err\n");  // 打印错误信息
		return -1;  // 返回-1
	}
    return 0;
}
static int TouchscreenDeviceExit(void){
    ts_close(g_ts);  // 关闭触摸设备
    return 0;
}
static InputDevice g_tTouchscreenDev = {
    .name = "touchscreen",
    .GetInputEvent = TouchscreenGetInputEvent,
    .DeviceInit = TouchscreenDeviceInit,
    .DeviceExit = TouchscreenDeviceExit,
};
void TouchscreenRegister(void){
    RegisterInputDevice(&g_tTouchscreenDev);
}
#if 0
int main(void){
    InputEvent Event;
    int ret;
    g_tTouchscreenDev.DeviceInit();
    while(1){
        ret = g_tTouchscreenDev.GetInputEvent(&Event);
        if(ret){
            printf("GetInputEvent err\n");
            return -1;
        }else{
            printf("type:%d, x:%d, y:%d, pressure:%d\n", Event.iType, Event.iX, Event.iY, Event.iPressure);
        }
    }
    return 0;
}
#endif