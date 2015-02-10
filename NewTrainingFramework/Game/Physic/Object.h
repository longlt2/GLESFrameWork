#pragma once
#include "VideoDriver.h"
#include "glm\glm.hpp"
#include "Physic.h"

using namespace glm;

class Object
{
private:

protected:
    int m_mass;
    glm::vec2 m_velocity;
    glm::vec2 m_touchedVec;
    glm::vec2 m_position;
    glm::vec2 m_normal;
    int type;

    glm::vec2 m_collisionPoint;
    glm::vec2 m_collisionVec;

    char m_collision;
    float m_deltaTime;
    float m_elasticRate;


public:
    Object();
    Object(int x, int y, int mass, glm::vec2 const &v, float eRate=ELASTIC_RATE);
    Object(glm::vec2 const &p, int mass, glm::vec2 const &v);
    Object(Object const &obj);
    virtual ~Object();
    virtual void Update(float frameTime);
    virtual void Render(GUtils::VideoDriver *video, float prediction = 0) const = 0;
    virtual bool isInside(glm::vec2 const &p) const = 0;

    friend class SceneManager;
    friend class CollisionManager;
};
