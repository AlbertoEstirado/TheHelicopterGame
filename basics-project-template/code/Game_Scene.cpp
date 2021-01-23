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

        loadScore();
    }

    bool Game_Scene::initialize ()
    {
        state     = LOADING;
        suspended = false;
        x         = 640;
        y         = 360;
        firstWall = 0;

        //player->start();

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
        if (state == RUNNING || state == PAUSE || state == GAMEOVER)
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

                    Point2f touch_location = { x, y };

                   if (option_at (touch_location) == PAUSE && state == RUNNING)
                   {
                       pause(true);
                   }
                   else{
                       pause(false);
                   }
                   if(state == GAMEOVER && option_at (touch_location) == MENU)
                   {
                       director.run_scene (shared_ptr< Scene >(new Menu_Scene));
                   }

                    break;
                }
            }
        }
        //if(state == GAMEOVER)
        //{
        //    switch (event.id) {
        //        case ID(touch - started):
        //        case ID(touch-moved):
        //            {
        //            //initialize();
        //            //director.run_scene (shared_ptr< Scene >(new Menu_Scene));
        //            break;
        //        }
        //    }
        //}
    }

    void Game_Scene::update (float time)
    {
        switch (state)
        {
            case LOADING: load ();     break;
            case WAITTING:   break;
            case PAUSE: break;
            case RUNNING: run  (time); break;
            case GAMEOVER: gameoverScreen(); break;
        }
    }

    void Game_Scene::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended && (state == RUNNING || state == WAITTING || state == GAMEOVER || state == PAUSE))
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

                for (int i = 0; i < walls.size(); ++i) {
                    walls[i].render(*canvas);
                }


                if(state == GAMEOVER)
                {
                    canvas->fill_rectangle({options[GAMEOVERHUD].x,options[GAMEOVERHUD].y},
                                           {options[GAMEOVERHUD].slice->width, options[GAMEOVERHUD].slice->height}
                            ,options[GAMEOVERHUD].slice);

                    canvas->fill_rectangle({options[MENU].x,options[MENU].y},
                                           {options[MENU].slice->width, options[MENU].slice->height}
                            ,options[MENU].slice);
                }

                if(state == RUNNING)
                {
                  canvas->fill_rectangle({options[PAUSEICON].x,options[PAUSEICON].y},
                                         {options[PAUSEICON].slice->width, options[PAUSEICON].slice->height}
                          ,options[PAUSEICON].slice);
                }
                else if(state == PAUSE)
                {
                    canvas->fill_rectangle({options[RESUMEICON].x,options[RESUMEICON].y},
                                           {options[RESUMEICON].slice->width, options[RESUMEICON].slice->height}
                            ,options[RESUMEICON].slice);
                }



                Text_Layout textLayout(*font, score_string);
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

                atlas.reset (new Atlas("atlas/helicopter_atlas.sprites", context));

                if(atlas->good())
                {
                    configureUI();
                }

                texturePlayer = Texture_2D::create(ID(texturePlayer),context, "player.png");
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
        if(score > scoreloaded)
        {
            saveScore();
        }

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

    void Game_Scene::pause(bool b)
    {
        if(b == true)
        {
            state = PAUSE;
        }
        else
        {
            state = RUNNING;
        }
    }

    void Game_Scene::configureUI()
    {
        //options[TRY_AGAIN].slice = atlas->get_slice(ID(tryagain));
        options[MENU].slice = atlas->get_slice(ID(menu));
        options[PAUSEICON].slice = atlas->get_slice(ID(pauseicon));
        //options[PAUSE].slice = atlas->get_slice(ID(pause));
        options[RESUMEICON].slice = atlas->get_slice(ID(resumeicon));
        options[GAMEOVERHUD].slice = atlas->get_slice(ID(gameover));

        options[MENU].x = canvas_width /2 ;
        options[MENU].y = canvas_height /2.5;

        options[PAUSEICON].x = canvas_width - (options[PAUSEICON].slice->width + 20) ;
        options[PAUSEICON].y = canvas_height - (options[PAUSEICON].slice->height + 20);

        options[RESUMEICON].x = options[PAUSEICON].x;
        options[RESUMEICON].y = options[PAUSEICON].y;

        options[GAMEOVERHUD].x = canvas_width/2 ;
        options[GAMEOVERHUD].y = canvas_height/1.5;

    }

    void Game_Scene::loadScore()
    {
        string path = application.get_internal_data_path() + "/save.data";
        basics::log.d (string("loading score from:  ") + path);

        ifstream reader(path, ofstream::binary);

        if(reader)
        {
            int auxscoreloaed;

            reader.read((char *)&auxscoreloaed, sizeof(auxscoreloaed));

            if (!reader.fail () && !reader.bad ())
            {
                scoreloaded = auxscoreloaed;
            }
            else
            {
                basics::log.e ("ERROR at loading score in game_scene failed reading save.data.");
            }

        }

    }

    void Game_Scene::saveScore()
    {
        string path = application.get_internal_data_path () + "/save.data";

        basics::log.d (string("saving score and creating file at ") + path);

        ofstream writer(path, ofstream::binary | ofstream::trunc);
        score = 7;
        if(writer)
        {
            writer.write ((char *)&score, sizeof(score));
        }
        if (writer.good ())
        {
            basics::log.d ("Score saved succesfully...");
        }
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

    int Game_Scene::option_at (const Point2f & point)
    {
        for (int index = 0; index < number_of_options; ++index)
        {
            const Option & option = options[index];

            if
                    (
                    point[0] > option.x - option.slice->width  &&
                    point[0] < option.x + option.slice->width  &&
                    point[1] > option.y - option.slice->height &&
                    point[1] < option.y + option.slice->height
                    )
            {
                return index;
            }
        }

        return -1;
    }



}
























