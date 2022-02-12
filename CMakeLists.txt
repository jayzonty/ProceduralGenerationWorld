cmake_minimum_required(VERSION 3.10)

# Project name
project(ProceduralGenerationWorld)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Generate compile_commands.json for YouCompleteMe (YCM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify include directories
include_directories(Deps/Include ${GLFW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} Headers)

# Set SOURCES to contain all the source files
set(SOURCES
    # GLAD
    Deps/Source/glad/glad.c

    Source/Engine/ECS/Systems/MouseButtonEventSystem.cpp
    Source/Engine/ECS/Systems/DragEventSystem.cpp
    Source/Engine/ECS/Systems/LineRendererSystem.cpp
    Source/Engine/ECS/Systems/ShapeRendererSystem.cpp
    Source/Engine/ECS/Systems/TextRendererSystem.cpp

    Source/Engine/Font/Font.cpp
    Source/Engine/WindowManager.cpp

    Source/Engine/Graphics/Renderer.cpp
    Source/Engine/Graphics/ShaderProgram.cpp
    
    Source/Engine/UI/Button.cpp
    Source/Engine/UI/ValueSpinner.cpp
    Source/Engine/UI/Spinner.cpp
    Source/Engine/UI/Text.cpp

    Source/Engine/BaseApplication.cpp
    Source/Engine/Clock.cpp
    Source/Engine/Input.cpp
    Source/Engine/Random.cpp
    Source/Engine/SceneGraph.cpp
    Source/Engine/TextureManager.cpp
    Source/Engine/Window.cpp

    Source/EntityTemplates/BlockTemplate.cpp
    Source/EntityTemplates/BlockTemplateManager.cpp

    Source/Application.cpp
    Source/Block.cpp
    Source/Camera.cpp
    Source/Chunk.cpp
    Source/MainScene.cpp
    Source/Mesh.cpp
    Source/MeshBuilder.cpp
    Source/ResourceManager.cpp
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
