// DirectionalLight.cpp
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

#include "DirectionalLight.hpp"

#include "Shader.hpp"
#include "Scene.hpp"
#include "Model.hpp"

#include <sstream>

DirectionalLight::DirectionalLight(const glm::vec3& pLightDirection, const glm::vec3& pLightColor, const float& pLightAmbientCoefficient):BaseLight(pLightDirection, pLightColor, 1.0, pLightAmbientCoefficient) { }
DirectionalLight::~DirectionalLight() { }

void DirectionalLight::setLight(Shader* const pShader, Scene* const pScene) //Trochę rakowe, do przepisania będzie, ale jest 6 rano
{
    glm::vec4 transformedPosition=glm::vec4(glm::vec3(glm::vec4(mLightPos,1)), 0);
    
    pShader->setVec4("light.position", &transformedPosition);
    pShader->setVec3("light.color", &mLightColor);
    pShader->setFloat("light.attenuation", &mLightAttenuation);
    pShader->setFloat("light.ambientCoefficient", &mLightAmbientCoefficient);
    pShader->setVec3("light.coneDirection", glm::vec3(0));
}

void DirectionalLight::drawBoundings(Model* pModel, Shader* const pShader, const glm::mat4& pVP)
{
    pModel->draw(pShader, pVP);
}
