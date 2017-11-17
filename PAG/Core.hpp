//
//  Core.hpp
//  Projekt bazowy OpenGL
//
//  Created by Krystian Owoc on 03.10.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef Core_hpp
#define Core_hpp

#define GLFW_INCLUDE_NONE

#include <stdio.h>
#include <glad/glad.h> //Do pobierania funkcji
#include <GLFW/glfw3.h> //Główna pętla i inicjalizacja

class Window;
class Mesh;
class Shader;
class Texture;

class Core
{
private:
    Window* mWindow;
    Mesh* mMesh;
    Shader* mShader;
    Texture* mTexture;
    void display();
public:
    Core();
    ~Core();
    void mainLoop();
};

#endif /* Core_hpp */
