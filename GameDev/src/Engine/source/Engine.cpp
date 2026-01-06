#include "Engine/source/Engine.h"
#include "Engine/source/Application.h" // <--- CRITICAL: Must be the full header

namespace GAMEDEV_ENGINE 
{
    Engine::Engine():
        _mApplication(nullptr),
        _mLastFrameTime(std::chrono::high_resolution_clock::now())
    {
        std::cout << "Engine created" << std::endl;
    }

    Engine::~Engine()
    {
        std::cout << "Engine destroyed" << std::endl;
    }

    bool Engine::Init()
    {
        // Compiler now knows what ->Init() is because we included Application.h
        if(_mApplication)
        {
            return _mApplication->Init();
        }
        return false;
    }

    void Engine::Run()
    {
        if(!_mApplication)
        {
            std::cerr << "Application not set!" << std::endl;
            return;
        }

        while(!_mApplication->NeedsToClosed())
        {
            auto currentFrameTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> deltaTime = currentFrameTime - _mLastFrameTime;
            _mLastFrameTime = currentFrameTime;

            _mApplication->Update(deltaTime.count());
        }
    }

    void Engine::Destroy()
    {
        if(_mApplication)
        {
            _mApplication->Destroy();
            _mApplication = nullptr; // Reset raw pointer
        }
    }

    void Engine::SetApplication(Application* app)
    {
        _mApplication = app;
    }

    Application* Engine::GetApplication() const
    {
        return _mApplication;
    }
}