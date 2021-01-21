/*
 * INTRO SCENE
 * Copyright © 2020+ Alberto Estirado López
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * estiradoalberto@gmail.com
 */

#include "Menu_Scene.hpp"
#include "Intro_Scene.hpp"
#include "Game_Scene.hpp"
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
    Intro_Scene::Intro_Scene()
    {
        state = UNINITIALIZED;
        suspended = true;
        canvas_width = 1280;
        canvas_height = 720;
    }

    bool Intro_Scene::initialize()
    {
        if(state == UNINITIALIZED)
        {
            state = LOADING;
        }
        else
        {
            suspended = false;
            opacity = 0.f;
            timer.reset();
            state = FADING_IN;
        }
        return true;
    }

    void Intro_Scene::suspend()
    {
        suspended = true;
    }

    void Intro_Scene::resume()
    {
        suspended = false;
    }

    /*
     * Renderizado de las texturas.
     * */
    void Intro_Scene::render(basics::Graphics_Context::Accessor &context)
    {
        if (!suspended)
        {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)
            {
                canvas->clear ();

                if (intro_texture)
                {
                    canvas->set_opacity (opacity);

                    canvas->fill_rectangle
                            (
                                    { canvas_width * .5f, canvas_height * .5f },
                                    { intro_texture->get_width (), intro_texture->get_height () },
                                    intro_texture. get ()
                            );
                }
            }
        }
    }

    /*
     * Gestion del estado de la escena intro.
     * */
    void Intro_Scene::update(float time)
    {
        if(!suspended)
        {
            switch (state)
            {
                case LOADING: update_loading();
                    break;
                case FADING_IN: update_fading_in();
                    break;
                case WAITING: update_waiting();
                    break;
                case FADING_OUT: update_fading_out();
                    break;
                default:
                    break;
            }
        }
    }
//----------------------------------------PRIVATE--------------------------------------------

    void Intro_Scene::update_loading()
    {
        Graphics_Context::Accessor context = director.lock_graphics_context();

        if(context)
        {
            intro_texture = Texture_2D::create(0, context, "helicopterLogo.png");

            if(intro_texture)
            {
                context->add(intro_texture);

                timer.reset();
                opacity = 0.f;
                state = FADING_IN;
            }
        }
    }

    void Intro_Scene::update_fading_in()
    {
        float elapsed_seconds = timer.get_elapsed_seconds();

        if(elapsed_seconds < 1.f)
        {
            opacity = elapsed_seconds;
        }
        else
        {
            timer.reset();
            opacity = 1.f;
            state = WAITING;
        }
    }

    void Intro_Scene::update_waiting()
    {
        if (timer.get_elapsed_seconds () > TIMEWAITING)
        {
            timer.reset ();

            state = FADING_OUT;
        }
    }

    void Intro_Scene::update_fading_out()
    {
        float elapsed_seconds = timer.get_elapsed_seconds ();

        if (elapsed_seconds < .5f)
        {
            opacity = 1.f - elapsed_seconds * 2.f;      // Se reduce la opacidad de 1 a 0 en medio segundo
        }
        else
        {
            state = FINISHED;

            director.run_scene (shared_ptr< Scene >(new Menu_Scene));
        }
    }

}
























