#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>        // 1. GLEW FIRST!
#include <GLFW/glfw3.h>     // 2. GLFW SECOND!
#include <memory>
#include <string>

#include "Engine/graphics/VertexLayout.h"



namespace GAMEDEV_ENGINE
{
 // here we will creatte a simple Mesh class to hold the vertex data and index data
// we will create VBO, VAO and EBO for the mesh
    class Mesh
    {
        public:
        // constructor we need two mesh with indices and not with indices
            Mesh(const VertexLayout& layout, const std::vector<float>& vertices, const std::vector<uint32_t>& indices);
            Mesh(const VertexLayout& layout, const std::vector<float>& vertices);
        // to avoid accidental copying we need to mark default, copy and copy assignment constructor as delete
            Mesh(const Mesh&) = delete;
            Mesh& operator=(const Mesh&) = delete;
        // bind 
            void Bind();
            void Unbind();
            void Draw();
        
         // need to create a load file to read gltf file

            static std::shared_ptr<Mesh> CreateBox(const glm::vec3& extents = glm::vec3(1.0f));
           /* static std::shared_ptr<Mesh> Load(const std::string& path);*/

            // to create a sphere
            static std::shared_ptr<Mesh> CreateSphere(float radius, int sectors, int stacks); // sectors and stacks define how smooth the bullet will be

        private:
            GLuint _mVAO = 0; // Vertex Array Object
            GLuint _mVBO = 0; // Vertex Buffer Object
            GLuint _mEBO = 0; // Element Buffer Object

            size_t _mIndexCount = 0; // number of indices
            size_t _mVertexCount = 0; // number of vertices

            VertexLayout _mVertexLayout;
    };  

}