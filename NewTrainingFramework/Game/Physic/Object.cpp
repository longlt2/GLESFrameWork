#include "Object.h"
#include "Physic.h"
#include "CollisionManager.h"

Object::Object()
{
    m_velocity = glm::vec2( 0,0 );
    m_position = glm::vec2( 0,0 );
    m_mass = 0;
    //m_acceleration = glm::vec2(0,0);
    //m_force = glm::vec2(0,0);
    type = OBJECT_TYPE_OBJECT;
    m_collision = NONE;
    m_deltaTime = FRAME_TIME;
    // m_willBeCollided = false;
}

Object::Object( int x, int y, int mass, glm::vec2 const &v, float eRate )
{
    m_position = glm::vec2( ( float )x, ( float )y );
    m_mass = mass;
    m_velocity = v;
    type = OBJECT_TYPE_OBJECT;
    m_collision = NONE;
    m_deltaTime = FRAME_TIME;
    m_elasticRate = eRate;
}

Object::Object( glm::vec2 const &p, int mass, glm::vec2 const &v )
{
    m_position = p;
    m_mass = mass;
    m_velocity = v;
    type = OBJECT_TYPE_OBJECT;
    m_collision = NONE;
    m_deltaTime = FRAME_TIME;
}

Object::Object( Object const &obj )
{
    m_position = obj.m_position;
    m_mass = obj.m_mass;
    m_velocity = obj.m_velocity;
    type = OBJECT_TYPE_OBJECT;
    m_collision = NONE;
    m_deltaTime = FRAME_TIME;
    m_elasticRate = obj.m_elasticRate;
}

Object::~Object()
{

}

void Object::Update( float frameTime )
{
    if( g_obj != this && m_deltaTime <= frameTime )
    {
        m_position += m_velocity * m_deltaTime;
        // calculate elastic velocity vector via: R= 2*(-I dot N)*N + I
        // R is the new direction vector
        // I is the old direction vector before the collision
        // N is the Normal at the collision poin
        // m_velocity = (2 * ( Dot(-m_velocity, m_collisionVec) ) * m_collisionVec + m_velocity) * ELASTIC_RATE;
        m_velocity = m_collisionVec * m_elasticRate;
        m_position += m_velocity * ( frameTime - m_deltaTime );
        m_deltaTime = frameTime * 2;
    }
    else
    {
        m_position += m_velocity * frameTime ;
    }
    m_velocity += g_gravity * frameTime;
}
