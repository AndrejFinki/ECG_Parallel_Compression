#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <bits/stdc++.h>
using namespace std;

class Data_Handler {

public:

    Data_Handler(
        string file_path_arg
    ) {
        file_path = file_path_arg;
    }

    const vector <int> * read();
    void write( const vector <int> * );

private:

    string file_path;
    vector <int> ecg_data;

};

const vector <int> * Data_Handler::read()
{
    fstream file;
    file.open( file_path, fstream::in );

    assert( file && "Read data file check failed!" );

    ecg_data.clear();

    int value;
    while( file >> value ) ecg_data.push_back( value );

    file.close();

    return &ecg_data;
}

void Data_Handler::write(
    const vector <int> * data
) {
    fstream file( file_path, fstream::out );

    assert( file && "Output file write failed!" );

    for( int i = 0 ; i < data->size() ; i++ ) {
        file << data->at(i) << endl;
    }

    file.close();
}

#endif