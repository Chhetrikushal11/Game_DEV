#pragma once
#include <GL/glew.h>
#include <vector>
#include <stdint.h>

namespace GAMEDEV_ENGINE 
{   
    // first we need to define vertex elment
    struct VertexElement
    {
        GLuint index; // attribute location in the shader
        GLuint size;  // number of components (e.g., 3 for vec3)
        GLuint type; // data type (e.g., GL_FLOAT)
        uint32_t offset; // offset in bytes from the start of the vertex
    };

    struct VertexLayout
    {
        // container that hold the list of vertex elements
        std::vector<VertexElement> elements;
        uint32_t stride = 0; // total size of the vertex in bytes
    };
}