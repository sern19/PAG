// ModelCreator.cpp
//
// Copyright (c) 2018 Krystian Owoc
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

#include "ModelCreator.hpp"

#include "Skybox.hpp"

#include <vector>

#include "Materials.hpp"
#include "Texture.hpp"
#include "CubeTexture.hpp"
#include "Mesh.hpp"
#include "Node.hpp"
#include "Model.hpp"

#include "Config.hpp"

Material ModelCreator::createDefaultMaterial()
{
    Material defaultMaterial;
    defaultMaterial.mDiffuseColor=glm::vec3(1);
    defaultMaterial.mSpecularColor=glm::vec3(1);
    defaultMaterial.mAmbientColor=glm::vec3(1);
    defaultMaterial.mShadingMode=aiShadingMode_Blinn;
    defaultMaterial.mShininess=255;
    return defaultMaterial;
}

Model ModelCreator::createCube()
{
    int i,j;
    Material defaultMaterial=createDefaultMaterial();
    Materials defaultMaterials(defaultMaterial);
    
    std::vector<Vertex> planeVertex(4);
    std::vector<unsigned int> planeIndices={2, 1, 0, 2, 3, 1};
    std::vector<Mesh> meshes;
    
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
    meshes.push_back(Mesh(planeVertex, planeIndices));
    
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
    meshes.push_back(Mesh(planeVertex, planeIndices));
    
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
    meshes.push_back(Mesh(planeVertex, planeIndices));
    
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
    meshes.push_back(Mesh(planeVertex, planeIndices));
    
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
    meshes.push_back(Mesh(planeVertex, planeIndices));
    
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
    meshes.push_back(Mesh(planeVertex, planeIndices));
    
    return Model(Node(meshes), defaultMaterials);
}

Model ModelCreator::createPlane()
{
    int i,j;
    Material defaultMaterial=createDefaultMaterial();
    Materials defaultMaterials(defaultMaterial);
    
    std::vector<Vertex> planeVertex(4);
    std::vector<unsigned int> planeIndices={0, 1, 2, 1, 3, 2};
    
    for (i=0;i<2;i++)
    {
        for (j=0;j<2;j++)
        {
            planeVertex[i+2*j].mPosition.x=((float)i-0.5f)*2.0f;
            planeVertex[i+2*j].mPosition.y=((float)j-0.5f)*2.0f;
            planeVertex[i+2*j].mNormal.z=1;
            planeVertex[i+2*j].mTexture.x=i;
            planeVertex[i+2*j].mTexture.y=1-j;
        }
    }
    return Model(Node(Mesh(planeVertex, planeIndices)), defaultMaterials);
}
