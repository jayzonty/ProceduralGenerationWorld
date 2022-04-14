cmake_minimum_required(VERSION 3.10)

# Project name
project(ProceduralGenerationWorld)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Generate compile_commands.json for YouCompleteMe (YCM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify include directories
include_directories(Deps/Include ${Vulkan_INCLUDE_DIR} ${GLFW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} Headers)

# Set SOURCES to contain all the source files
set(SOURCES
    #Source/EntityTemplates/BlockTemplate.cpp
    #Source/EntityTemplates/BlockTemplateManager.cpp

    #Source/Scenes/MainScene.cpp
    Source/Scenes/SandboxScene.cpp

    Source/Utils/FileUtils.cpp
    #Source/Utils/MathUtils.cpp
    #Source/Utils/NoiseUtils.cpp

    Source/Vulkan/VulkanBuffer.cpp
    Source/Vulkan/VulkanContext.cpp
    Source/Vulkan/VulkanImage.cpp
    Source/Vulkan/VulkanImageView.cpp

    Source/Application.cpp
    Source/BaseApplication.cpp
    #Source/BatchRenderer.cpp
    #Source/Block.cpp
    #Source/BlockUtils.cpp
    Source/Camera.cpp
    #Source/Chunk.cpp
    #Source/Framebuffer.cpp
    #Source/Image.cpp
    Source/Input.cpp
    #Source/Mesh.cpp
    #Source/ResourceManager.cpp
    Source/SceneManager.cpp
    #Source/ShaderProgram.cpp
    #Source/Texture.cpp
    #Source/UIRenderer.cpp
    Source/Window.cpp
    Source/WindowManager.cpp
    #Source/World.cpp

    Source/Main.cpp
)

# Executable
add_executable(ProceduralGenerationWorld ${SOURCES})

# Link libraries
#target_link_libraries(ProceduralGenerationWorld ${OPENGL_gl_LIBRARY} ${FREETYPE_LIBRARIES} glfw ${CMAKE_DL_LIBS})
target_link_libraries(ProceduralGenerationWorld ${Vulkan_LIBRARY} ${FREETYPE_LIBRARIES} glfw ${CMAKE_DL_LIBS})

# Post-build copy command
add_custom_command(TARGET ProceduralGenerationWorld POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources/
    $<TARGET_FILE_DIR:ProceduralGenerationWorld>/Resources/
)
# Post-build copy command
add_custom_command(TARGET ProceduralGenerationWorld POST_BUILD
    COMMAND glslangValidator -S vert -e main -o ${CMAKE_SOURCE_DIR}/Resources/Shaders/basic_vert.spv -V ${CMAKE_SOURCE_DIR}/Resources/Shaders/basic_vert.glsl
    COMMAND glslangValidator -S frag -e main -o ${CMAKE_SOURCE_DIR}/Resources/Shaders/basic_frag.spv -V ${CMAKE_SOURCE_DIR}/Resources/Shaders/basic_frag.glsl
    #COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:VulkanModelViewer>/resources/
)
