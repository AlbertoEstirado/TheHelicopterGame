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

        Font_Handle font;
        std::wstring score_string;

        enum State
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

        std::unique_ptr<Player> player;
        std::vector<Wall> walls;

        Texture_Handle texturePlayer;
        std::shared_ptr<Texture_2D> gameover_texture;

        enum Option_Id
        {
            //TRY_AGAIN,
            MENU,
            //PAUSETEXT,
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

        static const unsigned number_of_options = 3;

        Option options[number_of_options];

        std::unique_ptr<Atlas> atlas;


        float          x, y;

        int firstWall = 0;
        float playerXCoordenate = 0;
        int currentWall = 0;
        int lastPath = 0 ;
        int pathCreated = 0;
        int score = 0;
        int scoreloaded;

        std::vector<Path> paths;


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
        void gameover();
        void gameoverScreen();

        void pause(bool b);

        void configureUI();

        void loadScore();
        void saveScore();

        void draw_slice (basics::Canvas * canvas, const basics::Point2f & where, basics::Atlas & atlas, basics::Id slice_id);

        void manageWalls();
        void manageSmokes();
        void calculateWallsColision();
        void generatePath(Path path);
        void setUpPath();
        //Path generateNextPath(int lasty);

        int option_at (const Point2f & point);

    };

}


