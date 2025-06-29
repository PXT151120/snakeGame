#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "input.h"


#define QUEUE_MAX_NUMBER 64


typedef enum
{
    EVENT_AXIS = 0,
    EVENT_BTN_PRESSED,
    EVENT_BTN_RELEASED
} tEvenType_e;


typedef struct
{
    tEvenType_e eType;
    union
    {
        tAxisState_s axisState;
        struct
        {
            int btnCode;
            int isPressed;
        };
    };
} tInputEvent_s;

typedef struct
{
    tInputEvent_s buffer[QUEUE_MAX_NUMBER];
    int qHead;
    int qTail;

} tEventQueue_s;


void EventQueueI_Init(tEventQueue_s* qObj);
void EventQueueI_Push(tEventQueue_s* qObj, const tInputEvent_s* inEvent);
void EventQueueI_Pop(tEventQueue_s* qObj, tInputEvent_s* outEvent);
void EventQueueI_Destroy(tEventQueue_s* qObj);

#endif
