#include "cli.h"

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

//Get graph file, make sure exists and unique
int check_for_graph_file(const vector<string>& cliArguments, string& filename_graph)
{
    int number_of_graph_files = 0;
    for(int i = 0; i<cliArguments.size(); i++)
    {
        if (cliArguments[i].size() >= 6 && cliArguments[i].substr(cliArguments[i].size() - 6) == ".graph")
        {
            number_of_graph_files++;
            filename_graph = cliArguments[i];
        }
    }
    if(number_of_graph_files != 1)
    {
        cout << "Arguments must contain a single .graph file" << endl;
        return 1;
    }
    else
        return 0;
}

int check_for_seed(const vector<string>& cliArguments, int& seed)
{
    int number_of_seeds = 0;
    for(size_t i = 0; i<cliArguments.size();i++)
    {
        if(!cliArguments[i].empty() && std::all_of(cliArguments[i].begin(), cliArguments[i].end(), ::isdigit))
        {
            number_of_seeds++;
            seed = stoi(cliArguments[i]);
        }
    }
    if(number_of_seeds != 1 || !(seed>0))
    {
        cout << "Arguments must contain exactly one seed greater than 0" << endl;
        return 1;
    }
    else
        return 0;
}

int check_for_options(const vector<string>& cliArguments, bool& print_td)
{
    if(cliArguments.size()>=2 && cliArguments.size()<4)
    {
        int number_of_td_options = 0;
        for(int i = 0; i<cliArguments.size(); i++)
        {
            if(cliArguments[i].size() >= 11 && cliArguments[i].substr(0, 11) == "--print_td=")
            {
                number_of_td_options++;
                if(cliArguments[i] == "--print_td=true" || cliArguments[i] == "--print_td=false")
                {
                    if(cliArguments[i] == "--print_td=true")
                        print_td=true;
                    else
                        print_td=false;
                }
                else
                {
                    cout << "Optional argument --print_td is set incorrectly" << endl;
                    return 1;
                }
            }
        }
        if(number_of_td_options>1)
        {
            cout << "Can only set --track_solution once" << endl;
            return 1;
        }
        if(cliArguments.size()-2 != number_of_td_options)
        {
            cout << "Invalid arguments" << endl;
            return 1;
        }
    }
    else
    {
        cout << "Takes at most 3 arguments" << endl;
        return 1;
    }
    return 0;
}