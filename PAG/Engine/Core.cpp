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
#include "Shader.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "Transform.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "UserInterface.hpp"

#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>

Core::Core()
{
    //Wczytanie bliblioteki
    if (!glfwInit()) //Zwraca 0  w przypadku błędu
    throw std::runtime_error("(Core::Core): Nie można zainicjalizować biblioteki GLFW");
    
    //Ustawiene profilu Core, OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Fix na MacOS
#endif
    
    try
    {
        mWindow=new Window();
        //Inicjalizacja GLAD
        if (!gladLoadGL()) throw std::runtime_error("(Core::Core): Nie można zainicjalizować biblioteki GLAD");
        mShader=new Shader();
        mScene=new Scene(mWindow->getWindow());
        mCamera=new Camera();
        mInput=new Input(mWindow->getWindow());
        mUI=new UserInterface(mWindow->getWindow());
    } catch (std::runtime_error err)
    {
        throw err;
    }
    //Inicjalizacja
    glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE); //Psuje piórka
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    mShader->useProgram();
}
Core::~Core()
{
    if (mWindow) delete mWindow;
    if (mShader) delete mShader;
    if (mScene) delete mScene;
    if (mCamera) delete mCamera;
    if (mInput) delete mInput;
    if (mUI) delete mUI;
}
void Core::display()
{
    glm::mat4 transform;
    int i;
    glClearColor(BACKGROUND_COLOR);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Czyszczenie sceny
    mScene->updateViewSpace(mCamera->generateViewSpace());
    mScene->updateWVP(mShader);
    for (i=0;i<mModels.size();i++)
        mModels[i].draw(mShader);
    mUI->draw();
    glfwSwapBuffers(mWindow->getWindow()); //Swap front- i backbuffer
    glfwPollEvents(); //Poll dla eventów
}

void Core::updateObjectsPositions()
{
}

void Core::loadModels()
{
    mModels.push_back(Model("Models/2B/source/2B.fbx", mShader));
    mModels.push_back(Model("Models/Chopper/source/chopper.obj", mShader));
    mModels.push_back(Model("Models/Nanosuit/source/nanosuit.obj", mShader));
    mModels.push_back(Model("Models/Cubes/source/Cubes.fbx", mShader));

    mModels[0].addGLSetting(GL_BLEND);
    mModels[0].addGLSetting(GL_SAMPLE_ALPHA_TO_COVERAGE);

    mModels[0].getRootNode()->getNodeTransform()->setScale(glm::vec3(0.01,0.01,0.01));
    mModels[1].getRootNode()->getNodeTransform()->setScale(glm::vec3(0.006,0.006,0.006));
    mModels[2].getRootNode()->getNodeTransform()->setScale(glm::vec3(0.059,0.059,0.059));
    mModels[3].getRootNode()->getNodeTransform()->setScale(glm::vec3(0.005,0.005,0.005));

    mModels[0].getRootNode()->getNodeTransform()->setPosition(glm::vec3(-0.5,0,0));
    mModels[1].getRootNode()->getNodeTransform()->setPosition(glm::vec3(0.5,0,0));
    mModels[3].getRootNode()->getNodeTransform()->setPosition(glm::vec3(0,0,-1));
}

void Core::mainLoop()
{
    double nextGameTick=glfwGetTime();
    int loops;
    
    loadModels();
    
    while ((mWindow->getWindow())&&(!glfwWindowShouldClose(mWindow->getWindow())))
    {
        loops=0;
        while ((glfwGetTime()>nextGameTick) && (loops<MAX_FRAMESKIP))
        {
            mInput->processKeyboard(mWindow->getWindow(), mUI, mCamera);
            mInput->processMouse(mWindow->getWindow(), mUI, mScene, &mModels, mCamera);
            updateObjectsPositions();
            nextGameTick+=SKIP_TICKS;
            loops++;
        }
        display();
    }
}
