# Collect all source files
file(GLOB_RECURSE ENGINE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# ✅ ADD: Collect all header files
file(GLOB_RECURSE ENGINE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Create Engine library with BOTH sources and headers
add_library(Engine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})

# Include directories
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link libraries - handle both GLFW find methods
if(TARGET glfw)
    target_link_libraries(Engine PUBLIC glfw)
elseif(TARGET glfw3)
    target_link_libraries(Engine PUBLIC glfw3)
else()
    target_include_directories(Engine PUBLIC ${GLFW_INCLUDE_DIRS})
    target_link_libraries(Engine PUBLIC ${GLFW_LIBRARIES})
endif()

# Link other libraries
target_link_libraries(Engine
    PUBLIC
        OpenGL::GL
        GLEW::GLEW
        glm::glm
        stb
        json
        cgltf
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

# ✅ ADD: Organize headers in Visual Studio filters
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${ENGINE_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${ENGINE_SOURCES})

# ✅ Generate config.h from template
configure_file(
    ${CMAKE_SOURCE_DIR}/config.h.in     
    ${PROJECT_BINARY_DIR}/config.h
)

# ✅ Include build directory (so config.h can be found)
target_include_directories(Engine PRIVATE ${PROJECT_BINARY_DIR})

# ✅ Copy assets to build folder after building
add_custom_command(TARGET Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASSETS_DIR}
    ${CMAKE_BINARY_DIR}/assets
    COMMENT "Copying assets to build directory..."
)