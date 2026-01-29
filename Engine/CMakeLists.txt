# Collect all engine source files
file(GLOB_RECURSE ENGINE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create Engine library
add_library(Engine STATIC ${ENGINE_SOURCES})

# Include directories
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(Engine
    PUBLIC
        OpenGL::GL
        GLEW
        ${GLFW_LIBRARIES}
        glm::glm
)

# Add GLFW include directories
target_include_directories(Engine PUBLIC ${GLFW_INCLUDE_DIRS})
