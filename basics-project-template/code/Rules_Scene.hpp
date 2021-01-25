/*
 * RULES SCENE
 * Copyright © 2020+ Alberto Estirado López
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * estiradoalberto@gmail.com
 */


#include <memory>
#include <basics/Scene>
#include <basics/Texture_2D>
#include <basics/internal/Raster_Font.hpp>
#include "Menu_Scene.hpp"


namespace helicopter
{

    class Rules_Scene : public basics::Scene
    {
        typedef std::unique_ptr< basics::Raster_Font > Font_Handle;
        typedef std::shared_ptr< basics::Texture_2D > Texture_Handle;

    public:

        enum State
        {
            LOADING,
            RUNNING,
        };

        State          state;
        bool           suspended;

        unsigned       canvas_width;
        unsigned       canvas_height;

        Texture_Handle texture;
        float          x, y;

        Font_Handle font;
        std::wstring rules = L"Press to rise and not crash into the walls";

    public:

        Rules_Scene();

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }

        bool initialize () override;
        void suspend    () override;
        void resume     () override;

        void handle     (basics::Event & event) override;
        void update     (float time) override;
        void render     (basics::Graphics_Context::Accessor & context) override;

    private:

        void load ();
        void run  (float time);

    };



}
