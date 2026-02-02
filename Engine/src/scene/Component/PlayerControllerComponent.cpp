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

            // Rotation around Y axis (yaw) ===============================
          //  rotation.y -= deltaX * _mSensitivity/* * deltaTime*/;
            // Rotation around X axis (pitch)
           // rotation.x -= deltaY * _mSensitivity /** deltaTime*/;
           // for global y
            float yAngle = -deltaX * _mSensitivity * deltaTime;
            // converting it to quaterion
            glm::quat yRot = glm::angleAxis(yAngle, glm::vec3(0.0f, 1.0f, 0.0f));

            // vertical rotation around local x -axis
            float xAngle = -deltaY * _mSensitivity * deltaTime;
            glm::vec3 right = rotation * glm::vec3(1.0f, 0.0f, 0.0f); // first we need to get the right vector
            glm::quat xRot = glm::angleAxis(xAngle, right);
            
           // to combine we multiply the xRot with yRot
            glm::quat deltaRot = yRot * xRot;

            rotation = glm::normalize(deltaRot * rotation);


            // ===========================================================
            _mGameObjectOwner->SetRotation(rotation);
        }


        // ✅ Calculate forward and right vectors
        glm::vec3 front = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);  // Right is +X

        auto position = _mGameObjectOwner->GetPosition();

        // ✅ Movement (use vec3 operations, not component assignment)
        if (inputManager.IskeyPressed(GLFW_KEY_A))
        {
            position -= right * _mMoveSpeed * deltaTime;
        }
        else if (inputManager.IskeyPressed(GLFW_KEY_D))  // Uses else-if
        {
            position += right * _mMoveSpeed * deltaTime;
        }  
        
        if (inputManager.IskeyPressed(GLFW_KEY_W))
        {
            position += front * _mMoveSpeed * deltaTime;  // Move forward
        }
        else if (inputManager.IskeyPressed(GLFW_KEY_S))
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