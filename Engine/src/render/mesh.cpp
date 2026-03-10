#include "Engine/render/mesh.h"
#include "Engine/Engine.h"


namespace GAMEDEV_ENGINE
{
    Mesh::Mesh(const VertexLayout& layout, const std::vector<float>& vertices, const std::vector<uint32_t>& indices)
    : _mVertexLayout{layout},
        _mIndexCount{indices.size()},
        _mVertexCount{vertices.size() / (layout.stride / sizeof(float))}// calculate number of vertices
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
        _mVertexCount{vertices.size() / (layout.stride / sizeof(float))}// calculate number of vertices
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

    void Mesh::Unbind()
    {
        glBindVertexArray(0); // after we render the a mesh we dont need to keep it active any more
    }

    void Mesh::Draw()
    {
        if(_mIndexCount >0)
        {
            glDrawElements(GL_TRIANGLES, _mIndexCount, GL_UNSIGNED_INT, 0);
        }

        else
        {
            glDrawArrays(GL_TRIANGLES, 0, _mVertexCount);
            // the difference between glDrawElements and glDrawArrays is that glDrawElements uses the index buffer to draw the vertices
            // while glDrawArrays draws the vertices in the order they are stored in the vertex buffer
            
        }
    }

    std::shared_ptr<Mesh> Mesh::CreateBox(const glm::vec3& extents)
    {
        const glm::vec3 half = extents * 0.5f;
        std::vector<float> vertices = {
            // Position (3)      Color (3)         UV (2)

            // FRONT FACE (indices 0-3)
             half.x,  half.y,  half.z,  1.0f, 0.0f, 0.0f,  extents.x, extents.y, 0.0f, 0.0f, 1.0f, // 0: top-right
             half.x, -half.y,  half.z,  0.0f, 1.0f, 0.0f, extents.x, 0.0f, 0.0f, 0.0f, 1.0f,  // 1: bottom-right
            -half.x, -half.y,  half.z,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 2: bottom-left
            -half.x,  half.y,  half.z,  1.0f, 1.0f, 0.0f,  0.0f, extents.y,  0.0f, 0.0f, 1.0f, // 3: top-left

            // BACK FACE (indices 4-7)
             half.x,  half.y, -half.z,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f,  // 4: top-right
             half.x, -half.y, -half.z,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,// 5: bottom-right
            -half.x, -half.y, -half.z,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,// 6: bottom-left
            -half.x,  half.y, -half.z,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // 7: top-left

            // RIGHT FACE (indices 8-11)
             half.x,  half.y, -half.z,  1.0f, 0.0f, 0.0f,  extents.z, extents.y, 1.0f, 0.0f,  0.0f, // 8: top-back
             half.x, -half.y, -half.z,  0.0f, 1.0f, 0.0f,  extents.z, 0.0f, 1.0f, 0.0f,  0.0f, // 9: bottom-back
             half.x, -half.y,  half.z,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, // 10: bottom-front
             half.x,  half.y,  half.z,  1.0f, 1.0f, 0.0f,  0.0f, extents.y, 1.0f, 0.0f,  0.0f, // 11: top-front

             // LEFT FACE (indices 12-15)
             -half.x,  half.y,  half.z,  1.0f, 0.0f, 0.0f,  extents.z, extents.y, -1.0f, 0.0f,  0.0f, // 12: top-front
             -half.x, -half.y,  half.z,  0.0f, 1.0f, 0.0f,  extents.z, 0.0f, -1.0f, 0.0f,  0.0f, // 13: bottom-front
             -half.x, -half.y, -half.z,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, -1.0f, 0.0f,  0.0f, // 14: bottom-back
             -half.x,  half.y, -half.z,  1.0f, 1.0f, 0.0f,  0.0f, extents.y, -1.0f, 0.0f,  0.0f, // 15: top-back

             // TOP FACE (indices 16-19)
             -half.x,  half.y, -half.z,  1.0f, 0.0f, 0.0f,  0.0f, extents.z, 0.0f, 1.0f, 0.0f, // 16: back-left
              half.x,  half.y, -half.z,  0.0f, 1.0f, 0.0f,  extents.x, extents.z, 0.0f, 1.0f, 0.0f, // 17: back-right
              half.x,  half.y,  half.z,  0.0f, 0.0f, 1.0f,  1.0f, extents.y, 0.0f, 1.0f, 0.0f, // 18: front-right
             -half.x,  half.y,  half.z,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 19: front-left

             // BOTTOM FACE (indices 20-23)
             -half.x, -half.y,  half.z,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, extents.z, -1.0f, 0.0f, // 20: front-left
              half.x, -half.y,  half.z,  0.0f, 1.0f, 0.0f,  1.0f, extents.x,  extents.z, -1.0f, 0.0f,  // 21: front-right
              half.x, -half.y, -half.z,  0.0f, 0.0f, 1.0f,  1.0f, extents.x, 0.0f, -1.0f, 0.0f, // 22: back-right
             -half.x, -half.y, -half.z,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f   // 23: back-left
        };

        std::vector<unsigned int> indices = {
            // FRONT FACE
            0, 1, 2,
            0, 2, 3,

            // BACK FACE
            4, 6, 5,
            4, 7, 6,

            // RIGHT FACE
            8, 9, 10,
            8, 10, 11,

            // LEFT FACE
            12, 13, 14,
            12, 14, 15,

            // TOP FACE
            16, 17, 18,
            16, 18, 19,

            // BOTTOM FACE
            20, 21, 22,
            20, 22, 23
        };

        // now creating vertex layout object and define the layout structure
        GAMEDEV_ENGINE::VertexLayout  vertexLayout;

        vertexLayout.elements.push_back(
            {
                VertexElement::PositionIndex,
                3, // number of position attribute
                GL_FLOAT, // data type
                0
            }
        );

        // for color
        vertexLayout.elements.push_back(
            {
                VertexElement::ColorIndex,
                3,
                GL_FLOAT,
                sizeof(float) * 3
            });

        // for UV
        vertexLayout.elements.push_back(
            {
                VertexElement::UVIndex,
                2,
                GL_FLOAT,
                sizeof(float) * 6
            });

            // for Nomrmal
          vertexLayout.elements.push_back(
                {
                    VertexElement::NormalIndex,
                    3,
                    GL_FLOAT,
                    sizeof(float) * 8
                }
        );

        vertexLayout.stride = sizeof(float) * 11;
        auto result = std::make_shared<GAMEDEV_ENGINE::Mesh>(vertexLayout, vertices, indices);
        return result;
    }

    std::shared_ptr<Mesh> Mesh::CreateSphere(float radius, int sectors, int stacks)
    {
        const float PI = 3.14159265358979323846f;
        std::vector<float> vertices((stacks + 1) * (sectors + 1) * 8);
        for (int i = 0; i <= stacks; ++i)
        {
            float stackAngle = PI / 2.0f - static_cast<float>(i) * (PI / static_cast<float>(stacks)); // from -PI/2 to PI/2
            float xy = radius * cosf(stackAngle); // x-y plane radius at this stack
            float z = radius * sinf(stackAngle); // z coordinates

            for (int j = 0; j <= sectors; ++j)
            {
                float sectorAngle = static_cast<float>(j) * (2.0f * PI / static_cast<float>(sectors)); // from 0 to 2PI
                float x = xy * cosf(sectorAngle); 
                float y = xy * sinf(sectorAngle); 

                size_t vertexStart = (i * (sectors + 1) + j) * 8;

                // Position
                vertices[vertexStart] = x;
                vertices[vertexStart + 1] = y;
                vertices[vertexStart + 2] = z;

                // Normal (normalized position vector)
                float length = sqrtf(x * x + y * y + z * z);
                vertices[vertexStart + 3] = x / length;
                vertices[vertexStart + 4] = y / length;
                vertices[vertexStart + 5] = z / length;

                // UV coordinates
                vertices[vertexStart + 6] = static_cast<float>(j) / static_cast<float>(sectors);
                vertices[vertexStart + 7] = static_cast<float>(i) / static_cast<float>(stacks);

            }

        }
        // Generate Indices
        std::vector<unsigned int> indices;
        for (int i = 0; i < stacks; ++i)
        {
            int k1 = i * (sectors + 1);
            int k2 = k1 + sectors + 1;

            for (int j = 0; j < sectors; ++j, ++k1, ++k2)
            {
                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (stacks - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

        VertexLayout vertexLayout;

        // Position
        vertexLayout.elements.push_back(
            {
                VertexElement::PositionIndex,
                3,
                GL_FLOAT,
                0
            });

        // Normal
        vertexLayout.elements.push_back(
            {
                VertexElement::NormalIndex,
                3,
                GL_FLOAT,
                sizeof(float) * 3
            });
        // UV Index
        vertexLayout.elements.push_back(
            {
                VertexElement::UVIndex,
                2,
                GL_FLOAT,
                sizeof(float) * 6
            });
        vertexLayout.stride = sizeof(float) * 8;

        auto result = std::make_shared<Mesh>(vertexLayout, vertices, indices); // here we create a mesh
        return result;
    }

#if 0
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
                cgltf_accessor* accessors[4] = { nullptr, nullptr, nullptr, nullptr }; // need three nullptr for instantition

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
                    case cgltf_attribute_type_normal:  // number of tex coordinates
                    {
                        accessors[VertexElement::NormalIndex] = acc;
                        element.index = VertexElement::NormalIndex;
                        element.size = 3;
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
#endif
}   