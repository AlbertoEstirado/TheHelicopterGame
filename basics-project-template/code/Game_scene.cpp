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

using namespace basics;
using namespace std;

namespace helicopter
{
    Game_Scene::Game_Scene()
    {
        canvas_width  = 1280;
        canvas_height =  720;
        touching = false;
        walls.resize(150);
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


                if (player)
                {
                    player->render(*canvas);
                }
                //testWall.render(* canvas);

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

            if(i > 0){
             walls[i].moveWall(walls[i-1]);
            }
        }
        //testWall.start();

    }

    void Game_Scene::run (float time)
    {
        player->update(time);
        if(touching) { player->fly(time);}

        for (int i = 0; i < walls.size(); ++i) {
            walls[i].update(time);
        }

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
        }
    }

}
























