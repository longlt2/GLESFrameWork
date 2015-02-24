#include "GameMap.h"
#include "TextureManager.h"
// glm::translate, glm::rotate, glm::scale
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

using namespace GameObject;

GameMap::GameMap()
{

}

GameMap::~GameMap()
{

}

static float sScale = 0;
GLint posLoc;
GLint texLoc;
GLint uSampl;
GLint uColor;
GLint world;

void GameMap::Init(char *vs, char *fs)
{
    mShader.Init(vs, fs);

//Initialize DevIL
// ilInit();
// ilClearColour( 255, 255, 255, 000 );
// if( !TextureManager::GetInstance()->loadTextureFromFile32( "../Image.bmp" ) )
// {
//     printf( "Unable to load file texture!\n" );
//     // return false;
// }

    glUseProgram(mShader.program);

    posLoc = glGetAttribLocation(mShader.program, "a_posL");
    texLoc = glGetAttribLocation(mShader.program, "a_texCoord");
    uSampl = glGetUniformLocation(mShader.program, "uSampler");
    uColor = glGetUniformLocation(mShader.program, "unifColor");
    world = glGetUniformLocation(mShader.program, "uniWorld");

    // We assume, that the parent program created the texture!
    if(uSampl != -1)
    {
        glUniform1i(uSampl, 0);
    }

    if(uColor != -1)
    {
        glUniform4fv(uColor, 1, glm::value_ptr(glm::vec4(1)));
    }

    if(world != -1)
    {
        glm::mat4 m4Model(1), m4View(1), m4Projection(1);

		// m4Projection = glm::ortho(-4.0f / 3.0f, 4.0f / 3.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        m4Model = glm::translate(m4Model, glm::vec3(0.0f, sScale, 1.0f));
        // m4Model = glm::rotate(m4Model, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 MVP = m4Model * m4View * m4Projection;
        glUniformMatrix4fv(world, 1, GL_FALSE, glm::value_ptr(MVP));
    }

    mTexCoord[0] = glm::vec2(0, 1);
    mTexCoord[1] = glm::vec2(1, 1);
    mTexCoord[2] = glm::vec2(1, 0);
    mTexCoord[3] = glm::vec2(0, 0);

    mIndices[0] = 0;
    mIndices[1] = 3;
    mIndices[2] = 2;
    mIndices[3] = 0;
    mIndices[4] = 2;
    mIndices[5] = 1;

    // Vertex VBO
    glGenBuffers(1, &mVertexVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexVbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), mVertices, GL_STATIC_DRAW);

    // Texture coordinate VBO
    glGenBuffers(1, &mTexCoordVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mTexCoordVbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), mTexCoord, GL_STATIC_DRAW);

    // IBO
    glGenBuffers(1, &mIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), (GLuint*) mIndices, GL_STATIC_DRAW);

    // VAO
    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    if(posLoc != -1)
    {
        glEnableVertexAttribArray(posLoc);
        glBindBuffer( GL_ARRAY_BUFFER, mVertexVbo );
        glVertexAttribPointer( posLoc, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    }

    if(texLoc != -1)
    {
        glEnableVertexAttribArray(texLoc);
        glBindBuffer( GL_ARRAY_BUFFER, mTexCoordVbo );
        glVertexAttribPointer( texLoc, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    }

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIbo );
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // FBO
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_W, SCREEN_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glBindTexture( GL_TEXTURE_2D, 0 );
    // bind texture
    glGenFramebuffers(1, &mFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glUseProgram(0);
}

void GameMap::Draw(int _x, int _y, int width, int height)
{
    if(world != -1)
    {
        sScale += 0.001f;
        glm::mat4 m4Model(1), m4View(1), m4Projection(1);

        // m4Projection = glm::ortho(-4.0f / 3.0f, 4.0f / 3.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        // m4Model = glm::translate(m4Model, glm::vec3(0.0f, sinf(sScale), 2.0f));
        // m4Model = glm::rotate(m4Model, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));

        m4Model = glm::translate(m4Model, glm::vec3(2.0f, 3.0f, 0.0f));

        glm::mat4 MVP = m4Model * m4View * m4Projection;

        glm::vec4 temp = MVP * glm::vec4(0,0,1,1);
        std::cout<<glm::to_string(m4Model[0])<<std::endl;

        glUniformMatrix4fv(world, 1, GL_FALSE, glm::value_ptr(MVP));
    }

    mVertices[0] = glm::vec2(X2GAME(_x, SCREEN_W), Y2GAME(_y, SCREEN_H));
    mVertices[1] = glm::vec2(X2GAME(_x + width, SCREEN_W), Y2GAME(_y, SCREEN_H));
    mVertices[2] = glm::vec2(X2GAME(_x + width, SCREEN_W), Y2GAME(_y + height, SCREEN_H));
    mVertices[3] = glm::vec2(X2GAME(_x, SCREEN_W), Y2GAME(_y + height, SCREEN_H));

    glUseProgram(mShader.program);

    glBindVertexArray(mVao);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexVbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), mVertices, GL_STATIC_DRAW);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIbo );

    glBindTexture(GL_TEXTURE_2D, mTexture);
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void GameMap::Bind(bool isStart)
{
    if(isStart)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
