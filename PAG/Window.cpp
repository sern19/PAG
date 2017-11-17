//
//  Window.cpp
//  Projekt bazowy OpenGL
//
//  Created by Krystian Owoc on 03.10.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include "Window.hpp"
#include <stdexcept>

Window::Window()
{
    //Tworzenie okna
    try
    {
        createWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
    } catch (std::runtime_error err)
    {
        throw err;
    }
    
    //Tworzenie kontekstu
    glfwMakeContextCurrent(mWindow);
}
Window::Window(int pScreenWidth, int pScreenHeight)
{
    //Tworzenie okna
    try
    {
        createWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
    } catch (std::runtime_error err)
    {
        throw err;
    }
    
    //Tworzenie kontekstu
    glfwMakeContextCurrent(mWindow);
}

void Window::createWindow(int pScreenWidth, int pScreenHeight)
{
    mWindow=glfwCreateWindow(pScreenWidth, pScreenHeight, WINDOW_NAME, NULL, NULL);
    if (!mWindow) //W przypadku niepowodzenia zwalniamy zasoby i niszczymy okienka
    {
        glfwTerminate();
        throw std::runtime_error("Nie można utworzyć okna");
    }
}
GLFWwindow* Window::getWindow()
{
    return mWindow;
    
}
