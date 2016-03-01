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

//test program loaded on startup

#ifndef CH_TEST_ROM_HPP_
#define CH_TEST_ROM_HPP_

#include <SFML/Config.hpp>

#include <array>

static const sf::Uint8 testRom[] = //TODO convert to std::array
{
    0x12, 0x39,                   //skip charset and start program (remember here we're @ 0x200)
    //charset
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0x90, 0x90, 0xF0, 0x90, 0x90, //H
    0xE0, 0x40, 0x40, 0x40, 0xE0, //I
    0x90, 0x50, 0x60, 0x50, 0x90, //K
    0xF0, 0x90, 0x90, 0x90, 0xF0, //O
    0xF0, 0x90, 0xF0, 0x80, 0x80, //P
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //S
    0xF0, 0x40, 0x40, 0x40, 0x40, //T
    0x40, 0x40, 0x40, 0x00, 0x40, //!
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    //program begin
    0xA2, 0x02,                   //set I reg to 0x202
    0x60, 0x01,                   //set V0 to X coord
    0x61, 0x01,                   //set V1 to Y coord
    0xD0, 0x15,                   //draw V0, V1 for 5 bytes
    0xA2, 0x0C,                   //set I reg to 0x20C
    0x60, 0x06,                   //set V0 to X coord
    0xD0, 0x15,                   //draw V0, V1 for 5 bytes
    0xA2, 0x11,                   //set I reg to 0x211
    0x60, 0x0C,                   //set V0 to X coord
    0xD0, 0x15,                   //draw V0, V1 for 5 bytes
    0xA2, 0x20,                   //set I reg to 0x220
    0x60, 0x11,                   //set V0 to X coord
    0xD0, 0x15,                   //draw V0, V1 for 5 bytes
    0xA2, 0x34,                   //set I reg to 0x234
    0x60, 0x17,                   //set V0 to X coord
    0xD0, 0x15,                   //draw V0, V1 for 5 bytes
    0x12, 0x39                    //loop back to draw (infinitely)
};

//CHIP8 TEST OK!


#endif //CH_TEST_ROM_HPP_