#include "Rectangle.h"
#include "Physic.h"
#include "TextureManager.h"

Rect::Rect() : Object()
{
    m_width = 0;
    m_height = 0;
    type = OBJECT_TYPE_RECT;
}

Rect::Rect( int x, int y, int w, int h ) : Object( x, y, 0, glm::vec2( 0,0 ) )
{
    m_width = w;
    m_height = h;
    type = OBJECT_TYPE_RECT;
}

Rect::Rect( int x, int y, int mass, glm::vec2 const & velocity, int w, int h ) : Object( x, y, mass, velocity )
{
    m_width = w;
    m_height = h;
    type = OBJECT_TYPE_RECT;
}

Rect::Rect( glm::vec2 const &p, int w, int h ) : Object( p, 0 ,glm::vec2( 0,0 ) )
{
    m_width = w;
    m_height = h;
    type = OBJECT_TYPE_RECT;
}

Rect::Rect( glm::vec2 const &p, int mass, glm::vec2 const & velocity, int w, int h ) : Object( p, mass, velocity )
{
    m_width = w;
    m_height = h;
    type = OBJECT_TYPE_RECT;
}

Rect::Rect( Rect const &rt )
{
    m_position = rt.m_position;
    m_mass = rt.m_mass;
    m_velocity = rt.m_velocity;
    m_width = rt.m_width;
    m_height = rt.m_height;
    type = OBJECT_TYPE_RECT;
}

Rect::~Rect()
{

}

void Rect::Update( float frameTime )
{
    Object::Update( frameTime );

    if( ( m_position.x < 0 ) )
    {
        m_position.x = 0;
    }
    else if( ( m_position.x + m_width ) > gWIDTH )
    {
        m_position.x = ( float )( gWIDTH - m_width );
    }

    if( ( m_position.y < 0 ) )
    {
        m_position.y = 0;
    }
    else if( ( m_position.y + m_height ) > gHEIGHT )
    {
        m_position.y = ( float )( gHEIGHT - m_height );
    }
}

void Rect::Render( GUtils::VideoDriver *video, float prediction ) const
{
    //video->DrawCustom( ( int )( m_position.x + m_velocity.x * prediction ), ( int )( m_position.y + m_velocity.y * prediction ), m_width, m_height, TextureManager::GetInstance()->TextureID() );
}

bool Rect::isInside( glm::vec2 const &p ) const
{
    // check is outside
    if( p.x < ( m_position.x * ODD_RATE ) ||
        p.y < ( m_position.y * ODD_RATE ) ||
        ( p.x * ODD_RATE ) > ( m_position.x + m_width ) ||
        ( p.y * ODD_RATE ) > ( m_position.y + m_height ) )
    {
        return false;
    }
    return true;
}
