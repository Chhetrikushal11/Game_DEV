#include "Engine/render/material.h"
#include "Engine/graphics/ShaderProgram.h"
#include "Engine/graphics/Texture.h"
#include "Engine/Engine.h"

// to parse the json file we need
#include <nlohmann/json.hpp>
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

    void Material::SetFloatParams3f(const std::string& name, const glm::vec3& value)
    {
        _mFloat3Params[name] = value;
    }

    void GAMEDEV_ENGINE::Material::SetTextureParams(const std::string& name, const std::shared_ptr<Texture>& texture)
    {
        _mTextureContainers[name] = texture;
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

          for (const auto& param : _mFloat3Params)
          {
              _mShaderProgram->SetUniform3f(param.first, param.second);
          }

          for (const auto& param : _mTextureContainers)
          {
              _mShaderProgram->SetTexture(param.first, param.second.get());
          }
            
        }

    }

    std::shared_ptr<Material> Material::Load(const std::string& path)
    {
        // need to read the content
        // to do so we need the engine

       auto contents = Engine::GetInstance().GetAssetFileSystem().LoadAssetFileText(path);

       if (contents.empty())
       {
           return nullptr;
       }
       // if the contents have some information we need to parse the json file
       nlohmann::json jsonRead = nlohmann::json::parse(contents);
       std::shared_ptr<Material> result;
       
       if (jsonRead.contains("shader"))
       {
           auto shaderObj = jsonRead["shader"];
           std::string vertexPath = shaderObj.value("vertex", "");
           std::string fragmentPath = shaderObj.value("fragment", "");

           auto& fs = Engine::GetInstance().GetAssetFileSystem();
           auto vertexSrc = fs.LoadAssetFileText(vertexPath);
           auto fragmentSrc = fs.LoadAssetFileText(fragmentPath);

           // now to run vertexShader and fragment shader we need GraphicsAPI
           auto& graphicsAPI = Engine::GetInstance().GetGraphicsAPI();
           auto shaderProgram = graphicsAPI.CreateShaderProgram(vertexSrc, fragmentSrc);

           // in case there is no shaderProgram conten return null
           if (!shaderProgram)
           {
               return nullptr;
           }

           result = std::make_shared<Material>();
           result->SetShaderProgram(shaderProgram);
       }

       // now for param section
       if (jsonRead.contains("params"))
       {
           auto paramsObj = jsonRead["params"];
           
           // for floats
           if (paramsObj.contains("float"))
           {
               for (auto& p : paramsObj["float"])
               {
                   std::string name = p.value("name", "");
                   float value = p.value("value", 0.0f);
                   result->SetFloatParams(name, value);
               }
           }
           // for 2 params
           if (paramsObj.contains("float2"))
           {
               for (auto& p : paramsObj["float2"])
               {
                   std::string name = p.value("name", "");
                   float v0= p.value("value0", 0.0f);
                   float v1 = p.value("value1", 0.0f);
                   result->SetFloatParams2f(name,v0, v1);
               }
           }

           // for 3 params
           if (paramsObj.contains("float3"))
           {
               for (auto& p : paramsObj["float3"])
               {
                   std::string name = p.value("name", "");
                   float v0 = p.value("value0", 0.0f);
                   float v1 = p.value("value1", 0.0f);
                   float v2 = p.value("value2", 0.0f);
                   result->SetFloatParams3f(name, glm::vec3(v0, v1, v2));
               }
           }

           // for texture
                      // for 2 params
           if (paramsObj.contains("texture"))
           {
               for (auto& p : paramsObj["texture"])
               {
                   std::string name = p.value("name", "");
                   std::string texturePath = p.value("path", "");
                   auto texture = Texture::Load(texturePath);
                   result->SetTextureParams(name, texture);
               }

           }
       }
       return result;


    }


}