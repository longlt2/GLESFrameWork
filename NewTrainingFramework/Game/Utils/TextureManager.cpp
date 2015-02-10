#include "TextureManager.h"

TextureManager *TextureManager::sInstance = 0;


TextureManager::TextureManager()
{
    //Initialize texture ID
    mTextureID = 0;

    //Initialize texture dimensions
    mTextureWidth = 0;
    mTextureHeight = 0;
}

TextureManager::~TextureManager()
{
    //Free texture data if needed
    sInstance->freeTexture();
    if(sInstance)
    {
        delete sInstance;
        sInstance = 0;
    }
}

bool TextureManager::loadTextureFromFile32( std::string path )
{
    //Texture loading success
    bool textureLoaded = false;

    //Generate and set current image ID
    ILuint imgID = 0;
    ilGenImages( 1, &imgID );
    ilBindImage( imgID );

    //Load image
    ILboolean success = ilLoadImage( path.c_str() );

    //Image loaded successfully
    if( success == IL_TRUE )
    {
        //Convert image to RGBA
        success = ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE );
        if( success == IL_TRUE )
        {
            //Initialize dimensions
            GLuint imgWidth = (GLuint)ilGetInteger( IL_IMAGE_WIDTH );
            GLuint imgHeight = (GLuint)ilGetInteger( IL_IMAGE_HEIGHT );

            //Calculate required texture dimensions
            GLuint texWidth = powerOfTwo( imgWidth );
            GLuint texHeight = powerOfTwo( imgHeight );

            //Texture is the wrong size
            if( imgWidth != texWidth || imgHeight != texHeight )
            {
                //Place image at upper left
                iluImageParameter( ILU_PLACEMENT, ILU_UPPER_LEFT );

                //Resize image
                iluEnlargeCanvas( (int)texWidth, (int)texHeight, 1 );
            }

            //Create texture from file pixels
            textureLoaded = loadTextureFromPixels32( (GLuint*)ilGetData(), imgWidth, imgHeight, texWidth, texHeight );
        }

        //Delete file from memory
        ilDeleteImages( 1, &imgID );

        //Set pixel format
        mPixelFormat = GL_RGBA;
    }

    //Report error
    if( !textureLoaded )
    {
        printf( "Unable to load %s\n", path.c_str() );
    }

    return textureLoaded;
}

bool TextureManager::loadTextureFromPixels32( GLuint* pixels, GLuint imgWidth, GLuint imgHeight, GLuint texWidth, GLuint texHeight )
{
    //Free texture data if needed
    freeTexture();

    //Get image dimensions
    mImageWidth = imgWidth;
    mImageHeight = imgHeight;
    mTextureWidth = texWidth;
    mTextureHeight = texHeight;

    //Generate texture ID
    glGenTextures( 1, &mTextureID );

    //Bind texture ID
    glBindTexture( GL_TEXTURE_2D, mTextureID );

    //Generate texture
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    //Set texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    //Unbind texture
    glBindTexture( GL_TEXTURE_2D, NULL );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        // printf( "Error loading texture from %p pixels! %s\n", pixels, gluErrorString( error ) );
        return false;
    }

    //Generate VBO
    // initVBO();

    //Set pixel format
    mPixelFormat = GL_RGBA;

    return true;
}

void TextureManager::freeTexture()
{
    //Delete texture
    if( mTextureID != 0 )
    {
        glDeleteTextures( 1, &mTextureID );
        mTextureID = 0;
    }

    mTextureWidth = 0;
    mTextureHeight = 0;
}

GLuint TextureManager::powerOfTwo( GLuint num )
{
    if( num != 0 )
    {
        num--;
        num |= (num >> 1); //Or first 2 bits
        num |= (num >> 2); //Or next 2 bits
        num |= (num >> 4); //Or next 4 bits
        num |= (num >> 8); //Or next 8 bits
        num |= (num >> 16); //Or next 16 bits
        num++;
    }

    return num;
}

void TextureManager::initVBO()
{
    //If texture is loaded and VBO does not already exist
    if( mTextureID != 0 && mVBOID == 0 )
    {
        //Vertex data
        TextureVertex2D vData[ 4 ];
        GLuint iData[ 4 ];

        //Set rendering indices
        iData[ 0 ] = 0;
        iData[ 1 ] = 1;
        iData[ 2 ] = 2;
        iData[ 3 ] = 3;

        //Create VBO
        glGenBuffers( 1, &mVBOID );
        glBindBuffer( GL_ARRAY_BUFFER, mVBOID );
        glBufferData( GL_ARRAY_BUFFER, 4 * sizeof(TextureVertex2D), vData, GL_DYNAMIC_DRAW );

        //Create IBO
        glGenBuffers( 1, &mIBOID );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIBOID );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), iData, GL_DYNAMIC_DRAW );

        //Unbind buffers
        glBindBuffer( GL_ARRAY_BUFFER, NULL );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL );
    }
}

void TextureManager::freeVBO()
{
    //Free VBO and IBO
    if( mVBOID != 0 )
    {
        glDeleteBuffers( 1, &mVBOID );
        glDeleteBuffers( 1, &mIBOID );
    }
}

GLuint TextureManager::TextureID()
{
    return mTextureID;
}

GLuint TextureManager::TextureWidth()
{
    return mTextureWidth;
}

GLuint TextureManager::TextureHeight()
{
    return mTextureHeight;
}

GLuint TextureManager::ImageWidth()
{
    return mImageWidth;
}

GLuint TextureManager::ImageHeight()
{
    return mImageHeight;
}

TextureManager *TextureManager::GetInstance(void)
{
    if(!sInstance)
    {
        sInstance = new TextureManager();
    }
    return sInstance;
}