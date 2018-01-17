// Scene.cpp
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

#include "Scene.hpp"
#include "Shader.hpp"
#include "Config.hpp"

#include <glm/gtc/matrix_transform.hpp>

Scene::Scene(GLFWwindow* const pWindow)
{
    int screenWidth, screenHeight;
    //Inicjalizacja macierzy świata oraz widoku
    mWorldSpace=glm::mat4(1.0f);
    mViewSpace=glm::lookAt(CAMERA_STARTING_POS,  // camera position in world space
                           glm::vec3(0.0f, 0.0f, 0.0f),  // at this point camera is looking
                           glm::vec3(0.0f, 1.0f, 0.0f));
    
    //Inicjalizacja macierzy projekcji
    glfwGetWindowSize(pWindow, &screenWidth, &screenHeight);
    mProjectionSpace=glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.001f, 50.0f); //FOV, Aspect Ratio, zNear, zFar
    updateWVP();
}

void Scene::updateWVP()
{
    //Ustawinie macierzy WVP
    if (mNeedsUpdateWVP) mWVP = mProjectionSpace * mViewSpace * mWorldSpace; //Musi być w odwrotnej kolejności
}

const glm::mat4& Scene::getWorldSpace() { return mWorldSpace; }
const glm::mat4& Scene::getViewSpace() { return mViewSpace; }
const glm::mat4& Scene::getProjectionSpace() { return mProjectionSpace; }
const glm::mat4& Scene::getWVP() { updateWVP(); return mWVP; }
void Scene::updateWorldSpace(const glm::mat4& pWorldSpace) { mWorldSpace=pWorldSpace; mNeedsUpdateWVP=true; }
void Scene::updateViewSpace(const glm::mat4& pViewSpace) { mViewSpace=pViewSpace; mNeedsUpdateWVP=true; }

Scene::~Scene() {}
