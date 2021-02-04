/*
 * SCORE MANAGER
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
#include "istream"
#include <fstream>



namespace helicopter
{

    class Score_Manager
    {

    public:

        static Score_Manager & get_instance ()
        {
            static Score_Manager score_manager;
            return score_manager;
        }



    private:


        Score_Manager() = default;

        ~Score_Manager() = default;

        Score_Manager(const Score_Manager & ) = delete;

    public:

        int load_score ();
        bool save_score (int newScore);


    };

}


