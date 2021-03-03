#pragma once

#include "Camera.hpp"
#include "Font.hpp"
#include "SceneBase.hpp"
#include "ShaderProgram.hpp"
#include "Text.hpp"
#include "Texture.hpp"
#include "World.hpp"

#include <glm/glm.hpp>

/// <summary>
/// Main scene class
/// </summary>
class MainScene : public SceneBase
{
private:
	/// <summary>
	/// Font used to display debug info
	/// </summary>
	Font m_debugInfoTextFont;

	/// <summary>
	/// Text display for debug text
	/// </summary>
	Text m_debugInfoTextDisplay;

	// TODO: Make the crosshair an actual sprite
	/// <summary>
	/// Text display for the crosshair
	/// </summary>
	Text m_crossHairTextDisplay;

	/// <summary>
	/// Shader used to render the scene
	/// </summary>
	ShaderProgram m_sceneShaderProgram;

	/// <summary>
	/// Texture atlas containing textures for each block
	/// </summary>
	Texture m_blocksTexture;

	/// <summary>
	/// Camera
	/// </summary>
	Camera m_camera;

	/// <summary>
	/// World data
	/// </summary>
	World* m_world;

	/// <summary>
	/// Chunk indices of the chunk we were previously in
	/// </summary>
	glm::ivec3 m_prevChunkIndices;

	/// <summary>
	/// Number of chunks to render in each axis that is visible
	/// at a time
	/// </summary>
	int m_chunkRenderDistance;

	/// <summary>
	/// Sky color
	/// </summary>
	glm::vec4 m_skyColor;

public:
	/// <summary>
	/// Constructor
	/// </summary>
	MainScene();

	/// <summary>
	/// Destructor
	/// </summary>
	~MainScene();

	/// <summary>
	/// Start scene
	/// </summary>
	void Start() override;

	/// <summary>
	/// Finish scene
	/// </summary>
	void Finish() override;

	/// <summary>
	/// Updates scene state
	/// </summary>
	/// <param name="deltaTime"></param>
	void Update(float deltaTime) override;

	/// <summary>
	/// Draws the scene
	/// </summary>
	void Draw() override;
};
