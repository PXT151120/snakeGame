#ifndef INPUT_H
#define INPUT_H

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct
{
    float lx, ly, rx, ry;
    float lt, rt;
} tAxisState_s;

typedef struct
{
    int up, down, left, right;
    int a, b, x, y;
    int lbumper, rbumper;
    int start, select, mode;
} tButtonState_s;

/* Reference type*/
struct GamePad_s;
struct AxisFilter_s;


typedef struct AxisFilter_s
{
    tAxisState_s prevVal;
    float deadzone;
} tAxisFilter_s;


typedef struct GamePad_s
{
    //Properties
    int fileDesc;
    struct AxisFilter_s axisFilter;

    // Methods
    int (*init)(struct GamePad_s*);
    void (*close)(struct GamePad_s*);
    int (*readAxis)(struct GamePad_s*, tAxisState_s*);
    int (*readButton)(struct GamePad_s*, tButtonState_s*);
} tGamePad_s;

tGamePad_s* GamePad_Constructor(void);
void        GamePad_Destructor(struct GamePad_s*);

#ifdef __cplusplus
}
#endif

#endif
