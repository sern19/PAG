// Window.cpp
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
Window::Window(const int& pScreenWidth, const int& pScreenHeight)
{
    //Tworzenie okna
    try
    {
        createWindow(pScreenWidth, pScreenHeight);
    } catch (std::runtime_error err)
    {
        throw err;
    }
    
    //Tworzenie kontekstu
    glfwMakeContextCurrent(mWindow);
}

void Window::createWindow(const int& pScreenWidth, const int& pScreenHeight)
{
    mWindow=glfwCreateWindow(pScreenWidth, pScreenHeight, WINDOW_NAME, NULL, NULL);
    if (!mWindow) //W przypadku niepowodzenia zwalniamy zasoby i niszczymy okienka
    {
        glfwTerminate();
        throw std::runtime_error("(Window::createWindow): Nie można utworzyć okna");
    }
}
GLFWwindow* const Window::getWindow()
{
    return mWindow;
    
}
