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

/*
The CHIP-8 interpreter.

Memory map:

0x000 - 0x1FF: Interpreter (contains emulator font)
0x050 - 0x0A0: Built in 4x5 pixel font (chars A-F)
0x200 - 0xFFF: Program ROM and working RAM
*/

#ifndef CH_CH8_STATE_HPP_
#define CH_CH8_STATE_HPP_

#include <CHIP8/State.hpp>
#include <CHIP8/ScreenData.hpp>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <string>

class ChipEight final : public State
{
public:
    ChipEight();
    ~ChipEight() = default;

    ChipEight(const ChipEight&) = delete;
    ChipEight& operator = (const ChipEight&) = delete;

    void handleEvent(const sf::Event&) override;
    void update(float) override;
    void draw(sf::RenderTarget&) override;

    void load(const std::string&);

private:
    using Opcode = sf::Uint16;

    std::array<sf::Uint8, 4096u> m_memory;
    std::array<sf::Uint8, 16u> m_registers; //< V registers 0 - F. F is used for carry flag, others are general purpose
    std::array<sf::Uint8, 16u> m_superRegisters; //< used by some superchip programs
    sf::Uint16 m_indexRegister;
    sf::Uint16 m_programCounter;
    
    sf::Uint8 m_delayTimer;
    sf::Uint8 m_soundTimer;

    std::array<sf::Uint16, 16u> m_stack;
    sf::Uint16 m_stackPointer;

    sf::Uint16 m_inputMask;

    ScreenData m_screenData;

    sf::SoundBuffer m_soundBuffer;
    sf::Sound m_beep;
    void initSound();

    void reset();
    void loadFontset();

    void execute();
};

#endif //CH_CH8_STATE_HPP_