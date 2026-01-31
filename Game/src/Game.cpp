
#include "Game.h"
#include "Engine/Application.h"
#include "Engine/graphics/GraphicsAPI.h"
#include "Engine/render/RenderQueue.h"
#include "Engine/scene/Component/CameraComponent.h"
#include "Engine/scene/Component/PlayerControllerComponent.h"
#include "TestObject.h"

 namespace GAMEDEV_ENGINE 
 {
bool Game::Init()
{   // now need to intantiate the _mScene
    _mScene = new Scene();
    // create a camera
    auto camera = _mScene->CreateGameObject("Camera");
    camera->AddComponent(new CameraComponent());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    camera->AddComponent(new PlayerControllerComponent());
    // need to set main camera for the secene
    _mScene->SetMainCameraGameObject(camera);
    // if(!shaderProgram)  // If shader creation FAILED (nullptr)
    // {
    //     std::cerr << "Failed to create shader program" << std::endl;  // ✅ Error message
    //     return false;  // ✅ Return false (failure)
    // }
    // else  // If shader creation SUCCEEDED
    // {
    //     return true;  // ✅ Return true (success)
    // }
    if (!_mScene->CreateGameObject<TestObject>("TestObject1"))
    {
        std::cerr << "Failed to create TestObject1" << std::endl;
        return false;
    }

    std::cout << "TestObject1 created successfully" << std::endl;

    // Tell Engine about the scene
    Engine::GetInstance().SetCurrentScene(_mScene);

    std::cout << "Game Initialized" << std::endl;
    return true;
}


    void Game::Update(float deltaTime)
    {
       // std::cout <<"Game Updated with deltaTime: "<< deltaTime << std::endl;
        _mScene->Update(deltaTime);
    }

    void Game::Destroy()
    {
        std::cout <<"Game Destroyed"<< std::endl;
    }
 }