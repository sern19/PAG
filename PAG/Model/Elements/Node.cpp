// Node.cpp
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

#include "Node.hpp"

#include "Mesh.hpp"
#include "Transform.hpp"
#include "Shader.hpp"
#include "Textures.hpp"

#include <iostream>
#include <algorithm>

Node::Node(const aiNode* const pNode, const aiScene* const pScene, Node* pParentNode, Textures* const pTextures): Node(pNode, pScene, pTextures) { mParentNode=pParentNode; }

Node::Node(const aiNode* const pNode, const aiScene* const pScene, Textures* const pTextures)
{
    processNode(pNode, pScene, pTextures);
    mElementTransform=new Transform();
    updateChildrenPointers(this); //Może nie będzie potrzebne, ale przy vectorach lepiej dać
    updateCache();
}

Node::Node(const Node& pSourceNode): mParentNode(pSourceNode.mParentNode), mCachedTransform(pSourceNode.mCachedTransform), mChildNodes(pSourceNode.mChildNodes), mMeshes(pSourceNode.mMeshes)
{
    if (pSourceNode.mElementTransform) mElementTransform=new Transform(*pSourceNode.mElementTransform);
    updateChildrenPointers(this);
}

void Node::processNode(const aiNode* const pNode, const aiScene* const pScene, Textures* const pTextures)
{
    int i;
    //Przetwarzanie własnych meshy
    for (i=0;i<pNode->mNumMeshes;i++)
        mMeshes.push_back(processMesh(pScene->mMeshes[pNode->mMeshes[i]], pScene, pTextures));
    
    //Przetwarzanie dzieci
    for (i=0;i<pNode->mNumChildren;i++)
        mChildNodes.push_back(Node(pNode->mChildren[i], pScene, this, pTextures));
}

Mesh Node::processMesh(const aiMesh* const pMesh, const aiScene* const pScene, Textures* const pTextures)
{
    std::vector<Vertex> verticles;
    std::vector<unsigned int> indices;
    unsigned int i,j;
    
    for (i=0;i<pMesh->mNumVertices;i++)
    {
        Vertex temporaryVertex;
        //Pozycja
        temporaryVertex.mPosition.x=pMesh->mVertices[i].x;
        temporaryVertex.mPosition.y=pMesh->mVertices[i].y;
        temporaryVertex.mPosition.z=pMesh->mVertices[i].z;
        //Normalne
        if (pMesh->mNormals)
        {
            temporaryVertex.mNormal.x=pMesh->mNormals[i].x;
            temporaryVertex.mNormal.y=pMesh->mNormals[i].y;
            temporaryVertex.mNormal.z=pMesh->mNormals[i].z;
        }
        //Tekstura
        if (pMesh->mTextureCoords[0])
        {
            temporaryVertex.mTexture.x=pMesh->mTextureCoords[0][i].x;
            temporaryVertex.mTexture.y=pMesh->mTextureCoords[0][i].y;
        }
        verticles.push_back(temporaryVertex);
    }
    
    for (i=0;i<pMesh->mNumFaces;i++)
    {
        for (j=0;j<pMesh->mFaces[i].mNumIndices;j++)
            indices.push_back(pMesh->mFaces[i].mIndices[j]);
    }
    
    Mesh output(verticles,indices);
    if (pScene->mMaterials[pMesh->mMaterialIndex]!=NULL) output.setMaterial(pTextures->findTexturesForMaterial(pScene->mMaterials[pMesh->mMaterialIndex]));
    else output.setMaterial(Material());
    return output;
}

void Node::updateCache()
{
    int i;
    if (mParentNode==NULL) mCachedTransform=mElementTransform->getChildCombinedTransform(0);
    else mCachedTransform=mParentNode->mCachedTransform*mElementTransform->getChildCombinedTransform(0);
    for (i=0;i<mChildNodes.size();i++)
        mChildNodes[i].updateCache();
}

void Node::updateChildrenPointers(Node* const pParent)
{
    int i;
    if (mParentNode!=NULL && pParent!=this) mParentNode=pParent;
    for (i=0;i<mChildNodes.size();i++)
        mChildNodes[i].updateChildrenPointers(this);
}

void Node::drawContent(Shader *const pShader, Textures* const pTextures)
{
    int i;
    if (mElementTransform&&mElementTransform->getNeedsUpdateCache()) updateCache();
    pShader->setMat4("model", &mCachedTransform);
    for (i=0;i<mMeshes.size();i++)
        mMeshes[i].drawContent(pShader, pTextures);
    for (i=0;i<mChildNodes.size();i++)
        mChildNodes[i].drawContent(pShader, pTextures);
}

void Node::setIsSelected(const bool& pIsSelected)
{
    int i;
    for (i=0;i<mMeshes.size();i++)
        mMeshes[i].setIsSelected(pIsSelected);
    for (i=0;i<mChildNodes.size();i++)
        mChildNodes[i].setIsSelected(pIsSelected);
}

const int& Node::getChildrensCount() { return mChildNodes.size(); }
Transform* const Node::getNodeTransform() { return mElementTransform; }
Node* const Node::getChildren(const unsigned int& pChildNumber)
{
    if (pChildNumber>mChildNodes.size()) throw std::runtime_error("(Node::getChildNode): Żądany numer dziecka jest większy od ilości dzieci");
    return &mChildNodes[pChildNumber];
}

const std::vector<std::pair<Node*,float>> Node::testRayOBBIntersection(const glm::vec3& pRaySource, const glm::vec3& pRayDirection)
{
    std::vector<std::pair<Node*,float>> output;
    float distance;
    int i;
    
    if (mElementTransform->getNeedsUpdateCache()) updateCache();
    
    for (i=0;i<mMeshes.size();i++)
        if (mMeshes[i].checkRayIntersections(pRaySource, pRayDirection, mCachedTransform, distance)) output.push_back(std::pair<Node*,float>(this,distance));
    
    //Łączenie vectorów
    if (mChildNodes.size()>0)
    {
        std::vector<std::pair<Node*,float>> childrenOutput;
        for (i=0;i<mChildNodes.size();i++)
        {
            childrenOutput=mChildNodes[i].testRayOBBIntersection(pRaySource, pRayDirection);
            output.insert(std::end(output), std::begin(childrenOutput), std::end(childrenOutput));
        }
    }
    return output;
}

Node::~Node()
{
    if (mElementTransform) delete mElementTransform;
}
