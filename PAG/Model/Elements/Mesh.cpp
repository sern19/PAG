// Mesh.cpp
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

#include "Mesh.hpp"

#include "Shader.hpp"
#include "Materials.hpp"
#include "ModelNodePicker.hpp"
#include <glad/glad.h>


Mesh::Mesh(const std::vector<Vertex>& pVerticles, const std::vector<unsigned int>& pIndices): mVerticles(pVerticles), mIndices(pIndices)
{
    calculateTanBitan();
    loadContent();
}

void Mesh::calculateTanBitan()
{
    int i;
    for (i=0;i<mIndices.size();i+=3)
    {
        //Shortcuts for vertices
        glm::vec3& v0 = mVerticles[mIndices[i]].mPosition;
        glm::vec3& v1 = mVerticles[mIndices[i+1]].mPosition;
        glm::vec3& v2 = mVerticles[mIndices[i+2]].mPosition;
        
        //Shortcuts for UVs
        glm::vec2& uv0 = mVerticles[mIndices[i]].mTexture;
        glm::vec2& uv1 = mVerticles[mIndices[i+1]].mTexture;
        glm::vec2& uv2 = mVerticles[mIndices[i+2]].mTexture;
        
        //Edges of the triangle : postion delta
        glm::vec3 deltaPos1 = v1-v0;
        glm::vec3 deltaPos2 = v2-v0;
        
        //UV delta
        glm::vec2 deltaUV1 = uv1-uv0;
        glm::vec2 deltaUV2 = uv2-uv0;
        
        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
        glm::vec3 biTangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;
        
        mVerticles[mIndices[i]].mTangent=tangent;
        mVerticles[mIndices[i+1]].mTangent=tangent;
        mVerticles[mIndices[i+2]].mTangent=tangent;
        
        mVerticles[mIndices[i]].mBitangent=biTangent;
        mVerticles[mIndices[i+1]].mBitangent=biTangent;
        mVerticles[mIndices[i+2]].mBitangent=biTangent;
    }
    for (Vertex& vertex: mVerticles) {
        if (glm::dot(glm::cross(vertex.mNormal, vertex.mTangent), vertex.mBitangent) < 0.0f)
        {
            vertex.mTangent*=-1.0f;
        }
    }
}

void Mesh::loadContent()
{
    //Generowanie tablicy obiektów
    glGenVertexArrays(1, &mVertexArrayObject);
    //Generowanie nowego bufora o rozmiarze 1
    glGenBuffers(1, &mVertexBufferObject);
    //Generowanie bufora elementów
    glGenBuffers(1, &mElementObjectBuffer);
    
    //Bindowanie tablicy obiektów
    glBindVertexArray(mVertexArrayObject);
    
    //Bindowanie bufora, informacja, że zawiera on tablicę wierzchołków
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    //Wypełnienie bufora danymi o wierzchołkach (STATIC_DRAW - wyznaczone raz i odczytywane wielokrotnie)
    glBufferData(GL_ARRAY_BUFFER, mVerticles.size()*sizeof(Vertex), &mVerticles[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementObjectBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);
    
    //Informacja o interpretacji danych - indeks, rozmiar, typ, czy powinien nrmalizować, odległość między wierzchołkami (w tablicy), odległość od początku danych (w tablicy)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); //Atrybut wierzchołków
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, mNormal)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, mTangent)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, mBitangent)));
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, mTexture)));
    
    //Podanie dostępu do wierzchołków w tablicy o indeksie 0-2
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(3);
}

void Mesh::setMaterial(const unsigned int& pMaterialID) { mMaterialID=pMaterialID; }
void Mesh::disableMaterialUsage() { mShouldUseMaterial=false; }
void Mesh::setIsSelected(const bool& pIsSelected) { mIsSelected=pIsSelected; }

void Mesh::bakeTransfrom(const glm::mat4& pBakeTransform, const glm::mat3& pNormalBakeTransform)
{
    for (Vertex& vertex: mVerticles)
    {
        vertex.mPosition=glm::vec3(pBakeTransform*glm::vec4(vertex.mPosition,1.0f));
        vertex.mNormal=pNormalBakeTransform*vertex.mNormal;
    }
    glDeleteVertexArrays(1,&mVertexArrayObject);
    glDeleteBuffers(1,&mVertexBufferObject);
    glDeleteBuffers(1,&mElementObjectBuffer);
    loadContent();
}

void Mesh::drawContent(Shader* const pShader, Materials* const pMaterials)
{
    if (!mIsSelected&&mShouldUseMaterial) pMaterials->setActiveMaterial(mMaterialID, pShader);
    else pMaterials->setDefaultMaterial(pShader);
    
    //Bindowanie tablicy obiektów
    glBindVertexArray(mVertexArrayObject);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(NULL);
}

const std::pair<glm::vec4, glm::vec4> Mesh::getMinMaxVerticles()
{
    int i;
    std::pair<glm::vec4, glm::vec4> output(glm::vec4(FLT_MAX), glm::vec4(FLT_MIN));
    
    if (mVerticles.size()==0) return output;
    
    output.first=glm::vec4(mVerticles[0].mPosition, 1);
    output.second=glm::vec4(mVerticles[0].mPosition, 1);
    //Minimum
    for (i=1;i<mVerticles.size();i++)
    {
        if (mVerticles[i].mPosition.x<output.first.x) output.first.x=mVerticles[i].mPosition.x;
        if (mVerticles[i].mPosition.y<output.first.y) output.first.y=mVerticles[i].mPosition.y;
        if (mVerticles[i].mPosition.z<output.first.z) output.first.z=mVerticles[i].mPosition.z;
    }
    //Maximum
    for (i=1;i<mVerticles.size();i++)
    {
        if (mVerticles[i].mPosition.x>output.second.x) output.second.x=mVerticles[i].mPosition.x;
        if (mVerticles[i].mPosition.y>output.second.y) output.second.y=mVerticles[i].mPosition.y;
        if (mVerticles[i].mPosition.z>output.second.z) output.second.z=mVerticles[i].mPosition.z;
    }
    return output;
}

const bool Mesh::checkRayIntersections(const glm::vec3& pRaySource, const glm::vec3& pRayDirection, const glm::mat4& pTransform, float& pDistanceOutput)
{
    int i;
    glm::vec3 triangle[3];
    float distance;
    std::vector<float> distances;
    //Sprawdzanie dla każdego trójkąta odległości
    for (i=0;i<mIndices.size();i+=3)
    {
        //Transformacja
        triangle[0]=glm::vec3(pTransform*glm::vec4(mVerticles[mIndices[i]].mPosition,1.0f)); //PAMIĘTAĆ NA PRZYSZŁOŚĆ O KOLEJNOŚCI - NIE STRACISZ BEZ SENSU GODZINY
        triangle[1]=glm::vec3(pTransform*glm::vec4(mVerticles[mIndices[i+1]].mPosition,1.0f));
        triangle[2]=glm::vec3(pTransform*glm::vec4(mVerticles[mIndices[i+2]].mPosition,1.0f));
        if (ModelNodePicker::checkRayIntersectionTriangle(pRaySource, pRayDirection, triangle, distance)) distances.push_back(distance);
    }
    if (distances.size()==0) return false;
    pDistanceOutput=*std::min_element(std::begin(distances), std::end(distances));
    return true;
}
