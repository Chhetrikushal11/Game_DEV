#include "Engine/source/render/mesh.h"
#include "Engine/source/Engine.h"

namespace GAMEDEV_ENGINE
{
    Mesh::Mesh(const VertexLayout& layout, const std::vector<float>& vertices, const std::vector<uint32_t>& indices)
    : _mVertexLayout{layout},
        _mIndexCount{indices.size()},
        __mVertexCount{vertices.size() / (layout.stride / sizeof(float))}// calculate number of vertices
    {
        auto& graphicsAPI = Engine::GetInstance().GetGraphicsAPI();
        _mVBO =  graphicsAPI.CreateVertexBuffer(vertices);
        _mEBO=  graphicsAPI.CreateIndexBuffer(indices);

        // generate and bind VAO
        glGenVertexArrays(1, &_mVAO);
        glBindVertexArray(_mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, _mVBO);

        // why we dont glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mEBO); here?
        // because when a VAO is bound, the GL_ELEMENT_ARRAY_BUFFER binding is stored in the VAO
         // why we need to iterate over each vertex element in the layout?
         // because each vertex element defines how the vertex data is laid out in the vertex buffer
          
        
        for (auto& element : _mVertexLayout.elements)
        {
            // now we iterate over each vertex element and set the vertex attribute pointers
            glVertexAttribPointer(
                element.index, // attribute location in the shader
                element.size, // number of  components (e.g., 3 for vec3)
                element.type, // data type (e.g., GL_FLOAT)
                GL_FALSE, // normalized     
                _mVertexLayout.stride, // stride
                (void*)(uintptr_t)(element.offset) // offset

            );

            // to enable the vertex attribute array
            glEnableVertexAttribArray(element.index);
        }

        // now bind indice buffer to the VAO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mEBO);

        // now unbind the VAO
        glBindVertexArray(0);

        // unbind VBO and EBO to avoid accidental modification
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

        Mesh::Mesh(const VertexLayout& layout, const std::vector<float>& vertices)
        : _mVertexLayout{layout},
        __mVertexCount{vertices.size() / (layout.stride / sizeof(float))}// calculate number of vertices
    {
                auto& graphicsAPI = Engine::GetInstance().GetGraphicsAPI();
        _mVBO =  graphicsAPI.CreateVertexBuffer(vertices);


        // generate and bind VAO
        glGenVertexArrays(1, &_mVAO);
        glBindVertexArray(_mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, _mVBO);

        // now unbind the VAO
        glBindVertexArray(0);

        // unbind VBO to avoid accidental modification
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        
    }
    void Mesh::Bind()
    {
        // by calling the glBindVertexArray function with the VAO ID, we bind the VAO for rendering
        glBindVertexArray(_mVAO);
    }

    void Mesh::Draw()
    {
        if(_mIndexCount >0)
        {
            glDrawElements(GL_TRIANGLES, _mIndexCount, GL_UNSIGNED_INT, 0);
        }

        else
        {
            glDrawArrays(GL_TRIANGLES, 0, __mVertexCount);
            // the difference between glDrawElements and glDrawArrays is that glDrawElements uses the index buffer to draw the vertices
            // while glDrawArrays draws the vertices in the order they are stored in the vertex buffer
            
        }
    }
}   