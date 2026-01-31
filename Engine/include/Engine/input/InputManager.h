#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
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


        // to get the Method to get information of mouse activity
        void SetMouseButtonPressed(int button, bool pressed);

        bool IsMouseButtonPressed(int button) const;

        void SetMousePositionOld(const glm::vec2& pos);
        const glm::vec2& GetMousePositionOld() const { return _mMousePositionOld; }

        void SetMousePositionNew(const glm::vec2& pos);
        const glm::vec2& GetMousePositionCurrent() const {return _mMousePositionNew;}
    private:
        std::array<bool, 256> _mKeys = { false }; // assuming 256 keys
        // an extra array to hold the states of mouse speciifically
        std::array<bool, 16>_mMouseKeys = { false };
        // now we need two variables to hold current and previous frame position value
        glm::vec2 _mMousePositionOld = glm::vec2(0.0f);
        glm::vec2 _mMousePositionNew = glm::vec2(0.0f);
        /*glm::vec2 _mMousePositionNew = glm::vec2(0.0f);*/

        friend class Engine; // only Engine class can create and destroy InputManager
      
    };
}