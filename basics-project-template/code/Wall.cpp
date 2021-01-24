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
    Wall::Wall(){}

    void Wall::render(Canvas &canvas)
    {
        canvas.fill_rectangle({x, topWall.y}, {topWall.width, topWall.height});

        canvas.fill_rectangle({x,botWall.y}, {botWall.width, botWall.height});
    }


    /*
     * Funcion que se ejecuta cada frame
     * */
    void Wall::update(float time)
    {
        //se mueve la pared en base a su velocidad
        x -= speed * time;
    }

    /*
     * inicializa las partes de la pared
     * */
    void Wall::setUp()
    {
        pathPart.y = 150;
        pathPart.width = 10;
        pathPart.height = 400;

        calculatTopAndBot();
    }

    /*
     * Calculamos la parte de arriba y la de abajo en base al centro y al tamaño de la pantalla
     * */
    void Wall::calculatTopAndBot()
    {
        botWall.y = 0;
        botWall.width = pathPart.width;
        botWall.height = pathPart.y;

        topWall.width = pathPart.width;
        topWall.height = 720 - (pathPart.height + botWall.height);
        topWall.y = 720 - topWall.height;

    }

    /*
     * Calcula la variacion en altura y height en base a la pared recibida
     * */
    void Wall::moveWall(const Wall wallBehind)
    {

        //si no nos encontramos dentro de los limites calculamos un height con una variacion estableciza
        //en caso de encontrarnos en los limites limitariamos la variacion para que el path no se salga de la pantalla
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

        //en este caso hacemos lo mismo que en el height pero con la y
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

    /*
     * Aumentar la dificultad
     * */
    void Wall::getHarder()
    {
        //aqui se determina la dificultad maxima
        if(minHeight > 50 )
        {
            minHeight -= 5;
        }
        if(maxHeight > 80)
        {
            maxHeight -= 10;
        }
    }


     int Wall::randomRange(int min, int max) //range : [min, max]
    {
        return min + rand() % (max - min);
    }

}









































