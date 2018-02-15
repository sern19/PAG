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

#include "Model.hpp"
#include "Transform.hpp"
#include "ModelNodePicker.hpp"
#include "Materials.hpp"
#include "Texture.hpp"
#include "Node.hpp"
#include "Shader.hpp"
#include "Config.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(const std::string& pModelPath, const bool& pIsReflective): mIsReflective(pIsReflective)
{
    loadModel(pModelPath);
}

Model::Model(const std::string& pModelPath, Transform* const pBakeTransform, const bool& pIsReflective): Model(pModelPath, pIsReflective)
{
    bakeTransfrom(pBakeTransform);
}

Model::Model(const Node& pRootNode, const Materials& pMaterials)
{
    mRootNode=new Node(pRootNode);
    mMaterials=new Materials(pMaterials);
}

Model::Model(const Model& pSourceModel): mModelDirectory(pSourceModel.mModelDirectory), mModelFilename(pSourceModel.mModelFilename), mAdditionalGLSettings(pSourceModel.mAdditionalGLSettings), mIsReflective(pSourceModel.mIsReflective)
{
    mRootNode=new Node(*pSourceModel.mRootNode);
    mMaterials=new Materials(*pSourceModel.mMaterials);
}
void Model::bakeTransfrom(Transform* const pBakeTransform)
{
    if (pBakeTransform==NULL||mRootNode==NULL) return;
    glm::mat4 bakeTransform=pBakeTransform->getChildCombinedTransform();
    glm::mat3 normalBakeTransform=glm::transpose(glm::inverse(glm::mat3(bakeTransform)));
    mRootNode->bakeTransfrom(bakeTransform, normalBakeTransform);
}

const std::pair<glm::vec4, glm::vec4>  Model::calculateModelOBB()
{
    int i;
    std::pair<glm::vec4, glm::vec4> output=std::pair<glm::vec4, glm::vec4>(glm::vec4(0), glm::vec4(0));
    
    if (mRootNode==NULL) return output;
    
    std::vector<std::pair<glm::vec4,glm::vec4>> obbs=mRootNode->getOBBs();
    
    if (obbs.size()==0) return output;
    
    output=obbs[0];
    
    //Minimum
    for (i=1;i<obbs.size();i++)
    {
        if (obbs[i].first.x<output.first.x) output.first.x=obbs[i].first.x;
        if (obbs[i].first.y<output.first.y) output.first.y=obbs[i].first.y;
        if (obbs[i].first.z<output.first.z) output.first.z=obbs[i].first.z;
    }
    //Maximum
    for (i=1;i<obbs.size();i++)
    {
        if (obbs[i].second.x>output.second.x) output.second.x=obbs[i].second.x;
        if (obbs[i].second.y>output.second.y) output.second.y=obbs[i].second.y;
        if (obbs[i].second.z>output.second.z) output.second.z=obbs[i].second.z;
    }
    return output;
}

void Model::loadModel(const std::string &pModelPath)
{
    Assimp::Importer importer;
    importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE,80.0f);
    importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_TANGENTS_AND_BITANGENTS | aiComponent_NORMALS);
    const aiScene *scene = importer.ReadFile(pModelPath, aiProcess_RemoveComponent | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_FixInfacingNormals | aiProcess_FindInvalidData | aiProcess_RemoveRedundantMaterials | aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices |  aiProcess_SplitLargeMeshes |  aiProcess_SortByPType | aiProcess_ImproveCacheLocality);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::string outputMessage="(Model::loadModel): Błąd wczytywania modelu\n\t(ASSIMP): ";
        outputMessage+=importer.GetErrorString();
        throw std::runtime_error(outputMessage);
    }
    
    
    mModelDirectory=pModelPath;
    mModelDirectory.erase(mModelDirectory.find(MODEL_SOURCE_FOLDER));
    mModelFilename=pModelPath.substr(pModelPath.rfind(MODEL_SOURCE_FOLDER)+std::string(MODEL_SOURCE_FOLDER).length());
    
    mMaterials=new Materials(scene, mModelDirectory.append(MODEL_TEXTURE_FOLDER));
    mRootNode=new Node(scene->mRootNode, scene, mMaterials);
}

void Model::draw(Shader* const pShader, const glm::mat4& pVP)
{
    int i;
    if (mRootNode)
    {
        pShader->setInt("isReflective", mIsReflective);
        for (i=0;i<mAdditionalGLSettings.size();i++)
            glEnable(mAdditionalGLSettings[i]);
        mRootNode->drawContent(pShader, pVP, mMaterials);
        for (i=0;i<mAdditionalGLSettings.size();i++)
            glDisable(mAdditionalGLSettings[i]);
    }
}

void Model::addGLSetting(const GLenum& pSetting)
{
    int i;
    for (i=0;i<mAdditionalGLSettings.size();i++)
        if (mAdditionalGLSettings[i]==pSetting) return;
    mAdditionalGLSettings.push_back(pSetting);
}
void Model::removeGLSetting(const GLenum& pSetting)
{
    int i;
    for (i=0;i<mAdditionalGLSettings.size();i++)
        if (mAdditionalGLSettings[i]==pSetting) mAdditionalGLSettings.erase(mAdditionalGLSettings.begin()+i);
}

Model* Model::findModelForNode(Node* const pNode)
{
    if (mRootNode) 
    {
        if (mRootNode->isNode(pNode))
            return this;
    }
    return NULL;
}

const bool& Model::getIsRefective() { return mIsReflective; }
void Model::setIsRefective(const bool& pIsReflective) { mIsReflective=pIsReflective; }

Node* const Model::getRootNode() { return mRootNode; }
Materials* const Model::getMaterials() { return mMaterials; }

Model::~Model()
{
    if (mRootNode) delete mRootNode;
    if (mMaterials) delete mMaterials;
}

const std::pair<Node*,float> Model::testRayOBBIntersection(const glm::vec3& pRaySource, const glm::vec3& pRayDirection)
{
    int i;
    float distance;
    if (mRootNode!=NULL && ModelNodePicker::checkRayIntersectionOBB(pRaySource, pRayDirection, calculateModelOBB(), glm::mat4(1.0f), distance)) // - wyłączone do momentu naprawienia
    {
        std::vector<std::pair<Node*,float>> intersectedNodes=mRootNode->testRayOBBIntersection(pRaySource, pRayDirection);
        
        if (intersectedNodes.size()==0) return std::pair<Node*,float>(NULL,FLT_MAX);
        
        std::pair<Node*,float> closestNode=intersectedNodes[0];
        for (i=1;i<intersectedNodes.size();i++)
            if (intersectedNodes[i].second<closestNode.second) closestNode=intersectedNodes[i];
        return closestNode;
    }
    return std::pair<Node*,float>(NULL,FLT_MAX);
}
