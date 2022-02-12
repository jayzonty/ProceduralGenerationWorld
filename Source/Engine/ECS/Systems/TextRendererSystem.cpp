#include "Engine/ECS/Systems/TextRendererSystem.hpp"
#include "Engine/ECS/Components/SceneComponent.hpp"
#include "Engine/ECS/ECS.hpp"

#include "Engine/ECS/Components/TextComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"

#include "Engine/MatrixTransform.hpp"
#include "Engine/Vector.hpp"
#include "Engine/WindowManager.hpp"
#include <algorithm>
#include <iostream>
#include <set>

namespace Engine
{
namespace ECS
{
/**
 * @brief Constructor
 */
TextRendererSystem::TextRendererSystem()
    : m_font(nullptr)
    , m_shader()
{
}

/**
 * @brief Destructor
 */
TextRendererSystem::~TextRendererSystem()
{
}

/**
 * @brief Initializes this system.
 */
void TextRendererSystem::Initialize(Renderer* renderer)
{
    m_renderer = renderer;

    m_shader.InitFromFiles("resources/shaders/font.vsh", "resources/shaders/font.fsh");

    m_font = new Font();
    if (!m_font->Load("resources/fonts/SourceCodePro-Regular.ttf"))
    {
        std::cout << "[TextRenderSystem] Failed to load font!" << std::endl;
    }
    m_font->SetSize(16);
}

/**
 * @brief Draws all the entities with transform and text components.
 */
void TextRendererSystem::Draw()
{
    std::set<entity_t> entities = Registry::GetEntitiesWithComponents<TransformComponent, TextComponent>();
    for (entity_t entity : entities)
    {
        TextComponent* text = Registry::GetComponent<TextComponent>(entity);
        if (!text->isVisible)
        {
            continue;
        }

        TransformComponent* transform = Registry::GetComponent<TransformComponent>(entity);
        Vector2f worldPosition = transform->GetWorldPosition();

        Vector2f textDimension { 0.0f, 0.0f };

        std::vector<std::string> lines;
        GetLines(text->text, lines);

        std::vector<LineDescription> lineDescriptions;
        float yOffset = 0.0f;
        for (size_t lineNumber = lines.size(); lineNumber > 0; --lineNumber)
        {
            float xOffset = 0.0f;
            int lineHeight = 0;

            lineDescriptions.emplace_back();
            LineDescription& lineDescription = lineDescriptions.back();

            const std::string& line = lines[lineNumber - 1];
            for (size_t i = 0; i < line.size(); ++i)
            {
                lineDescription.glyphQuads.emplace_back();
                GlyphQuad& glyphQuad = lineDescription.glyphQuads.back();

                const Font::GlyphMetrics* glyphMetrics = m_font->GetGlyphMetrics(line[i]);

                Vector2f min { 0.0f, 0.0f };
                min.x() = worldPosition.x() + xOffset + glyphMetrics->bearingX;
                min.y() = worldPosition.y() + yOffset - (glyphMetrics->height - glyphMetrics->bearingY);

                Vector2f max { 0.0f, 0.0f };
                max.x() = min.x() + glyphMetrics->width;
                max.y() = min.y() + glyphMetrics->height;

                glyphQuad.quadRect.center = (min + max) / 2.0f;
                glyphQuad.quadRect.halfExtents = (max - min) / 2.0f;
                glyphQuad.uvRect = m_font->GetGlyphUVRect(line[i]);

                xOffset += glyphMetrics->advance;

                lineHeight = std::max(lineHeight, glyphMetrics->height);
            }

            lineDescription.lineWidth = xOffset;

            textDimension.x() = std::max(textDimension.x(), xOffset);

            yOffset += lineHeight + text->lineSpacing;
        }

        textDimension.y() = yOffset;

        // Calculate the ratio of the blank space in the line
        // to be used as left padding depending on the alignment.
        // If left align, 0% free space padded to the left side
        // If center, 50% free space padded to the left side
        // If right align 100% free space padded to the left side
        float leftFreeSpaceRatio = 0.0f;
        if (text->horizontalAlign == TextComponent::Alignment::Center)
        {
            leftFreeSpaceRatio = 0.5f;
        }
        else if (text->horizontalAlign == TextComponent::Alignment::Right)
        {
            leftFreeSpaceRatio = 1.0f;
        }

        // Offset each line based on the alignment
        for (size_t i = 0; i < lineDescriptions.size(); ++i)
        {
            LineDescription& lineDescription = lineDescriptions[i];
            float freeSpaceInLine = textDimension.x() - lineDescription.lineWidth;
            float lineXOffset = freeSpaceInLine * leftFreeSpaceRatio;

            for (size_t j = 0; j < lineDescriptions[i].glyphQuads.size(); ++j)
            {
                lineDescriptions[i].glyphQuads[j].quadRect.center.x() += lineXOffset;
            }
        }

        // Offset each line based on the origin
        Vector2f originOffset { textDimension.x() * text->origin.x(), textDimension.y() * text->origin.y() };
        for (size_t i = 0; i < lineDescriptions.size(); ++i)
        {
            for (size_t j = 0; j < lineDescriptions[i].glyphQuads.size(); ++j)
            {
                lineDescriptions[i].glyphQuads[j].quadRect.center = lineDescriptions[i].glyphQuads[j].quadRect.center - originOffset;
            }
        }

        int32_t order = transform->GetWorldDepth();

        // Draw the quads of each glyph
        for (size_t i = lineDescriptions.size(); i > 0; --i)
        {
            LineDescription& lineDescription = lineDescriptions[i - 1];
            for (size_t j = 0; j < lineDescription.glyphQuads.size(); ++j)
            {
                GlyphQuad& glyphQuad = lineDescription.glyphQuads[j];

                m_renderer->DrawQuad(
                    glyphQuad.quadRect.center,
                    glyphQuad.quadRect.halfExtents,
                    text->textColor,
                    0,
                    order,
                    &m_shader,
                    m_font->GetFontAtlasTexture(),
                    glyphQuad.uvRect
                );
            }
        }
    }
}

/**
 * @brief Cleans up the resources used by this system.
 */
void TextRendererSystem::Cleanup()
{
    if (m_font != nullptr)
    {
        delete m_font;
        m_font = nullptr;
    }
}

/**
 * @brief Splits the provided string into lines of strings.
 * @param[in] str String to split into lines
 * @param[out] outLines Vector to put the output into
 */
void TextRendererSystem::GetLines(const std::string& str, std::vector<std::string>& outLines)
{
    size_t lineStart = 0;
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (str[i] == '\n')
        {
            if (i - lineStart > 0)
            {
                outLines.push_back(str.substr(lineStart, i - lineStart));
                lineStart = i + 1;
            }
        }
    }

    if (str.size() - lineStart > 0)
    {
        outLines.push_back(str.substr(lineStart));
    }
}
}
}
