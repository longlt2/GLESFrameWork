#ifndef QFONT_H
#define QFONT_H

#define GLM_PRECISION_MEDIUMP_INT

#include <freetype2/ft2build.h>
#include <freetype2/freetype.h>
#include <freetype2/ftglyph.h>
#include <freetype2/ftoutln.h>
#include <freetype2/fttrigon.h>
#include <vector>
#include <GLES3/gl3.h>
#include <EGL/egl.h>

#include <glm/glm.hpp>
// #include <glm/type.hpp>

namespace Qua
{

typedef struct _QRect_t
{
    glm::ivec2 clip;
    GLubyte *data;
} QRect_t;

///This function gets the first power of 2 >= the
///int that we pass it.
inline unsigned int next_p2 ( unsigned int n )
{
    n--;
    n |= n >> 1;   // Divide by 2^k for consecutive doublings of k up to 32,
    n |= n >> 2;   // and then or the results.
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return (n + 1);
}

//void _MakeDList ( FT_Face face, char ch, GLuint list_base, GLuint * tex_base );
// void print(QFont const &qFont, float x, float y, const char *fmt, ...);

class QFont
{
public:
    QFont();
    ~QFont();
    void LoadFont( std::string path, GLuint pixelSize );
    void DeleteFont(void);
    void renderText( GLfloat x, GLfloat y, std::string text );
    //Font TTF library
    FT_Library mLibrary;

    GLfloat mSpace;
    GLfloat mLineHeight;
    GLfloat mNewLine;

    GLubyte *mBitmap;

    std::vector< glm::ivec4 > mClips;

    GLuint mTextureID;
};
}

#endif