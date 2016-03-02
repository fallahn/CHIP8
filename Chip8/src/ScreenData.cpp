/*********************************************************************
Matt Marchant 2016
http://trederia.blogspot.com

CHIP8 - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#include <CHIP8/ScreenData.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cassert>
#include <cstring>

namespace
{
    const sf::Uint8 width = 64u;
    const sf::Uint8 height = 32u;
}

ScreenData::ScreenData(sf::Uint8 pixelSize)
    : m_resolution          (width * pixelSize, height * pixelSize),
    m_pixelData             (m_pixelDataLow.data()),
    m_pixelDataSize         (m_pixelDataLow.size()),
    m_vertexArray           (m_vertexArrayLow.data()),
    m_vertexArraySize       (m_vertexArrayLow.size()),
    m_horizontalPixelCount  (64)
{
    //init low res arrays
    std::memset(m_pixelDataLow.data(), 0, m_pixelDataLow.size());

    for (auto y = 0u; y < height; ++y)
    {
        for (auto x = 0u; x < width; ++x)
        {
            auto idx = (y * width + x) * 4;
            sf::Vertex vertex;
            vertex.color = sf::Color::Black;

            vertex.position = sf::Vector2f(static_cast<float>(x * pixelSize), static_cast<float>(y * pixelSize));
            m_vertexArrayLow[idx++] = vertex;
            vertex.position.x += pixelSize;
            m_vertexArrayLow[idx++] = vertex;
            vertex.position.y += pixelSize;
            m_vertexArrayLow[idx++] = vertex;
            vertex.position.x -= pixelSize;
            m_vertexArrayLow[idx++] = vertex;
        }
    }

    //init high res arrays
    std::memset(m_pixelDataHigh.data(), 0, m_pixelDataHigh.size());

    sf::Uint8 width2 = width * 2;
    sf::Uint8 height2 = height * 2;
    float pixelSize2 = pixelSize / 2.f;

    for (auto y = 0u; y < height2; ++y)
    {
        for (auto x = 0u; x < width2; ++x)
        {
            auto idx = (y * width2 + x) * 4;
            sf::Vertex vertex;
            vertex.color = sf::Color::Black;

            vertex.position = sf::Vector2f(pixelSize2 * x, pixelSize2 * y);
            m_vertexArrayHigh[idx++] = vertex;
            vertex.position.x += pixelSize2;
            m_vertexArrayHigh[idx++] = vertex;
            vertex.position.y += pixelSize2;
            m_vertexArrayHigh[idx++] = vertex;
            vertex.position.x -= pixelSize2;
            m_vertexArrayHigh[idx++] = vertex;
        }
    }
}

//public
void ScreenData::setPixel(std::size_t idx, sf::Uint8 value)
{
    idx = std::min(idx, m_pixelDataSize - 1);
    
    assert(value < 2);
    m_pixelData[idx] = value;

    value *= 255u;
    idx *= 4u;

    sf::Color colour(value, value, value);
    m_vertexArray[idx++].color = colour;
    m_vertexArray[idx++].color = colour;
    m_vertexArray[idx++].color = colour;
    m_vertexArray[idx++].color = colour;
}

void ScreenData::enableHires(bool enable)
{
    if (enable)
    {
        if (m_pixelData == m_pixelDataHigh.data()) return; //already set

        m_pixelData = m_pixelDataHigh.data();
        m_pixelDataSize = m_pixelDataHigh.size();
        m_vertexArray = m_vertexArrayHigh.data();
        m_vertexArraySize = m_vertexArrayHigh.size();

        m_horizontalPixelCount = 128;
    }
    else
    {
        if (m_pixelData = m_pixelDataLow.data()) return; //already set

        m_pixelData = m_pixelDataLow.data();
        m_pixelDataSize = m_pixelDataLow.size();
        m_vertexArray = m_vertexArrayLow.data();
        m_vertexArraySize = m_vertexArrayLow.size();

        m_horizontalPixelCount = 64;
    }
    clear();
}

void ScreenData::clear()
{
    const auto pixelCount = width * height;
    for (auto i = 0; i < pixelCount; ++i)
    {
        setPixel(i, 0);
    }
}

//private
void ScreenData::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    rt.draw(m_vertexArray, m_vertexArraySize, sf::Quads, states);
}