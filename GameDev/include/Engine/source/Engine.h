#pragma once
#include <iostream>
#include <chrono>

struct GLFWwindow;
namespace GAMEDEV_ENGINE 
{
    // Forward declaration: Tells the compiler "Application" exists 
    // without needing to #include "Application.h" here.
    class Application; 

    class Engine
    {
    public:
        Engine();
        ~Engine();

        bool Init(int width, int height, const char* title);
        virtual  void Run();
        virtual void Destroy();

        // FIX: Removed "Engine::" from the member declaration
        void SetApplication(Application* app);
        
        Application* GetApplication() const ;

    private:
        Application* _mApplication; // Raw pointer
        std::chrono::high_resolution_clock::time_point _mLastFrameTime;
        
    };
}