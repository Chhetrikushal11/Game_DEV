#include "Engine/scene/Component/CameraComponent.h"
#include "Engine/scene/GameObject.h"

namespace GAMEDEV_ENGINE
{
    void CameraComponent::Update(float deltaTime)
    {
        // Camera-specific update logic can be added here
    }

    glm::mat4 CameraComponent::GetViewMatrix() const
    {
        if (GetOwner())
        {
           return  glm::inverse(_mGameObjectOwner->GetWorldTransformMatrix());
        }
        
    }

    glm::mat4 CameraComponent::GetProjectionMatrix() const
    {
        // For simplicity, we return an identity matrix here.
        // In a real implementation, you would return a proper projection matrix (e.g., perspective or orthographic).
        return glm::mat4(1.0f);
    }
}
