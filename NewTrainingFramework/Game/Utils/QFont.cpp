#include <string>

#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <GLFW/glfw3.h>
#include "QFont.h"

using namespace Qua;

QFont::QFont()
{

}

QFont::~QFont()
{

}

void QFont::LoadFont( std::string path, GLuint pixelSize )
{
    //Error flag
    FT_Error error = NULL;
    error = FT_Init_FreeType( &mLibrary );
    //Get cell dimensions
    GLuint cellW = 0;
    GLuint cellH = 0;
    int maxBearing = 0;
    int minHang = 0;

    //Character data
    // LTexture bitmaps[ 256 ];
    QRect_t bitmaps[ 0x100 ];
    memset(bitmaps, 0, sizeof(QRect_t) * 0x100);
    FT_Glyph_Metrics metrics[ 0x100 ];
    FT_Face face = NULL;

    error = FT_New_Face( mLibrary, path.c_str(), 0, &face );
    //Set face size
    error = FT_Set_Pixel_Sizes( face, 0, pixelSize );
    //Go through extended ASCII to get glyph data
    for( int i = 0; i < 0x100; ++i )
    {
        //Load and render glyph
        error = FT_Load_Char( face, i, FT_LOAD_RENDER );
        //Get metrics
        metrics[ i ] = face->glyph->metrics;

        //Copy glyph bitmap
        // bitmaps[ i ].copyPixels8( face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows );
        if( face->glyph->bitmap.width > 0 && face->glyph->bitmap.rows > 0 )
        {
            //Get rid of any current texture data
            // freeTexture();
            int w = face->glyph->bitmap.width;
            int h = face->glyph->bitmap.rows;
            //Copy pixels
            GLuint size = w * h;
            bitmaps[i].data = new GLubyte[ size ];
            memcpy( bitmaps[i].data, face->glyph->bitmap.buffer, size );
            bitmaps[i].clip.x = face->glyph->bitmap.width;
            bitmaps[i].clip.y = face->glyph->bitmap.rows;
            //Copy pixel data
            // mImageWidth = face->glyph->bitmap.width;
            // mImageHeight = face->glyph->bitmap.rows;
            // mTextureWidth = mImageWidth;
            // mTextureHeight = mImageHeight;

            //Set pixel format
            // mPixelFormat = GL_ALPHA;
        }

        //Calculate max bearing
        if( metrics[ i ].horiBearingY / 64 > maxBearing )
        {
            maxBearing = metrics[ i ].horiBearingY / 64;
        }

        //Calculate max width
        if( metrics[ i ].width / 64 > cellW )
        {
            cellW = metrics[ i ].width / 64;
        }

        //Calculate gylph hang
        int glyphHang = ( metrics[ i ].horiBearingY - metrics[ i ].height ) / 64;
        if( glyphHang < minHang )
        {
            minHang = glyphHang;
        }
    }

    //Create bitmap font
    cellH = maxBearing - minHang;
    // createPixels8( cellW * 16, cellH * 16 );
    int _width = cellW * 0x10;
    int _height = cellH * 0x10;
    mBitmap = new GLubyte[ _width * _height ];
    memset( mBitmap, 0, _width * _height );

    //Begin creating bitmap font
    GLuint currentChar = 0;
    // LFRect nextClip = { 0.f, 0.f, cellW, cellH };
    glm::ivec4 nextClip(0, 0, cellW, cellH);

    //Blitting coordinates
    int bX = 0;
    int bY = 0;

    //Go through cell rows
    for( unsigned int rows = 0; rows < 0x10; rows++ )
    {
        //Go through each cell column in the row
        for( unsigned int cols = 0; cols < 0x10; cols++ )
        {
            //Set base offsets
            bX = cellW * cols;
            bY = cellH * rows;

            //Initialize clip
            nextClip.x = bX;
            nextClip.y = bY;
            nextClip.z = metrics[ currentChar ].width / 64;
            nextClip.w = cellH;
            int _bY = bY + maxBearing - metrics[ currentChar ].horiBearingY / 64;
            //Blit character
            // bitmaps[ currentChar ].blitPixels8( bX, bY + maxBearing - metrics[ currentChar ].horiBearingY / 64, *this );
            //Copy pixels rows
            // GLubyte* dPixels = destination.mPixels8;
            // GLubyte* sPixels = mPixels8;
            for( int i = 0; i < bitmaps[i].clip.y; ++i )
            {
                memcpy( &mBitmap[ ( i + _bY ) * _width + bX ], &bitmaps[ i * bitmaps[i].clip.x ], bitmaps[i].clip.x );
            }

            //Go to the next character
            mClips.push_back( nextClip );
            currentChar++;
        }
    }

    //Make texture power of two
    // padPixels8();
    unsigned int _width2 = next_p2(_width);
    unsigned int _height2 = next_p2(_height);
    if((_width2 != _width) || (_height2 != _height))
    {
        //Allocate pixels
        GLuint size = _width2 * _height2;
        GLubyte* pixels = new GLubyte[ size ];
        //Copy pixels rows
        for( int i = 0; i < _height; ++i )
        {
            memcpy( &pixels[ i * _width2 ], &mBitmap[ i * _width ], _width );
        }

        //Change pixels
        delete[] mBitmap;
        mBitmap = pixels;
    }

    //Create texture
    //Generate texture ID
    glGenTextures( 1, &mTextureID );

    //Bind texture ID
    glBindTexture( GL_TEXTURE_2D, mTextureID );

    //Generate texture
    glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA, _width2, _height2, 0, GL_ALPHA, GL_UNSIGNED_BYTE, mBitmap );

    //Set texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, DEFAULT_TEXTURE_WRAP );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, DEFAULT_TEXTURE_WRAP );

    //Unbind texture
    glBindTexture( GL_TEXTURE_2D, NULL );

    {
        //Check for error
        GLenum error = glGetError();
        if( error != GL_NO_ERROR )
        {
            // printf( "Error loading texture from %p pixels! %s\n", mBitmap, gluErrorString( error ) );
        }
        else
        {
            //Release pixels
            delete[] mBitmap;
            mBitmap = NULL;

            //Vertex data
            LVertexData2D vData[ 4 ];
            GLuint iData[ 4 ];

            //Set rendering indices
            iData[ 0 ] = 0;
            iData[ 1 ] = 1;
            iData[ 2 ] = 2;
            iData[ 3 ] = 3;

            //Create VBO
            glGenBuffers( 1, &mVBOID );
            glBindBuffer( GL_ARRAY_BUFFER, mVBOID );
            glBufferData( GL_ARRAY_BUFFER, 4 * sizeof(LVertexData2D), vData, GL_DYNAMIC_DRAW );

            //Create IBO
            glGenBuffers( 1, &mIBOID );
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIBOID );
            glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), iData, GL_DYNAMIC_DRAW );

            //Unbind buffers
            glBindBuffer( GL_ARRAY_BUFFER, NULL );
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL );
        }
    }

    //Set font texture wrap
    // glBindTexture( GL_TEXTURE_2D, mTextureID );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );

    //Set spacing variables
    mSpace = cellW / 2;
    mNewLine = maxBearing;
    mLineHeight = cellH;

    //Free face
    FT_Done_Face( face );
}

void QFont::renderText( GLfloat x, GLfloat y, std::string text )
{
    // //If there is a texture to render from
    // if( getTextureID() != 0 )
    // {
    //     //Draw positions
    //     GLfloat dX = x;
    //     GLfloat dY = y;

    //     //Move to draw position
    //     glTranslatef( x, y, 0.f );

    //     //Set texture
    //     glBindTexture( GL_TEXTURE_2D, getTextureID() );

    //     //Enable vertex and texture coordinate arrays
    //     glEnableClientState( GL_VERTEX_ARRAY );
    //     glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    //     //Bind vertex data
    //     glBindBuffer( GL_ARRAY_BUFFER, mVertexDataBuffer );

    //     //Set texture coordinate data
    //     glTexCoordPointer( 2, GL_FLOAT, sizeof(LVertexData2D), (GLvoid*) offsetof( LVertexData2D, texCoord ) );

    //     //Set vertex data
    //     glVertexPointer( 2, GL_FLOAT, sizeof(LVertexData2D), (GLvoid*) offsetof( LVertexData2D, position ) );

    //         //Go through string
    //         for( int i = 0; i < text.length(); ++i )
    //         {
    //             //Space
    //             if( text[ i ] == ' ' )
    //             {
    //                 glTranslatef( mSpace, 0.f, 0.f );
    //                 dX += mSpace;
    //             }
    //             //Newline
    //             else if( text[ i ] == '\n' )
    //             {
    //                 glTranslatef( x - dX, mNewLine, 0.f );
    //                 dY += mNewLine;
    //                 dX += x - dX;
    //             }
    //             //Character
    //             else
    //             {
    //                 //Get ASCII
    //                 GLuint ascii = (unsigned char)text[ i ];

    //                 //Draw quad using vertex data and index data
    //                 glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIndexBuffers[ ascii ] );
    //                 glDrawElements( GL_QUADS, 4, GL_UNSIGNED_INT, NULL );

    //                 //Move over
    //                 glTranslatef( mClips[ ascii ].w, 0.f, 0.f );
    //                 dX += mClips[ ascii ].w;
    //             }
    //         }

    //     //Disable vertex and texture coordinate arrays
    //     glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    //     glDisableClientState( GL_VERTEX_ARRAY );
    // }
}


void QFont::DeleteFont(void)
{

}

/*void QFont::Clean(void)
{
    glDeleteLists(mListBase, 128);
    glDeleteTextures(128, mTextures);
    delete [] mTextures;
}

*/