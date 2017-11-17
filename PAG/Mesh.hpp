//
//  Mesh.hpp
//  Projekt bazowy OpenGL
//
//  Created by Krystian Owoc on 04.10.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

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
    VerticlesColorTexture(glm::vec3 pVerticles, glm::vec3 pColor, glm::vec2 pTexture) {mVerticles=pVerticles;mColor=pColor;mTexture=pTexture;};
};

class Mesh
{
private:
    GLuint mVertexBufferObject=NULL;
    GLuint mVertexArrayObject=NULL;
    GLuint mElementObjectBuffer=NULL;
    VerticlesColorTexture mVerticles[4]={ VerticlesColorTexture(glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)), //GP
        VerticlesColorTexture(glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)), //DP
        VerticlesColorTexture(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3( 0.0f, 0.0f, 1.0f)), //DL
      VerticlesColorTexture(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)) //DG
    }; //Tu by się przydało jakiegoś vectora dać, możliwość dynamicznej zmiany zawartości i rozmiaru
    unsigned int mIndices[6]={0,1,2,
        0,2,3}; //Tu to samo co wyżej
    void loadContent();
public:
    Mesh();
    void drawContent();
};

#endif /* Mesh_hpp */
