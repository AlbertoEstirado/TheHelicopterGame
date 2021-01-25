/*
 * RULES SCENE
 * Copyright © 2020+ Alberto Estirado López
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * estiradoalberto@gmail.com
 */


#include "Rules_Scene.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scaling>
#include <basics/Rotation>
#include <basics/Translation>

using namespace basics;
using namespace std;

namespace helicopter
{

    Rules_Scene::Rules_Scene()
    {
        canvas_width  = 1280;
        canvas_height =  720;
    }

    bool Rules_Scene::initialize ()
    {
        state     = LOADING;
        suspended = false;
        x         = 640;
        y         = 360;

        return true;
    }

    void Rules_Scene::suspend ()
    {
        suspended = true;
    }

    void Rules_Scene::resume ()
    {
        suspended = false;
    }

    void Rules_Scene::handle (Event & event)
    {
        if (state == RUNNING)
        {
            switch (event.id)
            {
                case ID(touch-started):
                case ID(touch-moved):
                case ID(touch-ended):
                {
                    x = *event[ID(x)].as< var::Float > ();
                    y = *event[ID(y)].as< var::Float > ();
                    break;
                }
            }
        }
    }

    void Rules_Scene::update (float time)
    {
        if (!suspended) if (state == LOADING)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();
            if (context)
            {
                font.reset (new Raster_Font("fonts/impact.fnt", context));
            }
        }
    }

    void Rules_Scene::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended && state == RUNNING)
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

                Text_Layout textLayout(*font, rules);
                canvas->draw_text({canvas_width/2, canvas_height/2}, textLayout);
            }
        }
    }

    void Rules_Scene::load ()
    {
        if (!suspended)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                texture = Texture_2D::create (ID(test), context, "test.png");

                if (texture)
                {
                    context->add (texture);

                    state = RUNNING;
                }
            }
        }
    }

    void Rules_Scene::run (float )
    {
    }

}















