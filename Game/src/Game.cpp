
#include "Game.h"
#include "Engine/Application.h"
#include "Engine/graphics/GraphicsAPI.h"
#include "Engine/render/RenderQueue.h"
#include "TestObject.h"

 namespace GAMEDEV_ENGINE 
 {
bool Game::Init()
{

    // if(!shaderProgram)  // If shader creation FAILED (nullptr)
    // {
    //     std::cerr << "Failed to create shader program" << std::endl;  // ✅ Error message
    //     return false;  // ✅ Return false (failure)
    // }
    // else  // If shader creation SUCCEEDED
    // {
    //     return true;  // ✅ Return true (success)
    // }
    _mScene.CreateGameObject<TestObject>("TestObject1");
    std::cout << "Game Initialized" << std::endl;
    if(!_mScene.CreateGameObject<TestObject>("TestObject1"))
    {
        std::cerr << "Failed to create TestObject1" << std::endl;
        return false;
    }
    else
    {
        std::cout << "TestObject1 created successfully" << std::endl;
        return true;

    }
}

    void Game::Update(float deltaTime)
    {
       // std::cout <<"Game Updated with deltaTime: "<< deltaTime << std::endl;
        _mScene.Update(deltaTime);
    }

    void Game::Destroy()
    {
        std::cout <<"Game Destroyed"<< std::endl;
    }
 }