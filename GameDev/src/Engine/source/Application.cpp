#include "Engine/source/Application.h"

 namespace GAMEDEV_ENGINE 
 {
    Application::Application()
    : _mNeedsToClose(false)
    {
    }

    Application::~Application()
    {
    }
    bool Application::Init()
    {
        return true;
    }

    void Application::Update(float deltaTime)
    {
    }

    void Application::Destroy()
    {
    }

    void Application::SetNeedsToClosed(bool needsToClose)
    {
        _mNeedsToClose = needsToClose;
    }

    bool Application::NeedsToClosed() const
    {
        return _mNeedsToClose;
    }


 }