// Model.hpp
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

#ifndef Model_hpp
#define Model_hpp

#include <stdio.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

class Transform;
class Materials;
class Node;
class Shader;
class Scene;

class Model
{
private:
    Node* mRootNode=NULL;
    Materials* mMaterials=NULL;
    std::vector<GLenum> mAdditionalGLSettings;
    std::string mModelDirectory;
    std::string mModelFilename;
    
    bool mIsReflective;
    
    void loadModel(const std::string& pModelPath);
    const std::pair<glm::vec4, glm::vec4> calculateModelOBB();
    void bakeTransfrom(Transform* const pBakeTransform);
public:
    Model(const Node& pRootNode, const Materials& pMaterials);
    Model(const std::string& pModelPath, const bool& pIsReflective=false);
    Model(const std::string& pModelPath, Transform* const pBakeTransform, const bool& pIsReflective=false);
    Model(const Model& pSourceModel);
    ~Model();
    void draw(Shader* const pShader, const glm::mat4& pVP);
    void addGLSetting(const GLenum& pSetting);
    void removeGLSetting(const GLenum& pSetting);
    
    Model* findModelForNode(Node* const pNode);
    
    Node* const getRootNode();
    Materials* const getMaterials();
    
    const bool& getIsRefective();
    void setIsRefective(const bool& pIsReflective);
    
    const std::pair<Node*,float> testRayOBBIntersection(const glm::vec3& pRaySource, const glm::vec3& pRayDirection); //To samo co w Node, OBB dla optymalizacji
};

#endif /* Model_hpp */
