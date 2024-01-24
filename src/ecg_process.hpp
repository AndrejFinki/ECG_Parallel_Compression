#ifndef ECG_PROCESS_H
#define ECG_PROCESS_H

#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

#include "compression.hpp"
#include "data_handler.hpp"

class ECG_Process {
public:
    ECG_Process() = default;
    ~ECG_Process(){ delete input; delete output; }
    virtual void main_process() = 0;
    virtual void secondary_process() = 0;
    void set_input( const string & );
    void set_output( const string & );
    const vector <int> * get_input();
    void write_output( vector <int> * );
protected:
    Data_Handler *input = nullptr;
    Data_Handler *output = nullptr;
};

void ECG_Process::set_input(
    const string &input_path
) {
    input = new Data_Handler( input_path );
}

void ECG_Process::set_output(
    const string &output_path
) {
    output = new Data_Handler( output_path );
}

const vector <int> * ECG_Process::get_input()
{
    return input->read();
}

void ECG_Process::write_output(
    vector <int> * compressed_data
) {
    output->write( compressed_data );
}

class ECG_Process_Method_1 : public ECG_Process {
public:
    ECG_Process_Method_1( int _rank, int _size ) : rank( _rank ), size( _size ) {};
    void main_process();
    void secondary_process();
    const int rank, size;
};

void ECG_Process_Method_1::main_process()
{
    const vector <int> * data = this->get_input();

    int data_per_process = data->size() / this->size;
    MPI_Bcast( &data_per_process, 1, MPI_INT, 0, MPI_COMM_WORLD );

    vector <int> pdata( data_per_process );
    MPI_Scatter( data->data(), data_per_process, MPI_INT, pdata.data(), data_per_process, MPI_INT, 0, MPI_COMM_WORLD );

    Compression::inplace_compress( pdata );

    vector <int> sq_data( data_per_process * this->size );
    MPI_Gather( pdata.data(), data_per_process, MPI_INT, sq_data.data(), data_per_process, MPI_INT, 0, MPI_COMM_WORLD );

    if( data_per_process * this->size != data->size() ) {
        vector <int> extra_data;
        for( int i = data_per_process * this->size ; i < data->size() ; i++ ) {
            extra_data.push_back( data->at(i) );
        }
        Compression::inplace_compress( extra_data );
        for( int &i : extra_data ) {
            sq_data.push_back( i );
        }
    }

    this->write_output( &sq_data );
}

void ECG_Process_Method_1::secondary_process()
{
    int data_per_process;
    MPI_Bcast( &data_per_process, 1, MPI_INT, 0, MPI_COMM_WORLD );

    vector <int> pdata( data_per_process );
    MPI_Scatter( NULL, 0, MPI_INT, pdata.data(), data_per_process, MPI_INT, 0, MPI_COMM_WORLD );

    Compression::inplace_compress( pdata );
    
    MPI_Gather( pdata.data(), data_per_process, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD );
}

#endif