#include "Player/Player.h"
#include "Engine/scene/Component/PhysicsComponent.h"
#include "Bullet.h"

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
            _mAudioComponent = GetComponent<AudioComponent>();
            _mPlayerControllerComponent = GetComponent<PlayerControllerComponent>();
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
                if (_mAudioComponent)
                {
                    if (_mAudioComponent->IsPlaying("shoot"))
                    {
                        _mAudioComponent->Stop("shoot");
                    }
                    _mAudioComponent->Play("shoot");
                }

                // for the bullet 
                auto bullet = _mScene->CreateGameObject<Bullet>("Bullet");
                // to provide bullet material
                auto bulletMaterial = Material::Load("materials/suzanne.mat");
                if (!bulletMaterial)
                {
                    bulletMaterial = Material::Load("materials/brick.mat"); // fallback
                }
                // to add the mesh for bullet
                auto bulletMesh = Mesh::CreateSphere(0.2, 32, 32);
                bullet->AddComponent(new MeshComponent(bulletMaterial, bulletMesh));

                glm::vec3 pos = glm::vec3(0.0f);

                // to spawn bullet from the mussel of the gun
                if (auto gunObj = FindChildByName("Gun"))
                {
                    if (auto child = gunObj->FindChildByName("BOOM_35"))
                    {
                        pos = child->GetWorldPosition();
                    }
                }
                bullet->SetPosition(pos + (_mRotation * glm::vec3(-0.2f, 0.2f, -1.75f)));
                // now we add collider and rigidbody to bullet
                auto collider = std::make_shared<SphereCollider>(0.2f);
                auto rigidBody = std::make_shared<RigidBody>(
                BodyType::Dynamic, collider, 10.0f, 0.1f);
                bullet->AddComponent(new PhysicsComponent(rigidBody));

                glm::vec3 frontVector = _mRotation * glm::vec3(0.0f, 0.0f, -1.0f);
                rigidBody->ApplyImpulse(frontVector * 500.0f);
              //  bullet->Init();


            }
        }
        // incase of the Jumping
        if (input.IskeyPressed(GLFW_KEY_SPACE))
        {
               if (_mAudioComponent && !_mAudioComponent->IsPlaying("jump"))
                {
                    _mAudioComponent->Play("jump");
                }
            
        }

        // walking 
        bool walking =
            input.IskeyPressed(GLFW_KEY_W) ||
            input.IskeyPressed(GLFW_KEY_A) ||
            input.IskeyPressed(GLFW_KEY_S) ||
            input.IskeyPressed(GLFW_KEY_D);

        if (walking && _mPlayerControllerComponent && _mPlayerControllerComponent->OnGround())
        {
            if (_mAudioComponent && !_mAudioComponent->IsPlaying("step"))
            {
                _mAudioComponent->Play("step", true);
            }
        }

        else
        {
            if (_mAudioComponent && _mAudioComponent->IsPlaying("step"))
            {
                _mAudioComponent->Stop("step");
            }
        }

    }
}