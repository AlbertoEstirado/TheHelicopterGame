/*
 * GAME SCENE
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
#include "Game.hpp"
#include "Sprite.hpp"


using namespace basics;
using namespace std;

namespace helicopter
{/*
    Player::Player(Texture_2D *texture)
    {
        x = sprite-> get_position_x();
        y = sprite-> get_position_y();
    }




    void Player::render(basics::Graphics_Context::Accessor &context)
    {
        Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

        if (!canvas)
        {
            canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
        }

        if (canvas)
        {
            canvas->clear        ();
            canvas->set_color    (1, 1, 1);
            canvas->draw_point   ({ 360, 360 });
            canvas->draw_segment ({   0,   0 }, { 1280, 720 });
            canvas->draw_segment ({   0, 720 }, { 1280,   0 });

            if (texture)
            {
                canvas->fill_rectangle ({ x, y }, { 100, 100 }, texture.get ());
            }
        }
    }

    void Player::load()
    {
        Graphics_Context::Accessor context = director.lock_graphics_context();

        if (context)
        {
            texture = Texture_2D::create (ID(test), context, "helicopterRocket.png");

            if (texture)
            {
                context->add (texture);

            }
        }
    }*/
}



























