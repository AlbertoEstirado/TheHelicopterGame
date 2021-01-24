/*
 * WALL
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
#include "Path.hpp"


namespace helicopter
{

    class Wall
    {
        public:

            float speed = 150;  //velocidad a la que se mueven las paredes

            float x = 0;  //coordenada en x

            struct levelPart{  //estructura para definir las partes de una pared
                float y;
                float width;
                float height;
            };

            int maxHeight = 400;  //valores de la generacion de mapa que impidan ser muy facil o imposible
            int minHeight = 100;  //estos valores son los que se cambian para que el juego se vuelva mas dificil

            //---------------------partes de una pared--------------------
            levelPart topWall;
            levelPart botWall;
            levelPart pathPart;
            //------------------------------------------------------------

            Wall();

            void render(Canvas & canvas);

            void update(float time);  //comportamientos que se ejecutan cada frame

            void setUp();  //setea el path

            void calculatTopAndBot();  //esta funcion calcula todas las partes del nivel en base a la del path

            void moveWall(const Wall wallBehind); //coloca la pared actual en base a la pared recibida dentro de unos maximos y minimos

            void getHarder();  //cambia los valores maximos y minimos dentro de unos limites

            int randomRange(int min, int max);  //funcion aux para calcular numeros random


    };


}


