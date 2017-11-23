// Node.hpp
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

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>

class Transform;
class Mesh;
class Shader;
class Textures;

class Node
{
private:
    Transform* mElementTransform=NULL; //Dla uproszczenia przyjmijmy, że może mieć tylko 1 gałąź dzieci, jest zawsze w parze z mCachedTransform
    Node* mParentNode=NULL;
    glm::mat4 mCachedTransform=glm::mat4(1.0f);
    std::vector<Node> mChildNodes;
    std::vector<Mesh> mMeshes;
    
    Node(const aiNode* const pNode, const aiScene* const pScene, Node* const pParentNode, Textures* const pTextures);
    void processNode(const aiNode* const pNode, const aiScene* const pScene, Textures* const pTextures);
    Mesh processMesh(const aiMesh* const pMesh, const aiScene* const pScene, Textures* const pTextures);
    void updateCache(); //Uaktualnia jedynie gdy istnieje cache
    void updateChildrenPointers(Node* const pParent);
    const glm::mat4& getTransform(); //Zwraca cache bądź niezmodyfikowane cache rodzica
public:
    Node(const aiNode* const pNode, const aiScene* const pScene, Textures* const pTextures);
    Node(const Node& pSourceNode);
    ~Node();
    
    void drawContent(Shader *const pShader, Textures* const pTextures);
    void setIsSelected(const bool& pIsSelected);
    
    const int& getChildrensCount();
    Transform* const getNodeTransform();
    Node* const getChildren(const unsigned int& pChildNumber);
    const std::vector<std::pair<Node*,float>> testRayOBBIntersection(const glm::vec3& pRaySource, const glm::vec3& pRayDirection); //Jeżeli będzie niedokładne, to możnaby przenieść do mesh'a
};

#endif /* Node_hpp */
