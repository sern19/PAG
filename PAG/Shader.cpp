//
//  Shader.cpp
//  Projekt bazowy OpenGL
//
//  Created by Krystian Owoc on 04.10.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include "Shader.hpp"
#include "Config.hpp"
#include <fstream>
#include <sstream>

Shader::Shader()
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

std::string Shader::loadFile(std::string pFileName)
{
    std::ifstream inputFile(pFileName);
    std::stringstream output;
    if (!inputFile.is_open()) throw std::runtime_error("Nie można otworzyć pliku");
    output << inputFile.rdbuf();
    return output.str();
}

void Shader::compileShaderFromFile(GLint pShaderType, std::string pFileName)
{
    std::string shaderCode;
    GLuint shaderObject=glCreateShader(pShaderType); //Utworzenie obiektu shadera
    
    if (!shaderObject) throw std::runtime_error("Błąd utworzenia shadera");
    
    try
    {
        shaderCode=loadFile(pFileName);
    } catch (std::runtime_error err)
    {
        throw err;
    }
    if (shaderCode.empty()) throw std::runtime_error("Plik shadera jest pusty");
    
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
        std::stringstream errorLog("Błąd kompilacji shadera\n");
        
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
        if (!mProgramHandle) throw std::runtime_error("Błąd utworzenia programu");
    } else mProgramHandle=glCreateProgram();
    try
    {
        compileShaderFromFile(GL_VERTEX_SHADER, VERTEX_SHADER_LOCATION);
        compileShaderFromFile(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_LOCATION);
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

void Shader::setBool(std::string pUniformName, bool pValue)
{
    glUniform1i(glGetUniformLocation(mProgramHandle, pUniformName.c_str()), pValue);
}
void Shader::setInt(std::string pUniformName, int pValue)
{
    glUniform1i(glGetUniformLocation(mProgramHandle, pUniformName.c_str()), pValue);
}
void Shader::setFloat(std::string pUniformName, float pValue)
{
    glUniform1f(glGetUniformLocation(mProgramHandle, pUniformName.c_str()), pValue);
}
