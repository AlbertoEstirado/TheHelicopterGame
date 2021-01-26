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
        if (state == RUNNING || state == PAUSE  || state == GAMEOVER)
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

                    //Calculamos input con las options

                    if (option_at(touch_location) == MENU && (state == GAMEOVER ||state == PAUSE)) {
                        director.run_scene(shared_ptr<Scene>(new Menu_Scene));
                    }
                    if (option_at(touch_location) == TRY_AGAIN && (state == GAMEOVER ||state == PAUSE)) {
                        initialize();
                    }

                    if (option_at(touch_location) == PAUSEICON && state == RUNNING) {
                        pause(true);
                    }
                    if (option_at(touch_location) == RESUMEICON && state == PAUSE) {
                        pause(false);
                    }

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
            case PAUSE: break;
            case RUNNING: run  (time); break;
            case GAMEOVER: ; break;
        }
    }

    void Game_Scene::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended &&  state != LOADING )//(state == RUNNING || state == WAITTING || state == GAMEOVER || state == PAUSE))
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

                //Renderizamos player
                if (player)
                {
                    player->render(*canvas);
                }

                //Renderizamos walls
                for (int i = 0; i < walls.size(); ++i) {
                    walls[i].render(*canvas);
                }

                //Renderizamos pantalla de gameover
                if(state == GAMEOVER)
                {
                    printOption(GAMEOVERHUD, *canvas);
                    printOption(MENU, *canvas);
                    printOption(TRY_AGAIN, *canvas);
                }

                //Renderizamos pantalla de running
                if(state == RUNNING)
                {
                    printOption(PAUSEICON, *canvas);

                }
                else if(state == PAUSE)
                {
                    //Renderizamos pantalla de pause
                    printOption(RESUMEICON, *canvas);
                    printOption(PAUSETEXT, *canvas);
                    printOption(MENU, *canvas);
                    printOption(TRY_AGAIN, *canvas);
                }

                Text_Layout textLayout(*font, score_string);
                canvas->draw_text({canvas_width/2.2, canvas_height/5.f}, textLayout);
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
        //Ejecutamos los starts

        player->start();

        //Generamos el mapa con las paredes

        for (int i = 0; i < walls.size(); ++i) {
            walls[i].setUp();
            walls[i].x = i*walls[i].botWall.width;

            //Movemos cada pared en base a la anterior
            if(i > 0){
             walls[i].moveWall(walls[i-1]);
            }
        }
    }

    void Game_Scene::run (float time)
    {
        //Ejecutamos todos los updates de la escena

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
        //En caso de que la score obtenida sea mayor a la cargada anteriormente,
        //lo sobreescribimos.

        if(score > scoreloaded)
        {
            saveScore();
        }

        state = GAMEOVER;
    }


    void Game_Scene::pause(bool b)
    {
        //En base a b, cambiamos el state de la escena

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
        //Asignamos slices del atlas a la option

        options[TRY_AGAIN].slice = atlas->get_slice(ID(tryagain));
        options[MENU].slice = atlas->get_slice(ID(menu));
        options[PAUSEICON].slice = atlas->get_slice(ID(pauseicon));
        options[PAUSETEXT].slice = atlas->get_slice(ID(pause));
        options[RESUMEICON].slice = atlas->get_slice(ID(resumeicon));
        options[GAMEOVERHUD].slice = atlas->get_slice(ID(gameover));

        //Asiganmos coordenada en las que se va a printear

        options[PAUSETEXT].x = canvas_width /2 ;
        options[PAUSETEXT].y = canvas_height /1.5;

        options[TRY_AGAIN].x = canvas_width /2 ;
        options[TRY_AGAIN].y = canvas_height /2;

        options[GAMEOVERHUD].x = canvas_width/2 ;
        options[GAMEOVERHUD].y = canvas_height/1.3;

        options[MENU].x = canvas_width /2 ;
        options[MENU].y = canvas_height /2.7;

        options[PAUSEICON].x = canvas_width - (options[PAUSEICON].slice->width + 20);
        options[PAUSEICON].y = canvas_height - (options[PAUSEICON].slice->height + 20);

        options[RESUMEICON].x = canvas_width - (options[RESUMEICON].slice->width + 100);
        options[RESUMEICON].y = canvas_height - (options[RESUMEICON].slice->height + 20);
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
        //Comprobamos si la pared en el index de firstwall se encuentra a la fuera de la pantalla
        //en la coordenada X. Si se sale, la posicionamos al final de la cola.

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

            //Si se sale del index reseteamos a 0
            if(firstWall >= walls.size()-1)
            {
                firstWall = 0;
            }
            else
            {
                //Si no, lo aumentamos
                firstWall++;
            }

            //Cada vez que movemos una pared aumentamos la score
            score++;
            score_string = to_wstring(score);

            //Cada X movimientos hacemos que todas las paredes se vuelven mas dificiles
            if(score % 30 == 0)
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
        //Si el smoke sale de la pantalla lo volvemos a posicionar en la posicion del player

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
        //Recorremos todas las parede y comprobamos si el player se encuentra dentro de las partes bot y top de las walls

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


    void Game_Scene::printOption(Option_Id option, Canvas & canvas)
    {
        //Printeamos la opcion

        canvas.fill_rectangle({options[option].x,options[option].y},
                               {options[option].slice->width, options[option].slice->height}
                ,options[option].slice);
    }


    int Game_Scene::option_at (const Point2f & point)
    {
        //Recorremos todas las opciones

        for (int index = 0; index < number_of_options; ++index)
        {
            const Option & option = options[index];

                //Si el point esta dentro de la opcion retorna el index

                if(point[0] > option.x - option.slice->width &&
                    point[0] < option.x + option.slice->width  &&
                    point[1] > option.y - option.slice->height &&
                    point[1] < option.y + option.slice->height )
                {
                    return index;
                }
        }

        return -1;
    }



}
























