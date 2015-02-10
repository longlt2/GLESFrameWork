#include "Plane.h"
#include "Physic.h"

Plane::Plane() : Object()
{

}

Plane::~Plane()
{

}

Plane::Plane( glm::vec2 const &p, glm::vec2 const &n, glm::vec2 const &v )
{
    m_position = p;
    m_normal = n;
    m_velocity = v;
    type = OBJECT_TYPE_PLANE;
}

Plane::Plane( Plane const &p )
{
    m_position = p.m_position;
    m_normal = p.m_normal;
    m_velocity = p.m_velocity;
    type = p.type;
}

void Plane::Render( GUtils::VideoDriver *video, float prediction ) const
{
    // video->DrawLine( m_position, m_velocity );
    // video->DrawRect(m_position.x, m_position.y, m_velocity.x, m_velocity.y,2);
    // TRACE_FUNCL("%f-%f",m_position.x, m_position.y);
}

bool Plane::isInside( glm::vec2 const &p ) const
{
    return false;
}

void Plane::Update( float frameTime )
{

}
