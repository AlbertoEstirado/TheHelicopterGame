/*
 * GAME SCENE
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
#include <vector>
#include <basics/Raster_Font>
#include "Player.hpp"
#include "Wall.hpp"
#include "Menu_Scene.hpp"
#include <basics/Atlas>
#include <fstream>

namespace helicopter
{
    using basics::Atlas;

    class Game_Scene : public basics::Scene
    {

        typedef std::shared_ptr< basics::Texture_2D > Texture_Handle;
        typedef std::unique_ptr< basics::Raster_Font > Font_Handle;

    public:

        Font_Handle font;  //fuentes
        std::wstring score_string;  //string de la score

        enum State  //estados de la escena
        {
            LOADING,
            RUNNING,
            WAITTING,
            GAMEOVER,
            PAUSE
        };

        State          state;
        bool           suspended;

        bool touching;

        unsigned       canvas_width;
        unsigned       canvas_height;

        std::unique_ptr<Player> player;  //player
        std::vector<Wall> walls;   //vector de paredes

        Texture_Handle texturePlayer;
        std::shared_ptr<Texture_2D> gameover_texture;

        enum Option_Id  //ids de las opciones
        {
            TRY_AGAIN,
            MENU,
            PAUSETEXT,
            PAUSEICON,
            RESUMEICON,
            GAMEOVERHUD,
        };

        struct Option
        {
            const Atlas::Slice * slice;
            int x;
            int y;
            float is_pressed;
        };

        static const unsigned number_of_options = 6; //numero de opciones

        Option options[number_of_options];  //array de opciones

        std::unique_ptr<Atlas> atlas;  //atlas


        float          x, y;  //cordenadas del input del player

        int firstWall = 0; //numero de paredes que se han desplazado al final de la pantalla

        int score = 0; //score

        int scoreloaded; //score cargada



    public:

        Game_Scene();

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }

        bool initialize () override;
        void suspend    () override;
        void resume     () override;

        void handle     (basics::Event & event) override;
        void update     (float time) override;
        void render     (basics::Graphics_Context::Accessor & context) override;

    private:

        void load ();
        void start();
        void run  (float time);
        void gameover();  //cambia el estado de la partida a GAMEOVER

        void pause(bool b);

        void configureUI();  //setea las posiciones de hud

        void loadScore();  //cargar score
        void saveScore();  //guardar score

        void draw_slice (basics::Canvas * canvas, const basics::Point2f & where, basics::Atlas & atlas, basics::Id slice_id);

        void manageWalls();  //gestion de las parades de la partida
        void manageSmokes();  //gestion de los smokes del player
        void calculateWallsColision();  //calcula las colisiones del player con las paredes

        void printOption(Option_Id option, basics::Canvas & canvas); //funcion que renderiza una opcion en el canvas
        int option_at (const Point2f & point);  //devuelve el id_option en el que se contengas las coordenadas recibidas

    };

}


