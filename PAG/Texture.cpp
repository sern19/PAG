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
#include <stb_image.h>

Texture::Texture()
{
    glGenTextures(NUMBER_OF_TEXTURES, mTextures); //Generowanie kontenera na tekstury
    
    //Wczytywanie domyślnej tekstury Janickiego i cegieł
    try
    {
        loadTexture("Textures/Janicki.png");
        loadTexture("Textures/Beata.png",1);
        loadTexture("Textures/Kotlet.png",2);
    } catch (std::runtime_error err)
    {
        throw err;
    }
}
Texture::~Texture()
{
    glDeleteTextures(NUMBER_OF_TEXTURES, mTextures);
}

void Texture::loadTexture(std::string pFileName, unsigned int pTextureNumber)
{
    int width, height, numberOfChannels;
    GLuint imageDepth;
    if (pTextureNumber>=NUMBER_OF_TEXTURES) throw std::runtime_error("(Texture::loadTexture): Wybrany numer tekstury jest większy od maksymalnej ilości tekstur");
    
    stbi_set_flip_vertically_on_load(true); //Obrócenie obrazu przed załadowaniem
    unsigned char* textureData=stbi_load(pFileName.c_str(), &width, &height, &numberOfChannels, 0);
    if (textureData)
    {
        //Parametry dla wczytanej tekstury
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST ); //Pomniejszenie tekstury, bierze najbliższą mipmapę i interpoluje liniowo
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR ); //Powiększenie tekstury, interpoluje liniowo
        
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT ); //Tekstura S - zawija
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT ); //Tekstura T - zawija
        
        if (numberOfChannels==4) imageDepth=GL_RGBA; //W przypadku gdy obrazek jest 4 kanałowy, tekstura będzie RGBA
        else imageDepth=GL_RGB;
        glBindTexture(GL_TEXTURE_2D, mTextures[pTextureNumber]);
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
void Texture::selectActiveTexture(unsigned int pTextureNumber)
{
    
    if (pTextureNumber>=NUMBER_OF_TEXTURES) throw std::runtime_error("(Texture::selectActiveTexture): Wybrany numer tekstury jest większy od maksymalnej ilości tekstur");
    //glActiveTexture(GL_TEXTURE0+pTextureNumber);
    glBindTexture(GL_TEXTURE_2D, mTextures[pTextureNumber]);
}
