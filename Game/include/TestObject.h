#pragma once
// CRITICAL ORDER: GLEW FIRST, GLFW SECOND!
#include <GL/glew.h>        // 1. GLEW FIRST!
#include <GLFW/glfw3.h>     // 2. GLFW SECOND!

#include "Engine/eng.h"


class TestObject : public GAMEDEV_ENGINE::GameObject
{
    public:
        TestObject();
        ~TestObject() override;

        void Update(float deltaTime) override; // overriding the update method
    private:

        /*
        // we create a unique pointer to Mesh be  cause mesh is heavy object and we dont want to copy0 it
        float _mOffsetX = 0.0f;
        float _mOffsetY = 0.0f;
        after adding transformation matrix we dont need offset anymore
        we can use matrixModel to handle position offset
        */
        // to track the time
        float _mTime = 0.0f;
        

};