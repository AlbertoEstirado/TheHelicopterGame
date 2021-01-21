/*
 * PATH
 * Copyright © 2020+ Alberto Estirado López
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * estiradoalberto@gmail.com
 */

#include "Path.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scaling>
#include <basics/Rotation>
#include <basics/Translation>
#include <cstdlib>

using namespace basics;
using namespace std;

namespace helicopter
{
    Path::Path()
    {

    }

    Path Path::generateNextPath(int lasty)
    {
        Path aux;

        aux.size = randomRange(50, 250);
        aux.y = lasty;
        aux.y2 = randomRange(0, 720);

        return aux;
    }

    int Path::randomRange(int min, int max) //range : [min, max]
    {
        return min + rand() % (max - min);
    }

    void Path::generateRandomPath()
    {
        y = randomRange(150, 600);
        y2 = randomRange(150, 600);
        size = randomRange(50, 150);
    }
}









































