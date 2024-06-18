#ifndef ECG_PROCESS_H
#define ECG_PROCESS_H

#include <bits/stdc++.h>
using namespace std;

class ECG_Process {

public:

    ECG_Process() = default;
    ~ECG_Process() = default;

    virtual void main_process( const vector <int> *, vector <int> * ) = 0;
    virtual void secondary_process() = 0;

};

#endif