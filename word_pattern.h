#pragma once
#include <map>
#include <string>

using namespace std;

string word_pattern(string s)
{
    string out;
    map<char, char> mp{};
    char index{ 'a' };

    for (auto c : s)
    {
        if (mp.find(c) not_eq mp.end())//Found
        {
            out += mp[c];
        }
        else//Not found
        {
            mp[c] = index;
            out += index;
            index++;
        }
    }
    return out;
}

bool match_wp(string& a, string& b)
{
    string wp_a = word_pattern(a);
    string wp_b = word_pattern(b);

    if (wp_a == wp_b)
    { 
        return true;
    }
    else 
    { 
        return false;
    }
}