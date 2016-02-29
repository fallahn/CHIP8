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

#include <CHIP8/App.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>

#include <cassert>

App::App()
{

}

//public
void App::run()
{
    m_renderWindow.create({ 800, 600 }, "CHIP8");

    m_states.push_back(&m_chipEight);

    sf::Clock clock;
    sf::Event evt;
    while (m_renderWindow.isOpen())
    {
        //events
        while (m_renderWindow.pollEvent(evt))
        {
            switch (evt.type)
            {
            case sf::Event::Closed:
                m_renderWindow.close();
                break;
            case sf::Event::KeyReleased:
                switch (evt.key.code)
                {
                case sf::Keyboard::BackSpace:
                    //TODO push/pop menu state
                    break;
                case sf::Keyboard::Escape:
                    m_renderWindow.close();
                    break;
                }
                break;
            default:break;
            }

            //pass event down stack
            for (auto& state : m_states)
            {
                state->handleEvent(evt);
            }
        }

        update(clock.restart().asSeconds());
        draw();
    }

}


//private
void App::update(float dt)
{
    for (const auto pa : m_pendingActions)
    {
        switch (pa.action)
        {
        case State::Action::None:
        default: break;
        case State::Action::Pop:
            m_states.pop_back();
            break;
        case State::Action::Push:
            assert(pa.state);
            m_states.push_back(pa.state);
            break;
        }
    }
    m_pendingActions.clear();

    for (auto& state : m_states)
    {
        state->update(dt);
    }
}

void App::draw()
{
    m_renderWindow.clear();
    for (const auto& state : m_states)
    {
        state->draw(m_renderWindow);
    }
    m_renderWindow.display();
}