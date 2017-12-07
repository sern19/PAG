// BaseLight.hpp
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

#ifndef BaseLight_hpp
#define BaseLight_hpp

#include <stdio.h>
#include <glm/glm.hpp>

class Model;
class Shader;
class Scene;

class BaseLight
{
protected:
    glm::vec3 mLightPos;
    glm::vec3 mLightColor;
    float mLightAttenuation; //Tłumienie
    float mLightAmbientCoefficient; //"Jasność" ambienta
protected:
    BaseLight(const glm::vec3& pLightPos, const glm::vec3& pLightColor, const float& pLightAttenuation, const float& pLightAmbientCoefficient);
public:
    virtual ~BaseLight();
    
    const glm::vec3& getLightPos();
    const glm::vec3& getLightColor();
    const float& getLightAttenuation();
    const float& getLightAmbientCoefficient();
    
    void setLightPos(const glm::vec3& pLightPos);
    void setLightColor(const glm::vec3& pLightColor);
    void setLightAttenuation(const float& pLightAttenuation);
    void setLightAmbientCoefficient(const float& pLightAmbientCoefficient);
    
    virtual void drawModel(Model* pModel, Shader* const pShader, Scene* const pScene);
    
    virtual void setLight(Shader* const pShader, Scene* const pScene, const unsigned int& pLightNumber)=0;
};

#endif /* BaseLight_hpp */
