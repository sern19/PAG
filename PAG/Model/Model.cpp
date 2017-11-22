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
#include "Textures.hpp"
#include "Texture.hpp"
#include "Node.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(const std::string& pModelPath, Shader *const pShader) { loadModel(pModelPath, pShader); }

Model::Model(const Model& pSourceModel): mModelDirectory(pSourceModel.mModelDirectory)
{
    mRootNode=new Node(*pSourceModel.mRootNode);
    mTextures=new Textures(*pSourceModel.mTextures);
}

void Model::loadModel(const std::string &pModelPath, Shader *const pShader)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(pModelPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        throw std::runtime_error("(Model::loadModel): Błąd wczytywania modelu");
    
    mModelDirectory=pModelPath;
    mModelDirectory.erase(mModelDirectory.find("source/"));
    
    mTextures=new Textures(scene, mModelDirectory.append("textures/"), pShader);
    mRootNode=new Node(scene->mRootNode, scene, mTextures);
}

void Model::draw(Shader *const pShader)
{
    if (mRootNode) mRootNode->drawContent(pShader, mTextures);
}

Node* const Model::getRootNode() { return mRootNode; }

Model::~Model()
{
    if (mRootNode) delete mRootNode;
    if (mTextures) delete mTextures;
}
