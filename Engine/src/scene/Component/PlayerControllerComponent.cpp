#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>  // ✅ Added for glm::rotate
#include <glm/vec4.hpp>
#include "Engine/scene/Component/PlayerControllerComponent.h"
#include "Engine/Engine.h"
#include "Engine/input/InputManager.h"

namespace GAMEDEV_ENGINE
{
    void PlayerControllerComponent::Update(float deltaTime)
    {
        auto& inputManager = Engine::GetInstance().GetInputManager();
        auto rotation = _mGameObjectOwner->GetRotation();

        // Mouse rotation
        if (inputManager.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            const auto& oldPos = inputManager.GetMousePositionOld();
            const auto& current = inputManager.GetMousePositionCurrent(); 

            float deltaX = current.x - oldPos.x; 
            float deltaY = current.y - oldPos.y;

            // Rotation around Y axis (yaw)
            rotation.y -= deltaX * _mSensitivity * deltaTime;
            // Rotation around X axis (pitch)
            rotation.x -= deltaY * _mSensitivity * deltaTime;

            _mGameObjectOwner->SetRotation(rotation);
        }

        // ✅ Build rotation matrix (accumulate rotations, don't redeclare)
        glm::mat4 rotMat(1.0f);
        rotMat = glm::rotate(rotMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw (Y)
        rotMat = glm::rotate(rotMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch (X)
        rotMat = glm::rotate(rotMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll (Z)

        // ✅ Calculate forward and right vectors
        glm::vec3 front = glm::normalize(glm::vec3(rotMat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f))); // Forward is -Z
        glm::vec3 right = glm::normalize(glm::vec3(rotMat * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));  // Right is +X

        auto position = _mGameObjectOwner->GetPosition();

        // ✅ Movement (use vec3 operations, not component assignment)
        if (inputManager.IskeyPressed(GLFW_KEY_A))
        {
            position -= right * _mMoveSpeed * deltaTime;  // Move left
        }
        if (inputManager.IskeyPressed(GLFW_KEY_D))
        {
            position += right * _mMoveSpeed * deltaTime;  // Move right
        }
        if (inputManager.IskeyPressed(GLFW_KEY_W))
        {
            position += front * _mMoveSpeed * deltaTime;  // Move forward
        }
        if (inputManager.IskeyPressed(GLFW_KEY_S))
        {
            position -= front * _mMoveSpeed * deltaTime;  // Move backward
        }
        if (inputManager.IskeyPressed(GLFW_KEY_H))
        {
            position = glm::vec3(0.0f, 0.0f, 0.0f);  // Reset to origin
        }

        _mGameObjectOwner->SetPosition(position);
    }
}