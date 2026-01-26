
#include "Engine/source/Game.h"
#include "Engine/source/Application.h"
#include "Engine/source/graphics/GraphicsAPI.h"
#include "Engine/source/render/RenderQueue.h"
#include "Engine/source/TestObject.h"

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
    //     std::cout << "Game Initialized" << std::endl;  // ✅ Success message
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