#pragma once

#include <glad/glad.h>

#include "Engine/Geometry/AABB.hpp"

#include <vector>
#include <string>

#define NUM_GLYPHS 128

namespace Engine
{
/**
 * Font
 */
class Font
{
public:
    /**
     * Glyph metrics
     */
	struct GlyphMetrics
	{
        /**
         * X-position in the font texture atlas
         */
        int textureX;

        /**
         * Y-position in the font texture atlas
         */
        int textureY;

        /**
         * Glyph width
         */
		int width;

        /**
         * Glyph height
         */
		int height;

        /**
         * Bearing (X)
         */
		int bearingX;

        /**
         * Bearing (Y)
         */
		int bearingY;

        /**
         * Advance amount
         */
		int advance;
	};

private:
	friend class Text;

    /**
     * Font size
     */
	int m_fontSize;

    /**
     * List of glyph metrics
     */
	std::vector<GlyphMetrics> m_glyphMetrics;

    /**
     * Flag indicating whether the font was loaded or not
     */
	bool m_isLoaded;

    /**
     * Path to the font file
     */
	std::string m_fontFilePath;

    /**
     * Width/height of the (square) font atlas
     */
    int m_textureSize;

    /**
     * Font atlas texture
     */
    GLuint m_fontAtlasTexture;

public:

    /**
     * @brief Constructor
     */
	Font();

    /**
     * @brief Copy constructor
     * @param[in] other Font object to copy from
     */
	Font(const Font& other);

    /**
     * @brief Destructor
     */
	~Font();

    /**
     * @brief Load the font from the given filename
     * @param[in] fontFilePath Path to the font file
     * @return True if loading was successful. False otherwise
     */
	bool Load(const char* fontFilePath);

    /**
     * @brief Query whether the font has already been loaded or not.
     * @return True if the font has been loaded. False otherwise
     */
	bool IsLoaded() const;

    /**
     * @brief Sets the font size
     * @param[in] fontSize New font size
     */
	void SetSize(int fontSize);

    /**
     * @brief Gets the glyph metrics for the provided character
     * @param[in] c Character whose glyph metrics we are querying
     * @return Glyph metrics for the provided character
     */
	const GlyphMetrics* GetGlyphMetrics(char c);

    /**
     * @brief Gets the UV rect of the specified character.
     * @param[in] c Character whose UV rect we are querying
     * @return UV rect for the provided character
     */
	AABB GetGlyphUVRect(char c);

    /**
     * @brief Gets the texture handle for the font atlas.
     * @return Texture handle for the font atlas
     */
    GLuint GetFontAtlasTexture() const;
};
}
