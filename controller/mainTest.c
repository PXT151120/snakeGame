#include <unistd.h>
#include <stdio.h>

#include "event_queue.h"


bool isRunning = true;

void* inputThread(void *arg)
{
    tEventQueue_s* l_Qctx = (tEventQueue_s*)arg;
    int l_Tick = 0;

    while(isRunning)
    {
        tEventData_s l_Ev;

        l_Ev.eType = EVENT_AXIS;
        l_Ev.axisData.lx = (l_Tick % 100) / 100.0f;
        l_Ev.axisData.ly = (l_Tick % 50) / 50.0f;
        l_Ev.axisData.rx = (l_Tick % 100) / 100.0f;
        l_Ev.axisData.ry = (l_Tick % 50) / 50.0f;

        if (EventQueueI_Push(l_Qctx, &l_Ev))
        {
            printf("[inputThread]: Push lx: %.2f, ly: %.2f, rx: %.2f, ry: %.2f\n", l_Ev.axisData.lx, l_Ev.axisData.ly, l_Ev.axisData.rx, l_Ev.axisData.ry);
        }
        else
        {
            printf("[inputThread]: Queue is full\n");
        }

        l_Tick++;
        usleep(500000);
    }

    return NULL;
}



int main()
{
    tEventQueue_s qObj;
    pthread_t l_Tid;


    EventQueueI_Init(&qObj);

    pthread_create (&l_Tid,
			        NULL,
			        inputThread,
			        &qObj);

    sleep(6);

    for (int i = 0; i < 5; i++)
    {
        tEventData_s l_Ev;
        if (EventQueueI_Pop(&qObj, &l_Ev))
        {
            printf("[main]: Pop lx: %.2f, ly: %.2f, rx: %.2f, ry: %.2f\n", l_Ev.axisData.lx, l_Ev.axisData.ly, l_Ev.axisData.rx, l_Ev.axisData.ry);
        }
        else
        {
            printf("[main]: Queue is full\n");
        }
    }

    isRunning = false;

    pthread_join(l_Tid, NULL);
    EventQueueI_Destroy(&qObj);

    return 0;
}

