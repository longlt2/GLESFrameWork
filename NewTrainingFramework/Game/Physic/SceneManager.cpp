#include <stdio.h>
#include <fstream>

#if (defined __ANDROID__)
#include <unistd.h>
#endif

#include "VideoDriver.h"

#include "SceneManager.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Physic.h"
#include "Plane.h"

SceneManager* SceneManager::sInstance = NULL;

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{
    Object *obj;
    while( m_ObjVector.empty() != true )

    {
        obj = m_ObjVector.back();
        m_ObjVector.pop_back();
        if( obj )
        {
            delete obj;
        }
    }

    if( sInstance != 0 )
    {
        delete sInstance;
        sInstance = 0;
    }
}

void SceneManager::Update( float frameTime )
{
    char rs = 0;
    if( g_obj )
    {
        g_obj->m_velocity = ( g_movePoint - ( g_obj->m_position + g_obj->m_touchedVec ) );
    }

    for( size_t i=4; i<m_ObjVector.size(); i++ )
    {
        if( m_ObjVector[i] )
        {
            m_ObjVector[i]->Update( frameTime );

            for( size_t j=0; j<4; j++ )
            {
                CollisionManager::GetInstance()->isCollided( m_ObjVector[i], m_ObjVector[j], frameTime );
            }

            for( size_t j=i+1; j<m_ObjVector.size(); j++ )
            {
                if( CollisionManager::GetInstance()->isCollided( m_ObjVector[i], m_ObjVector[j], frameTime ) == true )
                {
                    // float deltaTtime = frameTime / COLLISION_STEP;

                    for( float deltaTtime = 0; deltaTtime < frameTime; deltaTtime += frameTime / COLLISION_STEP )
                    {
                        bool rs = CollisionManager::GetInstance()->isCollided( m_ObjVector[i], m_ObjVector[j], deltaTtime );
                        if( rs == true )
                        {
                            break;
                        }
                    }

                    // while(CollisionManager::GetInstance()->isCollided(m_ObjVector[i], m_ObjVector[j], deltaTtime) == false)
                    // {
                    //     deltaTtime += frameTime / COLLISION_STEP;
                    // }
                }
            }
        }
    }
}

void SceneManager::Render( GUtils::VideoDriver *video ) const
{
    if( !video )
    {
        return;
    }

    for( std::vector<Object *>::const_iterator it = m_ObjVector.begin(); it != m_ObjVector.end(); it++ )
    {
        if( *it )
        {
            ( *it )->Render( video );
            if( g_drawVelocity == true )
            {
                video->DrawLine( ( glm::vec2 )( *it )->m_position, ( glm::vec2 )( ( *it )->m_position + ( *it )->m_velocity ) );
            }
        }
    }
}

void SceneManager::Render( GUtils::VideoDriver *video, float prediction ) const
{
    if( !video )
    {
        return;
    }

    for( std::vector<Object *>::const_iterator it = m_ObjVector.begin(); it != m_ObjVector.end(); it++ )
    {
        if( *it )
        {
            ( *it )->Render( video, prediction );
            if( g_drawVelocity == true )
            {
                video->DrawLine( ( glm::vec2 )( *it )->m_position, ( glm::vec2 )( ( *it )->m_position + ( *it )->m_velocity ) );
            }
        }
    }
}

void SceneManager::AddObject( Object *obj )
{
    m_ObjVector.push_back( obj );
}

int SceneManager::Init( char const* url )
{

    // add border planes.
    m_ObjVector.push_back( new Plane( PLANE_TOP_POSITION, PLANE_TOP_NORMAL, PLANE_TOP_VELOCITY ) );

    m_ObjVector.push_back( new Plane( PLANE_BOTTOM_POSITION, PLANE_BOTTOM_NORMAL, PLANE_BOTTOM_VELOCITY ) );

    m_ObjVector.push_back( new Plane( PLANE_LEFT_POSITION, PLANE_LEFT_NORMAL, PLANE_LEFT_VELOCITY ));

    m_ObjVector.push_back( new Plane( PLANE_RIGHT_POSITION, PLANE_RIGHT_NORMAL, PLANE_RIGHT_VELOCITY ) );

    if( !url )
    {
        return 1;
    }

    FILE *file;
    file = fopen( url, "r" );
    if( !file )
    {
        return 1;
    }

    rewind( file );
    int type = OBJECT_TYPE_MAX;
    int posX, posY, width, height, radius, vecX, vecY, mass;
    Object *obj;

    while( !feof( file ) )
    {
        fscanf( file, "%d", &type );
        switch( type )
        {
            case OBJECT_TYPE_CIRCLE:
            {
                fscanf( file, "%d", &posX );
                fscanf( file, "%d", &posY );
                fscanf( file, "%d", &mass );
                fscanf( file, "%d", &vecX );
                fscanf( file, "%d", &vecY );
                fscanf( file, "%d", &radius );
                obj = new Circle( posX, posY, mass, glm::vec2( ( float )vecX, ( float )vecY ), radius );
                break;
            }

            case OBJECT_TYPE_RECT:
            {
                fscanf( file, "%d", &posX );
                fscanf( file, "%d", &posY );
                fscanf( file, "%d", &mass );
                fscanf( file, "%d", &vecX );
                fscanf( file, "%d", &vecY );
                fscanf( file, "%d", &width );
                fscanf( file, "%d", &height );
                obj = new Rect( posX, posY, mass, glm::vec2( ( float )vecX, ( float )vecY ), width, height );
                break;
            }

            default:
            {
                obj = 0;
                break;
            }
        }

        if( obj )
        {
            m_ObjVector.push_back( obj );
        }
    }

    fclose( file );

    return 0;
}

Object * const & SceneManager::getTouched( float x, float y ) const
{
    return this->getTouched( glm::vec2( x,y ) );
}

Object * const & SceneManager::getTouched( glm::vec2 const &p ) const
{
    for( std::vector<Object *>::const_iterator it = m_ObjVector.begin(); it != m_ObjVector.end(); it++ )
    {
        if( ( *it ) && ( *it )->isInside( p ) )
        {
            return ( *it );
        }
    }

    return 0;
}

void SceneManager::touchActionDown( float x, float y )
{
    g_movePoint = glm::vec2( x, y );
    g_obj = this->getTouched( g_movePoint );
    if( g_obj )
    {
        g_obj->m_touchedVec = g_movePoint - g_obj->m_position;
    }
}

void SceneManager::touchActionMove( float x, float y )
{
    g_movePoint = glm::vec2( x, y );
    if( g_obj )
    {
        g_obj->m_velocity = ( g_movePoint - ( g_obj->m_position + g_obj->m_touchedVec ) );
    }
}

void SceneManager::touchActionUp( float x, float y )
{

    g_obj = 0;

}

SceneManager * SceneManager::GetInstance( void )
{
    if( sInstance == 0 )
    {
        sInstance = new SceneManager();
        // sInstance->m_ObjVector.resize(10);
    }

    return sInstance;
}
