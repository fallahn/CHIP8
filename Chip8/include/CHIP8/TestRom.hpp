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
//TODO instructions should start on even number addresses
static const std::vector<sf::Uint8> testRom = //TODO convert to std::array
{
    0x12, 0x47,                   //0x200 skip charset / subs and start program
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
    //draw then delay loop (subroutine)
    0x62, 0x0F,                   //0x239 set V2 to 15
    0xF2, 0x15,                   //0x23B set timer to V2
    0xD0, 0x15,                   //0x23D draw V0, V1 for 5 bytes
    0x42, 0x00,                   //0x23F skip next if V2 not 0
    0x00, 0xEE,                   //0x241 quit subroutine
    0xF2, 0x07,                   //0x243 copy timer to V2
    0x12, 0x3F,                   //0x245 goto skip check
    //program begin
    0xA2, 0x02,                   //0x247 set I reg to C
    0x60, 0x01,                   //0x249 set V0 to X coord
    0x61, 0x01,                   //0x24B set V1 to Y coord
    0x22, 0x39,                   //0x24D goto draw/delay sub
    0xA2, 0x0C,                   //0x24F set I reg to H
    0x60, 0x06,                   //0x251 set V0 to X coord
    0x22, 0x39,                   //0x253 goto draw/delay sub
    0xA2, 0x11,                   //0x255 set I reg to I
    0x60, 0x0B,                   //0x257 set V0 to X coord
    0x22, 0x39,                   //0x259 goto draw/delay sub
    0xA2, 0x20,                   //0x25B set I reg to P
    0x60, 0x0F,                   //0x25D set V0 to X coord
    0x22, 0x39,                   //0x25F goto draw/delay sub
    0xA2, 0x34,                   //0x261 set I reg to 8
    0x60, 0x17,                   //0x263 set V0 to X coord
    0x22, 0x39,                   //0x265 goto draw/delay sub
    0xA2, 0x2A,                   //0x267 set I reg to T
    0x60, 0x01,                   //0x269 set V0 to X coord
    0x61, 0x07,                   //0x26B set V1 to Y coord
    0x22, 0x39,                   //0x26D goto draw/delay sub
    0xA2, 0x07,                   //0x26F set I reg to E
    0x60, 0x05,                   //0x271 set V0 to X coord
    0x22, 0x39,                   //0x273 goto draw/delay sub
    0xA2, 0x25,                   //0x275 set I reg to S
    0x60, 0x0A,                   //0x277 set V0 to X coord
    0x22, 0x39,                   //0x279 goto draw/delay sub
    0xA2, 0x2A,                   //0x27B set I reg to T
    0x60, 0x0F,                   //0x27D set V0 to X coord
    0x22, 0x39,                   //0x27F goto draw/delay sub
    0xA2, 0x1B,                   //0x281 set I reg to O
    0x60, 0x15,                   //0x283 set V0 to X coord
    0x22, 0x39,                   //0x285 goto draw/delay sub
    0xA2, 0x16,                   //0x287 set I reg to K
    0x60, 0x1A,                   //0x289 set V0 to X coord
    0x22, 0x39,                   //0x28B goto draw/delay sub
    0xA2, 0x2F,                   //0x28D set I reg to !
    0x60, 0x1F,                   //0x28F set V0 to X coord
    0x22, 0x39,                   //0x291 goto draw/delay sub
    
    0x82, 0x14,                   //0x293 set V2 to 20
    0xF2, 0x18,                   //0x295 set sound timer to V2
    0x62, 0x3C,                   //0x297 set V2 to 60
    0xF2, 0x15,                   //0x299 set timer to V2
    0x42, 0x00,                   //0x29B skip next if V2 not 0
    0x12, 0x47,                   //0x29D goto start
    0xF2, 0x07,                   //0x29F copy timer to V2
    0x12, 0x9B                    //0x2A1 goto skip check
};

//CHIP8 TEST OK!


#endif //CH_TEST_ROM_HPP_