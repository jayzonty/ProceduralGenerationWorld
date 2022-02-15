#pragma once

#include <cstdint>
namespace Constants
{
	/**
	 * Scene ID for the main scene
	 */
	const uint32_t MAIN_SCENE_ID = 0;

	/**
	 * Scene ID for the sandbox scene
	 */
	const uint32_t SANDBOX_SCENE_ID = 1;

	/**
	 * Chunk width
	 */
	const int CHUNK_WIDTH = 16;

	/**
	 * Chunk height
	 */
	const int CHUNK_HEIGHT = 256;

	/**
	 * Chunk depth
	 */
	const int CHUNK_DEPTH = 16;

	/**
	 * Block size
	 */
	const float BLOCK_SIZE = 1.0f;
}
