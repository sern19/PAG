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
#include <chrono>

#include "Window.hpp"
#include "Shader.hpp"
#include "GBuffer.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "Transform.hpp"
#include "Model.hpp"
#include "Postprocess.hpp"
#include "Skybox.hpp"
#include "ModelCreator.hpp"
#include "Materials.hpp"
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

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Fix na MacOS
#endif
    
    try
    {
        mWindow=new Window(SCREEN_WIDTH, SCREEN_HEIGHT);
        //Inicjalizacja GLAD
        if (!gladLoadGL()) throw std::runtime_error("(Core::Core): Nie można zainicjalizować biblioteki GLAD");
        mGeometryPassShader=new Shader({{"Shaders/geometryPass.vert", GL_VERTEX_SHADER}, {"Shaders/geometryPass.frag", GL_FRAGMENT_SHADER}});
        mStencilTestShader=new Shader({{"Shaders/lightPass.vert", GL_VERTEX_SHADER}, {"Shaders/nullShader.frag", GL_FRAGMENT_SHADER}});
        mLightPassShader=new Shader({{"Shaders/lightPass.vert", GL_VERTEX_SHADER}, {"Shaders/lightPass.frag", GL_FRAGMENT_SHADER}});
		mSkyboxShader = new Shader({ { "Shaders/skybox.vert", GL_VERTEX_SHADER },{ "Shaders/skybox.frag", GL_FRAGMENT_SHADER } });
        mGBuffer=new GBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
        mPostProcess.push_back(Shader({ { "Shaders/hdr.vert", GL_VERTEX_SHADER },{ "Shaders/hdr.frag", GL_FRAGMENT_SHADER } }));
        mScene=new Scene(mWindow->getWindow());
        mCamera=new Camera();
        mInput=new Input(mWindow->getWindow());
        mUI=new UserInterface(mWindow->getWindow());
    } catch (std::runtime_error err)
    {
        throw err;
    }
    //Inicjalizacja
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    //Początkowe wartości lightPassa
    mLightPassShader->useProgram();
    mLightPassShader->setVec2("screenSize",glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
    mLightPassShader->setInt("diffuseMap", 0);
    mLightPassShader->setInt("normalMap", 1);
    mLightPassShader->setInt("positionMap", 2);
    mLightPassShader->setInt("specularColorMap", 3);
    mLightPassShader->setInt("ambientColorMap", 4);
    mLightPassShader->setInt("texCoordsMap", 5);
    //Początkowe wartości skyboxPassa
    mSkyboxShader->useProgram();
    mSkyboxShader->setInt("cubeMap", 31);
    mGeometryPassShader->useProgram();
    mGeometryPassShader->setInt("cubeMap", 31);
}
Core::~Core()
{
    if (mLights.size()!=0)
    {
        int i;
        for (BaseLight* light: mLights)
            if (light) delete light;
    }
    if (mLightModel) delete mLightModel;
	if (mSkybox) delete mSkybox;
    if (mWindow) delete mWindow;
    if (mGeometryPassShader) delete mGeometryPassShader;
    if (mStencilTestShader) delete mStencilTestShader;
    if (mLightPassShader) delete mLightPassShader;
	if (mSkyboxShader) delete mSkyboxShader;
    if (mScene) delete mScene;
    if (mCamera) delete mCamera;
    if (mInput) delete mInput;
    if (mUI) delete mUI;
}

void Core::geometryPass()
{
    mGeometryPassShader->useProgram();
    mGeometryPassShader->setVec3("cameraPos", mCamera->getCameraPos());
    mGBuffer->bindForWritingGeometryPass();
    
    //Żeby tylko geometrypass modyfikował głębokość
    glDepthMask(GL_TRUE);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);

    for (Model& model: mModels)
        model.draw(mGeometryPassShader, mScene->getWVP());
    if (mInput->isEditMode())
        for (BaseLight* light: mLights)
            light->drawModel(mLightModel, mGeometryPassShader, mScene->getWVP());
    glDepthMask(GL_FALSE);
}

void Core::lightPass()
{
    int i;
    mLightPassShader->useProgram();
    mLightPassShader->setVec3("cameraPos", mCamera->getCameraPos());

    //Punktowe
    glEnable(GL_STENCIL_TEST);
    
    for (i=0; i<mLights.size(); i++)
    if (mLights[i] && (dynamic_cast<PointLight*>(mLights[i]) || dynamic_cast<SpotLight*>(mLights[i])))
    {
        //Stencil
        mStencilTestShader->useProgram();
        mGBuffer->bindForStencilPass();
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glClear(GL_STENCIL_BUFFER_BIT);
        glStencilFunc(GL_ALWAYS, 0, 0);
        glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
        glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
        mLights[i]->drawBoundings(mLightModel, mStencilTestShader, mScene->getWVP());
        //
        
        //Punktowe lub stożkowe
        mGBuffer->bindForLightPass();
        mLightPassShader->useProgram();
        glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
        
        //Potrzebujemy blendingu bo każde światło ma swoje własne wywołanie
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);
        
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        
        mLights[i]->setLight(mLightPassShader, mScene);
        mLights[i]->drawBoundings(mLightModel, mLightPassShader, mScene->getWVP());
        glCullFace(GL_BACK);
        
        glDisable(GL_BLEND);
        //
    }
    
    glDisable(GL_STENCIL_TEST);

    //Kierunkowe światło
    mGBuffer->bindForLightPass();
    mLightPassShader->useProgram();
    //Potrzebujemy blendingu bo każde światło ma swoje własne wywołanie
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    for (i=0; i<mLights.size(); i++)
        if (mLights[i] && dynamic_cast<DirectionalLight*>(mLights[i]))
        {
            Model planeModel=ModelCreator::createPlane();
            mLights[i]->setLight(mLightPassShader, mScene);
            mLights[i]->drawBoundings(&planeModel, mLightPassShader, glm::mat4(1));
        }
    glDisable(GL_BLEND);
}

void Core::skyboxPass()
{
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    
    glDepthFunc(GL_LEQUAL);
    mSkyboxShader->useProgram();
    mSkybox->draw(mSkyboxShader, mScene->getWVP());
    glDepthFunc(GL_LESS);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}


void Core::finalPass()
{
    for (PostProcess& postProcess: mPostProcess)
    {
        postProcess.preparePostProcess();
        mGBuffer->bindForPostProcess();
        postProcess.applyPostProcess();
    }
    mGBuffer->bindForFinalPass();
    glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 
                      0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void Core::display()
{
    int i;
    std::chrono::high_resolution_clock::time_point time=std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::ratio<1, 1000>> timePoint=time.time_since_epoch(); //W ms
    
    mScene->updateViewSpace(mCamera->generateViewSpace());
    //mLights[3]->setLightPos(glm::vec3(sin(timePoint.count()/1000.0)*2, 0.5, cos(timePoint.count()/1000.0)*2));
    
    mGBuffer->clearFinalTexture();
    geometryPass();
    lightPass();
	skyboxPass();
    finalPass();

    mUI->draw();
    glfwSwapBuffers(mWindow->getWindow()); //Swap front- i backbuffer
    glfwPollEvents(); //Poll dla eventów
}

void Core::loadModels()
{
    //mModels.push_back(Model("Models/2B/source/2B.fbx"));
    mModels.push_back(Model("Models/Skull/source/Skull.obj",true));
    mModels[0].getMaterials()->getMaterial(0)->mAmbientColor=glm::vec3(1,1,1);
    mModels[0].getRootNode()->getNodeTransform()->setRotation(glm::vec3(0,-90,0));
    mModels[0].getRootNode()->getNodeTransform()->setPosition(glm::vec3(0.5,0,-0.2));

//    mModels[0].getRootNode()->getNodeTransform()->setScale(glm::vec3(0.004,0.004,0.004));
//    mModels[0].getRootNode()->getNodeTransform()->setRotation(glm::vec3(0,-90,0));
//    mModels[0].getRootNode()->getNodeTransform()->setPosition(glm::vec3(-0.5,0,-0.2));

    
    //Transform sponzaScaler;
    //sponzaScaler.setScale(glm::vec3(0.005,0.005,0.005));

    //mModels.push_back(Model("Models/Sponza/source/sponza.obj", &sponzaScaler));
    
    
    mSkybox=new Skybox("Skybox/");
    mLightModel=new Model("Models/LightSphere/source/LightSphere.obj");
}

void Core::loadLights()
{
    int i;
    //Kierunkowe
    mLights.push_back(new DirectionalLight(glm::vec3(0.8, -1, 0), glm::vec3(1.5,1.5,1.5), 0.3));
    //Spotlighty
//    mLights.push_back(new SpotLight(glm::vec3(-1,1,1), glm::vec3(0,0.7,0)-glm::vec3(-1,1,1), glm::vec3(1.2,0,0), 8, 0.5, 0));
//    mLights.push_back(new SpotLight(glm::vec3(1,1,1), glm::vec3(0,0.7,0)-glm::vec3(1,1,1), glm::vec3(0,0,1.2), 8, 0.5, 0));
    //Punktowe
    mLights.push_back(new PointLight(glm::vec3(0.5,0.5,2), glm::vec3(5,1.2,1.2), 0.1));
    mLights.push_back(new PointLight(glm::vec3(0.5,0.5,-2), glm::vec3(1.2,1.2,5), 0.1));
    
    for (i=0; i<mLights.size(); i++)
        if (mLights[i]) mLights[i]->setLight(mLightPassShader, mScene);
    
    mLightPassShader->setInt("numberOfActiveLights", mLights.size());
}

void Core::mainLoop()
{
    double nextGameTick=glfwGetTime();
    int loops;
    
    loadModels();
    loadLights();
    
    Materials::prepareTextureUnits(mGeometryPassShader);
    while ((mWindow->getWindow())&&(!glfwWindowShouldClose(mWindow->getWindow())))
    {
        loops=0;
        while ((glfwGetTime()>nextGameTick) && (loops<MAX_FRAMESKIP))
        {
            mInput->processKeyboard(mWindow->getWindow(), mUI, mCamera);
            mInput->processMouse(mWindow->getWindow(), mUI, mScene, &mModels, &mLights, mLightModel, mCamera);
            
            nextGameTick+=SKIP_TICKS;
            loops++;
        }
        display();
    }
}
