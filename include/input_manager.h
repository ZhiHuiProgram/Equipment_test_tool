#ifndef __INPUT_MANAGER_H
#define __INPUT_MANAGER_H

#include <sys/time.h>

#define INPUT_TYPE_TOUCH 1
#define INPUT_TYPE_NET 2

typedef struct InputEvent{
	struct timeval tTime;
	int iType;
	int iX;
	int iY;
	int iPressure;
	char str[1024];
	
}InputEvent, *pInputEvent;

typedef struct InputDevice{
	char *name;
	int (*GetInputEvent)(pInputEvent);
	int (*DeviceInit)(void);
	int (*DeviceExit)(void);
	struct InputDevice *next;
}InputDevice, *pInputDevice;
void InputSystemRegister();
void RegisterInputDevice(pInputDevice ptInputDevice);
void InputEventPut(InputEvent *ptEvent);
void InputDeviceInit();
int GetInputEvent(pInputEvent ptEvent);

#endif
