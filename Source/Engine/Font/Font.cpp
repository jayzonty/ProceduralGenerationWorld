#include "Engine/Font/Font.hpp"

#include <cstdint>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <cmath>
#include <iostream>

namespace Engine
{
/**
 * @brief Constructor
 */
Font::Font()
	: m_fontSize(30)
	, m_glyphMetrics()
	, m_isLoaded(false)
	, m_fontFilePath()
	, m_textureSize(0)
	, m_fontAtlasTexture(0)
{
}

/**
 * @brief Copy constructor
 * @param[in] other Font object to copy from
 */
Font::Font(const Font& other)
{
	m_fontSize = other.m_fontSize;
	m_glyphMetrics = other.m_glyphMetrics;

	m_isLoaded = other.m_isLoaded;

	m_fontFilePath = other.m_fontFilePath;

	m_textureSize = other.m_textureSize;
	m_fontAtlasTexture = other.m_fontAtlasTexture;
}

/**
 * @brief Destructor
 */
Font::~Font()
{
}

/**
 * @brief Load the font from the given filename
 * @param[in] fontFilePath Path to the font file
 * @return True if loading was successful. False otherwise
 */
bool Font::Load(const char* fontFilePath)
{
	// Reference: https://learnopengl.com/In-Practice/Text-Rendering

	m_isLoaded = false;

	FT_Library ftLibray;
	FT_Face ftFace;

	// FT libraries return 0 on success
	if (FT_Init_FreeType(&ftLibray))
	{
		std::printf("Failed to load freetype library!\n");
		return false;
	}
	if (FT_New_Face(ftLibray, fontFilePath, 0, &ftFace))
	{
		std::printf("Failed to load font! %s\n", fontFilePath);
		return false;
	}

	m_fontFilePath = std::string(fontFilePath);

	m_glyphMetrics.clear();

	FT_Set_Pixel_Sizes(ftFace, 0, m_fontSize);

	int totalHeight = (ftFace->size->metrics.height >> 6) * ceilf(sqrtf(NUM_GLYPHS));
	m_textureSize = 1;
	while (m_textureSize < totalHeight) // Look for the nearest power of 2
	{
		m_textureSize = m_textureSize << 1;
	}

	std::vector<unsigned char> buffer(m_textureSize * m_textureSize);

	int x = 0, y = 0;
	for (int c = 0; c < NUM_GLYPHS; ++c)
	{
		if (FT_Load_Char(ftFace, c, FT_LOAD_RENDER))
		{
			std::printf("Failed to load char %c\n", static_cast<char>(c));
			continue;
		}

		if (static_cast<int>(x + ftFace->glyph->bitmap.width) >= m_textureSize)
		{
			x = 0;
			y += (ftFace->size->metrics.height >> 6);
		}

		GlyphMetrics metrics =
		{
			x,
			y,
			static_cast<int>(ftFace->glyph->bitmap.width),
			static_cast<int>(ftFace->glyph->bitmap.rows),

			ftFace->glyph->bitmap_left,
			ftFace->glyph->bitmap_top,

			static_cast<int>(ftFace->glyph->advance.x >> 6)
		};
		m_glyphMetrics.push_back(metrics);

		for (int row = 0; row < metrics.height; ++row)
		{
			for (int col = 0; col < metrics.width; ++col)
			{
				int index = (y + row) * m_textureSize + (x + col);
				buffer[index] = ftFace->glyph->bitmap.buffer[row * ftFace->glyph->bitmap.width + col];
			}
		}

		x += metrics.width;
	}

	if (m_fontAtlasTexture == 0)
	{
		glGenTextures(1, &m_fontAtlasTexture);
	}
	glBindTexture(GL_TEXTURE_2D, m_fontAtlasTexture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
		m_textureSize,
		m_textureSize,
		0, GL_RED, GL_UNSIGNED_BYTE, buffer.data());

	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(ftFace);
	FT_Done_FreeType(ftLibray);

	m_isLoaded = true;

	return true;
}

/**
 * @brief Query whether the font has already been loaded or not.
 * @return True if the font has been loaded. False otherwise
 */
bool Font::IsLoaded() const
{
	return m_isLoaded;
}

/**
 * @brief Sets the font size
 * @param[in] fontSize New font size
 */
void Font::SetSize(int fontSize)
{
	if (!m_isLoaded)
	{
		return;
	}

	if (m_fontSize != fontSize)
	{
		m_fontSize = fontSize;
		Load(m_fontFilePath.c_str());
	}
}

/**
 * @brief Gets the glyph metrics for the provided character
 * @param[in] c Character whose glyph metrics we are querying
 * @return Glyph metrics for the provided character
 */
const Font::GlyphMetrics* Font::GetGlyphMetrics(char c)
{
	return &m_glyphMetrics[c];
}

/**
 * @brief Gets the UV rect of the specified character.
 * @param[in] c Character whose UV rect we are querying
 * @return UV rect for the provided character
 */
AABB Font::GetGlyphUVRect(char c)
{
	AABB ret;

	ret.center.x() = (m_glyphMetrics[c].textureX + m_glyphMetrics[c].width / 2.0f) / m_textureSize;
	ret.center.y() = (m_glyphMetrics[c].textureY + m_glyphMetrics[c].height / 2.0f) / m_textureSize;

	ret.halfExtents.x() = (m_glyphMetrics[c].width / 2.0f) / m_textureSize;
	ret.halfExtents.y() = -(m_glyphMetrics[c].height / 2.0f) / m_textureSize;

	return ret;
}

/**
 * @brief Gets the texture handle for the font atlas.
 * @return Texture handle for the font atlas
 */
GLuint Font::GetFontAtlasTexture() const
{
	return m_fontAtlasTexture;
}
}
