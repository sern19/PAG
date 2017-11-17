// Mesh.hpp
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

#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct VerticlesColorTexture
{
    glm::vec3 mVerticles={0,0,0};
    glm::vec3 mColor={0,0,0};
    glm::vec2 mTexture={0,0};
    VerticlesColorTexture(glm::vec3 pVerticles) {mVerticles=pVerticles;};
    VerticlesColorTexture(glm::vec3 pVerticles, glm::vec3 pColor) {mVerticles=pVerticles;mColor=pColor;};
    VerticlesColorTexture(float x, float y, float z, float u, float v) {mVerticles=glm::vec3(x,y,z); mTexture=glm::vec2(u,v);};
    VerticlesColorTexture(glm::vec3 pVerticles, glm::vec3 pColor, glm::vec2 pTexture) {mVerticles=pVerticles;mColor=pColor;mTexture=pTexture;};
};

class Mesh
{
private:
    GLuint mVertexBufferObject=NULL;
    GLuint mVertexArrayObject=NULL;
    GLuint mElementObjectBuffer=NULL;
    VerticlesColorTexture mVerticles[36]={
        VerticlesColorTexture(0.5f,  0.5f, -0.5f,  1.0f, 1.0f),
        VerticlesColorTexture(0.5f, -0.5f, -0.5f,  1.0f, 0.0f),
        VerticlesColorTexture(-0.5f, -0.5f, -0.5f,  0.0f, 0.0f),
        VerticlesColorTexture(-0.5f, -0.5f, -0.5f,  0.0f, 0.0f),
        VerticlesColorTexture(-0.5f,  0.5f, -0.5f,  0.0f, 1.0f),
        VerticlesColorTexture(0.5f,  0.5f, -0.5f,  1.0f, 1.0f),
        
        VerticlesColorTexture(-0.5f, -0.5f,  0.5f,  0.0f, 0.0f),
        VerticlesColorTexture(0.5f, -0.5f,  0.5f,  1.0f, 0.0f),
        VerticlesColorTexture(0.5f,  0.5f,  0.5f,  1.0f, 1.0f),
        VerticlesColorTexture(0.5f,  0.5f,  0.5f,  1.0f, 1.0f),
        VerticlesColorTexture(-0.5f,  0.5f,  0.5f,  0.0f, 1.0f),
        VerticlesColorTexture(-0.5f, -0.5f,  0.5f,  0.0f, 0.0f),
        
        VerticlesColorTexture(-0.5f,  0.5f,  0.5f,  1.0f, 0.0f),
        VerticlesColorTexture(-0.5f,  0.5f, -0.5f,  1.0f, 1.0f),
        VerticlesColorTexture(-0.5f, -0.5f, -0.5f,  0.0f, 1.0f),
        VerticlesColorTexture(-0.5f, -0.5f, -0.5f,  0.0f, 1.0f),
        VerticlesColorTexture(-0.5f, -0.5f,  0.5f,  0.0f, 0.0f),
        VerticlesColorTexture(-0.5f,  0.5f,  0.5f,  1.0f, 0.0f),
        
        VerticlesColorTexture(0.5f, -0.5f, -0.5f,  0.0f, 1.0f),
        VerticlesColorTexture(0.5f,  0.5f, -0.5f,  1.0f, 1.0f),
        VerticlesColorTexture(0.5f,  0.5f,  0.5f,  1.0f, 0.0f),
        VerticlesColorTexture(0.5f,  0.5f,  0.5f,  1.0f, 0.0f),
        VerticlesColorTexture(0.5f, -0.5f,  0.5f,  0.0f, 0.0f),
        VerticlesColorTexture(0.5f, -0.5f, -0.5f,  0.0f, 1.0f),
        
        VerticlesColorTexture(-0.5f,  -0.5f, -0.5f,  0.0f, 1.0f),
        VerticlesColorTexture(0.5f,  -0.5f, -0.5f,  1.0f, 1.0f),
        VerticlesColorTexture(0.5f,  -0.5f,  0.5f,  1.0f, 0.0f),
        VerticlesColorTexture(0.5f,  -0.5f,  0.5f,  1.0f, 0.0f),
        VerticlesColorTexture(-0.5f,  -0.5f,  0.5f,  0.0f, 0.0f),
        VerticlesColorTexture(-0.5f,  -0.5f, -0.5f,  0.0f, 1.0f),
        
        
        VerticlesColorTexture(0.5f,  0.5f,  0.5f,  1.0f, 0.0f),
        VerticlesColorTexture(0.5f,  0.5f, -0.5f,  1.0f, 1.0f),
        VerticlesColorTexture(-0.5f,  0.5f, -0.5f,  0.0f, 1.0f),
        VerticlesColorTexture(-0.5f,  0.5f, -0.5f,  0.0f, 1.0f),
        VerticlesColorTexture(-0.5f,  0.5f,  0.5f,  0.0f, 0.0f),
        VerticlesColorTexture(0.5f,  0.5f,  0.5f,  1.0f, 0.0f)
    }; //Tu by się przydało jakiegoś vectora dać, możliwość dynamicznej zmiany zawartości i rozmiaru
    unsigned int mIndices[3]={0,1,2
    }; //Tu to samo co wyżej
    void loadContent();
public:
    Mesh();
    void drawContent();
};

#endif /* Mesh_hpp */
