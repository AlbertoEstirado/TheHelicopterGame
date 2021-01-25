/*
 * SMOKE
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
    class Smoke
    {
    public:
        int x;                  ///< Posicion en la coordenada X
        int y;                  ///< Posicion en la coordenada Y
        float speed = 150;      ///< Velociada a la que se mueve cada particula


        Smoke();

        /**
         * Renderizamos la particula con un tamaño
         * @param canvas
         */
        void render(Canvas & canvas);

        /**
         * Se ejecuta el movimiento de la particula
         * @param time
         */
        void update(float time);
    };



}


