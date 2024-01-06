#ifndef TIMER_H
#define TIMER_H

#include <bits/stdc++.h>
using namespace std;

class Timer {

public:
    
    Timer(
        const string &name,
        bool print_on_destroy_arg = true
    ) {
        timer_name = name;
        print_on_destroy = print_on_destroy_arg;
        start_time = chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        if( !print_on_destroy ) return;
        end_time = chrono::high_resolution_clock::now();
        int ms_elapsed = chrono::duration_cast <chrono::milliseconds> ( end_time - start_time ).count();
        if( timer_name.empty() ) cout << "Time elapsed: " << ms_elapsed << "ms" << endl;
        else                     cout << timer_name << ": " << ms_elapsed << "ms" << endl;
        cout << endl;
    }

    int check();

private:

    chrono::_V2::system_clock::time_point start_time;
    chrono::_V2::system_clock::time_point end_time;
    string timer_name = "";
    bool print_on_destroy = true;

};

int Timer::check()
{
    end_time = chrono::high_resolution_clock::now();
    int ms_elapsed = chrono::duration_cast <chrono::milliseconds> ( end_time - start_time ).count();
    return ms_elapsed;
}

#endif