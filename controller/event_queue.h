#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <stdbool.h>
#include <pthread.h>
#include "input.h"

#define MAX_QUEUE_EVENTS   10

typedef enum
{
    EVENT_AXIS = 0,
    EVENT_BTN_PRESSED,
    EVENT_BTN_RELEASED
} tEventType_e;


typedef struct
{
    tEventType_e eType;
    union
    {
        tAxisState_s    axisData;
        tButtonState_s  btnData;
    };
} tEventData_s;


typedef struct
{
    tEventData_s queue[MAX_QUEUE_EVENTS];
    int qTail;
    int qHead;
    int qUsedSlot;

    pthread_mutex_t mutexLock;
    pthread_cond_t  condEmpty;

} tEventQueue_s;


void EventQueueI_Init(tEventQueue_s *qObj);
void EventQueueI_Destroy(tEventQueue_s *qObj);
bool EventQueueI_Push(tEventQueue_s* qObj, const tEventData_s* inData);
bool EventQueueI_Pop(tEventQueue_s* qObj, tEventData_s* outData);

#endif
