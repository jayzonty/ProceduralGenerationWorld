#include "BatchRenderer.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define _USE_MATH_INCLUDES
#include <algorithm>
#include <cmath>
#include <cstddef>

/**
 * @brief Constructor
 */
BatchRenderer::BatchRenderer()
    : m_batches(1)
    , m_vbo(0)
    , m_vao(0)
    , m_ebo(0)
    , m_maxNumVertices(0)
    , m_whiteTexture(0)
{
}

/**
 * @brief Destructor
 */
BatchRenderer::~BatchRenderer()
{
}


/**
 * @brief Initialize the renderer
 * @param[in] maxNumVertices Maximum number of vertices
 * @param[in] maxNumIndices Maximum number of indices
 */
void BatchRenderer::Initialize(const uint32_t& maxNumVertices, const uint32_t &maxNumIndices)
{
    m_maxNumVertices = maxNumVertices;
    m_maxNumIndices = maxNumIndices;

    m_batches[0].vertices.resize(maxNumVertices);
    m_batches[0].numActiveVertices = 0;
    m_batches[0].indices.resize(maxNumIndices);
    m_batches[0].numActiveIndices = 0;

    if (m_vao == 0)
    {
        glGenVertexArrays(1, &m_vao);
    }
    glBindVertexArray(m_vao);

    if (m_vbo == 0)
    {
        glGenBuffers(1, &m_vbo);
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * maxNumVertices, nullptr, GL_DYNAMIC_DRAW);

    if (m_ebo == 0)
    {
        glGenBuffers(1, &m_ebo);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * maxNumIndices, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));

    glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));

    // Create a single-pixel white texture
    if (m_whiteTexture == 0)
    {
        glGenTextures(1, &m_whiteTexture);
    }
    glBindTexture(GL_TEXTURE_2D, m_whiteTexture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char data[4] = { 255, 255, 255, 255 };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		1,
		1,
		0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

/**
 * @brief Cleans up resources
 */
void BatchRenderer::Cleanup()
{
    glDeleteTextures(1, &m_whiteTexture);
    m_whiteTexture = 0;

    glDeleteBuffers(1, &m_vbo);
    m_vbo = 0;

    glDeleteVertexArrays(1, &m_vao);
    m_vao = 0;

    m_batches.clear();
}

/**
 * @brief Begin listening for draw commands.
 */
void BatchRenderer::Begin()
{
    for (size_t i = 0; i < m_batches.size(); ++i)
    {
        m_batches[i].numActiveVertices = 0;
        m_batches[i].numActiveIndices = 0;
        m_batches[i].batchUnits.clear();
    }
}

/**
 * @brief Finish listening for draw commands.
 */
void BatchRenderer::End()
{
}

/**
 * @brief Renders the result.
 */
void BatchRenderer::Render()
{
    // Sort the batches based on z-order
    std::sort(
        m_batches.begin(),
        m_batches.end(),
        [&](const Batch& a, const Batch& b) -> bool
        {
            return a.renderLayer < b.renderLayer;
        }
    );

    for (size_t i = 0; i < m_batches.size(); ++i)
    {
        if (m_batches[i].numActiveVertices > 0)
        {
            // Sort batch units based on order in render layer
            std::sort(
                m_batches[i].batchUnits.begin(),
                m_batches[i].batchUnits.end(),
                [&](const BatchUnit& a, const BatchUnit& b) -> bool
                {
                    return a.orderInRenderLayer < b.orderInRenderLayer;
                }
            );

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBindTexture(GL_TEXTURE_2D, m_batches[i].texture);

            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
            GLint vertexOffset = 0;
            GLint indexOffset = 0;
            for (auto& it : m_batches[i].batchUnits)
            {
                GLint verticesSize = sizeof(Vertex) * it.numVertices;
                glBufferSubData(GL_ARRAY_BUFFER, vertexOffset, verticesSize, &m_batches[i].vertices[it.vertexStartIndex]);
                vertexOffset += verticesSize;

                GLint indicesSize = sizeof(GLuint) * it.numIndices;
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexOffset, indicesSize, &m_batches[i].indices[it.indicesStartIndex]);
                indexOffset += indicesSize;
            }

            glBindVertexArray(m_vao);
            glDrawElements(GL_TRIANGLES, m_batches[i].numActiveIndices, GL_UNSIGNED_INT, nullptr);
        }
    }
}

/**
 * @brief Adds a mesh to the batch renderer
 * @param[in] mesh Mesh data
 * @param[in] transform Transformation matrix to apply to the vertices
 * @param[in] renderLayer Render layer
 * @param[in] orderInRenderLayer Rendering order within the render layer
 * @param[in] texture Texture ID
 */
void BatchRenderer::AddMesh(const Mesh &mesh, const glm::mat4 &transform, const uint32_t& renderLayer, const int32_t& orderInRenderLayer, const GLuint& texture)
{
    Batch* batch = GetSuitableBatch(mesh.vertices.size(), mesh.indices.size(), renderLayer, texture);
    size_t vertexStartIndex = batch->numActiveVertices;
    size_t indicesStartIndex = batch->numActiveIndices;
    batch->batchUnits.emplace_back(vertexStartIndex, mesh.vertices.size(), indicesStartIndex, mesh.indices.size(), orderInRenderLayer);

    for (size_t i = 0; i < mesh.vertices.size(); ++i)
    {
        glm::vec4 transformedPosition(mesh.vertices[i].position, 1.0f);
        transformedPosition = transform * transformedPosition;

        batch->vertices[batch->numActiveVertices] = mesh.vertices[i];
        batch->vertices[batch->numActiveVertices].position = transformedPosition;
        ++batch->numActiveVertices;
    }

    for (size_t i = 0; i < mesh.indices.size(); ++i)
    {
        batch->indices[batch->numActiveIndices++] = vertexStartIndex + mesh.indices[i];
    }
}

/**
 * @brief Gets a suitable batch that meets the specified requirements.
 * @param[in] numFreeVertices Minimum number of free vertices that the batch should contain
 * @param[in] numFreeIndices Minimum number of free indices that the batch should contain
 * @param[in] renderLayer Render layer of the batch
 * @param[in] texture Texture ID
 * @return Returns a batch that satisfies the specified requirements
 */
BatchRenderer::Batch* BatchRenderer::GetSuitableBatch(const uint32_t& numFreeVertices, const uint32_t &numFreeIndices, const uint32_t& renderLayer, const GLuint& texture)
{
    GLuint tex = texture;
    if (tex == 0)
    {
        tex = m_whiteTexture;
    }

    Batch* ret = nullptr;

    // Try to look for a suitable batch
    for (size_t i = 0; i < m_batches.size(); ++i)
    {
        // Replace this condition in the future if needed
        // (e.g., need to check if this batch supports the requested draw mode, etc.)
        uint32_t numActiveVertices = m_batches[i].numActiveVertices;
        uint32_t numActiveIndices = m_batches[i].numActiveIndices;
        uint32_t currentBatchRenderLayer = m_batches[i].renderLayer;
        GLuint currentBatchTexture = m_batches[i].texture;
        if ((numActiveVertices + numFreeVertices <= m_maxNumVertices)
            && (numActiveIndices + numFreeIndices <= m_maxNumIndices)
            && (currentBatchRenderLayer == renderLayer)
            && (currentBatchTexture == tex))
        {
            ret = &m_batches[i];
            break;
        }
    }

    // If we weren't able to find a suitable batch, create a new one
    if (ret == nullptr)
    {
        m_batches.emplace_back();

        m_batches.back().vertices.resize(m_maxNumVertices);
        m_batches.back().indices.resize(m_maxNumIndices);
        m_batches.back().numActiveVertices = 0;
        m_batches.back().numActiveIndices = 0;
        m_batches.back().renderLayer = renderLayer;
        m_batches.back().texture = tex;

        ret = &m_batches.back();
    }

    return ret;
}
