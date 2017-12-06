// BaseLight.cpp
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

#include "BaseLight.hpp"

BaseLight::BaseLight(const glm::vec3& pLightPos, const glm::vec3& pLightColor, const float& pLightAttenuation, const float& pLightAmbientCoefficient):mLightPos(pLightPos), mLightColor(pLightColor), mLightAttenuation(pLightAttenuation), mLightAmbientCoefficient(pLightAmbientCoefficient) { }
BaseLight::~BaseLight() { }

const glm::vec3& BaseLight::getLightPos() { return mLightPos; }
const glm::vec3& BaseLight::getLightColor() { return mLightColor; }
const float& BaseLight::getLightAttenuation() { return mLightAttenuation; }
const float& BaseLight::getLightAmbientCoefficient() { return mLightAmbientCoefficient; }

void BaseLight::setLightPos(const glm::vec3& pLightPos) { mLightPos=pLightPos; }
void BaseLight::setLightColor(const glm::vec3& pLightColor) { mLightColor=pLightColor; }
void BaseLight::setLightAttenuation(const float& pLightAttenuation) { mLightAttenuation=pLightAttenuation; }
void BaseLight::setLightAmbientCoefficient(const float& pLightAmbientCoefficient) { mLightAmbientCoefficient=pLightAmbientCoefficient; }
