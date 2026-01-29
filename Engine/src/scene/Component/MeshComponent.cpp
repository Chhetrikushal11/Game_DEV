#include "Engine/scene/Component/MeshComponent.h"
#include "Engine/render/mesh.h"
#include "Engine/render/material.h"
#include "Engine/render/RenderQueue.h"
#include "Engine/scene/GameObject.h"
#include "Engine/Engine.h"  // Engine.h is included to access the RenderQueue through the Engine singleton

namespace GAMEDEV_ENGINE
{
    MeshComponent::MeshComponent(std::shared_ptr<Material> material, std::shared_ptr<Mesh> mesh)
        : _mMaterialComponet{material},
         _mMeshComponent{mesh}
    {
    }

    void MeshComponent::Update(float deltaTime)
    {
        // first we need to check if material and mesh are valid
        if (_mMaterialComponet && _mMeshComponent)
        {   
            // we will create the render command and pass it to the render queue
            RenderCommand command;
                command.material = _mMaterialComponet.get();
                command.mesh = _mMeshComponent.get();
                // now we need to get the model matrix from the game object owner
                if (GetOwner())
                {
                    command.modelMatrix = GetOwner()->GetWorldTransformMatrix();
                    // now we need to submit the command to the render queue
                    // we need to get the render queue from the engine
                    // for that we need to include Engine.h
                    auto& renderQueue = Engine::GetInstance().GetRenderQueue();
                    renderQueue.Submit(command);
                }

        }
    }
}