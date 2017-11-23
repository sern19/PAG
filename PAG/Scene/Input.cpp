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
#include "ModelNodePicker.hpp"
#include "Config.hpp"


#include "Node.hpp"
#include "Transform.hpp"
Input::Input(GLFWwindow* const pWindow): mMouseSensivity(MOUSE_SENSIVITY)
{
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Przechwytuje i ukrywa kursor
    glfwGetCursorPos(pWindow, &mLastMousePosX, &mLastMousePosY);
}

void Input::toggleEditMode(GLFWwindow* const pWindow)
{
    mIsEditMode=!mIsEditMode;
    mIsKeyTPressed=0;
    if (mIsEditMode) glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::processKeyboard(GLFWwindow* const pWindow, Camera* const pCamera)
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
    
    //Uruchomienie trybu edycji
    if (glfwGetKey(pWindow, GLFW_KEY_T)==GLFW_PRESS) mIsKeyTPressed=1;
    if (glfwGetKey(pWindow, GLFW_KEY_T)==GLFW_RELEASE&&mIsKeyTPressed) toggleEditMode(pWindow);
    
    pCamera->moveInDirection(cameraMoveVector);
}

void Input::processMouse(GLFWwindow* const pWindow, Scene* const pScene, std::vector<Model>* const pModels, Camera* const pCamera)
{
    double mousePosX, mousePosY;
    
    glfwGetCursorPos(pWindow, &mousePosX, &mousePosY);

    float offsetX = (mousePosX - mLastMousePosX)*mMouseSensivity;
    float offsetY = (mLastMousePosY - mousePosY)*mMouseSensivity; // Odwrócone, ponieważ współrzędne y zmieniają się od dołu do góry

    mLastMousePosX = mousePosX;
    mLastMousePosY = mousePosY;

    //W trybie edycji środkowy przycisk myszy pozwala poruszać kamerą
    if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_MIDDLE)==GLFW_RELEASE&&mIsMouseMiddlePressed) mIsMouseMiddlePressed=0;
    if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_MIDDLE)==GLFW_PRESS) mIsMouseMiddlePressed=1;
    
    if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_LEFT)==GLFW_RELEASE&&mIsMouseLeftPressed)
    {
        std::pair<int, int> screenSize;
        std::pair<double, double> mousePos;
        if (prev!=NULL) prev->setIsSelected(false);
        glfwGetWindowSize(pWindow, &screenSize.first, &screenSize.second);
        glfwGetCursorPos(pWindow, &mousePos.first, &mousePos.second);
        Node* lil=ModelNodePicker::pickNode(pScene, pModels, screenSize, mousePos);
        prev=lil;
        //if (lil!=NULL) lil->setIsSelected(true);
        mIsMouseLeftPressed=0;
    }
    if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS) mIsMouseLeftPressed=1;
    
    if (!mIsEditMode||mIsMouseMiddlePressed) pCamera->rotateByOffset(offsetX, offsetY);
}

Input::~Input() {}

