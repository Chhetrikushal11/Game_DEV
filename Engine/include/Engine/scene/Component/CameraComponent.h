#pragma once
#include <glm/mat4x4.hpp>
#include "Engine/scene/Component.h"

namespace GAMEDEV_ENGINE
{
    class CameraComponent : public Component
    {
        public:
         

            void Update(float deltaTime) override;
            glm::mat4 GetViewMatrix() const;
            glm::mat4 GetProjectionMatrix(float aspect) const;

        private:
            // Camera properties can be added here
            float _mfov = 60.0f;
            // for near plane
            float _mNearPlane = 0.1f;
            float _mFarPlane = 1000.0f;
    };
}