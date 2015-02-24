#ifndef PHYSIC_H
#define PHYSIC_H
#include "glm/glm.hpp"
#include "Define.h"
#include "VideoDriver.h"
#include "utilities.h"
#include <stdio.h>

#if (defined __ANDROID__)
#include "jni_base.h"
#include "config_Android.h"
#undef SCREEN_W
#define SCREEN_W                        1280

#else
#define LOGI(...)
#define SD_FOLDER                       "."
#endif

#ifndef SCREEN_H
#define SCREEN_H                        720
#endif

#if (defined LIMIT_FPS)
#undef LIMIT_FPS
#define LIMIT_FPS                       50
#endif

#define FRAME_TIME                      (1000.0f/LIMIT_FPS)

#define EPSILON 1.0e-8
#define ZERO EPSILON

#define GRAVITY_ACC                     80.0f
#define ACCUMULATOR_RATE                0.1f
#define DRAW_VELOCITY                   false
#define GAME_SPEED                      1.5f
#define MOUSE_FORCE_MULTI_CONST         1

#define COLLISION_STEP                  100

#define ODD_RATE                        0.9f
#define ELASTIC_RATE                    1.0f
#define FRICTION_RATE                   0.1f

#define DATA_URL                        SD_FOLDER"/Data.txt"

#define TRACE_FULL(fmt, args)  printf(" + %s (%s) [%d]: " fmt"\n",__FILE__,__FUNCTION__,__LINE__, ##args)
#define TRACE_FUNCL(fmt, args)  printf(" - (%s) [%d]: " fmt"\n",__FUNCTION__,__LINE__, ##args)
#define TRACE_FILEL(fmt, args)  printf(" - (%s) [%d]: " fmt"\n",__FILE__,__LINE__, ##args)

#define TRACE()                 printf("%s %d\n", __FUNCTION__, __LINE__)
#define TRACEINT(x)             printf("%s %d %d\n", __FUNCTION__, __LINE__, x)
#define TRACEINT2(x,y)          printf("%s %d %d %d\n", __FUNCTION__, __LINE__, x, y)
#define TRACEFLT(x)             printf("%s %d %f\n", __FUNCTION__, __LINE__, x)
#define TRACEFLT2(x,y)          printf("%s %d %f %f\n", __FUNCTION__, __LINE__, x, y)
#define TRACESTR(x)             printf("%s %d %s\n", __FUNCTION__, __LINE__, x)


class GUtils::VideoDriver;
class SceneManager;
class CollisionManager;
class Object;
class Plane;

enum OBJECT_TYPE
{
    OBJECT_TYPE_OBJECT,
    OBJECT_TYPE_CIRCLE,
    OBJECT_TYPE_RECT,
    OBJECT_TYPE_PLANE,
    OBJECT_TYPE_MAX,
};

#define PLANE_TOP_POSITION              glm::vec2(0, 0)
#define PLANE_TOP_NORMAL                glm::vec2(0, 1)
#define PLANE_TOP_VELOCITY              glm::vec2(SCREEN_W, 1)

#define PLANE_BOTTOM_POSITION           glm::vec2(0, SCREEN_H)
#define PLANE_BOTTOM_NORMAL             glm::vec2(0, -1)
#define PLANE_BOTTOM_VELOCITY           glm::vec2(SCREEN_W, SCREEN_H - 1)

#define PLANE_LEFT_POSITION             glm::vec2(0, 0)
#define PLANE_LEFT_NORMAL               glm::vec2(1, 0)
#define PLANE_LEFT_VELOCITY             glm::vec2(1, SCREEN_H)

#define PLANE_RIGHT_POSITION            glm::vec2(SCREEN_W, 0)
#define PLANE_RIGHT_NORMAL              glm::vec2(-1, 0)
#define PLANE_RIGHT_VELOCITY            glm::vec2(SCREEN_W, SCREEN_H - 1)

#define PLANE_TYPE_TOP           (1 >> 0)
#define PLANE_TYPE_BOTTOM        (PLANE_TYPE_TOP >> 1)
#define PLANE_TYPE_LEFT          (PLANE_TYPE_TOP >> 2)
#define PLANE_TYPE_RIGHT         (PLANE_TYPE_TOP >> 3)
#define PLANE_TYPE_COUNT         4

extern SceneManager *g_sceneManager;
extern CollisionManager *g_collisionManager;
extern glm::vec2 g_touchPoint;
extern glm::vec2 g_movePoint;
extern Object *g_obj;
extern int gWIDTH;
extern int gHEIGHT;
extern GUtils::VideoDriver *video;
extern glm::vec2 g_gravity;

extern float volatile g_gameSpeed;
extern bool volatile g_drawVelocity;
extern float volatile g_gravityAcc;
extern double volatile gStartTime;
extern double volatile gEndTime;
extern float volatile gAccumulator;

// extern Plane const g_planes[];

#endif
