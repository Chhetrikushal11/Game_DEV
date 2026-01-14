#include "Engine/source/Application.h"
#include <iostream>

namespace GAMEDEV_ENGINE
{
    Application::Application() : _mNeedsToClose(false)
    {
        std::cout << "Application created" << std::endl;
    }

    Application::~Application()
    {
        std::cout << "Application destroyed" << std::endl;
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