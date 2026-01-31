#include "Engine/input/InputManager.h"

namespace GAMEDEV_ENGINE
{
    void InputManager::SetKeyPressed(int key, bool pressed)
    {
        if(key < 0 || key >= static_cast<int>(_mKeys.size()))
        {
            std::cerr << "Key index out of bounds: " << key << std::endl;
            return;
        }
        _mKeys[key] = pressed;

    }

    bool InputManager::IskeyPressed(int key) const
    {
        if(key < 0 || key >= static_cast<int>(_mKeys.size()))
        {
            std::cerr << "Key index out of bounds: " << key << std::endl;
            return false;
        }
        return _mKeys[key];
    }

    void InputManager::SetMouseButtonPressed(int button, bool pressed)
    {
        if (button < 0 || button >= static_cast<int>(_mMouseKeys.size()))
        {
            std::cerr << "Key index out of bounds: " << button << std::endl;
            return;
        }
        _mMouseKeys[button] = pressed;

    }

    bool InputManager::IsMouseButtonPressed(int button) const
    {
        if (button < 0 || button >= static_cast<int>(_mMouseKeys.size()))
        {
            std::cerr << "Key index out of bounds: " << button << std::endl;
            return false;
        }
        return _mMouseKeys[button];
    }

    void InputManager::SetMousePositionNew(const glm::vec2& pos)
    {
        _mMousePositionNew = pos;
    }

    void InputManager::SetMousePositionOld(const glm::vec2& pos)
    {
        _mMousePositionOld = pos;
    }

   

}

