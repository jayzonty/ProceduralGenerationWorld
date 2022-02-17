#pragma once

#include <glad/glad.h>

/**
 * Framebuffer
 */
class Framebuffer
{
private:
    /**
     * FBO handle
     */
    GLuint m_fbo;

public:
    /**
     * @brief Constructor
     */
    Framebuffer();

    /**
     * @brief Destructor
     */
    ~Framebuffer();

    /**
     * @brief Initializes this framebuffer
     */
    void Initialize();

    /**
     * @brief Cleans up this framebuffer
     */
    void Cleanup();

    /**
     * @brief Attaches a texture to the framebuffer
     * @param[in] attachmentType Attachment type
     * @param[in] texture Texture
     */
    void AttachTexture2D(const GLenum &attachmentType, const GLuint &texture);

    /**
     * @brief Attaches a renderbuffer to the framebuffer
     * @param[in] attachmentType Attachment type
     * @param[in] renderBuffer Renderbuffer
     */
    void AttachRenderbuffer(const GLenum &attachmentType, const GLuint &renderBuffer);

    /**
     * @brief Checks if this framebuffer is complete or not
     * @return Returns true if this framebuffer is complete
     */
    bool IsComplete();

    /**
     * @brief Binds this framebuffer
     */
    void Bind();

    /**
     * @brief Unbinds this framebuffer
     */
    void Unbind();
};
