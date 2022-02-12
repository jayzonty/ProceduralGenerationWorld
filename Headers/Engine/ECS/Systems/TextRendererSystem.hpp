#pragma once

#include "Engine/ECS/ECS.hpp"

#include "Engine/Font/Font.hpp"

#include "Engine/Graphics/Renderer.hpp"
#include "Engine/Graphics/ShaderProgram.hpp"

#include "Engine/Matrix.hpp"

namespace Engine
{
namespace ECS
{
/**
 * ECS system for rendering text
 */
class TextRendererSystem
{
private:
    /**
     * Quad for a glyph
     */
    struct GlyphQuad
    {
        /**
         * Geometry rect
         */
        AABB quadRect;

        /**
         * UV rect
         */
        AABB uvRect;
    };

    /**
     * Description for a line of text
     */
    struct LineDescription
    {
        /**
         * List of glyph quads in the line
         */
        std::vector<GlyphQuad> glyphQuads;

        /**
         * Line width
         */
        float lineWidth;
    };

private:
    /**
     * Font used to render all the text.
     * TODO: Make the text renderer system work for arbitrary font
     */
    Font *m_font;

    /**
     * Reference to the batch renderer
     */
    Renderer* m_renderer;

    /**
     * Font rendering shader
     */
    ShaderProgram m_shader;

public:
    /**
     * @brief Constructor
     */
    TextRendererSystem();

    /**
     * @brief Destructor
     */
    ~TextRendererSystem();

    /**
     * @brief Initializes this system.
     */
    void Initialize(Renderer* renderer);

    /**
     * @brief Draws all the entities with transform and text components.
     */
    void Draw();

    /**
     * @brief Cleans up the resources used by this system.
     */
    void Cleanup();

private:
    /**
     * @brief Splits the provided string into lines of strings.
     * @param[in] str String to split into lines
     * @param[out] outLines Vector to put the output into
     */
    void GetLines(const std::string& str, std::vector<std::string>& outLines);

};
}
}