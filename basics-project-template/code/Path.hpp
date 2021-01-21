/*
 * PATH
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

    class Path
    {

    public:

        float y;
        float y2;
        int size;

        Path();

        Path generateNextPath(int lasty);
        int randomRange(int min, int max);
        void generateRandomPath();

    };


}


