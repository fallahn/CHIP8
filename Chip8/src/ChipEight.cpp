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
#include <CHIP8/TestRom.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cstring>
#include <fstream>
#include <random>
#include <iostream>

namespace
{
    //fonts are 4px wide by 5 px high
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

    std::default_random_engine rndEngine(static_cast<unsigned long>(std::time(0)));
    std::uniform_int_distribution<unsigned short> distribution(0, 0xFF);
    sf::Uint16 randNext() { return distribution(rndEngine); }

    //keyboard mapping - TODO make this configurable?
    const sf::Keyboard::Key key0x1 = sf::Keyboard::Key::Num1;
    const sf::Keyboard::Key key0x2 = sf::Keyboard::Key::Num2;
    const sf::Keyboard::Key key0x3 = sf::Keyboard::Key::Num3;
    const sf::Keyboard::Key key0xC = sf::Keyboard::Key::Num4;

    const sf::Keyboard::Key key0x4 = sf::Keyboard::Key::Q;
    const sf::Keyboard::Key key0x5 = sf::Keyboard::Key::W;
    const sf::Keyboard::Key key0x6 = sf::Keyboard::Key::E;
    const sf::Keyboard::Key key0xD = sf::Keyboard::Key::R;

    const sf::Keyboard::Key key0x7 = sf::Keyboard::Key::A;
    const sf::Keyboard::Key key0x8 = sf::Keyboard::Key::S;
    const sf::Keyboard::Key key0x9 = sf::Keyboard::Key::D;
    const sf::Keyboard::Key key0xE = sf::Keyboard::Key::F;

    const sf::Keyboard::Key key0xA = sf::Keyboard::Key::Z;
    const sf::Keyboard::Key key0x0 = sf::Keyboard::Key::X;
    const sf::Keyboard::Key key0xB = sf::Keyboard::Key::C;
    const sf::Keyboard::Key key0xF = sf::Keyboard::Key::V;
}

ChipEight::ChipEight()
    : m_indexRegister   (0u),
    m_programCounter    (0x200),
    m_delayTimer        (0u),
    m_soundTimer        (0u),
    m_stackPointer      (0u),
    m_inputMask         (0u)
{
    reset();
    loadFontset();
    initSound();

    //load in test rom
    std::cout << "Test ROM size: " << testRom.size() << std::endl;
    std::memcpy(&m_memory[0x200], testRom.data(), testRom.size());
}

//public
void ChipEight::handleEvent(const sf::Event& evt)
{
    if (evt.type == sf::Event::KeyPressed)
    {
        switch (evt.key.code)
        {
        default: break;
        case key0x0:
            m_inputMask |= (1 << 0x0);
            break;
        case key0x1:
            m_inputMask |= (1 << 0x1);
            break;
        case key0x2:
            m_inputMask |= (1 << 0x2);
            break;
        case key0x3:
            m_inputMask |= (1 << 0x3);
            break;
        case key0x4:
            m_inputMask |= (1 << 0x4);
            break;
        case key0x5:
            m_inputMask |= (1 << 0x5);
            break;
        case key0x6:
            m_inputMask |= (1 << 0x6);
            break;
        case key0x7:
            m_inputMask |= (1 << 0x7);
            break;
        case key0x8:
            m_inputMask |= (1 << 0x8);
            break;
        case key0x9:
            m_inputMask |= (1 << 0x9);
            break;
        case key0xA:
            m_inputMask |= (1 << 0xA);
            break;
        case key0xB:
            m_inputMask |= (1 << 0xB);
            break;
        case key0xC:
            m_inputMask |= (1 << 0xC);
            break;
        case key0xD:
            m_inputMask |= (1 << 0xD);
            break;
        case key0xE:
            m_inputMask |= (1 << 0xE);
            break;
        case key0xF:
            m_inputMask |= (1 << 0xF);
            break;
        }
    }
    else if (evt.type == sf::Event::KeyReleased)
    {
        switch (evt.key.code)
        {
        default: break;
        case key0x0:
            m_inputMask &= ~(1 << 0x0);
            break;
        case key0x1:
            m_inputMask &= ~(1 << 0x1);
            break;
        case key0x2:
            m_inputMask &= ~(1 << 0x2);
            break;
        case key0x3:
            m_inputMask &= ~(1 << 0x3);
            break;
        case key0x4:
            m_inputMask &= ~(1 << 0x4);
            break;
        case key0x5:
            m_inputMask &= ~(1 << 0x5);
            break;
        case key0x6:
            m_inputMask &= ~(1 << 0x6);
            break;
        case key0x7:
            m_inputMask &= ~(1 << 0x7);
            break;
        case key0x8:
            m_inputMask &= ~(1 << 0x8);
            break;
        case key0x9:
            m_inputMask &= ~(1 << 0x9);
            break;
        case key0xA:
            m_inputMask &= ~(1 << 0xA);
            break;
        case key0xB:
            m_inputMask &= ~(1 << 0xB);
            break;
        case key0xC:
            m_inputMask &= ~(1 << 0xC);
            break;
        case key0xD:
            m_inputMask &= ~(1 << 0xD);
            break;
        case key0xE:
            m_inputMask &= ~(1 << 0xE);
            break;
        case key0xF:
            m_inputMask &= ~(1 << 0xF);
            break;
        }
    }
}

void ChipEight::update(float dt)
{
    execute();    
    
    //we're using a fixed time step so counters can be decremented here
    static float timeAccumulator = 0.f;
    static const float timeStep = 1.f / 60.f;

    timeAccumulator += dt;
    while (timeAccumulator >= timeStep)
    {
        if (m_delayTimer > 0) m_delayTimer--;

        if (m_soundTimer > 0) m_soundTimer--;
        else m_beep.stop();
        
        timeAccumulator -= timeStep;
    }
}

void ChipEight::draw(sf::RenderTarget& rt) const
{
    rt.draw(m_screenData);
}

void ChipEight::load(const std::string& path)
{
    reset();

    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.fail() && file.good())
    {
        file.seekg(0, file.end);
        auto size = file.tellg();
        file.seekg(0, file.beg);

        if (size > 0 && size < 3583) //0xFFF - 0x200
        {
            file.read((char*)(&m_memory[m_programCounter]), size);
        }
        else
        {
            //TODO signal some kind of error
            std::cerr << "Failed reading file into RAM" << std::endl;
        }
    }
    else
    {
        //TODO signal some kind of error
        std::cerr << "Failed opening file: " << path << std::endl;
    }
    file.close();    
}

//private
void ChipEight::initSound()
{
    float frequency = 880.f;
    float updateRate = 22100.f;
    float amplitude = 16000.f;
    
    float stepCount = updateRate / frequency;
    float step = (6.284f) / stepCount;

    std::vector<sf::Int16> wavetable;
    for (float i = 0.f; i < stepCount; ++i)
    {
        wavetable.push_back(static_cast<sf::Int16>(std::sin(step * i) * amplitude));
    }

    m_soundBuffer.loadFromSamples(wavetable.data(), wavetable.size(), 1u, static_cast<sf::Uint32>(updateRate));

    m_beep.setBuffer(m_soundBuffer);
    m_beep.setLoop(true);
}

void ChipEight::reset()
{
    std::memset(m_memory.data() + fontset.size(), 0u, m_memory.size() - fontset.size());
    std::memset(m_registers.data(), 0u, m_registers.size());
    std::memset(m_stack.data(), 0u, m_stack.size() * sizeof(sf::Uint16));
    
    m_screenData.clear();

    m_delayTimer = 0u;
    m_soundTimer = 0u;
}

void ChipEight::loadFontset()
{
    for (auto i = 0u; i < fontset.size(); ++i)
    {
        m_memory[i] = fontset[i];
    }
}

void ChipEight::execute()
{
    //TODO replace mask values with consts

    Opcode currentOpcode = m_memory[m_programCounter] << 8 | m_memory[m_programCounter + 1];

    switch (currentOpcode & 0xF000)
    {
    default: break;
    //let's do these in order! :)
    case 0x0000:
        switch (currentOpcode & 0x00FF)
        {
        //0x00E0 - Clear Screen
        case 0x00E0:
            m_screenData.clear();
            m_programCounter += 2;
            break;
        //return from sub routine
        //restore the program counter from the stack
        case 0x00EE:
            m_stackPointer--;
            m_programCounter = m_stack[m_stackPointer];
            m_programCounter += 2;
            break;
        //0x00FB - SCHIP, scroll screen 4px right
        case 0x00FB:
            m_programCounter += 2;
            break;
        //0x00FC - SCHIP, scroll screen 4px left
        case 0x00FC:
            m_programCounter += 2;
            break;
        //0x00FD - SCHIP, exit
        case 0x00FD:

            break;
        //0x00FE - SCHIP, disable high res mode
        case 0x00FE:
            m_screenData.enableHires(false);
            m_programCounter += 2;
            break;
        //0x00FF - SCHIP, enable hi-res mode
        case 0x00FF:
            m_screenData.enableHires(true);
            m_programCounter += 2;
            break;
        default:
            if ((currentOpcode & 0x00F0) == 0x00C0)
            {
                //0x00CN - SCHIP, scroll scren down N lines
                m_programCounter += 2;
            }
            break;
        }
        break;
    //0x1NNN - jump to address
    case 0x1000:
        m_programCounter = currentOpcode & 0x0FFF;
        break;
    //0x2NNN - call sub routine
    case 0x2000:
        //store current position on stack so we can return to it
        m_stack[m_stackPointer++] = m_programCounter;
        m_programCounter = currentOpcode & 0xFFF;
        break;
    //0x3XNN skips next instruction if register X == NN
    case 0x3000:
        if (m_registers[(currentOpcode & 0x0F00) >> 8] == (currentOpcode & 0x00FF))
        {
            m_programCounter += 4;
        }
        else
        {
            m_programCounter += 2;
        }
        break;
    //0x4XNN skips next instruction if register X != NN
    case 0x4000:
        if (m_registers[(currentOpcode & 0x0F00) >> 8] != (currentOpcode & 0x00FF))
        {
            m_programCounter += 4;
        }
        else
        {
            m_programCounter += 2;
        }
        break;
    //0x5XY0 skips next instruction if register X == register Y
    case 0x5000:
        if (m_registers[(currentOpcode & 0x0F00) >> 8] == m_registers[(currentOpcode & 0x00F0) >> 4])
        {
            m_programCounter += 4;
        }
        else
        {
            m_programCounter += 2;
        }
        break;
    //0x6XNN - set register X to NN
    case 0x6000:
        m_registers[(currentOpcode & 0x0F00) >> 8] = currentOpcode & 0x00FF;
        m_programCounter += 2;
        break;
    //0x7XNN add NN to register X
    case 0x7000:
        m_registers[(currentOpcode & 0x0F00) >> 8] += currentOpcode & 0x00FF;
        m_programCounter += 2;
        break;

    case 0x8000:
        switch (currentOpcode & 0x000F)
        {
        default: break;
        //0x8XY0 - assign register Y to register X
        case 0x0000:
            m_registers[(currentOpcode & 0x0F00) >> 8] = m_registers[(currentOpcode & 0x00F0) >> 4];
            m_programCounter += 2;
            break;
        //0x8XY1 - sets register X to Register X OR'd with register Y
        case 0x0001:
            m_registers[(currentOpcode & 0x0F00) >> 8] |= m_registers[(currentOpcode & 0x00F0) >> 4];
            m_programCounter += 2;
            break;
        //0x8XY2 - sets register X to register X AND'd with register Y
        case 0x0002:
            m_registers[(currentOpcode & 0x0F00) >> 8] &= m_registers[(currentOpcode & 0x00F0) >> 4];
            m_programCounter += 2;
            break;
        //0x8XY3 - sets register X to register X XOR'd with register Y
        case 0x0003:
            m_registers[(currentOpcode & 0x0F00) >> 8] ^= m_registers[(currentOpcode & 0x00F0) >> 4];
            m_programCounter += 2;
            break;
        //0x8XY4 - adds register Y to register X. Register 15 is carry flag
        case 0x0004:
        {
            auto x = (currentOpcode & 0x0F00) >> 8;
            auto y = (currentOpcode & 0x00F0) >> 4;

            if (m_registers[y] > m_registers[x])
            {
                m_registers[15] = 1; //carry
            }
            else
            {
                m_registers[15] = 0;
            }
            m_registers[x] += m_registers[y];
            m_programCounter += 2;
        }
            break;
        //0x8XY5 - subtracts register Y for register X. Register 15 is set to 0 when borrowing
        case 0x0005:
        {
            auto x = (currentOpcode & 0x0F00) >> 8;
            auto y = (currentOpcode & 0x00F0) >> 4;

            if (m_registers[y] > m_registers[x])
            {
                //borrow
                m_registers[15] = 0;
            }
            else
            {
                m_registers[15] = 1;
            }
            m_registers[x] -= m_registers[y];
            m_programCounter += 2;
        }
            break;
        //0x8X06 - right shifts register X, Register 15 holds LSB before shift
        case 0x0006:
        {
            auto x = (currentOpcode & 0x0F00) >> 8;
            m_registers[15] = m_registers[x] & 0x1;
            m_registers[x] >>= 1;
            m_programCounter += 2;
        }
            break;
        //0x8XY7 - set register X to register Y - register X. Register 15 is set to 0 when borrowing
        case 0x0007:
        {
            auto x = (currentOpcode & 0x0F00) >> 8;
            auto y = (currentOpcode & 0x00F0) >> 4;

            if (m_registers[x] > m_registers[y])
            {
                //borrow
                m_registers[15] = 0;
            }
            else
            {
                m_registers[15] = 1;
            }
            m_registers[x] = m_registers[y] - m_registers[x];
            m_programCounter += 2;
        }
            break;
        //0x8X0E - shifts register X left by one. Register 15 is MSB of register X  before shift
        case 0x000E:
        {
            auto x = (currentOpcode & 0x0F00) >> 8;
            m_registers[15] = m_registers[x] >> 7;
            m_registers[x] <<= 1;
            m_programCounter += 2;
        }
            break;
        }
        break;

    //0x9XY0 - skips next instruction of register X != register Y
    case 0x9000:
        if (m_registers[(currentOpcode & 0x0F00) >> 8] != m_registers[(currentOpcode & 0x00F0) >> 4])
        {
            m_programCounter += 4;
        }
        else
        {
            m_programCounter += 2;
        }
        break;

    //0xANNN - sets I register to address NNN
    case 0xA000:
        m_indexRegister = currentOpcode & 0x0FFF;
        m_programCounter += 2;
        break;
    //0xB000 - jumps to address NNN + register 0
    case 0xB000:
        m_programCounter = (currentOpcode & 0x0FFF) + m_registers[0];
        break;
    //case 0xCXNN - sets register X to random number AND'd with NN
    case 0xC000:
        m_registers[(currentOpcode & 0x0F00) >> 8] = randNext() & (currentOpcode & 0x00FF);
        m_programCounter += 2;
        break;
    //case 0xDXYN - draws sprite at register X, register Y with height N
    //each row of 8 pixels is read starting at address in register I
    //until I + N. Register 15 is set to 1 if any pixels are unset
    case 0xD000:
        switch(currentOpcode & 0x000F)
        {
            case 0x000:
                //SCHIP draw 16x16 sprite
                m_programCounter += 2;
                break;
            default: //CHIP-8

            auto x = m_registers[(currentOpcode & 0x0F00) >> 8];
            auto y = m_registers[(currentOpcode & 0x00F0) >> 4];
            auto height = (currentOpcode & 0x000F);
            sf::Uint16 pixel = 0;

            m_registers[15] = 0;

            for (auto row = 0; row < height; ++row)
            {
                pixel = m_memory[m_indexRegister + row];
                for (auto col = 0; col < 8; ++col)
                {
                    //if the bit for this pixel is set...
                    if ((pixel & (0x80 >> col)) != 0)
                    {
                        auto pxIndex = ((y + row) * m_screenData.getHorizontalPixelCount()) + (col + x);
                        if (m_screenData[pxIndex] == 1)
                        {
                            m_registers[15] = 1;
                        }
                        m_screenData.setPixel(pxIndex, m_screenData[pxIndex] ^ 1);
                    }
                }
            }
            m_programCounter += 2;
        }
        break;
    case 0xE000:
        switch (currentOpcode & 0x00FF)
        {
        default: break;
        //0xEX9E skips next instruction if key stored in register X is pressed
        case 0x009E:
            if ((m_inputMask & (1 << m_registers[(currentOpcode & 0x0F00) >> 8])) != 0)
            {
                m_programCounter += 4;
                break;
            }
            m_programCounter += 2;
            break;
        //0xEXA1 - skips the next instruction of the key  stored in register X isn't pressed
        case 0x00A1:
            if ((m_inputMask & (1 << m_registers[(currentOpcode & 0x0F00) >> 8])) == 0)
            {
                m_programCounter += 4;
                break;
            }
            m_programCounter += 2;
            break;
        }
        break;
    case 0xF000:
        switch (currentOpcode & 0x00FF)
        {
        default: break;
        //0xFX07 - set register X to value of delay timer
        case 0x0007:
            m_registers[(currentOpcode & 0x0F00) >> 8] = m_delayTimer;
            m_programCounter += 2;
            break;
        //0xFX0A wait for keypress and store it when received (blocking!!)
        case 0x000A:
        {
            for (auto i = 0; i < 16; ++i)
            {
                if (m_inputMask & (1 << i))
                {
                    m_registers[(currentOpcode & 0x0F00) >> 8] = i;
                    m_programCounter += 2;
                    return;
                }
            }
        }           
            break;
        //0xFX15 - sets delay timer to register X
        case 0x0015:
            m_delayTimer = m_registers[(currentOpcode & 0x0F00) >> 8];
            m_programCounter += 2;
            break;
        //0xFX18 - sets the sound  timer to register X
        case 0x0018:
            m_soundTimer = m_registers[(currentOpcode & 0x0F00) >> 8];
            if (m_soundTimer > 0) m_beep.play();
            m_programCounter += 2;
            break;
        //0xFX1E - adds register X to I
        case 0x001E:
        {
            auto x = (currentOpcode & 0x0F00) >> 8;
            if (m_indexRegister + m_registers[x] > 0xFFF)
            {
                //overflow
                m_registers[15] = 1;
            }
            else
            {
                m_registers[15] = 0;
            }
            m_indexRegister += m_registers[x];
            m_programCounter += 2;
        }
            break;
        //0xFX29 - sets I to the location of the font sprite for the value in register X
        case 0x0029:
            m_indexRegister = m_registers[(currentOpcode & 0x0F00) >> 8] * 0x5;
            m_programCounter += 2;
            break;
        //0xFX30 - SCHIP. As FX29 but for 10px font
        case 0x0030:
            m_programCounter += 2;
            break;
        //0xFX33 - stores the decimal representation of register X at I to I + 2
        case 0x0033:
        {
            auto x = (currentOpcode & 0x0F00) >> 8;
            m_memory[m_indexRegister] = m_registers[x] / 100;
            m_memory[m_indexRegister + 1] = (m_registers[x] / 10) % 10;
            m_memory[m_indexRegister + 2] = (m_registers[x] % 100) % 10;
            m_programCounter += 2;
        }
            break;
        //0xFX55 stores registers 0 - X in memory starting at I
        case 0x0055:
        {
            auto x = (currentOpcode & 0x0F00) >> 8;
            for (auto i = 0; i <= x; ++i)
            {
                m_memory[m_indexRegister + i] = m_registers[i];
            }

            //move I on to next operational address
            m_indexRegister += x + 1;
            m_programCounter += 2;
        }
            break;
        //0xFX65 - set registers 0 - X with values starting at I
        case 0x0065:
        {
            auto x = (currentOpcode & 0x0F00) >> 8;
            for (auto i = 0; i <= x; ++i)
            {
                m_registers[i] = m_memory[m_indexRegister + i];
            }

            m_indexRegister += x + 1;
            m_programCounter += 2;
        }
            break;
        //0xFX75 - SCHIP as 0xFX55, but unclear what R is?
        case 0x0075:
            m_programCounter += 2;
            break;
        //0xFx85 - SCHIP as 0xFX65, but unclear what R is?
            m_programCounter += 2;
            break;
        }
        break;
    }
}