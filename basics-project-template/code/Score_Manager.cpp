/*
 * SCORE MANAGER
 * Copyright © 2020+ Alberto Estirado López
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * estiradoalberto@gmail.com
 */


#include "Score_Manager.hpp"
#include <basics/Log>
#include <basics/Application>
#include <fstream>

using namespace basics;
using namespace std;

namespace helicopter
{
/*
    bool Score_Manager::load_score()
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
                loadedScore = auxscoreloaed;

                basics::log.e (std::to_string(loadedScore));

            }
            else
            {
                basics::log.e ("ERROR at loading score in game_scene failed reading save.data.");
                basics::log.e ("------------------------------------------------------------------------------------------------");
            }
        }
    }

    bool Score_Manager::save_score(int newScore)
    {
        string path = application.get_internal_data_path () + "/save.data";

        basics::log.d (string("saving score and creating file at ") + path);

        ofstream writer(path, ofstream::binary | ofstream::trunc);

        if(writer)
        {
            writer.write ((char *)&newScore, sizeof(newScore));
        }
        if (writer.good ())
        {
            basics::log.d ("Score saved succesfully...");
        }
    }*/
}



























