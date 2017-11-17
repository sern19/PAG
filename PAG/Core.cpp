//
//  Core.cpp
//  Projekt bazowy OpenGL
//
//  Created by Krystian Owoc on 03.10.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include "Core.hpp"

#include <stdexcept>
#include "Window.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

Core::Core()
{
    //Wczytanie bliblioteki
    if (!glfwInit()) //Zwraca 0  w przypadku błędu
        throw std::runtime_error("Nie można zainicjalizować biblioteki GLFW");
    
    //Ustawiene profilu Core, OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Fix na MacOS
#endif
    
    try
    {
        mWindow=new Window();
        //Inicjalizacja GLAD
        if (!gladLoadGL())
            throw std::runtime_error("Nie można zainicjalizować biblioteki GLAD");
        mMesh=new Mesh();
        mShader=new Shader();
        mTexture=new Texture();
    } catch (std::runtime_error err)
    {
        throw err;
    }
    //Ustawienie blendingu do przezroczystych tekstur
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    mShader->useProgram();
}
Core::~Core()
{
    if (mWindow) delete mWindow;
    if (mMesh) delete mMesh;
    if (mShader) delete mShader;
    if (mTexture) delete mTexture;
}

void Core::display()
{
    glClearColor(BACKGROUND_COLOR);
    glClear(GL_COLOR_BUFFER_BIT); //Czyszczenie sceny
    //mTexture->selectActiveTexture(ACTIVE_TEXTURE_FOR_PROGRAM);
    mMesh->drawContent();
    glfwSwapBuffers(mWindow->getWindow()); //Swap front- i backbuffer
    glfwPollEvents(); //Poll dla eventów
}

void Core::mainLoop()
{
    double nextGameTick=glfwGetTime();
    int loops;
    
    mTexture->selectActiveTexture(0);
    mTexture->selectActiveTexture(1);
    
    mShader->setInt("myTexture1", 0);
    mShader->setInt("myTexture2", 1);
    
    while ((mWindow->getWindow())&&(!glfwWindowShouldClose(mWindow->getWindow())))
    {
        loops=0;
        while ((glfwGetTime()>nextGameTick) && (loops<MAX_FRAMESKIP))
        {
            //Update gry
            nextGameTick+=SKIP_TICKS;
            loops++;
        }
        display();
    }
}
