# Collect all engine source files
file(GLOB_RECURSE ENGINE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create Engine library
add_library(Engine STATIC ${ENGINE_SOURCES})

# Include directories
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link libraries - handle both GLFW find methods
if(TARGET glfw)
    # Modern CMake target (Windows/macOS via vcpkg/brew)
    target_link_libraries(Engine PUBLIC glfw)
elseif(TARGET glfw3)
    target_link_libraries(Engine PUBLIC glfw3)
else()
    # pkg-config fallback (Linux)
    target_include_directories(Engine PUBLIC ${GLFW_INCLUDE_DIRS})
    target_link_libraries(Engine PUBLIC ${GLFW_LIBRARIES})
endif()

# Link other libraries
target_link_libraries(Engine
    PUBLIC
        OpenGL::GL
        GLEW::GLEW
        glm::glm
)

# Windows-specific settings
if(WIN32)
    target_compile_definitions(Engine PRIVATE 
        _CRT_SECURE_NO_WARNINGS
        NOMINMAX
    )
endif()

# macOS-specific frameworks
if(APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)
    target_link_libraries(Engine PUBLIC
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
endif()
