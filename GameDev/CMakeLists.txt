cmake_minimum_required(VERSION 3.20)
project(GameDevelopmentProject VERSION 1.0.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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
# Local Libraries
# ============================================
# GLAD
add_library(glad STATIC "${CMAKE_SOURCE_DIR}/external/glad/src/glad.c")
target_include_directories(glad PUBLIC "${CMAKE_SOURCE_DIR}/external/glad/include")

# GLEW
add_library(glew STATIC "${CMAKE_SOURCE_DIR}/external/glew/src/glew.c")
target_include_directories(glew PUBLIC "${CMAKE_SOURCE_DIR}/external/glew/include")
target_compile_definitions(glew PUBLIC GLEW_STATIC)

# ============================================
# Executable Setup
# ============================================
add_executable(GameDevExecutable "src/main.cpp")
target_link_libraries(GameDevExecutable
    PRIVATE
        glad
        glew
        glfw
        glm::glm
        opengl32
)
target_include_directories(GameDevExecutable PRIVATE "${CMAKE_SOURCE_DIR}/include")