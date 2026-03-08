#pragma once
#include<string>
#include <memory>
#include "Engine/scene/Component.h"

namespace GAMEDEV_ENGINE
{
    class Material; // forward declaration
    class Mesh; // forward declaration
    
    class MeshComponent : public Component
    {
    public:
        MeshComponent() = default;
        COMPONENT(MeshComponent)
        // constructor will accept shared pointer of material and mesh
        MeshComponent(std::shared_ptr<Material> material, std::shared_ptr<Mesh> mesh);
       
        // loading properties
        void LoadProperties(const nlohmann::json& json) override;
        
        // overriding the update method
        void Update(float deltaTime) override;

        // ADD THE SET METHODS
        void SetMaterial(const std::shared_ptr<Material>& material);
        void SetMesh(const std::shared_ptr<Mesh>& mesh);
        
        // ✅ ADD THESE GETTER METHODS
        std::shared_ptr<Material> GetMaterial() const { return _mMaterialComponent; }
        std::shared_ptr<Mesh> GetMesh() const { return _mMeshComponent; }
        
    private:
        // since it is mesh component it should have material and mesh
        // we will create a shared pointer for mesh and material
        std::shared_ptr<Material> _mMaterialComponent;
        std::shared_ptr<Mesh> _mMeshComponent;
    };
}