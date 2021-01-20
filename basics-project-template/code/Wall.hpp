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

            float speed = 200;

            float x = 0;

            struct levelPart{
                float y;
                float width;
                float height;
            };

            levelPart topWall;
            levelPart botWall;

            levelPart pathPart;

            Wall();

            void render(Canvas & canvas);
            void start();
            void update(float time);
            void setUp();
            void calculatTopAndBot();
            void moveWall(const Wall wallBehind);
            int randomRange(int min, int max);


    };


}


