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

void Mesh::drawContent(Shader* const pShader, Textures* const pTextures)
{
    pTextures->setActiveTextures(mMaterial, pShader);
    //Bindowanie tablicy obiektów
    glBindVertexArray(mVertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, mIndices.size());
    glBindVertexArray(NULL);
}
