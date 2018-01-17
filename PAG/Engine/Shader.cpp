// Shader.cpp
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

#include "Shader.hpp"
#include "Config.hpp"
#include <fstream>
#include <sstream>

Shader::Shader(): Shader({ {VERTEX_SHADER_LOCATION, GL_VERTEX_SHADER}, {FRAGMENT_SHADER_LOCATION, GL_FRAGMENT_SHADER} })
{}

Shader::Shader(std::vector<std::pair<std::string, GLenum>> pShaderPrograms): mShaderPrograms(pShaderPrograms)
{
    try
    {
        linkAndReloadProgram();
    } catch (std::runtime_error err)
    {
        throw err;
    }
}

Shader::~Shader()
{
    if (mProgramHandle) glDeleteProgram(mProgramHandle);
}

std::string Shader::loadFile(const std::string& pFileName)
{
    std::ifstream inputFile(pFileName);
    std::stringstream output;
    if (!inputFile.is_open()) throw std::runtime_error("(Shader::loadFile): Nie można otworzyć pliku");
    output << inputFile.rdbuf();
    return output.str();
}

void Shader::compileShaderFromFile(const GLint& pShaderType, const std::string& pFileName)
{
    std::string shaderCode;
    GLuint shaderObject=glCreateShader(pShaderType); //Utworzenie obiektu shadera
    
    if (!shaderObject) throw std::runtime_error("(Shader::compileShaderFromFile): Błąd utworzenia shadera");
    
    try
    {
        shaderCode=loadFile(pFileName);
    } catch (std::runtime_error err)
    {
        throw err;
    }
    if (shaderCode.empty()) throw std::runtime_error("(Shader::compileShaderFromFile): Plik shadera jest pusty");
    
    //Przepisanie kodu i długości do zmiennych pomocniczych
    const char* shaderCodeChar=shaderCode.c_str();
    const GLint shaderCodeSize=(GLint)shaderCode.size();
    
    //Wczytanie kodu shadera - Obiekt shadera, liczba kodów, kod, długość kodu
    glShaderSource(shaderObject, 1, &shaderCodeChar, &shaderCodeSize);
    
    //Kompilacja kodu
    glCompileShader(shaderObject);
    
    //Sprawdzanie pomyślności kompilacji
    GLint compileStatus;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus)
    {
        std::stringstream errorLog("(Shader::compileShaderFromFile): Błąd kompilacji shadera\n");
        
        GLint logLength;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logLength); //Pobranie długości loga
        
        char* log=new char[logLength+1];
        
        glGetShaderInfoLog(shaderObject, logLength, NULL, log); //Zapizanie loga do zmiennej
        errorLog << log;
        
        delete[] log;
        
        throw std::runtime_error(errorLog.str());
    }
    
    //Podpięcie shadera do programu i zwolnienie zasobów
    glAttachShader(mProgramHandle, shaderObject);
    glDeleteShader(shaderObject);
}

void Shader::linkAndReloadProgram()
{
    if (mProgramHandle)
    {
        unlinkAndDeleteProgram();
        mProgramHandle=glCreateProgram();
        if (!mProgramHandle) throw std::runtime_error("(Shader::linkAndReloadProgram): Błąd utworzenia programu");
    } else mProgramHandle=glCreateProgram();
    try
    {
        for (auto shaderProgram: mShaderPrograms)
            compileShaderFromFile(shaderProgram.second, shaderProgram.first);
    } catch (std::runtime_error err)
    {
        throw err;
    }
    glLinkProgram(mProgramHandle);
}
void Shader::unlinkAndDeleteProgram()
{
    glDeleteProgram(mProgramHandle);
}
void Shader::useProgram()
{
    glUseProgram(mProgramHandle);
}

void Shader::setBool(const std::string& pUniformName, const bool* const pValue, const int& pArraySize)
{
    glUniform1iv(glGetUniformLocation(mProgramHandle, pUniformName.c_str()), pArraySize, (int*)pValue);
}
void Shader::setInt(const std::string& pUniformName, const int* const pValue, const int& pArraySize)
{
    glUniform1iv(glGetUniformLocation(mProgramHandle, pUniformName.c_str()), pArraySize, (int*)pValue);
}
void Shader::setFloat(const std::string& pUniformName, const float* const pValue, const int& pArraySize)
{
    glUniform1fv(glGetUniformLocation(mProgramHandle, pUniformName.c_str()), pArraySize, (float*)pValue);
}
void Shader::setVec3(const std::string& pUniformName, const glm::vec3* const pValue, const int& pArraySize)
{
    glUniform3fv(glGetUniformLocation(mProgramHandle, pUniformName.c_str()), pArraySize, (float*)pValue);
}
void Shader::setVec4(const std::string& pUniformName, const glm::vec4* const pValue, const int& pArraySize)
{
    glUniform4fv(glGetUniformLocation(mProgramHandle, pUniformName.c_str()), pArraySize, (float*)pValue);
}
void Shader::setMat3(const std::string& pUniformName, const glm::mat3* const pValue, const int& pArraySize)
{
    glUniformMatrix3fv(glGetUniformLocation(mProgramHandle, pUniformName.c_str()), pArraySize, GL_FALSE, (float*)pValue); //Lokacja uniformu, ilość macierzy, czy transportować, wskażnik początku macierzy
}
void Shader::setMat4(const std::string& pUniformName, const glm::mat4* const pValue, const int& pArraySize)
{
    glUniformMatrix4fv(glGetUniformLocation(mProgramHandle, pUniformName.c_str()), pArraySize, GL_FALSE, (float*)pValue); //Lokacja uniformu, ilość macierzy, czy transportować, wskażnik początku macierzy
}
void Shader::setBool(const std::string& pUniformName, const bool& pValue)
{
    glUniform1i(glGetUniformLocation(mProgramHandle, pUniformName.c_str()),pValue);
}
void Shader::setInt(const std::string& pUniformName, const int& pValue)
{
    glUniform1i(glGetUniformLocation(mProgramHandle, pUniformName.c_str()),pValue);
}
void Shader::setFloat(const std::string& pUniformName, const float& pValue)
{
    glUniform1f(glGetUniformLocation(mProgramHandle, pUniformName.c_str()),pValue);
}
void Shader::setVec3(const std::string& pUniformName, const glm::vec3& pValue)
{
    glUniform3f(glGetUniformLocation(mProgramHandle, pUniformName.c_str()),pValue[0],pValue[1],pValue[2]);
}
