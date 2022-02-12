#include "Engine/Graphics/Renderer.hpp"

#include "Engine/Graphics/ShaderProgram.hpp"
#include "Engine/Matrix.hpp"
#include "Engine/MatrixTransform.hpp"

#define _USE_MATH_INCLUDES
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>

namespace Engine
{
/**
 * @brief Constructor
 */
Renderer::Renderer()
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
Renderer::~Renderer()
{
}


/**
 * @brief Initialize the renderer
 * @param[in] maxVertices Maximum number of vertices
 */
void Renderer::Initialize(const uint32_t& maxNumVertices)
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, x)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, r)));

    glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, u)));

    m_shader.InitFromFiles("resources/shaders/basic.vsh", "resources/shaders/basic.fsh");

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
void Renderer::Cleanup()
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
void Renderer::Begin()
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
void Renderer::End()
{
}

/**
 * @brief Renders the result.
 * @param[in] viewProjectionMatrix View-projection matrix
 */
void Renderer::Render(const Matrix4& viewProjectionMatrix)
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
            m_batches[i].shader->SetUniformMatrix4fv("mvpMatrix", false, viewProjectionMatrix.values);
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

    m_shader.Unuse();
}

/**
 * @brief Draws a triangle.
 * @param[in] a First point of the triangle
 * @param[in] b Second point of the triangle
 * @param[in] c Third point of the triangle
 * @param[in] color Triangle color
 * @param[in] renderLayer Render layer
 * @param[in] orderInRenderLayer Rendering order within the render layer
 */
void Renderer::DrawTriangle(const Vector2f& a, const Vector2f& b, const Vector2f& c, const Color& color, const uint32_t& renderLayer, const int32_t& orderInRenderLayer)
{
    Batch* batch = GetSuitableBatch(3, renderLayer, 0);

    size_t vertexStartIndex = batch->numActiveVertices;
    batch->AddVertex( { a.x(), a.y(), color.r, color.g, color.b, color.a } );
    batch->AddVertex( { b.x(), b.y(), color.r, color.g, color.b, color.a } );
    batch->AddVertex( { c.x(), c.y(), color.r, color.g, color.b, color.a } );

    batch->batchUnits.emplace_back(vertexStartIndex, 3, orderInRenderLayer);
}

/**
 * @brief Draws a circle.
 * @param[in] center Circle center
 * @param[in] radius Circle radius
 * @param[in] color Circle color
 * @param[in] renderLayer Render layer
 * @param[in] orderInRenderLayer Rendering order within the render layer
 */
void Renderer::DrawCircle(const Vector2f &center, const float &radius, const Color &color, const uint32_t& renderLayer, const int32_t& orderInRenderLayer)
{
    int numSections = 180;
    uint32_t numVertices = numSections * 3;

    Batch* batch = GetSuitableBatch(numVertices, renderLayer, 0);
    
    size_t vertexStartIndex = batch->numActiveVertices;
	float anglePerSection = 360.0f / numSections * M_PI / 180.0f;
	for (int i = 1; i <= numSections; ++i)
    {
		float x0 = center.x() + radius * cosf(anglePerSection * (i - 1));
		float y0 = center.y() + radius * sinf(anglePerSection * (i - 1));
		float x1 = center.x() + radius * cosf(anglePerSection * i);
		float y1 = center.y() + radius * sinf(anglePerSection * i);
		batch->AddVertex( { center.x(), center.y(), color.r, color.g, color.b, color.a } );
		batch->AddVertex( { x0, y0, color.r, color.g, color.b, color.a } );
		batch->AddVertex( { x1, y1, color.r, color.g, color.b, color.a } );
	}

    batch->batchUnits.emplace_back(vertexStartIndex, batch->vertices.size() - vertexStartIndex, orderInRenderLayer);
}

/**
 * @brief Draws a line segment.
 * @param[in] a First point of the line segment
 * @param[in] b Second point of the line segment
 * @param[in] lineWidth Line width
 * @param[in] color Line color
 * @param[in] renderLayer Render layer
 * @param[in] orderInRenderLayer Rendering order within the render layer
 */
void Renderer::DrawLineSegment(const Vector2f &a, const Vector2f &b, const float &width, const Color &color, const uint32_t& renderLayer, const int32_t& orderInRenderLayer)
{
    Vector2f ab = b - a;
    Vector2f n = Engine::Normalize(Engine::Perp(ab));

    Vector2f offset = n * width / 2.0f;
    Vector2f p0 = a + offset, p1 = a - offset;
    Vector2f p2 = b - offset, p3 = b + offset;

    Batch* batch = GetSuitableBatch(4, renderLayer, 0);

    size_t vertexStartIndex = batch->numActiveVertices;
    batch->AddVertex( { p0.x(), p0.y(), color.r, color.g, color.b, color.a } );
    batch->AddVertex( { p1.x(), p1.y(), color.r, color.g, color.b, color.a } );
    batch->AddVertex( { p2.x(), p2.y(), color.r, color.g, color.b, color.a } );
    batch->AddVertex( { p2.x(), p2.y(), color.r, color.g, color.b, color.a } );
    batch->AddVertex( { p3.x(), p3.y(), color.r, color.g, color.b, color.a } );
    batch->AddVertex( { p0.x(), p0.y(), color.r, color.g, color.b, color.a } );

    batch->batchUnits.emplace_back(vertexStartIndex, 6, orderInRenderLayer);
}

/**
 * @brief Draws a quad.
 * @param[in] center Center of the quad
 * @param[in] halfExtents Half extents
 * @param[in] color Color
 * @param[in] renderLayer Render layer
 * @param[in] orderInRenderLayer Rendering order within the render layer
 * @param[in] shader Shader to use
 * @param[in] texture Texture ID
 * @param[in] uvRect UV rect
 */
void Renderer::DrawQuad(const Vector2f& center, const Vector2f& halfExtents, const Color& color, const uint32_t& renderLayer, const int32_t& orderInRenderLayer, const ShaderProgram* shader, const GLuint& texture, const AABB& uvRect)
{
    Batch* batch = GetSuitableBatch(6, renderLayer, texture, shader);
    size_t vertexStartIndex = batch->numActiveVertices;

    Vector2f lowerLeft = center - halfExtents, lowerRight = { center.x() + halfExtents.x(), center.y() - halfExtents.y() };
    Vector2f upperRight = center + halfExtents, upperLeft = { center.x() - halfExtents.x(), center.y() + halfExtents.y() };

    batch->AddVertex( { lowerLeft.x(), lowerLeft.y(), color.r, color.g, color.b, color.a, uvRect.GetMin().x(), uvRect.GetMin().y() } );
    batch->AddVertex( { lowerRight.x(), lowerRight.y(), color.r, color.g, color.b, color.a, uvRect.GetMax().x(), uvRect.GetMin().y() } );
    batch->AddVertex( { upperRight.x(), upperRight.y(), color.r, color.g, color.b, color.a, uvRect.GetMax().x(), uvRect.GetMax().y() } );
    batch->AddVertex( { upperRight.x(), upperRight.y(), color.r, color.g, color.b, color.a, uvRect.GetMax().x(), uvRect.GetMax().y() } );
    batch->AddVertex( { upperLeft.x(), upperLeft.y(), color.r, color.g, color.b, color.a, uvRect.GetMin().x(), uvRect.GetMax().y() } );
    batch->AddVertex( { lowerLeft.x(), lowerLeft.y(), color.r, color.g, color.b, color.a, uvRect.GetMin().x(), uvRect.GetMin().y() } );

    batch->batchUnits.emplace_back(vertexStartIndex, 6, orderInRenderLayer);
}

/**
 * @brief Gets a suitable batch that meets the specified requirements.
 * @param[in] numFreeVertices Minimum number of free vertices that the batch should contain
 * @param[in] renderLayer Render layer of the batch
 * @return Returns a batch that satisfies the specified requirements
 */
Renderer::Batch* Renderer::GetSuitableBatch(const uint32_t& numFreeVertices, const uint32_t& renderLayer, const GLuint& texture, const ShaderProgram* shader)
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
}
