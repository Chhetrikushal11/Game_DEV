#pragma once

#include <GL/glew.h>

#include "Engine/source/graphics/VertexLayout.h"


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
            void Draw();
        
        private:
            GLuint _mVAO = 0; // Vertex Array Object
            GLuint _mVBO = 0; // Vertex Buffer Object
            GLuint _mEBO = 0; // Element Buffer Object

            size_t _mIndexCount = 0; // number of indices
            size_t __mVertexCount = 0; // number of vertices

            VertexLayout _mVertexLayout;
    };  

}