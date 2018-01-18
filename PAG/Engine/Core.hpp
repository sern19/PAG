// Core.hpp
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

#ifndef Core_hpp
#define Core_hpp

#define GLFW_INCLUDE_NONE

#include <stdio.h>
#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h> //Do pobierania funkcji
#include <GLFW/glfw3.h> //Główna pętla i inicjalizacja

class Window;
class Shader;
class GBuffer;
class Scene;
class Camera;
class Input;
class Model;
class BaseLight;
class UserInterface;

class Core
{
private:
    Window* mWindow=NULL;
    Shader* mGeometryPassShader=NULL;
    Shader* mStencilTestShader=NULL;
    Shader* mLightPassShader=NULL;
    GBuffer* mGBuffer=NULL;
    Scene* mScene=NULL;
    Camera* mCamera=NULL;
    Input* mInput=NULL;
    UserInterface* mUI=NULL;
    
    Model* mLightModel=NULL;
    std::vector<Model> mModels;
    std::vector<BaseLight*> mLights;
    
    void geometryPass();
    void debugPass();
    void lightPass();
    void stencilPass();
    void finalPass();
    void display();
    void loadModels();
    void loadLights();
public:
    Core();
    ~Core();
    void mainLoop();
};

#endif /* Core_hpp */
