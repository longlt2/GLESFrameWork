#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <stdio.h>
#include <string>
#include <string.h>

#include "utilities.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <glm/glm.hpp>


typedef struct _TextureVertex2D_t
{
    glm::vec2 position;
    glm::vec2 textCoord;
} TextureVertex2D;

class TextureManager
{
    public:
        static TextureManager *GetInstance(void);

        GLuint TextureID();

        GLuint TextureWidth();

        GLuint TextureHeight();

        GLuint ImageWidth();

        GLuint ImageHeight();

    protected:
        TextureManager();
        ~TextureManager();

        GLuint powerOfTwo( GLuint num );
        void initVBO();
        void freeVBO();

        bool loadTextureFromFile32( std::string path );
        bool loadTextureFromPixels32( GLuint* pixels, GLuint imgWidth, GLuint imgHeight, GLuint texWidth, GLuint texHeight );

        virtual void freeTexture();


    private:
        static TextureManager *sInstance;

        //Texture name
        GLuint mTextureID;

        //Current pixels
        GLuint* mPixels32;
        GLubyte* mPixels8;

        //Pixel format
        GLuint mPixelFormat;

        //Texture dimensions
        GLuint mTextureWidth;
        GLuint mTextureHeight;

        //Unpadded image dimensions
        GLuint mImageWidth;
        GLuint mImageHeight;

        //VBO IDs
        GLuint mVBOID;
        GLuint mIBOID;

        friend class VideoDriver;
};

#endif