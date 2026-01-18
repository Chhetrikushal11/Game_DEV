#include "Engine/source/render/material.h"
#include "Engine/source/graphics/ShaderProgram.h"

namespace GAMEDEV_ENGINE
{
    void Material::SetShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram)
    {
        _mShaderProgram = shaderProgram;
    }


    void Material::SetFloatParams(const std::string& name, float value)
    {
        _mFloatParams[name] = value;
    }

    void Material::SetFloatParams2f(const std::string& name, float v0, float v1)
    {
        _mFloat2Params[name] = {v0, v1};
    }

    void Material::Bind() const
    {
        if(_mShaderProgram)
        {
            // first we need to know if there is a shader object
            // then we bind the _mShaderProgram
            _mShaderProgram->Bind();
            // set all float parameters to shader uniforms
            for(const auto& param : _mFloatParams)
            {
                _mShaderProgram->SetUniform1f(param.first, param.second);
                // what does _shaderProgram->SetUniform1f do here?
                // it sets the uniform variable in the shader program with the name param.first to the value param.second   
                // leyman terms: it links the float parameters in the material to the shader program so that the shader can use them during rendering
                // let say we have a float parameter called "u_Shininess" in the material with value 32.0f
                // when we call _mShaderProgram->SetUniform1f("u_Shininess", 32.0f);
                // it will set the uniform variable "u_Shininess" in the shader program to 32.0f
            }
          for (const auto& param : _mFloat2Params)
          {
            _mShaderProgram->SetUniform2f(param.first, param.second.first, param.second.second);
          }
            
        }
    }
}