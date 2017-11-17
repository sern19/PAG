//
//  Shader.hpp
//  Projekt bazowy OpenGL
//
//  Created by Krystian Owoc on 04.10.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
#include <string>
#include <glad/glad.h>

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
};

#endif /* Shader_hpp */
