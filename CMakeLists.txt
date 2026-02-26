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

# Find OpenGL (removed duplicate)
find_package(OpenGL REQUIRED)

# Find GLFW3 (cross-platform)
find_package(glfw3 QUIET CONFIG)
if(NOT glfw3_FOUND)
    if(UNIX AND NOT APPLE)
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(GLFW REQUIRED glfw3)
    else()
        message(FATAL_ERROR "GLFW3 not found. Please install via vcpkg on Windows or brew on macOS")
    endif()
endif()

# Find GLM (cross-platform)
find_package(glm QUIET CONFIG)
if(NOT glm_FOUND)
    find_path(GLM_INCLUDE_DIR glm/glm.hpp
        PATHS /usr/include /usr/local/include
    )
    if(GLM_INCLUDE_DIR)
        add_library(glm INTERFACE)
        target_include_directories(glm INTERFACE ${GLM_INCLUDE_DIR})
        add_library(glm::glm ALIAS glm)
        message(STATUS "Found GLM at: ${GLM_INCLUDE_DIR}")
    else()
        message(FATAL_ERROR "GLM not found")
    endif()
endif()

# Find or build GLEW
find_package(GLEW QUIET CONFIG)
if(NOT GLEW_FOUND)
    message(STATUS "Building GLEW from source")
    file(GLOB GLEW_SOURCES external/glew/src/glew.c)
    add_library(GLEW STATIC ${GLEW_SOURCES})
    target_include_directories(GLEW PUBLIC external/glew/include)
    target_compile_definitions(GLEW PUBLIC GLEW_STATIC)
    target_link_libraries(GLEW PUBLIC OpenGL::GL)
    add_library(GLEW::GLEW ALIAS GLEW)
endif()

# Build GLAD from source
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# Add STB
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE external/stb)

# Add JSON
add_library(json INTERFACE)
target_include_directories(json INTERFACE external/json)

# Add cgltf (fixed typo - was targeting json twice)
add_library(cgltf INTERFACE)
target_include_directories(cgltf INTERFACE external/cgltf)

# ===================================================================
# Bullet Physics - Lean Build Configuration
# ===================================================================
message(STATUS "Configuring Bullet3 Physics...")

# Set Bullet options BEFORE add_subdirectory (critical!)
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET3 OFF CACHE BOOL "" FORCE)
set(USE_MSVC_RUNTIME_LIBRARY_DLL ON CACHE BOOL "" FORCE)
set(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "" FORCE)

# Only build what we need: Core Bullet libraries
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Add Bullet subdirectory
add_subdirectory(external/bullet3-3.25)

# Create a clean interface target that links the essentials
add_library(BulletPhysics INTERFACE)
target_link_libraries(BulletPhysics INTERFACE
    BulletDynamics
    BulletCollision
    LinearMath
)
target_include_directories(BulletPhysics INTERFACE 
    ${CMAKE_SOURCE_DIR}/external/bullet3-3.25/src
)

# Optional: Suppress Bullet warnings if needed
if(MSVC)
    target_compile_options(BulletDynamics PRIVATE /W3)
    target_compile_options(BulletCollision PRIVATE /W3)
    target_compile_options(LinearMath PRIVATE /W3)
else()
    target_compile_options(BulletDynamics PRIVATE -w)
    target_compile_options(BulletCollision PRIVATE -w)
    target_compile_options(LinearMath PRIVATE -w)
endif()

message(STATUS "Bullet3 configured (lean build)")

# Define assets directory
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")

# Add project components
add_subdirectory(Engine)
add_subdirectory(Game)