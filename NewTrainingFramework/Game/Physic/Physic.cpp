#include "Physic.h"
#include "SceneManager.h"
#include "Object.h"
#include "glm/glm.hpp"
#include "Plane.h"

SceneManager *g_sceneManager;
CollisionManager *g_collisionManager;

glm::vec2 g_touchPoint;
glm::vec2 g_movePoint;
Object *g_obj;
int gWIDTH;
int gHEIGHT;
GUtils::VideoDriver *video;

glm::vec2 g_gravity = glm::vec2( 0, g_gravityAcc );

float volatile g_gameSpeed = GAME_SPEED;
bool volatile g_drawVelocity = DRAW_VELOCITY;
float volatile g_gravityAcc = GRAVITY_ACC;
double volatile gStartTime;
double volatile gEndTime;
float volatile gAccumulator = 0;
