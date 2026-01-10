cmake_minimum_required(VERSION 3.20)
project(GameDevelopmentProject VERSION 1.0.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Add this line

include(FetchContent)

# ============================================
# Fetch GLFW
# ============================================
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
message(STATUS "Populating GLFW...")
FetchContent_MakeAvailable(glfw)

# ============================================
# Fetch GLM
# ============================================
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    GIT_SHALLOW TRUE
)
message(STATUS "Populating GLM...")
FetchContent_MakeAvailable(glm)

# ============================================
# Find System Libraries
# ============================================
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# ============================================
# Create Engine Library
# ============================================
set(ENGINE_SOURCE_FILES 
    "src/Engine/source/Engine.cpp"
    "src/Engine/source/Application.cpp"
    "src/Engine/source/Game.cpp"
)

set(ENGINE_HEADER_FILES 
    "include/Engine/source/Engine.h"
    "include/Engine/source/Application.h"
    "include/Engine/source/eng.h"
    "include/Engine/source/Game.h"
)

add_library(${PROJECT_NAME}_Engine STATIC 
    ${ENGINE_SOURCE_FILES}
    ${ENGINE_HEADER_FILES}
)

# Link libraries to Engine (CRITICAL FIX)
target_link_libraries(${PROJECT_NAME}_Engine
    PUBLIC
        GLEW::GLEW
        glfw
        glm::glm
        OpenGL::GL
)

target_include_directories(${PROJECT_NAME}_Engine PUBLIC "${CMAKE_SOURCE_DIR}/include")

# ============================================
# Executable Setup
# ============================================
add_executable(GameDevExecutable "src/main.cpp")

target_link_libraries(GameDevExecutable
    PRIVATE
        ${PROJECT_NAME}_Engine
        ${CMAKE_DL_LIBS}
)

target_include_directories(GameDevExecutable PRIVATE "${CMAKE_SOURCE_DIR}/include")