/*
 * WALL
 * Copyright © 2020+ Alberto Estirado López
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * estiradoalberto@gmail.com
 */

#include "Wall.hpp"
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
    Wall::Wall()
    {

    }

    void Wall::render(Canvas &canvas)
    {
        canvas.fill_rectangle({x, topWall.y}, {topWall.width, topWall.height});

        canvas.fill_rectangle({x,botWall.y}, {botWall.width, botWall.height});
    }

    void Wall::start()
    {
        setUp();
    }

    void Wall::update(float time)
    {
        x -= speed * time;
    }

    void Wall::setUp()
    {
        pathPart.y = 100;
        pathPart.width = 10;
        pathPart.height = 300;

        calculatTopAndBot();
    }

    void Wall::calculatTopAndBot()
    {
        botWall.y = 0;
        botWall.width = pathPart.width;
        botWall.height = pathPart.y;

        topWall.width = pathPart.width;
        topWall.height = 720 - (pathPart.height + botWall.height);
        topWall.y = 720 - topWall.height;

    }

    void Wall::moveWall(const Wall wallBehind)
    {
        do
        {
            pathPart.y = (float)randomRange((int)wallBehind.pathPart.y -20,
                                            (int)wallBehind.pathPart.y +20 );
        }while (pathPart.y < 0 && pathPart.y > 420);

        do
        {
            pathPart.height = (float)randomRange((int)wallBehind.pathPart.height - 5 ,
                                                 (int)wallBehind.pathPart.height + 5);
        }while (pathPart.height < 170 && pathPart.height > 350);


        calculatTopAndBot();
    }

     int Wall::randomRange(int min, int max) //range : [min, max]
    {
        return min + rand() % (( max + 1 ) - min);
    }

}









































