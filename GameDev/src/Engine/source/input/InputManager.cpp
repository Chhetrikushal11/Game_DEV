#include "Engine/source/input/InputManager.h"

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
}

