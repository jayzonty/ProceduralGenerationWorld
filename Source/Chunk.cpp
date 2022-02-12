#include "Chunk.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "Constants.hpp"
#include "MeshBuilder.hpp"
#include "ResourceManager.hpp"

#include "Enums/BlockTypeEnum.hpp"
#include "EntityTemplates/BlockTemplate.hpp"
#include "EntityTemplates/BlockTemplateManager.hpp"

/**
 * @brief Constructor
 * @param[in] chunkIndexX Chunk x-index
 * @param[in] chunkIndexZ Chunk z-index
 */
Chunk::Chunk(const int& chunkIndexX, const int& chunkIndexZ)
	: m_mesh()
	, m_waterMesh()
	, m_blocks()
	, m_chunkIndex(chunkIndexX, 0, chunkIndexZ)
{
	int size = Constants::CHUNK_WIDTH * Constants::CHUNK_DEPTH * Constants::CHUNK_HEIGHT;
	m_blocks.resize(size);
}

/**
 * @brief Destructor
 */
Chunk::~Chunk()
{
	for (size_t i = 0; i < m_blocks.size(); ++i)
	{
		delete m_blocks[i];
	}
	m_blocks.clear();
}

/**
 * @brief Gets the x chunk index for this chunk
 * @return Chunk x-index
 */
int Chunk::GetChunkIndexX() const
{
	return m_chunkIndex.x;
}

/**
 * @brief Gets the y chunk index for this chunk
 * @return Chunk y-index
 */
int Chunk::GetChunkIndexY() const
{
	return m_chunkIndex.y;
}

/**
 * @brief Gets the z chunk index for this chunk
 * @return Chunk z-index
 */
int Chunk::GetChunkIndexZ() const
{
	return m_chunkIndex.z;
}

/**
 * @brief Gets the chunk indices in each axis for this chunk
 * @return Chunk indices for each axis
 */
glm::ivec3 Chunk::GetChunkIndices() const
{
	return m_chunkIndex;
}

/**
 * @brief Generates the mesh for this chunk
 */
void Chunk::GenerateMesh()
{
	float blockSize = Constants::BLOCK_SIZE;

	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec4> vertexColors;
	std::vector<glm::vec2> vertexUVs;
	std::vector<glm::vec3> vertexNormals;
	std::vector<GLuint> indices;
	glm::vec3 origin(m_chunkIndex.x * Constants::CHUNK_WIDTH * blockSize, 0.0f, m_chunkIndex.z * Constants::CHUNK_DEPTH * blockSize * 1.0f);
	for (int x = 0; x < Constants::CHUNK_WIDTH; ++x)
	{
		for (int z = 0; z < Constants::CHUNK_DEPTH; ++z)
		{
			for (int y = 0; y < Constants::CHUNK_HEIGHT; ++y)
			{
				Block* currentBlock = GetBlockAt(x, y, z);
				if (currentBlock != nullptr && currentBlock->GetBlockType() != BlockTypeEnum::WATER)
				{
					const BlockTemplate* blockTemplate = BlockTemplateManager::GetInstance().GetBlockTemplate(currentBlock->GetBlockType());

					glm::vec4 uvRects[] =
					{
						blockTemplate->GetFaceUVRect(BlockFaceEnum::TOP),
						blockTemplate->GetFaceUVRect(BlockFaceEnum::BOTTOM),
						blockTemplate->GetFaceUVRect(BlockFaceEnum::LEFT),
						blockTemplate->GetFaceUVRect(BlockFaceEnum::RIGHT),
						blockTemplate->GetFaceUVRect(BlockFaceEnum::FRONT),
						blockTemplate->GetFaceUVRect(BlockFaceEnum::BACK),
					};

					// Vertex order: lower-left, lower-right, upper-right, upper-left
					
					// Top face
					if ((y == Constants::CHUNK_HEIGHT - 1) || ((GetBlockAt(x, y + 1, z) == nullptr) || (GetBlockAt(x, y + 1, z)->GetBlockType() == BlockTypeEnum::WATER)))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, z * blockSize));
						
						float color = 0.75f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						vertexUVs.emplace_back(uvRects[0].x, uvRects[0].y);
						vertexUVs.emplace_back(uvRects[0].x + uvRects[0].z, uvRects[0].y);
						vertexUVs.emplace_back(uvRects[0].x + uvRects[0].z, uvRects[0].y + uvRects[0].w);
						vertexUVs.emplace_back(uvRects[0].x, uvRects[0].y + uvRects[0].w);

						vertexNormals.emplace_back(0.0f, 1.0f, 0.0f);
						vertexNormals.emplace_back(0.0f, 1.0f, 0.0f);
						vertexNormals.emplace_back(0.0f, 1.0f, 0.0f);
						vertexNormals.emplace_back(0.0f, 1.0f, 0.0f);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}

					// Bottom face
					if ((y == 0) || (GetBlockAt(x, y - 1, z) == nullptr))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, (z + 1) * blockSize));
						
						float color = 0.35f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						vertexUVs.emplace_back(uvRects[1].x, uvRects[1].y);
						vertexUVs.emplace_back(uvRects[1].x + uvRects[1].z, uvRects[1].y);
						vertexUVs.emplace_back(uvRects[1].x + uvRects[1].z, uvRects[1].y + uvRects[1].w);
						vertexUVs.emplace_back(uvRects[1].x, uvRects[1].y + uvRects[1].w);

						vertexNormals.emplace_back(0.0f, -1.0f, 0.0f);
						vertexNormals.emplace_back(0.0f, -1.0f, 0.0f);
						vertexNormals.emplace_back(0.0f, -1.0f, 0.0f);
						vertexNormals.emplace_back(0.0f, -1.0f, 0.0f);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}
					
					// Left face
					if ((x == Constants::CHUNK_WIDTH - 1) || (GetBlockAt(x + 1, y, z) == nullptr))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, (z + 1) * blockSize));
						
						float color = 0.6f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						vertexUVs.emplace_back(uvRects[2].x, uvRects[2].y);
						vertexUVs.emplace_back(uvRects[2].x + uvRects[2].z, uvRects[2].y);
						vertexUVs.emplace_back(uvRects[2].x + uvRects[2].z, uvRects[2].y + uvRects[2].w);
						vertexUVs.emplace_back(uvRects[2].x, uvRects[2].y + uvRects[2].w);

						vertexNormals.emplace_back(-1.0f, 0.0f, 0.0f);
						vertexNormals.emplace_back(-1.0f, 0.0f, 0.0f);
						vertexNormals.emplace_back(-1.0f, 0.0f, 0.0f);
						vertexNormals.emplace_back(-1.0f, 0.0f, 0.0f);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}

					// Right face
					if ((x == 0) || (GetBlockAt(x - 1, y, z) == nullptr))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, z * blockSize));
						
						float color = 0.6f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						vertexUVs.emplace_back(uvRects[3].x, uvRects[3].y);
						vertexUVs.emplace_back(uvRects[3].x + uvRects[3].z, uvRects[3].y);
						vertexUVs.emplace_back(uvRects[3].x + uvRects[3].z, uvRects[3].y + uvRects[3].w);
						vertexUVs.emplace_back(uvRects[3].x, uvRects[3].y + uvRects[3].w);

						vertexNormals.emplace_back(1.0f, 0.0f, 0.0f);
						vertexNormals.emplace_back(1.0f, 0.0f, 0.0f);
						vertexNormals.emplace_back(1.0f, 0.0f, 0.0f);
						vertexNormals.emplace_back(1.0f, 0.0f, 0.0f);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}

					// Front face
					if ((z == 0) || (GetBlockAt(x, y, z - 1) == nullptr))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, z * blockSize));
						
						float color = 0.5f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						vertexUVs.emplace_back(uvRects[4].x, uvRects[4].y);
						vertexUVs.emplace_back(uvRects[4].x + uvRects[4].z, uvRects[4].y);
						vertexUVs.emplace_back(uvRects[4].x + uvRects[4].z, uvRects[4].y + uvRects[4].w);
						vertexUVs.emplace_back(uvRects[4].x, uvRects[4].y + uvRects[4].w);

						vertexNormals.emplace_back(0.0f, 0.0f, -1.0f);
						vertexNormals.emplace_back(0.0f, 0.0f, -1.0f);
						vertexNormals.emplace_back(0.0f, 0.0f, -1.0f);
						vertexNormals.emplace_back(0.0f, 0.0f, -1.0f);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}

					// Back face
					if ((z == Constants::CHUNK_DEPTH - 1) || (GetBlockAt(x, y, z + 1) == nullptr))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, (z + 1) * blockSize));
						
						float color = 0.5f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						vertexUVs.emplace_back(uvRects[5].x, uvRects[5].y);
						vertexUVs.emplace_back(uvRects[5].x + uvRects[5].z, uvRects[5].y);
						vertexUVs.emplace_back(uvRects[5].x + uvRects[5].z, uvRects[5].y + uvRects[5].w);
						vertexUVs.emplace_back(uvRects[5].x, uvRects[5].y + uvRects[5].w);

						vertexNormals.emplace_back(0.0f, 0.0f, 1.0f);
						vertexNormals.emplace_back(0.0f, 0.0f, 1.0f);
						vertexNormals.emplace_back(0.0f, 0.0f, 1.0f);
						vertexNormals.emplace_back(0.0f, 0.0f, 1.0f);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}
				}
			}
		}
	}

	MeshBuilder meshBuilder;
	meshBuilder.SetVertexPositions(vertexPositions);
	meshBuilder.SetVertexColors(vertexColors);
	meshBuilder.SetVertexUVs(vertexUVs);
	meshBuilder.SetVertexNormals(vertexNormals);
	meshBuilder.SetIndices(indices);
	meshBuilder.BuildMesh(m_mesh);

	vertexPositions.clear();
	indices.clear();

	// Generate water mesh
	for (int x = 0; x < Constants::CHUNK_WIDTH; ++x)
	{
		for (int z = 0; z < Constants::CHUNK_DEPTH; ++z)
		{
			for (int y = 0; y < Constants::CHUNK_HEIGHT; ++y)
			{
				Block* currentBlock = GetBlockAt(x, y, z);
				if ((currentBlock != nullptr) && (currentBlock->GetBlockType() == BlockTypeEnum::WATER))
				{
					if ((y == Constants::CHUNK_HEIGHT - 1) || (GetBlockAt(x, y + 1, z) == nullptr))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						float yOffset = -0.1f;
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize + yOffset, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize + yOffset, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize + yOffset, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize + yOffset, z * blockSize));

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}
				}
			}
		}
	}

	meshBuilder.Clear();
	meshBuilder.SetVertexPositions(vertexPositions);
	meshBuilder.SetIndices(indices);
	meshBuilder.BuildMesh(m_waterMesh);
}

/**
 * @brief Draw the chunk
 * @param[in] camera Camera
 */
void Chunk::Draw(const Camera& camera)
{
	Engine::ShaderProgram* mainShader = ResourceManager::GetInstance().GetShader("main");
	mainShader->Use();

	mainShader->SetUniformMatrix4fv("projMatrix", false, glm::value_ptr(camera.GetProjectionMatrix()));
	mainShader->SetUniformMatrix4fv("viewMatrix", false, glm::value_ptr(camera.GetViewMatrix()));

	glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);

	glm::vec3 lightAmbient(0.1f, 0.1f, 0.1f);
	glm::vec3 lightDiffuse(1.0f, 1.0f, 1.0f);

	glm::vec3 materialAmbient(1.0f, 1.0f, 1.0f);
	glm::vec3 materialDiffuse(1.0f, 1.0f, 1.0f);

	mainShader->SetUniform3f("light.direction", lightDirection.x, lightDirection.y, lightDirection.z);
	mainShader->SetUniform3f("light.ambient", lightAmbient.x, lightAmbient.y, lightAmbient.z);
	mainShader->SetUniform3f("light.diffuse", lightDiffuse.x, lightDiffuse.y, lightDiffuse.z);
	mainShader->SetUniform3f("material.ambient", materialAmbient.x, materialAmbient.y, materialAmbient.z);
	mainShader->SetUniform3f("material.diffuse", materialDiffuse.x, materialDiffuse.y, materialDiffuse.z);

	glm::vec4 skyColor = glm::vec4(0.678f, 0.847f, 0.902f, 1.0f);
	mainShader->SetUniform4f("skyColor", skyColor.r, skyColor.g, skyColor.b, skyColor.a);
	mainShader->SetUniform1f("fogGradient", 1.5f);
	mainShader->SetUniform1f("fogDensity", 0.01f);

	Engine::Texture* blocksTexture = ResourceManager::GetInstance().GetTexture("blocks");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, blocksTexture->texID);
	mainShader->SetUniform1i("tex", 0);

	m_mesh.Draw();

	mainShader->Unuse();

	glm::vec4 waterColor(0.0f, 0.0f, 0.75f, 1.0f);
	Engine::ShaderProgram* waterShader = ResourceManager::GetInstance().GetShader("water");
	waterShader->Use();
	waterShader->SetUniformMatrix4fv("projMatrix", false, glm::value_ptr(camera.GetProjectionMatrix()));
	waterShader->SetUniformMatrix4fv("viewMatrix", false, glm::value_ptr(camera.GetViewMatrix()));
	waterShader->SetUniform4f("skyColor", skyColor.r, skyColor.g, skyColor.b, skyColor.a);
	waterShader->SetUniform1f("fogGradient", 1.5f);
	waterShader->SetUniform1f("fogDensity", 0.01f);
	waterShader->SetUniform4f("waterColor", waterColor.r, waterColor.g, waterColor.b, waterColor.a);
	m_waterMesh.Draw();
	waterShader->Unuse();
}

/**
 * @brief Gets the block at the specified location
 * @param[in] x X-coordinate
 * @param[in] y Y-coordinate
 * @param[in] z Z-coordinate
 * @return Block at the specified location
 */
Block* Chunk::GetBlockAt(int x, int y, int z)
{
	int index = (z * Constants::CHUNK_WIDTH + x) * Constants::CHUNK_HEIGHT + y;
	return m_blocks[index];
}

/**
 * @brief Sets the block at the specified location with the specified block
 * @param[in] x X-coordinate
 * @param[in] y Y-coordinate
 * @param[in] z Z-coordinate
 * @param[in] block Data for the new block. Can be set to nullptr if it's an air block.
 */
void Chunk::SetBlockAt(const int& x, const int& y, const int& z, Block* block)
{
	Block* existingBlock = GetBlockAt(x, y, z);
	if (existingBlock != nullptr)
	{
		delete existingBlock;
	}

	int index = (z * Constants::CHUNK_WIDTH + x) * Constants::CHUNK_HEIGHT + y;
	m_blocks[index] = block;
}
