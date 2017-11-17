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
    std::string loadFile(std::string pFileName);
    void compileShaderFromFile(GLint pShaderType, std::string pFileName);
public:
    Shader();
    ~Shader();
    void linkAndReloadProgram();
    void unlinkAndDeleteProgram();
    void useProgram();
    //Uniformy
    void setBool(std::string pUniformName, bool pValue);
    void setInt(std::string pUniformName, int pValue);
    void setFloat(std::string pUniformName, float pValue);
    void setMat4(std::string pUniformName, glm::mat4* pValue);
};

#endif /* Shader_hpp */
