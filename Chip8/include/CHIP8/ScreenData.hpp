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

    const sf::Vector2u& getResolution() const { return m_resolution; }
    void clear();

private:

    sf::Vector2u m_resolution; //< the actual resolution in SFML units

    std::array<sf::Uint8, 2048u> m_pixelData; //<64 *  32 pixels, either on or off
    std::array<sf::Vertex, 8192u> m_vertexArray; //<64 * 32 * 4 verts
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

#endif //CH_SCREEN_DATA_HPP_
