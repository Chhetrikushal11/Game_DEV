#pragma once
#include <iostream>
#include <memory>
#include <string>
/*
    This will work as centerlize interface for graphics implementation
    such as DirectX, OpenGL, Vulkan, Metal etc.
 */
namespace GAMEDEV_ENGINE
{   
    // need to forward declare class ShaderProgram
    class ShaderProgram;
    class GraphicsAPI
    {
        // we will create single instance of the graphics api just as the input manager
    private:
        GraphicsAPI() = default;
        GraphicsAPI(const GraphicsAPI&) = delete; // we dont want copy constructor
        GraphicsAPI& operator=(const GraphicsAPI&) = delete; // we dont want copy assignment operator
        GraphicsAPI(GraphicsAPI&&) = delete; // we dont want move constructor
        GraphicsAPI& operator=(GraphicsAPI&&) = delete; // we dont want move assignment operator
        ~GraphicsAPI() = default;
    public:
    // add the method to set the graphics API
    std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource); 

    // we need to bind the graphics API with the engine so that engine can use it
    void BindShaderProgram(ShaderProgram* shaderProgram);

    private:
        friend class Engine; // only Engine class can create and destroy GraphicsAPI
    };
}