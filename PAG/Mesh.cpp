//
//  Mesh.cpp
//  Projekt bazowy OpenGL
//
//  Created by Krystian Owoc on 04.10.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include "Mesh.hpp"

#include <glad/glad.h>


Mesh::Mesh()
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(mVerticles), mVerticles, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementObjectBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndices), mIndices, GL_STATIC_DRAW);
    
    //Informacja o interpretacji danych - indeks, rozmiar, typ, czy powinien nrmalizować, odległość między wierzchołkami (w tablicy), odległość od początku danych (w tablicy)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VerticlesColorTexture), 0); //Atrybut wierzchołków
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VerticlesColorTexture), (GLvoid*)(sizeof(glm::vec3))); //Atrybut koloru - start po wierzchołkach (glm::vec3)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VerticlesColorTexture), (GLvoid*)(sizeof(glm::vec3)*2)); //Atrubut textury
    
    //Podanie dostępu do wierzchołków w tablicy o indeksie 0-2
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

void Mesh::drawContent()
{
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
