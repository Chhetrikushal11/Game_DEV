#include "Engine/scene/Component/MeshComponent.h"
#include "Engine/render/mesh.h"
#include "Engine/render/material.h"
#include "Engine/render/RenderQueue.h"
#include "Engine/scene/GameObject.h"
#include "Engine/Engine.h"

namespace GAMEDEV_ENGINE
{
    MeshComponent::MeshComponent(std::shared_ptr<Material> material, std::shared_ptr<Mesh> mesh)
        : _mMaterialComponent{ material },
        _mMeshComponent{ mesh }
    {
    }

    void MeshComponent::LoadProperties(const nlohmann::json& json)
    {
        // ── Material ─────────────────────────────────────────────────────────────
        if (json.contains("material"))
        {
            auto& matObj = json["material"];
            const std::string path = matObj.value("path", "");
            auto mat = Material::Load(path);
            if (mat && matObj.contains("params"))
            {
                auto& paramsObj = matObj["params"];
                // for floats
                if (paramsObj.contains("float"))
                {
                    for (auto& p : paramsObj["float"])
                    {
                        std::string name = p.value("name", "");
                        float value = p.value("value", 0.0f);
                        mat->SetFloatParams(name, value);
                    }
                }
                // for 2 params
                if (paramsObj.contains("float2"))
                {
                    for (auto& p : paramsObj["float2"])
                    {
                        std::string name = p.value("name", "");
                        float v0 = p.value("value0", 0.0f);
                        float v1 = p.value("value1", 0.0f);
                        mat->SetFloatParams2f(name, v0, v1);
                    }
                }

                // for 3 params
                if (paramsObj.contains("float3"))
                {
                    for (auto& p : paramsObj["float3"])
                    {
                        std::string name = p.value("name", "");
                        float v0 = p.value("value0", 0.0f);
                        float v1 = p.value("value1", 0.0f);
                        float v2 = p.value("value2", 0.0f);
                        mat->SetFloatParams3f(name, glm::vec3(v0, v1, v2));
                    }
                }

                // for texture
                           // for 2 params
                if (paramsObj.contains("texture"))
                {
                    for (auto& p : paramsObj["texture"])
                    {
                        std::string name = p.value("name", "");
                        std::string texturePath = p.value("path", "");
                        auto texture = Texture::Load(texturePath);
                        mat->SetTextureParams(name, texture);
                    }

                }
            }
            std::string matPath;
            SetMaterial(mat);

        }

        // ── Mesh ─────────────────────────────────────────────────────────────────
        if (json.contains("mesh"))
        {
            const auto& meshObj = json["mesh"];
            const std::string type = meshObj.value("type", "box");
            if (type == "box")
            {
                glm::vec3 extents(
                    meshObj.value("x", 1.0f),
                    meshObj.value("y", 1.0f),
                    meshObj.value("z", 1.0f)
                );
                auto mesh = Mesh::CreateBox(extents);
                SetMesh(mesh);
            }
        }
    }

    void MeshComponent::Update(float deltaTime)
    {
        if (_mMaterialComponent && _mMeshComponent)
        {
            RenderCommand command;
            command.material = _mMaterialComponent.get();
            command.mesh = _mMeshComponent.get();
            if (GetOwner())
            {
                command.modelMatrix = GetOwner()->GetWorldTransformMatrix();
                auto& renderQueue = Engine::GetInstance().GetRenderQueue();
                renderQueue.Submit(command);
            }
        }
    }

    void MeshComponent::SetMaterial(const std::shared_ptr<Material>& material)
    {
        _mMaterialComponent = material;
    }

    void MeshComponent::SetMesh(const std::shared_ptr<Mesh>& mesh)
    {
        _mMeshComponent = mesh;
    }
}