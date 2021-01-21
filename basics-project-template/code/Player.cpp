/*
 * PLAYER
 * Copyright © 2020+ Alberto Estirado López
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * estiradoalberto@gmail.com
 */


#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scaling>
#include <basics/Rotation>
#include <basics/Translation>
#include "Player.hpp"
#include "Sprite.hpp"


using namespace basics;
using namespace std;

namespace helicopter
{
    Player::Player(Texture_2D *texture) : Sprite(texture)
    {
        anchor   = basics::CENTER;
        size     = { texture->get_width (), texture->get_height () };
        position = { 0.f, 0.f };
        scale    = 1.f;
        speed    = { 0.f, 0.f };
        visible  = true;
        smokes.resize(5);
    }

    void Player::render(Canvas &canvas)
    {
        if (visible)
        {
            for (int i = 0; i < smokes.size(); ++i) {
                smokes[i].render(canvas);
            }
            canvas.fill_rectangle (position, size * scale, texture, anchor);

        }
    }

    void Player::start()
    {
        set_position({240,360});
        for (int i = 0; i < smokes.size(); ++i) {
            smokes[i].y = get_position_y();
            smokes[i].x = get_position_x() - 8*(i+1);
        }
    }

    void Player::update(float time)
    {
        set_position({position[0], position[1] -= GRAVITY * time});
    }

    void Player::fly(float time)
    {
        set_position({position[0], position[1] += GRAVITY * time * 2});
    }

}



























