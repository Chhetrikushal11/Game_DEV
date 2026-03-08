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
            std::string matPath;

            // Format 1: "material": "materials/brick.mat"
            if (json["material"].is_string())
            {
                matPath = json["material"].get<std::string>();
            }
            // Format 2: "material": { "path": "materials/brick.mat" }
            else if (json["material"].is_object() && json["material"].contains("path"))
            {
                matPath = json["material"]["path"].get<std::string>();
            }

            if (!matPath.empty())
            {
                auto material = Material::Load(matPath);
                if (material)
                {
                    SetMaterial(material);
                }
            }
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