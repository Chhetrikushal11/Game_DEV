#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Engine/source/Engine.h"
#include "Engine/source/Application.h" // <--- CRITICAL: Must be the full header

namespace GAMEDEV_ENGINE 
{
    static GLFWwindow* _gWindow = nullptr;
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

    bool Engine::Init(int width, int height, const char* title)
    {
        // Compiler now knows what ->Init() is because we included Application.h
        if(!_mApplication)
        {
            return false;
        } 
           if (!glfwInit()) {
        std::cout << "GLFW Init failed!" << std::endl;
        return false;
    }
    std::cout << "GLFW initialized successfully" << std::endl;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    std::cout << "Creating window..." << std::endl;
    _gWindow= glfwCreateWindow(width, height, title, nullptr, nullptr);
    
    if (_gWindow == nullptr)
    {
        const char* description;
        int code = glfwGetError(&description);
        std::cout << "Error creating window!" << std::endl;
        std::cout << "Error code: " << code << std::endl;
        if (description) {
            std::cout << "Description: " << description << std::endl;
        }
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(_gWindow);
    glewExperimental = GL_TRUE;
    glewInit();
    glGetError();
    std::cout << "GLEW initialized successfully" << std::endl;
    std::cout<<"OPENGL VERSION: "<<glGetString(GL_VERSION)<<std::endl;
    std::cout << "Window created successfully" << std::endl;

    return _mApplication->Init();
        
    }

    void Engine::Run()
    {
        if(!_mApplication)
        {
            std::cerr << "Application not set!" << std::endl;
            return;
        }
        _mLastFrameTime = std::chrono::high_resolution_clock::now();
        while(!glfwWindowShouldClose(_gWindow) && !_mApplication->NeedsToClosed())
        {
        auto currentFrameTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentFrameTime - _mLastFrameTime;
        _mLastFrameTime = currentFrameTime;
        
        // Clear screen
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        _mApplication->Update(deltaTime.count());
        
        glfwSwapBuffers(_gWindow);
        glfwPollEvents();
    }
    
    }

    void Engine::Destroy()
    {
        if(_mApplication)
        {
            _mApplication->Destroy();
            _mApplication = nullptr; // Reset raw pointer
            glfwTerminate();
            _gWindow = nullptr;
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