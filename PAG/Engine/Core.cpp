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
#include <ctime>

#include "Window.hpp"
#include "Shader.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "Transform.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "UserInterface.hpp"

#include "PointLight.hpp"
#include "DirectionalLight.hpp"
#include "SpotLight.hpp"

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
    if (mLights.size()!=0)
    {
        int i;
        for (i=0;i<mLights.size();i++)
            if (mLights[i]) delete mLights[i];
    }
    if (mLightModel) delete mLightModel;
    if (mWindow) delete mWindow;
    if (mShader) delete mShader;
    if (mScene) delete mScene;
    if (mCamera) delete mCamera;
    if (mInput) delete mInput;
    if (mUI) delete mUI;
}
void Core::display()
{
    int i;
    glClearColor(BACKGROUND_COLOR);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Czyszczenie sceny
    for (i=0;i<mModels.size();i++)
        mModels[i].draw(mShader, mScene);
    for (i=0;i<mLights.size();i++)
        mLights[i]->drawModel(mLightModel, mShader, mScene);
    mUI->draw();
    glfwSwapBuffers(mWindow->getWindow()); //Swap front- i backbuffer
    
    //Zaraz po swap, żeby nie było efektu ducha
    mScene->updateViewSpace(mCamera->generateViewSpace());
    mShader->setVec3("cameraPos", mCamera->getCameraPos());
    //To samo z ruchomym światełkiem
    mLights[3]->setLightPos(glm::vec3(sin(clock()/100000.f)*2, 0.5, cos(clock()/100000.f)*2));
    mLights[3]->setLight(mShader, mScene, 3);
    
    glfwPollEvents(); //Poll dla eventów
}

void Core::loadModels()
{
    mModels.push_back(Model("Models/2B/source/2B.fbx", mShader));
    mModels.push_back(Model("Models/Spheres/source/Spheres.obj", mShader));

    mModels[0].addGLSetting(GL_BLEND);
    mModels[0].addGLSetting(GL_SAMPLE_ALPHA_TO_COVERAGE);

    mModels[0].getRootNode()->getNodeTransform()->setScale(glm::vec3(0.004,0.004,0.004));
    mModels[1].getRootNode()->getNodeTransform()->setScale(glm::vec3(0.3,0.3,0.3));
    
    mModels[1].getRootNode()->getNodeTransform()->setPosition(glm::vec3(0,0.3,0));
    
    mLightModel=new Model("Models/LightSphere/source/LightSphere.obj", mShader);
}

void Core::loadLights()
{
    int i;
    //Kierunkowe
    mLights.push_back(new DirectionalLight(glm::vec3(0,-1,-0.5), glm::vec3(0.5,0.5,0.5), 0));
    //Spotlighty
    mLights.push_back(new SpotLight(glm::vec3(-1,1,1), glm::vec3(0,0.7,0)-glm::vec3(-1,1,1), glm::vec3(1.2,0,0), 8, 0.5, 0));
    mLights.push_back(new SpotLight(glm::vec3(1,1,1), glm::vec3(0,0.7,0)-glm::vec3(1,1,1), glm::vec3(0,0,1.2), 8, 0.5, 0));
    //Punktowe
    mLights.push_back(new PointLight(glm::vec3(2,0.5,2), glm::vec3(1.2,1.2,1.2), 0.5));
    
    for (i=0;i<mLights.size();i++)
        if (mLights[i]) mLights[i]->setLight(mShader, mScene, i);
    
    mShader->setInt("numberOfActiveLights", mLights.size());
}

void Core::mainLoop()
{
    double nextGameTick=glfwGetTime();
    int loops;
    
    //
    //mTemporaryLight=new PointLight(glm::vec3(0,1,1), glm::vec3(1.2,1.2,1.2), 0.5);
    //mTemporaryLight=new SpotLight(glm::vec3(-1,1,1), glm::vec3(0,0.7,0)-glm::vec3(-1,1,1), glm::vec3(1.2,0,0), 8, 0.5, 0); //Aka lewe
    //mTemporaryLight=new DirectionalLight(glm::vec3(0,-1,-0.5), glm::vec3(0.8,0.8,0.8), 0);
    //mTemporaryLight->setLight(mShader, mScene, 0);
    //mShader->setInt("numberOfActiveLights", 2);
    //
    
    loadModels();
    loadLights();
    
    while ((mWindow->getWindow())&&(!glfwWindowShouldClose(mWindow->getWindow())))
    {
        loops=0;
        while ((glfwGetTime()>nextGameTick) && (loops<MAX_FRAMESKIP))
        {
            mInput->processKeyboard(mWindow->getWindow(), mUI, mCamera);
            mInput->processMouse(mWindow->getWindow(), mUI, mScene, &mModels, mCamera);
            
            nextGameTick+=SKIP_TICKS;
            loops++;
        }
        display();
    }
}
