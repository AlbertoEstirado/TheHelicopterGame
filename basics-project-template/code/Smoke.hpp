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
        int x;
        int y;
        float speed = 150;


        Smoke();
        void render(Canvas & canvas);
        void update(float time);
    };



}


