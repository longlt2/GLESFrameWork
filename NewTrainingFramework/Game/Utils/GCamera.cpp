#include <assert.h>

// glm::translate, glm::rotate, glm::scale
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/constants.hpp>

#include "TextureManager.h"
#include <iostream>
#include "Define.h"
#include "Physic.h"

#include "GCamera.h"

using namespace std;

static float sScale = 0;
static float sRotate = 0;
glm::mat4 m4Model(1);
glm::mat4 m4View;
glm::mat4 m4Proj;

GCamera::GCamera()
{
    Init();
}

GCamera::~GCamera()
{

}

void GCamera::Init(float fFoV, float fScale, float fNear, float fFar)
{
    assert(fScale > 1);
    assert(fNear < fFar);

    mFoVAngle = fFoV;
    mScale = fScale;
    mNear = fNear;
    mFar = fFar;

    mHoriAngle = PI;
    mVertAngle = 0.0f;

    mPosition = glm::vec3( 0, 2, 3 );
    SetDirection(glm::vec2(0));

    mMouseSpeed = 0.00005f;
    mSpeed = 0.0005f;

    mModel = glm::mat4(1);
	mView = glm::mat4(1);
	mProj = glm::mat4(1);

    mKey = eCameraKeyNone;
}

void GCamera::SetDirection(glm::vec2 const &horiVertAngl)
{
    mHoriAngle += mMouseSpeed * horiVertAngl.x;
    mVertAngle += mMouseSpeed * horiVertAngl.y;

    mDirection = glm::vec3(
        cos(mVertAngle) * sin(mHoriAngle),
        sin(mVertAngle),
        cos(mVertAngle) * cos(mHoriAngle)
    );

    mRight = glm::vec3(
        sin(mHoriAngle - HALFPI),
        0,
        cos(mHoriAngle - HALFPI)
    );

    mUp = glm::cross( mRight, mDirection );
}

eCameraKey_t GCamera::KeyPressed(eCameraKey_t key)
{
	mKey = (eCameraKey_t)(mKey | key);
    return mKey;
}

eCameraKey_t GCamera::KeyReleased(eCameraKey_t key)
{
	mKey = (eCameraKey_t)(mKey & (~key));
    return mKey;
}

void GCamera::UpdateMatrix(float frameTime)
{
    if(mKey & eCameraKeyUp)
    {
        mPosition += mDirection * mSpeed * frameTime;
    }
    if(mKey & eCameraKeyDown)
    {
        mPosition -= mDirection * mSpeed * frameTime;
    }
    if(mKey & eCameraKeyRight)
    {
        mPosition += mRight * mSpeed * frameTime;
    }
    if(mKey & eCameraKeyLeft)
    {
        mPosition -= mRight * mSpeed * frameTime;
    }

    mProj = glm::perspective<glm::lowp_float>(PI * mFoVAngle / 180.f, mScale, mNear, mFar);
    mView = glm::lookAt( mPosition, mPosition + mDirection, mUp );
}

glm::mat4 const &GCamera::GetProjection(void) const
{
    return mProj;
}

glm::mat4 const &GCamera::GetView(void) const
{
    return mView;
}

void GCamera::SetFoV(float fFoV)
{
    mFoVAngle = fFoV;
}
float GCamera::GetFoV(void) const
{
    return mFoVAngle;
}
void GCamera::SetScale(float fScale)
{
    mScale = fScale;
}
float GCamera::GetScale(void) const
{
    return mScale;
}
void GCamera::SetNear(float fNear)
{
    mNear = fNear;
}
float GCamera::GetNear(void) const
{
    return mNear;
}
void GCamera::SetFar(float fFar)
{
    mFar = fFar;
}
float GCamera::GetFar(void) const
{
    return mFar;
}