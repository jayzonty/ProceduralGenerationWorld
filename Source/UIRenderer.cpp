#include "UIRenderer.hpp"

#include <glm/gtc/type_ptr.hpp>

#define _USE_MATH_INCLUDES
#include <algorithm>
#include <cmath>
#include <cstddef>

/**
 * @brief Constructor
 */
UIRenderer::UIRenderer()
    : m_batches(1)
    , m_vbo(0)
    , m_vao(0)
    , m_maxNumVertices(0)
    , m_shader()
    , m_whiteTexture(0)
{
}

/**
 * @brief Destructor
 */
UIRenderer::~UIRenderer()
{
}


/**
 * @brief Initialize the renderer
 * @param[in] maxVertices Maximum number of vertices
 */
void UIRenderer::Initialize(const uint32_t& maxNumVertices)
{
    m_maxNumVertices = maxNumVertices;

    m_batches[0].vertices.resize(maxNumVertices);
    m_batches[0].numActiveVertices = 0;

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

    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));

    glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));

    m_shader.InitFromFiles("Resources/Shaders/UI.vsh", "Resources/Shaders/UI.fsh");

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
void UIRenderer::Cleanup()
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
void UIRenderer::Begin()
{
    for (size_t i = 0; i < m_batches.size(); ++i)
    {
        m_batches[i].numActiveVertices = 0;
        m_batches[i].batchUnits.clear();
    }
}

/**
 * @brief Finish listening for draw commands.
 */
void UIRenderer::End()
{
}

/**
 * @brief Renders the result.
 * @param[in] viewProjectionMatrix View-projection matrix
 */
void UIRenderer::Render(const glm::mat4& viewProjectionMatrix)
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

            m_batches[i].shader->Use();
            m_batches[i].shader->SetUniformMatrix4fv("viewProjectionMatrix", false, glm::value_ptr(viewProjectionMatrix));
            m_batches[i].shader->SetUniform1i("tex0", 0);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBindTexture(GL_TEXTURE_2D, m_batches[i].texture);

            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            GLint offset = 0;
            for (auto& it : m_batches[i].batchUnits)
            {
                GLint size = sizeof(Vertex) * it.numVertices;
                glBufferSubData(GL_ARRAY_BUFFER, offset, size, &m_batches[i].vertices[it.vertexStartIndex]);
                offset += size;
            }

            glBindVertexArray(m_vao);
            glDrawArrays(GL_TRIANGLES, 0, m_batches[i].numActiveVertices);
        }
    }
}

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
void UIRenderer::DrawQuad(const glm::vec2& lowerLeft, const glm::vec2& size, const glm::vec4& color, const uint32_t& renderLayer, const int32_t& orderInRenderLayer, const GLuint& texture, const glm::vec4& uvRect)
{
    Batch* batch = GetSuitableBatch(6, renderLayer, texture, &m_shader);
    size_t vertexStartIndex = batch->numActiveVertices;

    glm::vec2 lowerRight { lowerLeft.x + size.x, lowerLeft.y };
    glm::vec2 upperRight { lowerLeft.x + size.x, lowerLeft.y + size.y };
    glm::vec2 upperLeft { lowerLeft.x, lowerLeft.y + size.y };

    glm::vec2 lowerLeftUV { uvRect.x, uvRect.y };
    glm::vec2 lowerRightUV { uvRect.x + uvRect.z, uvRect.y };
    glm::vec2 upperRightUV { uvRect.x + uvRect.z, uvRect.y + uvRect.w };
    glm::vec2 upperLeftUV { uvRect.x, uvRect.y + uvRect.w };

    batch->AddVertex( { lowerLeft, color, lowerLeftUV } );
    batch->AddVertex( { lowerRight, color, lowerRightUV } );
    batch->AddVertex( { upperRight, color, upperRightUV } );

    batch->AddVertex( { upperRight, color, upperRightUV } );
    batch->AddVertex( { upperLeft, color, upperLeftUV } );
    batch->AddVertex( { lowerLeft, color, lowerLeftUV } );

    batch->batchUnits.emplace_back(vertexStartIndex, 6, orderInRenderLayer);
}

/**
 * @brief Gets a suitable batch that meets the specified requirements.
 * @param[in] numFreeVertices Minimum number of free vertices that the batch should contain
 * @param[in] renderLayer Render layer of the batch
 * @return Returns a batch that satisfies the specified requirements
 */
UIRenderer::Batch* UIRenderer::GetSuitableBatch(const uint32_t& numFreeVertices, const uint32_t& renderLayer, const GLuint& texture, const ShaderProgram* shader)
{
    ShaderProgram* requiredShader = const_cast<ShaderProgram*>(shader);
    if (requiredShader == nullptr)
    {
        requiredShader = &m_shader;
    }

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
        uint32_t currentBatchRenderLayer = m_batches[i].renderLayer;
        GLuint currentBatchTexture = m_batches[i].texture;
        ShaderProgram* currentBatchShader = m_batches[i].shader;
        if ((numActiveVertices + numFreeVertices <= m_maxNumVertices)
            && (currentBatchRenderLayer == renderLayer)
            && (currentBatchTexture == tex)
            && (currentBatchShader == requiredShader))
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
        m_batches.back().numActiveVertices = 0;
        m_batches.back().renderLayer = renderLayer;
        m_batches.back().texture = tex;
        m_batches.back().shader = requiredShader;

        ret = &m_batches.back();
    }

    return ret;
}
