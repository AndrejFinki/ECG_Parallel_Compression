#ifndef TIMER_H
#define TIMER_H

#include <bits/stdc++.h>
using namespace std;

class Timer {

public:
    
    Timer(
        const string & name,
        bool print_on_destroy_arg = false
    ) {
        timer_name = name;
        print_on_destroy = print_on_destroy_arg;
        start_time = chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        if( !print_on_destroy ) return;
        this->display();
    }

    int stop();
    int check();
    void display();
    string get_name();

private:

    chrono::_V2::system_clock::time_point start_time;
    chrono::_V2::system_clock::time_point end_time;
    string timer_name = "";
    bool print_on_destroy = false;
    bool stop_was_called = false;

};

int Timer::stop()
{
    stop_was_called = true;
    end_time = chrono::high_resolution_clock::now();
    return this->check();
}

int Timer::check()
{
    if( !stop_was_called ) this->stop();
    int ms_elapsed = chrono::duration_cast <chrono::milliseconds> ( end_time - start_time ).count();
    return ms_elapsed;
}

void Timer::display()
{
    int ms_elapsed = this->check();
    if( timer_name.empty() ) cout << "Time elapsed: " << ms_elapsed << "ms" << endl;
    else                     cout << timer_name << ": " << ms_elapsed << "ms" << endl;
}

string Timer::get_name()
{
    return this->timer_name;
}

#endif