cmake_minimum_required(VERSION 3.20)
project(GameDevelopmentProject)

set(GAME_DEVELOPMENT_PROJECT GameDevExecutable)

add_executable(${GAME_DEVELOPMENT_PROJECT}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

include(FetchContent)

# GLFW
set(GLFW_LIB_NAME glfw)
FetchContent_Declare(
    ${GLFW_LIB_NAME}
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.10
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(${GLFW_LIB_NAME})

# GLEW - Use glew-cmake which is FetchContent-friendly
set(glew-cmake-BUILD_SHARED ON CACHE BOOL "Build shared GLEW library")
set(glew-cmake-BUILD_STATIC OFF CACHE BOOL "Don't build static GLEW library")

FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
    GIT_TAG glew-cmake-2.2.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glew)

# Link libraries
target_link_libraries(${GAME_DEVELOPMENT_PROJECT} PUBLIC ${GLFW_LIB_NAME} libglew_shared)

# Function to copy DLL after build
function(CopyLibToTarget LIB_NAME TARGET_NAME)
    add_custom_command(TARGET ${TARGET_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:${LIB_NAME}>
        $<TARGET_FILE_DIR:${TARGET_NAME}>
    )
endfunction()

# Copy DLLs (Windows only)
if(WIN32)
    CopyLibToTarget(${GLFW_LIB_NAME} ${GAME_DEVELOPMENT_PROJECT})
    CopyLibToTarget(libglew_shared ${GAME_DEVELOPMENT_PROJECT})
endif()