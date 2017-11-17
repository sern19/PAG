//
//  Texture.cpp
//  Projekt bazowy OpenGL
//
//  Created by Krystian Owoc on 04.10.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include "Texture.hpp"
#include <stb_image.h>

Texture::Texture()
{
    glGenTextures(NUMBER_OF_TEXTURES, mTextures); //Generowanie kontenera na tekstury
    
    //Wczytywanie domyślnej tekstury Janickiego i cegieł
    try
    {
        loadTexture("Textures/Janicki.png");
        loadTexture("Textures/Bricks.png",1);
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
    if (pTextureNumber>=NUMBER_OF_TEXTURES) throw std::runtime_error("Wybrany numer tekstury jest większy od maksymalnej ilości tekstur");
    
    //stbi_set_flip_vertically_on_load(true); //Obrócenie obrazu przed załadowaniem
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
    else throw std::runtime_error("Błąd odczytu pliku");
}
void Texture::selectActiveTexture(unsigned int pTextureNumber)
{
    
    if (pTextureNumber>=NUMBER_OF_TEXTURES) throw std::runtime_error("Wybrany numer tekstury jest większy od maksymalnej ilości tekstur");
    glActiveTexture(GL_TEXTURE0+pTextureNumber);
    glBindTexture(GL_TEXTURE_2D, mTextures[pTextureNumber]);
}
