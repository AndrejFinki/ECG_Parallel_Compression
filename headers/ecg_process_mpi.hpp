#ifndef ECG_PROCESS_MPI
#define ECG_PROCESS_MPI

#include <bits/stdc++.h>
using namespace std;

#include <mpi.h>

#include "compression.hpp"
#include "ecg_process.hpp"

class ECG_Process_MPI : public ECG_Process {

public:

    ECG_Process_MPI( int _rank, int _size ) : rank( _rank ), size( _size ) {};

    void main_process( const vector <int> *, vector <int> * );
    void secondary_process();
    
    const int rank, size;
};

void ECG_Process_MPI::main_process(
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

void ECG_Process_MPI::secondary_process()
{
    int data_per_process;
    MPI_Bcast( &data_per_process, 1, MPI_INT, 0, MPI_COMM_WORLD );

    vector <int> pdata( data_per_process );
    MPI_Scatter( NULL, 0, MPI_INT, pdata.data(), data_per_process, MPI_INT, 0, MPI_COMM_WORLD );

    Compression::inplace_compress( pdata );
    
    MPI_Gather( pdata.data(), data_per_process, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD );
}

#endif