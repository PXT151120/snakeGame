#include <string.h>
#include <stdio.h>
#include "event_queue.h"


void EventQueueI_Init(tEventQueue_s* qObj)
{
    if (!qObj)  return;
    else
    {
        memset(qObj->buffer, 0, sizeof(((tEventQueue_s*)0)->buffer));

        qObj->qHead = 0;
        qObj->qTail = 0;

        pthread_mutex_init(&qObj->mutexLock, NULL);
        pthread_cond_init(&qObj->condEmpty, NULL);
    }
}

bool EventQueueI_Push(tEventQueue_s* qObj, const tInputEvent_s* inEvent)
{
    bool l_RetVal = false;
    if (qObj)
    {
        int l_Next = (qObj->qHead + 1) % QUEUE_MAX_EVENTS;

        pthread_mutex_lock(&qObj->mutexLock);

        // Queue is full
        if (l_Next == qObj->qTail)
        {
            pthread_mutex_unlock(&qObj->mutexLock);
        }
        else
        {
            printf("[Push] Signaling data ready\n");
            qObj->buffer[qObj->qHead] = *inEvent;
            qObj->qHead = l_Next;
            pthread_cond_signal(&qObj->condEmpty);
            pthread_mutex_unlock(&qObj->mutexLock);
            l_RetVal = true;
        }
    }

    return l_RetVal;
}

bool EventQueueI_Pop(tEventQueue_s* qObj, tInputEvent_s* outEvent)
{
    bool l_RetVal = false;

    if (qObj)
    {
        pthread_mutex_lock(&qObj->mutexLock);

        // Queue is empty, wait
        while (qObj->qHead == qObj->qTail)
        {
            printf("[Pop] Waiting...\n");
            pthread_cond_wait(&qObj->condEmpty, &qObj->mutexLock);
        }

        *outEvent = qObj->buffer[qObj->qTail];
        qObj->qTail = (qObj->qTail + 1) % QUEUE_MAX_EVENTS;

        pthread_mutex_unlock(&qObj->mutexLock);
        l_RetVal = true;
    }

    return l_RetVal;
}

void EventQueueI_Destroy(tEventQueue_s* qObj)
{
    pthread_mutex_destroy(&qObj->mutexLock);
    pthread_cond_destroy(&qObj->condEmpty);
}




