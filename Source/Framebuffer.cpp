#include "Framebuffer.hpp"

/**
 * @brief Constructor
 */
Framebuffer::Framebuffer()
    : m_fbo(0)
{
}

/**
 * @brief Destructor
 */
Framebuffer::~Framebuffer()
{
    Cleanup();
}

/**
 * @brief Initializes this framebuffer
 */
void Framebuffer::Initialize()
{
    if (m_fbo == 0)
    {
        glGenFramebuffers(1, &m_fbo);
    }
}

/**
 * @brief Cleans up this framebuffer
 */
void Framebuffer::Cleanup()
{
    if (m_fbo != 0)
    {
        glDeleteFramebuffers(1, &m_fbo);
        m_fbo = 0;
    }
}

/**
 * @brief Attaches a texture to the framebuffer
 * @param[in] attachmentType Attachment type
 * @param[in] texture Texture
 */
void Framebuffer::AttachTexture2D(const GLenum &attachmentType, const GLuint &texture)
{
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, texture, 0);
    Unbind();
}

/**
 * @brief Attaches a renderbuffer to the framebuffer
 * @param[in] attachmentType Attachment type
 * @param[in] renderBuffer Renderbuffer
 */
void Framebuffer::AttachRenderbuffer(const GLenum &attachmentType, const GLuint &renderBuffer)
{
    Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, renderBuffer);
    Unbind();
}

/**
 * @brief Checks if this framebuffer is complete or not
 * @return Returns true if this framebuffer is complete
 */
bool Framebuffer::IsComplete()
{
    Bind();
    bool ret = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    Unbind();
    return ret;
}

/**
 * @brief Binds this framebuffer
 */
void Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

/**
 * @brief Unbinds this framebuffer
 */
void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
