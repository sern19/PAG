// Core.cpp
//
// Copyright (c) 2017 Krystian Owoc
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

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
