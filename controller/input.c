
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <linux/input.h>
#include <libgen.h>
#include <signal.h>
#include <math.h>

#include "udev_helper.h"
#include "input.h"

/*
+ l_RAxis/button mapping
+ normalization
+ deadzone filtering
+ working with Flydigi controller?
*/

static int  GamePad_Init(struct GamePad_s*);
static void GamePad_Close(struct GamePad_s*);
static int  GamePad_ReadAxis(struct GamePad_s*, tAxisState_s*);
static int  GamePad_ReadButton(struct GamePad_s*, tButtonState_s*);

static void FilterI_Init(tAxisFilter_s* axisfilter, float dz);
static void FilterI_AxisUpdate(tAxisFilter_s* axisFilter, tAxisState_s* axisState);


static int Filter_IsChange(float old, float new, float dz)
{
    return (fabs(new - old) > dz);
}

void FilterI_Init(tAxisFilter_s* axisfilter, float dz)
{
    if (axisfilter)
    {
        memset(axisfilter, 0, sizeof(tAxisFilter_s));
        axisfilter->deadzone = dz;
    }
}

void FilterI_AxisUpdate(tAxisFilter_s* axisFilter, tAxisState_s* axisState)
{
    if (Filter_IsChange(axisFilter->prevVal.lx, axisState->lx, axisFilter->deadzone))
    {
        axisFilter->prevVal.lx = axisState->lx;
    }
    else {axisState->lx = axisFilter->prevVal.lx;}

    if (Filter_IsChange(axisFilter->prevVal.ly, axisState->ly, axisFilter->deadzone))
    {
        axisFilter->prevVal.ly = axisState->ly;
    }
    else {axisState->ly = axisFilter->prevVal.ly;}

    if (Filter_IsChange(axisFilter->prevVal.rx, axisState->rx, axisFilter->deadzone))
    {
        axisFilter->prevVal.rx = axisState->rx;
    }
    else {axisState->rx = axisFilter->prevVal.rx;}

    if (Filter_IsChange(axisFilter->prevVal.ry, axisState->ry, axisFilter->deadzone))
    {
        axisFilter->prevVal.ry = axisState->ry;
    }
    else {axisState->ry = axisFilter->prevVal.ry;}

    if (Filter_IsChange(axisFilter->prevVal.lt, axisState->lt, axisFilter->deadzone))
    {
        axisFilter->prevVal.lt = axisState->lt;
    }
    else {axisState->lt = axisFilter->prevVal.lt;}

    if (Filter_IsChange(axisFilter->prevVal.rt, axisState->rt, axisFilter->deadzone))
    {
        axisFilter->prevVal.rt = axisState->rt;
    }
    else {axisState->rt = axisFilter->prevVal.rt;}
}


tGamePad_s* GamePad_Constructor()
{
    tGamePad_s *newGamePad = (tGamePad_s*)malloc(sizeof(tGamePad_s));
    if (!newGamePad)    return NULL;

    newGamePad->fileDesc = -1;
    newGamePad->init = GamePad_Init;
    newGamePad->close = GamePad_Close;
    newGamePad->readAxis = GamePad_ReadAxis;
    newGamePad->readButton = GamePad_ReadButton;

    FilterI_Init(&newGamePad->axisFilter, 0.02f);

    return newGamePad;
}

void GamePad_Destructor(tGamePad_s* gp)
{
    if (gp)
    {
        gp->close(gp);
        free(gp);
    }
}

/********************************************************************************/
/*************************** Method implementation ******************************/
/********************************************************************************/

static int  GamePad_Init(struct GamePad_s* this)
{
    char *l_devPath = (char*)malloc(PATH_MAX);
    UdevHelpI_FindGamePadPath(l_devPath);

    if (!l_devPath)
    {
        fprintf(stderr, "Cannot find gamepad!!!\n");
        return -1;
    }
    else
    {
        printf("dev path is %s\n", l_devPath);
        this->fileDesc = open(l_devPath, O_RDONLY | O_NONBLOCK);
    }
    free(l_devPath);
    return -1;
}

static void GamePad_Close(struct GamePad_s* this)
{
    if (this->fileDesc >= 0)
    {
        close(this->fileDesc);
        this->fileDesc = -1;
    }
}

static int  GamePad_ReadAxis(struct GamePad_s* this, tAxisState_s* axisState)
{
    if (this->fileDesc < 0 || !axisState)   return -1;

    struct input_event l_Ev;
    memset(axisState, 0, sizeof(tAxisState_s));

    while (read(this->fileDesc, &l_Ev, sizeof(l_Ev)) == sizeof(l_Ev))
    {
        float l_NormalizedVal = (float)l_Ev.value/32767.0f;

        if (EV_ABS ==  l_Ev.type)
        {
            switch(l_Ev.code)
            {
                case ABS_X:
                {
                    axisState->lx = l_NormalizedVal;
                    break;
                }
                case ABS_Y:
                {
                    axisState->ly = l_NormalizedVal;
                    break;
                }
                case ABS_RX:
                {
                    axisState->rx = l_NormalizedVal;
                    break;
                }
                case ABS_RY:
                {
                    axisState->ry = l_NormalizedVal;
                    break;
                }

                default: break;
            }

            FilterI_AxisUpdate(&this->axisFilter, axisState);
        }
    }

    return 1;
}

static int  GamePad_ReadButton(struct GamePad_s* this, tButtonState_s* btnState)
{
    if (this->fileDesc < 0 || !btnState)    return -1;

    struct input_event l_Ev;
    memset(btnState, 0, sizeof(tButtonState_s));

    while (read(this->fileDesc, &l_Ev, sizeof(l_Ev)) == sizeof(l_Ev))
    {
        if (EV_KEY == l_Ev.type)
        {
            switch(l_Ev.code)
            {
                case BTN_A:         {btnState->a = l_Ev.value; break;}
                case BTN_B:         {btnState->b = l_Ev.value; break;}
                case BTN_X:         {btnState->x = l_Ev.value; break;}
                case BTN_Y:         {btnState->y = l_Ev.value; break;}

                case BTN_TL:        {btnState->lbumper = l_Ev.value; break;}
                case BTN_TR:        {btnState->rbumper = l_Ev.value; break;}

                case BTN_SELECT:    {btnState->select = l_Ev.value; break;}
                case BTN_START:     {btnState->start = l_Ev.value; break;}
                case BTN_MODE:      {btnState->mode = l_Ev.value; break;}

                default: break;
            }
        }
        else if (EV_ABS == l_Ev.type)
        {
            if (ABS_HAT0X == l_Ev.code)
            {
                if (-1 == l_Ev.value)       btnState->left = 1;
                else if (1 == l_Ev.value)   btnState->right = 1;
                else{}
            }
            else if (ABS_HAT0Y == l_Ev.code)
            {
                if (-1 == l_Ev.value)       btnState->up = 1;
                else if (1 == l_Ev.value)   btnState->down = 1;
                else{}
            }
            else{}
        }
        else{}
    }

    return 1;
}

volatile sig_atomic_t stop = 0;

void handle_sigint(int sig)
{
    stop = 1;
}


int main()
{
    tAxisState_s l_RAxis;
    tButtonState_s l_RBtn;
    tGamePad_s *obj = GamePad_Constructor();

    if (!obj || !obj->init(obj))
    {
        fprintf(stderr, "Fail to initialize gamepad!!!\n");
        return 1;
    }

    printf("Begin reading data:\n");

    while(!stop)
    {
        obj->readAxis(obj, &l_RAxis);
        // obj->readButton(obj, &l_RBtn);

        printf("LX: %.2f LY: %.2f RX: %.2f RY: %.2f\n", l_RAxis.lx, l_RAxis.ly, l_RAxis.rx, l_RAxis.ry);
        // printf("Buttons: UP=%d DOWN=%d LEFT=%d RIGHT=%d\n", l_RBtn.up, l_RBtn.down, l_RBtn.left, l_RBtn.right);
        usleep(10000);  // 10 ms delay
    }

    obj->close(obj);
    GamePad_Destructor(obj);
    return 0;
}


