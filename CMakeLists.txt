cmake_minimum_required(VERSION 3.20)
project(GameDevelopmentProject VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform Detection
if(WIN32)
    message(STATUS "Building for Windows")
elseif(APPLE)
    message(STATUS "Building for macOS")
elseif(UNIX)
    message(STATUS "Building for Linux")
endif()

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLFW3
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# Find GLM - simple approach
find_path(GLM_INCLUDE_DIR glm/glm.hpp PATHS /usr/include /usr/local/include)
if(GLM_INCLUDE_DIR)
    add_library(glm INTERFACE)
    target_include_directories(glm INTERFACE ${GLM_INCLUDE_DIR})
    add_library(glm::glm ALIAS glm)
    message(STATUS "Found GLM at: ${GLM_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "GLM not found")
endif()

# Build GLAD from source
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# Build GLEW from source
file(GLOB GLEW_SOURCES external/glew/src/glew.c)
add_library(GLEW STATIC ${GLEW_SOURCES})
target_include_directories(GLEW PUBLIC external/glew/include)
target_compile_definitions(GLEW PUBLIC GLEW_STATIC)
target_link_libraries(GLEW PUBLIC OpenGL::GL)

# Add STB
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE external/stb)

# Add project components
add_subdirectory(Engine)
add_subdirectory(Game)
