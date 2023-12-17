#pragma once
#include <vector>
#include <string>

using namespace std;

auto first_column_analysis = [](string& s, int period)
    {
        vector<string> columns(period);
        vector<string> suspected_letters{};
        int index{};
        for (auto& c : s)
        {
            columns[index] += c;
            index = (index += 1) % period;
        }

        for (auto& column : columns)
        {
            vector<pair<int, char>> freqs{};
            bool high_freq_letters_present{ false };
            string selected_letters{};

            for (auto& c : "ABCDEFGHIJKLMNOPQRSTUVWXYZ")
            {
                int n = count(column.begin(), column.end(), c);
                freqs.emplace_back(make_pair(n, c));
                if (n > 1) { high_freq_letters_present = true; }
            }

            //Selects appropriate letters for each column based on feq when possible.
            /*if (high_freq_letters_present)
            {
                for (auto& x : freqs)
                {
                    if (x.first > 2) { selected_letters += (x.second); }
                }
            }
            else
            {
                for (auto& x : freqs)
                {
                    if (x.first > 0) { selected_letters += (x.second); }
                }
            }*/
            //Above disabled because it was missing some of the A's.
            for (auto& x : freqs)
            {
                if (x.first > 0) { selected_letters += (x.second); }
            }

            suspected_letters.emplace_back(selected_letters);
        }

        return suspected_letters;
    };