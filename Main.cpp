//All working but some things still set up for test mode.
//295 minutes multi cpu. GPU would be worth it.
//most common 4 word sequences used. 5 is too short, 3 doesn't get a hit with longer periods.
//3 word sequences would be too much computation to be practial. Without GPU anyways.
//4 random words from list is way way to long but would work on super computers.
#include<execution> 
#include<mutex>

#include "functions.h"
#include "crib_placer.h"
#include "Timer.h"

using namespace std;

mutex m;

int main()
{
    initilaize_rotations_table();
    qt = make_shared<quad_table>();
    string pt = "A number of years ago in a room in Philadelphia, a group of men signed their names"
        "to a Declaration. For that, they could have been hanged. For that Declaration was"
        "revolutionary. It said that all men were created equal and were possessed of certain"
        "inalienable rights.";

    string alpha_word = "PRESIDENT";
    string vert_word = "KENNEDY";
        
    pt = preprocess(pt);
    string ct = encrypt(pt, alpha_word, vert_word);
    
    //Determine likely period.
    //auto period = get_best_period(ct);TURNED OFF FOR TESTING
    int period{7};
    
    //off for testing
    /*cout << "Do you have a crib? (y/n)(Optional)\n";

    char choice{};
    cin >> choice; cin.ignore();*/

    //vector<string> complete{};
    //vector<Data_Pack> incomplete{};

    char choice{ 'n' };//testing only
    if (choice == 'y')
    {
        string crib{};
        //cout << "Enter Crib: ";//off for testing
        
        //getline(cin, crib);//off for testing
        cout << "Working, Please Wait...\n\n";
        //crib = preprocess(crib);
        crib = "CREATEDEQUALANDWEREPOSSESSED";//Testing only
        //crib = "CREATEDEQUALANDWERE";//Testing only

        auto out = crib_placer(ct, period, crib);
        cout << out << endl;
    }
    else
    {
        //string crib = "CREATEDEQUALANDWEREPOSSESSED";//Testing only
                
        Timer mt; mt.start();
        auto possible_pt = read_list_from_file("common 4 words.txt", period);
        //possible_pt.push_back(crib);
        //vector<string >possible_pt{};
        //possible_pt.emplace_back(crib);
        double best_score{-1000000000};

        auto lambda = [&](string& p)
            {
                string s = crib_placer(ct, period, p);
                if (s.size() > 0)
                {
                    double current_score = qt->score_string(s);
                    if (current_score > best_score)
                    {
                        lock_guard<mutex> lg(m);
                        if (current_score > best_score)
                        {
                            best_score = current_score;
                            cout << s << "\n\n";
                        }
                    }
                }
            };
        for_each(execution::par, possible_pt.begin(), possible_pt.end(), lambda);
        mt.stop('M');
    }
    
    
    
    
    

}

