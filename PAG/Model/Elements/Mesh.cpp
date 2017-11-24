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
#include "Textures.hpp"
#include <glad/glad.h>


Mesh::Mesh(const std::vector<Vertex>& pVerticles, const std::vector<unsigned int>& pIndices): mVerticles(pVerticles), mIndices(pIndices)
{
    loadContent();
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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, mNormal))); //Atrybut koloru - start po wierzchołkach (glm::vec3)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, mTexture))); //Atrubut textury
    
    //Podanie dostępu do wierzchołków w tablicy o indeksie 0-2
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

void Mesh::setMaterial(const Material& pMaterial) { mMaterial=pMaterial; }
void Mesh::setIsSelected(const bool& pIsSelected) { mIsSelected=pIsSelected; }

void Mesh::drawContent(Shader* const pShader, Textures* const pTextures)
{
    Material temporaryMaterial;
    if (!mIsSelected) temporaryMaterial=mMaterial;
    pTextures->setActiveTextures(temporaryMaterial, pShader);
    //Bindowanie tablicy obiektów
    glBindVertexArray(mVertexArrayObject);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(NULL);
}

const bool Mesh::checkRayIntersection(const glm::vec3& pRaySource, const glm::vec3& pRayDirection, const glm::vec3 triangle[3], const glm::mat4& pTransform, float& pDistanceOutput)
{
    const float epsilon = 0.00000001;
    glm::vec3 edge1, edge2, h, s, q;
    float a,f,u,v,t;
    
    //Obliczanie krawędzi
    edge1=triangle[1]-triangle[0];
    edge2=triangle[2]-triangle[0];
    h=glm::cross(pRayDirection, edge2);
    a=glm::dot(edge1, h);
    if (fabs(a)< epsilon)
        return false;
    f=1/a;
    s=pRaySource-triangle[0];
    u=f*glm::dot(s,h);
    if (u<0.0||u>1.0)
        return false;
    q=glm::cross(s, edge1);
    v=f*glm::dot(pRayDirection, q);
    if (v<0.0||(u+v)>1.0)
        return false;
    t=f*glm::dot(edge2,q);
    if (t>epsilon)
    {
        pDistanceOutput=glm::distance(pRaySource, pRaySource+(pRayDirection*t));
        return true;
    }
    else
        return false;
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
        if (checkRayIntersection(pRaySource, pRayDirection, triangle, pTransform, distance)) distances.push_back(distance);
    }
    if (distances.size()==0) return false;
    pDistanceOutput=*std::min_element(std::begin(distances), std::end(distances));
    return true;
}
