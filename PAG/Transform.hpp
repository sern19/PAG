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

class Shader;

class Transform
{
private:
    Transform* mParentTransform;
    std::vector<std::pair<Transform,bool>> mChildrensTransform; // <Transform, flaga update>
    std::vector<glm::mat4> mCachedMatrix; //Liczba dzieci na końcu drzewa
    glm::vec3 mPosition = glm::vec3(0.0f);
    glm::vec3 mRotationAxis = glm::vec3(1.0f);
    glm::vec3 mScale=glm::vec3(1.0f);
    float mRotationAngle=0.0f;
	
    bool mNeedsUpdateCache=true;
    
    
    void updateChildrenPointers(Transform* pParent);
    void setParentsCacheUpdate();
    void setNeedsUpdateCache(Transform* pThisTransform);
    
    void recalculateCacheVectors();
    
    glm::mat4 combineTransformWithChildren(int pChildNumber, int pChildCombinedTransformNumber=0);
public:
    Transform();
    Transform(Transform* pParent);
    Transform(const Transform& pSourceTransform);
    ~Transform();
    
    void pushChildren();
    void popChildren();
    void removeChildren(int pChildNumber);
    Transform* getChildren(int pChildNumber);
    
    void setPosition(glm::vec3 pPosition);
    void setRotation(glm::vec3 pRotationAxis, float pRotationAngle);
    void setScale(glm::vec3 pScale);
    
    
    void updateCache();
    
    //W tym kontekście dzieci oznaczają koniec drzewa
    int getAllChildrensCount();
    glm::mat4 getChildCombinedTransformRotatedTowardsCamera(glm::vec3 pCameraPosition, int pChildNumber);
    glm::mat4 getChildCombinedTransform(int pChildNumber);
};

#endif /* Transform_hpp */
