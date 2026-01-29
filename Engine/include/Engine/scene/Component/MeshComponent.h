#pragma once
#include <memory>
#include "Engine/scene/Component.h"

namespace GAMEDEV_ENGINE
{
    class Material; // forward declaration
    class Mesh; // forward declaration
    
    class MeshComponent : public Component
    {
    public:
        COMPONENT(MeshComponent)
        // constructor will accept shared pointer of material and mesh
        MeshComponent(std::shared_ptr<Material> material, std::shared_ptr<Mesh> mesh);
        
        // overriding the update method
        void Update(float deltaTime) override;
        
        // ✅ ADD THESE GETTER METHODS
        std::shared_ptr<Material> GetMaterial() const { return _mMaterialComponet; }
        std::shared_ptr<Mesh> GetMesh() const { return _mMeshComponent; }
        
    private:
        // since it is mesh component it should have material and mesh
        // we will create a shared pointer for mesh and material
        std::shared_ptr<Material> _mMaterialComponet;
        std::shared_ptr<Mesh> _mMeshComponent;
    };
}