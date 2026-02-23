#include "Engine/scene/GameObject.h"
#include "Engine/Engine.h"
#include "Engine/graphics/VertexLayout.h"
#include "Engine/render/material.h"
#include "Engine/render/mesh.h"
#include "Engine/graphics/Texture.h"
#include "Engine/scene/Component/MeshComponent.h"

#include <glm/gtc/matrix_transform.hpp> // for glm::translate, glm::rotate, glm::scale
#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>


// to load gltf we need to define CGLTF_IMPLEMENTATION
#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

namespace GAMEDEV_ENGINE
{
    void GameObject::Update(float deltaTime)
    {
        // iterate the _mComponents and call their update methods
        for (auto& component : _mComponents)
        {
            component->Update(deltaTime);
        
        }
        // here we iterate over children containers and call their update methods
        for (auto childrenIt = _mChildren.begin(); childrenIt != _mChildren.end(); )
        {
            GameObject* child = childrenIt->get();
            if(child->IsAlive())
            {
                child->Update(deltaTime);
                ++childrenIt;
            }
            else
            {
                // remove the child from the children vector
                childrenIt = _mChildren.erase(childrenIt);
            }
        }
    }

    void GameObject::SetName(const std::string& name)
    {
        _mName = name;
    }

    bool GameObject::SetParent(GameObject* parent)
    {
        if (!_mScene)
        {
            return false;
        }

        return _mScene->SetParent(this, parent);
           
    }

    // void GameObject::SetParent(GameObject* parent)
    // {
    //     _mParent = parent;
    // }

    void GameObject::AddComponent(Component* component)
    {
        if(component)
        {
            _mComponents.emplace_back(component);
            component->_mGameObjectOwner = this; // setting the owner pointer
            // this can be access the component member as GameObject is declared as friend class in Component class

        }
    }



    void GameObject::MarkForDestroy()
    {
        if(_misAlive)
        {
            _misAlive = false;
        }
    }

    // for local transformation matrix
    glm::mat4 GameObject::GetLocalTransformMatrix() const
    {
        // this method combines translation, rotation, and scaling into a single transformation matrix

        // step 1 : Creating the identity matrix
        glm::mat4 identityMatrix = glm::mat4(1.0f); 
        
        // step 2: Translation first
        glm::mat4 translationMatrix = glm::translate(identityMatrix, _mPosition);

        // step 3: Rotation second (applying in ZYX order)
        
            // --- using quaternion --
        glm::mat4 rotationMatrix = glm::mat4_cast(_mRotation); // here glm::mat4_cast simply convert quat to mat4 for rotation

        //glm::mat4 rotationXMatrix = glm::rotate(identityMatrix, _mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around X axis
        //glm::mat4 rotationYMatrix = glm::rotate(identityMatrix, _mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); // rotate around X axis
        //glm::mat4 rotationZMatrix = glm::rotate(identityMatrix, _mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f)); // rotate around X axis
        // combine rotations
       /* glm::mat4 rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;*/



        // step 4: Scaling last
        glm::mat4 scaleMatrix = glm::scale(identityMatrix, _mScale);

        // step 5: combine all transformations: Translation * Rotation * Scale  
        return translationMatrix * rotationMatrix * scaleMatrix;
    }   
    
    // for world transformation matrix
    glm::mat4 GameObject::GetWorldTransformMatrix() const
    {
        if(_mParent)
        {
            return _mParent->GetWorldTransformMatrix() * GetLocalTransformMatrix();
        }
        else
        {
            return GetLocalTransformMatrix();
        }   
        // this way every game object can be compute with respect to world scene rather than just local space
    } 

    // for world position
    glm::vec3 GameObject::GetWorldPosition()
    {
        glm::vec4 hom = GetWorldTransformMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        return glm::vec3(hom) / hom.w;
    }
    
    void ParseGLTFNode(cgltf_node* node, GameObject* parent, std::filesystem::path& folder)
    {
        // this function will create the gameobject for the node
         auto object = parent->GetScene()->CreateGameObject(node->name, parent);

        // handling transformation
        if (node->has_matrix)
        {
            auto mat = glm::make_mat4(node->matrix);
            glm::vec3 translation, scale, skew;
            glm::vec4  perspective;
            glm::quat orientation;
            glm::decompose(mat, scale, orientation, translation, skew, perspective);

            object->SetPosition(translation);
            object->SetRotation(orientation);
            object->SetScale(scale);
        }
        else
        {
            if (node->has_translation)
            {
                object->SetPosition(glm::vec3(node ->translation[0],
                    node->translation[1],
                    node->translation[2]));
            }
            if (node->has_rotation)
            {
                object->SetRotation(glm::quat(node->rotation[3],
                    node->rotation[0],
                    node->rotation[1],
                    node->rotation[2]));
            }
            if (node->has_scale)
            {
                object->SetScale(glm::vec3(node->scale[0],
                    node->scale[1],
                    node->scale[2]));
            }
        }

        if (node->mesh)
        {
            for (cgltf_size pi = 0; pi < node->mesh->primitives_count; ++pi)
            {
                auto& primitive = node->mesh->primitives[pi];
                if (primitive.type != cgltf_primitive_type_triangles)
                {
                    continue;
                }
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

                // here we process primitve loading
                VertexLayout vertexLayout;
                cgltf_accessor* accessors[4] = { nullptr, nullptr, nullptr, nullptr };

                // iterate through all the attributes
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
                std::vector<float> vertices;
                vertices.resize((vertexLayout.stride / sizeof(float))* vertexCount);

                for (cgltf_size vi = 0; vi < vertexCount; ++vi)
                {
                    for (auto& el : vertexLayout.elements)
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
                std::shared_ptr<Mesh> mesh;
                if (primitive.indices)
                {
                    auto indexCount = primitive.indices->count;
                    std::vector<uint32_t> indices(indexCount);
                    for (cgltf_size i = 0; i < indexCount; ++i)
                    {
                        indices[i] = readIndex(primitive.indices, i);
                    }
                    mesh = std::make_shared<Mesh>(vertexLayout, vertices, indices);
                }

                else
                {
                    mesh = std::make_shared<Mesh>(vertexLayout, vertices);
                }
                
                // gltf material are PBR based

                auto mat = std::make_shared<Material>();
                mat->SetShaderProgram(Engine::GetInstance().GetGraphicsAPI().GetDefaultShaderProgram());
                
                if (primitive.material)
                {
                    auto gltfMat = primitive.material;
                    if (gltfMat->has_pbr_metallic_roughness)
                    {
                        auto pbr = gltfMat->pbr_metallic_roughness;
                        auto texture = pbr.base_color_texture.texture;
                        if (texture && texture->image)
                        {
                            if (texture->image->uri)
                            {
                                auto path = folder / std::string(texture->image->uri);
                                auto tex = Texture::Load(path.string());
                                mat->SetTextureParams("baseColorTexture", tex);
                            }
                        }
                    }
                    else if (gltfMat->has_pbr_specular_glossiness)
                    {
                        auto pbr = gltfMat->pbr_specular_glossiness;
                        auto texture = pbr.diffuse_texture.texture;
                        if (texture && texture->image)
                        {
                            if (texture->image->uri)
                            {
                                auto path = folder / std::string(texture->image->uri);
                                auto tex = Texture::Load(path.string());
                                mat->SetTextureParams("baseColorTexture", tex);
                            }
                        }
                    }

                    object->AddComponent(new MeshComponent(mat, mesh));
                }
            }
        }
        for (cgltf_size ci = 0; ci < node->children_count; ++ci)
        {
            ParseGLTFNode(node->children[ci], object, folder);
        }
    }


    
    GameObject* GameObject::LoadGLTF(const std::string& path)
    {
        // we will follow same procedure we follow in mesh load
        // step 1 :Read the file content
         // we can load file using GetInstance()
        auto contents = Engine::GetInstance().GetAssetFileSystem().LoadAssetFile(path);
        if (contents.empty())
        {
            return nullptr;
        }

        cgltf_options options = {};
        cgltf_data* data = nullptr;
        cgltf_result res = cgltf_parse(&options, contents.data(), contents.size(), &data);
        if (res != cgltf_result_success)
        {
            return nullptr;
        }
        auto fullPath = Engine::GetInstance().GetAssetFileSystem().GetAssetsFolder() / path;
        // also need to store relative folder path
        auto fullFolderPath = fullPath.remove_filename();
        auto relativeFolderPath = std::filesystem::path(path).remove_filename();
        res = cgltf_load_buffers(&options, data, fullPath.remove_filename().string().c_str());
        if (res != cgltf_result_success)
        {
            cgltf_free(data);
            return nullptr;
        }

        // gltf store file data as node
            // a gltf can have more than one nodes so we need one root gameobject to hold everything
        auto resultObject = Engine::GetInstance().GetCurrentScene()->CreateGameObject("Result");
        auto scene = &data->scenes[0]; // to use it as pointer
       
        for (cgltf_size i = 0; i < scene->nodes_count; ++i)
        {
            auto node = scene->nodes[i];
            ParseGLTFNode(node, resultObject, relativeFolderPath);
        }

        cgltf_free(data);
        return resultObject;


    }

} // namespace GAMEDEV_ENGINE   