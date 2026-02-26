// include/Engine/eng.h
#pragma once

// CRITICAL ORDER: GLEW FIRST, GLFW SECOND!
#include <GL/glew.h>        // 1. GLEW FIRST!
#include <GLFW/glfw3.h>     // 2. GLFW SECOND!

// Now include engine headers
#include "Engine/Engine.h"
#include "Engine/Application.h"
#include "Engine/input/InputManager.h"
#include "Engine/graphics/ShaderProgram.h"
#include "Engine/graphics/GraphicsAPI.h"
#include "Engine/graphics/Texture.h"
#include "Engine/render/material.h"
#include "Engine/render/mesh.h"
#include "Engine/render/RenderQueue.h"
#include "Engine/scene/Scene.h"
#include "Engine/scene/GameObject.h"
#include "Engine/scene/Component.h"
#include "Engine/scene/Component/MeshComponent.h"
#include "Engine/scene/Component/CameraComponent.h"
#include "Engine/scene/Component/PlayerControllerComponent.h"
#include "Engine/scene/Component/AnimationComponent.h"
#include "Engine/Physics/PhysicsManager.h"
#include "io/AssetFileSystem.h"
