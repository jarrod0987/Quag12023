#pragma once
#include<set>
#include<vector>
#include<algorithm>

vector<string> rotations(26);

void initilaize_rotations_table()
{
    for (auto& itr : rotations) { itr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; }
    for (int i{}; i < 26; i++) { rotate(rotations[i].begin(), rotations[i].begin()+i, rotations[i].end()); }
}

struct Data_Pack
{
	//Must give period and crib.
    int period{};
	string crib{};
	vector<int> positions{};

    vector<set<pair<char, char>>> equivilancies{};

	bool all_keys_set{false};
    vector<string> key_ring{};

	Data_Pack() = delete;
	Data_Pack(int period_in, string crib_in):period(period_in), crib(crib_in){};

    void crib_dragger(string& ct)
    {
        for (int i{}; i < ct.size() - crib.size() + 1; i++)//Loops though all positions.
        {
            //Breaks the CT and the PT word into appropriate columns.
            string ct_substr = ct.substr(i, crib.size());
            vector<string> ct_columns(period), pt_columns(period);
            int index{};
            for (int j{}; j < crib.size(); j++)
            {
                pt_columns[index] += crib[j];
                ct_columns[index] += ct_substr[j];
                index = (index + 1) % period;
            }

            //Checks column word patterns for specific placement to see if they match.
            bool can_place = true;
            for (int j{}; j < period; j++)
            {
                if (match_wp(pt_columns[j], ct_columns[j]) not_eq true) { can_place = false; }
            }

            if (can_place == true) { positions.push_back(i); }
        }
    }
    void make_equivs(string& ct, int position)
    {
        {
            vector<set<pair<char, char>>> temp(period);
            equivilancies = temp;
        }
                
        string ct_sub = ct.substr(position, crib.size());

        int index = position % period;
        for (int i{}; i < crib.size(); i++)
        {
            auto p = make_pair(ct_sub[i], crib[i]);
            equivilancies[index].insert(p);
            index = (index + 1) % period;
        }
    }
    void build_keys()
    {
        {//Initializing key ring.
            vector<string> temp(period);
            this->key_ring = temp;
        }
       
        //Set all CT alphabet shifts
        vector<bool> each_shift(period, false);
        vector<bool> old_state(period, false);

        //Initializeing key ring.
        //for (int i{}; i < period; i++) { key_ring.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZ"); }
        
        //Aligns alphabets that have an PT 'A' equivilancy.
        for (int i{}; i < period; i++)
        {
            for (auto& eq : equivilancies[i])
            {
                if (eq.second == 'A')
                {
                    int num = eq.first - 65;
                    key_ring[i] = rotations[num];
                    each_shift[i] = true;
                }
            }
        }

        while (each_shift != old_state)//Tries to set the other alphabets.
        {
            old_state = each_shift;
            vector<int> shifted{}, not_shifted{};

            //Tries to align an alpahabet to a already shifted one.
            for (int i{}; i < period; i++)
            {
                if (each_shift[i] == true) { shifted.push_back(i); }
                else { not_shifted.push_back(i); }
            }
            bool found_one = false;//Breaks the 4 nested loops if a match is found.
            for (auto& i : not_shifted)
            {
                for (auto& eq_not_shifted : equivilancies[i])
                {
                    for (auto& j : shifted)
                    {
                        for (auto& eq_shifted : equivilancies[j])
                        {
                            //If an equivilancy PT letter in an unshifted alphabet matches one
                            //from a shifted alphabet equivilancy, then the unshifted CT
                            //equivilancy letter is aligned with the shifted CT equivilancy letter.

                            if (eq_not_shifted.second == eq_shifted.second and found_one == false)
                            {
                                int offset_shifted = key_ring[j].find(eq_shifted.first);
                                int offset_unshifted = eq_not_shifted.first-65;

                                int a{};
                                if (offset_shifted <= offset_unshifted) { a = offset_unshifted - offset_shifted; }
                                else { a = 26 - (offset_shifted - offset_unshifted); }
                                key_ring[i] = rotations[a];
                                each_shift[i] = true;
                                found_one = true;
                            }
                        }
                    }
                }
            }
            
            //Checks to see if all CT alphabets have been shifted yet.
            if (find(each_shift.begin(), each_shift.end(), false) == each_shift.end())
            { 
                all_keys_set = true;
            }
        }
    }
    string convert_mono(string& ct)
    {
        if (all_keys_set == true)
        {
            string mono{};
            int index{}, pos{};
            for (auto& c : ct)
            {
                pos = key_ring[index].find(c);
                mono += pos + 65;
                index = (index + 1) % period;
            }
            return mono;
        }
        else { cout << "Cannot convert. All keys not set.\n"; }
                
                    
    }
    bool can_coexist(string ct, int pos_a, Data_Pack b, int pos_b)
    {
        this->make_equivs(ct, pos_a);
        b.make_equivs(ct, pos_b);

        for (int i{}; i<period; i++)
        {
            for (auto& eq_a : this->equivilancies[i])
            {
                for (auto& eq_b : b.equivilancies[i])
                {
                    if 
                    (
                        (eq_a.first == eq_b.first and eq_a.second != eq_b.second)
                        or
                        (eq_a.second == eq_b.second and eq_a.first != eq_b.first)
                        )
                    { return false; }
                }
            }
        }
        return true;
    }

    bool can_coexist3(string ct, int pos_a, Data_Pack b, int pos_b, Data_Pack c, int pos_c)
    {
        this->make_equivs(ct, pos_a);
        b.make_equivs(ct, pos_b);
        for (int i{}; i < period; i++)
        {
            for (auto& j : b.equivilancies[i]) { this->equivilancies[i].insert(j); }
        }
        c.make_equivs(ct, pos_c);
        for (int i{}; i < period; i++)
        {
            for (auto& j : c.equivilancies[i]) { this->equivilancies[i].insert(j); }
        }

        for (int i{}; i < period; i++)
        {
            for (auto& a:this->equivilancies[i])
            {
                for (auto& b:this->equivilancies[i])
                {
                    if (//CONVERT THIS TO 3 WAYS, MAYBE BULD INDEPENDANT EQUIVS TABLE
                       (a.first == b.first and a.second != b.second)
                        or
                       (a.second == b.second and a.first != b.first)
                       ) { return false; }
                }
            }
        }
        return true;
    }
};