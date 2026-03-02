#include "Player/Player.h"

namespace GAMEDEV_ENGINE
{
	void Player::Init()
	{
       
        AddComponent(new CameraComponent());
        SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
        AddComponent(new PlayerControllerComponent());
        
        // for the FPS
        auto gunObj = GameObject::LoadGLTF("models/sten_gunmachine_carbine/scene.gltf");
        // we will attach weapon as child object of the camera
        gunObj->SetParent(this); // this represent the Camera component
        gunObj->SetPosition(glm::vec3(0.75f, -0.5f, -0.75f));
        gunObj->SetScale(glm::vec3(-1.0f, 1.0f, 1.0f));

        // In Init() — don't play on startup, just register
        if (auto anim = gunObj->GetComponent<AnimationComponent>())
        {
            // hide bullet and fire effects
            if (auto bullet = gunObj->FindChildByName("bullet_33"))
                bullet->SetActive(false);
            if (auto fire = gunObj->FindChildByName("BOOM_35"))
                fire->SetActive(false);
            // Don't call anim->Play() here — let mouse click trigger it
        }
        _mAnimationComponent = gunObj->GetComponent<AnimationComponent>();
       

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