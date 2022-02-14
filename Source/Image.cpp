#include "Image.hpp"

#include <cassert>
#include <stb_image.h>

/**
 * @brief Constructor
 * @param[in] newWidth Image width
 * @param[in] newHeight Image height
 * @param[in] newNumChannels Number of channels in the image
 */
Image::Image(const uint32_t &newWidth, const uint32_t &newHeight, const uint32_t &newNumChannels)
    : width(newWidth)
    , height(newHeight)
    , numChannels(newNumChannels)
    , data()
{
    data.resize(width * height * numChannels, 0);
}

/**
 * @brief Destructor
 */
Image::~Image()
{
}

/**
 * @brief Creates an image from the specified file path
 * @param[in] imageFilePath Path to the image file
 * @return Image data loaded from the specified file path
 */
Image Image::LoadFromFile(const std::string &imageFilePath)
{
	stbi_set_flip_vertically_on_load(true);

    int width, height, numChannels;
	unsigned char* data = stbi_load
    (
        imageFilePath.c_str(), 
        &width,
        &height,
        &numChannels,
        0
    );

    Image ret(width, height, numChannels);
    size_t size = width * height * numChannels;
    assert(size == ret.data.size());
    for (size_t i = 0; i < size; ++i)
    {
        ret.data[i] = data[i];
    }

    return ret;
}

/**
 * @brief Sets the data at the specified image location
 * @param[in] x X-coordinate
 * @param[in] y Y-coordinate
 * @param[in] c0 Data for component 0
 */
void Image::SetData(const uint32_t &x, const uint32_t &y, const unsigned char &c0)
{
    uint32_t index = (y * width + x) * numChannels;
    for (uint32_t i = 0; i < numChannels; ++i)
    {
        data[index + i] = c0;
    }
}

/**
 * @brief Sets the data at the specified image location
 * @param[in] x X-coordinate
 * @param[in] y Y-coordinate
 * @param[in] c0 Data for component 0
 * @param[in] c1 Data for component 1
 */
void Image::SetData(const uint32_t &x, const uint32_t &y, const unsigned char &c0, const unsigned char &c1)
{
    if (numChannels > 0)
    {
        uint32_t index = (y * width + x) * numChannels;
        data[index] = c0;
        for (uint32_t i = 1; i < numChannels; ++i)
        {
            data[index + i] = c1;
        }
    }
}

/**
 * @brief Sets the data at the specified image location
 * @param[in] x X-coordinate
 * @param[in] y Y-coordinate
 * @param[in] c0 Data for component 0
 * @param[in] c1 Data for component 1
 * @param[in] c2 Data for component 2
 */
void Image::SetData(const uint32_t &x, const uint32_t &y, const unsigned char &c0, const unsigned char &c1, const unsigned char &c2)
{
    if (numChannels > 0)
    {
        uint32_t index = (y * width + x) * numChannels;
        data[index] = c0;
        data[index + 1] = c1;
        for (uint32_t i = 2; i < numChannels; ++i)
        {
            data[index + i] = c1;
        }
    }
}

/**
 * @brief Sets the data at the specified image location
 * @param[in] x X-coordinate
 * @param[in] y Y-coordinate
 * @param[in] c0 Data for component 0
 * @param[in] c1 Data for component 1
 * @param[in] c2 Data for component 2
 * @param[in] c3 Data for component 3
 */
void Image::SetData(const uint32_t &x, const uint32_t &y, const unsigned char &c0, const unsigned char &c1, const unsigned char &c2, const unsigned char &c3)
{
    if (numChannels > 0)
    {
        uint32_t index = (y * width + x) * numChannels;
        data[index] = c0;
        data[index + 1] = c1;
        data[index + 2] = c2;
        for (uint32_t i = 3; i < numChannels; ++i)
        {
            data[index + i] = c1;
        }
    }
}
