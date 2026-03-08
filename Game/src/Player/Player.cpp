#include "Player/Player.h"

namespace GAMEDEV_ENGINE
{
    void Player::Init()
    {
        if (auto gunObj = FindChildByName("Gun"))
        {
            _mAnimationComponent = gunObj->GetComponent<AnimationComponent>();
            if (auto bullet = gunObj->FindChildByName("bullet_33"))
                bullet->SetActive(false);
            if (auto fire = gunObj->FindChildByName("BOOM_35"))
                fire->SetActive(false);
        }
    }

    void Player::Update(float deltaTime)
    {
        GameObject::Update(deltaTime);
        auto& input = Engine::GetInstance().GetInputManager();
        if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            if (_mAnimationComponent && !_mAnimationComponent->IsPlaying())
            {
                _mAnimationComponent->Play("shoot", false);
            }
        }
    }
}