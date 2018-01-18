// PointLight.cpp
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

#include "PointLight.hpp"

#include "Shader.hpp"
#include "Scene.hpp"
#include "Node.hpp"
#include "Transform.hpp"
#include "Model.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <sstream>

PointLight::PointLight(const glm::vec3& pLightPos, const glm::vec3& pLightColor, const float& pLightAttenuation, const float& pLightAmbientCoefficient):BaseLight(pLightPos, pLightColor, pLightAttenuation, pLightAmbientCoefficient) { }
PointLight::~PointLight() { }

void PointLight::setLight(Shader* const pShader, Scene* const pScene) //Trochę rakowe, do przepisania będzie, ale jest 6 rano
{
    glm::vec4 transformedPosition=glm::vec4(glm::vec3(glm::vec4(mLightPos,1)), 1);
    
    pShader->setVec4("light.position", &transformedPosition);
    pShader->setVec3("light.color", &mLightColor);
    pShader->setFloat("light.attenuation", &mLightAttenuation);
    pShader->setFloat("light.attenuationLin", &mLightAttenuationLin);
    pShader->setFloat("light.attenuationExp", &mLightAttenuationExp);
    pShader->setFloat("light.ambientCoefficient", &mLightAmbientCoefficient);
    pShader->setVec3("light.coneDirection", glm::vec3(0));
}

void PointLight::drawBoundings(Model* pModel, Shader* const pShader, const glm::mat4& pVP)
{
    float radius=calcPointLightBSphere();
    pModel->getRootNode()->getNodeTransform()->setPosition(mLightPos);
    pModel->getRootNode()->getNodeTransform()->setScale(glm::vec3(radius*2.5f, radius*2.5f, radius*2.5f));
    pModel->draw(pShader, pVP);
}
