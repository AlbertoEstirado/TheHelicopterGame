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
        pathPart.y = 150;
        pathPart.width = 10;
        pathPart.height = 400;

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
            pathPart.height = (float)randomRange((int)wallBehind.pathPart.height - 5 ,
                                                 (int)wallBehind.pathPart.height + 5);
        }while (pathPart.height < 270 && pathPart.height > 550);

        if(wallBehind.pathPart.height > maxHeight)
        {
            pathPart.height = (float)randomRange((int)wallBehind.pathPart.height - 5 ,
                                                 (int)wallBehind.pathPart.height);
        }
        else if(wallBehind.pathPart.height < minHeight)
        {
            pathPart.height = (float)randomRange((int)wallBehind.pathPart.height  ,
                                                 (int)wallBehind.pathPart.height + 6);
        }
        else
        {
            pathPart.height = (float)randomRange((int)wallBehind.pathPart.height - 5 ,
                                                 (int)wallBehind.pathPart.height + 6);
        }



        if(wallBehind.pathPart.y < 0)
        {
            pathPart.y = (float)randomRange((int)wallBehind.pathPart.y,
                                            (int)wallBehind.pathPart.y +21 );
        }
        else if(wallBehind.pathPart.height + wallBehind.pathPart.y > 720)
        {
            pathPart.y = (float)randomRange((int)wallBehind.pathPart.y -20,
                                            (int)wallBehind.pathPart.y);
        }
        else
        {
            pathPart.y = (float)randomRange((int)wallBehind.pathPart.y -20,
                                            (int)wallBehind.pathPart.y +21 );
        }

        calculatTopAndBot();
    }

    void Wall::getHarder()
    {
        maxHeight -= 10;
        minHeight -= 1;
    }


     int Wall::randomRange(int min, int max) //range : [min, max]
    {
        return min + rand() % (max - min);
    }

}









































