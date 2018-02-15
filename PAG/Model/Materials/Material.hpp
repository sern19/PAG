// Material.hpp
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

#ifndef Material_hpp
#define Material_hpp

#include <stdio.h>
#include <glm/glm.hpp>

struct Material
{
    glm::vec3 mDiffuseColor=glm::vec3(1,0,0);
    glm::vec3 mSpecularColor=glm::vec3(0,0,0);
    glm::vec3 mAmbientColor=glm::vec3(0,0,0);
    
    float mShininess=0.0f;
    float mShininessStrength=1.0f;
    
    std::vector<int> mDiffuseTextureID;
    std::vector<int> mSpecularTextureID;
    std::vector<int> mNomralTextureID;
    
    int mShadingMode=0x9; //Bez cieniowania
    Material() {};
};

#endif /* Material_hpp */
