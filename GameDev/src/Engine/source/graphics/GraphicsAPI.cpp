#include <GL/glew.h>        // 1. GLEW FIRST!
#include <GLFW/glfw3.h>     // 2. GLFW SECOND!

#include <iostream>
#include <memory>
#include <string>

#include "Engine/source/graphics/GraphicsAPI.h"
#include "Engine/source/graphics/ShaderProgram.h"
#include "Engine/source/render/material.h"
#include "Engine/source/render/mesh.h"
namespace GAMEDEV_ENGINE
{
    // this method will get the vertex and fragment shader source code as strings, compile shaders and link them to crate a shader program, Return shader program instance
    std::shared_ptr<ShaderProgram> GraphicsAPI::CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // this will create vertex shader object
        const char* vertexSourceCStr = vertexShaderSource.c_str(); // converting the string to character pointer so that opengl can understand
        glShaderSource(vertexShader, 1, &vertexSourceCStr, nullptr); // attach the source code to the shader object. Second parameter is number of strings, here we have only one string
        glCompileShader(vertexShader); // compile the vertex shader, everytime we create a shader object we need to compile the shader

        // to check the compilation status of vertex shader
        GLint success; // this is to store the compilation status code
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // get the compilation status of vertex shader, we use same vertexShader object
        if(!success) // if compilation failed
        {
            char infoLog[512]; // to store the error message
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog); // get the error message
            // in glGetShaderInfoLog, second parameter is the size of the buffer, third parameter is length of the log, we can pass nullptr if we dont want it
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n " << infoLog << std::endl;
            glDeleteShader(vertexShader); // delete the shader object to free resources
            // vertexSourceCStr is local variable no need to delete

            return nullptr; // return null pointer
            // we are returing nullptr because the shared_ptr will be null if shader program creation failed
        }

        // to create fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // create fragment shader object
        const char* fragmentSourceCStr = fragmentShaderSource.c_str(); // converting the string to character pointer so that opengl can understand
        glShaderSource(fragmentShader, 1, &fragmentSourceCStr, nullptr); // attach the source code to the shader object
        glCompileShader(fragmentShader); // compile the fragment shader object
        // to check the compilation status of fragment shader
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // get the compilation status of fragment shader
        if(!success) // if compilation failed
        {
            char infoLog[512]; // to store the error message
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog); // get the error message
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n " << infoLog << std::endl;
            glDeleteShader(vertexShader); // delete vertex shader object
            // if we delete fragment shader before vertexShader the vertexShader will be leaked. its just like store pointer in temp variable and delete original pointer first then the temp pointer will be dangling pointer
            glDeleteShader(fragmentShader); // delete fragment shader object
            // fragmentSourceCStr is local variable no need to delete
            return nullptr; // return null pointer
            // why to return nullptr?
            // we are returing nullptr because the shared_ptr will be null if shader program creation failed
            // but the shader_ptr delete itself when it goes out of scope
        }
        // now we need to link the shaders to create shader program
        GLuint shaderProgramID = glCreateProgram(); // create shader program object
        glAttachShader(shaderProgramID, vertexShader); // attach vertex shader to shader program
        glAttachShader(shaderProgramID, fragmentShader); // attach fragment shader to shader program
        //first we link the vertexShader and fragmentShader to shaderProgram
        // both ID will be stored in shaderProgram now, it will have two unique IDs stored as array internally
        glLinkProgram(shaderProgramID); // link the shader program
        // here we are linking the shaders to create shader program to openGL context, this means it is ready to use now in GPU
        // to check the linking status of shader program
        glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success); // get the linking status of shader program
        if(!success) // if linking failed
        {
            char infoLog[512]; // to store the error message
            glGetProgramInfoLog(shaderProgramID, 512, nullptr, infoLog); // get the error message
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n " << infoLog << std::endl;
            // lets say vertex shader compiled successfully but fragment shader compilation failed, in that case the error log will show that fragment shader compilation failed with ID assiocated with fragment shader
            // now we need to delete the shaders and shader program to free resources, these are resources allocated in GPU memory
            glDeleteShader(vertexShader); // delete vertex shader object
            glDeleteShader(fragmentShader); // delete fragment shader object
            glDeleteProgram(shaderProgramID); // delete shader program object
            return nullptr; // return null pointer
            // we are returing nullptr because the shared_ptr will be null if shader program creation failed

        }

        // shaders are linked to shader program, we can delete the shader objects now
        glDeleteShader(vertexShader); // delete vertex shader object
        glDeleteShader(fragmentShader); // delete fragment shader object    
        return std::make_shared<ShaderProgram>(shaderProgramID); // we use smart pointer to manage the shader program object memory
        // we are returning shared_ptr because multiple objects might need to use the same shader program
    }

    void GraphicsAPI::BindShaderProgram(ShaderProgram* shaderProgram)
    {
        shaderProgram->Bind();
    }

    void GraphicsAPI::BindMaterial(Material* material)
    {
        if(material)
        {
            material->Bind();
        }
        
    }

    GLuint GraphicsAPI::CreateVertexBuffer(const std::vector<float>& vertices)
    {
        GLuint VBO = 0;
        glGenBuffers(1, &VBO); // generate vertex buffer object
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the vertex buffer object
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW); // upload the vertex data to GPU
        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the vertex buffer object
        return VBO; // return the vertex
    }

     GLuint GraphicsAPI::CreateIndexBuffer(const std::vector<uint32_t>& indices)
     {
        GLuint EBO = 0;
        glGenBuffers(1, &EBO); // generate element buffer object
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the element buffer object
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW); // upload the index data to GPU
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind the element buffer object
        return EBO; // return the element buffer object
     }


    void GraphicsAPI::BindMesh(Mesh* mesh)
    {
        if(mesh)
        {
            mesh->Bind();
        }
        
    }


    void GraphicsAPI::DrawMesh(Mesh* mesh)
    {
        if(mesh)
        {
            mesh->Draw();
        }
        
    }

    void GraphicsAPI::SetClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }

    void GraphicsAPI::ClearBuffers()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

}