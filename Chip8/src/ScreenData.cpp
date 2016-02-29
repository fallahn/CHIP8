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
    : m_resolution(width * pixelSize, height * pixelSize)
{
    std::memset(m_pixelData.data(), 0, m_pixelData.size());

    for (auto y = 0u; y < height; ++y)
    {
        for (auto x = 0u; x < width; ++x)
        {
            auto idx = (y * width + x) * 4;
            sf::Vertex vertex;
            vertex.color = sf::Color::Black;

            vertex.position = sf::Vector2f(x * pixelSize, y * pixelSize);
            m_vertexArray[idx++] = vertex;
            vertex.position.x += pixelSize;
            m_vertexArray[idx++] = vertex;
            vertex.position.y += pixelSize;
            m_vertexArray[idx++] = vertex;
            vertex.position.x -= pixelSize;
            m_vertexArray[idx++] = vertex;
        }
    }
}

//public
void ScreenData::setPixel(std::size_t idx, sf::Uint8 value)
{
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
    rt.draw(m_vertexArray.data(), m_vertexArray.size(), sf::Quads, states);
}