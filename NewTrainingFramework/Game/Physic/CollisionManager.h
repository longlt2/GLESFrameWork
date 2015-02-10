#pragma once

#include <vector>

#include "Circle.h"
#include "Rectangle.h"

#define NONE            0
#define X_AXIS          1
#define Y_AXIS          2
#define OBJECT          4


class CollisionManager
{
private:
    static CollisionManager *sInstance;
    CollisionManager();
    virtual ~CollisionManager();

public:
    bool isCollided(Object *obj1, Object *obj2, float deltaTime=0);
    bool isCollided(Circle *obj1, Object *obj2, float deltaTime=0);
    bool isCollided(Rect *obj1, Object *obj2, float deltaTime=0);
    bool isCollided(Plane *obj1, Object *obj2, float deltaTime=0, float offset=0);

    bool isCollided(Circle *obj1, Rect *obj2, float deltaTime=0);
    bool isCollided(Circle *obj1, Circle *obj2, float deltaTime=0);
    // bool isCollided(Circle *obj1, Plane *obj2, float deltaTime=0);
    bool isCollided(Rect *obj1, Rect *obj2, float deltaTime=0);
    // bool isCollided(Rect *obj1, Plane *obj2, float deltaTime=0);
	void Collided(Object *obj1, Object* obj2, float deltaTime);
    static CollisionManager *GetInstance(void);
};
