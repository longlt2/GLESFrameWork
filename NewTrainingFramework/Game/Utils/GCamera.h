#ifndef GCAMERA_H
#define GCAMERA_H

#include <stdio.h>
#include <string>
#include <string.h>

#include <GLES3/gl3.h>
#include <EGL/egl.h>

#include <IL/il.h>

#include <glm/glm.hpp>

#include "Shaders.h"

#define DF_CAM_FOV              (60.f)
#define DF_CAM_SCALE            (4.f/3.f)
#define DF_CAM_NEAR             (.1f)
#define DF_CAM_FAR              (100.f)

typedef enum _eLocationType_t
{
    eLocationTypeUniform,
    eLocationTypeAttribute,
} eLocationType_t;

typedef enum _eCameraKey_t
{
    eCameraKeyNone = 0,
    eCameraKeyUp = 1,
    eCameraKeyDown = 2,
    eCameraKeyRight = 4,
    eCameraKeyLeft = 8,
} eCameraKey_t;

class GCamera
{
public:
    GCamera();
    ~GCamera();

    void Update(glm::vec2 const &mousePos);
    void Init(float fFoV=DF_CAM_FOV, float fScale=DF_CAM_SCALE, float fNear=DF_CAM_NEAR, float fFar=DF_CAM_FAR);

    void SetFoV(float fFoV);
    float GetFoV(void) const;
    void SetScale(float fScale);
    float GetScale(void) const;
    void SetNear(float fNear);
    float GetNear(void) const;
    void SetFar(float fFar);
    float GetFar(void) const;

    void SetDirection(glm::vec2 const &mousePos);
    eCameraKey_t KeyPressed(eCameraKey_t key);
    eCameraKey_t KeyReleased(eCameraKey_t key);
    void UpdateMatrix(float frameTime);

    glm::mat4 const &GetProjection(void) const;

    glm::mat4 const &GetView(void) const;

    glm::vec3 mPosition;
    glm::vec3 mDirection;
    glm::vec3 mRight;
    glm::vec3 mUp;

    float mHoriAngle;
    float mVertAngle;
    float mFoVAngle;
    float mMouseSpeed;
    float mSpeed;

    float mScale;

    float mNear;
    float mFar;

    eCameraKey_t mKey;

    glm::mat4 mModel;
    glm::mat4 mView;
    glm::mat4 mProj;
};

#endif