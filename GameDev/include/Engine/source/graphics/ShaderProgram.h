#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>

namespace GAMEDEV_ENGINE 
{

    class ShaderProgram
    {
        public:


            // we need to mark default, copy and copy assignment constructor as delete to avoid accidental copying
            ShaderProgram() = delete;
            ShaderProgram(const ShaderProgram&) = delete;
            ShaderProgram& operator=(const ShaderProgram&) = delete;
        // we need to add explicit constructor to create shader program from vertex and fragment shader source code
           explicit ShaderProgram(GLuint shaderProgramID);
           //explicit ensure the constructor is created only once to avoid the bugs of conversion

           // now we need to add destructor to delete the shader program when the object is destroyed
           ~ShaderProgram();

        // to use bind method so we can bind openGL shader program before use
            void Bind() const;
        // to use unbind method so we can unbind openGL shader program after use
            void Unbind() const;
        // need get uniform location method
        GLint GetUniformLocation(const std::string& name) const;
        // just to set different uniforms we will use set uniform methods
        void SetUniform1i(const std::string& name, int value);
        void SetUniform1f(const std::string& name, float value);
       
        private:
            GLuint _mShaderProgramID = 0; // for storing the shader program ID
            // need to create an array to store the uniform locations for local caching to avoid multiple glGetUniformLocation calls
            mutable std::unordered_map<std::string, GLint> _mUniformLocationCache;

    };  
}