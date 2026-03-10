// Engine.h
#pragma once
#include <iostream>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Engine/input/InputManager.h"
#include "Engine/graphics/GraphicsAPI.h"
#include "Engine/graphics/Texture.h"
#include "Engine/render/RenderQueue.h"
#include "Engine/scene/Scene.h"
#include "Engine/io/AssetFileSystem.h"
#include "Engine/Physics/PhysicsManager.h"
#include "Engine/audio/AudioManager.h"

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
        // for mouseButtonCallBack
        void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        // for mouseCursor CallBack
        void mouseCursorCallback(GLFWwindow* window, double xPos, double yPos);
        static GraphicsAPI& GetGraphicsAPI();  // ✅ Returns reference

        RenderQueue& GetRenderQueue()  { return _mRenderQueue; } 
        
        // we need to add scene related methods
        Scene* GetCurrentScene() const { return _mCurrentEngineScene.get(); }
        void SetCurrentScene(Scene* scene);

        // adding Getter method for the file system
        AssetFileSystem& GetAssetFileSystem() { return _mAssetFileSystem; }

        // now to get the texture manager
        TextureManager& GetTextureManager() { return _mTextureManager; }

        // now to get the physics manager
        PhysicsManager& GetPhysicsManager() { return _mPhyicsManager;  }

        // now to get the audio manager
        AudioManager& GetAudioManager() { return _mAudioManager; }

        GLFWwindow* GetWindow() const { return _gWindow; }
        private:
        Application* _mApplication;
        std::chrono::high_resolution_clock::time_point _mLastFrameTime;
        GLFWwindow* _gWindow;
        InputManager _mInputManager;
        RenderQueue _mRenderQueue;
        // now we need to add scene pointer
        std::unique_ptr<Scene> _mCurrentEngineScene;
        // need to add FileSystem
        AssetFileSystem  _mAssetFileSystem;
        // for handling TextureManager
        TextureManager _mTextureManager;
        // for handling PhysicsManager
        PhysicsManager _mPhyicsManager;
        // for handlling audioManager
        AudioManager _mAudioManager;
    };
}