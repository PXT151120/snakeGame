#include <math.h>
#include <string.h>
#include "filter.h"


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
