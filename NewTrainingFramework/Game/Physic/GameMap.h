#ifndef FILLED_CIRCLE_H
#define FILLED_CIRCLE_H

#include "Define.h"
#include "Physic.h"

#include <glm/glm.hpp>

#include "Object.h"
#include "Shaders.h"



namespace GameObject
{

class GameMap
{

private:


protected:

public:

    glm::vec2 mVertices[6];
    glm::vec2 mTexCoord[6];
    unsigned int mIndices[6];

    GLuint mVao;
    GLuint mVertexVbo;
    GLuint mTexCoordVbo;
    GLuint mIbo;
    GLuint mFbo;
    GLuint mTexture;

    Shaders mShader;

    GameMap();
    ~GameMap();

    void Init(char *vs, char *fs);
    void Draw(int, int, int, int);
    void Bind(bool);
};

}

#endif
