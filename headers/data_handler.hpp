#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <bits/stdc++.h>
using namespace std;

class Data_Handler {

public:

    Data_Handler( const string & );

    const vector <int> * read();
    void write( const vector <int> * );
    vector <int> * data();
    static vector <string> get_files_in_dir( const string & );

private:

    string file_path;
    vector <int> ecg_data;

};

Data_Handler::Data_Handler(
    const string &file_path_arg
) {
    file_path = file_path_arg;
}

const vector <int> * Data_Handler::read()
{
    fstream file;
    file.open( file_path, fstream::in );

    assert( file && "Read data file check failed!" );

    ecg_data.clear();

    int value;
    while( file >> value )
        ecg_data.push_back( value );

    file.close();

    return & ecg_data;
}

void Data_Handler::write(
    const vector <int> * data
) {
    fstream file( file_path, fstream::out );

    assert( file && "Output file write failed!" );

    for( int i = 0 ; i < data->size() ; i++ )
        file << data->at(i) << '\n';

    file.close();
}

vector <int> * Data_Handler::data()
{
    return & ecg_data;
}

vector <string> Data_Handler::get_files_in_dir(
    const string & dir
) {
    const string temp_file_name = "files.tmp";
    string command = "ls " + dir + " > " + temp_file_name;
    system( command.c_str() );

    fstream file;
    file.open( temp_file_name, fstream::in );

    assert( file && "Get files in directory check failed!" );

    vector <string> files;
    string current_file;
    while( file >> current_file )
        files.push_back( current_file );

    return files;
}

#endif