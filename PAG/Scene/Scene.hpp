// Scene.hpp
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

#ifndef Scene_hpp
#define Scene_hpp

#define GLFW_INCLUDE_NONE

#include <stdio.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Shader;

class Scene
{
private:
    glm::mat4 mWorldSpace;
    glm::mat4 mViewSpace;
    glm::mat4 mProjectionSpace;
    glm::mat4 mWVP; //World View Projection
    
    bool mNeedsUpdateWVP=true;
    
    void updateWVP();
public:
    Scene(GLFWwindow* const pWindow);
    const glm::mat4& getWorldSpace();
    const glm::mat4& getViewSpace();
    const glm::mat4& getProjectionSpace();
    const glm::mat4& getWVP();
    void updateWorldSpace(const glm::mat4& pWorldSpace);
    void updateViewSpace(const glm::mat4& pViewSpace);
    ~Scene();
};

#endif /* Scene_hpp */
