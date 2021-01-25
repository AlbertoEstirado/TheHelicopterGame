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

        Font_Handle font;                                   ///< Fuentes
        std::wstring score_string;                          ///< String de la score

        enum State                                          ///< Estados de la escena
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

        std::unique_ptr<Player> player;                     ///< Player
        std::vector<Wall> walls;                            ///< Vector de paredes

        Texture_Handle texturePlayer;

        enum Option_Id                                      ///< Ids de las opciones
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
        };

        static const unsigned number_of_options = 6;         ///< Numero de opciones

        Option options[number_of_options];                  ///< Array de opciones

        std::unique_ptr<Atlas> atlas;                       ///< Atlas


        float          x, y;                                ///< Coordenadas del input del player

        int firstWall = 0;                                  ///< Numero de paredes que se han desplazado al final de la pantalla

        int score = 0;                                      ///< Score

        int scoreloaded;                                    ///< Score cargada



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

        /**
         *Cambia el estado de la partida a GAMEOVER
         * */
        void gameover();

        /**
         * Recibe un parametro que hace establece el estado de PAUSE (true) y RUNNING (false)
         * @param b
         */
        void pause(bool b);

        /**
         *Setea las posiciones de hud
         * */
        void configureUI();

        /**
         *Cargar score
         * */
        void loadScore();

        /**
         *Guardar score
         * */
        void saveScore();

        void draw_slice (basics::Canvas * canvas, const basics::Point2f & where, basics::Atlas & atlas, basics::Id slice_id);

        /**
         *Gestion de las parades de la partida
         * */
        void manageWalls();

        /**
         *Gestion de los smokes del player
         * */
        void manageSmokes();

        /**
         *Calcula las colisiones del player con las paredes
         * */
        void calculateWallsColision();

        /**
         *Funcion que renderiza una opcion en el canvas
         * @param canvas
         * */
        void printOption(Option_Id option, basics::Canvas & canvas);

        /**
         *Devuelve el id_option en el que se contengas las coordenadas recibidas
         * @param point
         * */
        int option_at (const Point2f & point);

    };

}


