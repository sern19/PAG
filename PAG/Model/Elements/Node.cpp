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
#include "Scene.hpp"
#include "ModelNodePicker.hpp"
#include "Materials.hpp"

#include <iostream>
#include <algorithm>

Node::Node(const aiNode* const pNode, const aiScene* const pScene, Node* pParentNode, Materials* const pTextures): Node(pNode, pScene, pTextures) { mParentNode=pParentNode; }

Node::Node(const Mesh& pMesh)
{
    mElementTransform=new Transform();
    mMeshes.push_back(pMesh);
    updateCache();
    generateOBB();
}

Node::Node(const aiNode* const pNode, const aiScene* const pScene, Materials* const pTextures): mOriginalTransform(pNode->mTransformation)
{
    mElementTransform=new Transform();
    processNode(pNode, pScene, pTextures);
    updateChildrenPointers(this);
    updateCache();
    generateOBB();
}

Node::Node(const Node& pSourceNode): mParentNode(pSourceNode.mParentNode), mCachedTransform(pSourceNode.mCachedTransform), mChildNodes(pSourceNode.mChildNodes), mMeshes(pSourceNode.mMeshes), mOriginalTransform(pSourceNode.mOriginalTransform), mOBB(pSourceNode.mOBB)
{
    if (pSourceNode.mElementTransform) mElementTransform=new Transform(*pSourceNode.mElementTransform);
    updateChildrenPointers(this);
}

void Node::processNode(const aiNode* const pNode, const aiScene* const pScene, Materials* const pTextures)
{
    int i;
    //Przetwarzanie własnych meshy
    for (i=0;i<pNode->mNumMeshes;i++)
        mMeshes.push_back(processMesh(pScene->mMeshes[pNode->mMeshes[i]], pScene, pTextures));
    
    resetNodeTransform();
    
    //Przetwarzanie dzieci
    for (i=0;i<pNode->mNumChildren;i++)
        mChildNodes.push_back(Node(pNode->mChildren[i], pScene, this, pTextures));
}

void Node::generateOBB()
{
    int i;
    std::vector<std::pair<glm::vec4,glm::vec4>> obbs;
    
    if (mMeshes.size()==0) return;
    
    for (Mesh& mesh: mMeshes)
        obbs.push_back(mesh.getMinMaxVerticles());
    
    mOBB=obbs[0];
    
    //Minimum
    for (i=1;i<obbs.size();i++)
    {
        if (obbs[i].first.x<mOBB.first.x) mOBB.first.x=obbs[i].first.x;
        if (obbs[i].first.y<mOBB.first.y) mOBB.first.y=obbs[i].first.y;
        if (obbs[i].first.z<mOBB.first.z) mOBB.first.z=obbs[i].first.z;
    }
    //Maximum
    for (i=1;i<obbs.size();i++)
    {
        if (obbs[i].second.x>mOBB.second.x) mOBB.second.x=obbs[i].second.x;
        if (obbs[i].second.y>mOBB.second.y) mOBB.second.y=obbs[i].second.y;
        if (obbs[i].second.z>mOBB.second.z) mOBB.second.z=obbs[i].second.z;
    }
}

Mesh Node::processMesh(const aiMesh* const pMesh, const aiScene* const pScene, Materials* const pMaterials)
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
        if (pMesh->HasTangentsAndBitangents())
        {
            temporaryVertex.mTangent.x=pMesh->mTangents[i].x;
            temporaryVertex.mTangent.y=pMesh->mTangents[i].y;
            temporaryVertex.mTangent.z=pMesh->mTangents[i].z;

            temporaryVertex.mBitangent.x=pMesh->mBitangents[i].x;
            temporaryVertex.mBitangent.y=pMesh->mBitangents[i].y;
            temporaryVertex.mBitangent.z=pMesh->mBitangents[i].z;
        }
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
    if (pScene->mMaterials[pMesh->mMaterialIndex]!=NULL) output.setMaterial(pMesh->mMaterialIndex);
    else output.disableMaterialUsage();
    return output;
}

void Node::updateCache()
{
    int i;
    if (mParentNode==NULL) mCachedTransform=mElementTransform->getChildCombinedTransform();
    else mCachedTransform=mParentNode->mCachedTransform*mElementTransform->getChildCombinedTransform();
    for (Node& childNode: mChildNodes)
        childNode.updateCache();
}

void Node::updateChildrenPointers(Node* const pParent)
{
    int i;
    if (mParentNode!=NULL && pParent!=this) mParentNode=pParent;
    for (Node& childNode: mChildNodes)
        childNode.updateChildrenPointers(this);
}

void Node::drawContent(Shader *const pShader, const glm::mat4& pVP, Materials* const pTextures)
{
    int i;
    if (mElementTransform&&mElementTransform->getNeedsUpdateCache()) updateCache();
    
    glm::mat4 MVPMatrix=pVP*mCachedTransform;
    glm::mat3 normalMatrix=glm::transpose(glm::inverse(glm::mat3(mCachedTransform)));
    
    pShader->setMat4("MVPMatrix", &MVPMatrix);
    pShader->setMat4("modelMatrix", &mCachedTransform);
    pShader->setMat3("normalMatrix", &normalMatrix);
    
    for (Mesh& mesh: mMeshes)
        mesh.drawContent(pShader, pTextures);
    for (Node& childNode: mChildNodes)
        childNode.drawContent(pShader, pVP, pTextures);
}

void Node::setIsSelected(const bool& pIsSelected)
{
    int i;
    for (Mesh& mesh: mMeshes)
        mesh.setIsSelected(pIsSelected);
    for (Node& childNode: mChildNodes)
        childNode.setIsSelected(pIsSelected);
}

void Node::resetNodeTransform() { mElementTransform->importAiTransform(mOriginalTransform); }

void Node::bakeTransfrom(const glm::mat4& pBakeTransform, const glm::mat3& pNormalBakeTransform)
{
    for (Mesh& mesh: mMeshes)
        mesh.bakeTransfrom(pBakeTransform, pNormalBakeTransform);
    for (Node& childNode: mChildNodes)
        childNode.bakeTransfrom(pBakeTransform, pNormalBakeTransform);
    updateCache();
    generateOBB();
}

const unsigned int Node::getNodeLevel()
{
    if (mParentNode==NULL) return 0;
    else return mParentNode->getNodeLevel()+1;
}

const std::vector<std::pair<glm::vec4, glm::vec4>> Node::getOBBs()
{
    int i;
    std::vector<std::pair<glm::vec4, glm::vec4>> output;
    
    if (mElementTransform&&mElementTransform->getNeedsUpdateCache()) updateCache();
    
    if (mMeshes.size()>0)
    {
        std::pair<glm::vec4, glm::vec4> transformedOBB=mOBB;
        transformedOBB.first=mCachedTransform*transformedOBB.first;
        transformedOBB.second=mCachedTransform*transformedOBB.second;
        output.push_back(transformedOBB);
    }
    
    if (mChildNodes.size()>0)
    {
        std::vector<std::pair<glm::vec4, glm::vec4>> childrenOutput;
        for (Node& childNode: mChildNodes)
        {
            childrenOutput=childNode.getOBBs();
            output.insert(std::end(output), std::begin(childrenOutput), std::end(childrenOutput));
        }
    }
    return output;
}

const unsigned int Node::getChildrensCount() { return mChildNodes.size(); }
Transform* const Node::getNodeTransform() { return mElementTransform; }
Node * const Node::getParentNode() { return mParentNode; }
Node* const Node::getChildren(const unsigned int& pChildNumber)
{
    if (pChildNumber>mChildNodes.size()) throw std::runtime_error("(Node::getChildNode): Żądany numer dziecka jest większy od ilości dzieci");
    return &mChildNodes[pChildNumber];
}

const std::vector<std::pair<Node*,float>> Node::testRayOBBIntersection(const glm::vec3& pRaySource, const glm::vec3& pRayDirection)
{
    std::vector<std::pair<Node*,float>> output;
    float distance;
    
    if (mElementTransform&&mElementTransform->getNeedsUpdateCache()) updateCache();
    
    if (mMeshes.size()>0 && ModelNodePicker::checkRayIntersectionOBB(pRaySource, pRayDirection, mOBB, mCachedTransform, distance))
    {
        for (Mesh& mesh: mMeshes)
            if (mesh.checkRayIntersections(pRaySource, pRayDirection, mCachedTransform, distance)) output.push_back(std::pair<Node*,float>(this,distance));
    }
    //Łączenie vectorów
    if (mChildNodes.size()>0)
    {
        std::vector<std::pair<Node*,float>> childrenOutput;
        for (Node& childNode: mChildNodes)
        {
            childrenOutput=childNode.testRayOBBIntersection(pRaySource, pRayDirection);
            output.insert(std::end(output), std::begin(childrenOutput), std::end(childrenOutput));
        }
    }
    return output;
}

Node::~Node()
{
    if (mElementTransform) delete mElementTransform;
}
