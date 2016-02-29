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

#include <CHIP8/ChipEight.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cstring>

namespace
{
    //fonts are 8px wide by 5 px high
    //each byte represents one row in the char
    const std::array<sf::Uint8, 80u> fontset =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, //0
        0x20, 0x60, 0x20, 0x20, 0x70, //1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
        0x90, 0x90, 0xF0, 0x10, 0x10, //4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
        0xF0, 0x10, 0x20, 0x40, 0x40, //7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
        0xF0, 0x90, 0xF0, 0x90, 0x90, //A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
        0xF0, 0x80, 0x80, 0x80, 0xF0, //C
        0xE0, 0x90, 0x90, 0x90, 0xE0, //D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
        0xF0, 0x80, 0xF0, 0x80, 0x80  //F
    };
}

ChipEight::ChipEight()
    : m_currentOpcode   (0u),
    m_indexRegister     (0u),
    m_programCounter    (0x200),
    m_delayTimer        (0u),
    m_soundTimer        (0u),
    m_stackPointer      (0u)
{
    reset();
    loadFontset();
}

//public
void ChipEight::handleEvent(const sf::Event& evt)
{

}

void ChipEight::update(float dt)
{
    //we're using a fixed time step so counters can be decremented here
    m_delayTimer--;
    m_soundTimer--;
}

void ChipEight::draw(sf::RenderTarget& rt) const
{
    rt.draw(m_screenData);
}

void ChipEight::reset()
{
    std::memset(m_memory.data(), 0u, m_memory.size());
    std::memset(m_registers.data(), 0u, m_registers.size());
    std::memset(m_stack.data(), 0u, m_stack.size() * sizeof(sf::Uint16));
    
    m_screenData.clear();

    m_delayTimer = 0u;
    m_soundTimer = 0u;
}

//private
void ChipEight::loadFontset()
{
    for (auto i = 0u; i < fontset.size(); ++i)
    {
        m_memory[i] = fontset[i];
    }
}