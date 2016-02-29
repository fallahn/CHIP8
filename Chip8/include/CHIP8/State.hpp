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

#ifndef CH_STATE_HPP_
#define CH_STATE_HPP_

#include <memory>

namespace sf
{
    class RenderTarget;
    class Event;
}

class State
{
public:
    State() = default;
    virtual ~State() = default;

    virtual void handleEvent(const sf::Event&) = 0;
    virtual void update(float) = 0;
    virtual void draw(sf::RenderTarget&) const = 0;

    enum class Action
    {
        None,
        Push,
        Pop
    };
    
    struct PendingAction final
    {
        Action action = Action::None;
        State* state = nullptr;
    };
private:

};

#endif //CH_STATE_HPP_