#include "test.h"

using namespace std;

bool is_unique(const vector<int>& v)
{
    for(int i = 1; i<v.size();i++)
    {
        if(v[i-1]==v[i])
            return false;
    }
    return true;
}