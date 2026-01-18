#include "Engine/source/Engine.h"
#include "Engine/source/Application.h"

namespace GAMEDEV_ENGINE 
{
    // ============================================
    // DEFINE STATIC MEMBERS
    // ============================================
    Engine* Engine::_sInstance = nullptr;
    GraphicsAPI* Engine::_sGraphicsAPI = new GraphicsAPI();  // ✅ Create on heap
    
    // ============================================
    // Constructor
    // ============================================
    Engine::Engine() :
        _mApplication(nullptr),
        _mLastFrameTime(std::chrono::high_resolution_clock::now()),
        _gWindow(nullptr),
        _mInputManager()
        // ❌ Remove _mGraphicsAPI initialization - it's static now!
    {
        _sInstance = this;
        std::cout << "Engine created" << std::endl;
    }

    // ============================================
    // Destructor
    // ============================================
    Engine::~Engine()
    {
        std::cout << "Engine destroyed" << std::endl;
        
        // Clean up static GraphicsAPI when last Engine is destroyed
        if (_sGraphicsAPI) {
            delete _sGraphicsAPI;
            _sGraphicsAPI = nullptr;
        }
    }

    // ============================================
    // Singleton Getter
    // ============================================
    Engine& Engine::GetInstance()
    {
        static Engine instance;
        return instance;
    }

    // ============================================
    // GraphicsAPI Getter
    // ============================================
    GraphicsAPI& Engine::GetGraphicsAPI()
    {
        return *_sGraphicsAPI;  // ✅ Dereference pointer to return reference
    }

    // ============================================
    // Init
    // ============================================
bool Engine::Init(int width, int height, const char* title)
{
    if(!_mApplication)
    {
        std::cerr << "No application set!" << std::endl;
        return false;
    }

    if (!glfwInit()) {
        std::cerr << "GLFW Init failed!" << std::endl;
        return false;
    }
    std::cout << "GLFW initialized successfully" << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::cout << "Creating window..." << std::endl;
    _gWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
    
    if (_gWindow == nullptr)
    {
        const char* description;
        int code = glfwGetError(&description);
        std::cerr << "Error creating window!" << std::endl;
        std::cerr << "Error code: " << code << std::endl;
        if (description) {
            std::cerr << "Description: " << description << std::endl;
        }
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(_gWindow);
    
    glewExperimental = GL_TRUE;
    glewInit();
    glGetError();
    
    std::cout << "GLEW initialized successfully" << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Window created successfully" << std::endl;

    // GraphicsAPI doesn't need Init() - it's ready to use!
    
    glfwSetKeyCallback(_gWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Engine::GetInstance().keyCallback(window, key, scancode, action, mods);
    });

    return _mApplication->Init();
}

    // ============================================
    // Run
    // ============================================
void Engine::Run()
{
    if(!_mApplication)
    {
        std::cerr << "Application not set!" << std::endl;
        return;
    }

    if (!_gWindow)
    {
        std::cerr << "Window not created!" << std::endl;
        return;
    }
    
    std::cout << "Starting game loop..." << std::endl;
    _mLastFrameTime = std::chrono::high_resolution_clock::now();
    
    while(!glfwWindowShouldClose(_gWindow) && !_mApplication->NeedsToClosed())
    {
        glfwPollEvents();
        
        auto currentFrameTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentFrameTime - _mLastFrameTime;
        _mLastFrameTime = currentFrameTime;
        
        // ✅ 1. CLEAR FIRST (at the beginning)
        _sGraphicsAPI->SetClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        _sGraphicsAPI->ClearBuffers();
        
        // ✅ 2. UPDATE (submits render commands)
        _mApplication->Update(deltaTime.count());
        
        // ✅ 3. DRAW (renders submitted commands)
        _mRenderQueue.Draw(*_sGraphicsAPI);
        
        // ✅ 4. SWAP BUFFERS (show result)
        glfwSwapBuffers(_gWindow);
    }
    
    std::cout << "Game loop ended" << std::endl;
}

    // ============================================
    // Destroy
    // ============================================
    void Engine::Destroy()
    {
        if(_mApplication)
        {
            _mApplication->Destroy();
            _mApplication = nullptr;
        }

        if (_gWindow)
        {
            glfwDestroyWindow(_gWindow);
            _gWindow = nullptr;
        }

        glfwTerminate();
    }

    // ============================================
    // SetApplication
    // ============================================
    void Engine::SetApplication(Application* app)
    {
        _mApplication = app;
    }

    // ============================================
    // GetApplication
    // ============================================
    Application* Engine::GetApplication() const
    {
        return _mApplication;
    }

    // ============================================
    // GetInputManager
    // ============================================
    InputManager& Engine::GetInputManager()
    {
        return _mInputManager;
    }

    // ============================================
    // keyCallback
    // ============================================
    void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if(action == GLFW_PRESS)
        {
            _mInputManager.SetKeyPressed(key, true);
        }
        else if(action == GLFW_RELEASE)
        {
            _mInputManager.SetKeyPressed(key, false);
        }
    }

}