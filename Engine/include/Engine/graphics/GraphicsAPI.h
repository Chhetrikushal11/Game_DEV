#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>

/*
    This will work as centerlize interface for graphics implementation
    such as DirectX, OpenGL, Vulkan, Metal etc.
 */
namespace GAMEDEV_ENGINE
{   
    // need to forward declare class ShaderProgram
    class ShaderProgram;
    // we will forward declare Material class
    class Material;
    // forward declare
    class Mesh;

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

    // here we add bind material methond
    void BindMaterial(Material* material); // we will implement this later when we have material class

    // to use other graphics API methods later
    GLuint CreateVertexBuffer(const std::vector<float>& vertices);
    GLuint CreateIndexBuffer(const std::vector<uint32_t>& indices);

    // here we add bind mesh
    void BindMesh(Mesh* mesh);
    // here we add draw mesh
    void DrawMesh(Mesh* mesh);
    
    // add the setColor method
    void SetClearColor(float r, float g, float b, float a);
    void ClearBuffers();

    private:
        friend class Engine; // only Engine class can create and destroy GraphicsAPI
    };
}