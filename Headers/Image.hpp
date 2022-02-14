#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * Struct containing data about an image
 */
struct Image
{
    /**
     * Image width
     */
    uint32_t width;

    /**
     * Image height
     */
    uint32_t height;

    /**
     * Number of channels in the image
     */
    uint32_t numChannels;

    /**
     * Image data
     */
    std::vector<unsigned char> data;

    /**
     * @brief Constructor
     * @param[in] newWidth Image width
     * @param[in] newHeight Image height
     * @param[in] newNumChannels Number of channels in the image
     */
    Image(const uint32_t &newWidth, const uint32_t &newHeight, const uint32_t &newNumChannels);

    /**
     * @brief Destructor
     */
    ~Image();

    /**
     * @brief Creates an image from the specified file path
     * @param[in] imageFilePath Path to the image file
     * @return Image data loaded from the specified file path
     */
    static Image LoadFromFile(const std::string &imageFilePath);

    /**
     * @brief Sets the data at the specified image location
     * @param[in] x X-coordinate
     * @param[in] y Y-coordinate
     * @param[in] c0 Data for component 0
     */
    void SetData(const uint32_t &x, const uint32_t &y, const unsigned char &c0);

    /**
     * @brief Sets the data at the specified image location
     * @param[in] x X-coordinate
     * @param[in] y Y-coordinate
     * @param[in] c0 Data for component 0
     * @param[in] c1 Data for component 1
     */
    void SetData(const uint32_t &x, const uint32_t &y, const unsigned char &c0, const unsigned char &c1);

    /**
     * @brief Sets the data at the specified image location
     * @param[in] x X-coordinate
     * @param[in] y Y-coordinate
     * @param[in] c0 Data for component 0
     * @param[in] c1 Data for component 1
     * @param[in] c2 Data for component 2
     */
    void SetData(const uint32_t &x, const uint32_t &y, const unsigned char &c0, const unsigned char &c1, const unsigned char &c2);

    /**
     * @brief Sets the data at the specified image location
     * @param[in] x X-coordinate
     * @param[in] y Y-coordinate
     * @param[in] c0 Data for component 0
     * @param[in] c1 Data for component 1
     * @param[in] c2 Data for component 2
     * @param[in] c3 Data for component 3
     */
    void SetData(const uint32_t &x, const uint32_t &y, const unsigned char &c0, const unsigned char &c1, const unsigned char &c2, const unsigned char &c3);
};
