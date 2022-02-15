#include "Scenes/SandboxScene.hpp"

#include "Input.hpp"
#include "Utils/NoiseUtils.hpp"
#include "Window.hpp"
#include "WindowManager.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <iostream>

/**
 * @brief Constructor
 * @param[in] sceneManager Scene manager
 */
SandboxScene::SandboxScene(SceneManager *sceneManager)
    : BaseScene(sceneManager)
    , m_noise()
    , m_uiRenderer()
    , m_noiseImage(512, 512, 3)
    , m_noiseTexture()
    , m_noiseSeed(0)
    , m_noiseNumOctaves(1)
    , m_noisePersistence(1.0f)
    , m_noiseLacunarity(2.0f)
    , m_noiseScale(1.0f)
{
}

/**
 * @brief Destructor
 */
SandboxScene::~SandboxScene()
{
}

/**
 * @brief Initializes the scene.
 */
void SandboxScene::Init()
{
    m_uiRenderer.Initialize(1000);

    UpdateNoiseImage();
    m_noiseTexture.CreateFromImage(m_noiseImage);
    std::cout << "Octaves: " << m_noiseNumOctaves << " persistence: " << m_noisePersistence << " lacunarity: " << m_noiseLacunarity << std::endl;

    //m_noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_noise.SetSeed(m_noiseSeed);
}

/**
 * @brief Updates the scene.
 * @param[in] deltaTime Time elapsed since the previous frame
 */
void SandboxScene::Update(const float &deltaTime)
{
    bool updateNoise = false;

    if (Input::IsKeyPressed(Input::Key::A))
    {
        m_noisePersistence = std::max(m_noisePersistence - 0.1f, 0.0f);
        updateNoise = true;
    }
    else if (Input::IsKeyPressed(Input::Key::D))
    {
        m_noisePersistence = std::min(m_noisePersistence + 0.1f, 10.0f);
        updateNoise = true;
    }
    if (Input::IsKeyPressed(Input::Key::Q))
    {
        m_noiseLacunarity = std::max(m_noiseLacunarity - 0.1f, 0.0f);
        updateNoise = true;
    }
    else if (Input::IsKeyPressed(Input::Key::E))
    {
        m_noiseLacunarity = std::min(m_noiseLacunarity + 0.1f, 10.0f);
        updateNoise = true;
    }
    if (Input::IsKeyPressed(Input::Key::UP))
    {
        m_noiseScale = std::min(m_noiseScale + 0.1f, 10.0f);
        updateNoise = true;
    }
    else if (Input::IsKeyPressed(Input::Key::DOWN))
    {
        m_noiseScale = std::max(m_noiseScale - 0.1f, 0.1f);
        updateNoise = true;
    }
    if (Input::IsKeyPressed(Input::Key::LEFT))
    {
        m_noiseNumOctaves = std::max(m_noiseNumOctaves - 1, 1u);
        updateNoise = true;
    }
    else if (Input::IsKeyPressed(Input::Key::RIGHT))
    {
        m_noiseNumOctaves = std::min(m_noiseNumOctaves + 1, 8u);
        updateNoise = true;
    }
    
    if (Input::IsKeyPressed(Input::Key::SPACE))
    {
        ++m_noiseSeed;
        m_noise.SetSeed(m_noiseSeed);
        updateNoise = true;
    }

    if (updateNoise)
    {
        UpdateNoiseImage();
        m_noiseTexture.UpdateFromImageData(m_noiseImage);
        std::cout << "Octaves: " << m_noiseNumOctaves << " Scale: " << m_noiseScale << " Persistence: " << m_noisePersistence << " Lacunarity: " << m_noiseLacunarity << std::endl;
    }
}

/**
 * @brief Updates the scene with a fixed timestep.
 * @param[in] timestep Fixed timestep
 */
void SandboxScene::FixedUpdate(const float &timestep)
{
}

/**
 * @brief Draws the scene
 */
void SandboxScene::Draw()
{
    Window *mainWindow = WindowManager::GetMainWindow();

    m_uiRenderer.Begin();
    m_uiRenderer.DrawQuad
    (
        { 10.0f, 10.0f },
        { 512.0f, 512.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        0,
        0,
        m_noiseTexture.GetHandle(),
        { 0.0f, 0.0f, 1.0f, 1.0f }
    );
    m_uiRenderer.End();

    glm::mat4 projMatrix = glm::ortho(0.0f, mainWindow->GetWidth() * 1.0f, 0.0f, mainWindow->GetHeight() * 1.0f);
    m_uiRenderer.Render(projMatrix);
}

/**
 * @brief Cleans up the resources used by the scene.
 */
void SandboxScene::Cleanup()
{
}

/**
 * @brief Updates the image that visualizes the noise map
 */
void SandboxScene::UpdateNoiseImage()
{
    for (size_t x = 0; x < m_noiseImage.width; ++x)
    {
        for (size_t y = 0; y < m_noiseImage.height; ++y)
        {
            float noiseValue = NoiseUtils::GetOctaveNoise(m_noise, x, y, m_noiseNumOctaves, m_noiseScale, m_noisePersistence, m_noiseLacunarity);
            noiseValue = (noiseValue + 1.0f) / 2.0f;

            unsigned char red = 0, green = 0, blue = 0;
            if (noiseValue < 0.15f)
            {
                red = 0; green = 0; blue = 200;
            }
            else if (noiseValue < 0.3f)
            {
				red = 194; green = 178; blue = 128;
            }
            else if (noiseValue < 0.4f)
            {
                red = 0; green = 100; blue = 0;
            }
            else if (noiseValue < 0.5f)
            {
                red = 0; green = 125; blue = 0;
            }
            else if (noiseValue < 0.6f)
            {
                red = 0; green = 150; blue = 0;
            }
            else if (noiseValue < 0.7f)
            {
                red = 0; green = 175; blue = 0;
            }
            else if (noiseValue < 0.8f)
            {
                red = 0; green = 200; blue = 0;
            }
            else
            {
                red = 200; green = 200; blue = 200;
            }
            //red = green = blue = static_cast<unsigned char>(std::floor(255 * noiseValue));

            m_noiseImage.SetData(x, y, red, green, blue);
        }
    }
}
