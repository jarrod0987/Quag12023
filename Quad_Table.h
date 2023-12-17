#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include "FileIO.h"

struct quad_table
{ 
	double table[26][26][26][26]{};

    quad_table() { load_data(); }

    void make_neg_scores(string& corpus)
    {
        cout << "Building Scoring Table From Corpus..." << endl;
        int quad_count{ 0 };

        for (size_t i = 0; i < corpus.length() - 4; i++)
        {
            this->table[corpus[i] - 65][corpus[i + 1] - 65][corpus[i + 2] - 65][corpus[i + 3] - 65]++;
            quad_count++;
        }

        for (int w = 0; w < 26; w++)
        {
            for (int x = 0; x < 26; x++)
            {
                for (int y = 0; y < 26; y++)
                {
                    for (int z = 0; z < 26; z++)
                    {
                        if (this->table[w][x][y][z] == 0) this->table[w][x][y][z] = -24.0;
                        else { this->table[w][x][y][z] = log((double)this->table[w][x][y][z] / (double)quad_count); }
                    }
                }
            }
        }
    }

	void load_data()
	{
        ifstream in;
        in.open("Quadragram_Scoring.bin", ios::in | ios::binary);
        if (in) //Reading scoring data from object.
        {
            if (in.read((char*)this, sizeof(quad_table))) { cout << "Scoring Object Loaded...\n"; }
        }
        else //Building scoring data from Corpus.
        {
            string corpus = read_file("Corpus.txt");
            if (corpus == "File Not Found!") 
            { 
                cout << "Neither Quadragram_Scoring.bin or Corpus.txt found." << endl;
                return;
            }
            else
            {
                this->make_neg_scores(corpus);
            }

            ofstream out;
            out.open("Quadragram_Scoring.bin", ios::out | ios::binary);
            out.write((char*)this, sizeof(quad_table));
            out.close();
        }
        in.close();
	}

    double score_string(string s)
    {
        double score{ -0.0 };
        auto lambda = [&](char& a) 
            {
                auto& b = *(&a + 1);
                auto& c = *(&a + 2);
                auto& d = *(&a + 3);

                score += table[a-65][b-65][c-65][d-65];
            };
        for_each(s.begin(), s.end()-3, lambda);
        return score;
    }

    double score_char_array(char ca[], int length)
    {
        double score{ -0.0 };
        for (int i{}; i < length - 3; i++)
        {
            score += this->table
                [static_cast<unsigned _int64>(ca[i]) - 65]
            [static_cast<unsigned _int64>(ca[i + 1]) - 65]
            [static_cast<unsigned _int64>(ca[i + 2]) - 65]
            [static_cast<unsigned _int64>(ca[i + 3]) - 65];
        }
        return score;
    }

    string score_vector(vector<string>& vec)
    {
        double best_score = -1000000000;
        string best_string{};

        for (auto& s : vec)
        {
            double d = score_string(s);
            if(d > best_score)
            {
                best_score = d;
                best_string = s;
            }
        }
        return best_string;
    }
};

