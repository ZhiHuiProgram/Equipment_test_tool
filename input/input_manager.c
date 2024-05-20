#include <input_manager.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <stdlib.h>

#define INPUT_EVENT_SIZE 20
static pInputDevice g_InputDevice;

static pthread_mutex_t g_tMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_tCond = PTHREAD_COND_INITIALIZER;

static int g_iRead =0;
static int g_iWrite =0;
static InputEvent g_InputEvents[INPUT_EVENT_SIZE];

static int InputBufferEmpty(){
    return (g_iRead == g_iWrite);
}


static int InputBufferFull(){
    return ((g_iWrite + 1) % INPUT_EVENT_SIZE == g_iRead);
}

static int PutInputEventToBuffer(InputEvent *ptEvent){
    if(!InputBufferFull()){
        g_InputEvents[g_iWrite] = *ptEvent;
        g_iWrite = (g_iWrite + 1) % INPUT_EVENT_SIZE;
        return 0;
    }
    printf("buffer满了等会\n");
    return 0;
}

static int GetInputEventFromBuffer(pInputEvent ptEvent){
    if (!InputBufferEmpty())
    {
        *ptEvent = g_InputEvents[g_iRead];
        g_iRead = (g_iRead + 1) % INPUT_EVENT_SIZE;
        return 1;
    }
    return 0;
}

void InputEventPut(InputEvent *ptEvent){
    pInputDevice pHead = g_InputDevice;
    while(pHead){
        PutInputEventToBuffer(ptEvent);
        pHead = pHead->next;
    }
}
void RegisterInputDevice(pInputDevice ptInputDevice){
    ptInputDevice->next = g_InputDevice;
    g_InputDevice = ptInputDevice;
}

void InputSystemRegister(){
    extern void NetinputRegister();
    NetinputRegister();
    extern void TouchscreenRegister();
    TouchscreenRegister();

}

static void *InputRecvThread(void *arg){
    pInputDevice ptInputDev = (pInputDevice)arg;
    InputEvent tEvent;
    int ret;
    while(1){
        ret = ptInputDev->GetInputEvent(&tEvent);
        if(!ret){

            pthread_mutex_lock(&g_tMutex);
            PutInputEventToBuffer(&tEvent);

            pthread_cond_signal(&g_tCond);
            pthread_mutex_unlock(&g_tMutex);
        }
    }
    return NULL;
}
void InputDeviceInit(){
    int ret;
    pthread_t thread;
    pInputDevice pHead = g_InputDevice;
    while(pHead){
        ret = pHead->DeviceInit();
        if(!ret){
            ret = pthread_create(&thread, NULL, InputRecvThread, (void*)pHead);
        }

        pHead = pHead->next;
    }
}
int GetInputEvent(pInputEvent ptEvent){
    pInputEvent pEvent;
    pEvent = (pInputEvent)malloc(sizeof(InputEvent));
    int ret ;
    pthread_mutex_lock(&g_tMutex);
    if(GetInputEventFromBuffer(pEvent)){
        printf("有数据了\n");
        *ptEvent = *pEvent;
        pthread_mutex_unlock(&g_tMutex);
        return 0;
    }else{
        pthread_cond_wait(&g_tCond, &g_tMutex);
        if(GetInputEventFromBuffer(pEvent)){
        //    printf("test:type :%d, ix:%d, iy:%d, pressure:%d\n",pEvent->iType,pEvent->iX, pEvent->iY, pEvent->iPressure);
        //    printf("没数据等会\n");
           *ptEvent = *pEvent;
            ret = 0 ;
        }else{
            printf("失败了\n");
            ret = -1;
        }
        pthread_mutex_unlock(&g_tMutex);
    }
    free(pEvent);
    return ret;
}