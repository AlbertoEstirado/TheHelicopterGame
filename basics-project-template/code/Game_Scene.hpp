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
#include "Player.hpp"
#include "Wall.hpp"

namespace helicopter
{

    class Game_Scene : public basics::Scene
    {

        typedef std::shared_ptr< basics::Texture_2D > Texture_Handle;

    public:

        enum State
        {
            LOADING,
            RUNNING,
        };

        State          state;
        bool           suspended;

        bool touching;

        unsigned       canvas_width;
        unsigned       canvas_height;

        std::unique_ptr<Player> player;
        std::vector<Wall> walls;

        Wall testWall;

        Texture_Handle texturePlayer;

        float          x, y;

        int firstWall = 0;
        float playerXCoordenate = 0;
        int currentWall = 0;
        int lastPath = 0 ;
        int pathCreated = 0;


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

        void manageWalls();
        void calculateWallsColision();
        void generatePath(Path path);
        void setUpPath();
        //Path generateNextPath(int lasty);

    };

}


