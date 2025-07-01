#include <string.h>

#include "event_queue.h"


void EventQueueI_Init(tEventQueue_s *qObj)
{
    if (qObj)
    {
        qObj->qHead = 0;
        qObj->qTail = 0;
        qObj->qUsedSlot = 0;
        memset(qObj->queue, 0, sizeof(((tEventQueue_s*)0)->queue));

        pthread_mutex_init(&qObj->mutexLock, NULL);
        pthread_cond_init(&qObj->condEmpty, NULL);
    }
}


void EventQueueI_Destroy(tEventQueue_s *qObj)
{
    if (qObj)
    {
        pthread_mutex_destroy(&qObj->mutexLock);
        pthread_cond_destroy(&qObj->condEmpty);
    }
}


bool EventQueueI_Push(tEventQueue_s* qObj, const tEventData_s* inData)
{
    bool l_RetVal = false;

    if (qObj)
    {
        pthread_mutex_lock(&qObj->mutexLock);

        // If queue is not full
        if (qObj->qUsedSlot < MAX_QUEUE_EVENTS)
        {
            qObj->queue[qObj->qHead] = *inData;
            qObj->qHead = (qObj->qHead + 1) % MAX_QUEUE_EVENTS;
            qObj->qUsedSlot++;

            // Allow pop to be ran
            pthread_cond_signal(&qObj->condEmpty);

            l_RetVal = true;
        }
        else{}

        pthread_mutex_unlock(&qObj->mutexLock);
    }

    return l_RetVal;
}


bool EventQueueI_Pop(tEventQueue_s* qObj, tEventData_s* outData)
{
    bool l_RetVal = false;

    if (qObj)
    {
        pthread_mutex_lock(&qObj->mutexLock);

        // If queue is empty, wait until push is called
        while (!qObj->qUsedSlot)
        {
            pthread_cond_wait(&qObj->condEmpty, &qObj->mutexLock);
        }

        *outData = qObj->queue[qObj->qTail];
        qObj->qTail = (qObj->qTail + 1) % MAX_QUEUE_EVENTS;
        qObj->qUsedSlot--;
        l_RetVal = true;

        pthread_mutex_unlock(&qObj->mutexLock);
    }

    return l_RetVal;
}




