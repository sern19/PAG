//
//  main.cpp
//  Projekt bazowy OpenGL
//
//  Created by Krystian Owoc on 03.10.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#define  STB_IMAGE_IMPLEMENTATION

#include <stdexcept>
#include <iostream>
#include "Core.hpp"

int main(int argc, const char * argv[])
{
    try
    {
        Core app;
        app.mainLoop();
    } catch (std::runtime_error err)
    {
        std::cout << err.what();
        return -1;
    }
    return 0;
}

