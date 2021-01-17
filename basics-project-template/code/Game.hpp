/*
 * GAME SCENE
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



namespace helicopter
{

    class Player
    {
        typedef std::shared_ptr< basics::Texture_2D > Texture_Handle;
        Texture_Handle texture;

        public:
            float x;
            float y;

            unsigned       canvas_width;
            unsigned       canvas_height;

            void render(basics::Graphics_Context::Accessor & context);

            Player();
            Player(unsigned cw, unsigned ch);


            void load();

    };

    class Walls
    {
        typedef std::shared_ptr< basics::Texture_2D > Texture_Handle;
        Texture_Handle texture;

        public:
            float x;
            float y;

            unsigned canvas_width;
            unsigned canvas_height;

            void render(basics::Graphics_Context::Accessor & context);

            Walls();
    };


}


