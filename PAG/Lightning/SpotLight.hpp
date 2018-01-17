// SpotLight.hpp
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

#ifndef SpotLight_hpp
#define SpotLight_hpp

#include <stdio.h>

#include "BaseLight.hpp"

class SpotLight:public BaseLight
{
private:
    glm::vec3 mConeDirection;
    float mConeAngle;
public:
    SpotLight(const glm::vec3& pLightPos, const glm::vec3& pConeDirection, const glm::vec3& pLightColor, const float& pConeAngle, const float& pLightAttenuation, const float& pLightAmbientCoefficient=0.14f);
    virtual ~SpotLight();
    
    const glm::vec3& getConeDirection();
    const float& getConeAngle();
    
    void setConeDirection(const glm::vec3& pConeDirection);
    void setConeAngle(const float& pConeAngle);
    
    virtual void setLight(Shader* const pShader, Scene* const pScene, const unsigned int& pLightNumber);
};

#endif /* SpotLight_hpp */
