#ifndef __DEFINE_H__
#define __DEFINE_H__

#if (defined __ANDROID__)
#define SCREEN_W            1024.f
#else
#define SCREEN_W            768.f
#endif

#define SCREEN_H            768.f
#define SCREEN_CENTER_W     (SCREEN_W/2)
#define SCREEN_CENTER_H     (SCREEN_H/2)
#define LIMIT_FPS           30

    // define variable
#define MAX_LENGTH          255

#define CENTER(x)           (x/2)

    // Macro
#define PI                  3.14159265f
#define HALFPI              (PI/2.0f)
#define SAFE_DEL(x)         {if(x){delete x; x = NULL;}}

// #define X2GAME(x)           ((1.0f*x - SCREEN_CENTER_W)/SCREEN_CENTER_W)
// #define Y2GAME(y)           (-1.0f*(1.0f*y - SCREEN_CENTER_H)/SCREEN_CENTER_H)

#define X2GAME(x,w)           ((1.0f*x - CENTER(w))/CENTER(w))
#define Y2GAME(y,h)           (-1.0f*(1.0f*y - CENTER(h))/CENTER(h))

typedef enum _eGameReturn_t
{
    eGameReturnOk = 0,
    eGameReturnErr = (1 << 1),
    eGameReturnNull = (1 << 2),
    eGameReturnMax = (1 << 3),
} eGameReturn_t;

#endif