// SpotLight.cpp
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

#include "SpotLight.hpp"

#include "Shader.hpp"
#include "Scene.hpp"

#include <sstream>

SpotLight::SpotLight(const glm::vec3& pLightPos, const glm::vec3& pConeDirection, const glm::vec3& pLightColor, const float& pConeAngle, const float& pLightAttenuation, const float& pLightAmbientCoefficient):BaseLight(pLightPos, pLightColor, pLightAttenuation, pLightAmbientCoefficient), mConeAngle(pConeAngle), mConeDirection(pConeDirection)
{
    //Bo 0,0,0 jest używany przy punktowym
    if (mConeDirection==glm::vec3(0))
        mConeDirection=glm::vec3(0,-1,0);
}
SpotLight::~SpotLight() { }

void SpotLight::setLight(Shader* const pShader, Scene* const pScene, const unsigned int& pLightNumber) //Trochę rakowe, do przepisania będzie, ale jest 6 rano
{
    glm::vec4 transformedPosition=glm::vec4(glm::vec3(glm::vec4(mLightPos,1)), 1);
    
    std::stringstream output;
    output << "lights[" << pLightNumber << "].position";
    pShader->setVec4(output.str().c_str(), &transformedPosition);
    output.str("");
    
    output << "lights[" << pLightNumber << "].color";
    pShader->setVec3(output.str().c_str(), &mLightColor);
    output.str("");
    
    output << "lights[" << pLightNumber << "].attenuation";
    pShader->setFloat(output.str().c_str(), &mLightAttenuation);
    output.str("");
    
    output << "lights[" << pLightNumber << "].ambientCoefficient";
    pShader->setFloat(output.str().c_str(), &mLightAmbientCoefficient);
    output.str("");
    
    output << "lights[" << pLightNumber << "].coneAngle";
    pShader->setFloat(output.str().c_str(), &mConeAngle);
    output.str("");
    
    output << "lights[" << pLightNumber << "].coneDirection";
    pShader->setVec3(output.str().c_str(), &mConeDirection);
    output.str("");
}
