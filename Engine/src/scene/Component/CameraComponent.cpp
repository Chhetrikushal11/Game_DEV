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
        glm::mat4 mat = glm::mat4(1.0f);
        //first apply the rotation
        mat = glm::mat4_cast(_mGameObjectOwner->GetRotation());
        // now translation
       
        // instead of translating after rotation it just  push directly the matrix the translation into the matrix directly
        mat[3] = glm::vec4(_mGameObjectOwner->GetPosition(), 1.0f);

        // incase camera have the parents
        if (_mGameObjectOwner->GetParent())
        {
            // we multiply to parent world transform
            mat = _mGameObjectOwner->GetParent()->GetWorldTransformMatrix() * mat;
        }
        // then we inverse the mat

         return  glm::inverse(mat);
  
    }

    glm::mat4 CameraComponent::GetProjectionMatrix(float aspect) const
    {
        // For simplicity, we return an identity matrix here.
        // In a real implementation, you would return a proper projection matrix (e.g., perspective or orthographic).
        return glm::perspective(glm::radians(_mfov), aspect, _mNearPlane, _mFarPlane);
    }

}
