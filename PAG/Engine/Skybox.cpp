// Model.cpp
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

#include "Skybox.hpp"
#include "Shader.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Config.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Skybox::Skybox(const std::string pTexturePath)
{
    int i;
    std::vector<std::string> texturesPath;
    
    for (i=0;i<6;i++)
    {
        std::string path;
        path+=pTexturePath;
        path+=std::to_string(i);
        path+=".tga";
        texturesPath.push_back(path);
    }
    
	mTexture = new CubeTexture(texturesPath);
	
    createMesh();
    
	mTransform = new Transform();
	mTransform->setScale(glm::vec3(10, 10, 10));
}

Skybox::Skybox(const Skybox & pSourceSkybox)
{
	mMeshes = pSourceSkybox.mMeshes;
	mTexture = new CubeTexture(*pSourceSkybox.mTexture);
	mTransform = new Transform(*pSourceSkybox.mTransform);
}

Skybox::~Skybox()
{
	if (mTransform) delete mTransform;
    if (mTexture) delete mTexture;
}

void Skybox::createMesh()
{
    int i,j;
    
    std::vector<Vertex> planeVertex(4);
    std::vector<unsigned int> planeIndices={2, 1, 0, 2, 3, 1};
    
    for (i=0;i<2;i++)
    {
        for (j=0;j<2;j++)
        {
            planeVertex[i+2*j].mPosition.x=((float)i-0.5f)*2.0f;
            planeVertex[i+2*j].mPosition.y=-1;
            planeVertex[i+2*j].mPosition.z=((float)j-0.5f)*2.0f;
            planeVertex[i+2*j].mNormal.y=1;
            planeVertex[i+2*j].mTexture.x=i;
            planeVertex[i+2*j].mTexture.y=1-j;
        }
    }
    mMeshes.push_back(Mesh(planeVertex, planeIndices));
    
    for (i=0;i<2;i++)
    {
        for (j=0;j<2;j++)
        {
            planeVertex[i+2*j].mPosition.x=((float)i-0.5f)*2.0f;
            planeVertex[i+2*j].mPosition.z=1;
            planeVertex[i+2*j].mPosition.y=((float)j-0.5f)*2.0f;
            planeVertex[i+2*j].mNormal.z=-1;
            planeVertex[i+2*j].mTexture.x=i;
            planeVertex[i+2*j].mTexture.y=1-j;
        }
    }
    mMeshes.push_back(Mesh(planeVertex, planeIndices));
    
    for (i=0;i<2;i++)
    {
        for (j=0;j<2;j++)
        {
            planeVertex[i+2*j].mPosition.z=((float)i-0.5f)*2.0f;
            planeVertex[i+2*j].mPosition.x=-1;
            planeVertex[i+2*j].mPosition.y=((float)j-0.5f)*2.0f;
            planeVertex[i+2*j].mNormal.x=1;
            planeVertex[i+2*j].mTexture.x=i;
            planeVertex[i+2*j].mTexture.y=1-j;
        }
    }
    mMeshes.push_back(Mesh(planeVertex, planeIndices));
    
    planeIndices={0, 1, 2, 1, 3, 2};
    
    for (i=0;i<2;i++)
    {
        for (j=0;j<2;j++)
        {
            planeVertex[i+2*j].mPosition.x=((float)i-0.5f)*2.0f;
            planeVertex[i+2*j].mPosition.y=1;
            planeVertex[i+2*j].mPosition.z=((float)j-0.5f)*2.0f;
            planeVertex[i+2*j].mNormal.y=-1;
            planeVertex[i+2*j].mTexture.x=i;
            planeVertex[i+2*j].mTexture.y=1-j;
        }
    }
    mMeshes.push_back(Mesh(planeVertex, planeIndices));
    
    for (i=0;i<2;i++)
    {
        for (j=0;j<2;j++)
        {
            planeVertex[i+2*j].mPosition.x=((float)i-0.5f)*2.0f;
            planeVertex[i+2*j].mPosition.z=-1;
            planeVertex[i+2*j].mPosition.y=((float)j-0.5f)*2.0f;
            planeVertex[i+2*j].mNormal.z=1;
            planeVertex[i+2*j].mTexture.x=i;
            planeVertex[i+2*j].mTexture.y=1-j;
        }
    }
    mMeshes.push_back(Mesh(planeVertex, planeIndices));
    
    for (i=0;i<2;i++)
    {
        for (j=0;j<2;j++)
        {
            planeVertex[i+2*j].mPosition.z=((float)i-0.5f)*2.0f;
            planeVertex[i+2*j].mPosition.x=1;
            planeVertex[i+2*j].mPosition.y=((float)j-0.5f)*2.0f;
            planeVertex[i+2*j].mNormal.x=-1;
            planeVertex[i+2*j].mTexture.x=i;
            planeVertex[i+2*j].mTexture.y=1-j;
        }
    }
    mMeshes.push_back(Mesh(planeVertex, planeIndices));
}

void Skybox::draw(Shader * const pShader, const glm::mat4 & pVP)
{
    //Ignoruj pozycję kamery
    glm::mat4 fixedVP=pVP;
    fixedVP[3]={0,0,0,1};
    //
	glm::mat4 MVPMatrix = fixedVP*mTransform->getChildCombinedTransform();
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(mTransform->getChildCombinedTransform())));

	pShader->setMat4("MVPMatrix", &MVPMatrix);
	pShader->setMat4("modelMatrix", &mTransform->getChildCombinedTransform());
	pShader->setMat3("normalMatrix", &normalMatrix);

	mTexture->selectActiveTexture(0);

	for (Mesh& mesh : mMeshes)
		mesh.drawContent(pShader);
}
