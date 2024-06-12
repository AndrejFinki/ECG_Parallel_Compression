#ifndef ECG_PROCESS_H
#define ECG_PROCESS_H

#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

#include "compression.hpp"
#include "data_handler.hpp"
#include "timer.hpp"

class ECG_Process {

public:

    ECG_Process() = default;
    ~ECG_Process() = default;

    virtual void main_process( const vector <int> *, vector <int> * ) = 0;
    virtual void secondary_process() = 0;

};



class ECG_Process_Fast : public ECG_Process {

public:

    ECG_Process_Fast( int _rank, int _size ) : rank( _rank ), size( _size ) {};

    void main_process( const vector <int> *, vector <int> * );
    void secondary_process();
    
    const int rank, size;
};

void ECG_Process_Fast::main_process(
    const vector <int> * data,
    vector <int> * sq_data
) {
    int data_per_process = data->size() / this->size;
    MPI_Bcast( &data_per_process, 1, MPI_INT, 0, MPI_COMM_WORLD );

    vector <int> pdata( data_per_process );
    MPI_Scatter( data->data(), data_per_process, MPI_INT, pdata.data(), data_per_process, MPI_INT, 0, MPI_COMM_WORLD );

    Compression::inplace_compress( pdata );

    sq_data->resize( data_per_process * this->size );
    MPI_Gather( pdata.data(), data_per_process, MPI_INT, sq_data->data(), data_per_process, MPI_INT, 0, MPI_COMM_WORLD );
}

void ECG_Process_Fast::secondary_process()
{
    int data_per_process;
    MPI_Bcast( &data_per_process, 1, MPI_INT, 0, MPI_COMM_WORLD );

    vector <int> pdata( data_per_process );
    MPI_Scatter( NULL, 0, MPI_INT, pdata.data(), data_per_process, MPI_INT, 0, MPI_COMM_WORLD );

    Compression::inplace_compress( pdata );
    
    MPI_Gather( pdata.data(), data_per_process, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD );
}

#endif