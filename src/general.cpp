#include "general.h"

#include <vector>
#include <iostream>

using namespace std;

void print_vector(const vector<int>& v)
{
    for(int i = 0; i< v.size(); i++)
        cout << v[i] << ' ';
    cout << endl;
}

//void insert_unique_value(vector<int>& input, int unique_value, vector<int>& output);
