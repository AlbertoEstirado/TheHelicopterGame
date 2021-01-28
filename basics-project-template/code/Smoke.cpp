/*
 * SMOKE
 * Copyright © 2020+ Alberto Estirado López
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * estiradoalberto@gmail.com
 */


#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scaling>
#include <basics/Rotation>
#include <basics/Translation>
#include "Smoke.hpp"


using namespace basics;
using namespace std;

namespace helicopter
{
    Smoke::Smoke()
    {
        x = 0;
        y = 0;
    }

    void Smoke::render(Canvas &canvas)
    {
        canvas.fill_rectangle ({x,y}, {3,3});
    }

    void Smoke::update(float time)
    {
        x -= speed * time;
    }

}



























