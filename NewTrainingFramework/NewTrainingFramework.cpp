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
#include "Cube.h"
#include <iostream>

GLuint vboId;
Shaders myShaders;
GameObject::GameMap gmap;
Cube cube;

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

    gmap.Init("../Resources/Shaders/GameMap.vs", "../Resources/Shaders/GameMap.fs");
    cube.Init("../Resources/Shaders/Cube.vs", "../Resources/Shaders/Cube.fs");
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

    gmap.Bind(true);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    g_sceneManager->Render(video, prediction * 0.01f);

    // video->DrawLine( glm::vec2(0, 0), glm::vec2(SCREEN_W, 0) );
    // video->DrawLine( glm::vec2(SCREEN_W, 0), glm::vec2(SCREEN_W, SCREEN_H) );
    // video->DrawLine( glm::vec2(SCREEN_W, SCREEN_H), glm::vec2(0, SCREEN_H) );
    // video->DrawLine( glm::vec2(0, SCREEN_H), glm::vec2(0, 0) );

    gmap.Bind(false);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // gmap.Draw(0, 0, SCREEN_W, SCREEN_H);
    g_sceneManager->Render(video, prediction * 0.01f);


}

void Draw ( ESContext *esContext )
{
    // video->CleanScreen();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // video->DrawFillRect(glm::vec2(0,0), 100, 100);
    // video->DrawRect(100, 100, 100, 100);
    // video->DrawLine(100, 100, 500, 500);
    // video->DrawCircle(200, 400, 100);

    // draw(esContext);
    cube.Draw();

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

