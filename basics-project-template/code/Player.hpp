/*
 * PLAYER
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
#include "Smoke.hpp"
#include <basics/Timer>


namespace helicopter
{
    using basics::Timer;

    class Player : public Sprite
    {
        public:

            static constexpr float GRAVITY = 150;

            std::vector<Smoke> smokes;

            Timer timer;

            Player(Texture_2D *texture);

            void render(Canvas & canvas);

            void start();
            void update(float time);

            void fly(float time);
    };


}


