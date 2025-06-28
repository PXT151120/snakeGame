#ifndef FILTER_H
#define FILTER_H

#include "input.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    tAxisState_s prevVal;
    float deadzone;
} tAxisFilter_s;

extern void FilterI_Init(tAxisFilter_s*, float);
extern int FilterI_AxisUpdate(tAxisFilter_s*, tAxisState_s*);

#ifdef __cplusplus
}
#endif

#endif
