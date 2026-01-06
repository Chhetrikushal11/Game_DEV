#include "Engine/source/Game.h"
#include "Engine/source/Application.h"

 namespace GAMEDEV_ENGINE 
 {
    bool Game::Init()
    {
        std::cout <<"Game Initiliazed"<< std::endl;
        return true;
    }

    void Game::Update(float deltaTime)
    {
        std::cout <<"Current deltaTime: "<< deltaTime << std::endl;
    }

    void Game::Destroy()
    {
        std::cout <<"Game Destroyed"<< std::endl;
    }
 }