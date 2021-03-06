// Materials.hpp
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

#ifndef Materials_hpp
#define Materials_hpp

#include <stdio.h>
#include <vector>
#include <assimp/scene.h>

#include "Material.hpp"

class Texture;
class Shader;

class Materials
{
private:
    std::vector<Texture> mDiffuseTextures;
    std::vector<Texture> mSpecularTextures;
    std::vector<Texture> mNormalTextures;
    
    std::vector<Material> mMaterials;
    
    std::string mTexturesPath;
    
    void loadTextures(const aiScene* const pScene);
    std::string getTextureName(const aiString& pTexturePath);
    const bool chcekIfIsLoaded(const std::string& pTexturePath, const std::string& pTextureType);
public:
    Materials(const Material& pMaterial);
    Materials(const Material& pMaterial, const Texture& pDiffuseTexture);
    Materials(const aiScene* const pScene, const std::string& pTexturesPath);
    const Material fillMaterialData(aiMaterial* const pMaterial);
    Material* getMaterial(const unsigned int& pMaterialID);
    
    static void prepareTextureUnits(Shader* pShader);
    
    void setActiveMaterial(const unsigned int& pMaterialID, Shader *const pShader);
    void setDefaultMaterial(Shader *const pShader);
};


#endif /* Materials_hpp */
