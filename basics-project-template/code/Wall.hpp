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

            float x = 0;          ///< Coordenada en x

            struct levelPart{     ///< Estructura para definir las partes de una pared
                float y;
                float width;
                float height;
            };

            int maxHeight = 400;  ///< Valores de la generacion de mapa que impidan ser muy facil o imposible
            int minHeight = 100;  ///< Estos valores son los que se cambian para que el juego se vuelva mas dificil

            //---------------------partes de una pared--------------------
            levelPart topWall;    ///< Parte de arriba de la pared
            levelPart botWall;    ///< Parte de abajo de la pared
            levelPart pathPart;   ///< Parte por la cual el player va a poder moverse
            //------------------------------------------------------------

            Wall();

            void render(Canvas & canvas);

            /**
             * Comportamientos que se ejecutan cada frame. En este caso el movimiento hacia la izquierda de
             * la pantalla.
             * @param time
             */
            void update(float time);

            /**
             * Setea el path inicial para que el jugador no aparezaca en ena pared.
             */
            void setUp();

            /**
             * Esta funcion calcula todas las partes del nivel en base a la del path.
             * Gracias a esto solo tenemos qe definir la parte centrar.
             */
            void calculatTopAndBot();

            /**
             * Coloca la pared actual en base a la pared recibida dentro de unos maximos y minimos.
             * Esto nos ayuda a generar el mapa de una manera sencilla.
             * @param wallBehind
             */
            void moveWall(const Wall wallBehind);

            /**
             * Cambia los valores maximos y minimos dentro de unos limites. Esto se ejecuta
             * cada vez que X paredes salen por la izquierda de la pantalla.
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


