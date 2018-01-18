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
#include <vector>
#include "Material.hpp"

class Shader;
class Materials;

struct Vertex
{
    glm::vec3 mPosition={0,0,0};
    glm::vec3 mNormal={0,0,0};
    glm::vec3 mTangent={0,0,0};
    glm::vec3 mBitangent={0,0,0};
    glm::vec2 mTexture={0,0};
    Vertex() { };
    Vertex(const glm::vec3& pPosition) {mPosition=pPosition;};
    Vertex(const glm::vec3& pPosition, const glm::vec3& pNormal) {mPosition=pPosition;mNormal=pNormal;};
    Vertex(const float& x, const float& y, const float& z, const float& u, const float& v) {mPosition=glm::vec3(x,y,z); mTexture=glm::vec2(u,v);};
    Vertex(const glm::vec3& pPosition, const glm::vec3& pNormal, const glm::vec2& pTexture) {mPosition=pPosition;mNormal=pNormal;mTexture=pTexture;};
};

class Mesh
{
private:
    GLuint mVertexBufferObject=NULL;
    GLuint mVertexArrayObject=NULL;
    GLuint mElementObjectBuffer=NULL;
    std::vector<Vertex> mVerticles;
    std::vector<unsigned int> mIndices;
    unsigned int mMaterialID=0;
    
    bool mIsSelected=false;
    bool mShouldUseMaterial=true;
    
    void fixMirroredTan();
    void loadContent();
public:
    Mesh(const std::vector<Vertex>& pVerticles, const std::vector<unsigned int>& pIndices);
    
    void setMaterial(const unsigned int& pMaterialID);
    void disableMaterialUsage();
    void setIsSelected(const bool& pIsSelected);
    
    void bakeTransfrom(const glm::mat4& pBakeTransform, const glm::mat3& pNormalBakeTransform);
    
	void drawContent(Shader* const pShader);
    void drawContent(Shader* const pShader, Materials* const pTextures);
    
    const std::pair<glm::vec4, glm::vec4> getMinMaxVerticles();
    const bool checkRayIntersections(const glm::vec3& pRaySource, const glm::vec3& pRayDirection, const glm::mat4& pTransform, float& pDistanceOutput);
};

#endif /* Mesh_hpp */
