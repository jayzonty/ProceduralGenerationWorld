#pragma once

#include "ShaderProgram.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

/**
 * Class for rendering UI-related stuff
 */
class UIRenderer
{
private:
    /**
     * Custom vertex struct for this renderer
     */
    struct Vertex
    {
        /**
         * Position
         */
        glm::vec2 position;

        /**
         * Color
         */
        glm::vec4 color;

        /**
         * UV-coordinates
         */
        glm::vec2 uv;
    };

    /**
     * A batch unit represents an "object" for rendering
     */
    struct BatchUnit
    {
        /**
         * Index in the batch vertices that marks the start of 
         * the vertices of this object
         */
        size_t vertexStartIndex;

        /**
         * Number of vertices in this object
         */
        size_t numVertices;

        /**
         * Order within the render layer
         */
        int32_t orderInRenderLayer;

        /**
         * @brief Constructor
         * @param[in] newVertexStartIndex Index in the batch vertices that marks the start of the vertices of this object
         * @param[in] newNumVertices Number of vertices in this object
         * @param[in] newOrderInRenderLayer Render order within the render layer
         */
        BatchUnit(const size_t& newVertexStartIndex, const size_t& newNumVertices, const int32_t& newOrderInRenderLayer)
            : vertexStartIndex(newVertexStartIndex)
            , numVertices(newNumVertices)
            , orderInRenderLayer(newOrderInRenderLayer)
        {
        }
    };

    /**
     * Render batch
     */
    struct Batch
    {
        /**
         * List of batch units
         */
        std::vector<BatchUnit> batchUnits;

        /**
         * List of vertices in the batch
         */
        std::vector<Vertex> vertices;

        /**
         * Number of active vertices in this batch
         */
        uint32_t numActiveVertices;

        /**
         * Render layer
         */
        uint32_t renderLayer;

        /**
         * Shader
         */
        ShaderProgram *shader;

        /**
         * Texture
         */
        GLuint texture;

        /**
         * @brief Adds a vertex to the batch.
         * @param[in] vertex Vertex to add
         * @return Returns true if successful.
         */
        bool AddVertex(const Vertex& vertex)
        {
            if (numActiveVertices + 1 < vertices.size())
            {
                vertices[numActiveVertices++] = vertex;
                return true;
            }
            return false;
        }
    };

private:
    /**
     * List of batches
     */
    std::vector<Batch> m_batches;

    /**
     * VBO
     */
    GLuint m_vbo;

    /**
     * VAO
     */
    GLuint m_vao;

    /**
     * Maximum number of vertices
     */
    uint32_t m_maxNumVertices;

    /**
     * Shader
     */
    ShaderProgram m_shader;

    /**
     * Single-pixel white texture
     */
    GLuint m_whiteTexture;

public:
    /**
     * @brief Constructor
     */
    UIRenderer();

    /**
     * @brief Destructor
     */
    ~UIRenderer();

    /**
     * @brief Initializes the renderer.
     * @param[in] maxVertices Maximum number of vertices
     */
    void Initialize(const uint32_t& maxNumVertices);

    /**
     * @brief Cleans up resources.
     */
    void Cleanup();

    /**
     * @brief Begin listening for draw commands.
     */
    void Begin();

    /**
     * @brief Finish listening for draw commands.
     */
    void End();

    /**
     * @brief Renders the result.
     * @param[in] viewProjectionMatrix View-projection matrix
     */
    void Render(const glm::mat4 &viewProjectionMatrix);

    /**
     * @brief Draws a quad.
     * @param[in] lowerLeft Position of the quad's lower-left corner
     * @param[in] size Size
     * @param[in] color Color
     * @param[in] renderLayer Render layer
     * @param[in] orderInRenderLayer Rendering order within the render layer
     * @param[in] texture Texture ID
     * @param[in] uvRect UV rect
     */
    void DrawQuad(const glm::vec2& lowerLeft, const glm::vec2& size, const glm::vec4& color, const uint32_t& renderLayer = 0, const int32_t& orderInRenderLayer = 0, const GLuint& texture = 0, const glm::vec4& uvRect = {});

private:
    /**
     * @brief Gets a suitable batch that meets the specified requirements.
     * @param[in] numFreeVertices Minimum number of free vertices that the batch should contain
     * @param[in] renderLayer Render layer of the batch
     * @param[in] texture Texture ID
     * @param[in] shader Shader
     * @return Returns a batch that satisfies the specified requirements
     */
    Batch* GetSuitableBatch(const uint32_t& numFreeVertices, const uint32_t& renderLayer, const GLuint& texture, const ShaderProgram* shader = nullptr);
};
