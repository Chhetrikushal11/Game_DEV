#include "Engine/render/mesh.h"
#include "Engine/Engine.h"

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

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

    std::shared_ptr<Mesh> Mesh::Load(const std::string& path)
    {
        auto contentsRead = Engine::GetInstance().GetAssetFileSystem().LoadAssetFileText(path);

        if (contentsRead.empty())
        {
            return nullptr;
        }

        // what is accessor ?
        auto readFloats = [](const cgltf_accessor* acc, cgltf_size i, float* out, int n)
            {
                std::fill(out, out + n, 0.0f);
                return cgltf_accessor_read_float(acc, i, out, n) == 1;


            };

        auto readIndex = [](const cgltf_accessor* acc, cgltf_size i)
            {
                cgltf_uint out = 0;
                cgltf_bool ok = cgltf_accessor_read_uint(acc, i, &out, 1);
                return ok ? static_cast<uint32_t>(out) : 0;
            };

        cgltf_options options = {};
        cgltf_data* data = nullptr;

        cgltf_result res = cgltf_parse(&options, contentsRead.data(), contentsRead.size(), &data);
        if (res != cgltf_result_success)
        {
            return nullptr;
        }

        auto fullPath = Engine::GetInstance().GetAssetFileSystem().GetAssetsFolder() / path;

        res = cgltf_load_buffers(&options, data, fullPath.remove_filename().string().c_str());
        if (res != cgltf_result_success)
        {
            cgltf_free(data);
            return nullptr;
        }

        std::shared_ptr<Mesh> result = nullptr;

        for (cgltf_size mi = 0; mi < data->meshes_count; ++mi)
        {
            auto mesh = data->meshes[mi];
            for (cgltf_size pi = 0; pi < mesh.primitives_count; pi++)
            {
                // what is primitive loading?
                auto& primitive = mesh.primitives[pi];
                if (primitive.type != cgltf_primitive_type_triangles)
                {
                    continue;
                }

                // here we process primitve loading
                VertexLayout vertexLayout;
                cgltf_accessor* accessors[3] = { nullptr, nullptr, nullptr }; // need three nullptr for instantition

                for (cgltf_size ai = 0; ai < primitive.attributes_count; ++ai)
                {
                    auto& attr = primitive.attributes[ai];
                    auto acc = attr.data;
                    if (!acc)
                    {
                        continue;
                    }

                    VertexElement element;
                    element.type = GL_FLOAT; //why?

                    switch (attr.type)
                    {
                    case cgltf_attribute_type_position:
                    {
                        accessors[VertexElement::PositionIndex] = acc;
                        element.index = VertexElement::PositionIndex;
                        element.size = 3;
                    }
                    break;
                    case cgltf_attribute_type_color:
                    {
                        // working with channel 0
                        if (attr.index != 0)
                        {
                            continue;
                        }
                        accessors[VertexElement::ColorIndex] = acc;
                        element.index = VertexElement::ColorIndex;
                        element.size = 3;
                    }
                    break;
                    case cgltf_attribute_type_texcoord:  // number of tex coordinates
                    {
                        // working with channel 0
                        if (attr.index != 0)
                        {
                            continue;
                        }
                        accessors[VertexElement::UVIndex] = acc;
                        element.index = VertexElement::UVIndex;
                        element.size = 2;
                    }
                    break;
                    default:
                        continue;
                    }

                    // need to push to vertex layout
                    if (element.size > 0)
                    {
                        element.offset = vertexLayout.stride;
                        vertexLayout.stride += element.size * sizeof(float);
                        vertexLayout.elements.push_back(element);
                    }
                }
                // after we get layout we can finally get the buffer
                if (!accessors[VertexElement::PositionIndex])
                {
                    continue;
                }

                auto vertexCount = accessors[VertexElement::PositionIndex]->count;
                // resize vertex buffers

                std::vector<float> vertices;
                vertices.resize((vertexLayout.stride / sizeof(float)) * vertexCount);

                for (cgltf_size vi = 0; vi < vertexCount; ++vi)
                {
                    for(auto& el : vertexLayout.elements)
                    { 
                        if (!accessors[el.index])
                        {
                            continue;
                        }

                        auto index = (vi * vertexLayout.stride + el.offset) / sizeof(float);
                        float* outData = &vertices[index];
                        readFloats(accessors[el.index], vi, outData, el.size);
                    }
                }

                if (primitive.indices)
                {
                    auto indexCount = primitive.indices->count;
                    std::vector<uint32_t> indices(indexCount);
                    for (cgltf_size i = 0; i < indexCount; ++i)
                    {
                        indices[i] = readIndex(primitive.indices, i);
                    }
                    result = std::make_shared<Mesh>(vertexLayout, vertices, indices);
                }

                else
                {
                    result = std::make_shared<Mesh>(vertexLayout, vertices);
                }

                if (result)
                {
                    break;
                }

            }
            if (result)
            {
                break;
            }
        }
        // free cgltf data
        cgltf_free(data);
        return result;
    }
}   