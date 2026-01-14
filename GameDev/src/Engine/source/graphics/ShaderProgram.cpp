#include <iostream>
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

} // namespace GAMEDEV_ENGINE