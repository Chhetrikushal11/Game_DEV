#include <glm/gtc/matrix_transform.hpp>

#include "Engine/scene/Component/CameraComponent.h"
#include "Engine/scene/GameObject.h"

#include <iostream>


namespace GAMEDEV_ENGINE
{
    void CameraComponent::Update(float deltaTime)
    {
        // Print camera info every 60 frames (~1 second at 60fps)
        static int frameCount = 0;
        if (++frameCount % 60 == 0)
        {
            if (_mGameObjectOwner)
            {
                glm::vec3 pos = _mGameObjectOwner->GetPosition();
                std::cout << "Camera Position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
                std::cout << "Camera FOV: " << _mfov << "°" << std::endl;
                std::cout << "Near/Far: " << _mNearPlane << "/" << _mFarPlane << std::endl;
            }
        }
    }

    glm::mat4 CameraComponent::GetViewMatrix() const
    {
        if (GetOwner())
        {
           return  glm::inverse(_mGameObjectOwner->GetWorldTransformMatrix());
        }
        
    }

    glm::mat4 CameraComponent::GetProjectionMatrix(float aspect) const
    {
        // For simplicity, we return an identity matrix here.
        // In a real implementation, you would return a proper projection matrix (e.g., perspective or orthographic).
        return glm::perspective(glm::radians(_mfov), aspect, _mNearPlane, _mFarPlane);
    }

}
