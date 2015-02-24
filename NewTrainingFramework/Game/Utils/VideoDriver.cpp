
// #include "stdafx.h"
#include "VideoDriver.h"
#include "glm/gtc/type_ptr.hpp"
// glm::translate, glm::rotate, glm::scale
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

#include "Define.h"
#include <cmath>
#include "Physic.h"

namespace GUtils
{

static float sScale = 0;

void mvp(GLuint program)
{
    sScale += 0.0001f;
    GLint locs = glGetUniformLocation(program, "uModel");
    if (locs != -1)
    {
        glm::mat4 matrix(1);

        // matrix = glm::translate(matrix, glm::vec3(sinf(sScale), 0, 1.0f));
        matrix = glm::translate(matrix, glm::vec3(-0.5f, 0.5f, 1.0f));

        glUniformMatrix4fv(locs, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    locs = glGetUniformLocation(program, "uView");
    if (locs != -1)
    {
        glm::mat4 matrix(1);

        glUniformMatrix4fv(locs, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    locs = glGetUniformLocation(program, "uProj");
    if (locs != -1)
    {
        glm::mat4 matrix(1);

        matrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f);

        glUniformMatrix4fv(locs, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}


VideoDriver* VideoDriver::msInstance = 0;

VideoDriver::VideoDriver()
{
    mWidth = 0;
    mHeight = 0;
    mProgram = 0;
}

VideoDriver::~VideoDriver()
{
    if(msInstance)
    {
        delete msInstance;
    }
    msInstance = 0;
}

VideoDriver *VideoDriver::GetInstance(void)
{
    if(!msInstance)
    {
        msInstance = new VideoDriver();
    }
    return msInstance;
}

eGameReturn_t VideoDriver::Draw(float x, float y, float width, float height, GLuint idTexture, glm::vec4 const &f) const
{
    float vertices[] =  {
        // X2Game(x), Y2Game(y),
        // X2Game(x), Y2Game(y + height),
        // X2Game(x + width), Y2Game(y + height),
        // X2Game(x), Y2Game(y),
        // X2Game(x + width), Y2Game(y + height),
        // X2Game(x + width), Y2Game(y)

        X2GAME(x, mWidth), Y2GAME(y, mHeight),
        X2GAME(x, mWidth), Y2GAME(y + height, mHeight),
        X2GAME(x + width, mWidth), Y2GAME(y + height, mHeight),
        X2GAME(x, mWidth), Y2GAME(y, mHeight),
        X2GAME(x + width, mWidth), Y2GAME(y + height, mHeight),
        X2GAME(x + width, mWidth), Y2GAME(y, mHeight)
    };

    if(!mProgram)
    {
		return eGameReturnErr;
    }

    // Use program
    glUseProgram(mProgram);

    if(idTexture == 0)
    {
        // Transfer data to verties
        GLint locs = glGetAttribLocation(mProgram, "a_posL");
        if (locs != -1)
        {
            glEnableVertexAttribArray(locs);
            glVertexAttribPointer(locs, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        }

        locs = glGetUniformLocation(mProgram, "unifColor");
        if (locs != -1)
        {
            glUniform4fv(locs, 1, glm::value_ptr(f));
        }

        mvp(mProgram);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    else
    {

    }

    return eGameReturnOk;
}

eGameReturn_t VideoDriver::DrawVertices(float const *vertices, unsigned int numVertices, GLenum mode, GLuint idTexture, glm::vec4 const &f) const
{
    if(!mProgram)
    {
        return eGameReturnErr;
    }

    // Use program
    glUseProgram(mProgram);
    // Transfer data to verties
    GLint locs = glGetAttribLocation(mProgram, "a_posL");
    if (locs != -1)
    {
        glEnableVertexAttribArray(locs);
        glVertexAttribPointer(locs, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    }

    locs = glGetUniformLocation(mProgram, "unifColor");
    if (locs != -1)
    {
        glUniform4fv(locs, 1, glm::value_ptr(f));
    }

    mvp(mProgram);

    glDrawArrays(mode, 0, numVertices);

    return eGameReturnOk;
}

eGameReturn_t VideoDriver::DrawVertices(glm::vec2 const *pv, unsigned int numVertices, GLenum mode, GLuint idTexture, glm::vec4 const &f) const
{
    return eGameReturnOk;
}

eGameReturn_t VideoDriver::DrawLine(float x, float y, float u, float v, glm::vec4 const &f) const
{
    float vertices [] = {
        X2GAME(x, mWidth),
        Y2GAME(y, mHeight),
        X2GAME(u, mWidth),
        Y2GAME(v, mHeight),
    };

    return DrawVertices(vertices, 2, GL_LINES, 0, f);
}

eGameReturn_t VideoDriver::DrawRect(float x, float y, float w, float h, float weight, glm::vec4 const &f) const
{
    eGameReturn_t rs = eGameReturnOk;
    //Top
	rs = (eGameReturn_t)(rs | VideoDriver::DrawFillRect(glm::vec2(x, y), w, weight, f));
    //Bottom
    rs = (eGameReturn_t)(rs | VideoDriver::DrawFillRect(glm::vec2(x, y + h - weight), w, weight, f));
    //Left
    rs = (eGameReturn_t)(rs | VideoDriver::DrawFillRect(glm::vec2(x, y + weight), weight, h - ((unsigned int)weight<<1), f));
    //Right
	rs = (eGameReturn_t)(rs | VideoDriver::DrawFillRect(glm::vec2(x + w - weight, y + weight), weight, h - ((unsigned int)weight << 1), f));
    return rs;
}

eGameReturn_t VideoDriver::DrawCircle(float x, float y, float r, glm::vec4 const &f) const
{
    float fx = (float)x;
    float fy = (float)y;
    eGameReturn_t rs;
    const int SEGMENTS = 32;
    const float INCREMENT = 2.0f*(float)PI/SEGMENTS;
    float theta = 0.0f;
    float* vertices = new float[SEGMENTS*2];

    for (int i = 0; i < SEGMENTS; ++i)
    {
        vertices[i*2] = X2GAME(fx + (r)*cosf(theta), mWidth, f);
        vertices[i*2 + 1] = Y2GAME(fy + (r)*sinf(theta), mHeight, f);
        theta += INCREMENT;
    }
    rs = DrawVertices(vertices, SEGMENTS, GL_LINE_LOOP, 0, f);
    delete [] vertices;

    return rs;
}

} // namespace