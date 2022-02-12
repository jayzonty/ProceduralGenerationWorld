#pragma once

#include "Engine/Graphics/Color.hpp"
#include "Engine/Vector.hpp"

#include <string>

namespace Engine
{
namespace ECS
{
    /**
     * Component containing data for rendering text
     */
    struct TextComponent
    {
        enum class Alignment
        {
            // Horizontal
            Left,
            Center,
            Right
        };

        /**
         * String for the text to display
         */
        std::string text;

        /**
         * Text color
         */
        Color textColor;

        /**
         * Line spacing
         */
        float lineSpacing;

        /**
         * Flag indicating whether the text is visible or not
         */
        bool isVisible;

        /**
         * Horizontal alignment
         */
        Alignment horizontalAlign;

        /**
         * Text origin 
         * ({0,0} is lower left, {1,1} is upper-right)
         */
        Vector2f origin;

        /**
         * Render layer
         */
        uint32_t renderLayer;

        /**
         * @brief Constructor
         */
        TextComponent()
            : text()
            , textColor()
            , lineSpacing(0.0f)
            , isVisible(true)
            , horizontalAlign(Alignment::Left)
            , origin({0.0f, 0.0f})
            , renderLayer(0)
        {
        }
    };
}
}
