#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <bits/stdc++.h>
using namespace std;

class Compression {

public:

    Compression() = default;

    static void inplace_compress( vector <int> & );

};

void Compression::inplace_compress( vector <int> &data )
{
    for( int &d : data ) d = d*d;
}

#endif