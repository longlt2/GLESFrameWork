#include <stdio.h>

#include "CollisionManager.h"
#include "Physic.h"
#include "Plane.h"

CollisionManager *CollisionManager::sInstance = 0;


void CollisionManager::Collided( Object *obj1, Object* obj2, float deltaTime )
{
    glm::vec2 xaxis, u1x, u1y, u2x, u2y, v1x, v1y, v2x, v2y;
    float a;

    if( obj1->m_deltaTime <= deltaTime && obj2->m_deltaTime <= deltaTime )
    {
        return;
    }

    xaxis = glm::normalize( obj2->m_position - obj1->m_position );
    a = glm::dot( xaxis, obj1->m_velocity );
    u1x = xaxis * a;
    u1y = obj1->m_velocity - u1x;

    xaxis *= -1;
    a = glm::dot( xaxis, obj2->m_velocity );
    u2x = xaxis * a;
    u2y = obj2->m_velocity - u2x;

    v1x = ( ( u1x * ( 1.0f * obj1->m_mass ) ) + ( u2x * ( 1.0f * obj2->m_mass ) ) - ( ( u1x - u2x )*( 1.0f * obj2->m_mass ) ) ) / ( ( 1.0f * obj1->m_mass ) + ( 1.0f * obj2->m_mass ) );
    v1y = u1y;

    if( obj1->m_deltaTime > deltaTime )
    {
        obj1->m_deltaTime = deltaTime;
        obj1->m_collisionVec = v1x + v1y;
    }

    v2x = ( ( u2x * ( 1.0f * obj2->m_mass ) ) + ( u1x * ( 1.0f * obj1->m_mass ) ) - ( ( u2x - u1x )*( 1.0f * obj1->m_mass ) ) ) / ( ( 1.0f * obj2->m_mass ) + ( 1.0f * obj1->m_mass ) );
    v2y = u2y;

    if( obj2->m_deltaTime > deltaTime )
    {
        obj2->m_deltaTime = deltaTime;
        obj2->m_collisionVec = v2x + v2y;
    }
}

CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{
    if( sInstance )
    {
        delete sInstance;
        sInstance = 0;
    }
}

CollisionManager * CollisionManager::GetInstance( void )
{
    if( sInstance == 0 )
    {
        sInstance = new CollisionManager();
    }
    return sInstance;
}

bool CollisionManager::isCollided( Object *obj1, Object *obj2, float deltaTime )
{
    switch( obj1->type )
    {
        case OBJECT_TYPE_CIRCLE:
        {
            return isCollided( ( Circle* )obj1, obj2, deltaTime );
        }
        break;

        case OBJECT_TYPE_RECT:
        {
            return isCollided( ( Rect* )obj1, obj2, deltaTime );
        }
        break;

        case OBJECT_TYPE_PLANE:
        {
            return isCollided( obj2, ( Plane* )obj1, deltaTime );
        }
        break;
    }
    return false;
}

bool CollisionManager::isCollided( Circle *obj1, Object *obj2, float deltaTime )
{
    switch( obj2->type )
    {
        case OBJECT_TYPE_CIRCLE:
        {
            return isCollided( obj1, ( Circle* )obj2, deltaTime );
        }
        break;

        case OBJECT_TYPE_RECT:
        {
            return isCollided( obj1, ( Rect* )obj2, deltaTime );
        }
        break;

        case OBJECT_TYPE_PLANE:
        {
            return isCollided( ( Plane* )obj2, obj1, deltaTime, ( float )( obj1->m_radius ) );
        }
        break;
    }
    return false;
}

bool CollisionManager::isCollided( Rect *obj1, Object *obj2, float deltaTime )
{
    switch( obj2->type )
    {
        case OBJECT_TYPE_CIRCLE:
        {
            return isCollided( obj1, ( Circle* )obj2, deltaTime );
        }
        break;

        case OBJECT_TYPE_RECT:
        {
            return isCollided( obj1, ( Rect* )obj2, deltaTime );
        }
        break;

        case OBJECT_TYPE_PLANE:
        {
            int offset = 0;
            glm::vec2 nm = ( ( Plane* )obj2 )->m_normal;
            if( nm.x + nm.y > 0 )
            {
                offset = 0;
            }
            else if( nm.x < 0 )
            {
                offset = obj1->m_width;
            }
            else
            {
                offset = obj1->m_height;
            }

            return isCollided( ( Plane* )obj2, obj1, deltaTime, ( float )( offset ) );
        }
        break;
    }
    return false;
}

// Collison between Plane & Obj
bool CollisionManager::isCollided( Plane *obj1, Object *obj2, float deltaTime, float offset )
{
    // obj2->m_deltaTime = deltaTime * 2;
    if( obj2->type == OBJECT_TYPE_PLANE )
    {
        return false;
    }

    glm::vec2 uv = glm::normalize( obj2->m_velocity );
    glm::vec2 nm = obj1->m_normal;
    float dDotProduct = glm::dot( uv, obj1->m_normal );
    float dCollisionDistance;
    float dCollisionTime;

    // Determine If Ray Parallel To Plane
    if( ( dDotProduct < ZERO ) && ( dDotProduct > -ZERO ) )
    {
        return false;
    }

    // Find Distance To Collision Point
    dCollisionDistance = ( ( glm::dot( obj1->m_normal, obj1->m_position + obj1->m_normal * offset - obj2->m_position ) ) / dDotProduct );

    // Test If Collision Behind Start
    if( dCollisionDistance < -ZERO )
    {
        return false;
    }

    float dDistance = glm::length( obj2->m_velocity * deltaTime );
    // time collision happens
    dCollisionTime = dCollisionDistance / dDistance * deltaTime;

    // check collision happens at next frame.
    if( dCollisionTime > deltaTime )
    {
        return false;
    }

    if( obj2->m_deltaTime >= dCollisionTime )
    {
        // collision time.
        obj2->m_deltaTime = dCollisionTime;
        // collision normal vector.
        // obj2->m_collisionVec = obj1->m_normal;

        // calculate elastic velocity vector via: R= 2*(-I dot N)*N + I
        // R is the new direction vector
        // I is the old direction vector before the collision
        // N is the Normal at the collision poin
        obj2->m_collisionVec = ( 2 * ( glm::dot( -( obj2->m_velocity ), obj1->m_normal ) ) * obj1->m_normal + obj2->m_velocity );

    }

    return true;
}

// Collison between Circle & Rect
bool CollisionManager::isCollided( Circle *obj1, Rect *obj2, float deltaTime )
{
    //Closest point on collision box
    int cX, cY;
    double circleX, circleY, rectX, rectY;

    circleX = obj1->m_position.x + ( obj1->m_velocity.x * deltaTime );
    circleY = obj1->m_position.y + ( obj1->m_velocity.y * deltaTime );
    rectX = obj2->m_position.x + ( obj2->m_velocity.x * deltaTime );
    rectY = obj2->m_position.y + ( obj2->m_velocity.y * deltaTime );

    //Find closest x offset
    if( circleX < rectX )
    {
        cX = ( int )( rectX );
    }
    else if( circleX > ( rectX + obj2->m_width ) )
    {
        cX = ( int )( rectX + obj2->m_width );
    }
    else
    {
        cX = ( int )( circleX );
    }
    //Find closest y offset
    if( circleY < rectY )
    {
        cY = ( int )( rectY );
    }
    else if( circleY > ( rectY + obj2->m_height ) )
    {
        cY = ( int )( rectY + obj2->m_height );
    }
    else
    {
        cY = ( int )( circleY );
    }

    //If the closest point is inside the circle
    if(
        ( ( circleX - cX ) * ( circleX - cX ) + ( circleY - cY ) * ( circleY - cY ) )
        <= obj1->m_radius * obj1->m_radius
    )
    {
        Collided( obj1, obj2, deltaTime );
        // glm::vec2 uv;

        // //This box and the circle have collided
        // if(cX == obj2->m_position.x)
        // {
        //     uv = glm::vec2(-1, 0);
        // }
        // else if(cX == obj2->m_position.x + obj2->m_width)
        // {
        //     uv = glm::vec2(1, 0);
        // }
        // else if(cY == obj2->m_position.y)
        // {
        //     uv = glm::vec2(0, -1);
        // }
        // else if(cY == obj2->m_position.y + obj2->m_height)
        // {
        //     uv = glm::vec2(0, 1);
        // }
        // else
        // {
        //     uv = obj1->m_velocity.Unit();
        // }

        // if(obj2->m_deltaTime > deltaTime)
        // {
        //     obj2->m_deltaTime = deltaTime;
        //     obj2->m_collisionVec = uv;
        // }

        // if(obj1->m_deltaTime > deltaTime)
        // {
        //     obj1->m_deltaTime = deltaTime;
        //     obj1->m_collisionVec = -uv;
        // }

        return true;
    }

    return false;
}

// Collison between Circle & Circle
bool CollisionManager::isCollided( Circle *obj1, Circle *obj2, float deltaTime )
{
    double circle1X = obj1->m_position.x + ( obj1->m_velocity.x * deltaTime );
    double circle1Y = obj1->m_position.y + ( obj1->m_velocity.y * deltaTime );

    double circle2X = obj2->m_position.x + ( obj2->m_velocity.x * deltaTime );
    double circle2Y = obj2->m_position.y + ( obj2->m_velocity.y * deltaTime );

    if( glm::length( glm::vec2( circle1X , circle1Y ) - glm::vec2( circle2X, circle2Y ) ) <= ( obj1->m_radius + obj2->m_radius ) )
    {
        // glm::vec2 uv = glm::vec2(obj1->m_position - obj2->m_position).Unit();
        Collided( obj1, obj2, deltaTime );

        // if(obj2->m_deltaTime > deltaTime)
        // {
        //     obj2->m_deltaTime = deltaTime;
        //     // if(Dot(uv, obj2->m_collisionVec) > 0)
        //     // {
        //     //     obj2->m_collisionVec = obj2->m_velocity.Unit();
        //     // }
        //     // else
        //     // {
        //         obj2->m_collisionVec = uv;
        //         // obj2->m_collisionVec = obj1->m_collisionVec;
        //     // }
        // }

        // if(obj1->m_deltaTime > deltaTime)
        // {
        //     obj1->m_deltaTime = deltaTime;
        //     // obj1->m_collisionVec = -uv;
        //     // if(Dot(uv, obj1->m_collisionVec) > 0)
        //     // {
        //     //     obj1->m_collisionVec = obj1->m_velocity.Unit();
        //     // }
        //     // else
        //     // {
        //         obj1->m_collisionVec = -uv;
        //         // obj1->m_collisionVec = obj2->m_collisionVec;
        //     // }
        // }

        return true;
    }

    return false;
}

// Collison between Rect & Rect
bool CollisionManager::isCollided( Rect *obj1, Rect *obj2, float deltaTime )
{
    double rect1X = obj1->m_position.x + ( obj1->m_velocity.x * deltaTime );
    double rect1Y = obj1->m_position.y + ( obj1->m_velocity.y * deltaTime );

    double rect2X = obj2->m_position.x + ( obj2->m_velocity.x * deltaTime );
    double rect2Y = obj2->m_position.y + ( obj2->m_velocity.y * deltaTime );

    if( ( rect1X < rect2X ) &&
        ( ( rect1X + obj1->m_width ) < rect2X ) )
    {
        return false;
    }
    else if( ( rect2X < rect1X ) &&
             ( ( rect2X + obj2->m_width ) < rect1X ) )
    {
        return false;
    }

    if( ( rect1Y < rect2Y ) &&
        ( ( rect1Y + obj1->m_width ) < rect2Y ) )
    {
        return false;
    }
    else if( ( rect2Y < rect1Y ) &&
             ( ( rect2Y + obj2->m_width ) < rect1Y ) )
    {
        return false;
    }

    glm::vec2 uv = glm::normalize( obj1->m_velocity );

    if( obj2->m_deltaTime > deltaTime )
    {
        obj2->m_deltaTime = deltaTime;
        obj2->m_collisionVec = uv;
    }

    if( obj1->m_deltaTime > deltaTime )
    {
        obj1->m_deltaTime = deltaTime;
        obj1->m_collisionVec = -uv;
    }

    return true;
}
