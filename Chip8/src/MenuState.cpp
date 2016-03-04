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

#include <CHIP8/MenuState.hpp>
#include <CHIP8/ChipEight.hpp>
#include <CHIP8/FileSystem.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <SFGUI/Window.hpp>
#include <SFGUI/ComboBox.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/CheckButton.hpp>

MenuState::MenuState(sf::RenderWindow& rw, ChipEight& ce)
    : m_renderWindow    (rw),
    m_chipEight         (ce)
{
    auto window = sfg::Window::Create(sfg::Window::BACKGROUND);
    window->SetPosition({ 0.f, 320.f });
    window->SetRequisition({ 640.f, 160.f });

    auto mainbox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 4.f);

    auto leftbox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 6.f);
    leftbox->SetAllocation({ 0.f, 0.f, 200.f, 30.f });

    auto gameList = sfg::ComboBox::Create();
    gameList->AppendItem("No games in 'roms' directory...");

    auto gameNames = listFiles("roms");
    if (!gameNames.empty()) gameList->RemoveItem(0);
    for (const auto& str : gameNames)
    {
        gameList->AppendItem(str);
    }
    gameList->SelectItem(0);
    leftbox->Pack(gameList, false, true);

    auto vsync = sfg::CheckButton::Create("V-Sync");
    vsync->SetActive(true);
    leftbox->Pack(vsync, false);

    auto limiterBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 6.f);
    auto limiterCombo = sfg::ComboBox::Create();
    limiterCombo->AppendItem("120");
    limiterCombo->AppendItem("240");
    limiterCombo->AppendItem("480");
    limiterCombo->AppendItem("640");
    limiterCombo->AppendItem("800");
    limiterCombo->AppendItem("1000");
    limiterCombo->SelectItem(1);

    auto frameLimiter = sfg::CheckButton::Create("Limit Frame Rate");
    frameLimiter->GetSignal(sfg::CheckButton::OnToggle).Connect
    (
        [this, frameLimiter, vsync, limiterCombo]()
        {
            vsync->SetActive(false);
            m_renderWindow.setFramerateLimit(frameLimiter->IsActive() ? std::atoi(limiterCombo->GetSelectedText().toAnsiString().c_str()) : 0);
        }
    );
    limiterBox->Pack(frameLimiter, false);
    limiterBox->Pack(limiterCombo, false);

    leftbox->Pack(limiterBox, false);

    vsync->GetSignal(sfg::CheckButton::OnToggle).Connect
    (
        [this, vsync, frameLimiter]()
        {
            m_renderWindow.setVerticalSyncEnabled(vsync->IsActive());
            frameLimiter->SetActive(false);
        }
    );


    mainbox->Pack(leftbox);

    auto rightbox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
    rightbox->SetAllocation({ 0.f, 0.f, 160.f, 28.f }); //why? this does fuck all

    auto loadButton = sfg::Button::Create("Load");
    loadButton->SetAllocation({ 0.f, 0.f, 160.f, 28.f });
    loadButton->GetSignal(sfg::Button::OnLeftClick).Connect
    (
        [this, gameList]()
        {
            auto str = gameList->GetSelectedText().toAnsiString();
            m_chipEight.load("roms/" + str);
        }
    );

    rightbox->Pack(loadButton, false, true);

    mainbox->Pack(rightbox, false);
    window->Add(mainbox);

    m_desktop.Add(window);
}

//public
void MenuState::handleEvent(const sf::Event& evt)
{
    m_desktop.HandleEvent(evt);
}

void MenuState::update(float dt)
{
    m_desktop.Update(dt);
}

void MenuState::draw(sf::RenderTarget& rt)
{
    m_sfg.Display(m_renderWindow);
}