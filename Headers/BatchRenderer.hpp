#pragma once

#include "Mesh.hpp"
#include "Vertex.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

/**
 * Class for rendering a batch of geometry
 */
class BatchRenderer
{
private:
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
         * Index in the batch indices that marks the start of
         * the indices of this object
         */
        size_t indicesStartIndex;

        /**
         * Number of indices in this object
         */
        size_t numIndices;

        /**
         * Order within the render layer
         */
        int32_t orderInRenderLayer;

        /**
         * @brief Constructor
         * @param[in] newVertexStartIndex Index in the batch vertices that marks the start of the vertices of this object
         * @param[in] newNumVertices Number of vertices in this object
         * @param[in] newIndicesStartIndex Index in the batch indices that marks the start of the indices of this object
         * @param[in] newNumIndices Number of indices in this object
         * @param[in] newOrderInRenderLayer Render order within the render layer
         */
        BatchUnit(const size_t& newVertexStartIndex, const size_t& newNumVertices, const size_t &newIndicesStartIndex, const size_t &newNumIndices, const int32_t& newOrderInRenderLayer)
            : vertexStartIndex(newVertexStartIndex)
            , numVertices(newNumVertices)
            , indicesStartIndex(newIndicesStartIndex)
            , numIndices(newNumIndices)
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
         * List of indices in the batch
         */
        std::vector<GLuint> indices;

        /**
         * Number of active vertices in this batch
         */
        uint32_t numActiveVertices;

        /**
         * Number of active indices in this batch
         */
        uint32_t numActiveIndices;

        /**
         * Render layer
         */
        uint32_t renderLayer;

        /**
         * Texture
         */
        GLuint texture;
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
     * EBO
     */
    GLuint m_ebo;

    /**
     * Maximum number of vertices
     */
    uint32_t m_maxNumVertices;

    /**
     * Maximum number of indices
     */
    uint32_t m_maxNumIndices;

    /**
     * Single-pixel white texture
     */
    GLuint m_whiteTexture;

public:
    /**
     * @brief Constructor
     */
    BatchRenderer();

    /**
     * @brief Destructor
     */
    ~BatchRenderer();

    /**
     * @brief Initializes the renderer.
     * @param[in] maxNumVertices Maximum number of vertices
     * @param[in] maxNumIndices Maximum number of indices
     */
    void Initialize(const uint32_t& maxNumVertices, const uint32_t &maxNumIndices);

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
     * @brief Renders all geometry
     */
    void Render();

    /**
     * @brief Adds a mesh to the batch renderer
     * @param[in] mesh Mesh data
     * @param[in] transform Transformation matrix to apply to the vertices
     * @param[in] renderLayer Render layer
     * @param[in] orderInRenderLayer Rendering order within the render layer
     * @param[in] texture Texture ID
     */
    void AddMesh(const Mesh &mesh, const glm::mat4 &transform = glm::mat4(1.0f), const uint32_t& renderLayer = 0, const int32_t& orderInRenderLayer = 0, const GLuint& texture = 0);

private:
    /**
     * @brief Gets a suitable batch that meets the specified requirements.
     * @param[in] numFreeVertices Minimum number of free vertices that the batch should contain
     * @param[in] numFreeIndices Minimum number of free indices that the batch should contain
     * @param[in] renderLayer Render layer of the batch
     * @param[in] texture Texture ID
     * @return Returns a batch that satisfies the specified requirements
     */
    Batch* GetSuitableBatch(const uint32_t& numFreeVertices, const uint32_t &numFreeIndices, const uint32_t& renderLayer, const GLuint& texture);
};
