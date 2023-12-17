#pragma once
#include <numeric>

#include "word_pattern.h"
#include "mono_hill_climb.h"

using namespace std;

auto get_ioc = [](string s)->float
    {
        //Gets the count of each letter.
        vector<float> letter_counts(26);
        for (auto& c : s) { letter_counts[c - 65]++; }

        //Lambda for calculating ioc along with reduce.
        auto ioc_lambda = [&](float a, float b)
            { return a + (b * (b - 1) / (0.0385 * s.size() * (s.size() - 1))); };

        float output = reduce(letter_counts.begin(), letter_counts.end(), 0.0, ioc_lambda);
        return output;
    };

auto get_best_period = [](string& s)
    {
        vector<pair<float, int>> results{};

        for (int i{ 4 }; i < 15; i++)//Loops to find average ioc for each period 4-15;
        {
            vector<string> columns(i);
            vector<float> scores(i);

            int index{};
            for (auto& c : s) //Populates columns.
            {
                columns[index] += c;
                index = (index + 1) % i;
            }

            //Gets ioc for each column.
            transform(columns.begin(), columns.end(), scores.begin(), get_ioc);

            //Lambda/Accumulate to get average of each columns ioc.
            auto get_av = [&](float a, float b) {return a + b / scores.size(); };
            float average_ioc = accumulate(scores.begin(), scores.end(), 0.0, get_av);

            results.emplace_back(make_pair(average_ioc, i));
        }

        //Sort and display results to analyst.
        sort(results.rbegin(), results.rend());

        for (auto& p : results) { cout << "IoC = " << p.first << "\t Period = " << p.second << endl; }

        //Get period selection from analyst.
        int choice{};

        while (choice < 4 or choice > 14)
        {
            cout << "\nEnter the period you wish to try: (Must be 4-14)\n";
            cin >> choice;
        }

        return choice;
    };

//Lambda for prepairing a string to be encrypted/decrypted. 
auto preprocess = [](string s)->string
    {
        string output{};
        for (auto& c : s)
        {
            if (c > 96 && c < 123) { c -= 32; }
            if (c > 64 && c < 91) { output += c; }
        }
        return output;
    };

//Makes keyed alphabets from a keyword.
string make_keyed_alpha(string& alpha_word)
{
    string output{};

    for (auto& c : alpha_word)
    {
        if (output.find(c) == string::npos) { output += c; }
    }

    for (auto& c : "ABCDEFGHIJKLMNOPQRSTUVWXYZ")
    {
        if (output.find(c) == string::npos) { output += c; }
    }
    return output;
}

string encrypt(string& plain_text, string& alpha_word, string& vert_word)
{
    string output(plain_text.size(), '*');
    string keyed_alpha = make_keyed_alpha(alpha_word);//Makes keyed alphabet from alphabet keyword.

    //Sets up keys for each period based on a vertical keyword.
    vector<string> key_ring(vert_word.size(), "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    for (int i{}; i < vert_word.size(); i++)
    {
        int x = vert_word[i] - 65;
        rotate(key_ring[i].begin(), key_ring[i].begin() + x, key_ring[i].end());
    }

    int index{};
    //Lambda function for encrypting each letter with the proper period key.  
    auto encrypt_letter = [&](char c)->char
        {
            int x = (keyed_alpha.find(c));
            char final = key_ring[index][x];
            index = (index += 1) % vert_word.size();
            return final;
        };

    //Encrypts the plaintext.
    transform(plain_text.begin(), plain_text.end(), output.begin(), encrypt_letter);

    return output;
}

string decrypt(string& cipher_text, string& alpha_word, string& vert_word)
{
    string output(cipher_text.size(), '*');
    string keyed_alpha = make_keyed_alpha(alpha_word);//Makes keyed alphabet from alphabet keyword.

    //Sets up keys for each period based on a vertical keyword.
    vector<string> key_ring(vert_word.size(), "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    for (int i{}; i < vert_word.size(); i++)
    {
        int x = vert_word[i] - 65;
        rotate(key_ring[i].begin(), key_ring[i].begin() + x, key_ring[i].end());
    }

    int index{};

    //Lambda function for encrypting each letter with the proper period key.
    auto decrypt_letter = [&](char c)->char
        {
            int x = (key_ring[index].find(c));
            char out = keyed_alpha[x];
            index = (index += 1) % vert_word.size();
            return out;
        };

    //Decrypts the ciphertext.
    transform(cipher_text.begin(), cipher_text.end(), output.begin(), decrypt_letter);
    return output;
}