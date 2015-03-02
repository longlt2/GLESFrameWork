
#ifndef CUBE_H
#define CUBE_H

#include <stdio.h>
#include <string>
#include <string.h>

#include <GLES3/gl3.h>
#include <EGL/egl.h>

#include <IL/il.h>

#include <glm/glm.hpp>

#include "Shaders.h"

class Cube
{
public:
    Cube();
    ~Cube();

    void Init(char *vs, char *fs);
    void Draw(void) const;


    glm::vec3 mVertices[8];
    glm::vec2 mTexCoord[4];
    GLuint mIndices[36];

    GLuint mVao;
    GLuint mVertexVbo;
    GLuint mTexCoordVbo;
    GLuint mIbo;
    GLuint mFbo;
    GLuint mTexture;

    GLuint v3lPos;
    GLuint v2lTexC;
    GLuint uilSampl;
    GLuint uv4lColor;
    GLuint um4lWorld;

    Shaders mShader;
};

#endif