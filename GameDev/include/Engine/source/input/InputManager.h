#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <array>

namespace GAMEDEV_ENGINE 
{
    class InputManager
    {
        // this class will only created and owned by Engine class and will not be exposed to Application or any other class.
    private:
        InputManager() = default;
        InputManager(const InputManager&) = delete; // we dont want copy constructor
        InputManager& operator=(const InputManager&) = delete; // we dont want copy assignment operator
        InputManager(InputManager&&) = delete; // we dont want move constructor
        InputManager& operator=(InputManager&&) = delete; // we dont want move assignment operator
        ~InputManager() = default;
    public:
        // add the array to hold the key states
        void SetKeyPressed(int key, bool pressed);

        bool IskeyPressed(int key) const;


    private:
        std::array<bool, 256> _mKeys = { false }; // assuming 256 keys
        friend class Engine; // only Engine class can create and destroy InputManager
      
    };
}