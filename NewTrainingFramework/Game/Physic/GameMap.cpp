#include "GameMap.h"
#include <glm/gtc/type_ptr.hpp>
#include "TextureManager.h"

using namespace GameObject;

GameMap::GameMap()
{

}

GameMap::~GameMap()
{

}

void GameMap::Init(char *vs, char *fs)
{
    mShader.Init(vs, fs);

//Initialize DevIL
ilInit();
ilClearColour( 255, 255, 255, 000 );
if( !TextureManager::GetInstance()->loadTextureFromFile32( "../Image.bmp" ) )
{
    printf( "Unable to load file texture!\n" );
    // return false;
}

    glUseProgram(mShader.program);

    GLint posLoc = glGetAttribLocation(mShader.program, "a_posL");
    GLint texLoc = glGetAttribLocation(mShader.program, "a_texCoord");
    GLint uSampl = glGetUniformLocation(mShader.program, "uSampler");
    GLint uColor = glGetUniformLocation(mShader.program, "unifColor");
    // We assume, that the parent program created the texture!
    if(uSampl != -1)
    {
        glUniform1i(uSampl, 0);
    }

    if(uColor != -1)
    {
        glUniform4fv(uColor, 1, glm::value_ptr(glm::vec4(1)));
    }

        // X2GAME(x), Y2GAME(y),
        // X2GAME(x), Y2GAME(y + height),
        // X2GAME(x + width), Y2GAME(y + height),

        // X2GAME(x), Y2GAME(y),
        // X2GAME(x + width), Y2GAME(y + height),
        // X2GAME(x + width), Y2GAME(y)

    mTexCoord[0] = glm::vec2(0, 1);
    mTexCoord[1] = glm::vec2(0, 0);
    mTexCoord[2] = glm::vec2(1, 0);

    mTexCoord[3] = glm::vec2(0, 1);
    mTexCoord[4] = glm::vec2(1, 0);
    mTexCoord[5] = glm::vec2(1, 1);

    mIndices[0] = 0;
    mIndices[1] = 1;
    mIndices[2] = 2;
    mIndices[3] = 3;
    mIndices[4] = 4;
    mIndices[5] = 5;

    // Vertex VBO
    glGenBuffers(1, &mVertexVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexVbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec2), mVertices, GL_STATIC_DRAW);
    // Texture coordinate VBO
    glGenBuffers(1, &mTexCoordVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mTexCoordVbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec2), mTexCoord, GL_STATIC_DRAW);

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
        // mVertices[0] = glm::vec2(X2GAME(_x), Y2GAME(_y));
        // mVertices[1] = glm::vec2(X2GAME(_x), Y2GAME(_y + height));
        // mVertices[2] = glm::vec2(X2GAME(_x + width), Y2GAME(_y + height));

        // mVertices[3] = glm::vec2(X2GAME(_x), Y2GAME(_y));
        // mVertices[4] = glm::vec2(X2GAME(_x + width), Y2GAME(_y + height));
        // mVertices[5] = glm::vec2(X2GAME(_x + width), Y2GAME(_y));

    mVertices[0].x = X2GAME(_x, SCREEN_W);
    mVertices[0].y = Y2GAME(_y, SCREEN_H);
    mVertices[1].x = X2GAME(_x, SCREEN_W);
    mVertices[1].y = Y2GAME(_y + height, SCREEN_H);
    mVertices[2].x = X2GAME(_x + width, SCREEN_W);
    mVertices[2].y = Y2GAME(_y + height, SCREEN_H);

    mVertices[3].x = X2GAME(_x, SCREEN_W);
    mVertices[3].y = Y2GAME(_y, SCREEN_H);
    mVertices[4].x = X2GAME(_x + width, SCREEN_W);
    mVertices[4].y = Y2GAME(_y + height, SCREEN_H);
    mVertices[5].x = X2GAME(_x + width, SCREEN_W);
    mVertices[5].y = Y2GAME(_y, SCREEN_H);

    glUseProgram(mShader.program);

    glBindVertexArray(mVao);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexVbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec2), mVertices, GL_STATIC_DRAW);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIbo );

    glBindTexture(GL_TEXTURE_2D, mTexture);
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GameMap::Bind(bool isStart)
{
    if(isStart)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
