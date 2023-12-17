#pragma once
#include <iostream>
#include <string>
#include "Data_Pack.h"
#include "mono_hill_climb.h"

using namespace std;

string crib_placer(string ct, int period, string crib)
{
    Data_Pack crib_pack(period, crib);
    crib_pack.crib_dragger(ct);
    vector<string> outputs;
    
    if (crib_pack.positions.size() > 0)
    {
        for (auto& itr : crib_pack.positions)
        {
            Data_Pack temp(period, crib);
            temp.make_equivs(ct, itr);
            temp.build_keys();
            if (temp.all_keys_set == true)
            {
                string mono = temp.convert_mono(ct);
                string de = hill_climb(mono);
                outputs.emplace_back(de);
            }
        }
    }

    if (outputs.size() > 0)
    {
        string best_string = qt->score_vector(outputs);
        return best_string;
    }
    else { return ""; }
}