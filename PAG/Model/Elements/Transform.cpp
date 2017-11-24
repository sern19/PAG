// Transform.cpp
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

#include "Transform.hpp"
#include "Shader.hpp"

#include <glm/gtx/matrix_decompose.hpp>
#include <assimp/scene.h>

Transform::Transform() { recalculateCacheVectors(); }
Transform::Transform(Transform* const pParent): Transform() { mParentTransform=pParent; }
Transform::Transform(const Transform& pSourceTransform): mParentTransform(pSourceTransform.mParentTransform), mChildrensTransform(pSourceTransform.mChildrensTransform), mCachedMatrix(pSourceTransform.mCachedMatrix), mPosition(pSourceTransform.mPosition), mRotationAxis(pSourceTransform.mRotationAxis), mRotationAngle(pSourceTransform.mRotationAngle),  mScale(pSourceTransform.mScale), mNeedsUpdateCache(true) { }

void Transform::updateChildrenPointers(Transform* const pParent)
{
    int i;
    if (mParentTransform!=NULL && pParent!=this) mParentTransform=pParent;
    for (i=0;i<mChildrensTransform.size();i++)
    mChildrensTransform[i].first.updateChildrenPointers(this);
}

void Transform::setParentsCacheUpdate()
{
    if (mParentTransform!=NULL) mParentTransform->setNeedsUpdateCache(this);
    mNeedsUpdateCache=true;
}

void Transform::setNeedsUpdateCache(const Transform* pThisTransform)
{
    int i;
    if (this!=pThisTransform)
    {
        for (i=0;i<mChildrensTransform.size();i++)
            if (&mChildrensTransform[i].first==pThisTransform) mChildrensTransform[i].second=true;
    }
    else
    {
        for (i=0;i<mChildrensTransform.size();i++)
            mChildrensTransform[i].second=true;
    }
    setParentsCacheUpdate();
}

void Transform::recalculateCacheVectors()
{
    int desiredCacheSize;
    if (mChildrensTransform.size()!=0)
    {
        int i;
        desiredCacheSize=getAllChildrensCount();
        for (i=0;i<mChildrensTransform.size();i++)
            mChildrensTransform[i].second=true;
    }
    else desiredCacheSize=1;
    
    mCachedMatrix.clear();
    mCachedMatrix.resize(desiredCacheSize);

    if (mParentTransform!=NULL) mParentTransform->recalculateCacheVectors();
    setNeedsUpdateCache(this);
}

glm::mat4 Transform::combineTransformWithChildren(const int& pChildNumber, const int& pChildCombinedTransformNumber)
{
    glm::mat4 output;
    output = glm::translate(output, mPosition);
    output = glm::rotate(output, mRotationAngle, mRotationAxis);
    output = glm::scale(output, mScale);
    if (pChildNumber<mChildrensTransform.size() && pChildNumber>=0) return output*mChildrensTransform[pChildNumber].first.getChildCombinedTransform(pChildCombinedTransformNumber);
    return output;
}

void Transform::pushChildren()
{
    mChildrensTransform.push_back(std::pair<Transform,bool>(Transform(this),true));
    updateChildrenPointers(this);
    recalculateCacheVectors();
}

void Transform::popChildren()
{
    if (mChildrensTransform.size()>0)
        mChildrensTransform.pop_back();
    updateChildrenPointers(this);
    recalculateCacheVectors();
}

void Transform::removeChildren(const int& pChildNumber)
{
    if (mChildrensTransform.size()>0 && pChildNumber<mChildrensTransform.size())
        mChildrensTransform.erase(mChildrensTransform.begin()+pChildNumber);
    updateChildrenPointers(this);
    recalculateCacheVectors();
}

Transform* const Transform::getChildren(const int& pChildNumber)
{
    if (pChildNumber>=mChildrensTransform.size()) throw std::runtime_error("(Transform::getChildren): Żądany numer dziecka jest większy od ilości dzieci");
    return &mChildrensTransform[pChildNumber].first;
}

Transform* const Transform::getParent() { return mParentTransform; }

void Transform::setPosition(const glm::vec3& pPosition) { if (mPosition!=pPosition) { mPosition=pPosition; setNeedsUpdateCache(this); } }
void Transform::setRotation(const glm::vec3& pRotationAxis, const float& pRotationAngle) { if (mRotationAxis!=pRotationAxis || mRotationAngle!=pRotationAngle) { mRotationAxis=pRotationAxis; mRotationAngle=pRotationAngle; setNeedsUpdateCache(this); } }
const glm::vec3 & Transform::getScale()
{
	return mScale;
}
void Transform::setScale(const glm::vec3& pScale) { if (mScale!=pScale) { mScale=pScale; setNeedsUpdateCache(this); } }

const glm::vec3 & Transform::getPosition()
{
	return mPosition;
}

const std::pair<glm::vec3, float> Transform::getRotation()
{
	return std::pair<glm::vec3, float>(mRotationAxis, mRotationAngle);
}

const bool& Transform::getNeedsUpdateCache() { return mNeedsUpdateCache; }

void Transform::importAiTransform(aiMatrix4x4 pMatrix)
{
    aiVector3t<float> scale, position;
    aiQuaterniont<float> rotation;
    
    glm::quat gRotation;
    
    pMatrix.Decompose(scale, rotation, position);
    
    gRotation.w=rotation.w;
    gRotation.x=rotation.x;
    gRotation.y=rotation.y;
    gRotation.z=rotation.z;
    
    setPosition(glm::vec3(position.x, position.y, position.z));
    setScale(glm::vec3(scale.x, scale.y, scale.z));
    setRotation(glm::axis(gRotation), glm::angle(gRotation));
}

void Transform::resetTransform()
{
    setPosition(glm::vec3(0));
    setScale(mScale=glm::vec3(1));
    setRotation(glm::vec3(1.0f,0,0), 0);
}

void Transform::updateCache()
{
    int i,j, cacheIndex=0;
    //Najpierw uaktualniamy cache na końcach drzewa
    if (mChildrensTransform.size()!=0)
    {
        for (i=0;i<mChildrensTransform.size();i++)
        {
            if (mChildrensTransform[i].second)
            {
                mChildrensTransform[i].first.updateCache();
            }
        }
    }
    else
    {
        if (mNeedsUpdateCache) mCachedMatrix[0]=combineTransformWithChildren(-1);
        mNeedsUpdateCache=false;
        return;
    }
    
    //Teraz uaktualniamy nasze cache
    for (i=0;i<mChildrensTransform.size();i++)
    {
        if (mChildrensTransform[i].second)
        {
            for (j=0;j<mChildrensTransform[i].first.getAllChildrensCount();j++)
            {
                mCachedMatrix[cacheIndex]=combineTransformWithChildren(i,j);
                cacheIndex++;
            }
            mChildrensTransform[i].second=false;
        }
        else cacheIndex+=mChildrensTransform[i].first.getAllChildrensCount();
    }
    
    mNeedsUpdateCache=false;
}

const unsigned int Transform::getTransformLevel()
{
    if (mParentTransform==NULL) return 0;
    else return mParentTransform->getTransformLevel()+1;
}

const int Transform::getAllChildrensCount()
{
    if (mChildrensTransform.size()==0) return 1;
    else
    {
        int sum=0,i;
        for (i=0;i<mChildrensTransform.size();i++)
            sum+=mChildrensTransform[i].first.getAllChildrensCount();
            return sum;
    }
}

const int Transform::getChildrensCount()
{
	return (int)mChildrensTransform.size();
}

const glm::mat4 Transform::getChildCombinedTransformRotatedTowardsCamera(const glm::vec3& pCameraPosition, const int& pChildNumber)
{
    glm::mat4 output;
    
    try {
        output=getChildCombinedTransform(pChildNumber);
    } catch (std::runtime_error err) {
        throw err;
    }
    
    if (mParentTransform==NULL)
    {
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        
        glm::decompose(output, scale, rotation, translation, skew, perspective);
        
        glm::vec3 F=pCameraPosition-translation;
        F=glm::normalize(F);
        glm::vec3 R=glm::cross(F,glm::vec3(0,1,0));
        glm::vec3 U=glm::cross(F,R);
        
        output=glm::mat4(glm::vec4(F,0),glm::vec4(R,0),glm::vec4(U,0),glm::vec4(translation,1));
        output = glm::scale(output, scale);
        return output;
    }
    return output;
}

const glm::mat4& Transform::getChildCombinedTransform(const int& pChildNumber)
{
    if (pChildNumber>mCachedMatrix.size()) throw std::runtime_error("(Transform::getChildCombinedTransform): Żądany numer dziecka jest większy od ilości dzieci");
    if (mNeedsUpdateCache) updateCache();
    return mCachedMatrix[pChildNumber];
}
