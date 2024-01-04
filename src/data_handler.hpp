#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <bits/stdc++.h>
using namespace std;

/*  TODO: Make DataHander easier to use and more flexible.
        *) Allow for specifying whether we want to use a data file or an output file
        *) Allow for reading/writing for either option
        *) Allow for specifying both the data and the output directory manually
        *) Make sure it is easy to use for the verifier, that will compare the decompressed output file with each original ecg file
*/

class DataHandler {

public:

    DataHandler( int argc, char ** argv )
    {
        assert( argc >= 2 && "DataHandler constructor failed! Missing data file path." );
        data_file_name = argv[1];
        data_path = data_directory + data_file_name;
        check_file_path( data_path );
    }

    const vector <int> * get_ecg_data();
    void read_data();
    void write_output( const vector <int> & );

private:

    const string data_directory = "../IV_PIDP/Proekt/data/";
    const string output_directory = "../IV_PIDP/Proekt/output/";
    string data_path = "";
    string data_file_name = "";
    vector <int> ecg_data;

    void check_file_path( const string & );

};

const vector <int> * DataHandler::get_ecg_data()
{
    if( ecg_data.empty() ) return nullptr;
    const vector <int> * ecg_data_cptr = &ecg_data;
    return ecg_data_cptr;
}

void DataHandler::read_data()
{
    fstream file;
    file.open( data_path, fstream::in );

    assert( file && "Read data file check failed!" );

    ecg_data.clear();

    int value;
    while( file >> value ) ecg_data.push_back( value );

    file.close();
}

void DataHandler::check_file_path(
    const string &path
) {
    fstream file;
    file.open( path, fstream::in );

    assert( file && "File path check failed!" );

    file.close();
}

void DataHandler::write_output(
    const vector <int> &data
) {
    fstream file( output_directory + data_file_name + ".cmp", fstream::out );
    for( const int &i : data ) file << i << endl;
    file.close();
}

#endif