#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>  // ✅ Added for glm::rotate
#include <glm/vec4.hpp>
#include "Engine/scene/Component/PlayerControllerComponent.h"
#include "Engine/Engine.h"
#include "Engine/input/InputManager.h"

namespace GAMEDEV_ENGINE
{
    void PlayerControllerComponent::Init()
    {
        std::cout << "PlayerController Init start" << std::endl;
        _mKinematicsController = std::make_unique<KinematicsCharacterController>(0.4f, 1.2f, _mGameObjectOwner->GetWorldPosition());
        std::cout << "PlayerController Init done" << std::endl;
    }
    void PlayerControllerComponent::Update(float deltaTime)
    {
        auto& inputManager = Engine::GetInstance().GetInputManager();
        auto rotation = _mGameObjectOwner->GetRotation();
        // ── Tab toggle ── must be OUTSIDE the mouse check ──────────────
        bool tabNow = inputManager.IskeyPressed(GLFW_KEY_TAB);
        if (tabNow && !_mTabWasPressed)
        {
            _mCursorLocked = !_mCursorLocked;
            glfwSetInputMode(Engine::GetInstance().GetWindow(),
                GLFW_CURSOR,
                _mCursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        }
        _mTabWasPressed = tabNow;
        // ──────────────────────────────────────────────────────────────────

        // Mouse rotation
        if (inputManager.IsMousePositionChanged() && _mCursorLocked)
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
            float yAngle = -deltaX * _mSensitivity;
            // converting it to quaterion
            glm::quat yRot = glm::angleAxis(yAngle, glm::vec3(0.0f, 1.0f, 0.0f));

            // Pitch — clamp accumulated total
            _mXRot += -deltaY * _mSensitivity;
            _mXRot = std::clamp(_mXRot, glm::radians(-80.0f), glm::radians(89.0f));

            // vertical rotation around local x -axis
            glm::vec3 right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);
            glm::quat xRot = glm::angleAxis(_mXRot, right);
           // to combine we multiply the xRot with yRot
            glm::quat deltaRot = yRot * xRot;

            rotation = glm::normalize(deltaRot * rotation);


            // ===========================================================
            _mGameObjectOwner->SetRotation(rotation);


        }


        // ✅ Calculate forward and right vectors
        glm::vec3 front = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);  // Right is +X

        glm::vec3 move(0.0f);

        // ✅ Movement (use vec3 operations, not component assignment)
        if (inputManager.IskeyPressed(GLFW_KEY_A))
        {
            move -= right;
        }
        else if (inputManager.IskeyPressed(GLFW_KEY_D))  // Uses else-if
        {
            move += right;
        }  
        
        if (inputManager.IskeyPressed(GLFW_KEY_W))
        {
            move += front;
        }
        else if (inputManager.IskeyPressed(GLFW_KEY_S))
        {
            move -= front;
        }

        if (inputManager.IskeyPressed(GLFW_KEY_SPACE))
        {
            _mKinematicsController->Jump(glm::vec3(0.0f, 5.0f, 0.0f));
        }

        if (inputManager.IskeyPressed(GLFW_KEY_H))
        {
            _mGameObjectOwner->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        }


        if (glm::dot(move, move) > 0)
        {
            move = glm::normalize(move);
        }
        
        _mKinematicsController->Walk(move * _mMoveSpeed * deltaTime);

        _mGameObjectOwner->SetPosition(_mKinematicsController->GetPosition());
      
    }

    bool PlayerControllerComponent::OnGround() const
    {
        if (_mKinematicsController)
        {
            return _mKinematicsController->OnGround();
        }

        return false;
    }
}