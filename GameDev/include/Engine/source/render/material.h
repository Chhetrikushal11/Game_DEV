#pragma once
#include <memory>
#include <unordered_map>
#include <string>

// need unordered map for texture  management in material later
namespace GAMEDEV_ENGINE 
{
    class ShaderProgram; // forward declaration
    
    class Material
    {
        public:
        // adding shader program using set method
        void SetShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram);
        // we are creating the shader program but using shader pointer to avoid copying.
        // we dont want to copy shader program object because its heavy object

        // to set the float parameters in the material
        void SetFloatParams(const std::string& name, float value);
        // to get the float parameters in the material

        // to set the float with two value
        void SetFloatParams2f(const std::string& name, float v0, float v1);


        // now to bind the material 
        void Bind() const;

        private :
        std::shared_ptr<ShaderProgram> _mShaderProgram;
        std::unordered_map<std::string, float> _mFloatParams;
        // need container which will hold 2 parameter for each key
        std::unordered_map<std::string, std::pair<float, float>> _mFloat2Params;

    };
}