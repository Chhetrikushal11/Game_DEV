// include/Engine/source/eng.h
#pragma once

// CRITICAL ORDER: GLEW FIRST, GLFW SECOND!
#include <GL/glew.h>        // 1. GLEW FIRST!
#include <GLFW/glfw3.h>     // 2. GLFW SECOND!

// Now include engine headers
#include "Engine/source/Engine.h"
#include "Engine/source/Application.h"
#include "Engine/source/input/InputManager.h"
#include "Engine/source/graphics/ShaderProgram.h"
#include "Engine/source/graphics/GraphicsAPI.h"