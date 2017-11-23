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

void Mesh::generateOBB()
{
    int i;
    mOBB.first=glm::vec4(mVerticles[0].mPosition, 1);
    mOBB.second=glm::vec4(mVerticles[0].mPosition, 1);
    //Minimum
    for (i=1;i<mVerticles.size();i++)
    {
        if (mVerticles[i].mPosition.x<mOBB.first.x) mOBB.first.x=mVerticles[i].mPosition.x;
        if (mVerticles[i].mPosition.y<mOBB.first.y) mOBB.first.y=mVerticles[i].mPosition.y;
        if (mVerticles[i].mPosition.z<mOBB.first.z) mOBB.first.z=mVerticles[i].mPosition.z;
    }
    //Maximum
    for (i=1;i<mVerticles.size();i++)
    {
        if (mVerticles[i].mPosition.x>mOBB.second.x) mOBB.second.x=mVerticles[i].mPosition.x;
        if (mVerticles[i].mPosition.y>mOBB.second.y) mOBB.second.y=mVerticles[i].mPosition.y;
        if (mVerticles[i].mPosition.z>mOBB.second.z) mOBB.second.z=mVerticles[i].mPosition.z;
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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, mNormal))); //Atrybut koloru - start po wierzchołkach (glm::vec3)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, mTexture))); //Atrubut textury
    
    //Podanie dostępu do wierzchołków w tablicy o indeksie 0-2
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    generateOBB();
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
    glDrawArrays(GL_TRIANGLES, 0, mIndices.size());
    glBindVertexArray(NULL);
}

const bool Mesh::checkRayIntersection(const glm::vec3& pRaySource, const glm::vec3& pRayDirection, const glm::mat4& pTransform, float& pDistanceOutput)
{
    //Transformacja OBB
    std::pair<glm::vec4, glm::vec4> transformedOBB=mOBB;
    transformedOBB.first=transformedOBB.first*pTransform;
    transformedOBB.second=transformedOBB.second*pTransform;
    
    float f,e,t1,t2;
    float tMin=0.0f;
    float tMax=FLT_MAX;
    
    //Używana do przeliczania przecięć z płaszczyznami
    glm::vec3 delta=glm::vec3(pTransform[3].x, pTransform[3].y, pTransform[3].z)-pRaySource;
    
    //Sprawdzanie z dwiema płaszczyznami równoległymi do osi x
    glm::vec3 axisX(pTransform[0].x, pTransform[0].y, pTransform[0].z);
    f=glm::dot(pRayDirection, axisX);
    e=glm::dot(axisX, delta);
    
    if (fabs(f) > 0.001f)
    {
        //Odległości między źródłem, a przecięciami płaszczyzn
        t1=(e+transformedOBB.first.x)/f; //Przecięcie z lewą płaszczyzną
        t2=(e+transformedOBB.second.x)/f; //Przecięcie z prawą płaszczyzną
        
        if (t1>t2)
            std::swap(t1,t2);
        
        if (t2<tMax) tMax=t2; //Najbliższe(bliższa płaszczyzna) dalekie przecięcie
        if (t1>tMin) tMin=t1; //Najdalsze(dalsza płaszczyzna) bliskie przecięcie
        
        //Z rysunku - jeżeli dalekie przecięcie jest bliżej niż bliskie, to nie ma przecięcia
        if (tMin>tMax)
            return false;
    } else //Gdy promień jest prawie równoległy do płaszczyzny
        if (-e+transformedOBB.first.x>0.0f || -e+transformedOBB.second.x<0.0f)
            return false;
    
    //Sprawdzanie z dwiema płaszczyznami równoległymi do osi y
    glm::vec3 axisY(pTransform[1].x, pTransform[1].y, pTransform[1].z);
    f=glm::dot(pRayDirection, axisY);
    e=glm::dot(axisY, delta);
    
    if (fabs(f) > 0.001f)
    {
        //Odległości między źródłem, a przecięciami płaszczyzn
        t1=(e+transformedOBB.first.y)/f; //Przecięcie z lewą płaszczyzną
        t2=(e+transformedOBB.second.y)/f; //Przecięcie z prawą płaszczyzną
        
        if (t1>t2)
            std::swap(t1,t2);
        
        if (t2<tMax) tMax=t2; //Najbliższe(bliższa płaszczyzna) dalekie przecięcie
        if (t1>tMin) tMin=t1; //Najdalsze(dalsza płaszczyzna) bliskie przecięcie
        
        //Z rysunku - jeżeli dalekie przecięcie jest bliżej niż bliskie, to nie ma przecięcia
        if (tMin>tMax)
            return false;
    } else //Gdy promień jest prawie równoległy do płaszczyzny
        if (-e+transformedOBB.first.y>0.0f || -e+transformedOBB.second.y<0.0f)
            return false;
    
    //Sprawdzanie z dwiema płaszczyznami równoległymi do osi z
    glm::vec3 axisZ(pTransform[2].x, pTransform[2].y, pTransform[2].z);
    f=glm::dot(pRayDirection, axisZ);
    e=glm::dot(axisZ, delta);
    
    if (fabs(f) > 0.001f)
    {
        //Odległości między źródłem, a przecięciami płaszczyzn
        t1=(e+transformedOBB.first.z)/f; //Przecięcie z lewą płaszczyzną
        t2=(e+transformedOBB.second.z)/f; //Przecięcie z prawą płaszczyzną
        
        if (t1>t2)
            std::swap(t1,t2);
        
        if (t2<tMax) tMax=t2; //Najbliższe(bliższa płaszczyzna) dalekie przecięcie
        if (t1>tMin) tMin=t1; //Najdalsze(dalsza płaszczyzna) bliskie przecięcie
        
        //Z rysunku - jeżeli dalekie przecięcie jest bliżej niż bliskie, to nie ma przecięcia
        if (tMin>tMax)
            return false;
    } else //Gdy promień jest prawie równoległy do płaszczyzny
        if (-e+transformedOBB.first.z>0.0f || -e+transformedOBB.second.z<0.0f)
            return false;
    
    pDistanceOutput=tMin;
    return true;
}
