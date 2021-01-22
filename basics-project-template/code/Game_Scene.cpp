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
#include <basics/Application>


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


    }

    bool Game_Scene::initialize ()
    {
        state     = LOADING;
        suspended = false;
        x         = 640;
        y         = 360;

        srand (unsigned(time(nullptr)));

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
        if(state == WAITTING)
        {
            state = RUNNING;
        }
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
            case WAITTING:   break;
            case RUNNING: run  (time); break;
            case GAMEOVER: gameoverScreen(); break;
        }
    }

    void Game_Scene::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended && (state == RUNNING || state == WAITTING || state == GAMEOVER))
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


                if (player)
                {
                    player->render(*canvas);
                }

                for (int i = 0; i < walls.size(); ++i) {
                    walls[i].render(*canvas);
                }

                if(gameover_texture)
                {
                    //draw_slice (canvas, { canvas_width * 0.5f, canvas_height * 0.7f }, *atlas, ID(gameover));

                    canvas->fill_rectangle({ canvas_width * .5f, canvas_height * .7f },
                                           { gameover_texture->get_width (), gameover_texture->get_height () },
                                           gameover_texture. get ());
                }

                canvas->draw_text({canvas_width/2, 150}, textLayout);
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


                //atlas.reset (new Atlas("hud-atlas/helicopterSpriteSheet.sprites", context));



                texturePlayer = Texture_2D::create(ID(texturePlayer),context, "helicopter_sprite.png");
                player.reset(new Player(texturePlayer.get()));

                if (texturePlayer)
                {
                    context->add(texturePlayer);
                }

                start();

                font.reset (new Raster_Font("fonts/impact.fnt", context));

                //state = atlas->good () ? WAITTING : LOADING;
                state = WAITTING;
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

        for (int i = 0; i < player->smokes.size(); ++i) {
            player->smokes[i].update(time);
        }

        calculateWallsColision();

        manageWalls();
        manageSmokes();
    }

    void Game_Scene::gameover()
    {
        state = GAMEOVER;
    }

    void Game_Scene::gameoverScreen()
    {
        Graphics_Context::Accessor contex = director.lock_graphics_context();

        if(contex)
        {
            gameover_texture = Texture_2D::create(1,contex, "gameover.png");
            if(gameover_texture)
            {
                contex->add(gameover_texture);
            }
        }
    }

    void Game_Scene::saveScore()
    {
        //string path = application.get_internal_data_path() + "/save.data";
//
        //ofstream writer(path, ofstream::binary | ofstream::trunc);
//
        //if(writer)
        //{
        //    writer.write();
        //}
    }

    void Game_Scene::draw_slice (Canvas * canvas, const basics::Point2f & where, basics::Atlas & atlas, basics::Id slice_id)
    {
        const Atlas::Slice * slice = atlas.get_slice (slice_id);

        if (slice)
        {
            canvas->fill_rectangle (where, { slice->width, slice->height }, slice);
        }
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

    void Game_Scene::manageSmokes()
    {
        for (int i = 0; i < player->smokes.size(); ++i) {
            if(player->smokes[i].x < 0)
            {
                player->smokes[i].x = player->get_position_x();
                player->smokes[i].y = player->get_position_y();
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
                    gameover();
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
























