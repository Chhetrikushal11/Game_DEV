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
            glm::mat4 GetProjectionMatrix() const;

        private:
            // Camera properties can be added here
    };
}