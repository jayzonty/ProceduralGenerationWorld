#pragma once

#include <glm/glm.hpp>

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

	/**
	 * Sky color (daytime)
	 */
	const glm::vec4 DAY_SKY_COLOR = { 0.678f, 0.847f, 0.902f, 1.0f };

	/**
	 * Sky color (nighttime)
	 */
	const glm::vec4 NIGHT_SKY_COLOR = { 0.0f, 0.0f, 0.0f, 1.0f };

	/**
	 * Amount of time until a time tick
	 */
	const float TIME_TICK_DURATION = 0.1f;

	/**
	 * Total time ticks
	 */
	const uint32_t TOTAL_TIME_TICKS = 1440;

	/**
	 * Time when dusk starts
	 */
	const uint32_t DUSK_START_TIME = 600;

	/**
	 * Time when night time starts
	 */
	const uint32_t NIGHT_START_TIME = 720;

	/**
	 * Time when dawn starts
	 */
	const uint32_t DAWN_START_TIME = 1320;
}
