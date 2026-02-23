#include <iostream>
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr   

#include "Engine/graphics/ShaderProgram.h"
#include "Engine/graphics/Texture.h"

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
    void ShaderProgram::Bind()
    {
        glUseProgram(_mShaderProgramID);
        // this is used to tell openGL to use this shader program for rendering
        // set the counter to zero
        _mCurrentTextureunit = 0;
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
    
    void ShaderProgram::SetUniform3f(const std::string& name, const glm::vec3& value)
    {
        GLint location = GetUniformLocation(name);
        glUniform3f(location, value.x, value.y, value.z);
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


    void ShaderProgram::SetTexture(const std::string& name, Texture* texture)
    {
        auto location = GetUniformLocation(name);

        glActiveTexture(GL_TEXTURE0 + _mCurrentTextureunit);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(location, _mCurrentTextureunit);
        ++_mCurrentTextureunit;


    }
    
    void ShaderProgram::SetUniform_Lights(const std::string& name,const glm::vec3& value)
    {
        auto location = GetUniformLocation(name);
        glUniform3fv(location, 1, glm::value_ptr(value));
        // here we are sending one value
    }



} // namespace GAMEDEV_ENGINE