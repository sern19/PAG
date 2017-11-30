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

#define GLM_FORCE_RADIANS

#include "Transform.hpp"
#include "Shader.hpp"

#include <glm/gtx/matrix_decompose.hpp>
#include <assimp/scene.h>

Transform::Transform() { }
Transform::Transform(Transform* const pParent): Transform() { mParentTransform=pParent; }
Transform::Transform(const Transform& pSourceTransform): mParentTransform(pSourceTransform.mParentTransform), mChildTransform(pSourceTransform.mChildTransform), mCachedMatrix(pSourceTransform.mCachedMatrix), mPosition(pSourceTransform.mPosition), mRotationEuler(pSourceTransform.mRotationEuler), mRotation(pSourceTransform.mRotation),  mScale(pSourceTransform.mScale), mNeedsUpdateCache(true) { }

void Transform::setParentsCacheUpdate()
{
    if (mParentTransform) {
        mParentTransform->setNeedsUpdateCache();
        mParentTransform->mChildTransform->second=true;
    }
}

void Transform::setNeedsUpdateCache()
{
    mNeedsUpdateCache=true;
    setParentsCacheUpdate();
}

glm::mat4 Transform::combineTransformWithChildren()
{
    glm::mat4 output;
    output = glm::translate(output, mPosition);
    output *= glm::mat4_cast(mRotation);
    output = glm::scale(output, mScale);
    if (mChildTransform) return output*mChildTransform->first.getChildCombinedTransform();
    return output;
}

void Transform::addChildren()
{
    if (!mChildTransform) mChildTransform=new std::pair<Transform,bool>(Transform(this), true);
}

void Transform::removeChildren()
{
    if (mChildTransform)
    {
        mChildTransform->first.removeChildren();
        delete mChildTransform;
        mChildTransform=NULL;
        setNeedsUpdateCache();
    }
}

Transform* const Transform::getChildren() { return &mChildTransform->first; }

Transform* const Transform::getParent() { return mParentTransform; }

void Transform::setPosition(const glm::vec3& pPosition) { if (mPosition!=pPosition) { mPosition=pPosition; setNeedsUpdateCache(); } }
void Transform::setRotation(const glm::vec3& pRotationEuler)
{
    if (mRotationEuler!=pRotationEuler)
    {
        mRotationEuler=pRotationEuler;
        mRotation=glm::quat(glm::radians(pRotationEuler));
        setNeedsUpdateCache();
    }
}
void Transform::setScale(const glm::vec3& pScale) { if (mScale!=pScale) { mScale=pScale; setNeedsUpdateCache(); } }

const glm::vec3& Transform::getScale() { return mScale; }
const glm::vec3& Transform::getPosition() { return mPosition; }
const glm::vec3& Transform::getRotation() { return mRotationEuler; }

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
    setRotation(glm::degrees(glm::eulerAngles(gRotation)));
}

void Transform::resetTransform()
{
    setPosition(glm::vec3(0));
    setScale(mScale=glm::vec3(1));
    setRotation(glm::vec3(0,0,0));
}

void Transform::updateCache()
{
    //Najpierw uaktualniamy cache na końcach drzewa
    if (mChildTransform)
    {
        if (mChildTransform->second)
        {
            mChildTransform->first.updateCache();
            mChildTransform->second=false;
        }
    }
    else
    {
        if (mNeedsUpdateCache) mCachedMatrix=combineTransformWithChildren();
        mNeedsUpdateCache=false;
        return;
    }
    
    //Teraz uaktualniamy nasze cache
    if (mChildTransform)
    {
        mCachedMatrix=combineTransformWithChildren();
    }
    mNeedsUpdateCache=false;
}

const unsigned int Transform::getTransformLevel()
{
    if (mParentTransform==NULL) return 0;
    else return mParentTransform->getTransformLevel()+1;
}

const glm::mat4 Transform::getChildCombinedTransformRotatedTowardsCamera(const glm::vec3& pCameraPosition)
{
    glm::mat4 output;
    
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

const glm::mat4& Transform::getChildCombinedTransform()
{
    if (mNeedsUpdateCache) updateCache();
    return mCachedMatrix;
}
