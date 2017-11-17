//
//  Texture.hpp
//  Projekt bazowy OpenGL
//
//  Created by Krystian Owoc on 04.10.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Config.hpp"

class Texture
{
private:
    GLuint mTextures[NUMBER_OF_TEXTURES]; //Tu by można spróbować vectora i dynamicznie dodawać i usuwać
public:
    Texture();
    ~Texture();
    void loadTexture(std::string pFileName, unsigned int pTextureNumber=0);
    void selectActiveTexture(unsigned int pTextureNumber=0);
};

#endif /* Texture_hpp */
