/*
 * GAME SCENE
 * Copyright © 2020+ Alberto Estirado López
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * estiradoalberto@gmail.com
 */

#include "Game_Scene.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scaling>
#include <basics/Rotation>
#include <basics/Translation>
#include <math.h>


using namespace basics;
using namespace std;

namespace helicopter
{
    Game_Scene::Game_Scene()
    {
        canvas_width  = 1280;
        canvas_height =  720;
        touching = false;
        walls.resize(130);

        srand (unsigned(time(nullptr)));
    }

    bool Game_Scene::initialize ()
    {
        state     = LOADING;
        suspended = false;
        x         = 640;
        y         = 360;

        return true;
    }

    void Game_Scene::suspend ()
    {
        suspended = true;
    }

    void Game_Scene::resume ()
    {
        suspended = false;
    }

    void Game_Scene::handle (Event & event)
    {
        if (state == RUNNING)
        {
            switch (event.id)
            {
                case ID(touch-started):
                case ID(touch-moved):
                {
                    touching = true;
                    break;
                }
                case ID(touch-ended):
                {
                    touching = false;
                    x = *event[ID(x)].as< var::Float > ();
                    y = *event[ID(y)].as< var::Float > ();
                    break;
                }
            }
        }
    }

    void Game_Scene::update (float time)
    {
        switch (state)
        {
            case LOADING: load ();     break;
            case RUNNING: run  (time); break;
        }
    }

    void Game_Scene::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended && state == RUNNING)
        {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)
            {
                canvas->clear        ();
                canvas->set_color    (1, 1, 1);

                Text_Layout textLayout(*font, score_string);
                canvas->draw_text({canvas_width/2, 150}, textLayout);



                if (player)
                {
                    player->render(*canvas);
                }

                for (int i = 0; i < walls.size(); ++i) {
                    walls[i].render(*canvas);
                }
            }
        }
    }

    void Game_Scene::load ()
    {
        if (!suspended)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                font.reset (new Raster_Font("fonts/impact.fnt", context));

                texturePlayer = Texture_2D::create(ID(texturePlayer),context, "helicopterRocket.png");
                player.reset(new Player(texturePlayer.get()));

                if (texturePlayer)
                {
                    context->add(texturePlayer);
                }

                start();

                state = RUNNING;
            }
        }
    }

    void Game_Scene::start()
    {
        player->start();

        for (int i = 0; i < walls.size(); ++i) {
            walls[i].setUp();
            walls[i].x = i*walls[i].botWall.width;
//
            if(i > 0){
             walls[i].moveWall(walls[i-1]);
            }
        }

        //paths[0].generateRandomPath();
        //paths[1].generateRandomPath();
        //generatePath(paths[0]);
        //generatePath(paths[1]);
        //testWall.start();

    }

    void Game_Scene::run (float time)
    {
        player->update(time);

        if(touching) { player->fly(time);}

        for (int i = 0; i < walls.size(); ++i) {
            walls[i].update(time);
        }


        //calculateWallsColision();

        manageWalls();
    }

    void Game_Scene::manageWalls()
    {
        if(walls[firstWall].x < 0 - walls[firstWall].pathPart.width)
        {
            if(firstWall == 0)
            {
                walls[firstWall].x = walls[walls.size()-1].x + walls[firstWall].pathPart.width;
                walls[firstWall].moveWall(walls[walls.size()-1]);
            }
            else
            {
                walls[firstWall].x = walls[firstWall-1].x + walls[firstWall].pathPart.width;
                walls[firstWall].moveWall(walls[firstWall - 1]);
            }

            if(firstWall >= walls.size()-1)
            {
                firstWall = 0;
            }
            else
            {
                firstWall++;
            }

            score++;
            score_string = to_wstring(score);
            if(score % 50 == 0)
            {
                for (int i = 0; i < walls.size()-1; ++i)
                {
                    walls[i].getHarder();
                }
            }
        }
    }

    void Game_Scene::calculateWallsColision()
    {
        for (int i = 0; i < walls.size() - 1; ++i)
        {
            if(walls[i].x < player->get_position_x() &&
            walls[i].x + walls[i].pathPart.width > player->get_position_x() )
            {
                if(player->get_position_y() < walls[i].pathPart.y ||
                player->get_position_y() > walls[i].topWall.y)
                {
                    suspend();
                }

            }
        }
    }

    void Game_Scene::generatePath(Path path)
    {
        for (int i = 1; i < path.size; ++i)
        {
            walls[i].setUp();


           int a = (int)(path.size - walls[i].x);
           int b = (int)(path.y2 - path.y);

            walls[i].pathPart.y = (b*i/a) + path.y;

            walls[i].x = i*walls[i].botWall.width;
            walls[i].pathPart.height = 100;
            walls[i].calculatTopAndBot();
        }

    }



}
























