//
//  Window.hpp
//  Projekt bazowy OpenGL
//
//  Created by Krystian Owoc on 03.10.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef Window_hpp
#define Window_hpp

#include <stdio.h>
#include <GLFW/glfw3.h> //Do tworzenia okna
#include "Config.hpp"

class Window
{
private:
    GLFWwindow *mWindow;
    void createWindow(int pScreenWidth, int pScreenHeight);
public:
    Window();
    Window(int pScreenWidth, int pScreenHeight);
    GLFWwindow* getWindow();
};

#endif /* Window_hpp */
