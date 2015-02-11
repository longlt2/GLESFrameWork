// NewTrainingFramework.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include "Globals.h"
#include <conio.h>
#include "Define.h"
#include "Physic.h"
#include "VideoDriver.h"
#include "SceneManager.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Plane.h"
#include "GameMap.h"

GLuint vboId;
Shaders myShaders;
GameObject::GameMap gmap;

enum FBO_TEXTURE
{
    FBO_TEXTURE_MAP,
    FBO_TEXTURE_OBJ,
    FBO_TEXTURE_MAX,
};

GLuint gIndex = 0;
//Framebuffer
GLuint gFbo = 0;
GLuint gFboTexture[FBO_TEXTURE_MAX];
GLuint gFboDepth = 0;

void InitFrameBuffer( void )
{
    // // depth buffer
    // glGenRenderbuffers(1, &gFboDepth);
    // glBindRenderbuffer(GL_RENDERBUFFER, gFboDepth);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_W, SCREEN_H);
    // glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // frame buffer object
    glGenFramebuffers(1, &gFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, gFbo);

    // // texture buffer
    for (int i = 0; i < FBO_TEXTURE_MAX; ++i)
    {
        glGenTextures(1, &gFboTexture[i]);
        glBindTexture(GL_TEXTURE_2D, gFboTexture[i]);
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_W, SCREEN_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glBindTexture( GL_TEXTURE_2D, NULL );


        // // bind depth & texture
        // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gFboDepth);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, gFboTexture[i], 0);
    }
    // g_sceneManager->Render(video);
    glBindFramebuffer( GL_FRAMEBUFFER, 0 ); // Unbind our frame buffer
}

int Init ( ESContext *esContext )
{
    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

    //creation of shaders and program
    myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");

    gWIDTH = SCREEN_W;
    gHEIGHT = SCREEN_H;
    GUtils::VideoDriver::GetInstance()->SetProgram(myShaders.program);
    GUtils::VideoDriver::GetInstance()->SetWidth(Globals::screenWidth);
    GUtils::VideoDriver::GetInstance()->SetHeight(Globals::screenHeight);
    video = GUtils::VideoDriver::GetInstance();
    g_sceneManager = SceneManager::GetInstance();
    g_collisionManager = CollisionManager::GetInstance();
    if(g_sceneManager->Init(DATA_URL))
    {
        g_sceneManager->AddObject(new Circle(200, 100, 50, glm::vec2(0, 0), 70));
    }

    gStartTime = GetTickCount();

    // InitFrameBuffer();

    gmap.Init("../Resources/Shaders/GameMap.vs", "../Resources/Shaders/GameMap.fs");

    return 0;
}

void draw(ESContext *esContext)
{
    char skipFrame = 0;
    while( GetTickCount() > gStartTime && skipFrame < 5 )
    {
        g_sceneManager->Update(g_gameSpeed/LIMIT_FPS);
        skipFrame++;
        gStartTime += FRAME_TIME;
    }

    float prediction = float( GetTickCount() + FRAME_TIME - gStartTime ) / float( FRAME_TIME );

    //-------------------------------------------------------------------------

// for (int i = 0; i < FBO_TEXTURE_MAX; ++i)
// {
//     // bind a framebuffer object
//     glBindFramebuffer(GL_FRAMEBUFFER, gFbo);
//     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, gFboTexture[i], 0);

//     //Clear framebuffer
//     video->CleanScreen();
// if(i==1)
// {
//     g_sceneManager->Render(video, prediction * 0.025f);
// }
// else
// {
//     video->DrawFillRect(( int )( 0 ), ( int )( 0 ), SCREEN_W , SCREEN_H);
// }
//     glBindFramebuffer(GL_FRAMEBUFFER, 0);

// }
//     //-------------------------------------------------------------------------

// for (int i = 0; i < FBO_TEXTURE_MAX; ++i)
// {
//     //draw fbo texture
//     // video->DrawCustom( ( int )( SCREEN_W - SCREEN_W/4 ), ( int )( 0 ), SCREEN_W/4 , SCREEN_H/4, gFboTexture[i] );
//     // video->DrawCustom( ( int )( 0 ), ( int )( 0 ), SCREEN_W , SCREEN_H, gFboTexture[i] );
// }

    gmap.Bind(true);
    g_sceneManager->Render(video, prediction * 0.01f);
    gmap.Bind(false);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    g_sceneManager->Render(video, prediction * 0.01f);

    gmap.Draw(0, 0, SCREEN_W/2, SCREEN_H/2);

}

void Draw ( ESContext *esContext )
{
    video->CleanScreen();

    // video->DrawFillRect(glm::vec2(0,0), 100, 100);
    // video->DrawRect(100, 100, 100, 100);
    // video->DrawLine(100, 100, 500, 500);
    // video->DrawCircle(200, 400, 100);

    draw(esContext);
    eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{

}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{

}

void MouseLeft(ESContext *esContext,int x, int y, bool isPressed)
{
    if(isPressed) g_sceneManager->touchActionDown((float)x, (float)y);
    else g_sceneManager->touchActionUp((float)x, (float)y);
}

void MouseRight(ESContext *esContext,int x, int y, bool isPressed)
{
    if(isPressed) g_sceneManager->touchActionDown((float)x, (float)y);
    else g_sceneManager->touchActionUp((float)x, (float)y);
}

void MouseMove(ESContext *esContext,int x, int y)
{
    g_sceneManager->touchActionMove((float)x, (float)y);
}

void CleanUp()
{
    // glDeleteBuffers(1, &vboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
    ESContext esContext;

    esInitContext ( &esContext );

    esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

    if ( Init ( &esContext ) != 0 )
        return 0;

    esRegisterDrawFunc ( &esContext, Draw );
    esRegisterUpdateFunc ( &esContext, Update );
    esRegisterKeyFunc ( &esContext, Key);

    esRegisterMouseLeftFunc(&esContext, MouseLeft);
	esRegisterMouseRightFunc(&esContext, MouseRight);
    esRegisterMouseMoveFunc(&esContext, MouseMove);


    esMainLoop ( &esContext );

    //releasing OpenGL resources
    // CleanUp();

    //identifying memory leaks
    // MemoryDump();
    printf("Press any key...\n");
    // _getch();

    return 0;
}

