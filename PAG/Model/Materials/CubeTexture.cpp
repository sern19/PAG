// CubeTexture.cpp
//
// Copyright (c) 2018 Krystian Owoc
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

#include "CubeTexture.hpp"
#include <stb_image.h>
#include <string>
#include <stdexcept>

CubeTexture::CubeTexture(const std::vector<std::string>& pTexturePath) 
{
    glGenTextures(1, &mTexture); //Generowanie kontenera na tekstury
    
    try
    {
        loadTexture(pTexturePath);
    } catch (std::runtime_error err)
    {
        throw err;
    }
}

void CubeTexture::loadTexture(const std::vector<std::string>& pTexturePath)
{
    int i, width, height, numberOfChannels;
    GLuint imageDepth;
    
    if (pTexturePath.size()!=6) throw std::runtime_error("(CubeTexture::loadTexture): Błędna ilość ścieżek tekstur");
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
    
    unsigned char* textureData;
    for (i=0; i<pTexturePath.size(); i++)
    {
        textureData=stbi_load(pTexturePath[i].c_str(), &width, &height, &numberOfChannels, 0);
        if (textureData)
        {
            if (numberOfChannels==4) imageDepth=GL_RGBA; //W przypadku gdy obrazek jest 4 kanałowy, tekstura będzie RGBA
            else if (numberOfChannels==3) imageDepth=GL_RGB;
            else imageDepth=GL_RED;
            //Parametry dla wczytanej tekstury
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, imageDepth, width, height, 0, imageDepth, GL_UNSIGNED_BYTE, textureData);
            stbi_image_free(textureData);
        } else throw std::runtime_error("(CubeTexture::loadTexture): Błąd odczytu pliku");
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
}

void CubeTexture::selectActiveTexture(const std::string& pTextureType, const unsigned int& pTextureNumber)
{
    glActiveTexture(GL_TEXTURE0+pTextureNumber);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
}
