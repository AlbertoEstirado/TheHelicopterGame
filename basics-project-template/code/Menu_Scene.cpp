/*
 * MENU SCENE
 * Copyright © 2020+ Alberto Estirado López
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * estiradoalberto@gmail.com
 */

#include "Menu_Scene.hpp"
#include "Game_Scene.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Transformation>
#include <basics/Application>
#include <basics/Log>

using namespace basics;
using namespace std;

namespace helicopter
{

    Menu_Scene::Menu_Scene()
    {
        state         = LOADING;
        suspended     = true;
        canvas_width  = 1280;
        canvas_height =  720;
    }

    // ---------------------------------------------------------------------------------------------

    bool Menu_Scene::initialize ()
    {
        for (auto & option : options)
        {
            option.is_pressed = false;
        }

        load();


        return true;
    }

    // ---------------------------------------------------------------------------------------------

    void Menu_Scene::handle (basics::Event & event)
    {
        if (state == READY)                     // Se descartan los eventos cuando la escena estÃ¡ LOADING
        {
            switch (event.id)
            {
                case ID(touch-started):         // El usuario toca la pantalla
                case ID(touch-moved):
                {
                    // Se determina quÃ© opciÃ³n se ha tocado:

                    Point2f touch_location = { *event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };
                    int     option_touched = option_at (touch_location);

                    // Solo se puede tocar una opciÃ³n a la vez (para evitar selecciones mÃºltiples),
                    // por lo que solo una se considera presionada (el resto se "sueltan"):

                    for (int index = 0; index < number_of_options; ++index)
                    {
                        options[index].is_pressed = index == option_touched;
                    }

                    break;
                }

                case ID(touch-ended):           // El usuario deja de tocar la pantalla
                {
                    // Se "sueltan" todas las opciones:

                    for (auto & option : options) option.is_pressed = false;

                    // Se determina quÃ© opciÃ³n se ha dejado de tocar la Ãºltima y se actÃºa como corresponda:

                    Point2f touch_location = { *event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };

                    if (option_at (touch_location) == PLAY)
                    {
                        director.run_scene (shared_ptr< Scene >(new helicopter::Game_Scene));
                    }

                    break;
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Menu_Scene::update (float time)
    {
        if (!suspended) if (state == LOADING)
            {
                Graphics_Context::Accessor context = director.lock_graphics_context ();

                if (context)
                {
                    font.reset (new Raster_Font("fonts/impact.fnt", context));

                    load();

                    // Se carga el atlas:

                    atlas.reset (new Atlas("atlas/helicopter_atlas.sprites", context));

                    // Si el atlas se ha podido cargar el estado es READY y, en otro caso, es ERROR:

                    state = atlas->good () ? READY : ERROR;

                    // Si el atlas estÃ¡ disponible, se inicializan los datos de las opciones del menÃº:

                    if (state == READY)
                    {
                        configure_options ();
                    }
                }
            }
    }

    // ---------------------------------------------------------------------------------------------

   void Menu_Scene::load()
   {
       if (!suspended)
       {
           loadScore();
           score_string = to_wstring(loadedscore);

       }
   }

    void Menu_Scene::render (Graphics_Context::Accessor & context)
    {
        if (!suspended)
        {
            // El canvas se puede haber creado previamente, en cuyo caso solo hay que pedirlo:

            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            // Si no se ha creado previamente, hay que crearlo una vez:

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            // Si el canvas se ha podido obtener o crear, se puede dibujar con Ã©l:

            if (canvas)
            {
                canvas->clear ();

                if (state == READY)
                {
                    // Se dibuja el slice de cada una de las opciones del menÃº:

                    for (auto & option : options)
                    {
                        canvas->set_transform
                                (
                                        scale_then_translate_2d
                                                (
                                                        option.is_pressed ? 1.f : 1.f,              // Escala de la opciÃ³n
                                                        { option.position[0], option.position[1] }      // TraslaciÃ³n
                                                )
                                );

                        canvas->fill_rectangle ({ 0.f, 0.f }, { option.slice->width, option.slice->height }, option.slice, CENTER | TOP);
                    }

                    // Se restablece la transformaciÃ³n aplicada a las opciones para que no afecte a
                    // dibujos posteriores realizados con el mismo canvas:

                    canvas->set_transform (Transformation2f());




                    //font.reset (new Raster_Font("fonts/impact.fnt", context));


                    Text_Layout textLayout(*font, score_string);
                    canvas->draw_text({canvas_width/2.2, canvas_height/3.6f}, textLayout);
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Menu_Scene::configure_options ()
    {
        // Se asigna un slice del atlas a cada opciÃ³n del menÃº segÃºn su ID:

        options[PLAY].slice = atlas->get_slice (ID(play));
        options[TITLE].slice = atlas->get_slice(ID(helicoptertitle));
        options[TOPSCORE].slice = atlas->get_slice(ID(topscore));


        options[TITLE].position[0] = canvas_width/2;
        options[TITLE].position[1] = canvas_height/1.2f;

        options[PLAY].position[0] = canvas_width/2;
        options[PLAY].position[1] = canvas_height/2;

        options[TOPSCORE].position[0] = canvas_width/2;
        options[TOPSCORE].position[1] = canvas_height/2.6f;


        initialize ();
    }

    // ---------------------------------------------------------------------------------------------

    int Menu_Scene::option_at (const Point2f & point)
    {
        for (int index = 0; index < number_of_options; ++index)
        {
            const Option & option = options[index];

            if
                    (
                    point[0] > option.position[0] - option.slice->width  &&
                    point[0] < option.position[0] + option.slice->width  &&
                    point[1] > option.position[1] - option.slice->height &&
                    point[1] < option.position[1] + option.slice->height
                    )
            {
                return index;
            }
        }

        return -1;
    }

   void Menu_Scene::loadScore()
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
              loadedscore = auxscoreloaed;
              basics::log.e ("*****************************************************************************************");
              basics::log.e (std::to_string(loadedscore));

          }
          else
          {
              basics::log.e ("ERROR at loading score in game_scene failed reading save.data.");
              basics::log.e ("------------------------------------------------------------------------------------------------");
          }

      }

   }

}