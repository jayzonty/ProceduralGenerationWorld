cmake_minimum_required(VERSION 3.10)

# Project name
project(ProceduralGenerationWorld)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)

# C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Generate compile_commands.json for YouCompleteMe (YCM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify include directories
include_directories(Deps/Include ${GLFW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} Headers)

# Set SOURCES to contain all the source files
set(SOURCES
    # GLAD
    Deps/Source/glad/glad.c

    Source/EntityTemplates/BlockTemplate.cpp
    Source/EntityTemplates/BlockTemplateManager.cpp

    Source/Application.cpp
    Source/BaseApplication.cpp
    Source/Block.cpp
    Source/Camera.cpp
    Source/Chunk.cpp
    Source/Input.cpp
    Source/MainScene.cpp
    Source/Mesh.cpp
    Source/MeshBuilder.cpp
    Source/ResourceManager.cpp
    Source/ShaderProgram.cpp
    Source/Texture.cpp
    Source/Window.cpp
    Source/WindowManager.cpp
    Source/World.cpp

    Source/Main.cpp
)

# Executable
add_executable(ProceduralGenerationWorld ${SOURCES})

# Link libraries
#target_link_libraries(ProceduralGenerationWorld ${OPENGL_gl_LIBRARY} ${FREETYPE_LIBRARIES} glfw ${CMAKE_DL_LIBS})
target_link_libraries(ProceduralGenerationWorld ${OPENGL_gl_LIBRARY} ${FREETYPE_LIBRARIES} glfw ${CMAKE_DL_LIBS})

# Post-build copy command
add_custom_command(TARGET ProceduralGenerationWorld POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources/
    $<TARGET_FILE_DIR:ProceduralGenerationWorld>/Resources/
)
