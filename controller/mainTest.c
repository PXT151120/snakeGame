#include <stdio.h>
#include <signal.h>

#include "input.h"
#include "event_queue.h"



// volatile sig_atomic_t stop = 0;

// void handle_sigint(int sig)
// {
//     stop = 1;
// }

// int main()
// {
//     tAxisState_s l_RAxis;
//     tButtonState_s l_RBtn;
//     tGamePad_s *obj = GamePad_Constructor();

//     if (!obj || !obj->init(obj))
//     {
//         fprintf(stderr, "Fail to initialize gamepad!!!\n");
//         return 1;
//     }

//     printf("Begin reading data:\n");

//     while(!stop)
//     {
//         obj->readAxis(obj, &l_RAxis);
//         // obj->readButton(obj, &l_RBtn);

//         printf("LX: %.2f LY: %.2f RX: %.2f RY: %.2f\n", l_RAxis.lx, l_RAxis.ly, l_RAxis.rx, l_RAxis.ry);
//         // printf("Buttons: UP=%d DOWN=%d LEFT=%d RIGHT=%d\n", l_RBtn.up, l_RBtn.down, l_RBtn.left, l_RBtn.right);
//         usleep(10000);  // 10 ms delay
//     }

//     obj->close(obj);
//     GamePad_Destructor(obj);
//     return 0;
// }







