#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
namespace GAMEDEV_ENGINE
{
	/*
	*	This file store reuseable data struct for scene.
	*/

    //------------------------ CREATING STURCT DATA for CAMERA ------------------------------
    struct CameraData
    {
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projectionMatrix = glm::mat4(1.0f);
    };

    //------------------------ CREATING STURCT DATA for LIGHT  ------------------------------
    struct LightData
    {
        // we need color and position
        glm::vec3 color;
        glm::vec3 position;
    };
}