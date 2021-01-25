/*
 * INTRO SCENE
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
#include <basics/Timer>


namespace helicopter
{
    using basics::Timer;
    using basics::Texture_2D;

    class Intro_Scene : public basics::Scene
    {

    private:

        enum State
        {
            UNINITIALIZED,
            LOADING,
            FADING_IN,
            WAITING,
            FADING_OUT,
            FINISHED,
        };

        State state;
        bool suspended;

        unsigned canvas_width;
        unsigned canvas_height;

        Timer timer;
        static constexpr float TIMEWAITING = 2;  ///< Tiempo que va a estar el logo en pantalla

        float opacity;

        std::shared_ptr<Texture_2D> intro_texture;

    public:

        Intro_Scene();

        bool initialize() override;
        void suspend() override ;
        void resume() override ;

        void render(basics::Graphics_Context::Accessor & context) override ;

        void update (float time) override;

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }



    private:

        void update_loading    ();
        void update_fading_in  ();
        void update_waiting    ();
        void update_fading_out ();

    };

}