// Texture.hpp
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

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture
{
protected:
    std::string mTexturePath;
    GLuint mTexture;
    
    Texture() {};
    virtual void loadTexture(const std::string& pTexturePath);
public:
    Texture(const std::string& pTexturePath);
    Texture(const Texture& pSourceTexture);
    virtual ~Texture();
    virtual void selectActiveTexture(const std::string& pTextureType, const unsigned int& pTextureNumber);
    const std::string& getTexturePath();
};

#endif /* Texture_hpp */
