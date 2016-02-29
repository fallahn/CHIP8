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

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Desktop.hpp>

int main()
{
    sf::RenderWindow renderWindow({ 800, 600 }, "CHIP8");
    sfg::SFGUI sfg;
    auto button = sfg::Button::Create("buns");
    auto window = sfg::Window::Create();
    window->Add(button);
    sfg::Desktop desktop;
    desktop.Add(window);

    while (renderWindow.isOpen())
    {
        sf::Event evt;
        while (renderWindow.pollEvent(evt))
        {
            desktop.HandleEvent(evt);
            if (evt.type == sf::Event::Closed)
            {
                renderWindow.close();
            }
        }
        desktop.Update(1.f);

        renderWindow.clear();
        sfg.Display(renderWindow);
        renderWindow.display();
    }

    return 0;
}