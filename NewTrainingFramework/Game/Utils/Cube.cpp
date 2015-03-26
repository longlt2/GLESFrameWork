#include "Cube.h"
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
using namespace std;

// static float sScale = 0;
// static float sRotate = 0;
// glm::mat4 m4Model(1);
// glm::mat4 m4View;
// glm::mat4 m4Proj;

Cube::Cube()
{

}

Cube::~Cube()
{

}

void Cube::Init(char *vs, char *fs)
{
    mShader.Init(vs, fs);

    // Initialize DevIL
    ilInit();
    ilClearColour( 255, 255, 255, 000 );
    if( !TextureManager::GetInstance()->loadTextureFromFile32( "../Resources/Textures/SpiderTex.jpg" ) )
    {
        // printf( "Unable to load file texture!\n" );
        cout<<"Unable to load file texture!\n";
        // return false;
    }

    glUseProgram(mShader.program);

    v3lPos = glGetAttribLocation(mShader.program, "v3Position");
    v2lTexC = glGetAttribLocation(mShader.program, "v2TexCoord");
    uilSampl = glGetUniformLocation(mShader.program, "uiSampler");
    uv4lColor = glGetUniformLocation(mShader.program, "uv4Color");
    um4lWorld = glGetUniformLocation(mShader.program, "um4World");

    // We assume, that the parent program created the texture!
    if(uilSampl != -1)
    {
        glUniform1i(uilSampl, 0);
    }

    if (uv4lColor != -1)
    {
        glUniform4fv(uv4lColor, 1, glm::value_ptr(glm::vec4(1)));
    }

    int i=0;

    // front
    mVertices[i++] = glm::vec3(-.5f,  .5f, -.5f);
    mVertices[i++] = glm::vec3( .5f,  .5f, -.5f);
    mVertices[i++] = glm::vec3(-.5f, -.5f, -.5f);
    mVertices[i++] = glm::vec3( .5f, -.5f, -.5f);
    // top
    mVertices[i++] = glm::vec3(-.5f,  .5f,  .5f);
    mVertices[i++] = glm::vec3( .5f,  .5f,  .5f);
    mVertices[i++] = glm::vec3(-.5f,  .5f, -.5f);
    mVertices[i++] = glm::vec3( .5f,  .5f, -.5f);
    // left
    mVertices[i++] = glm::vec3(-.5f,  .5f,  .5f);
    mVertices[i++] = glm::vec3(-.5f,  .5f, -.5f);
    mVertices[i++] = glm::vec3(-.5f, -.5f,  .5f);
    mVertices[i++] = glm::vec3(-.5f, -.5f, -.5f);
    // back
    mVertices[i++] = glm::vec3( .5f,  .5f,  .5f);
    mVertices[i++] = glm::vec3(-.5f,  .5f,  .5f);
    mVertices[i++] = glm::vec3( .5f, -.5f,  .5f);
    mVertices[i++] = glm::vec3(-.5f, -.5f,  .5f);
    // bottom
    mVertices[i++] = glm::vec3(-.5f, -.5f, -.5f);
    mVertices[i++] = glm::vec3( .5f, -.5f, -.5f);
    mVertices[i++] = glm::vec3(-.5f, -.5f,  .5f);
    mVertices[i++] = glm::vec3( .5f, -.5f,  .5f);
    // right
    mVertices[i++] = glm::vec3( .5f,  .5f, -.5f);
    mVertices[i++] = glm::vec3( .5f,  .5f,  .5f);
    mVertices[i++] = glm::vec3( .5f, -.5f, -.5f);
    mVertices[i++] = glm::vec3( .5f, -.5f,  .5f);

    i=0;
    mTexCoord[i++] = glm::vec2(0, 1);
    mTexCoord[i++] = glm::vec2(1, 1);
    mTexCoord[i++] = glm::vec2(0, 0);
    mTexCoord[i++] = glm::vec2(1, 0);

    mTexCoord[i++] = glm::vec2(0, 1);
    mTexCoord[i++] = glm::vec2(1, 1);
    mTexCoord[i++] = glm::vec2(0, 0);
    mTexCoord[i++] = glm::vec2(1, 0);

    mTexCoord[i++] = glm::vec2(0, 1);
    mTexCoord[i++] = glm::vec2(1, 1);
    mTexCoord[i++] = glm::vec2(0, 0);
    mTexCoord[i++] = glm::vec2(1, 0);

    mTexCoord[i++] = glm::vec2(0, 1);
    mTexCoord[i++] = glm::vec2(1, 1);
    mTexCoord[i++] = glm::vec2(0, 0);
    mTexCoord[i++] = glm::vec2(1, 0);

    mTexCoord[i++] = glm::vec2(0, 1);
    mTexCoord[i++] = glm::vec2(1, 1);
    mTexCoord[i++] = glm::vec2(0, 0);
    mTexCoord[i++] = glm::vec2(1, 0);

    mTexCoord[i++] = glm::vec2(0, 1);
    mTexCoord[i++] = glm::vec2(1, 1);
    mTexCoord[i++] = glm::vec2(0, 0);
    mTexCoord[i++] = glm::vec2(1, 0);

    mTexCoord[i++] = glm::vec2(0, 1);
    mTexCoord[i++] = glm::vec2(1, 1);
    mTexCoord[i++] = glm::vec2(0, 0);
    mTexCoord[i++] = glm::vec2(1, 0);

    i=0;
    // front
    mIndices[i++] = 0;
    mIndices[i++] = 3;
    mIndices[i++] = 1;
    mIndices[i++] = 0;
    mIndices[i++] = 3;
    mIndices[i++] = 2;
    // back
    mIndices[i++] = 4;
    mIndices[i++] = 7;
    mIndices[i++] = 5;
    mIndices[i++] = 4;
    mIndices[i++] = 7;
    mIndices[i++] = 6;
    // top
    mIndices[i++] = 8;
    mIndices[i++] = 11;
    mIndices[i++] = 9;
    mIndices[i++] = 8;
    mIndices[i++] = 11;
    mIndices[i++] = 10;

    // left
    mIndices[i++] = 12;
    mIndices[i++] = 15;
    mIndices[i++] = 13;
    mIndices[i++] = 12;
    mIndices[i++] = 15;
    mIndices[i++] = 14;
    // back
    mIndices[i++] = 16;
    mIndices[i++] = 19;
    mIndices[i++] = 17;
    mIndices[i++] = 16;
    mIndices[i++] = 19;
    mIndices[i++] = 18;
    // bottom
    mIndices[i++] = 20;
    mIndices[i++] = 23;
    mIndices[i++] = 21;
    mIndices[i++] = 20;
    mIndices[i++] = 23;
    mIndices[i++] = 22;

    // Vertex VBO
    glGenBuffers(1, &mVertexVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexVbo);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(glm::vec3), mVertices, GL_STATIC_DRAW);

    // Texture coordinate VBO
    glGenBuffers(1, &mTexCoordVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mTexCoordVbo);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(glm::vec2), mTexCoord, GL_STATIC_DRAW);

    // IBO
    glGenBuffers(1, &mIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), (GLuint*) mIndices, GL_STATIC_DRAW);

    // VAO
    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    if(v3lPos != -1)
    {
        glEnableVertexAttribArray(v3lPos);
        glBindBuffer( GL_ARRAY_BUFFER, mVertexVbo );
        glVertexAttribPointer( v3lPos, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    }

    if(v2lTexC != -1)
    {
        glEnableVertexAttribArray(v2lTexC);
        glBindBuffer( GL_ARRAY_BUFFER, mTexCoordVbo );
        glVertexAttribPointer( v2lTexC, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    }

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIbo );
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // FBO
    // glGenTextures(1, &mTexture);
    // glBindTexture(GL_TEXTURE_2D, mTexture);
    // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_W, SCREEN_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
    // glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    // glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    // glBindTexture( GL_TEXTURE_2D, 0 );

    // bind texture
    // glGenFramebuffers(1, &mFbo);
    // glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);
    // glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glUseProgram(0);
}

void Cube::Draw(void) const
{
// static glm::vec3 position = glm::vec3( 1, 1, 3 );
// static float horizontalAngle = PI;
// static float verticalAngle = 0.0f;
// static float FoV = 30.f;
// static glm::vec3 direction(
//         cos(verticalAngle) * sin(horizontalAngle),
//         sin(verticalAngle),
//         cos(verticalAngle) * cos(horizontalAngle)
//     );
// static glm::vec3 right = glm::vec3(
// 		sin(horizontalAngle - HALFPI),
//         0,
// 		cos(horizontalAngle - HALFPI)
//     );
// static glm::vec3 up = glm::cross( right, direction );
// // m4Proj = glm::perspective(FoV, 4.f/3.f, .1f, 100.f);
// m4Proj = glm::perspective<glm::lowp_float>(PI * FoV / 180.f, 4.0f / 3.0f, 0.1f, 100.0f);
// m4View = glm::lookAt( position, glm::vec3(0,0,0), up );
// glm::mat4 MVP = m4Proj * m4View * m4Model;

    // glFrontFace(GL_CCW);
    glUseProgram(mShader.program);
    if(um4lWorld != -1)
    {
        // static float tmp = .05f;
        // sRotate += 0.01f;
        // if(sScale > 10)
        // {
        //     tmp = -.05f;
        // }

        // if(sScale < 0)
        // {
        //     tmp = .05f;
        // }

        // sScale += tmp;
        // if(sScale > 90.f) sScale = 0.f;
        // TRACEFLT(sScale);
        // m4Proj = glm::ortho(-4.0f / 3.0f, 4.0f / 3.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        // m4Model = glm::translate(m4Model, glm::vec3(0.0f, sinf(sScale), 2.0f));
        // m4Model = glm::rotate(m4Model, sinf(sScale), glm::vec3(0.0f, 0.0f, 1.0f));

        // m4Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.f));
		// m4Model = glm::rotate(m4Model, sRotate, glm::vec3(0,1.f,0));
        // m4Model = glm::scale(m4Model, glm::vec3(0.8f));



        // glm::vec4 temp = MVP * glm::vec4(0,0,1,1);
        // std::cout<<glm::to_string(m4Model[0])<<std::endl;

        glUniformMatrix4fv(um4lWorld, 1, GL_FALSE, &mMVP[0][0]);
    }

    glBindVertexArray(mVao);
    glBindTexture(GL_TEXTURE_2D, TextureManager::GetInstance()->TextureID());
// glBindTexture( GL_TEXTURE_2D, NULL );

    glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0 );

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Cube::SetMVP(glm::mat4 const &mvp)
{
    mMVP = mvp;
}