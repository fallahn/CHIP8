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

#ifndef CH_MENU_STATE_HPP_
#define CH_MENU_STATE_HPP_

#include <CHIP8/State.hpp>

#include <SFGUI/Desktop.hpp>
#include <SFGUI/SFGUI.hpp>

class ChipEight;
class MenuState final : public State
{
public:
    MenuState(sf::RenderWindow&, ChipEight&);
    ~MenuState() = default;

    void handleEvent(const sf::Event&) override;
    void update(float) override;
    void draw(sf::RenderTarget&) override;

private:
    sf::RenderWindow& m_renderWindow;
    ChipEight& m_chipEight;

    sfg::SFGUI m_sfg;
    sfg::Desktop m_desktop;
};

#endif //CH_MENU_STATE_HPP_