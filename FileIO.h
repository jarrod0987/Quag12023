#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> read_list_from_file(string file_name, int min_size)
{
    vector<string> empty_list{};
    ifstream in_file;
    in_file.open(file_name);

    if (!in_file) { printf("File Not Found!"); }

    string current_line{};
    while (getline(in_file, current_line)) 
    { 
        if (current_line.size() >= min_size) { empty_list.push_back(current_line); }
    }

    in_file.close();
    return empty_list;
}

string read_file(string file_name)
{
    ifstream in_file;
    in_file.open(file_name);
    
    if (!in_file)
    {
        printf("File Not Found!");
        return "File Not Found!";
    }

    string s{}, current_line{};

    while (getline(in_file, current_line)) { s += current_line; }
                    
    in_file.close();
    return s;
}

void write_file(string s, string file_name)
{
    ofstream out_file;
    out_file.open(file_name);

    if (!out_file) { printf("File Not Found!"); }
    else
    {
        out_file << s;
        out_file.close();
    }
}

void write_double_array(string file_name, double my_array[], int byte_size)
{
    ofstream out(file_name, ios::out | ios::binary);
    out.write((char*)my_array, byte_size);
    out.close();
}

void read_double_array(string file_name, double my_array[], int byte_size)
{
    ifstream in(file_name, ios::in | ios::binary);
    in.read((char*)my_array, byte_size);
    in.close();
}