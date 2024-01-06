#ifndef TIMER_H
#define TIMER_H

#include <bits/stdc++.h>
using namespace std;

class Timer {

public:

    Timer()
    {
        start_time = chrono::high_resolution_clock::now();
    };
    
    Timer(
        const string &name
    ) {
        timer_name = name;
        start_time = chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        end_time = chrono::high_resolution_clock::now();
        int ms_elapsed = chrono::duration_cast <chrono::milliseconds> ( end_time - start_time ).count();
        if( timer_name.empty() ) cout << "Time elapsed: " << ms_elapsed << "ms" << endl;
        else                     cout << timer_name << ": " << ms_elapsed << "ms" << endl;
    }

private:

    chrono::_V2::system_clock::time_point start_time;
    chrono::_V2::system_clock::time_point end_time;
    string timer_name = "";

};

#endif