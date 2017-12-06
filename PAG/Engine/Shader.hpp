// Shader.hpp
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

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
private:
    GLuint mProgramHandle;
    
    std::string loadFile(const std::string& pFileName);
    void compileShaderFromFile(const GLint& pShaderType, const std::string& pFileName);
public:
    Shader();
    ~Shader();
    void linkAndReloadProgram();
    void unlinkAndDeleteProgram();
    void useProgram();
    //Uniformy
    void setBool(const std::string& pUniformName, const bool* const pValue, const int& pArraySize=1);
    void setInt(const std::string& pUniformName, const int* const pValue, const int& pArraySize=1);
    void setFloat(const std::string& pUniformName, const float* const pValue, const int& pArraySize=1);
    void setVec3(const std::string& pUniformName, const glm::vec3* const pValue, const int& pArraySize=1);
    void setVec4(const std::string& pUniformName, const glm::vec4* const pValue, const int& pArraySize=1);
    void setMat3(const std::string& pUniformName, const glm::mat3* const pValue, const int& pArraySize=1);
    void setMat4(const std::string& pUniformName, const glm::mat4* const pValue, const int& pArraySize=1);
    void setBool(const std::string& pUniformName, const bool& pValue);
    void setInt(const std::string& pUniformName, const int& pValue);
    void setFloat(const std::string& pUniformName, const float& pValue);
    void setVec3(const std::string& pUniformName, const glm::vec3& pValue);
};

#endif /* Shader_hpp */
