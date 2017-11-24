// Transform.hpp
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

#ifndef Transform_hpp
#define Transform_hpp

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/matrix4x4.h>

class Shader;

class Transform
{
private:
    Transform* mParentTransform=NULL;
    std::vector<std::pair<Transform,bool>> mChildrensTransform; // <Transform, flaga update>
    std::vector<glm::mat4> mCachedMatrix; //Liczba dzieci na końcu drzewa
    glm::vec3 mPosition = glm::vec3(0.0f);
    glm::vec3 mRotationAxis = glm::vec3(1.0f,0,0);
    glm::vec3 mScale=glm::vec3(1.0f);
    float mRotationAngle=0.0f;
	
    bool mNeedsUpdateCache=true;
    
    
    void updateChildrenPointers(Transform* const pParent);
    void setParentsCacheUpdate();
    void setNeedsUpdateCache(const Transform* pThisTransform);
    
    void recalculateCacheVectors();
    
    glm::mat4 combineTransformWithChildren(const int& pChildNumber, const int& pChildCombinedTransformNumber=0);
    
    Transform(Transform* const pParent);
public:
    Transform();
    Transform(const Transform& pSourceTransform);
    
    void pushChildren();
    void popChildren();
    void removeChildren(const int& pChildNumber);
    Transform* const getChildren(const int& pChildNumber);
    Transform* const getParent();
    
    void setPosition(const glm::vec3& pPosition);
    void setRotation(const glm::vec3& pRotationAxis, const float& pRotationAngle);
	void setScale(const glm::vec3& pScale);
	const glm::vec3&  getPosition();
	const std::pair<glm::vec3,float> getRotation();
	const glm::vec3& getScale();
    
    const bool& getNeedsUpdateCache();
    
    void importAiTransform(aiMatrix4x4 pMatrix);
    
    void updateCache();
    
    void resetTransform();
    
    const unsigned int getTransformLevel();
    //W tym kontekście dzieci oznaczają koniec drzewa
    const int getAllChildrensCount();
	const int getChildrensCount();
    const glm::mat4 getChildCombinedTransformRotatedTowardsCamera(const glm::vec3& pCameraPosition, const int& pChildNumber);
    const glm::mat4& getChildCombinedTransform(const int& pChildNumber);
};

#endif /* Transform_hpp */
