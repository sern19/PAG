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
#include "Scene.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "Transform.hpp"

#include <iostream>

#include <glm/gtx/vector_angle.hpp>

Core::Core()
{
    //Wczytanie bliblioteki
    if (!glfwInit()) //Zwraca 0  w przypadku błędu
    throw std::runtime_error("(Core::Core): Nie można zainicjalizować biblioteki GLFW");
    
    //Ustawiene profilu Core, OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Fix na MacOS
#endif
    
    try
    {
        mWindow=new Window();
        //Inicjalizacja GLAD
        if (!gladLoadGL()) throw std::runtime_error("(Core::Core): Nie można zainicjalizować biblioteki GLAD");
        //mMesh=new Mesh();
        mShader=new Shader();
        mTexture=new Texture();
        mScene=new Scene(mWindow->getWindow());
        mCamera=new Camera();
        mInput=new Input(mWindow->getWindow());
        mTransform= new Transform();
    } catch (std::runtime_error err)
    {
        throw err;
    }
    //Ustawienie blendingu do przezroczystych tekstur
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    mShader->useProgram();
}
Core::~Core()
{
    if (mWindow) delete mWindow;
    if (mMesh) delete mMesh;
    if (mShader) delete mShader;
    if (mTexture) delete mTexture;
    if (mScene) delete mScene;
    if (mCamera) delete mCamera;
    if (mInput) delete mInput;
    if (mTransform) delete mTransform;
}
void Core::display()
{
    glm::mat4 transform;
    int i;
    glClearColor(BACKGROUND_COLOR);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Czyszczenie sceny
    mScene->updateViewSpace(mCamera->generateViewSpace());
    mScene->updateWVP(mShader);
    for (i=0;i<mTransform->getAllChildrensCount();i++) //
    {
		transform=mTransform->getChildCombinedTransformRotatedTowardsCamera(mCamera->getCameraPos(), i);
        mShader->setMat4("transform", &transform);
        //mTexture->selectActiveTexture(i%NUMBER_OF_TEXTURES);
        //mMesh->drawContent();
    }
    glfwSwapBuffers(mWindow->getWindow()); //Swap front- i backbuffer
    glfwPollEvents(); //Poll dla eventów
}

void Core::updateObjectsPositions()
{
    //mTransform->setRotation(glm::vec3(0,1,0),glfwGetTime()/3.0f);
    
    ////Dzieci prawego
    //mTransform->getChildren(0)->setRotation(glm::vec3(0,0,1),glfwGetTime()/2.0f);
    //mTransform->getChildren(0)->getChildren(0)->setRotation(glm::vec3(1,0,0),glfwGetTime()/1.5f);
    //mTransform->getChildren(0)->getChildren(1)->setRotation(glm::vec3(0,1,0),glfwGetTime()/1.5f);
    //mTransform->getChildren(0)->getChildren(2)->setRotation(glm::vec3(0,0,1),glfwGetTime()/1.5f);
    ////Dzieci lewego
    //mTransform->getChildren(1)->setRotation(glm::vec3(0,1,1),glfwGetTime()/2.0f);
    //mTransform->getChildren(1)->getChildren(0)->setRotation(glm::vec3(1,0,0),glfwGetTime()/1.5f);
    //mTransform->getChildren(1)->getChildren(1)->setRotation(glm::vec3(0,1,0),glfwGetTime()/1.5f);
    //mTransform->getChildren(1)->getChildren(2)->setRotation(glm::vec3(0,0,1),glfwGetTime()/1.5f);
    ////Dzieci przedniego
    //mTransform->getChildren(2)->setRotation(glm::vec3(1,0,1),glfwGetTime()/2.0f);
    //mTransform->getChildren(2)->getChildren(0)->setRotation(glm::vec3(1,0,0),glfwGetTime()/1.5f);
    //mTransform->getChildren(2)->getChildren(1)->setRotation(glm::vec3(0,1,0),glfwGetTime()/1.5f);
    //mTransform->getChildren(2)->getChildren(2)->setRotation(glm::vec3(0,0,1),glfwGetTime()/1.5f);
}

void Core::initializeTransforms()
{
    int i,j;
    //Hierarchia
    for (i=0;i<3;i++)
    {
        mTransform->pushChildren();
        for(j=0;j<3;j++)
        mTransform->getChildren(i)->pushChildren();
    }
    for (int i=0;i<3;i++)
    mTransform->pushChildren();
    
    //Bazowe
    mTransform->getChildren(0)->setPosition(glm::vec3(-3,0,1.5));
    mTransform->getChildren(1)->setPosition(glm::vec3(3,0,1.5));
    mTransform->getChildren(2)->setPosition(glm::vec3(0,0,-3));
    mTransform->getChildren(3)->setPosition(glm::vec3(-3,0,1.5));
    mTransform->getChildren(4)->setPosition(glm::vec3(3,0,1.5));
    mTransform->getChildren(5)->setPosition(glm::vec3(0,0,-3));
    
    ////Dzieci prawego
    mTransform->getChildren(0)->getChildren(0)->setPosition(glm::vec3(0,-2,0)); //
    mTransform->getChildren(0)->getChildren(0)->setScale(glm::vec3(0.1,0.1,0.1));
    mTransform->getChildren(0)->getChildren(1)->setPosition(glm::vec3(-2,1,0));
    mTransform->getChildren(0)->getChildren(1)->setScale(glm::vec3(0.1,0.1,0.1));
    mTransform->getChildren(0)->getChildren(2)->setPosition(glm::vec3(2,1,0));
    mTransform->getChildren(0)->getChildren(2)->setScale(glm::vec3(0.1,0.1,0.1));
    ////Dzieci lewego
    mTransform->getChildren(1)->getChildren(0)->setPosition(glm::vec3(0,-2,0)); //
    mTransform->getChildren(1)->getChildren(0)->setScale(glm::vec3(0.1,0.1,0.1));
    mTransform->getChildren(1)->getChildren(1)->setPosition(glm::vec3(-2,1,0)); //
    mTransform->getChildren(1)->getChildren(1)->setScale(glm::vec3(0.1,0.1,0.1));
    mTransform->getChildren(1)->getChildren(2)->setPosition(glm::vec3(2,1,0));
    mTransform->getChildren(1)->getChildren(2)->setScale(glm::vec3(0.1,0.1,0.1));
    ////Dzieci przedniego
    mTransform->getChildren(2)->getChildren(0)->setPosition(glm::vec3(0,-2,0)); //
    mTransform->getChildren(2)->getChildren(0)->setScale(glm::vec3(0.1,0.1,0.1));
    mTransform->getChildren(2)->getChildren(1)->setPosition(glm::vec3(-2,1,0));
    mTransform->getChildren(2)->getChildren(1)->setScale(glm::vec3(0.1,0.1,0.1));
    mTransform->getChildren(2)->getChildren(2)->setPosition(glm::vec3(2,1,0));
    mTransform->getChildren(2)->getChildren(2)->setScale(glm::vec3(0.1,0.1,0.1));
}

void Core::mainLoop()
{
    double nextGameTick=glfwGetTime();
    int loops;
    
    mTexture->selectActiveTexture(0);
    mTexture->selectActiveTexture(1);
    
    mShader->setInt("myTexture1", 0);
    mShader->setInt("myTexture2", 1);
    
    glfwSetInputMode(mWindow->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Przechwytuje i ukrywa kursor
    initializeTransforms();
    
    while ((mWindow->getWindow())&&(!glfwWindowShouldClose(mWindow->getWindow())))
    {
        loops=0;
        while ((glfwGetTime()>nextGameTick) && (loops<MAX_FRAMESKIP))
        {
            mInput->processKeyboard(mWindow->getWindow(), mCamera);
            mInput->processMouse(mWindow->getWindow(), mCamera);
            updateObjectsPositions();
            nextGameTick+=SKIP_TICKS;
            loops++;
        }
        display();
    }
}
