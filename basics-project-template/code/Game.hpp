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
#include "Sprite.hpp"


namespace helicopter
{

    class Player : public Sprite
    {
        public:

            void render(Canvas & canvas);


            Player(Texture_2D * texture);

            void load();



    };

    class Walls
    {
        typedef std::shared_ptr< basics::Texture_2D > Texture_Handle;
        Texture_Handle texture;

        public:


            void render(Canvas & canvas);

            Walls();
    };


}


