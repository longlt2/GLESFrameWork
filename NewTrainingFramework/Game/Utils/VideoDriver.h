#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include "Shaders.h"
#include "glm/glm.hpp"

#define DEFAULT_COLOR               glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)

typedef enum _eGameReturn_t eGameReturn_t;

namespace GUtils
{

class VideoDriver
{
public:

    static VideoDriver *GetInstance(void);

	eGameReturn_t Draw(float x, float y, float width, float height, GLuint idTexture = 0, glm::vec4 const &f = DEFAULT_COLOR) const;
    eGameReturn_t Draw(glm::vec2 const &v, float width, float height, GLuint idTexture = 0, glm::vec4 const &f=DEFAULT_COLOR) const
    {
        return VideoDriver::Draw(v.x, v.y, width, height, idTexture, f);
    }

    eGameReturn_t DrawVertices(float const * vertices, unsigned int numVertices, GLenum mode = GL_TRIANGLES, GLuint idTexture = 0, glm::vec4 const &f=DEFAULT_COLOR) const;
	eGameReturn_t DrawVertices(glm::vec2 const *pv, unsigned int numVertices, GLenum mode = GL_TRIANGLES, GLuint idTexture = 0, glm::vec4 const &f=DEFAULT_COLOR) const;

    eGameReturn_t DrawLine(float x, float y, float u, float v, glm::vec4 const &f=DEFAULT_COLOR) const;
    eGameReturn_t DrawLine(glm::vec2 const &v, glm::vec2 const &u, glm::vec4 const &f=DEFAULT_COLOR) const
    {
        return VideoDriver::DrawLine(v.x, v.y, u.x, u.y, f);
    }

    eGameReturn_t DrawRect(float x, float y, float w, float h, float weight=1, glm::vec4 const &f=DEFAULT_COLOR) const;
	eGameReturn_t DrawRect(glm::vec2 const &v, glm::vec2 const &u, float weight = 1, glm::vec4 const &f=DEFAULT_COLOR) const
    {
        return VideoDriver::Draw(v, u.x, u.y, weight, f);
    }

    eGameReturn_t DrawFillRect(float x, float y, float w, float h, glm::vec4 const &f=DEFAULT_COLOR) const
    {
        return VideoDriver::Draw(glm::vec2(x,y), w, h, 0, f);
    }
    eGameReturn_t DrawFillRect(glm::vec2 const &v, float w, float h, glm::vec4 const &f=DEFAULT_COLOR) const
    {
        return VideoDriver::Draw(v, w, h, 0, f);
    }
    eGameReturn_t DrawFillRect(glm::vec2 const &v, glm::vec2 const &u, glm::vec4 const &f=DEFAULT_COLOR) const
    {
        return VideoDriver::Draw(v, u.x, u.y, 0, f);
    }

    eGameReturn_t DrawCircle(float x, float y, float r, glm::vec4 const &f=DEFAULT_COLOR) const;
    eGameReturn_t DrawCircle(glm::vec2 const &v, float r, glm::vec4 const &f=DEFAULT_COLOR) const
    {
        return VideoDriver::DrawCircle(v.x, v.y, r, f);
    }

    void SetWidth(float w) { mWidth = w; }
    void SetHeight(float h) { mHeight = h; }

    float GetWidth(void) const { return mWidth; }
    float GetHeight(void) const { return mHeight; }

    void SetProgram(GLuint prog) { mProgram = prog; }

    void CleanScreen(void)
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

protected:
    VideoDriver();
    virtual ~VideoDriver();
    inline float X2Game(float x) const { return ((1.0f * x - mWidth / 2) * 2 / mWidth); }
    inline float Y2Game(float y) const { return ((-1.0f) * (1.0f * y - mHeight / 2) * 2 / mHeight); }
    glm::vec2 XY2Game(float x, float y) const { return glm::vec2(VideoDriver::X2Game(x), VideoDriver::Y2Game(y)); }
    glm::vec2 XY2Game(glm::vec2 v) const { return VideoDriver::XY2Game(v.x, v.y); }

private:

    float mWidth;
    float mHeight;
    GLuint mProgram;
    static VideoDriver* msInstance;
};

}// namespace

#endif