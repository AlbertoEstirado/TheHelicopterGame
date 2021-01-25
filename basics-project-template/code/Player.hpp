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

            static constexpr float GRAVITY = 150;   ///< Valor que controla la velocidad en la que se mueve verticalmente

            std::vector<Smoke> smokes;              ///< Humos que siguen al player

            Player(Texture_2D *texture);            ///< Textura

            void render(Canvas & canvas);

            /**
             * Se inicializa
             */
            void start();

            /**
             * Comportamientos que necesitan ejecutarse cada frame
             * @param time
             */
            void update(float time);

            /**
             * Funcion de volar
             * @param time
             */
            void fly(float time);
    };


}


