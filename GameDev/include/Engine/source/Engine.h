// Engine.h
#pragma once
#include <iostream>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Engine/source/input/InputManager.h"
#include "Engine/source/graphics/GraphicsAPI.h"
#include "Engine/source/render/RenderQueue.h"

namespace GAMEDEV_ENGINE 
{
    class Application; 

    class Engine
    {
    private:
        static Engine* _sInstance;
        static GraphicsAPI* _sGraphicsAPI;  // ✅ Static POINTER (not reference)
        
        Engine();
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;
        
    public:
        ~Engine();
        
        static Engine& GetInstance();
        
        bool Init(int width, int height, const char* title);
        void Run();
        void Destroy();
        
        void SetApplication(Application* app);
        Application* GetApplication() const;
        
        InputManager& GetInputManager();
        void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        
        static GraphicsAPI& GetGraphicsAPI();  // ✅ Returns reference

        RenderQueue& GetRenderQueue()  { return _mRenderQueue; } 
        
    private:
        Application* _mApplication;
        std::chrono::high_resolution_clock::time_point _mLastFrameTime;
        GLFWwindow* _gWindow;
        InputManager _mInputManager;
        RenderQueue _mRenderQueue;
    };
}