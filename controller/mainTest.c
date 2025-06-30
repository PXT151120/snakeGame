#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#include "event_queue.h"


typedef struct
{
    tEventQueue_s *queue;
    bool running;
} tInputContex_s;


void* input_thread(void *arg)
{
    tInputContex_s *l_Ctx = (tInputContex_s*)arg;
    int l_Tick = 0;

    while(l_Ctx->running)
    {
        tInputEvent_s l_Ev;

        l_Ev.eType = EVENT_AXIS;
        l_Ev.axisState.lx = (l_Tick % 100) / 100.0f;
        l_Ev.axisState.ly = (l_Tick % 50) / 50.0f;
        l_Ev.axisState.rx = (l_Tick % 100) / 100.0f;
        l_Ev.axisState.ry = (l_Tick % 50) / 50.0f;

        if (!EventQueueI_Push(l_Ctx->queue, &l_Ev))
        {
            printf("[InputThread] Queue full, dropping event.\n");
        }
        else
        {
            printf("[InputThread] Pushed event: lx=%.2f ly=%.2f\n", l_Ev.axisState.lx, l_Ev.axisState.ly);
            printf("[InputThread] Pushed event: rx=%.2f ry=%.2f\n", l_Ev.axisState.rx, l_Ev.axisState.ry);
        }

        l_Tick++;
        usleep(500000);
    }

    return NULL;
}

int main()
{
    tEventQueue_s l_Queue;
    tInputContex_s l_InputCtx = {.queue = &l_Queue, .running = true};

    EventQueueI_Init(l_InputCtx.queue);

    pthread_t l_Tid;

    if (pthread_create(&l_Tid, NULL, input_thread, &l_InputCtx))
    {
        perror("Create input thread failed\n");
        return 1;
    }

    for (int i = 0; i < 5; i++)
    {
        tInputEvent_s l_Ev;
        EventQueueI_Pop(l_InputCtx.queue, &l_Ev);

        if (l_Ev.eType == EVENT_AXIS)
        {
            printf("[Main] Popped event: lx=%.2f ly=%.2f\n", l_Ev.axisState.lx, l_Ev.axisState.ly);
            printf("[Main] Popped event: rx=%.2f ry=%.2f\n", l_Ev.axisState.rx, l_Ev.axisState.ry);
        }
    }

    l_InputCtx.running = false;
    pthread_join(l_Tid, NULL);
    EventQueueI_Destroy(l_InputCtx.queue);
    return 0;
}

