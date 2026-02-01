# Collect all source files
file(GLOB_RECURSE GAME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# ✅ ADD: Collect all header files
file(GLOB_RECURSE GAME_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Create Game executable with BOTH sources and headers
add_executable(Game ${GAME_SOURCES} ${GAME_HEADERS})

# Include directories
target_include_directories(Game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link to Engine (which brings in all Engine dependencies)
target_link_libraries(Game PRIVATE Engine)

# Set output directory
set_target_properties(Game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# ✅ ADD: Organize headers in Visual Studio filters
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${GAME_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${GAME_SOURCES})