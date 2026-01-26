#include <iostream>
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr    
#include "Engine/source/graphics/ShaderProgram.h"

namespace GAMEDEV_ENGINE 
{
    ShaderProgram::ShaderProgram(GLuint shaderProgramID)
        : _mShaderProgramID(shaderProgramID)
    {
        // constructor to initialize the shader program ID
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(_mShaderProgramID);
        // destructor to delete the shader program
        // this happen inside OpenGL context
    }   
    void ShaderProgram::Bind() const
    {
        glUseProgram(_mShaderProgramID);
        // this is used to tell openGL to use this shader program for rendering
    }

    void ShaderProgram::Unbind() const
    {
        glUseProgram(0);
    }

    // to get the uniform location from the shader program

    GLint ShaderProgram::GetUniformLocation(const std::string& name) const
    {
        auto it = _mUniformLocationCache.find(name);
        if (it != _mUniformLocationCache.end())
        {
            return it->second; // return cached location, second is the location value.
        }

        // if location is not found 
        GLint location = glGetUniformLocation(_mShaderProgramID, name.c_str());
        // here we are storing the location in the cache with program id as key and location as value.
        // location is store as character array so we need to convert it to string using c_str() method.
        _mUniformLocationCache[name] = location;
        return location;
        if (location == -1)
        {
            std::cerr << "Warning: uniform '" << name << "' doesn't exist or is not used in shader!" << std::endl;
        }   

    }

    void ShaderProgram::SetUniform1i(const std::string& name, int value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    void ShaderProgram::SetUniform1f(const std::string& name, float value)
    {
        glUniform1f(GetUniformLocation(name), value);
    }   

    void ShaderProgram::SetUniform2f(const std::string& name, float v0, float v1)
    {
        glUniform2f(GetUniformLocation(name), v0, v1);
    } 
    
    void ShaderProgram::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
    {
        glUniformMatrix4fv(GetUniformLocation(name),1, GL_FALSE, glm::value_ptr(mat));
        // here first paramter is the location of the uniform variable
        // second parameter is the count of matrices to be sent
        // third parameter is whether to transpose the matrix or not
        // fourth parameter is the pointer to the first element of the matrix
        // glm::value_ptr(mat) returns the pointer to the first element of the matrix
       
    }

    




} // namespace GAMEDEV_ENGINE