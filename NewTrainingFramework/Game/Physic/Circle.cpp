#include "Circle.h"
#include "Physic.h"
#include "VideoDriver.h"

Circle::Circle() : Object()
{
    m_radius = 0;
    type = OBJECT_TYPE_CIRCLE;
}

Circle::Circle( int radius )
{
    m_radius = radius;
    type = OBJECT_TYPE_CIRCLE;
}

Circle::Circle( int x, int y, int radius ) : Object( x, y, 0 , glm::vec2( 0,0 ) )
{
    m_radius = radius;
    type = OBJECT_TYPE_CIRCLE;
}

Circle::Circle( int x, int y, int mass, glm::vec2 const & velocity, int radius ) : Object( x, y, mass , velocity )
{
    m_radius = radius;
    type = OBJECT_TYPE_CIRCLE;
}

Circle::Circle( glm::vec2 const &p, int radius ) : Object( p, 0, glm::vec2( 0,0 ) )
{
    m_radius = radius;
    type = OBJECT_TYPE_CIRCLE;
}

Circle::Circle( glm::vec2 const &p, int mass, glm::vec2 const & velocity, int radius ) : Object( p, mass, velocity )
{
    m_radius = radius;
    type = OBJECT_TYPE_CIRCLE;
}

Circle::Circle( Circle const &cl )
{
    m_position = cl.m_position;
    m_radius = cl.m_radius;
    type = OBJECT_TYPE_CIRCLE;
}

Circle::~Circle()
{

}

void Circle::Update( float frameTime )
{

    Object::Update( frameTime );

    if( ( m_position.x < m_radius ) )
    {
        m_position.x = ( float )m_radius;
    }
    else if( ( m_position.x + m_radius ) > gWIDTH )
    {
        m_position.x = ( float )( gWIDTH - m_radius );
    }

    if( ( m_position.y < m_radius ) )
    {
        m_position.y = ( float )m_radius;
    }
    else if( ( m_position.y + m_radius ) > gHEIGHT )
    {
        m_position.y = ( float )( gHEIGHT - m_radius );
    }
}

bool Circle::isInside( glm::vec2 const &p ) const
{
    double rs = sqrt( ( ( p.x - m_position.x ) * ( p.x - m_position.x ) ) + ( ( p.y - m_position.y ) * ( p.y - m_position.y ) ) ) * ODD_RATE;

    if( rs > m_radius )
    {
        return false;
    }

    return true;
}

void Circle::Render( GUtils::VideoDriver *video, float prediction ) const
{
    video->DrawCircle( m_position + m_velocity * prediction, m_radius );
}
