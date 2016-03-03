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

//simulates an array of pixels which can be drawn with SFML
//chip-8 uses a resolution of 64 * 32 pixels
//super chip extended mode allows 128 * 64 pixels

#ifndef CH_SCREEN_DATA_HPP_
#define CH_SCREEN_DATA_HPP_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <array>

class ScreenData final : public sf::Drawable
{
public:
    explicit ScreenData(sf::Uint8 pixelSize = 10);
    ~ScreenData() = default;
    ScreenData(const ScreenData&) = delete;
    ScreenData& operator = (const ScreenData&) = delete;

    const sf::Uint8& operator [] (std::size_t idx) const { return m_pixelData[idx]; }

    void setPixel(std::size_t, sf::Uint8);
    void enableHires(bool);
    sf::Uint8 getHorizontalPixelCount() const { return m_horizontalPixelCount; }
    const sf::Vector2u& getResolution() const { return m_resolution; }
    
    void scrollLeft();
    void scrollRight();
    void scrollDown(sf::Uint8);

    void clear();

private:

    sf::Vector2u m_resolution; //< the actual resolution in SFML units

    std::array<sf::Uint8, 2048u> m_pixelDataLow; //<64 *  32 pixels, either on or off
    std::array<sf::Vertex, 8192u> m_vertexArrayLow; //<64 * 32 * 4 verts

    std::array<sf::Uint8, 8192u> m_pixelDataHigh; //<128 * 64 pixels
    std::array<sf::Vertex, 32768> m_vertexArrayHigh; //<128 * 64 * 4 verts

    sf::Uint8* m_pixelData;
    std::size_t m_pixelDataSize;
    sf::Vertex* m_vertexArray;
    std::size_t m_vertexArraySize;
    sf::Uint8 m_horizontalPixelCount;

    void updateVertices();
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

#endif //CH_SCREEN_DATA_HPP_
