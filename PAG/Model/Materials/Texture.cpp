// Texture.cpp
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

#include "Texture.hpp"
#include "Config.hpp"
#include <stb_image.h>
#include <iostream>

Texture::Texture(const std::string& pTexturePath): mTexturePath(pTexturePath)
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

Texture::Texture(const Texture& pSourceTexture):Texture(pSourceTexture.mTexturePath) {}

Texture::~Texture()
{
    glDeleteTextures(1, &mTexture);
}

void Texture::loadTexture(const std::string& pTexturePath)
{
    int width, height, numberOfChannels;
    GLuint imageDepth;
    
    //stbi_set_flip_vertically_on_load(true); //Obrócenie obrazu przed załadowaniem
    unsigned char* textureData=stbi_load(pTexturePath.c_str(), &width, &height, &numberOfChannels, 0);
    if (textureData)
    {
        //Parametry dla wczytanej tekstury
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST ); //Pomniejszenie tekstury, bierze najbliższą mipmapę i interpoluje liniowo
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR ); //Powiększenie tekstury, interpoluje liniowo
        
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT ); //Tekstura S - zawija
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT ); //Tekstura T - zawija
        
        if (numberOfChannels==4) imageDepth=GL_RGBA; //W przypadku gdy obrazek jest 4 kanałowy, tekstura będzie RGBA
        else imageDepth=GL_RGB;
        glBindTexture(GL_TEXTURE_2D, mTexture);
        glTexImage2D(GL_TEXTURE_2D,
                     0, //Stopnie mipmappingu
                     imageDepth, //Wewnętrzny format teksela
                     width, height, //Szerokośc i wysokość obrazka
                     0, //Kontur??
                     imageDepth, //Format danych obrazka
                     GL_UNSIGNED_BYTE, //Typ danych obrazka
                     textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(textureData);
    }
    else throw std::runtime_error("(Texture::loadTexture): Błąd odczytu pliku");
}
void Texture::selectActiveTexture(const std::string& pTextureType, const unsigned int& pTextureNumber)
{
    if (pTextureNumber>=MAXIMUM_NUMBER_OF_TEXTURES_PER_TYPE) std::runtime_error("(Texture::selectActiveTexture): Numer tekstury jest większy od dopuszczalnej ilości tekstur");
    if (pTextureType.compare(DIFFUSE_NAME)==0)
    {
        glActiveTexture(GL_TEXTURE0+pTextureNumber);
    } else if (pTextureType.compare(SPECULAR_NAME)==0)
    {
        glActiveTexture(GL_TEXTURE0+SPECULAR_STARTING_INDEX+pTextureNumber);
    }
    else if (pTextureType.compare(NORMAL_NAME)==0)
    {
        glActiveTexture(GL_TEXTURE0+NORMAL_STARTING_INDEX+pTextureNumber);
    }
    glBindTexture(GL_TEXTURE_2D, mTexture);
}

const std::string& Texture::getTexturePath(){ return mTexturePath; }
