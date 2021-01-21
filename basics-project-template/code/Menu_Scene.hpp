/*
 * MENU SCENE
 * Copyright © 2020+ Alberto Estirado López
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * estiradoalberto@gmail.com
 */

#ifndef MENU_SCENE_HEADER
#define MENU_SCENE_HEADER

#include <memory>
#include <basics/Atlas>
#include <basics/Canvas>
#include <basics/Point>
#include <basics/Scene>
#include <basics/Size>
#include <basics/Timer>

namespace helicopter
{

    using basics::Atlas;
    using basics::Timer;
    using basics::Canvas;
    using basics::Point2f;
    using basics::Size2f;
    using basics::Texture_2D;
    using basics::Graphics_Context;

    class Menu_Scene : public basics::Scene
    {

        enum State
        {
            LOADING,
            READY,
            FINISHED,
            ERROR
        };

        enum Option_Id
        {
            PLAY,
            SCORES,
            HELP,
            CREDITS
        };

        struct Option
        {
            const Atlas::Slice * slice;
            Point2f position;
            float   is_pressed;
        };

        static const unsigned number_of_options = 4;

    private:

        State    state;                                     ///< Estado de la escena.
        bool     suspended;                                 ///< true cuando la escena estÃ¡ en segundo plano y viceversa.

        unsigned canvas_width;                              ///< Ancho de la resoluciÃ³n virtual usada para dibujar.
        unsigned canvas_height;                             ///< Alto  de la resoluciÃ³n virtual usada para dibujar.

        Timer    timer;                                     ///< CronÃ³metro usado para medir intervalos de tiempo.

        Option   options[number_of_options];                ///< Datos de las opciones del menÃº

        std::unique_ptr< Atlas > atlas;                     ///< Atlas que contiene las imÃ¡genes de las opciones del menÃº

    public:

        Menu_Scene();

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }


        bool initialize () override;

        void suspend () override
        {
            suspended = true;
        }

        void resume () override
        {
            suspended = false;
        }


        void handle (basics::Event & event) override;

        void update (float time) override;

        void render (Graphics_Context::Accessor & context) override;

    private:


        void configure_options ();

        int option_at (const Point2f & point);

    };

}

#endif