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

#include <vector>
//TODO this should check as many opcodes as possible
//TODO display a pixel for each test passed

//V0, V1 sprite coords
//V2 timer counter
//V3 loop counter
//V4 hires state

static const std::vector<sf::Uint8> testRom = //TODO convert to std::array
{
    0x12, 0x48,                   //0x200 skip charset / subs and start program
    //charset
    0x70, 0x80, 0x80, 0x80, 0x70, //0x202 C
    0xF0, 0x80, 0xE0, 0x80, 0xF0, //0x207 E
    0x90, 0x90, 0xF0, 0x90, 0x90, //0x20C H
    0xE0, 0x40, 0x40, 0x40, 0xE0, //0x211 I
    0x90, 0xA0, 0xC0, 0xA0, 0x90, //0x216 K
    0x60, 0x90, 0x90, 0x90, 0x60, //0x21B O
    0x60, 0x90, 0xE0, 0x80, 0x80, //0x220 P
    0x70, 0x80, 0xF0, 0x10, 0xE0, //0x225 S
    0xE0, 0x40, 0x40, 0x40, 0x40, //0x22A T
    0x40, 0x40, 0x40, 0x00, 0x40, //0x22F !
    0x60, 0x90, 0x60, 0x90, 0x60, //0x234 8
    0x00,                         //0x239 pad first instruction to even number address
    
    //draw then delay loop (subroutine)
    0x62, 0x07,                   //0x23A set V2 to 7
    0xF2, 0x15,                   //0x23C set timer to V2
    0xD0, 0x15,                   //0x23E draw V0, V1 for 5 bytes
    0x42, 0x00,                   //0x240 skip next if V2 not 0
    0x00, 0xEE,                   //0x242 quit subroutine
    0xF2, 0x07,                   //0x244 copy timer to V2
    0x12, 0x40,                   //0x246 goto skip check
    
    //program begin
    0xA2, 0x02,                   //0x248 set I reg to C
    0x60, 0x01,                   //0x24A set V0 to X coord
    0x61, 0x01,                   //0x24C set V1 to Y coord
    0x22, 0x3A,                   //0x24E goto draw/delay sub
    0xA2, 0x0C,                   //0x250 set I reg to H
    0x60, 0x06,                   //0x252 set V0 to X coord
    0x22, 0x3A,                   //0x254 goto draw/delay sub
    0xA2, 0x11,                   //0x256 set I reg to I
    0x60, 0x0B,                   //0x258 set V0 to X coord
    0x22, 0x3A,                   //0x25A goto draw/delay sub
    0xA2, 0x20,                   //0x25C set I reg to P
    0x60, 0x0F,                   //0x25E set V0 to X coord
    0x22, 0x3A,                   //0x260 goto draw/delay sub
    0xA2, 0x34,                   //0x262 set I reg to 8
    0x60, 0x17,                   //0x264 set V0 to X coord
    0x22, 0x3A,                   //0x266 goto draw/delay sub
    0xA2, 0x2A,                   //0x268 set I reg to T
    0x60, 0x01,                   //0x26A set V0 to X coord
    0x61, 0x07,                   //0x26C set V1 to Y coord
    0x22, 0x3A,                   //0x26E goto draw/delay sub
    0xA2, 0x07,                   //0x270 set I reg to E
    0x60, 0x05,                   //0x272 set V0 to X coord
    0x22, 0x3A,                   //0x274 goto draw/delay sub
    0xA2, 0x25,                   //0x276 set I reg to S
    0x60, 0x0A,                   //0x278 set V0 to X coord
    0x22, 0x3A,                   //0x27A goto draw/delay sub
    0xA2, 0x2A,                   //0x27C set I reg to T
    0x60, 0x0F,                   //0x27E set V0 to X coord
    0x22, 0x3A,                   //0x280 goto draw/delay sub
    0xA2, 0x1B,                   //0x282 set I reg to O
    0x60, 0x15,                   //0x284 set V0 to X coord
    0x22, 0x3A,                   //0x286 goto draw/delay sub
    0xA2, 0x16,                   //0x288 set I reg to K
    0x60, 0x1A,                   //0x28A set V0 to X coord
    0x22, 0x3A,                   //0x28C goto draw/delay sub
    0xA2, 0x2F,                   //0x28E set I reg to !
    0x60, 0x1F,                   //0x290 set V0 to X coord
    0x22, 0x3A,                   //0x292 goto draw/delay sub

    //count number of times test loops and toggle res mode
    0x73, 0x01,                   //0x294 increment demo loop counter V3
    0x33, 0x02,                   //0x296 skip instruction if V3 == 2
    0x12, 0xAA,                   //0x298 jump to delay loop at end
    0x63, 0x00,                   //0x29A reset V3, loop counter, to zero
    0x34, 0x00,                   //0x29C if set to lowres skip to setting hires
    0x12, 0xA6,                   //0x29E jump to setting low res
    0x00, 0xFF,                   //0x2A0 switch to hires mode
    0x64, 0x01,                   //0x2A2 set hires flag
    0x12, 0xAA,                   //0x2A4 jump to delay loop at end
    0x00, 0xFE,                   //0x2A6 switch to lowres
    0x64, 0x00,                   //0x2A8 set lowres flag

    0x12, 0xCC,                   //0x2AA jump over large sprite

    0xAA, 0xAA, 0x55, 0x55, 0xAA, 0xAA, 0x55, 0x55, //0x2AC
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //0x2B4
    0x55, 0x55, 0xAA, 0xAA, 0x55, 0x55, 0xAA, 0xAA, //0x2BC
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //0x2C4

    0x60, 0x22,                   //0x2CC set v0 to X coord
    0x61, 0x01,                   //0x2CE set V1 to Y coord
    0xA2, 0xAC,                   //0x2D0 set I reg to uber sprite
    0xD0, 0x10,                   //0x2D20 draw ubersprite

    //delay loop before restarting
    0x62, 0x14,                   //0x2D4 set V2 to 20
    0xF2, 0x18,                   //0x2D6 set sound timer to V2
    0x62, 0x3C,                   //0x2D8 set V2 to 60
    0xF2, 0x15,                   //0x2DA set timer to V2
    0x42, 0x00,                   //0x2DC skip next if V2 not 0
    0x12, 0x48,                   //0x2DE goto start
    0xF2, 0x07,                   //0x2E0 copy timer to V2    
    0x12, 0xDC                    //0x2E2 goto skip check
};

//CHIP8 TEST OK!


#endif //CH_TEST_ROM_HPP_