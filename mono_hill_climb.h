#pragma once
#include <iostream>
#include <ctime>
#include <random>

//#include "decrypt_mono.h"
#include "Quad_Table.h"

using namespace std;
shared_ptr<quad_table> qt;

string decrypt_mono(string& ct, string key)
{
	string out = ct;

	for (int i{}; i < ct.size(); i++)
	{
		int offset = key.find(ct[i]);
		out[i] = static_cast<int>(offset + 65);
	}

	return out;
}

string hill_climb(string& ct)
{
	//Setting up RNG.
	default_random_engine r(time(NULL));

	double global_best{ -1000000.0 };
	int outer_count{};
	string output{};
	string old_key = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string best_key{};
	
	while (outer_count < 10)
	{
		shuffle(old_key.begin(), old_key.end(), r);

		int inner_count = 0;
		double local_best = -100000.0;

		while (inner_count < 5000)
		{
			string new_key = old_key;
			inner_count++;

			int a = r() % 26;
			int b = r() % 26;

			while (b == a) { b = r() % 26; }
			swap(new_key[a], new_key[b]);

			//Decrypt and score message with new key.
			string de = decrypt_mono(ct, new_key);
			
			double current_score = qt->score_string(de);
			
			//Update scoring data.

			if (current_score > local_best)
			{
				local_best = current_score;
				old_key = new_key;

				if (current_score > global_best)
				{
					global_best = current_score;
					output = de;
					inner_count = 0;
				}
			}
		}
		outer_count++;
	}
	
	return output;
}