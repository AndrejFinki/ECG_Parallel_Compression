#ifndef PROGRAM_STATE_H
#define PROGRAM_STATE_H

#include <bits/stdc++.h>
using namespace std;

class ProgramState { 
    
public:

    ProgramState( int argc, char ** argv ) 
    {
        assert( argc == 2 && "Missing file directory" );
        check_file_dir( file_directory + argv[1] );
    }

    const vector <int> * get_ecg_data();
    void read_file();

private:

    string file_directory = "../IV_PIDP/Proekt/data/";
    string file_path = "";
    vector <int> file_ecg_data;

    void check_file_dir( const string & );

};

const vector <int> * ProgramState::get_ecg_data()
{
    if( file_ecg_data.empty() ) return nullptr;
    const vector <int> * file_ecg_data_cptr = &file_ecg_data;
    return file_ecg_data_cptr;
}

void ProgramState::read_file()
{
    fstream file;
    file.open( file_path, fstream::in );

    assert( file && "File not found!" );
    
    int value;
    while( file >> value ) file_ecg_data.push_back( value );

    file.close();
}

void ProgramState::check_file_dir( const string &filepath )
{
    fstream file;
    file.open( filepath, fstream::in );

    assert( file && "File not found!" );

    file_path = filepath;

    file.close();
}

#endif