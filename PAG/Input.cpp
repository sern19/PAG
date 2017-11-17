// Input.cpp
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

#include "Input.hpp"
#include "Camera.hpp"
#include "Config.hpp"

Input::Input(GLFWwindow *pWindow): mMouseSensivity(MOUSE_SENSIVITY)
{
    glfwGetCursorPos(pWindow, &mLastMousePosX, &mLastMousePosY);
}

void Input::processKeyboard(GLFWwindow *pWindow, Camera *pCamera)
{
    glm::vec3 cameraMoveVector(0.0f);
    
    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
        cameraMoveVector[0]=1;
    else if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
        cameraMoveVector[0]=-1;
    if (glfwGetKey(pWindow, GLFW_KEY_R) == GLFW_PRESS)
        cameraMoveVector[1]=-1;
    else if (glfwGetKey(pWindow, GLFW_KEY_F) == GLFW_PRESS)
        cameraMoveVector[1]=1;
    if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
        cameraMoveVector[2]=-1;
    else if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
        cameraMoveVector[2]=1;
    
    if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
    
    pCamera->moveInDirection(cameraMoveVector);
}

void Input::processMouse(GLFWwindow *pWindow, Camera *pCamera)
{
    double mousePosX, mousePosY;
    glfwGetCursorPos(pWindow, &mousePosX, &mousePosY);
    
    float offsetX = (mousePosX - mLastMousePosX)*mMouseSensivity;
    float offsetY = (mLastMousePosY - mousePosY)*mMouseSensivity; // Odwrócone, ponieważ współrzędne y zmieniają się od dołu do góry
    
    mLastMousePosX = mousePosX;
    mLastMousePosY = mousePosY;
    
    pCamera->rotateByOffset(offsetX, offsetY);
}

Input::~Input() {}

