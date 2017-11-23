// Input.hpp
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

#ifndef Input_hpp
#define Input_hpp

#define GLFW_INCLUDE_NONE

#include <stdio.h>
#include <vector>
#include <GLFW/glfw3.h>

class Camera;
class Scene;
class Model;
class Node;

class Input
{
private:
    double mLastMousePosX;
    double mLastMousePosY;
    const double mMouseSensivity;
    
    Node* prev=NULL;
    
    bool mIsEditMode=false;
    bool mIsKeyTPressed=0;
    bool mIsMouseMiddlePressed=0;
    bool mIsMouseLeftPressed=0;
    
    void toggleEditMode(GLFWwindow* const pWindow);
public:
    Input(GLFWwindow* const pWindow);
    ~Input();
    void processKeyboard(GLFWwindow* const pWindow, Camera* const pCamera);
    void processMouse(GLFWwindow* const pWindow, Scene* const pScene, std::vector<Model>* const pModels, Camera* const pCamera);
};

#endif /* Input_hpp */
