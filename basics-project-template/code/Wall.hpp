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


namespace helicopter
{

    class Wall
    {
        public:

            float speed = 150;    ///< Velocidad a la que se mueven las paredes

            float x = 0;          ///<coordenada en x

            struct levelPart{     ///<estructura para definir las partes de una pared
                float y;
                float width;
                float height;
            };

            int maxHeight = 400;  ///<valores de la generacion de mapa que impidan ser muy facil o imposible
            int minHeight = 100;  ///<estos valores son los que se cambian para que el juego se vuelva mas dificil

            //---------------------partes de una pared--------------------
            levelPart topWall;
            levelPart botWall;
            levelPart pathPart;
            //------------------------------------------------------------

            Wall();

            void render(Canvas & canvas);

            /**
             * Comportamientos que se ejecutan cada frame
             * @param time
             */
            void update(float time);

            /**
             * Setea el path
             */
            void setUp();

            /**
             * Esta funcion calcula todas las partes del nivel en base a la del path
             */
            void calculatTopAndBot();

            /**
             * Coloca la pared actual en base a la pared recibida dentro de unos maximos y minimos
             * @param wallBehind
             */
            void moveWall(const Wall wallBehind);

            /**
             * Cambia los valores maximos y minimos dentro de unos limites
             */
            void getHarder();

            /**
             * Funcion aux para calcular numeros random
             * @param min
             * @param max
             * @return
             */
            int randomRange(int min, int max);


    };

}


